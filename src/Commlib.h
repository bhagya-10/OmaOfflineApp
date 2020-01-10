
/****************************************************************************
NAME
	commlib.h - 实现公共通讯函数

DESCRIPTION
	1. 实现POS应用开发中常见通讯方式的连接的建立、断开、数据收发等功能.
	2. 本模块内部占用定时器4#(进入模块后申请,退出模块后释放)

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	pyming     2008.09.05      - created
****************************************************************************/

#ifndef _COMMLIB_H
#define _COMMLIB_H

#include "modem_iface.h"
#include "modem.h"

#define MASK_COMM_TYPE			0xFF00000		// 提取错误类别
#define MASK_ERR_CODE			0x00FFFFF		// 提取错误代码

#define ERR_COMM_MODEMPPP_BASE	0x2000000		// ModemPPP类错误
#define ERR_COMM_RS232_BASE		0x100000		// RS232类错误
#define ERR_COMM_MODEM_BASE		0x200000		// Modem类错误
#define ERR_COMM_TCPIP_BASE		0x400000		// TCPIP类错误
#define ERR_COMM_WIRELESS_BASE	0x800000		// GPRS/CDMA类错误
#define ERR_COMM_WIFI_BASE      0x4000000		// WIFI类错误

// 公共类型错误
#define ERR_COMM_ALL_BASE		0x0000		// 公共类型错误
#define ERR_COMM_INV_PARAM		0x0001		// 参数错误
#define ERR_COMM_INV_TYPE		0x0002		// 无效通讯类型
#define ERR_COMM_CANCEL			0x0003		// 用户取消
#define ERR_COMM_TIMEOUT		0x0004		// 通讯超时
#define ERR_COMM_COMERR			0x0005
#define	ERR_COMM_TOOBIG			0x0006
#define ERR_COMM_STATUS_TIMEOUT 0x0007      // 获取状态超时
#define ERR_COMM_MODEM_INIT     0x0008      // 初始化失败
#define ERR_COMM_MODEM_NOPARAM  0x0009      // 设置参数失败
#define ERR_COMM_NOT_ATTACHED   0x000A      // 没有附着成功

// 拨号方式
#define PREDIAL_MODE		0		// 预拨号/预连接
#define ACTDIAL_MODE			1		// 实际拨号/连接

// 发送数据方式
#define CM_RAW				0		// 原数据发送方式(不添加起止字符,For RS232)
#define CM_SYNC				1		// 同步
#define CM_ASYNC			2		// 异步

// for RS232 communication
#define STX             0x02
#define ETX             0x03
#define ENQ             0x05
#define ACK             0x06
#define NAK             0x15

// PSTN配置参数
typedef struct _tagST_PSTN_PARA
{
	uint8_t		        ucSendMode;		     // 数据发送方式
	uint8_t             szTxnTelNo[100+1];   // 拨号号码
	ModemDialParms_t    stDialPara;          // modem拨号参数
}ST_PSTN_PARA; 

// RS232配置参数
typedef struct _tagRS232_INFO
{
	uint8_t	ucPortNo;			// Port #, COM1, COM2 ....
	uint8_t	ucSendMode;			// 数据发送方式
	uint8_t	szAttr[20+1];		// 串口打开参数, "9600,8,n,1", ....
}ST_RS232_INFO;

// TCP/IP配置参数
typedef struct _tagST_TCPIP_INFO
{
	uint8_t	szNetMask[15+1];
	uint8_t	szGatewayIP[15+1];
	uint8_t	szLocalIP[15+1];
	uint8_t	szRemoteIP[15+1];
	uint8_t	szRemotePort[5+1];
	uint8_t	szDNS[32+1];
}ST_TCPIP_INFO;


// GPRS/CDMA configurations
typedef struct _tagST_WIRELESS_INFO
{
	uint8_t		szAPN[64+1];  ///拨号号码 CDMA: #777; GPRS: cmnet
	uint8_t		szUID[64+1];
	uint8_t		szPwd[16+1];
	uint8_t		szSimPin[16+1];     // SIM card PIN
	uint8_t		szDNS[32+1];
	uint8_t		szRemoteIP[20+1];
	uint8_t		szRemotePort[5+1];
}ST_WIRELESS_INFO;

// WIFI  AP 配置参数
typedef struct _tagST_WIFI_AP_INFO
{
	int         iChannel;                  //  Wireless LAN communication channel(0 - 13)－－通讯信道
	int         iSecurityType;             //  Sets the Wireless LAN security type. －－ 加密模式
	int         iWEPKeyIdx;                //  WEP key index (1 - 4) -- WEP密钥组
    char        szListWEPKey[4][32];       //  4 sets of WEP Key  --- 密钥组密钥
	char        szWPAPSK[64];              //  Personal Shared Key Pass-Phrase,
	char        szSSID[64];                //  SSID
}ST_WIFI_AP_INFO;

// 通讯模块错误信息
typedef struct _tagCOMM_ERR_MSG
{
	uint8_t		szChnMsg[16+1];		// 中文错误信息
	uint8_t		szEngMsg[16+1];		// 英文错误信息
}COMM_ERR_MSG;


