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
#define CANCEL		-2	//����ȡ��
#define TIMEOUT		-3  //��ʱ
#define PARAM_ERR  -4	//��������


#define VERSION_LEN		30

//�Ҷ�ģʽö��
typedef enum /*�Ҷ�ģʽ*/
{
	HANGUP_TCP_PPP,		/*�Ҷϳ�����PPP��·*/
	HANGUP_TCP,		/*�Ҷ϶����� TCP��*/
	
}HangUpMode;

/* 
 * ע�⣺ͨ��ģ��״̬��һ��32λ���ݣ�
 * ���λΪ1��ʾ����һ���ȶ�״̬��Ϊ0��ʾ��һ���ȶ�״̬
 * STATE_BEGIN��STATE_END��STATE_PPP_READY���ȶ�״̬��
 * ����״̬���Ƿ��ȶ�״̬�����ȶ�״̬�᲻ͣ��Ǩ�����ձ�Ϊ�ȶ�״̬��
 * ״̬�ĵ�31λ��״̬���壬������ö�����Ͷ���
 */
typedef enum _E_PPPSTATUS
{
	STATE_BEGIN=0,		// ��ʼ״̬
	STATE_POWER_ON,		// ģ���ϵ�
	STATE_POWER_OFF,	// ģ���µ�
	STATE_INITIALIZE,	// ģ���ʼ��
	STATE_CHECK_SIM_1,	// ���sim��״̬1
	STATE_CHECK_SIM_2,	// ���sim��״̬2
	STATE_PPP_CHECK,	// ���ppp
	STATE_PPP_CLOSE_1,	// �ر�ppp״̬1
	STATE_PPP_CLOSE_2,	// �ر�ppp״̬2
	STATE_PPP_OPEN,		// ��ppp
	STATE_PPP_READY,	// ppp��·����
	STATE_ATTACH_1, 	// ���總��״̬1
	STATE_ABNORMAL,		// ģ���쳣����
	STATE_END,			// �쳣��ֹ״̬
	STATE_PPP_SHORT_CONNET,	//������
}E_PPPSTATUS;



/*��ʾ����Ϣ���*/
typedef enum _E_DISPLAY_INFO
{
	DISP_POWERONING 		= 2000,				/*�����ϵ���*/
	DISP_POWERONSUCCESS 	= 2001,			/*�ϵ�ɹ�*/	
	DISP_POWERONFAILED	= 2002,			/*�ϵ�ʧ��*/
	DISP_POWERON_OVERTIMES,		/*�ϵ糬��3��ʧ��*/
	DISP_POWERDOWNING, 			/*�����µ�*/
	DISP_POWERDOWNSUCCESS,		/*�µ�ɹ�*/
	DISP_POWERDOWNFAILED,			/*�µ�ʧ��*/
	DISP_WNETINTING,				/*wnet��ʼ����*/
	DISP_WNETINITSUCCESS,			/*wnet��ʼ���ɹ�*/
	DISP_WNETINITFAILED,			/*wnet��ʼ��ʧ��*/
	DISP_CHECKSIMING		=2010,				/*���ڼ��sim��״̬*/
	DISP_CHECKSIM_TIMOUT,			/*���sim��ʧ��*/
	DISP_CHECKSIM_NOINSTER,		/*Sim��û�в���*/
	DISP_INPUTPIN_ERROR,			/*����pin����*/
	DISP_INPUTPIN_OK,				/*����pin��ȷ*/
	DISP_NO_SIMPIN			=2015,				/*û��simpin*/
	DISP_PUK_ERROR,				/*PUK����*/
	DISP_SIMCARD_OK,				/*sim��״̬OK*/		
	DISP_SIMCARD_FAILED,			/*���sim��ʧ��*/
	DISP_ATTACHING,				/*�������總��*/
	DISP_ATTACH_TIMEOUT,			/*���總�ų�ʱʧ��*/
	DISP_ATTACH_OK,				/*���總��OK*/
	DISP_ATTACH_FAILED,			/*���總��ʧ��*/
	DISP_PPPCHECKING,				/*PPP��·�����*/
	DISP_PPPCHECK_TIMEOUT,		/*PPPcheck��ʱ*/
	DISP_PPPCHECK_OK = 2025,				/*PPP���OK*/
	DISP_PPP_BUILDING,			/*PPP��������*/
	DISP_PPPCHECK_FAILED,			/*ppp���ʧ��*/
	DISP_PPPCLOSEING,				/*���ڹر�PPP��·*/
	DISP_PPPCLOSE_OK,				/*�����ر�ppp��·*/
	DISP_PPPCLOSE_FAILED,			/*PPP��·�ر�ʧ��*/
	DISP_PPPOPENING = 2031,				/*���ڴ�PPP��·*/
	DISP_PPPOPEN_OVERTIMES,		/*ppp�򿪳�����������*/
	DISP_PPPOPEN_OK,				/*PPP��OK*/
	DISP_PPPOPEN_FAILE,			/*PPP��ʧ��*/
	DISP_PPPDETACHING,			/*PPP��·����ȥ����*/
	DISP_PPPDETACH_FAILED,		/*PPP����ȥ����ʧ��*/
	DISP_PPPDETACH_OK,			/*PPP����ȥ����OK*/
	DISP_PPPDETACH_OVERTIMES = 2038,	/*����ȥ���ų�������*/
	DISP_PPP_SHORT_CONNECT,			
	DISP_PPP_END,
}E_DISPLAY_INFO;

