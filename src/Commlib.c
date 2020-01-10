
#include "global.h"
//#include <posixlib.h>
#include "Commlib.h"

/********************** Internal macros declaration ************************/
#define   NET_ERR_RETRY         1000
#define   MODEM_PATHNAME        "/dev/ttyS1"
#define   MODEM_PATHNAME1       "/dev/modem0"
#define   LEN_WORKBUF			(1024*4)
#define   INVALID_SOCKET        0xFFFF
/********************** Internal structure declaration *********************/
typedef struct _tagERR_INFO
{
	INT32		iErrCode;
	uint8_t	szChnMsg[16+1];
	uint8_t	szEngMsg[16+1];
}ERR_INFO;

/********************** Internal functions declaration *********************/
static INT32 RS232Dial(uint8_t ucDialMode);
static INT32 RS232Txd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut);
//static INT32 RS232RawTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut);
static INT32 RS232NacTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut);
static INT32 RS232Rxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32 RS232RawRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32 RS232NacRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32 RS232OnHook(uint8_t bReleaseAll);

static INT32  TcpDial(uint8_t ucDialMode);
static INT32  TcpTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut);
static INT32  TcpRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32  TcpOnHook(uint8_t bReleaseAll);

static INT32  PSTNDial(uint8_t ucDialMode);
static INT32  PSTNTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut);
static INT32  PSTNRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32  PSTNSyncRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32  PSTNAsyncRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32  PSTNOnHook(uint8_t bReleaseAll);
static INT32  PSTNGetStatus(int *piStatus);
static INT32  PSTNInitModem(ModemDialParms_t *pstModemParam);
static INT32  PSTNOpenModem(void);

static INT32   InitWirelessModule(ST_WIRELESS_INFO *pstParam);
static INT32   WirelessDial(uint8_t ucDialFlag);
static INT32   WirelessRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);
static INT32   WirelessTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut);
static INT32   WirelessClose(uint8_t bRelease);
//static int   DispWirelessSignal(void);
static int     WirelessReset(ST_WIRELESS_INFO *pstParam);

static int   SetSockAddr(ST_COMM_CONFIG *pstCfg);

static int   WiFiDial(UINT8 ucDialMode);

static void  GetAllErrMsg(INT32 iErrCode, ERR_INFO *pstInfo, COMM_ERR_MSG *pstCommErrMsg);
//static void  CalcCRC32(uint8_t *psData, UINT32 uiLength, uint8_t sCRC[4]);
static uint8_t CalcLRC(uint8_t *psData, UINT32 uiLength, uint8_t ucInit);
//static INT32   Conv2AsyncTxd(uint8_t *psTxdData, UINT32 uiDataLen);
/********************** Internal variables declaration *********************/
//static int          sg_Wiresocket = -1 ;   
static int          sg_Tcpsocket =  -1 ;
static int          sg_Wire_PreDial_Ret;
static int          sg_Wire_Status = -1;
static int          sg_iModemfd = -1;
static int          sg_iRs232Status = -1;
static UINT32       sg_uiWirePPPLogoutTime = 0; // 存储前一次ppp成功挂断的时间，从而保证有3秒的间隔才开始新的ppp拨号动作
static UINT8        sg_ucWirePPPFlag = 1; // 表示无线ppp链接的状态 0表示已挂断 1表示未挂断 -- 8210第一次也能挂断，否则在没有ppplogin的情况下不能关闭模块
static UINT8        sg_pppWaitFlag = 0; // 1表示 ppp 拨号后需要延时
static int		    sg_PredialdisplayMsgFlag = 0;    //预拨号是否在屏幕上显示信息，由于8210是线程处理不能显示，8110需要显示
static int          sg_InitModulRet = 0 ;   // 由于启用线程在处理初始化，因此用一个变量保持初始化值, 默认成功，避免第一次启动两个线程初始化
//static struct sockaddr     sg_stWiresockaddr;
static struct sockaddr     sg_stTcpsockaddr;
//static struct sockaddr     sg_stModemPPPsockaddr; //***MODEMPPP**

static ST_COMM_CONFIG	sg_stCurCfg = {0xFF};
static uint8_t		sg_sWorkBuf[LEN_WORKBUF+50];
static ERR_INFO		sg_stCommErrMsg[] =
{
	{ERR_COMM_INV_PARAM, "参数错误",     "INVALID PARAM"},
	{ERR_COMM_INV_TYPE,  "无效通讯类型", "INV COMM TYPE"},
	{ERR_COMM_CANCEL,    "用户取消",     "USER CANCEL"},
	{ERR_COMM_TIMEOUT,   "通讯超时",     "TIMEOUT"},
	{ERR_COMM_COMERR,    "通讯错误",     "COMM ERROR"},
	{ERR_COMM_TOOBIG,    "数据包太大",   "DATA TOO BIG"},
	{0, "", ""},
};

static ERR_INFO		sg_stRS232ErrMsg[] =
{
	{PORT_RET_NOTEMPTY,    "发送缓冲区满", "OVERFLOW"},
	{PORT_RET_PORTERR,     "无效端口号",   "INVALID PORT"},
	{PORT_RET_NOTOPEN,     "端口未打开",   "PORT CLOSED"},
	{PORT_RET_BUSY,        "无可用端口",   "NO PORT AVAIL"},
	{PORT_RET_MODEM_INUSE, "端口被占用",   "PORT OCCUPY"},
	{PORT_RET_PARAERR,     "无效通讯参数", "INVALID PARAM"},
	{PORT_RET_TIMEOUT,     "串口超时",     "TIMEOUT"},
	{0, "", ""},
};

static ERR_INFO		sg_stPPPErrMsg[] =
{
	{NET_ERR_RETRY,        "通讯超时",        "TIMEOUT"},
	{ERR_COMM_NOT_ATTACHED,  "模块附网失败",        "ATTACHED ERROR"},
	{NET_ERR_RSP,            "无应答,检查模块",     "CHECK MODULE"},
	{NET_ERR_NOSIM,          "SIM卡不存在",         "NO SIM CARD"},
	{NET_ERR_PIN,            "需要输入PIN码",       "NEED SIM PIN"},
	{NET_ERR_PUK,            "需要输入PUK码",       "NEED SIM PUK"},
	{NET_ERR_PWD,            "PIN码错误",           "SIM PIN ERROR"},
	{NET_ERR_SIMDESTROY,     "SIM卡错误",           "NO SIM/NEED PIN"},
	{NET_ERR_CSQWEAK,        "信号太弱",            "SIGNAL TOO WEAK"},
	{NET_ERR_LINKCLOSED,     "网络无载波",          "NO CARRIER"},
	{NET_ERR_LINKOPENING,    "连接成功",            "LINK OK"},
	{NET_ERR_ATTACHING,      "正在查找网络",        "SEEKING NETWORK"},
	{NET_ERR_DETTACHED,      "已断开无线网络",      "DETTACH NETWORK"},
	{NET_ERR_EMERGENCY,      "检查SIM卡",           "PLS CHECK SIM"},
	{NET_ERR_RING,           "等待接入中",          "ACCEPTED ERR"},
	{NET_ERR_BUSY,           "正在通讯中",          "COMMINICATE ERR"},
	{NET_ERR_DIALING,        "模块正在拨号",        "LINKING"},
	{NET_ERR_UNKNOWN,        "未知错误码",          "UNKNOW ERR"},
	{NET_ERR_ABNORMAL,       "未知错误码",          "UNKNOW ERR"},
	{-WNET_DLOPEN_ERR,       "初始化错误(open)",    "open lib err"},
	{-WNET_DLSYM_ERR,        "初始化错误(lib)",     "get point err"},
	{-WNET_INIT_ERR,         "初始化错误",          "wnet init err"},
	{-WNET_NOINIT_ERR,       "模块没有初始化",      "wnet do not init"},
	{-WNET_PARAM_ERR,        "无线参数错误",        "param err"},
	{-WNET_GET_OP_ERR,       "通讯错误err",         "get struct err"},
	{-WNET_NO_FUN_ERR,       "通讯错误call",        "function error "},
	{-WNET_POWER_DOWN_ERR,   "通讯模块下电错误",    "went powerdown "},
	{-WNET_BEEN_USE_ERR,     "模块被占用",          "wnet userd now"},
	{-WNET_SUSPEND_ERR,      "模块被挂起",          "wnet suspend now"},
	{-WNET_NO_MODULE_ERR,    "检查通讯模块",        "not wnet module"},
	{-WNET_RECV_TIMEOUT_ERR, "模块通讯超时",        "receive timeout"},
	{-WNET_USER_CANCEL_ERR,  "用户取消",            "user cancel"},
	{-WNET_RECV_DATA_ERR,    "模块接收超时",        "not used"},
	{-WNET_MOD_RESP_ERR,     "模块接收错误",        "receive data err"},
	{-WNET_SEND_DATA_ERR,    "模块发送错误",        "send data  err"},
	{-WNET_COMM_OPEN_ERR,    "打开模块设备错误",    "port open err"},
	{-WNET_SMS_DIV_ERR,      "通讯错误(SMS)",       "not used"},
	{-WET_DETTACHED_ERR,     "附着网络错误",        "Dettached"},
	{-WNET_ATTACHING_ERR,    "附着基站错误",        "Dettached error"},
	{-WNET_ABNORMAL_ERR,     "未知错误码err",       "abnormal error"},
	{-WNET_CSQWEAK_ERR,      "信号太弱",            "signal too weak"},
	{-WET_EMERGENCY_ERR,     "SIM卡状态错误",       "SIM/UIM error"},
	{-WET_RING_ERR,          "等待接入中",          "detect ringing"},
	{-WET_BUSY_ERR,          "正在通讯中",          "detect call err"},
	{-WET_DIALING_ERR,       "模块正在拨号",        "dialing"},
	{0, "", ""},
};

//#define	EINTR		 4	/* Interrupted system call */
//#define	EIO		 5	/* I/O error */
#ifndef ETIME
#define  ETIME   62
#endif
static ERR_INFO	sg_stModemErrMsg[] =
{
	{-MODEM_ERRNO_ERROR,             "设备异常",       "MODEM ERROR"},
	{-MODEM_ERRNO_BUSY,              "设 备 忙",       "MODEM BUSY"},
	{-MODEM_ERRNO_NO_DIALTONE,       "无拨号音",       "NO DIAL TONE"},
	{-MODEM_ERRNO_NO_CARRIER,        "线路载波丢失",   "LINE BREAK"},
	{-MODEM_ERRNO_NO_LINE,           "请接好电话线",   "LINE READY ?"},
	{-MODEM_ERRNO_NO_ANSWER,         "拨号无应答",     "NO ACK"},
	{-MODEM_ERRNO_OFF_HOOK,          "请先挂机",       "PLS ONHOOK"},
	{-MODEM_ERRNO_LINE_IN_USE,       "被叫线路忙",     "LINE BUSY"},
	{-MODEM_ERRNO_UN_OBTAINABLE,     "线路不可到达",   "UNOBTAINABLE"},
	{-MODEM_ERRNO_LINE_REVERSAL,     "线路反转",       "LINE REVERSAL"},
	{-MODEM_ERRNO_EXTENSION_IN_USE,  "旁置电话占用",   "PHONE OCCUPIED"},
	{ERR_COMM_STATUS_TIMEOUT,        "连接主机超时",   "TIME OUT"},
	{ERR_COMM_MODEM_INIT,            "初始化设备失败", "MODEM INIT ERROR"},
	{NET_ERR_RETRY,                  "通讯超时",       "TIMEOUT"},
	{4,                              "链路已断开",     "LINE BREAK "},
	{5,                              "链路被断开",     "LINE BREAK "},
	{16,                             "设 备 忙",       "MODEM BUSY"},
	{22,                             "参数错误",       "INVALID ARGUMENT"},
	{ETIMEDOUT,                      "连接主机超时",   "TIME OUT"},
	{ETIME,                          "连接主机超时",   "TIME OUT"},
	{0, "", ""},
};

