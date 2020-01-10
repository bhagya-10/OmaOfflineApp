
// 定义全局变量、常量等
#include "global.h"

ST_SYSTEM_PARAM	      gstSysParam;		    // 系统配置参数
ST_SYSCONTROL_PARAM	  gstSysCtrlParam;		// 系统控制参数
ST_POS_CONFIG_INFO    gstPosCapability;     // 终端配置的性能

ST_TRANSINFO	      gstTransInfo;		    // 交易处理信息

ST_COMM_DATA	      gstSendData;      // 发送的通讯数据
ST_COMM_DATA	      gstRecvData;	    // 接收通讯数据
ST_ISO8583	       	  gstSendPack;		// 发送报文
ST_ISO8583		      gstRecvPack;		// 接收报文

ST_ACQUIRER_INFO	  gstCurAcq;		// 当前收单行
ST_ISSUER_INFO		  gstCurIssuer;	    // 当前发卡行
ST_CARD_INFO          gstCurCard;     // 当前的卡表

ST_COMM_CONFIG		  gstCurCommCfg;	// 当前通讯配置参数

ST_TOTAL_INFO		  gstAcqTotal[MAX_ACQ];		    // 收单行汇总信息
ST_TOTAL_INFO		  gstIssuerTotal[MAX_ISSUER];	// 发卡行汇总信息
ST_TOTAL_INFO		  gstEdcTotal;		            // 终端汇总信息
ST_TOTAL_INFO		  gstTransTotal;       	        // 交易处理使用的汇总信息
ST_TOTAL_INFO		  gstPrnTotal;		            // 单据打印使用的汇总信息

// end of file
