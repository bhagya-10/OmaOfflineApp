/**
 * @file    wnetclass.h
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

#ifndef __MSG_H__
#define __MSG_H__

#include <stdint.h>
#include<time.h>

#include "wnet.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#ifndef NULL
#define NULL	((void*) 0)
#endif



#define RECORD_PORT_FILE	"/var/port"
#define WNET_UNIX_FILE		"/var/wnet.str"
#define UNIX_DOMAIN_PATH  "/var/"




typedef enum _E_UDPPORT
{
	E_MAIN_PORT = 6000,		//主应用端口
	E_WNET_PORT,				//wnet端口定义
	E_APP1_PORT,
	E_APP2_PORT
}E_UDPPORT;

typedef enum _E_APP_MSG_TYPE
{
	/*应用程序向wnet进程发送的消息类型*/
	E_MSG_SET_STATE_TO_WNET = 1000,	//通知wnet设置状态
	E_MSG_GET_STATE_FROM_WNET,	//从wnet进程获取状态
	E_MSG_NOTIFY_WNET_STOP,		//通知wnet停止运行
	E_MSG_SET_PARAM_TO_WNET,		//向wnet设置参数
	E_MSG_NOTIFY_WNET_CLOSEPPP,	//关闭PPP链路
	E_MSG_NOTIFY_WNETMODE,			//wnet启动状态机模式
	E_MSG_NOTIFY_CONNECT_SUCCESS,	//通知wnet链接成功
	E_MSG_GET_PPP_AUTH,				//获取ppp认证方式
	E_MSG_NOTIFY_PPPCOLSE,			//连接失败通知wnet
	E_MSG_GET_SIMID,					//获取simid
	E_MSG_GET_WNETVER,				//获取版本号
	E_MSG_SET_ATTACH_END,         //状态图走到附网状态截止
	

	/*wnet进程回送的消息类型*/
	E_MSG_WNET_OK = 2000,		//wnet准备OK
	E_MSG_RESULT_SET_STATE,	//设置状态结果
	E_MSG_RESULT_GET_STATE,	//获取当前状态结果
	E_MSG_RESULT_NOTIFY_WNET_STOP,	//通知wnet停止运行的结果
	E_MSG_RESULT_SET_PARAM_OK,		//设置参数成功
	E_MSG_RESULT_CLOSEPPP,			//ppp链路挂断成功
	E_MSG_RESULT_CONNECT_SUCCESS,	//链接成功
	E_MSG_RESULT_PPP_AUTH,			//获取PPP认证成功
	E_MSG_RESULT_PPPCLOSE,				//连接失败通知wnet成功
	E_MSG_RESULT_SIMID,				//simid		
	E_MSG_RESULT_WNETVER,				//版本号
	E_MSG_RESULT_ATTACH_END,       //状态图走到附网状态截止
	
}E_APP_MSG_TYPE;


typedef enum _E_APP_TYPE
{
	E_APPTYPE_MAINAPP,
	E_APPTYPE_WNET,	
}E_APP_TYPE;

typedef struct
{
  unsigned int   	appType; 		//消息来源
  unsigned int   	msgType; 		//消息类型指要操作的消息比如让你做什么
  unsigned long    	iMsgLen;  		//消息的长度
  unsigned char    	msgData[1024];	//消息附加数据
} T_APP_MSG; //应用子系统到浏览器消息结构定义



int unix_socket_connect(void);
ssize_t master_recv(int fd, void *buf, size_t len);
ssize_t master_send(int fd, const void *buf, size_t len);


#if defined(CONFIG_DEBUG_LEVEL)
#define debuging(fmt, arg...)	\
		do { \
			int saveerr=errno; \
			syslog(LOG_INFO, " %s, %d, " fmt "\n", __FUNCTION__, __LINE__,  ##arg); \
			errno = saveerr; \
		} while (0)
#else /* !defined(CONFIG_DEBUG_LEVEL) */
	
#define debuging(fmt, arg...)	do { } while (0)
	
#endif



#ifdef __cplusplus
	}
#endif /* __cplusplus */
	
#endif	/* MSG_H*/

