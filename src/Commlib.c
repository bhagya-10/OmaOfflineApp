
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
static UINT32       sg_uiWirePPPLogoutTime = 0; // �洢ǰһ��ppp�ɹ��Ҷϵ�ʱ�䣬�Ӷ���֤��3��ļ���ſ�ʼ�µ�ppp���Ŷ���
static UINT8        sg_ucWirePPPFlag = 1; // ��ʾ����ppp���ӵ�״̬ 0��ʾ�ѹҶ� 1��ʾδ�Ҷ� -- 8210��һ��Ҳ�ܹҶϣ�������û��ppplogin������²��ܹر�ģ��
static UINT8        sg_pppWaitFlag = 0; // 1��ʾ ppp ���ź���Ҫ��ʱ
static int		    sg_PredialdisplayMsgFlag = 0;    //Ԥ�����Ƿ�����Ļ����ʾ��Ϣ������8210���̴߳�������ʾ��8110��Ҫ��ʾ
static int          sg_InitModulRet = 0 ;   // ���������߳��ڴ����ʼ���������һ���������ֳ�ʼ��ֵ, Ĭ�ϳɹ��������һ�����������̳߳�ʼ��
//static struct sockaddr     sg_stWiresockaddr;
static struct sockaddr     sg_stTcpsockaddr;
//static struct sockaddr     sg_stModemPPPsockaddr; //***MODEMPPP**

static ST_COMM_CONFIG	sg_stCurCfg = {0xFF};
static uint8_t		sg_sWorkBuf[LEN_WORKBUF+50];
static ERR_INFO		sg_stCommErrMsg[] =
{
	{ERR_COMM_INV_PARAM, "��������",     "INVALID PARAM"},
	{ERR_COMM_INV_TYPE,  "��ЧͨѶ����", "INV COMM TYPE"},
	{ERR_COMM_CANCEL,    "�û�ȡ��",     "USER CANCEL"},
	{ERR_COMM_TIMEOUT,   "ͨѶ��ʱ",     "TIMEOUT"},
	{ERR_COMM_COMERR,    "ͨѶ����",     "COMM ERROR"},
	{ERR_COMM_TOOBIG,    "���ݰ�̫��",   "DATA TOO BIG"},
	{0, "", ""},
};

static ERR_INFO		sg_stRS232ErrMsg[] =
{
	{PORT_RET_NOTEMPTY,    "���ͻ�������", "OVERFLOW"},
	{PORT_RET_PORTERR,     "��Ч�˿ں�",   "INVALID PORT"},
	{PORT_RET_NOTOPEN,     "�˿�δ��",   "PORT CLOSED"},
	{PORT_RET_BUSY,        "�޿��ö˿�",   "NO PORT AVAIL"},
	{PORT_RET_MODEM_INUSE, "�˿ڱ�ռ��",   "PORT OCCUPY"},
	{PORT_RET_PARAERR,     "��ЧͨѶ����", "INVALID PARAM"},
	{PORT_RET_TIMEOUT,     "���ڳ�ʱ",     "TIMEOUT"},
	{0, "", ""},
};

