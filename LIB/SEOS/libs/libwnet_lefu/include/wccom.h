/**
 * @file    wccom.h
 *          
 * @author  Tim.zhang 
 *          
 * @date    2014-06-23
 *          
 * @brief   
 *          
 * @note    
 *          
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 *
 */

#ifndef __WC_COM_H__
#define __WC_COM_H__

#include <stdint.h>
#include "wnet.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PPP_CONNECT_SHORT	1
#define SUCCESS		0
#define FAIL		-1
#define CANCEL		-2	//按键取消
#define TIMEOUT		-3  //超时
#define PARAM_ERR  -4	//参数不对


#define VERSION_LEN		30

//挂断模式枚举
typedef enum /*挂断模式*/
{
	HANGUP_TCP_PPP,		/*挂断长连接PPP链路*/
	HANGUP_TCP,		/*挂断短连接 TCP连*/
	
}HangUpMode;

/* 
 * 注意：通信模块状态是一个32位数据，
 * 最高位为1表示不是一个稳定状态，为0表示是一个稳定状态
 * STATE_BEGIN、STATE_END、STATE_PPP_READY是稳定状态，
 * 其它状态都是非稳定状态，非稳定状态会不停变迁，最终变为稳定状态。
 * 状态的低31位是状态定义，见下面枚举类型定义
 */
typedef enum _E_PPPSTATUS
{
	STATE_BEGIN=0,		// 初始状态
	STATE_POWER_ON,		// 模块上电
	STATE_POWER_OFF,	// 模块下电
	STATE_INITIALIZE,	// 模块初始化
	STATE_CHECK_SIM_1,	// 检查sim卡状态1
	STATE_CHECK_SIM_2,	// 检查sim卡状态2
	STATE_PPP_CHECK,	// 检查ppp
	STATE_PPP_CLOSE_1,	// 关闭ppp状态1
	STATE_PPP_CLOSE_2,	// 关闭ppp状态2
	STATE_PPP_OPEN,		// 打开ppp
	STATE_PPP_READY,	// ppp链路就绪
	STATE_ATTACH_1, 	// 网络附着状态1
	STATE_ABNORMAL,		// 模块异常错误
	STATE_END,			// 异常终止状态
	STATE_PPP_SHORT_CONNET,	//短连接
}E_PPPSTATUS;



/*显示的信息情况*/
typedef enum _E_DISPLAY_INFO
{
	DISP_POWERONING 		= 2000,				/*正在上电中*/
	DISP_POWERONSUCCESS 	= 2001,			/*上电成功*/	
	DISP_POWERONFAILED	= 2002,			/*上电失败*/
	DISP_POWERON_OVERTIMES,		/*上电超过3次失败*/
	DISP_POWERDOWNING, 			/*正在下电*/
	DISP_POWERDOWNSUCCESS,		/*下电成功*/
	DISP_POWERDOWNFAILED,			/*下电失败*/
	DISP_WNETINTING,				/*wnet初始化中*/
	DISP_WNETINITSUCCESS,			/*wnet初始化成功*/
	DISP_WNETINITFAILED,			/*wnet初始化失败*/
	DISP_CHECKSIMING		=2010,				/*正在检查sim卡状态*/
	DISP_CHECKSIM_TIMOUT,			/*检查sim卡失败*/
	DISP_CHECKSIM_NOINSTER,		/*Sim卡没有插入*/
	DISP_INPUTPIN_ERROR,			/*输入pin错误*/
	DISP_INPUTPIN_OK,				/*输入pin正确*/
	DISP_NO_SIMPIN			=2015,				/*没有simpin*/
	DISP_PUK_ERROR,				/*PUK错误*/
	DISP_SIMCARD_OK,				/*sim卡状态OK*/		
	DISP_SIMCARD_FAILED,			/*检查sim卡失败*/
	DISP_ATTACHING,				/*正在网络附着*/
	DISP_ATTACH_TIMEOUT,			/*网络附着超时失败*/
	DISP_ATTACH_OK,				/*网络附着OK*/
	DISP_ATTACH_FAILED,			/*网络附着失败*/
	DISP_PPPCHECKING,				/*PPP链路检查中*/
	DISP_PPPCHECK_TIMEOUT,		/*PPPcheck超时*/
	DISP_PPPCHECK_OK = 2025,				/*PPP检查OK*/
	DISP_PPP_BUILDING,			/*PPP正在链接*/
	DISP_PPPCHECK_FAILED,			/*ppp检查失败*/
	DISP_PPPCLOSEING,				/*正在关闭PPP链路*/
	DISP_PPPCLOSE_OK,				/*正常关闭ppp链路*/
	DISP_PPPCLOSE_FAILED,			/*PPP链路关闭失败*/
	DISP_PPPOPENING = 2031,				/*正在打开PPP链路*/
	DISP_PPPOPEN_OVERTIMES,		/*ppp打开超过次数限制*/
	DISP_PPPOPEN_OK,				/*PPP打开OK*/
	DISP_PPPOPEN_FAILE,			/*PPP打开失败*/
	DISP_PPPDETACHING,			/*PPP链路正在去附着*/
	DISP_PPPDETACH_FAILED,		/*PPP网络去附着失败*/
	DISP_PPPDETACH_OK,			/*PPP网络去附着OK*/
	DISP_PPPDETACH_OVERTIMES = 2038,	/*网络去附着超过次数*/
	DISP_PPP_SHORT_CONNECT,			
	DISP_PPP_END,
}E_DISPLAY_INFO;