/********************** external reference declaration *********************/
extern pthread_t  PreDialTid,InitCommTid;
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

int  GetCurSocket(void)
{
	if (sg_stCurCfg.ucCommType == CT_CDMA || sg_stCurCfg.ucCommType == CT_GPRS)
	{
		return sg_Tcpsocket;
	}

	if (sg_stCurCfg.ucCommType == CT_TCPIP || sg_stCurCfg.ucCommType == CT_WIFI)
	{
		return sg_Tcpsocket;
	}

	return -1;
}
 
int   SetSockAddr(ST_COMM_CONFIG *pstCfg)
{
	int iRet,iPort;

	if (pstCfg->ucCommType == CT_CDMA || pstCfg->ucCommType == CT_GPRS)
	{
		iPort = atoi((char *)pstCfg->stWirlessPara.szRemotePort);
//		iRet = SockAddrset(&sg_stWiresockaddr,pstCfg->stWirlessPara.szRemoteIP,iPort);
		iRet = SockAddrset(&sg_stTcpsockaddr,pstCfg->stWirlessPara.szRemoteIP,iPort);
		return iRet;		
	} 

	if (pstCfg->ucCommType==CT_TCPIP)
	{
		iPort = atoi((char*)pstCfg->stTcpIpPara.szRemotePort);
		iRet = SockAddrset(&sg_stTcpsockaddr,pstCfg->stTcpIpPara.szRemoteIP,iPort);	
		return iRet;		
	}

	if (pstCfg->ucCommType == CT_WIFI)
	{
		iPort = atoi((char*)pstCfg->stWIFIIpPara.szRemotePort);
		iRet = SockAddrset(&sg_stTcpsockaddr,pstCfg->stWIFIIpPara.szRemoteIP,iPort);	
		return iRet;
	}

	return 0;
}


// 通讯模块初始化
INT32 CommInitModule(ST_COMM_CONFIG *pstCfg)
{
	int    iRet;

	CommSetCfgParam(pstCfg);

	switch (pstCfg->ucCommType)
	{
	case CT_WIFI:
		iRet = InitWiFiModule();
		break;

	case CT_CDMA:
	case CT_GPRS:
		iRet = InitWirelessModule(&pstCfg->stWirlessPara);
		break;

	case CT_MODEM:
		iRet = PSTNInitModem(&pstCfg->stPSTNPara.stDialPara);
		break;

	default:
		iRet = 0;
		break;
	}

	sg_InitModulRet = iRet;
	if ((iRet & MASK_ERR_CODE) != 0)
	{
		return iRet;	
	}
	return 0;
}

// 设置通讯模块参数
INT32 CommSetCfgParam(ST_COMM_CONFIG *pstCfg)
{
	INT32		iLen;

	if( pstCfg==NULL )
	{
		return ERR_COMM_INV_PARAM;
	}

	switch( pstCfg->ucCommType )
	{
	case CT_RS232:
		iLen = strlen((char *)pstCfg->stRS232Para.szAttr);
		if( iLen<10 || iLen>20 )	// 简单检查参数
		{
			return ERR_COMM_INV_PARAM;
		}
		break;

	case CT_MODEM:
		if (!(gstPosCapability.uiCommType & CT_MODEM))
		{
			return ERR_COMM_INV_PARAM;
		}
		if(  pstCfg->stPSTNPara.szTxnTelNo[0]==0  )
		{
			return ERR_COMM_INV_PARAM;
		}
		if( pstCfg->stPSTNPara.ucSendMode!=MODEM_COMM_ASYNC &&
			pstCfg->stPSTNPara.ucSendMode!=MODEM_COMM_SYNC )
		{
			return ERR_COMM_INV_PARAM;
		} 
		break;

	case CT_TCPIP:
		if (!(gstPosCapability.uiCommType & CT_TCPIP))
		{
			return ERR_COMM_INV_PARAM;
		}

		if( pstCfg->stTcpIpPara.szRemoteIP[0]==0   ||
			pstCfg->stTcpIpPara.szRemotePort[0]==0 )
		{
			return ERR_COMM_INV_PARAM;
		}
	    break;

	case CT_CDMA:
	case CT_GPRS:
		if (!(gstPosCapability.uiCommType & CT_CDMA) && !(gstPosCapability.uiCommType & CT_GPRS))
		{
			return ERR_COMM_INV_PARAM;
		}

		if( pstCfg->stWirlessPara.szAPN[0]==0     ||
			pstCfg->stWirlessPara.szRemoteIP[0]==0 )
		{
			return ERR_COMM_INV_PARAM;
		}
		break;

	case CT_WIFI:
		if (!(gstPosCapability.uiCommType & CT_WIFI))
		{
			return ERR_COMM_INV_PARAM;
		}

		if( pstCfg->stWIFIIpPara.szRemoteIP[0]==0   ||
			pstCfg->stWIFIIpPara.szRemotePort[0]==0 )
		{
			return ERR_COMM_INV_PARAM;
		}
		break;

	default:
		return ERR_COMM_INV_TYPE;
	}

	memcpy(&sg_stCurCfg, pstCfg, sizeof(ST_COMM_CONFIG));

	return 0;
}


// 通讯模块拨号(MODEM拨号或者TCP建立TCP连接等)
INT32 CommDial(uint8_t ucDialMode)
{
	int		iRet;

	if (ucDialMode==ACTDIAL_MODE)
	{
		printf("end  init moduel fun \n");
		ThreadJoinInitModule();
	}

	switch( sg_stCurCfg.ucCommType )
	{
	case CT_RS232:
		iRet = RS232Dial(ucDialMode);
		break;
		
	case CT_MODEM:
		iRet = PSTNDial(ucDialMode);
		break;
		
	case CT_TCPIP:
		iRet = TcpDial(ucDialMode);
		break;
		
	case CT_CDMA:
	case CT_GPRS:
		iRet = WirelessDial(ucDialMode);
		break;
		
	case CT_WIFI:
		if (gstPosCapability.uiPosType == 0x02) // 6110 的WIFI和TCP一样处理
		{
			iRet = TcpDial(ucDialMode);
		}
		else
		{
			iRet = WiFiDial(ucDialMode);
		}
		break;
		
	default:
		iRet = ERR_COMM_INV_TYPE;
		break;
	}

	return iRet;
}

// 通讯模块发送数据
INT32 CommTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	INT32		iRet; 

	if( psTxdData==NULL )
	{
		return ERR_COMM_INV_PARAM;
	}
	
	switch( sg_stCurCfg.ucCommType )
	{
	case CT_RS232:
		iRet = RS232Txd(psTxdData, uiDataLen, uiTimeOut);
		break;

	case CT_MODEM:
		iRet = PSTNTxd(psTxdData, uiDataLen, uiTimeOut);
		break;

	case CT_TCPIP:
	case CT_WIFI:
		iRet = TcpTxd(psTxdData, uiDataLen, uiTimeOut);
		break;

	case CT_CDMA:
	case CT_GPRS:
		iRet = WirelessTxd(psTxdData, uiDataLen, uiTimeOut);
		break;

	default:
		iRet = ERR_COMM_INV_TYPE;
		break;
	}

	return iRet;
}

// 通讯模块接收数据
INT32 CommRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	INT32		iRet;

	if( psRxdData==NULL )
	{
		return ERR_COMM_INV_PARAM;
	}

	switch( sg_stCurCfg.ucCommType )
	{
	case CT_RS232:
		if (uiTimeOut<=5)
		{
			uiTimeOut = 5;
		}
		iRet = RS232Rxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
		break;

	case CT_MODEM:
		if (uiTimeOut<=15)
		{
			uiTimeOut = 15;
		}
		iRet = PSTNRxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
		break;

	case CT_TCPIP:
	case CT_WIFI:
		if (uiTimeOut<=10)
		{
			uiTimeOut = 10;
		}
		iRet = TcpRxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
		TcpOnHook(FALSE);
		break;

	case CT_CDMA:
	case CT_GPRS:
		if (uiTimeOut<=15)
		{
			uiTimeOut = 15;
		}
		iRet = WirelessRxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
//		WirelessClose(FALSE);  // 接受后需要断开TCP连接，下次联机时重新连接
		break;

	default:
		iRet = ERR_COMM_INV_TYPE;
		break;
	}

	return iRet;
}

// 通讯模块断开链路(MODEM挂机或者TCP断开TCP连接等)
INT32 CommOnHook(uint8_t bReleaseAll)
{
	INT32		iRet;
	INT32 Col, Line;
	UINT8 Buff[2048]; // 当前屏幕为128*64，建议开辟的缓冲区大小为2K；
	uint8_t ucCommType;

	Col = Line = 0;
	memset(Buff,0,sizeof(Buff));
//	lcdStore(&Col, &Line, Buff);
// 	lcdClrLine(2,7);
// 	DisplayProcessing();
// 	lcdFlip();
	ucCommType = sg_stCurCfg.ucCommType;
	printf("commhook12: %d \n",ucCommType);
	if (ucCommType==0 || ucCommType > CT_ALLCOMM)
	{
		ucCommType = gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType;
	}
	printf("commhook: %d \n",ucCommType);

	switch( ucCommType )
	{
	case CT_RS232:
		iRet = RS232OnHook(bReleaseAll);
		break;

	case CT_MODEM:
		iRet = PSTNOnHook(bReleaseAll);
		break;

	case CT_TCPIP:
	case CT_WIFI:
		iRet = TcpOnHook(bReleaseAll);
		break;

	case CT_CDMA:
	case CT_GPRS:
		iRet = WirelessClose(bReleaseAll);
		break;

	default:
		iRet = ERR_COMM_INV_TYPE;
		break;
	}

//	lcdRestore(Col, Line, Buff);
	return iRet;
}
 
// 获取通讯错误信息
void CommGetErrMsg(INT32 iErrCode, COMM_ERR_MSG *pstCommErrMsg)
{
	int  iReErrCode;

	if( pstCommErrMsg==NULL )
	{
		return;
	}

	iReErrCode = iErrCode & MASK_ERR_CODE;

	switch( iErrCode & MASK_COMM_TYPE )
	{
	case ERR_COMM_ALL_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "通讯错误:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "COMM ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stCommErrMsg, pstCommErrMsg);
		break;

	case ERR_COMM_RS232_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "串口错误:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "PORT ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stRS232ErrMsg, pstCommErrMsg);
		break;

	case ERR_COMM_MODEM_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "拨号错误:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "MODEM ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stModemErrMsg, pstCommErrMsg);
	    break;

	case ERR_COMM_TCPIP_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "TCPIP错误:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "TCPIP ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stPPPErrMsg, pstCommErrMsg);
		break;
	case ERR_COMM_WIRELESS_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "无线错误:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "WIRE ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stPPPErrMsg, pstCommErrMsg);
		break;
	default:
		sprintf((char *)pstCommErrMsg->szChnMsg, "通讯错误");
		sprintf((char *)pstCommErrMsg->szEngMsg, "ERR CODE:%04X", iReErrCode);
		break;
	}
}