typedef enum	{
	CONNECT_FIRST_IP = 1,		//�������ӵ�һ��ip
	CONNECT_FIRSTIP_TIMEOUT,	//��һ��ip���ӳ�ʱ
	CONNECT_FIRSTIP_FAILED,	//��һ��ip����ʧ��
	CONNECT_SECOND_IP,		//�������ӵڶ���ip
	CONNECT_SECONDIP_TIMEOUT,	//�ڶ���IP���ӳ�ʱ
	CONNECT_SECONDIP_FAILED,		//�ڶ���IP����ʧ��

	CONNECT_SUCCESS,				//IP���ӳɹ�
}E_CONNECT_DISPLAY;

typedef enum
{
	E_WNET_GPRS = 1,
	E_WNET_CDMA = 2,
}E_WNET_TYPE;


typedef enum {
	E_WNET_AUTO_RESTRAT = 1,	//��������ģʽ
	E_WNET_RENEW_PPPOPEN,		//����pppopen

}E_WNET_MODE;

typedef enum {
	E_COMM_WIRELESS = 1,	//gprsͨѶ
	E_COMM_TCPIP,			//TCPIP��ʽ
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
	char	HostDns[50];   /*����������*/
	char 	HostIp[2][16]; /*������IP��ַ*/
	int		HostPort[2];
	int     iDnsMethod;     /*����������ΪDNS*/
	int		iTimeout;		/*connect��ʱʱ��*/
	int 	iCurrentIp;	/*���ص����ӳɹ���ip*/
	E_COMM_TYPE 	iCurrentComm;	/*��ǰ��ͨѶ��ʽ*/
	int		(*wnet_conncet_display)(E_CONNECT_DISPLAY edisplay, int ierror);	//��ʾ�ص�����
	uint32_t			(*wait_key)(void);
	E_COMM_CONNECT	eCurrentComm;
}Wnet_TcpipInfo_t;

#define WC_STATE_MASK			0x7fffffff
#define WC_STATE_BUSY			(~WC_STATE_MASK)
#define wc_is_state_busy(state)	((state) & WC_STATE_BUSY)


int wc_start_wnet(void);								//����wnet����
int wc_param_set(WnetPPPInfo_t *wnetinfo);						//����wnet����pin�û��������
int wc_get_state(int *state,  int *idispinfo, int *ierror);		//��ȡ״̬��״̬����ʾ����Ϣ�Լ�������
int wc_set_state(int newstate);									//�����µ�״̬��״̬
int wc_wait_ppp_ready(WnetCallback_t *wc, RecvDataTime RecvTimeDisp,DispWnitTime WnitTimeDisp);
void wc_stop_wnet(void);										//ǿ��ֹͣPPP��·

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