static ERR_INFO		sg_stPPPErrMsg[] =
{
	{NET_ERR_RETRY,        "ͨѶ��ʱ",        "TIMEOUT"},
	{ERR_COMM_NOT_ATTACHED,  "ģ�鸽��ʧ��",        "ATTACHED ERROR"},
	{NET_ERR_RSP,            "��Ӧ��,���ģ��",     "CHECK MODULE"},
	{NET_ERR_NOSIM,          "SIM��������",         "NO SIM CARD"},
	{NET_ERR_PIN,            "��Ҫ����PIN��",       "NEED SIM PIN"},
	{NET_ERR_PUK,            "��Ҫ����PUK��",       "NEED SIM PUK"},
	{NET_ERR_PWD,            "PIN�����",           "SIM PIN ERROR"},
	{NET_ERR_SIMDESTROY,     "SIM������",           "NO SIM/NEED PIN"},
	{NET_ERR_CSQWEAK,        "�ź�̫��",            "SIGNAL TOO WEAK"},
	{NET_ERR_LINKCLOSED,     "�������ز�",          "NO CARRIER"},
	{NET_ERR_LINKOPENING,    "���ӳɹ�",            "LINK OK"},
	{NET_ERR_ATTACHING,      "���ڲ�������",        "SEEKING NETWORK"},
	{NET_ERR_DETTACHED,      "�ѶϿ���������",      "DETTACH NETWORK"},
	{NET_ERR_EMERGENCY,      "���SIM��",           "PLS CHECK SIM"},
	{NET_ERR_RING,           "�ȴ�������",          "ACCEPTED ERR"},
	{NET_ERR_BUSY,           "����ͨѶ��",          "COMMINICATE ERR"},
	{NET_ERR_DIALING,        "ģ�����ڲ���",        "LINKING"},
	{NET_ERR_UNKNOWN,        "δ֪������",          "UNKNOW ERR"},
	{NET_ERR_ABNORMAL,       "δ֪������",          "UNKNOW ERR"},
	{-WNET_DLOPEN_ERR,       "��ʼ������(open)",    "open lib err"},
	{-WNET_DLSYM_ERR,        "��ʼ������(lib)",     "get point err"},
	{-WNET_INIT_ERR,         "��ʼ������",          "wnet init err"},
	{-WNET_NOINIT_ERR,       "ģ��û�г�ʼ��",      "wnet do not init"},
	{-WNET_PARAM_ERR,        "���߲�������",        "param err"},
	{-WNET_GET_OP_ERR,       "ͨѶ����err",         "get struct err"},
	{-WNET_NO_FUN_ERR,       "ͨѶ����call",        "function error "},
	{-WNET_POWER_DOWN_ERR,   "ͨѶģ���µ����",    "went powerdown "},
	{-WNET_BEEN_USE_ERR,     "ģ�鱻ռ��",          "wnet userd now"},
	{-WNET_SUSPEND_ERR,      "ģ�鱻����",          "wnet suspend now"},
	{-WNET_NO_MODULE_ERR,    "���ͨѶģ��",        "not wnet module"},
	{-WNET_RECV_TIMEOUT_ERR, "ģ��ͨѶ��ʱ",        "receive timeout"},
	{-WNET_USER_CANCEL_ERR,  "�û�ȡ��",            "user cancel"},
	{-WNET_RECV_DATA_ERR,    "ģ����ճ�ʱ",        "not used"},
	{-WNET_MOD_RESP_ERR,     "ģ����մ���",        "receive data err"},
	{-WNET_SEND_DATA_ERR,    "ģ�鷢�ʹ���",        "send data  err"},
	{-WNET_COMM_OPEN_ERR,    "��ģ���豸����",    "port open err"},
	{-WNET_SMS_DIV_ERR,      "ͨѶ����(SMS)",       "not used"},
	{-WET_DETTACHED_ERR,     "�����������",        "Dettached"},
	{-WNET_ATTACHING_ERR,    "���Ż�վ����",        "Dettached error"},
	{-WNET_ABNORMAL_ERR,     "δ֪������err",       "abnormal error"},
	{-WNET_CSQWEAK_ERR,      "�ź�̫��",            "signal too weak"},
	{-WET_EMERGENCY_ERR,     "SIM��״̬����",       "SIM/UIM error"},
	{-WET_RING_ERR,          "�ȴ�������",          "detect ringing"},
	{-WET_BUSY_ERR,          "����ͨѶ��",          "detect call err"},
	{-WET_DIALING_ERR,       "ģ�����ڲ���",        "dialing"},
	{0, "", ""},
};