void GetAllErrMsg(INT32 iErrCode, ERR_INFO *pstInfo, COMM_ERR_MSG *pstCommErrMsg)
{
	INT32		iCnt;

	for(iCnt=0; pstInfo[iCnt].iErrCode!=0; iCnt++)
	{
		if( pstInfo[iCnt].iErrCode==iErrCode ||
			pstInfo[iCnt].iErrCode==(iErrCode & MASK_ERR_CODE) )
		{
			sprintf((char *)pstCommErrMsg->szChnMsg, "%.16s", pstInfo[iCnt].szChnMsg);
			sprintf((char *)pstCommErrMsg->szEngMsg, "%.16s", pstInfo[iCnt].szEngMsg);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// RS232 通讯模块
//////////////////////////////////////////////////////////////////////////
// 串口打开
INT32 RS232Dial(uint8_t ucDialMode)
{
	int 	iRet;

	if( ucDialMode==PREDIAL_MODE )
	{
		return 0;
	}

	if (sg_iRs232Status != -1)
	{
		return PORT_RET_OK;
	}

	iRet = portOpen(sg_stCurCfg.stRS232Para.ucPortNo,(char *)sg_stCurCfg.stRS232Para.szAttr);
	if( iRet != PORT_RET_OK )
	{
		return (ERR_COMM_RS232_BASE | iRet);
	}

	sg_iRs232Status = 1;
	return PORT_RET_OK;
}

// 串口发送数据
INT32 RS232Txd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	INT32		iRet;

	if (sg_stCurCfg.stRS232Para.ucSendMode == CM_RAW)
	{	
		iRet = RS232RawTxd(psTxdData, uiDataLen, uiTimeOut);
	}
	else
	{
		iRet = RS232NacTxd(psTxdData, uiDataLen, uiTimeOut);
	}

	return iRet;
}

// 串口直接发送
int RS232RawTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	int 	iRet;
	int     iOldTime,iNowTime;

	if (uiTimeOut<=0)
	{
		uiTimeOut = 2;
	}
	iOldTime = sysGetTimerCount();
	iRet = 0;
	while (1)
	{
		iNowTime = sysGetTimerCount();
		if ((iNowTime-iOldTime) >= uiTimeOut*1000)
		{
			iRet =  ERR_COMM_RS232_BASE | iRet;
			break;
		}

		iRet = portSends(sg_stCurCfg.stRS232Para.ucPortNo, psTxdData,uiDataLen);
		if( iRet == PORT_RET_OK )
		{
			break;
		}
		sysDelayMs(50);
	}
 
	return iRet;
}

// STX+Len1+Len2+strings+ETX+CKS, CKS = Len1 -- ETX (^)
INT32 RS232NacTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	INT32		iRet;

	if( uiDataLen>LEN_WORKBUF )
	{
		return ERR_COMM_TOOBIG;
	}

	sg_sWorkBuf[0] = STX;
	sg_sWorkBuf[1] = (uiDataLen/1000)<<4    | (uiDataLen/100)%10;	// convert to BCD
	sg_sWorkBuf[2] = ((uiDataLen/10)%10)<<4 | uiDataLen%10;
	memcpy(&sg_sWorkBuf[3], psTxdData, uiDataLen);
	sg_sWorkBuf[3+uiDataLen]   = ETX;
	sg_sWorkBuf[3+uiDataLen+1] = CalcLRC(psTxdData, uiDataLen, (uint8_t)(sg_sWorkBuf[1] ^ sg_sWorkBuf[2] ^ ETX));

	iRet = RS232RawTxd(sg_sWorkBuf, (UINT32)(uiDataLen+5), uiTimeOut);	// data
	if( iRet!=0 )
	{
		return iRet;
	}

	return 0;
}

// 串口接收
INT32 RS232Rxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	INT32		iRet;

	if (sg_stCurCfg.stRS232Para.ucSendMode == CM_RAW)
	{
		iRet = RS232RawRxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
	}
	else
	{
		iRet = RS232NacRxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
	}

	return iRet;
}

// 串口直接接收
INT32 RS232RawRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	int   iRet,iFlag;
	UINT32	uiReadCnt, uiTemp;
	UINT32  iOldTimeMs,iNewTimeMs;

	uiReadCnt = iFlag = 0;
	uiTemp = 5000;
	iOldTimeMs = sysGetTimerCount();
	while( uiReadCnt<uiExpLen )
	{
		if (iFlag==0)
		{
			iNewTimeMs= sysGetTimerCount();
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
			{
				if( uiReadCnt>0 )	// 已经读取到数据
				{
					break;
				}

				return ERR_COMM_TIMEOUT;
			}

			lcdDisplay(187,4,DISP_CFONT,"(%d)",(iNewTimeMs-iOldTimeMs)/1000);
			lcdFlip();
			iRet = portCheckRecvBuf(sg_stCurCfg.stRS232Para.ucPortNo);
			if (iRet != PORT_RET_NOTEMPTY)
			{
				if (iRet != PORT_RET_OK )
				{
					return (ERR_COMM_RS232_BASE | iRet);
				}
				
				continue;
			}
			else
			{
				iFlag = 1;
			}
		}

		iRet = portRecv(sg_stCurCfg.stRS232Para.ucPortNo, psRxdData, uiTemp);
		if( iRet==0x00 )
		{	// 接收成功,继续
			uiTemp = 80;
			psRxdData++;
			uiReadCnt++;
		}
		else if( iRet==0xFF )
		{
			if( uiReadCnt>0 )
			{
				break;
			}

			return ERR_COMM_TIMEOUT;
		}
		else
		{	// 非超时错误,退出
			return (ERR_COMM_RS232_BASE | iRet);
		}
	}   // end of while( uiReadCnt<uiExpLen

	if( puiOutLen!=NULL )
	{
		*puiOutLen = uiReadCnt;
	}

	return 0;
}

// STX+Len1+Len2+strings+ETX+CKS, CKS = Len1 -- ETX (^)
INT32 RS232NacRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	INT32   iRet ,iFlag;
	UINT32	uiReadCnt, uiLength;
	UINT32  iOldTimeMs,iNewTimeMs,uiTemp;

	if( uiExpLen>LEN_WORKBUF )
	{
		return ERR_COMM_TOOBIG;
	}

	iFlag = 0;
	uiTemp = 5000;
	uiReadCnt = uiLength = 0;
	memset(sg_sWorkBuf, 0, sizeof(sg_sWorkBuf));
	iOldTimeMs = sysGetTimerCount();
	while( 1 )
	{
		if (iFlag==0)
		{
			iNewTimeMs= sysGetTimerCount();
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
			{
				if( uiReadCnt>0 )	// 已经读取到数据
				{
					break;
				}
				
				return ERR_COMM_TIMEOUT;
			}
			
			lcdDisplay(187,4,DISP_CFONT,"(%d)",(iNewTimeMs-iOldTimeMs)/1000);
			lcdFlip();
			iRet = portCheckRecvBuf(sg_stCurCfg.stRS232Para.ucPortNo);
			if (iRet != PORT_RET_NOTEMPTY)
			{
				if (iRet != PORT_RET_OK )
				{
					return (ERR_COMM_RS232_BASE | iRet);
				}
				
				continue;
			}
			else
			{
				iFlag = 1;
			}
		}

		iRet = portRecv(sg_stCurCfg.stRS232Para.ucPortNo, &sg_sWorkBuf[uiReadCnt], uiTemp);
		if( iRet!=0 )
		{
			if( iRet==0xFF )
			{
				continue;
			}
			return ERR_COMM_RS232_BASE|iRet;
		}
		uiTemp = 100;
		if( sg_sWorkBuf[0]!=STX )
		{
			continue;
		}

		uiReadCnt++;
		if( uiReadCnt==3 )
		{
			uiLength =  ((sg_sWorkBuf[1]>>4) & 0x0F) * 1000 + (sg_sWorkBuf[1] & 0x0F) * 100 +
						((sg_sWorkBuf[2]>>4) & 0x0F) * 10   + (sg_sWorkBuf[2] & 0x0F);
		}
		if( uiReadCnt==uiLength+5 )
		{	// read data ok, verify it ...
			if( sg_sWorkBuf[uiReadCnt-2]==ETX &&
				CalcLRC(&sg_sWorkBuf[1], (UINT32)(uiReadCnt-1), 0)==0 )
			{
				break;
			}
			return ERR_COMM_COMERR;
		}
	}

	memcpy(psRxdData, &sg_sWorkBuf[3], uiLength);
	if( puiOutLen!=NULL )
	{
		*puiOutLen = uiLength;
	}

	return 0;
}


// 串口关闭
INT32 RS232OnHook(uint8_t bReleaseAll)
{
	int	iRet;

	iRet = portClose(sg_stCurCfg.stRS232Para.ucPortNo);
	if( iRet ==0 )
	{
		sg_iRs232Status = -1;
		return 0;
	}

	return (ERR_COMM_RS232_BASE | iRet);
}

//////////////////////////////////////////////////////////////////////////
// TCP 通讯模块
//////////////////////////////////////////////////////////////////////////
// 建立TCP连接
INT32 TcpDial(uint8_t ucDialMode)
{
	int iRet,iFlag,iRetyTimes;
	UINT32   uiOldTime;

	if( ucDialMode==PREDIAL_MODE )
	{
		return 0;
	}

    //  实际连接
	iFlag = 1;
	iRetyTimes = 1;
	uiOldTime = sysGetTimerCount();	
	while (iFlag)
	{
		if ( sysGetTimerCount() >= (uiOldTime+60000) )
		{ // 拨号连接超时60s，则返回
			TcpOnHook(FALSE);
			return (ERR_COMM_TCPIP_BASE|NET_ERR_RETRY);
		}
		
		if( PubChkKeyInput() )
		{
			if (kbGetKey() == KEY_CANCEL)
			{
				TcpOnHook(FALSE);
				return ERR_USERCANCEL;
			}
		}
		
		if (iRetyTimes > 3 )
		{// 三次拨号连接不成功则关闭套节字，断开PPP连接
			TcpOnHook(FALSE);
			return (ERR_COMM_TCPIP_BASE|NET_ERR_RETRY);
		}
		
		if (sg_Tcpsocket < 0)
		{
			sg_Tcpsocket = NetSocket(AF_INET,SOCK_STREAM,0);
			if (sg_Tcpsocket < 0)
			{
				TcpOnHook(FALSE);
				iRet = sg_Tcpsocket;
				if (iRet<0)
				{
					iRet = -iRet;
				}
				return (ERR_COMM_TCPIP_BASE|iRet);
			}
			iRet = SetSockAddr(&sg_stCurCfg);
			if (iRet != NET_OK)
			{
				if (iRet<0)
				{
					iRet = -iRet;
				}
				TcpOnHook(TRUE);
				return ERR_COMM_TCPIP_BASE|iRet;
			}

			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT,"   连 接 中 ... ");	
				lcdDisplay(0,5,DISP_CFONT,"TCPIP CONNECT(%d)",iRetyTimes++);
			}
			else
			{
				lcdDisplay(0,4,DISP_CFONT,"TCPIP CONNECT(%d)",iRetyTimes++);
			}
			DrawRect(0, 17, 127, 63);
			lcdFlip();

			iRet = NetConnect(sg_Tcpsocket,&sg_stTcpsockaddr,sizeof(struct sockaddr));
			if (iRet != NET_OK)
			{
				CommOnHook(FALSE);  // 挂断TCPIP重新连接
				if(iRet < 0)
				{
					iRet = ABS(iRet);
				}
				iRet = ERR_COMM_TCPIP_BASE|iRet;
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			return 0;
		}
	}

	return iRet;
}

