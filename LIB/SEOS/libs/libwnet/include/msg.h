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
	E_MAIN_PORT = 6000,		//��Ӧ�ö˿�
	E_WNET_PORT,				//wnet�˿ڶ���
	E_APP1_PORT,
	E_APP2_PORT
}E_UDPPORT;

typedef enum _E_APP_MSG_TYPE
{
	/*Ӧ�ó�����wnet���̷��͵���Ϣ����*/
	E_MSG_SET_STATE_TO_WNET = 1000,	//֪ͨwnet����״̬
	E_MSG_GET_STATE_FROM_WNET,	//��wnet���̻�ȡ״̬
	E_MSG_NOTIFY_WNET_STOP,		//֪ͨwnetֹͣ����
	E_MSG_SET_PARAM_TO_WNET,		//��wnet���ò���
	E_MSG_NOTIFY_WNET_CLOSEPPP,	//�ر�PPP��·
	E_MSG_NOTIFY_WNETMODE,			//wnet����״̬��ģʽ
	E_MSG_NOTIFY_CONNECT_SUCCESS,	//֪ͨwnet���ӳɹ�
	E_MSG_GET_PPP_AUTH,				//��ȡppp��֤��ʽ
	E_MSG_NOTIFY_PPPCOLSE,			//����ʧ��֪ͨwnet
	E_MSG_GET_SIMID,					//��ȡsimid
	E_MSG_GET_WNETVER,				//��ȡ�汾��
	E_MSG_SET_ATTACH_END,         //״̬ͼ�ߵ�����״̬��ֹ
	

	/*wnet���̻��͵���Ϣ����*/
	E_MSG_WNET_OK = 2000,		//wnet׼��OK
	E_MSG_RESULT_SET_STATE,	//����״̬���
	E_MSG_RESULT_GET_STATE,	//��ȡ��ǰ״̬���
	E_MSG_RESULT_NOTIFY_WNET_STOP,	//֪ͨwnetֹͣ���еĽ��
	E_MSG_RESULT_SET_PARAM_OK,		//���ò����ɹ�
	E_MSG_RESULT_CLOSEPPP,			//ppp��·�Ҷϳɹ�
	E_MSG_RESULT_CONNECT_SUCCESS,	//���ӳɹ�
	E_MSG_RESULT_PPP_AUTH,			//��ȡPPP��֤�ɹ�
	E_MSG_RESULT_PPPCLOSE,				//����ʧ��֪ͨwnet�ɹ�
	E_MSG_RESULT_SIMID,				//simid		
	E_MSG_RESULT_WNETVER,				//�汾��
	E_MSG_RESULT_ATTACH_END,       //״̬ͼ�ߵ�����״̬��ֹ
	
}E_APP_MSG_TYPE;


typedef enum _E_APP_TYPE
{
	E_APPTYPE_MAINAPP,
	E_APPTYPE_WNET,	
}E_APP_TYPE;

typedef struct
{
  unsigned int   	appType; 		//��Ϣ��Դ
  unsigned int   	msgType; 		//��Ϣ����ָҪ��������Ϣ����������ʲô
  unsigned long    	iMsgLen;  		//��Ϣ�ĳ���
  unsigned char    	msgData[1024];	//��Ϣ��������
} T_APP_MSG; //Ӧ����ϵͳ���������Ϣ�ṹ����



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