#define CT_RS232	0x01			// RS232通讯
#define CT_MODEM	0x02			// Modem通讯
#define CT_TCPIP	0x04			// TCP/IP通讯
#define	CT_CDMA		0x08			// CDMA通讯
#define CT_GPRS		0x10			// GPRS通讯
#define CT_WIFI     0x20            // WIFI通讯
#define CT_ALLCOMM  0x3F            // 所有的通讯类型

// 通讯模块配置信息
typedef struct _tagST_COMM_CONFIG
{
	uint8_t			  ucCommType;		    // 通讯类型(RS232/modem/tcp...)    
	ST_RS232_INFO	  stRS232Para;	        // RS232 配置参数
	ST_TCPIP_INFO	  stTcpIpPara;	        // TCP 配置参数
	ST_WIRELESS_INFO  stWirlessPara;        // GRPS/CDMA无线通讯配置参数
	ST_PSTN_PARA	  stPSTNPara;		    // modem 配置参数
	ST_TCPIP_INFO	  stWIFIIpPara;	        // WIFI 通讯模式配置参数
}ST_COMM_CONFIG;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************
 Function:		通讯模块初始化(只适用于无线模块)
 Param In:
	pstComCfg	参数信息
 Param Out:		none
 Return Code:
	0			OK
	other		fail
****************************************************************************/
INT32 CommInitModule(ST_COMM_CONFIG *pstCfg);

/****************************************************************************
 Function:		设置通讯模块参数
 Param In:
	pstComCfg	参数信息
 Param Out:		none
 Return Code:
	0			OK
	other		fail
****************************************************************************/
INT32 CommSetCfgParam(ST_COMM_CONFIG *pstCfg);

/****************************************************************************
 Function:		通讯模块拨号(MODEM拨号或者TCP建立TCP连接等)
 Param In:
	ucDialMode	拨号标志,PREDIAL_MODE:拨号后立刻返回, ACTDIAL_MODE: 链路建立后返回
 Param Out:		none
 Return Code:
	0			OK
	other		fail
****************************************************************************/
INT32 CommDial(uint8_t ucDialMode);

/****************************************************************************
 Function:		通讯模块发送数据
 Param In:
	psTxdData	待发送的数据
	uiDataLen	待发送数据的实际长度(以字节为单位)
	ulTimeOut	发送超时,以100毫秒计(RFU)
 Param Out:		none
 Return Code:
	0			OK
	other		fail
****************************************************************************/
INT32 CommTxd(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut);

/****************************************************************************
 Function:		通讯模块接收数据
 Param In:
	uiExpLen	期望接收的最大数据长度(以字节为单位)
	ulTimeOut	等待接收数据的超时时间,以100毫秒计
 Param Out:
	psRxdData	接收到的数据
	puiOutLen	接收到的实际数据长度
 Return Code:
	0			OK
	other		fail
****************************************************************************/
INT32 CommRxd(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen);

/****************************************************************************
 Function:		通讯模块断开链路(MODEM挂机或者TCP断开TCP连接等)
 Param In:
	bReleaseAll	是否完全断开链路层连接, TRUE: 断开, FALSE: 断开应用数据链路
				(RFU for 无线模块)
 Param Out:		none
 Return Code:
	0			OK
	other		fail
****************************************************************************/
INT32 CommOnHook(uint8_t bReleaseAll);

/****************************************************************************
 Function:			获取通讯错误信息
 Param In:
	iErrCode		其他通讯函数返回的错误代码
	pstCommErrMsg	错误信息(包括中英文)
 Param Out:			none
 Return Code:		none
****************************************************************************/
void CommGetErrMsg(INT32 iErrCode, COMM_ERR_MSG *pstCommErrMsg);

//  获取当前正在使用的套节字
int  GetCurSocket(void);

/****************************************************************************
 Function:			初始化WIFI模块，连接AP本获取本地IP地址
 Param In:
 Param Out:			none
 Return Code:		0---初始化成功;  其他--－失败
****************************************************************************/
INT32   InitWiFiModule(void);

/****************************************************************************
 Function:			获取WIFI模块是否连接AP本获取本地IP地址，该函数和上面
                    InitWiFiModule函数类似，只是该函数只是获取状态，在不成功时
					不会提示用户修改WIFI的配置参数。
 Param In:
 Param Out:			none
 Return Code:		0---初始化成功;  其他--－失败
****************************************************************************/
INT32   GetWiFiStatus(void);

int    DispWirelessSignal(void);

int     CommResetModule(ST_COMM_CONFIG *pstCfg);

int     CommCheckModemModule(ST_COMM_CONFIG *pstCfg);

int     CheckWirelessAttached(uint32_t uiTimeOut);

void    SetPreDialDisplayFlag(int idisplayflag);

int     CheckPreDialDisplayFlag(void);
int RS232RawTxd(uint8_t *psTxdData, uint32_t uiDataLen, uint32_t uiTimeOut);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _CommLIB_H

// end of file