// 发送数据
INT32 TcpTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	int  iRet,iRelSendlen;
	struct timeval stTimeVal;
	
	stTimeVal.tv_sec = 30;
	stTimeVal.tv_usec = 0;
	NetSetsockopt(sg_Tcpsocket,SOL_SOCKET,SO_SNDTIMEO,&stTimeVal,sizeof(stTimeVal));
	
	for (iRelSendlen=0; iRelSendlen<uiDataLen;)
	{
		iRet = NetSend(sg_Tcpsocket,psTxdData+iRelSendlen,(uiDataLen-iRelSendlen),0);
		if (iRet < 0)
		{
			TcpOnHook(TRUE);
			iRet = -iRet;
			return ERR_COMM_TCPIP_BASE|iRet;
		}
		iRelSendlen += iRet;
	}
	
	return 0;
}

// 接收数据
INT32 TcpRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	int  iRet,iRetryCnt;
	int  iOldTime,iNowTime;
	UINT32  ulRealRecvLen = 0;
	unsigned long  ulTemp;
	struct timeval stTimeVal;
	socklen_t      Socklen;
	
	stTimeVal.tv_sec = 1;
	stTimeVal.tv_usec = 0;
	Socklen = sizeof(stTimeVal);
	NetSetsockopt(sg_Tcpsocket,SOL_SOCKET,SO_RCVTIMEO,&stTimeVal,Socklen);

	iRetryCnt = 0;
	iOldTime = sysGetTimerCount();
	while( 1 )
	{
		iNowTime= sysGetTimerCount();
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
		{
			iRet = ERR_COMM_TIMEOUT;
			TcpOnHook(TRUE);
			break;
		}

		lcdDisplay(187,4,DISP_CFONT,"(%d)",(iNowTime-iOldTime)/1000);
		lcdFlip();
		iRet = NetRecv(sg_Tcpsocket,psRxdData+ulRealRecvLen,uiExpLen,0);
		if (iRet > 0)
		{
		// BCD 格式的长度字节
			if (gstSysParam.stEdcInfo.ucTcpMsgHeadLenType == _TCP_BCD_LEN_BYTE)
				PubBcd2Long((char*)psRxdData,  2, &ulTemp);
			else
				PubChar2Long((char*)psRxdData, 2, &ulTemp);
			ulRealRecvLen += iRet;
			if (ulRealRecvLen < ulTemp+2)
			{
				continue;
			}

			*puiOutLen = ulRealRecvLen;
			iRet = 0;
			break;
		}
		else
		{
			iRetryCnt++;
			if (iRetryCnt>3)
			{
				if (iRet<0)
				{
					iRet = -iRet;
				}
				iRet = ERR_COMM_TCPIP_BASE|iRet;
			}
			
			sysDelayMs(20);
		}
	}

	return iRet;
}

// 断开TCP连接
INT32 TcpOnHook(uint8_t bReleaseAll)
{
	INT32  iRet=0;

    if (-1 != sg_Tcpsocket)
	{
	    iRet = NetClose(sg_Tcpsocket);
	    sg_Tcpsocket = -1;
	}

	if (bReleaseAll)
	{
	    if (-1 != sg_Wire_Status)
		{
		    iRet = PPPLogout(OPT_DEVETH);
            sg_Wire_Status = -1;
    		if (iRet<0)
    		{
    			iRet = -iRet;
    		}
        }
	}

	return iRet;
}


//////////////////////////////////////////////////////////////////////////
// MODEM 通讯模块
//////////////////////////////////////////////////////////////////////////
#define MODEM_COMM_ERR(a)		(ERR_COMM_MODEM_BASE|(a))
// modem 拨号
INT32 PSTNDial(uint8_t ucDialMode)
{
	int      iRet, iStatus, iDialTimes, iLastErr;
	UINT32   uiOldTime;
	int      iReInitFlag;

	if (ucDialMode == PREDIAL_MODE)
	{
		iDialTimes = 0;
		iStatus = MODEM_STATE_NOT_INIT;

		uiOldTime = sysGetTimerCount();
		while (1)
		{
			if (sg_PredialdisplayMsgFlag)
			{
				lcdClrLine(2, 7);
				lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "  预拨号中... ");
				lcdDisplay(0, 5, DISP_CFONT|DISP_MEDIACY, " PREDIALING..."); 
				DrawRect(0, 17, 127, 63);
				lcdFlip();
			}
			
		    if (kbhit())
		    {
				lcdFlip();
		    	iRet = kbGetKey();
				if (KEY_CANCEL == iRet)
				{
				    return MODEM_COMM_ERR(ERR_COMM_CANCEL);
				}
		    }
			if ( sysGetTimerCount() >= (uiOldTime+120000) )
			{ // 拨号连接超时60s，则返回
				PSTNOnHook(TRUE);
				return MODEM_COMM_ERR(NET_ERR_RETRY);
			}

			if (iDialTimes>3)  // 三次不成功则退出失败
			{
				modem_get_last_errno(sg_iModemfd,&iLastErr);
				iLastErr = ABS(iLastErr);
				if (iLastErr == 0)
				{
					iLastErr = -MODEM_ERRNO_NO_CARRIER;
				}
				return MODEM_COMM_ERR(iLastErr);
			}
			
			iStatus = MODEM_STATE_NOT_INIT;
			iRet = PSTNGetStatus(&iStatus);
			if (iRet !=0 ) 
			{
				if (sg_iModemfd < 0 )  // 如果modem句柄无效，则先打开
				{
					PSTNOnHook(TRUE);
					sg_iModemfd = PSTNOpenModem();
					if (sg_iModemfd < 0)
					{
						return MODEM_COMM_ERR(ERR_COMM_MODEM_INIT); 
					}
					continue;
				}
				else
				{
					iRet = ABS(iRet);
					return MODEM_COMM_ERR(iRet);
				}
			}

			switch (iStatus)
			{
			case MODEM_STATE_NOT_INIT:
				PSTNOnHook(TRUE);
				sg_iModemfd = PSTNOpenModem();
				if (sg_iModemfd < 0)
				{
					return MODEM_COMM_ERR(ERR_COMM_MODEM_INIT); 
				}
				break;

			case MODEM_STATE_NO_SET_MODE:
				iRet = modem_set_dial_parms(sg_iModemfd,&sg_stCurCfg.stPSTNPara.stDialPara);
				if (iRet !=0 )
				{
					return MODEM_COMM_ERR(ERR_COMM_MODEM_NOPARAM);
				}
				break;

			case MODEM_STATE_SYNC_MODE:  // 该状态表示正在设置,需要等待设置成功
			case MODEM_STATE_ASYN_MODE:  // 该状态表示正在设置,需要等待设置成功
				break;

			case MODEM_STATE_DISCONNECT:
				iDialTimes++;
				iRet = modem_get_last_errno(sg_iModemfd,&iLastErr);
				if (iLastErr != 0)
				{
					PSTNOnHook(FALSE);
				}

				iRet = modem_dialing(sg_iModemfd,(char *)sg_stCurCfg.stPSTNPara.szTxnTelNo);
				iRet = ABS(iRet);
				if (iRet != 0)
					return MODEM_COMM_ERR(iRet);
				else
					return 0;
				break;

			case MODEM_STATE_WAITTING:  // 被拨号状态，先挂断当前被拨连接，再重新拨号
				PSTNOnHook(FALSE);
				break;

			default:
				return 0;
				break;
			}
		}

		return MODEM_COMM_ERR(ERR_COMM_MODEM_INIT);  
	}

	iDialTimes = 0;
	iReInitFlag = 0;
	uiOldTime = sysGetTimerCount();
	while (1)
	{
		if ( sysGetTimerCount() >= (uiOldTime+120000) )
		{ // 拨号连接超时60s，则返回
			PSTNOnHook(TRUE);
			return MODEM_COMM_ERR(NET_ERR_RETRY);
		}

		if( PubChkKeyInput() )
		{
			if (kbGetKey() == KEY_CANCEL)
			{
				PSTNOnHook(FALSE);
				return ERR_USERCANCEL;
			}
		}

		lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{				
			lcdDisplay(0, 5, DISP_CFONT, "  DIALING...(%d)", iDialTimes);
		}
		else
		{
			lcdDisplay(0, 3, DISP_CFONT, "    拨 号 中   ");				
			lcdDisplay(0, 5, DISP_CFONT, "  DIALING...(%d)", iDialTimes);
		}
		DrawRect(0, 17, 127, 63);
		lcdFlip();

		iStatus = MODEM_STATE_NOT_INIT;
		iRet = PSTNGetStatus(&iStatus);
		if (iRet !=0 ) 
		{
			if (sg_iModemfd < 0)
			{
				PSTNOnHook(TRUE);
				sg_iModemfd = PSTNOpenModem();
				if (sg_iModemfd < 0)
				{
					return MODEM_COMM_ERR(ERR_COMM_MODEM_INIT); 
				}
				continue;
			}
			else
			{
				iRet = ABS(iRet);
				return MODEM_COMM_ERR(iRet);
			}
		}
		
		switch (iStatus)
		{
		case MODEM_STATE_NOT_INIT:
			PSTNOnHook(TRUE);
			sg_iModemfd = PSTNOpenModem();
			if (sg_iModemfd < 0)
			{
				return MODEM_COMM_ERR(ERR_COMM_MODEM_INIT); 
			}
			break;

		case MODEM_STATE_NO_SET_MODE:
			iRet = modem_set_dial_parms(sg_iModemfd,&sg_stCurCfg.stPSTNPara.stDialPara);
			if (iRet !=0 )
			{
				return MODEM_COMM_ERR(ERR_COMM_MODEM_NOPARAM);
			}
			break;

		case MODEM_STATE_SYNC_MODE:  // 该状态表示正在设置,需要等待设置成功
		case MODEM_STATE_ASYN_MODE:  // 该状态表示正在设置,需要等待设置成功
		case MODEM_STATE_DAILING:
		case MODEM_STATE_CONNECT_SDLC:
			sysDelayMs(100); // 延时后在查看
			break;
			
		case MODEM_STATE_DISCONNECT:
			if (iDialTimes >= 3)  // 三次不成功则退出失败
			{
				modem_get_last_errno(sg_iModemfd,&iLastErr);
				iLastErr = ABS(iLastErr);
				if (iLastErr == 0)
				{
					iLastErr = -MODEM_ERRNO_NO_CARRIER;
				}
				return MODEM_COMM_ERR(iLastErr);
			}

			if (iReInitFlag == 1)
			{
				PSTNOnHook(TRUE);
				iReInitFlag = 0;
				continue;
			}

			iRet = modem_dialing(sg_iModemfd,(char*)sg_stCurCfg.stPSTNPara.szTxnTelNo);
			if (iRet != 0)
			{
				iRet = ABS(iRet);
				return MODEM_COMM_ERR(iRet);
			}
			iDialTimes++;
			iReInitFlag = 1;
			break;
			
		case MODEM_STATE_WAITTING:  // 被拨号状态，先挂断当前被拨连接，再重新拨号
			PSTNOnHook(FALSE);
			break;
	
		case MODEM_STATE_CONNECT:
			return 0;

		default:
			iStatus = ABS(iStatus);
			return MODEM_COMM_ERR(iStatus);
		}
	}
	
	iRet = -MODEM_ERRNO_ERROR;
	return MODEM_COMM_ERR(iRet);
}