//#define	EINTR		 4	/* Interrupted system call */
//#define	EIO		 5	/* I/O error */
#ifndef ETIME
#define  ETIME   62
#endif
static ERR_INFO	sg_stModemErrMsg[] =
{
	{-MODEM_ERRNO_ERROR,             "�豸�쳣",       "MODEM ERROR"},
	{-MODEM_ERRNO_BUSY,              "�� �� æ",       "MODEM BUSY"},
	{-MODEM_ERRNO_NO_DIALTONE,       "�޲�����",       "NO DIAL TONE"},
	{-MODEM_ERRNO_NO_CARRIER,        "��·�ز���ʧ",   "LINE BREAK"},
	{-MODEM_ERRNO_NO_LINE,           "��Ӻõ绰��",   "LINE READY ?"},
	{-MODEM_ERRNO_NO_ANSWER,         "������Ӧ��",     "NO ACK"},
	{-MODEM_ERRNO_OFF_HOOK,          "���ȹһ�",       "PLS ONHOOK"},
	{-MODEM_ERRNO_LINE_IN_USE,       "������·æ",     "LINE BUSY"},
	{-MODEM_ERRNO_UN_OBTAINABLE,     "��·���ɵ���",   "UNOBTAINABLE"},
	{-MODEM_ERRNO_LINE_REVERSAL,     "��·��ת",       "LINE REVERSAL"},
	{-MODEM_ERRNO_EXTENSION_IN_USE,  "���õ绰ռ��",   "PHONE OCCUPIED"},
	{ERR_COMM_STATUS_TIMEOUT,        "����������ʱ",   "TIME OUT"},
	{ERR_COMM_MODEM_INIT,            "��ʼ���豸ʧ��", "MODEM INIT ERROR"},
	{NET_ERR_RETRY,                  "ͨѶ��ʱ",       "TIMEOUT"},
	{4,                              "��·�ѶϿ�",     "LINE BREAK "},
	{5,                              "��·���Ͽ�",     "LINE BREAK "},
	{16,                             "�� �� æ",       "MODEM BUSY"},
	{22,                             "��������",       "INVALID ARGUMENT"},
	{ETIMEDOUT,                      "����������ʱ",   "TIME OUT"},
	{ETIME,                          "����������ʱ",   "TIME OUT"},
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


// ͨѶģ���ʼ��
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

// ����ͨѶģ�����
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
		if( iLen<10 || iLen>20 )	// �򵥼�����
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


// ͨѶģ�鲦��(MODEM���Ż���TCP����TCP���ӵ�)
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
		if (gstPosCapability.uiPosType == 0x02) // 6110 ��WIFI��TCPһ������
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

// ͨѶģ�鷢������
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

// ͨѶģ���������
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
//		WirelessClose(FALSE);  // ���ܺ���Ҫ�Ͽ�TCP���ӣ��´�����ʱ��������
		break;

	default:
		iRet = ERR_COMM_INV_TYPE;
		break;
	}

	return iRet;
}

// ͨѶģ��Ͽ���·(MODEM�һ�����TCP�Ͽ�TCP���ӵ�)
INT32 CommOnHook(uint8_t bReleaseAll)
{
	INT32		iRet;
	INT32 Col, Line;
	UINT8 Buff[2048]; // ��ǰ��ĻΪ128*64�����鿪�ٵĻ�������СΪ2K��
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
 
// ��ȡͨѶ������Ϣ
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
		sprintf((char *)pstCommErrMsg->szChnMsg, "ͨѶ����:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "COMM ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stCommErrMsg, pstCommErrMsg);
		break;

	case ERR_COMM_RS232_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "���ڴ���:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "PORT ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stRS232ErrMsg, pstCommErrMsg);
		break;

	case ERR_COMM_MODEM_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "���Ŵ���:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "MODEM ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stModemErrMsg, pstCommErrMsg);
	    break;

	case ERR_COMM_TCPIP_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "TCPIP����:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "TCPIP ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stPPPErrMsg, pstCommErrMsg);
		break;
	case ERR_COMM_WIRELESS_BASE:
		sprintf((char *)pstCommErrMsg->szChnMsg, "���ߴ���:%04X", iReErrCode);
		sprintf((char *)pstCommErrMsg->szEngMsg, "WIRE ERR:%04X", iReErrCode);
		GetAllErrMsg(iErrCode, sg_stPPPErrMsg, pstCommErrMsg);
		break;
	default:
		sprintf((char *)pstCommErrMsg->szChnMsg, "ͨѶ����");
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
// RS232 ͨѶģ��
//////////////////////////////////////////////////////////////////////////
// ���ڴ�
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