typedef enum	{
	CONNECT_FIRST_IP = 1,		//正在连接第一个ip
	CONNECT_FIRSTIP_TIMEOUT,	//第一个ip连接超时
	CONNECT_FIRSTIP_FAILED,	//第一个ip连接失败
	CONNECT_SECOND_IP,		//正在连接第二个ip
	CONNECT_SECONDIP_TIMEOUT,	//第二个IP连接超时
	CONNECT_SECONDIP_FAILED,		//第二个IP连接失败

	CONNECT_SUCCESS,				//IP连接成功
}E_CONNECT_DISPLAY;

typedef enum
{
	E_WNET_GPRS = 1,
	E_WNET_CDMA = 2,
}E_WNET_TYPE;


typedef enum {
	E_WNET_AUTO_RESTRAT = 1,	//启动重启模式
	E_WNET_RENEW_PPPOPEN,		//重新pppopen

}E_WNET_MODE;

typedef enum {
	E_COMM_WIRELESS = 1,	//gprs通讯
	E_COMM_TCPIP,			//TCPIP方式
}E_COMM_TYPE;

typedef enum {
	E_COMM_CONNECT_PPP = 1,
	E_COMM_CONNECT_TCPIP,
	E_COMM_CONNECT_WIFI,
}E_COMM_CONNECT;

typedef struct {
	uint32_t			(*disp_currentstate)(E_DISPLAY_INFO edisplay, int ierror);
	uint32_t			(*wait_key)(void);
} WnetCallback_t;

typedef uint32_t (*RecvDataTime)(uint32_t uiTimeOut);
typedef uint32_t (*DispWnitTime)(uint32_t uiTimeOut);


typedef struct{
	char 	acSimPin[50];
	char 	acApn[64];
	char    acDialNum[64];
	char    acUserName[128];
	char    acPassWord[128];
	uint32_t	uiAuth;
	E_WNET_MODE	eWnetMode;
}WnetPPPInfo_t;

typedef struct {
	char	HostDns[50];   /*服务器域名*/
	char 	HostIp[2][16]; /*服务器IP地址*/
	int		HostPort[2];
	int     iDnsMethod;     /*连到服务器为DNS*/
	int		iTimeout;		/*connect超时时间*/
	int 	iCurrentIp;	/*返回的连接成功的ip*/
	E_COMM_TYPE 	iCurrentComm;	/*当前的通讯方式*/
	int		(*wnet_conncet_display)(E_CONNECT_DISPLAY edisplay, int ierror);	//显示回调函数
	uint32_t			(*wait_key)(void);
	E_COMM_CONNECT	eCurrentComm;
}Wnet_TcpipInfo_t;

#define WC_STATE_MASK			0x7fffffff
#define WC_STATE_BUSY			(~WC_STATE_MASK)
#define wc_is_state_busy(state)	((state) & WC_STATE_BUSY)


int wc_start_wnet(void);								//启动wnet进程
int wc_param_set(WnetPPPInfo_t *wnetinfo);						//设置wnet参数pin用户名密码等
int wc_get_state(int *state,  int *idispinfo, int *ierror);		//获取状态机状态、显示的信息以及错误码
int wc_set_state(int newstate);									//设置新的状态机状态
int wc_wait_ppp_ready(WnetCallback_t *wc, RecvDataTime RecvTimeDisp,DispWnitTime WnitTimeDisp);
void wc_stop_wnet(void);										//强制停止PPP链路

int wc_tcpip_connect(Wnet_TcpipInfo_t *pstTcpipInfo);
int wc_send_data(int iHandle, const void *psData, int iDataLen);
int wc_recv_data(int iHandle,	void *psData,int iDataLen,int iTimeOut, RecvDataTime RecvTimeDisp);
int wc_ppp_close(int HandleId,int Type);
void  wc_ready_pppclose(void);
int wc_get_ppp_auth(int *piPPPAuth);
int wc_get_wirelesstype(void);
int wc_get_basestationinfo(tBaseStation *psBaseStationInfo, unsigned int uiTimeOut);
int wc_get_neighbourbasestationinfo(tBaseStation *psBaseStationInfo, int iNum, int *piOutNum);
int wc_get_simid(char *psSimID, int iLen);
int wc_get_libversion(char *pszVer);
int wc_get_wnetversion(char *pszVer);







#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __WNET_CLASS_H__ */