// Modem 发送
INT32 PSTNTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	int  iRet,iRelSendlen;
		
	for (iRelSendlen=0; iRelSendlen<uiDataLen;)
	{
		iRet = modem_write_timeout(sg_iModemfd,psTxdData+iRelSendlen,(uiDataLen-iRelSendlen),uiTimeOut);
		if (iRet < 0)
		{
			PSTNOnHook(FALSE);   // 发送失败后断开连接，下次重新连接
			iRet = ABS(iRet);
			return MODEM_COMM_ERR(iRet);
		}
		iRelSendlen += iRet;
	}

	return 0;
}

// Modem接收
INT32 PSTNRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	int		iRet;
	
	switch( sg_stCurCfg.stPSTNPara.ucSendMode )
	{
	case MODEM_COMM_ASYNC:
		iRet = PSTNAsyncRxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
		break;
		
	case MODEM_COMM_SYNC:
		iRet = PSTNSyncRxd(psRxdData, uiExpLen, uiTimeOut, puiOutLen);
		break;
		
	default:
		iRet = MODEM_COMM_ERR(ERR_COMM_INV_PARAM);
		break;
	}
	
	return iRet;
}


// Modem同步接收
INT32 PSTNSyncRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	int  iRet,  iRetryCnt;
	int  iOldTime,iNowTime;
	UINT32  ulRealRecvLen = 0;
	unsigned long  ulTemp = 0;

	ulTemp = 0;
	iRet = MODEM_ERRNO_ERROR;
	iRetryCnt = 0;
	iOldTime = sysGetTimerCount();
	while( 1 )
	{
		iNowTime= sysGetTimerCount();
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
		{
			iRet = ERR_COMM_TIMEOUT;
			break;
		}
		
		lcdDisplay(187,4,DISP_CFONT,"(%d)",(iNowTime-iOldTime)/1000);
		lcdFlip();
		iRet = modem_read_timeout(sg_iModemfd,psRxdData+ulRealRecvLen,uiExpLen,850);
		if (iRet > 0)
		{
/*			PubBcd2Long(psRxdData+1, 2, &ulTemp);
			ulRealRecvLen += iRet;
			// 0x02 + 2byte(len) + MSG + 0x03 + crc
			if (ulRealRecvLen < ulTemp+5)*/
			ulRealRecvLen += iRet;
			if (ulRealRecvLen < 40)			
			{
				continue;
			}
			
			*puiOutLen = ulRealRecvLen;
			iRet = 0;
			break;
		}
		else
		{
			if (iRet < 0)
			{
//				iRet = PosixGeterrno();
				iRet = ABS(iRet);
				iRet = MODEM_COMM_ERR(iRet);
				return iRet;
			}

			iRetryCnt++;
			if (iRetryCnt>3)
			{
				iRet = ABS(iRet);
				iRet = MODEM_COMM_ERR(iRet);
			}
			
			sysDelayMs(100);
		}
	}
	
	if (iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = MODEM_COMM_ERR(iRet);
	}

	return iRet;
}

// Modem异步接收
INT32 PSTNAsyncRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	int  iRet,  iRetryCnt;
	int  iOldTime,iNowTime;
	UINT32  ulRealRecvLen = 0;
	unsigned long  ulTemp = 0;
	
	iRet = MODEM_ERRNO_ERROR;
	iRetryCnt = 0;
	iOldTime = sysGetTimerCount();
	while( 1 )
	{
		iNowTime= sysGetTimerCount();
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
		{
			iRet = ERR_COMM_TIMEOUT;
			break;
		}
		
		lcdDisplay(187,4,DISP_CFONT,"(%d)",(iNowTime-iOldTime)/1000);
		lcdFlip();
		iRet = modem_read_timeout(sg_iModemfd,psRxdData+ulRealRecvLen,uiExpLen,850);
		if (iRet > 0)
		{			
			PubBcd2Long((char*)psRxdData+1, 2, &ulTemp);
			ulRealRecvLen += iRet;
			// 0x02 + 2byte(len) + MSG + 0x03 + crc
			if (ulRealRecvLen < ulTemp+5)
			{
				continue;
			}
			
			*puiOutLen = ulRealRecvLen;
			iRet = 0;
			break;
		}
		else
		{			
			if (iRet < 0)
			{
//				iRet = PosixGeterrno();
				iRet = ABS(iRet);
				iRet = MODEM_COMM_ERR(iRet);
				return iRet;
			}
			
			iRetryCnt++;
			if (iRetryCnt>3)
			{
				iRet = ABS(iRet);
				iRet = MODEM_COMM_ERR(iRet);
			}
			
			sysDelayMs(100);
		}
	}
	
	if (iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = MODEM_COMM_ERR(iRet);
	}
	return iRet;
}

// Modem 挂机
INT32 PSTNOnHook(uint8_t bReleaseAll)
{
	int  iRet,iStatus;

	iStatus = 0;
    if (sg_iModemfd < 0)
    {
        return 0;
    }

	modem_wait_until_sent(sg_iModemfd);

/*	// 排除连续多次挂断的请求
	iRet = modem_get_status(sg_iModemfd, &iStatus);
	if ( ! (iStatus == (0x80000000 | MODEM_STATE_CONNECT)) )
		iRet = modem_hangup(sg_iModemfd);

	if (bReleaseAll)
	{
		while(1)
		{
			iRet = modem_get_status(sg_iModemfd, &iStatus);
			if (iRet != 0)
				break;
			
			if (iStatus & 0x80000000) // 目前最高位为1表示操作还没完成，需要再次检测
			{
				sysDelayMs(100);
				continue;
			}

			break;
		}
		iRet = modem_close(sg_iModemfd);
		sg_iModemfd = -1;
	} */
	
	// 马来西亚特殊版本，由于后台NAC没有响应挂断请求导致挂断时间较长，故直接关闭modem模块
	iRet = modem_close(sg_iModemfd);
	sg_iModemfd = -1;
	if (iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = MODEM_COMM_ERR(iRet);
	}
	return iRet;
}


INT32  PSTNGetStatus(int *piStatus)
{
	int  iRet, iStatus;
	INT32  iOldTime , iTimeOut;

	iTimeOut = sg_stCurCfg.stPSTNPara.stDialPara.dial_timeo;
	if (iTimeOut >= 60 || iTimeOut<=0)
	{
		iTimeOut = 60;
	}
	iOldTime = sysGetTimerCount();
	while (1) 
	{
		if( PubChkKeyInput() )
		{
			if (kbGetKey() == KEY_CANCEL)
			{
				return ERR_USERCANCEL;
			}
		}
		if ( sysGetTimerCount() >= (iOldTime+iTimeOut*1000) )
		{ // 超时10s，则返回
			modem_get_last_errno(sg_iModemfd,&iStatus);
			iStatus = ABS(iStatus);	
			if (iStatus != 0)
			{
				return MODEM_COMM_ERR(iStatus);
			}
			else
			{
				return MODEM_COMM_ERR(ERR_COMM_STATUS_TIMEOUT);
			}			
		}

		iRet = modem_get_status(sg_iModemfd, &iStatus);
		if (iRet != 0)
			break;
		
		if (iStatus & 0x80000000) // 目前最高位为1表示操作还没完成，需要再次检测
		{
			sysDelayMs(100);
			continue;
		}
		
		*piStatus = iStatus & 0x7FFFFFFF;
		break;
	}
	
	if (iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = MODEM_COMM_ERR(iRet);
	}
	return iRet;
}

INT32  PSTNInitModem(ModemDialParms_t *pstModemParam)
{
	int  iRet, iStatus;
	ModemDialParms_t  stOldModemParam;
	
	iStatus = MODEM_STATE_NOT_INIT;
	iRet = PSTNGetStatus(&iStatus);
	if (iRet !=0 ) 
	{
		if (sg_iModemfd < 0 )  // 如果modem句柄无效，则先打开
		{
			PSTNOnHook(TRUE);
			sg_iModemfd = PSTNOpenModem(); 
			if (sg_iModemfd < 0)
			{
				return MODEM_COMM_ERR(ERR_COMM_MODEM_INIT); 
			}
		}
		else
		{
			iRet = ABS(iRet);
			return MODEM_COMM_ERR(iRet);
		}
	}

	iStatus = MODEM_STATE_NOT_INIT;
	iRet = PSTNGetStatus(&iStatus);
	if (iRet != 0 || iStatus == MODEM_STATE_NOT_INIT)
	{
		return MODEM_COMM_ERR(MODEM_STATE_NOT_INIT);
	}

	iRet = modem_get_dial_parms(sg_iModemfd, &stOldModemParam);
	if (iRet != 0 || memcmp(&stOldModemParam,pstModemParam,sizeof(ModemDialParms_t)) !=0 )
	{
		iRet = modem_set_dial_parms(sg_iModemfd,pstModemParam);
	}
	else
	{
		iRet = 0;
	}
	
	if (iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = MODEM_COMM_ERR(iRet);
	}
	return iRet;  
}


INT32  PSTNOpenModem(void)
{
	int   iFd;

	iFd = modem_open(MODEM_PATHNAME, O_RDWR); 
	if (iFd < 0)
	{
		iFd = modem_open(MODEM_PATHNAME1, O_RDWR); 
	}

	return iFd;
}

//////////////////////////////////////////////////////////////////////////
// GPRS/CDMA 通讯模块
//////////////////////////////////////////////////////////////////////////

#define WIRELESS_ERR(a)		(ERR_COMM_WIRELESS_BASE|(a))
INT32 InitWirelessModule(ST_WIRELESS_INFO *pstParam)
{
	int   iRet,iCnt;
//	UINT32 uiOldTime;
//	char      sVersion[10];
	
	sg_InitModulRet = sg_Tcpsocket = -1;
	
	iRet = WnetInit(20000);
	if( iRet!= NET_OK )
	{		
		iRet = ABS(iRet);
		return WIRELESS_ERR(iRet);
	}

	iCnt = 2;  // 2次检查SIM卡，中延时2S，间防止SIM卡上电时间不够的问题
	while(iCnt--)
	{
		// check PIN of SIM card
		iRet = WnetCheckSim();
		if( iRet == -NET_ERR_PIN )
		{
			iRet =  WnetInputSimPin((char*)pstParam->szSimPin);
			if( iRet != NET_OK )
			{
				iRet = ABS(iRet);
				return WIRELESS_ERR(iRet);
			}

			break;
		}
		else
		{	
			if (iRet != NET_OK)
			{
				iRet = ABS(iRet);
				iRet = WIRELESS_ERR(iRet);
				sysDelayMs(1000);
				continue;
			}
			else
			{
				break;
			}
		}
	}

	if (iRet != 0)
	{
		printf("wnet check error :%d \n",iRet);
		iRet = ABS(iRet);
		return WIRELESS_ERR(iRet);
	}

	iRet = CheckWirelessAttached(30*1000);
	printf("wnet check attached :%d \n",iRet);
	if (iRet != 0)
	{
		sg_InitModulRet = WIRELESS_ERR(ERR_COMM_NOT_ATTACHED);
		return sg_InitModulRet;
	}
	
	printf("wnet init ret :%d \n",iRet);

//	DispWirelessSignal();
	sg_InitModulRet = 0;
	return iRet;
}

