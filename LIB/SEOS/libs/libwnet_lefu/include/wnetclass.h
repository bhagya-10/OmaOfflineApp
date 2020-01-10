/**
 * @file    wnetclass.h
 *          
 * @author  Li.Hua <lihua_338@163.com>.
 *          
 * @date    2014-06-13
 *          
 * @brief   
 *          
 * @note    
 *          
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 *2014-06-24	1.1.1  tim.zhang Modified
 */

#ifndef __WNET_CLASS_H__
#define __WNET_CLASS_H__

#include <stdint.h>
#include <pthread.h>

#include "wccom.h"
#include "wnet.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 
 * ע�⣺ͨ��ģ��״̬��һ��32λ���ݣ�
 * ���λΪ1��ʾ����һ���ȶ�״̬��Ϊ0��ʾ��һ���ȶ�״̬
 * STATE_BEGIN��STATE_END��STATE_PPP_READY���ȶ�״̬��
 * ����״̬���Ƿ��ȶ�״̬�����ȶ�״̬�᲻ͣ��Ǩ�����ձ�Ϊ�ȶ�״̬��
 * ״̬�ĵ�31λ��״̬���壬������ö�����Ͷ���
 */


typedef struct tagWnetClass {
	int			state;					// ��ǰ״̬
	int 		displayinfo;			//��������ʾ����ʾ��Ϣ
	int 		ierror;
	wnet_moduleinfo_t	minfo;			// ģ����Ϣ
	pthread_mutex_t		lock;
	pthread_t	tid;
	WnetPPPInfo_t	wnetinfo;
	char				simid[64];
	//char				simIMSI[20];
	
	char				var_begin[0];
	int					nr_poweron;				// �ϵ����
	int					nr_pppopen;				// ppp open����
	int					nr_checksim2;				// check sim�������
	int					nr_deattach;				// ȥ���Ŵ������
	int					nr_record_poweron;		//��¼poweron�Ĵ���
	int					nr_stop_wnet;				//ǿ��ֹͣwnet
	int 				nr_nopin;					//û��pin����
	int					nr_connect;				//��¼���Ӵ���
	int                 nr_checkPPPtimeout;     //��¼���PPPʱ��
	int                 nr_iPPPnum1;
	int                 nr_iPPPnum2;
	struct timeval		tm_checksim1;				// ���sim1��ʱ
	struct timeval 		tm_checksim2;				//���sim2��ʱ
	struct timeval		tm_checkppp;				// ppp����ʱ
	struct timeval		tm_attach;					// ���總�ż�ʱ
	
	char				msg[64];					// ��ǰ������Ϣ
	char				var_end[0];
}WnetClass_t;

WnetClass_t	*wnet_create(void);
int wnet_destroy(WnetClass_t *wc);
int wnet_get_state(WnetClass_t *wc, int *state,  int *idispinfo, int *ierror);
int wnet_set_state(WnetClass_t *wc, int newstate);
int wnet_pthread_exit(WnetClass_t *pstWnet);
int wnet_pthread_start(WnetClass_t *pstWnet);
void wnet_debugout(const char *format, ...);
void wnet_SetSignalHandler(void);
int wnet_ppp_end(WnetClass_t *wc);
int wnet_set_param(WnetClass_t *wc, WnetPPPInfo_t *psWnetInfo);
int wnet_ppp_close(WnetClass_t *wc);
void wnet_setsignal_handler(void);





#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __WNET_CLASS_H__ */