// ���ڷ�������
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

// ����ֱ�ӷ���
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

// ���ڽ���
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

// ����ֱ�ӽ���
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
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(uiTimeOut *1000) )	// ��鶨ʱ��
			{
				if( uiReadCnt>0 )	// �Ѿ���ȡ������
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
		{	// ���ճɹ�,����
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
		{	// �ǳ�ʱ����,�˳�
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
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(uiTimeOut *1000) )	// ��鶨ʱ��
			{
				if( uiReadCnt>0 )	// �Ѿ���ȡ������
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


// ���ڹر�
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
// TCP ͨѶģ��
//////////////////////////////////////////////////////////////////////////
// ����TCP����
INT32 TcpDial(uint8_t ucDialMode)
{
	int iRet,iFlag,iRetyTimes;
	UINT32   uiOldTime;

	if( ucDialMode==PREDIAL_MODE )
	{
		return 0;
	}

    //  ʵ������
	iFlag = 1;
	iRetyTimes = 1;
	uiOldTime = sysGetTimerCount();	
	while (iFlag)
	{
		if ( sysGetTimerCount() >= (uiOldTime+60000) )
		{ // �������ӳ�ʱ60s���򷵻�
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
		{// ���β������Ӳ��ɹ���ر��׽��֣��Ͽ�PPP����
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
				lcdDisplay(0,3,DISP_CFONT,"   �� �� �� ... ");	
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
				CommOnHook(FALSE);  // �Ҷ�TCPIP��������
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

// ��������
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

// ��������
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
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// ��鶨ʱ��
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
		// BCD ��ʽ�ĳ����ֽ�
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

// �Ͽ�TCP����
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
// MODEM ͨѶģ��
//////////////////////////////////////////////////////////////////////////
#define MODEM_COMM_ERR(a)		(ERR_COMM_MODEM_BASE|(a))
// modem ����
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
				lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "  Ԥ������... ");
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
			{ // �������ӳ�ʱ60s���򷵻�
				PSTNOnHook(TRUE);
				return MODEM_COMM_ERR(NET_ERR_RETRY);
			}

			if (iDialTimes>3)  // ���β��ɹ����˳�ʧ��
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
				if (sg_iModemfd < 0 )  // ���modem�����Ч�����ȴ�
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

			case MODEM_STATE_SYNC_MODE:  // ��״̬��ʾ��������,��Ҫ�ȴ����óɹ�
			case MODEM_STATE_ASYN_MODE:  // ��״̬��ʾ��������,��Ҫ�ȴ����óɹ�
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

			case MODEM_STATE_WAITTING:  // ������״̬���ȹҶϵ�ǰ�������ӣ������²���
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
		{ // �������ӳ�ʱ60s���򷵻�
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
			lcdDisplay(0, 3, DISP_CFONT, "    �� �� ��   ");				
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

		case MODEM_STATE_SYNC_MODE:  // ��״̬��ʾ��������,��Ҫ�ȴ����óɹ�
		case MODEM_STATE_ASYN_MODE:  // ��״̬��ʾ��������,��Ҫ�ȴ����óɹ�
		case MODEM_STATE_DAILING:
		case MODEM_STATE_CONNECT_SDLC:
			sysDelayMs(100); // ��ʱ���ڲ鿴
			break;
			
		case MODEM_STATE_DISCONNECT:
			if (iDialTimes >= 3)  // ���β��ɹ����˳�ʧ��
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
			
		case MODEM_STATE_WAITTING:  // ������״̬���ȹҶϵ�ǰ�������ӣ������²���
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

// Modem ����
INT32 PSTNTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	int  iRet,iRelSendlen;
		
	for (iRelSendlen=0; iRelSendlen<uiDataLen;)
	{
		iRet = modem_write_timeout(sg_iModemfd,psTxdData+iRelSendlen,(uiDataLen-iRelSendlen),uiTimeOut);
		if (iRet < 0)
		{
			PSTNOnHook(FALSE);   // ����ʧ�ܺ�Ͽ����ӣ��´���������
			iRet = ABS(iRet);
			return MODEM_COMM_ERR(iRet);
		}
		iRelSendlen += iRet;
	}

	return 0;
}

// Modem����
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


// Modemͬ������
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
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// ��鶨ʱ��
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

// Modem�첽����
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
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// ��鶨ʱ��
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

// Modem �һ�
INT32 PSTNOnHook(uint8_t bReleaseAll)
{
	int  iRet,iStatus;

	iStatus = 0;
    if (sg_iModemfd < 0)
    {
        return 0;
    }

	modem_wait_until_sent(sg_iModemfd);

/*	// �ų�������ιҶϵ�����
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
			
			if (iStatus & 0x80000000) // Ŀǰ���λΪ1��ʾ������û��ɣ���Ҫ�ٴμ��
			{
				sysDelayMs(100);
				continue;
			}

			break;
		}
		iRet = modem_close(sg_iModemfd);
		sg_iModemfd = -1;
	} */
	
	// ������������汾�����ں�̨NACû����Ӧ�Ҷ������¹Ҷ�ʱ��ϳ�����ֱ�ӹر�modemģ��
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
		{ // ��ʱ10s���򷵻�
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
		
		if (iStatus & 0x80000000) // Ŀǰ���λΪ1��ʾ������û��ɣ���Ҫ�ٴμ��
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
		if (sg_iModemfd < 0 )  // ���modem�����Ч�����ȴ�
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
// GPRS/CDMA ͨѶģ��
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

	iCnt = 2;  // 2�μ��SIM��������ʱ2S�����ֹSIM���ϵ�ʱ�䲻��������
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

	// 8210��ppp���ź������豸���ƺ�ԭ���Ĳ�ͬ����Ҫ����GPRS,CDMA
#ifdef _POS_TYPE_8210
	if (sg_stCurCfg.ucCommType == CT_CDMA)
		iDevType = PPP_DEV_CDMA;
	else
		iDevType = PPP_DEV_GPRS;
#endif

	kbFlush();
	// Ԥ����
	if( ucDialFlag== PREDIAL_MODE )  
	{
		printf("Pre dial :%d \n",sg_InitModulRet);

		if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
		{
			if (sg_InitModulRet != WIRELESS_ERR(ERR_COMM_NOT_ATTACHED)){
				return sg_InitModulRet; // ��ʼ��ʧ��,��������ڷǸ����ɹ����򷵻�
			}
		}

		iReInitTimes = 0;
		sg_Wire_PreDial_Ret = 0;
		iRet = PPPCheck(iDevType);
		printf("Pre dial pppcheck :%d \n",iRet);
		if (iRet < 0)
		{
			if (iRet == -NET_ERR_LINKOPENING) // ���ڽ��������У�Ԥ���žͷ��سɹ�
			{
				return 0;
			}

#ifdef _POS_TYPE_8210
			iRet = -ENETDOWN;  // 8210 ��pppcheck�����0��PPP_LINK_BUILDING���͸�λģ��,Ȼ�󲦺�
#endif
			if (iRet == -ETIMEDOUT || iRet == -ENETDOWN)    // iRet == -NET_ERR_UNKNOWN || iRet == -ENETDOWN)
			{   // ��ʱ�˳����ȸ�λģ��
				if (iRet == -ETIMEDOUT){
					sg_InitModulRet = -1;
				}
				if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
				{
					if (sg_InitModulRet == WIRELESS_ERR(ERR_COMM_NOT_ATTACHED))
					{
						iRet = CheckWirelessAttached(10*1000); // Ԥ����ʱ��⸽�����ʱ���һЩ
						printf("Pre dial atattched :%d \n",iRet);
						if (sg_InitModulRet!=0 && sg_InitModulRet !=WIRELESS_ERR(0))
						{
							iRet = ABS(iRet);
							return WIRELESS_ERR(iRet);
						}
						sg_InitModulRet = 0;  // �����ɹ����ó�ʼ���ɹ�
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
			while((UINT32)(sysGetTimerCount() - sg_uiWirePPPLogoutTime) < 3000); // �ҶϺ�����3�����ϲ����ٲ���
			iRet = PPPLogin(iDevType,(char *)sg_stCurCfg.stWirlessPara.szAPN,
				            (char *)sg_stCurCfg.stWirlessPara.szUID,
							(char *)sg_stCurCfg.stWirlessPara.szPwd,PPP_ALG_CHAP,60000);
			sg_ucWirePPPFlag = 1;	// ÿ��������PPP���ŵĶ������������ɹ���񣬶�������һ�ιҶ�ppp�Ķ���			
			iRet = ABS(iRet);
			sg_Wire_PreDial_Ret = iRet;
			sg_pppWaitFlag = 1; // ppp ���ź���Ҫ��ʱ
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
				WirelessReset(&sg_stCurCfg.stWirlessPara); //������ζ�ʧ����λģ��
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
				{// ��������ڸ���û�гɹ���������������ģ��
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
						return iRet; // ��ʼ��ʧ���򷵻�
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
			//��δ����ƽ�����ԭ���ǣ����ڳ�ʱ�ж��Ѿ��ƽ����ˣ�����Ϊ�˷��ӳ�ʱ�жϵ����ң��������δ���Ҳ�ƽ�����
			if (iRet == -NET_ERR_LINKOPENING)
			{
				sysDelayMs(400);
				continue;
			}
		
#ifdef _POS_TYPE_8210
			iRet = -ENETDOWN;  // 8210 ��pppcheck�����0��PPP_LINK_BUILDING���͸�λģ��,Ȼ�󲦺�
#endif
			CommOnHook(FALSE);// �ȹҶ�����
			if (iRet == -ETIMEDOUT || iRet == -ENETDOWN)    // iRet == -NET_ERR_UNKNOWN || iRet == -ENETDOWN)
			{   // ��ʱ�˳����ȸ�λģ��
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
			sysDelayMs(3000); // �ҶϺ�����3�����ϲ����ٲ���
#endif
			iDialTimes++;
			iRet = PPPLogin(iDevType,(char *)sg_stCurCfg.stWirlessPara.szAPN,
				            (char *)sg_stCurCfg.stWirlessPara.szUID,
							(char *)sg_stCurCfg.stWirlessPara.szPwd,PPP_ALG_CHAP,60000);
			sg_ucWirePPPFlag = 1; // ÿ��������PPP���ŵĶ������������ɹ���񣬶�������һ�ιҶ�ppp�Ķ���
			if (iRet<0)
			{
				iRet = -iRet;
				
				if (iDialTimes > 3)
				{
					//WirelessClose(TRUE);
					WirelessReset(&sg_stCurCfg.stWirlessPara); // ���β��ɹ���λģ��
					return WIRELESS_ERR(NET_ERR_RETRY);
				}
			}
			sg_Wire_PreDial_Ret = iRet;
			sysDelayMs(1000);  // ppplogin����ʱ1s������pppcheck���ش��󣬻����ppplogin
			sg_pppWaitFlag = 1; // ppp ���ź���Ҫ��ʱ
			continue;
		}
		iTimeoutTimes =0; //�ɹ����ó�ʱ����Ϊ0
#ifndef _POS_TYPE_8210
		// ����Ŀǰʹ�õ��׽��������0,1,2 . �������sg_Tcpsocketֵ�쳣���Ƚ��丳��ֵ(�Ҷ�)
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
                 sysDelayMs(2000); // ppp���ź����½���TCP����֮ǰ��ʱ2s���ڽ��д�����ǿ���ض������µķ��ͳɹ���
                 sg_pppWaitFlag = 0;
            }
            
			//  ʵ������
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
			��δ�����ô�޸ĵ�ԭ���ǣ�����tcp���ӳ��Դ����Ƿ��޵��ж��Ѿ����ں��棬���ԣ������������޸ġ�
			��tcp����ʧ�ܣ���Ȼ��ִ�к���Ĵ����жϴ��롣
			***/						
			if (NET_OK == iRet)
			{
				sysDelayMs(200);  //  ���ض�����£�connect����ʱ�ڽ��з���
				return iRet;
			}

			/***
			������δ����ԭ����: ������-1043ʱ��˵���û���ȡ����ȡ��TCP���ӣ���ôӦ���˳�ͨѶ��
			***/
			if (-NET_ERR_USER_CANCEL == iRet)
			{
				return ERR_USERCANCEL;
			}	
		}
		else
		{ 
			// Ŀǰ�����ܼ����׽����Ƿ񻹴������ӣ������У�ֻ��Ӧ���Լ��ϸ�????????
			return 0;
		}
		
		/***
		��δ����Ƶ������ԭ���ǣ���Ȼ���һ�γ���tcp���Ӷ����ɹ��ˣ���������Ҫѭ������ͷȥ����ֱ���˳�������
		���ң���δ�����ǰ���һ��Σ���ǣ�������һ�γ���tcp����ʧ�ܣ�ѭ����ͷ����pppcheck��飬���pppcheck���ط�0����ô������ȥ��ppplogin����ʱ����ֱ��
		ppp���ӽ��������ˣ�ȴ����δ����˳��˺��������������ù�������ʱ�䡣
		***/
		if (iRetyTimes > 3 )
		{// ���β������Ӳ��ɹ���ر��׽��֣��Ͽ�PPP����
			WirelessClose(TRUE);
			return WIRELESS_ERR(NET_ERR_RETRY);
		}
		else /***��δ�����ǰ��tcp����ʧ�ܺ�Ĵ����Ƶ���������***/
		{
			CommOnHook(FALSE);  // �Ҷ�TCPIP��������
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
#ifndef _POS_TYPE_8210   // ģ��ϵ��gsmMuxd����رգ�reset��������ʧ�ܣ���ʱ������Ҫ��ģ���ϵ縴λ
		iRet = ABS(iRet);
		return WIRELESS_ERR(iRet);
#endif
	}
//	lcdFlip();
//	sysDelayMs(5000); // ��λ����ʱ5s
	
	return InitWirelessModule(pstParam);
}


//����ͨѶ����
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
		if( (iNowTime-iOldTime) >= (UINT32)(uiTimeOut *1000) )	// ��鶨ʱ��
		{
			iRet = ERR_COMM_TIMEOUT;
			break;
		}
		
		lcdDisplay(187,4,DISP_CFONT,"(%d)",(iNowTime-iOldTime)/1000);
		lcdFlip();
		iRet = NetRecv(sg_Tcpsocket,psRxdData+ulRealRecvLen,uiExpLen,0);
		if (iRet > 0)
		{
		    // BCD ��ʽ�ĳ����ֽ�
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

// ����ͨѶ����
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
			WirelessClose(TRUE);   // ����ʧ�ܺ�Ͽ����ӣ��´���������
			iRet = -iRet;
			return WIRELESS_ERR(iRet);
		}
		iRelSendlen += iRet;
	}

	return 0;
}

// ����ģ��һ�
INT32 WirelessClose(uint8_t bRelease)
{
	int  iRet = 0, iCnt = 0;
	int        iDevType = OPT_DEVWNET;
	
	// 8210��ppp���ź������豸���ƺ�ԭ���Ĳ�ͬ����Ҫ����GPRS,CDMA
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
		for (iCnt = 0; iCnt < 2; iCnt++) //�������ر��û���ȡ��������ֹppplogout����������û������2��
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
		
		wnet_power_down();  // 8210�ر�ϵͳ��gsmMuxd����
		sg_InitModulRet = -1;  // ģ���ʼ��ʧ��
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

// ��ʾ�ź�ǿ��
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
// WIFI ͨѶģ��
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

	if (gstPosCapability.uiPosType == 0x02) // 6110 ��WIFI��TCPһ������
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
			{ // �������ӳ�ʱ10s���򷵻�
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
        // �ȼ������״̬
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
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"���Ժ�...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"WIFI������..(%d)",iRetry);
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
					PubDispString("��ȡ��ǰ����ʧ��",2|DISP_LINE_CENTER);
					PubDispString("�����ܻ�˵���",4|DISP_LINE_CENTER);
					PubDispString("����WIFI���ò���",6|DISP_LINE_CENTER);
				}
				iRet = PubWaitKey(6);
				if (iRet == KEY_FN || iRet == KEY_MENU)
				{
					iRet = SelectAndSetWIFIAP();
					if (iRetry>=3)
					{
						return  WIFI_COMM_ERR(iTemp);
					}
					sysDelayMs(2000);// ��ʱ1s
					continue;
				}
				else
				{
					return  WIFI_COMM_ERR(iTemp);
				}
			}
			break;
		}

		// ����Ƿ��ȡ����IP��ַ
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
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"���Ժ�...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"��ȡ����IP..(%d)",iRetry);
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
					PubDispString("��ȡ��ǰ����ʧ��",2|DISP_LINE_CENTER);
					PubDispString("�����ܻ�˵���",4|DISP_LINE_CENTER);
					PubDispString("����WIFI���ò���",6|DISP_LINE_CENTER);
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

	if (gstPosCapability.uiPosType == 0x02) // 6110 ��WIFI��TCPһ������
	{
		return 0;
	}

	memset(szlocalIP, 0, sizeof(szlocalIP));
	iRetry = 0;
	while(1)
	{
        // �ȼ������״̬
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
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"���Ժ�...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"WIFI������..(%d)",iRetry);
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
				sysDelayMs(2000);// ��ʱ1s
				continue;	
			}

			break;
		}

		// ����Ƿ��ȡ����IP��ַ
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
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"���Ժ�...");
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"��ȡ����IP..(%d)",iRetry);
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
	
    //  ʵ������
	iFlag = 1;
	iRetyTimes = 1;
	uiOldTime = sysGetTimerCount();	
	while (iFlag)
	{
		if ( sysGetTimerCount() >= (uiOldTime+60000) )
		{ // �������ӳ�ʱ60s���򷵻�
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
		{// ���β������Ӳ��ɹ���ر��׽��֣��Ͽ�PPP����
			TcpOnHook(FALSE);
			return (ERR_COMM_TCPIP_BASE|NET_ERR_RETRY);
		}

		// ����Ƿ�������WIFI,���ʱ�����û�����ӳɹ��Ͳ���ʾ�û�����WIFI���ö���ֱ�ӷ���
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
				lcdDisplay(0,3,DISP_CFONT,"   �� �� �� ... ");
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
				CommOnHook(FALSE);  // �Ҷ�TCPIP��������
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
// ��������ת��ģ��
//////////////////////////////////////////////////////////////////////////
// ����CRC32
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
// ����LRC
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
// ת��Ϊ�첽�������ݸ�ʽ,���洢��sg_sWorkBuf��
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


// ͨѶģ���ʼ��
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