INT32 WirelessDial(uint8_t ucDialFlag)
{
	int iRet,iFlag,iRetyTimes,iDialTimes;
	uint32_t   uiOldTime;
	int        iDevType = OPT_DEVWNET;
	static   int  iReInitTimes = 0;
	static   int  iTimeoutTimes = 0;

	// 8210的ppp拨号函数的设备名称和原来的不同，需要区分GPRS,CDMA
#ifdef _POS_TYPE_8210
	if (sg_stCurCfg.ucCommType == CT_CDMA)
		iDevType = PPP_DEV_CDMA;
	else
		iDevType = PPP_DEV_GPRS;
#endif

	kbFlush();
	// 预拨号
	if( ucDialFlag== PREDIAL_MODE )  
	{
		printf("Pre dial :%d \n",sg_InitModulRet);

		if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
		{
			if (sg_InitModulRet != WIRELESS_ERR(ERR_COMM_NOT_ATTACHED)){
				return sg_InitModulRet; // 初始化失败,如果是由于非附网成功则则返回
			}
		}

		iReInitTimes = 0;
		sg_Wire_PreDial_Ret = 0;
		iRet = PPPCheck(iDevType);
		printf("Pre dial pppcheck :%d \n",iRet);
		if (iRet < 0)
		{
			if (iRet == -NET_ERR_LINKOPENING) // 正在建立连接中，预拨号就返回成功
			{
				return 0;
			}

#ifdef _POS_TYPE_8210
			iRet = -ENETDOWN;  // 8210 的pppcheck如果非0和PPP_LINK_BUILDING，就复位模块,然后拨号
#endif
			if (iRet == -ETIMEDOUT || iRet == -ENETDOWN)    // iRet == -NET_ERR_UNKNOWN || iRet == -ENETDOWN)
			{   // 超时退出则先复位模块
				if (iRet == -ETIMEDOUT){
					sg_InitModulRet = -1;
				}
				if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
				{
					if (sg_InitModulRet == WIRELESS_ERR(ERR_COMM_NOT_ATTACHED))
					{
						iRet = CheckWirelessAttached(10*1000); // 预拨号时检测附网情况时间短一些
						printf("Pre dial atattched :%d \n",iRet);
						if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
						{
							iRet = ABS(iRet);
							return WIRELESS_ERR(iRet);
						}
						sg_InitModulRet = 0;  // 附网成功，置初始化成功
					}
					else
					{
						sg_InitModulRet = -1;
						iRet = WirelessReset(&sg_stCurCfg.stWirlessPara);
						printf("Pre dial reset :%d \n",iRet);
						if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
						{
							iRet = ABS(iRet);
							return WIRELESS_ERR(iRet);
						}
					}
				}
			}
			while((UINT32)(sysGetTimerCount() - sg_uiWirePPPLogoutTime) < 3000); // 挂断后必须隔3秒以上才能再拨号
			iRet = PPPLogin(iDevType,(char *)sg_stCurCfg.stWirlessPara.szAPN,
				            (char *)sg_stCurCfg.stWirlessPara.szUID,
							(char *)sg_stCurCfg.stWirlessPara.szPwd,PPP_ALG_CHAP,60000);
			sg_ucWirePPPFlag = 1;	// 每次做过了PPP拨号的动作，不管最后成功与否，都可以做一次挂断ppp的动作			
			iRet = ABS(iRet);
			sg_Wire_PreDial_Ret = iRet;
			sg_pppWaitFlag = 1; // ppp 拨号后需要延时
			printf("Pre dial ppplogin :%d \n",iRet);
		}

		return WIRELESS_ERR(iRet);
	}

	iDialTimes = 1;
	iFlag = 1;
	iRetyTimes = 1;
	uiOldTime = sysGetTimerCount();
	while (iFlag)
	{
		if( PubChkKeyInput() )
		{
			if (kbGetKey() == KEY_CANCEL)
			{
				return ERR_USERCANCEL;
			}
		}

		if ( sysGetTimerCount() >= (uiOldTime+60000) )
		{ 
			iTimeoutTimes++;
			printf("Dial timeout :%d \n",sg_InitModulRet);
			if (iTimeoutTimes<3){
				WirelessClose(TRUE);
				printf("wireless close timeout :%d \n",sg_InitModulRet);
			}
			else{
				printf("wireless reset timeout :%d \n",iTimeoutTimes);
				WirelessReset(&sg_stCurCfg.stWirlessPara); //如果三次都失败则复位模块
				iTimeoutTimes =0;
			}			
			return WIRELESS_ERR(NET_ERR_RETRY);
		}

		printf("Dial :%d \n",sg_InitModulRet);

		if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
		{
			iReInitTimes++;
			if (iReInitTimes<=2)
			{
				lcdClrLine(2, 7);
				lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "INIT MODULE...");
				lcdDisplay(0, 5, DISP_CFONT|DISP_MEDIACY, " PLS WAIT...  ");
				lcdFlip();
				if (sg_InitModulRet == WIRELESS_ERR(ERR_COMM_NOT_ATTACHED))
				{// 如果是由于附网没有成功，则附网，不重启模块
					iRet = CheckWirelessAttached(30*1000);  
					printf("dial wnet check attached ret :%d \n",iRet);
					if (iRet != 0)
					{
						iRet = ABS(iRet);
						return WIRELESS_ERR(iRet);
					}
					sg_InitModulRet = 0;
				}
				else
				{
					printf("Dial wireclose :%d \n",sg_InitModulRet);
					iRet = WirelessReset(&sg_stCurCfg.stWirlessPara);
					printf("Dial reset :%d \n",iRet);
					if (iRet!=0 && iRet !=WIRELESS_ERR(0))
					{
						return iRet; // 初始化失败则返回
					}
				}				
			}
			else
			{
				return sg_InitModulRet;
			}
		}
		
		iReInitTimes = 0;
		if( sg_Wire_PreDial_Ret==NET_ERR_NOSIM || sg_Wire_PreDial_Ret==NET_ERR_SIMDESTROY)
		{
			printf("Dial predial ret :%d \n",sg_InitModulRet);
			WirelessClose(TRUE);
			return WIRELESS_ERR(sg_Wire_PreDial_Ret);
		}

		lcdClrLine(2, 7);	
		lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY|DISP_CLRLINE, "PPP CONNECT...(%d)", iDialTimes); 
		DrawRect(0, 17, 127, 63);
		lcdFlip();

		iRet = PPPCheck(iDevType);
		printf("Dial pppcheck :%d \n",iRet);
		if (iRet < 0)
		{
			//这段代码移进来的原因是：由于超时判断已经移进来了，所以为了发挥超时判断的左右，必须把这段代码也移进来。
			if (iRet == -NET_ERR_LINKOPENING)
			{
				sysDelayMs(400);
				continue;
			}
		
#ifdef _POS_TYPE_8210
			iRet = -ENETDOWN;  // 8210 的pppcheck如果非0和PPP_LINK_BUILDING，就复位模块,然后拨号
#endif
			CommOnHook(FALSE);// 先挂断链接
			if (iRet == -ETIMEDOUT || iRet == -ENETDOWN)    // iRet == -NET_ERR_UNKNOWN || iRet == -ENETDOWN)
			{   // 超时退出则先复位模块
				if (iRet == -ETIMEDOUT){
					sg_InitModulRet = -1;
				}
				if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
				{
					lcdClrLine(2, 7);
					lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "INIT MODULE...");
					lcdDisplay(0, 5, DISP_CFONT|DISP_MEDIACY, " PLS WAIT...  ");
					lcdFlip();
					sg_InitModulRet = -1;
					iRet = WirelessReset(&sg_stCurCfg.stWirlessPara);
					printf("Pre dial reset :%d \n",iRet);
					if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
					{
						iRet = ABS(iRet);
						return WIRELESS_ERR(iRet);
					}
				}
			}
#ifndef _POS_TYPE_8210			
			sysDelayMs(3000); // 挂断后必须隔3秒以上才能再拨号
#endif
			iDialTimes++;
			iRet = PPPLogin(iDevType,(char *)sg_stCurCfg.stWirlessPara.szAPN,
				            (char *)sg_stCurCfg.stWirlessPara.szUID,
							(char *)sg_stCurCfg.stWirlessPara.szPwd,PPP_ALG_CHAP,60000);
			sg_ucWirePPPFlag = 1; // 每次做过了PPP拨号的动作，不管最后成功与否，都可以做一次挂断ppp的动作
			if (iRet<0)
			{
				iRet = -iRet;
				
				if (iDialTimes > 3)
				{
					//WirelessClose(TRUE);
					WirelessReset(&sg_stCurCfg.stWirlessPara); // 三次不成功则复位模块
					return WIRELESS_ERR(NET_ERR_RETRY);
				}
			}
			sg_Wire_PreDial_Ret = iRet;
			sysDelayMs(1000);  // ppplogin后延时1s，否则pppcheck返回错误，会继续ppplogin
			sg_pppWaitFlag = 1; // ppp 拨号后需要延时
			continue;
		}
		iTimeoutTimes =0; //成功则置超时次数为0
#ifndef _POS_TYPE_8210
		// 由于目前使用的套节字最多是0,1,2 . 所以如果sg_Tcpsocket值异常，先将其赋初值(挂断)
		if(sg_Tcpsocket<0 || sg_Tcpsocket>3)  
			CommOnHook(FALSE);
#endif			
		if (sg_Tcpsocket <= -1)
		{
			lcdClrLine(2,7);	
			lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"TCPIP CONNECT(%d)",iRetyTimes++); 
			DrawRect(0, 17, 127, 63);
			lcdFlip();
			
			 if (sg_pppWaitFlag)
            {
                 sysDelayMs(2000); // ppp拨号后，重新建立TCP连接之前延时2s后在进行处理，增强在特定环境下的发送成功率
                 sg_pppWaitFlag = 0;
            }
            
			//  实际连接
			sg_Tcpsocket = NetSocket(AF_INET,SOCK_STREAM,0);
			if (sg_Tcpsocket < 0)
			{
				iRet = sg_Tcpsocket;
				if (iRet<0)
				{
					iRet = -iRet;
				}

				WirelessClose(FALSE);
				iRet = ABS(iRet);
				return WIRELESS_ERR(iRet);
			}

			iRet = SetSockAddr(&sg_stCurCfg);
			printf("Dial socket addr :%d %d \n",iRet,sysGetTimerCount());
			if (iRet != NET_OK)
			{
				iRet = ABS(iRet);
				WirelessClose(FALSE);
				return WIRELESS_ERR(iRet);
			}

			iRet = NetConnect(sg_Tcpsocket,&sg_stTcpsockaddr,sizeof(struct sockaddr));
			/***
			这段代码这么修改的原因是：由于tcp连接尝试次数是否超限的判断已经放在后面，所以，这里必须如此修改。
			当tcp连接失败，自然地执行后面的次数判断代码。
			***/						
			if (NET_OK == iRet)
			{
				sysDelayMs(200);  //  在特定情况下，connect后延时在进行发送
				return iRet;
			}

			/***
			增加这段代码的原因是: 当返回-1043时，说明用户按取消键取消TCP连接，那么应该退出通讯。
			***/
			if (-NET_ERR_USER_CANCEL == iRet)
			{
				return ERR_USERCANCEL;
			}	
		}
		else
		{ 
			// 目前还不能检测该套节字是否还处在连接，可用中，只有应用自己严格处????????
			return 0;
		}
		
		/***
		这段代码移到后面的原因是：既然最后一次尝试tcp连接都不成功了，根本不需要循环到开头去，可直接退出函数。
		而且，这段代码在前面的一个危害是：如果最后一次尝试tcp连接失败，循环回头后，做pppcheck检查，如果pppcheck返回非0，那么将会再去做ppplogin，耗时耗力直到
		ppp链接建立起来了，却在这段代码退出了函数，白做了无用功，还耗时间。
		***/
		if (iRetyTimes > 3 )
		{// 三次拨号连接不成功则关闭套节字，断开PPP连接
			WirelessClose(TRUE);
			return WIRELESS_ERR(NET_ERR_RETRY);
		}
		else /***这段代码是前面tcp连接失败后的处理，移到这里来了***/
		{
			CommOnHook(FALSE);  // 挂断TCPIP重新连接
			iRet = ABS(iRet);
			iRet = WIRELESS_ERR(iRet);
			continue;
		}
	}
	
	if (iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = WIRELESS_ERR(iRet);
	}
	return iRet;
}


