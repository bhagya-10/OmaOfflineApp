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
 * 注意：通信模块状态是一个32位数据，
 * 最高位为1表示不是一个稳定状态，为0表示是一个稳定状态
 * STATE_BEGIN、STATE_END、STATE_PPP_READY是稳定状态，
 * 其它状态都是非稳定状态，非稳定状态会不停变迁，最终变为稳定状态。
 * 状态的低31位是状态定义，见下面枚举类型定义
 */


typedef struct tagWnetClass {
	int			state;					// 当前状态
	int 		displayinfo;			//给界面显示的提示信息
	int 		ierror;
	wnet_moduleinfo_t	minfo;			// 模块信息
	pthread_mutex_t		lock;
	pthread_t	tid;
	WnetPPPInfo_t	wnetinfo;
	char				simid[64];
	//char				simIMSI[20];
	
	char				var_begin[0];
	int					nr_poweron;				// 上电计数
	int					nr_pppopen;				// ppp open计数
	int					nr_checksim2;				// check sim错误计数
	int					nr_deattach;				// 去附着错误计数
	int					nr_record_poweron;		//记录poweron的次数
	int					nr_stop_wnet;				//强制停止wnet
	int 				nr_nopin;					//没有pin计数
	int					nr_connect;				//记录链接次数
	int                 nr_checkPPPtimeout;     //记录检测PPP时间
	int                 nr_iPPPnum1;
	int                 nr_iPPPnum2;
	struct timeval		tm_checksim1;				// 检查sim1计时
	struct timeval 		tm_checksim2;				//检查sim2计时
	struct timeval		tm_checkppp;				// ppp检查计时
	struct timeval		tm_attach;					// 网络附着计时
	
	char				msg[64];					// 当前操作信息
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

