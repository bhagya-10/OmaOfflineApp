
// ����ȫ�ֱ�����������
#include "global.h"

ST_SYSTEM_PARAM	      gstSysParam;		    // ϵͳ���ò���
ST_SYSCONTROL_PARAM	  gstSysCtrlParam;		// ϵͳ���Ʋ���
ST_POS_CONFIG_INFO    gstPosCapability;     // �ն����õ�����

ST_TRANSINFO	      gstTransInfo;		    // ���״�����Ϣ

ST_COMM_DATA	      gstSendData;      // ���͵�ͨѶ����
ST_COMM_DATA	      gstRecvData;	    // ����ͨѶ����
ST_ISO8583	       	  gstSendPack;		// ���ͱ���
ST_ISO8583		      gstRecvPack;		// ���ձ���

ST_ACQUIRER_INFO	  gstCurAcq;		// ��ǰ�յ���
ST_ISSUER_INFO		  gstCurIssuer;	    // ��ǰ������
ST_CARD_INFO          gstCurCard;     // ��ǰ�Ŀ���

ST_COMM_CONFIG		  gstCurCommCfg;	// ��ǰͨѶ���ò���

ST_TOTAL_INFO		  gstAcqTotal[MAX_ACQ];		    // �յ��л�����Ϣ
ST_TOTAL_INFO		  gstIssuerTotal[MAX_ISSUER];	// �����л�����Ϣ
ST_TOTAL_INFO		  gstEdcTotal;		            // �ն˻�����Ϣ
ST_TOTAL_INFO		  gstTransTotal;       	        // ���״���ʹ�õĻ�����Ϣ
ST_TOTAL_INFO		  gstPrnTotal;		            // ���ݴ�ӡʹ�õĻ�����Ϣ

// end of file