int WirelessReset(ST_WIRELESS_INFO *pstParam)
{
    int   iRet;
	
	WirelessClose(TRUE);
	
    iRet = WnetReset();
	if( iRet!= NET_OK )
	{
#ifndef _POS_TYPE_8210   // 模块断电后gsmMuxd程序关闭，reset函数返回失败，此时还是需要对模块上电复位
		iRet = ABS(iRet);
		return WIRELESS_ERR(iRet);
#endif
	}
//	lcdFlip();
//	sysDelayMs(5000); // 复位后，延时5s
	
	return InitWirelessModule(pstParam);
}


//无线通讯接收
INT32 WirelessRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	int  iRet,  iRetryCnt;
	int  iOldTime,iNowTime;
	UINT32  ulRealRecvLen = 0;
	unsigned long  ulTemp = 0;
	struct timeval stTimeVal;
	socklen_t      Socklen;

	stTimeVal.tv_sec = 1;
	stTimeVal.tv_usec = 0;
	Socklen = sizeof(stTimeVal);
	NetSetsockopt(sg_Tcpsocket,SOL_SOCKET,SO_RCVTIMEO,&stTimeVal,Socklen);
	
	iRetryCnt = 0;
	iOldTime = sysGetTimerCount();
	while( 1 )
	{
		iNowTime= sysGetTimerCount();
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
		{
			iRet = ERR_COMM_TIMEOUT;
			break;
		}
		
		lcdDisplay(187,4,DISP_CFONT,"(%d)",(iNowTime-iOldTime)/1000);
		lcdFlip();
		iRet = NetRecv(sg_Tcpsocket,psRxdData+ulRealRecvLen,uiExpLen,0);
		if (iRet > 0)
		{
		    // BCD 格式的长度字节
			if (gstSysParam.stEdcInfo.ucTcpMsgHeadLenType == _TCP_BCD_LEN_BYTE)
				PubBcd2Long((char*)psRxdData,  2, &ulTemp);
			else
				PubChar2Long((char*)psRxdData, 2, &ulTemp);
			ulRealRecvLen += iRet;
			if (ulRealRecvLen < ulTemp+2)
			{
				continue;
			}

			*puiOutLen = ulRealRecvLen;
			iRet = 0;
			break;
		}
		else
		{
			iRetryCnt++;
			if (iRetryCnt>3)
			{
				iRet = ABS(iRet);
				iRet = WIRELESS_ERR(iRet);
			}

			sysDelayMs(20);
		}
	}

	return iRet;
}

// 无线通讯发送
INT32 WirelessTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	int  iRet,iRelSendlen;
	struct timeval stTimeVal;
	
	stTimeVal.tv_sec = 30;
	stTimeVal.tv_usec = 0;
	NetSetsockopt(sg_Tcpsocket,SOL_SOCKET,SO_SNDTIMEO,&stTimeVal,sizeof(stTimeVal));

	for (iRelSendlen=0; iRelSendlen<uiDataLen;)
	{
		iRet = NetSend(sg_Tcpsocket,psTxdData+iRelSendlen,(uiDataLen-iRelSendlen),0);
		if (iRet < 0)
		{
			WirelessClose(TRUE);   // 发送失败后断开连接，下次重新连接
			iRet = -iRet;
			return WIRELESS_ERR(iRet);
		}
		iRelSendlen += iRet;
	}

	return 0;
}

// 无线模块挂机
INT32 WirelessClose(uint8_t bRelease)
{
	int  iRet = 0, iCnt = 0;
	int        iDevType = OPT_DEVWNET;
	
	// 8210的ppp拨号函数的设备名称和原来的不同，需要区分GPRS,CDMA
#ifdef _POS_TYPE_8210
	if (sg_stCurCfg.ucCommType == CT_CDMA)
		iDevType = PPP_DEV_CDMA;
	else
		iDevType = PPP_DEV_GPRS;
#endif

	kbFlush();

	if(sg_Tcpsocket != -1)
	{
		iRet = NetClose(sg_Tcpsocket);
	}
	sg_Tcpsocket = -1;

	if (bRelease && 0 != sg_ucWirePPPFlag)
	{
		for (iCnt = 0; iCnt < 2; iCnt++) //不能随便地被用户按取消键来阻止ppplogout，最多允许用户误操作2次
		{
			iRet = PPPLogout(iDevType);
			if (-NET_ERR_USER_CANCEL == iRet)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		
		wnet_power_down();  // 8210关闭系统中gsmMuxd程序
		sg_InitModulRet = -1;  // 模块初始化失败
		PreDialTid = InitCommTid=-1;
		if (NET_OK == iRet || -NET_ERR_LINKCLOSED == iRet)
		{
			if (NET_OK == iRet){
				sg_uiWirePPPLogoutTime = sysGetTimerCount();
			}		
			sg_ucWirePPPFlag = 0;
			iRet = NET_OK;  
		}		
	}

	if(iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = WIRELESS_ERR(iRet);
	}
	return iRet;
}

// 显示信号强度
int  DispWirelessSignal(void)
{
	return 0;

	/*int  iRet, iSignal;
	
//	if( sg_stCurCfg.ucCommType!=CT_GPRS && sg_stCurCfg.ucCommType!=CT_CDMA )
	if( stPosParam.stTxnCommCfg.ucCommType!=CT_GPRS && stPosParam.stTxnCommCfg.ucCommType!=CT_CDMA )
	{
		return;
	}

	iRet = WnetSignal(&iSignal);
	if( (iRet != 0 ) || (iSignal == 99) )
	{
		lcdSetIcon(ICON_SIGNAL, CLOSEICON);
		return;
	}
	
    if((iSignal >= 1) && (iSignal <= 7))
    {
        iSignal = 1;
    }
    else if((iSignal >= 8) && (iSignal <= 13))
    {
        iSignal = 3;
    }
    else if((iSignal >= 14) && (iSignal <= 19))
    {
        iSignal = 4;
    }
    else if((iSignal >= 20) && (iSignal <= 25))
    {
        iSignal = 5;
    }
    else if((iSignal >= 26) && (iSignal <= 31))
    {
        iSignal = 6;
    }

	lcdSetIcon(ICON_SIGNAL, iSignal);*/
}


int     CheckWirelessAttached(uint32_t uiTimeOut)
{
	int iRet = WIRELESS_ERR(ERR_COMM_TIMEOUT);
	
	uiTimeOut += sysGetTimerCount();
	while(1)
	{
		if (sysGetTimerCount()>=uiTimeOut)
		{
			return iRet;
		}
		
		iRet = wnet_set_attached(1);
		if (iRet != 0)
		{
			sysDelayMs(500);
			continue;
		}
		break;
	}
	
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
// WIFI 通讯模块
//////////////////////////////////////////////////////////////////////////
#define WIFI_COMM_ERR(a)		(ERR_COMM_WIFI_BASE|(a))
INT32   InitWiFiModule(void)
{
#ifdef _POS_TYPE_8210
	if (!(gstPosCapability.uiCommType & CT_WIFI))
	{
		return ERR_COMM_INV_TYPE;
	}
	return 0;

#else

	int       iRet, iRetry, iTemp;
    char      szlocalIP[32];
    WiFiStatus_t    stWiFiStatus;
	UINT32    uiOldTime;
	char      sVersion[10];
	if (!(gstPosCapability.uiCommType & CT_WIFI))
	{
		return ERR_COMM_INV_TYPE;
	}

	if (gstPosCapability.uiPosType == 0x02) // 6110 的WIFI和TCP一样处理
	{
		return 0;
	}

	memset(sVersion,0,sizeof(sVersion));
	sysReadVer((uchar*)sVersion);
	if (sVersion[6]== 0x30 || sVersion[6]==0x32)
	{
		uiOldTime = sysGetTimerCount();
		while(1)
		{
			if ( sysGetTimerCount() >= (uiOldTime+10*1000) )
			{ // 拨号连接超时10s，则返回
				WirelessClose(TRUE);
				return WIRELESS_ERR(NET_ERR_RETRY);
			}
			
			if( PubChkKeyInput() )
			{
				if (kbGetKey() == KEY_CANCEL)
				{
					return ERR_USERCANCEL;
				}
			}
			
			iRet = WnetSelectModule(1);
			if (iRet < 0)
			{
				sysDelayMs(200);
				continue;
			}
			
			break;
		}
	}

	iRet = WnetInit(20000);
	if( iRet!= NET_OK )
	{		
		if (iRet<0)
		{
			iRet = -iRet;
		}
		return WIFI_COMM_ERR(iRet);
	}

	memset(szlocalIP, 0, sizeof(szlocalIP));
	iRetry = 0;
	while(1)
	{
        // 先检测连接状态
		while(1)
		{
			iRetry++;
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"PLS WAIT...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"CONNECTING..(%d)",iRetry);
			}
			else
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请稍候...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"WIFI连接中..(%d)",iRetry);
			}
			DrawRect(0, 17, 127, 63);
			lcdFlip();
			iRet = WifiGetCurConnect(&stWiFiStatus);
			iTemp = iRet;
			if (iRet != NET_OK || stWiFiStatus.Status != WIFI_STATUS_CONNECTED)
			{
				if (iTemp== NET_OK)
				{
					iTemp = WIFI_COMM_ERR(EWIFI_ONLINELOST);
				}
				lcdClrLine(2,7);
				if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				{
					lcdDisplay(0,2,DISP_CFONT|DISP_MEDIACY,"DON'T CONNECT");
					lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"PLS ENTER FUNC OR");
					lcdDisplay(0,6,DISP_CFONT|DISP_MEDIACY,"OR MENU TO SET");
				}
				else
				{
					PubDispString("获取当前连接失败",2|DISP_LINE_CENTER);
					PubDispString("按功能或菜单键",4|DISP_LINE_CENTER);
					PubDispString("设置WIFI配置参数",6|DISP_LINE_CENTER);
				}
				iRet = PubWaitKey(6);
				if (iRet == KEY_FN || iRet == KEY_MENU)
				{
					iRet = SelectAndSetWIFIAP();
					if (iRetry>=3)
					{
						return  WIFI_COMM_ERR(iTemp);
					}
					sysDelayMs(2000);// 延时1s
					continue;
				}
				else
				{
					return  WIFI_COMM_ERR(iTemp);
				}
			}
			break;
		}

		// 检测是否获取本地IP地址
		iRetry = 0;
		while(1)
		{
			iRetry++;
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"PLS WAIT...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"LOCAL IP..(%d)",iRetry);
			}
			else
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请稍候...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"获取本地IP..(%d)",iRetry);
			}
			DrawRect(0, 17, 127, 63);
			lcdFlip();
			memset(szlocalIP, 0, sizeof(szlocalIP));
			iRet = WifiGetLocalIP(szlocalIP);
			if(iRet != NET_OK)
			{
				iTemp = iRet;
				lcdClrLine(2,7);
				if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				{
					lcdDisplay(0,2,DISP_CFONT|DISP_MEDIACY,"DON'T CONNECT");
					lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"PLS ENTER FUNC OR");
					lcdDisplay(0,6,DISP_CFONT|DISP_MEDIACY,"OR MENU TO SET");
				}
				else
				{
					PubDispString("获取当前连接失败",2|DISP_LINE_CENTER);
					PubDispString("按功能或菜单键",4|DISP_LINE_CENTER);
					PubDispString("设置WIFI配置参数",6|DISP_LINE_CENTER);
				}
				iRet = PubWaitKey(6);
				if (iRet == KEY_FN || iRet == KEY_MENU)
				{
					iRet = SelectAndSetWIFIAP();
					if (iRetry>=3)
					{
						return  WIFI_COMM_ERR(iTemp);
					}
					continue;
				}
				else
				{
					return  WIFI_COMM_ERR(iTemp);
				}
			}

			if (strcmp(szlocalIP, "0.0.0.0") == 0)
			{
				if (iRetry>=3)
				{
					return  WIFI_COMM_ERR(EWIFI_SOCKIPREG);
				}
				sysDelayMs(2000);
				continue;
			}
			else
			{
				break;
			}
		}

		if (strcmp(szlocalIP, "0.0.0.0") != 0)
		{
			strcpy(gstCurAcq.stTxnCommCfg.stWIFIIpPara.szLocalIP,szlocalIP);
			break;
		}
	}

	return 0;
#endif
} 

INT32   GetWiFiStatus(void)
{
#ifdef _POS_TYPE_8210
	if (!(gstPosCapability.uiCommType & CT_WIFI))
	{
		return ERR_COMM_INV_TYPE;
	}
	
	return 0;

#else

	int       iRet, iRetry, iTemp;
    char      szlocalIP[32];
    WiFiStatus_t    stWiFiStatus;

	if (!(gstPosCapability.uiCommType & CT_WIFI))
	{
		return ERR_COMM_INV_TYPE;
	}

	if (gstPosCapability.uiPosType == 0x02) // 6110 的WIFI和TCP一样处理
	{
		return 0;
	}

	memset(szlocalIP, 0, sizeof(szlocalIP));
	iRetry = 0;
	while(1)
	{
        // 先检测连接状态
		while(1)
		{
			iRetry++;
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"PLS WAIT...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"CONNECTING..(%d)",iRetry);
			}
			else
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请稍候...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"WIFI连接中..(%d)",iRetry);
			}
			DrawRect(0, 17, 127, 63);
			lcdFlip();
			iRet = WifiGetCurConnect(&stWiFiStatus);
			iTemp = iRet;
			if (iRet != NET_OK || stWiFiStatus.Status != WIFI_STATUS_CONNECTED)
			{
				if (iTemp== NET_OK)
				{
					iTemp = WIFI_COMM_ERR(EWIFI_ONLINELOST);
				}

				if (iRetry>=3)
				{
					return  WIFI_COMM_ERR(iTemp);
				}
				sysDelayMs(2000);// 延时1s
				continue;	
			}

			break;
		}

		// 检测是否获取本地IP地址
		iRetry = 0;
		while(1)
		{
			iRetry++;
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"PLS WAIT...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"LOCAL IP..(%d)",iRetry);
			}
			else
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请稍候...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"获取本地IP..(%d)",iRetry);
			}
			DrawRect(0, 17, 127, 63);
			lcdFlip();
			memset(szlocalIP, 0, sizeof(szlocalIP));
			iRet = WifiGetLocalIP(szlocalIP);
			if(iRet != NET_OK)
			{
				if (iRetry>=3)
				{
					return  WIFI_COMM_ERR(iRet);
				}
				continue;
			}

			if (strcmp(szlocalIP, "0.0.0.0") == 0)
			{
				if (iRetry>=3)
				{
					return  WIFI_COMM_ERR(EWIFI_SOCKIPREG);
				}
				sysDelayMs(2000);
				continue;
			}
			else
			{
				break;
			}
		}

		if (strcmp(szlocalIP, "0.0.0.0") != 0)
		{
			break;
		}
	}

	return 0;
#endif
}

int   WiFiDial(UINT8 ucDialMode)
{
	int iRet,iFlag,iRetyTimes;
	UINT32   uiOldTime;
	
	if( ucDialMode==PREDIAL_MODE )
	{
		iRet = InitWiFiModule();
		return iRet;
	}
	
    //  实际连接
	iFlag = 1;
	iRetyTimes = 1;
	uiOldTime = sysGetTimerCount();	
	while (iFlag)
	{
		if ( sysGetTimerCount() >= (uiOldTime+60000) )
		{ // 拨号连接超时60s，则返回
			TcpOnHook(FALSE);
			return (ERR_COMM_TCPIP_BASE|NET_ERR_RETRY);
		}
				
		if( PubChkKeyInput() )
		{
			if (kbGetKey() == KEY_CANCEL)
			{
				TcpOnHook(FALSE);
				return ERR_USERCANCEL;
			}
		}

		if (iRetyTimes > 3 )
		{// 三次拨号连接不成功则关闭套节字，断开PPP连接
			TcpOnHook(FALSE);
			return (ERR_COMM_TCPIP_BASE|NET_ERR_RETRY);
		}

		// 检测是否连接上WIFI,这个时候如果没有连接成功就不提示用户进行WIFI设置而是直接返回
		iRet = GetWiFiStatus();
		if (iRet != 0)
		{
			return iRet;
		}

		if (sg_Tcpsocket < 0)
		{
			sg_Tcpsocket = NetSocket(AF_INET,SOCK_STREAM,0);
			if (sg_Tcpsocket < 0)
			{
				TcpOnHook(FALSE);
				iRet = sg_Tcpsocket;
				if (iRet<0)
				{
					iRet = -iRet;
				}
				return (ERR_COMM_TCPIP_BASE|iRet);
			}
			iRet = SetSockAddr(&sg_stCurCfg);
			if (iRet != NET_OK)
			{
				if (iRet<0)
				{
					iRet = -iRet;
				}
				TcpOnHook(TRUE);
				return ERR_COMM_TCPIP_BASE|iRet;
			}
			
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT,"   连 接 中 ... ");
				lcdDisplay(0,5,DISP_CFONT,"TCPIP CONNECT(%d)",iRetyTimes++);
			}
			else
			{
				lcdDisplay(0,4,DISP_CFONT,"TCPIP CONNECT(%d)",iRetyTimes++);
			}
			DrawRect(0, 17, 127, 63);
			lcdFlip();
			
			iRet = NetConnect(sg_Tcpsocket,&sg_stTcpsockaddr,sizeof(struct sockaddr));
			if (iRet != NET_OK)
			{
				CommOnHook(FALSE);  // 挂断TCPIP重新连接
				if(iRet<0)
				{
					iRet = -iRet;
				}
				iRet = ERR_COMM_TCPIP_BASE|iRet;
				continue;
			}
			else
			{
				break;
			}
		}
		else
		{
			return 0;
		}
	}
	
	if (iRet != 0)
	{
		iRet = ABS(iRet);
		iRet = WIRELESS_ERR(iRet);
	}
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
// 公共数据转换模块
//////////////////////////////////////////////////////////////////////////
// 计算CRC32
/*
void CalcCRC32(uint8_t *psData, UINT32 uiLength, uint8_t sCRC[4])
{
	ulong	ulRSL, tl;
	UINT32	ii;
	uint8_t	ucTemp, k;

	ulRSL = 0xFFFFFFFFL;
	for(ii=0; ii<uiLength; ii++)
	{
		ucTemp = (uint8_t)ulRSL;
		ucTemp = ucTemp^psData[ii];
		tl = (ulong)ucTemp;
		for(k=0; k<8; k++)
		{
			if( tl&1 )
			{
				tl = 0xedb88320L^(tl>>1);
			}
			else
			{
				tl = tl>>1;
			}
		}
		ulRSL = tl^(ulRSL>>8);
	}
	ulRSL ^= 0xFFFFFFFFL;

	sCRC[0] = (uint8_t)(ulRSL>>24);
	sCRC[1] = (uint8_t)(ulRSL>>16);
	sCRC[2] = (uint8_t)(ulRSL>>8);
	sCRC[3] = (uint8_t)(ulRSL);
}
*/
// 计算LRC
uint8_t CalcLRC(uint8_t *psData, UINT32 uiLength, uint8_t ucInit)
{
	while( uiLength>0 )
	{
		ucInit ^= *psData++;
		uiLength--;
	}

	return ucInit;
}

/*
// 转换为异步发送数据格式,并存储在sg_sWorkBuf中
INT32 Conv2AsyncTxd(uint8_t *psTxdData, UINT32 uiDataLen)
{
	if( uiDataLen>LEN_WORKBUF )
	{
		return ERR_COMM_TOOBIG;
	}

	sg_sWorkBuf[0] = STX;
	sg_sWorkBuf[1] = 0x80;
	sg_sWorkBuf[2] = (uint8_t)(uiDataLen>>8);
	sg_sWorkBuf[3] = (uint8_t)(uiDataLen & 0xFF);
	memcpy(&sg_sWorkBuf[4], psTxdData, uiDataLen);
	CalcCRC32(&sg_sWorkBuf[1], (UINT32)(uiDataLen+3), &sg_sWorkBuf[4+uiDataLen]);

	return 0;
}
*/


// 通讯模块初始化
int CommResetModule(ST_COMM_CONFIG *pstCfg)
{
	int  iRet;
	if( pstCfg->ucCommType!=CT_GPRS && pstCfg->ucCommType!=CT_CDMA )
	{
		return 0;
	}
	
	CommSetCfgParam(pstCfg);
	
	iRet =  WirelessReset(&pstCfg->stWirlessPara);
	return iRet;
}


int     CommCheckModemModule(ST_COMM_CONFIG *pstCfg)
{
	int  iRet;
	if( pstCfg->ucCommType!=CT_MODEM || sg_iModemfd >= 0 )
	{
		return 0;
	}
	
	iRet = CommInitModule(pstCfg);

	return iRet;
}

void    SetPreDialDisplayFlag(int idisplayflag)
{
	sg_PredialdisplayMsgFlag = idisplayflag;
}

int     CheckPreDialDisplayFlag(void)
{
	return sg_PredialdisplayMsgFlag;
}
// end of file
