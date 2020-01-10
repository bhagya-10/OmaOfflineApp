/****************************************************************************
NAME
    global.h - 定义公共数据结构

DESCRIPTION
    定义应用的全局变量/全局常量等

REFERENCE
   警告: 为了结构变量的对齐，添加结构成员或者生成新结构时，结构变量最好是
         char(1byte)类型或者4*n byte类型，不要声明2byte类型的变量。
		 由于编译器选项默认的编译选项是#pragma pack(4)。 

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.01      - created
****************************************************************************/
#ifndef _GLOBAL_H
#define _GLOBAL_H

#define _POS_TYPE_6210
#define _EMV_TEST_VERSION	// use hard coded emv parameters
//#define _NO_TMS_			// use hard coded parameters
#define  _POS_TYPE_8210
#define  _APP_SUPP_ELECSIGN  
#define  OFFLINE_TEST_VERSION
#include <ctype.h>
#include "posapi.h"
#include "New8210.h"
#include "seos.h"
#include "tmslib.h"
#include "MathLib.h"
#include "PosLib.h"
#include "fileoper.h"
#include "main.h"
#include "Transproc.h"
#include "ISO8583.h"
#include "Setup.h"
#include "Commlib.h"
#include "Display.h"
#include "Function.h"
#include "Manage.h"
#include "Print.h"
#include "Tools.h"
#include "Transproc.h"
#include "AppInit.h"
#include "Card.h"
#include "ChkOption.h"
#include "Password.h"
#include "Commproc.h"
#include "Emvproc.h"
#include "Emvtest.h"
#include "emvapi.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	
#define JAY_TEST
#define EDCAPP_AID		"CardWorks"
#define EDC_VER_STRING	"CardWk_Build06"
#define STR_ELECSIGNATURE_FILE_PATH   "./image"

#define LEN_WORK_KEY	16
#define MASTER_KEY_ID	1
#define PIN_KEY_ID		1
#define MAC_KEY_ID		10
#define ZMK_KEY_ID	    6
#define DATA_KEY_ID     20
#define TAK_KEY_ID      10

// #define FUN2_READ_ONLY		// open this macro to disable modify acquirer's parameter

// 系统限制宏定义
#define MAX_PLAN 				20
#define MAX_ACQ					10
#define MAX_ISSUER				20
#define MAX_CARD				40
#define MAX_DESCRIPTOR			10
#define MAX_CARDBIN_NUM			2    // 800
#define MAX_CARDBIN_ISSUER		2    // 250
#define MAX_CARBIN_NAME_LEN		30
#define LEN_MAX_AD_DATA			4000
#define MAX_TRANLOG				400		// must <65535
#define MAX_ERR_LOG				10		// # of EMV error transactions, adjust as needed

#define USER_OPER_TIMEOUT		60		// 用户超时设置

#define ACQ_ALL					0xFF	// 所有Acquirer
#define INV_ACQ_KEY				0xFF	// 无效的acquirer key
#define INV_ISSUER_KEY			0xFF	// 无效的issuer key

// EDC 选项访问宏定义
#define EDC_SETTLE_PWD			    0x0001
#define EDC_VOID_PWD			    0x0002
#define EDC_REFUND_PWD			    0x0004
#define EDC_ADJUST_PWD			    0x0008
#define EDC_MANUAL_PWD			    0x0010
#define EDC_KEYBOARD_LOCKED		    0x0101
#define EDC_TIP_PROCESS				0x0201
#define EDC_ENABLE_INSTALMENT		0x0202
#define EDC_AUTH_PREAUTH			0x0204
#define EDC_CASH_PROCESS			0x0208
#define EDC_OFFLINE_TXN				0x0210
#define EDC_ENABLE_DCC				0x0220
#define EDC_SELECT_ACQ_FOR_CARD		0x0301
#define EDC_REFERRAL_DIAL			0x0304
#define EDC_AUTH_PAN_MASKING		0x0310
#define EDC_ENABLE_ELECSIGN	    	0x0320
#define EDC_AUTO_SETTLEMENT         0x0340

// 发卡行选项访问宏定义
#define ISSUER_EN_ADJUST				0x0080
#define ISSUER_EN_DISCRIPTOR 			0x0040
#define ISSUER_EN_VOICE_REFERRAL		0x0020
#define ISSUER_EN_OFFLINE				0x0010
#define ISSUER_EN_EXPIRY				0x0008
#define ISSUER_EN_MANUL					0x0004
#define ISSUER_EN_PIN					0x0002
#define ISSUER_EN_ACCOUNT_SELECTION		0x0001
#define ISSUER_EN_PREAUTH				0x0180
#define ISSUER_EN_REFUND				0x0140
#define ISSUER_CHECK_EXPIRY_OFFLINE		0x0110
#define ISSUER_CAPTURE_TXN				0x0108
#define ISSUER_EN_PRINT					0x0104
#define ISSUER_ROC_INPUT_REQ			0x0102
#define ISSUER_CHKPAN_MOD10				0x0101
#define ISSUER_CHKPAN_MOD11				0x0280
#define ISSUER_CHECK_EXPIRY				0x0210
#define ISSUER_AUTH_CODE				0x0208
#define ALLOW_EXTEND_PAY				0x0204
#define ISSUER_ADDTIONAL_DATA			0x0202
#define ISSUER_EN_VOID					0x0201
#define ISSUER_SECURITY_MANUL			0x0380
#define ISSUER_SECURITY_SWIPE			0x0340
#define ISSUER_DISP_EXPIRY			    0x0302
#define ISSUER_EN_BALANCE				0x0301
#define ISSUER_EN_SWIPE	    			0x0304

// 收单行选项访问宏定义
#define ACQ_VISA_CHOICE						0x0080
#define ACQ_PRINT_AUDIT_TRAIL				0x0040
#define ACQ_EXPRESS_PAYMENT_PROMPT			0x0020
#define ACQ_DISABLE_BUSINESS_DATE			0x0010
#define ACQ_IGNORE_HOST_TIME				0x0008
#define ACQ_REQ_OPEN_SUSPEND_BATCH			0x0004
#define ACQ_CLEAR_BATCH						0x0002
#define ACQ_AMEX_SPECIFIC_FEATURE			0x0001
#define ACQ_PRINT_AUTH_CODE					0x0120
#define ACQ_DISABLE_TRICK_FEED				0x0110
#define ACQ_BLOCK_AUTH_FOR_OFFLINE			0x0108
#define ACQ_USE_TRACK3						0x0104
#define ACQ_NOT_PRINT_RRN					0x0102
#define ACQ_ADDTIONAL_PROMPT				0x0101
#define ACQ_CITYBANK_INSTALMENT_FEATURE		0x0240
#define ACQ_DBS_FEATURE						0x0220
#define ACQ_BOC_INSTALMENT_FEATURE			0x0210
#define ACQ_ONLINE_VOID						0x0208
#define ACQ_ONLINE_REFUND					0x0204
#define ACQ_AIR_TICKET						0x0201
#define ACQ_EMV_FEATURE						0x0202
#define ACQ_ASK_CVV2						0x0380

// 终端能力配置信息，例如打印类型，通讯类型
typedef struct _tag_POS_CONFIG_INFO
{
	UINT32   uiPosType;            // 终端型号
	UINT32   uiSecuType;           // 安全模块类型，是否支持PCI
	UINT32   uiPrnType;            // 终端打印机类型，针打－－热敏
	UINT32   uiCommType;           // 终端配置的通讯类型
	UINT32   uiMaxSyncPSTNRate;    // 同步拨号的最高波特率
	UINT32   uiMaxAsynPSTNRate;    // 异步拨号的最高波特率
	UINT32   uiUnContactlessCard;  // 非接触式卡
	UINT32   uipostsScreen;        // 手写触摸屏
}ST_POS_CONFIG_INFO;


// 终端存储EDC信息
#define    EDC_INIT_OK		    0x2143
#define    EDC_LANGUGE_EN		0x00
#define    EDC_LANGUGE_SM		0x01
#define    EDC_LANGUGE_CH		0x02
#define    _TCP_BCD_LEN_BYTE	0x01  // TCP通讯,BCD 格式的长度字节
typedef struct _tag_ST_EDC_INFO
{
	UINT32	  uiInitFlag;				// 0x5A5A OK
	char	  szInitTime[14+1];		    // YYMMDDhhmmss
	char	  sInitSTAN[3];			    // to be sync to SYS_CONTROL
	char	  bClearBatch;			    // 0-none, 1-clear former batch
	char	  szHelpTelNo[24+1];		// for help function
//  sOption[0]: b0-settle pwd, b1-void pwd, b2-refund pwd, b3-adjust pwd, b4-manule pwd
//	sOption[1]: b0-locked keyborad
//  sOption[2]: b0-TIP PROC, b1-ENBALE INSTALL, b2-AUTH OR PREAUTH, b3-CASH PROC, b4-ENABLE ECR, b5-ENABLE DCC
//  sOption[3]: b0-SELECT ACQ, b1-REFERRAL_DIAL, b2- MASK PAN FOR AUTH T
	char	  sOption[5];				// option for access
	char	  szMerchantAddr[80+1];	     // line 2 and 3 on invoice
	char	  szMerchantName[30+1];	     // merchant name,line 1
	char	  szAddlPrompt[20+1];		 // for show and print
	char	  bPreDial;				     // TRUE: enable pre-dial for modem
	char      szCurrencyName[3+1];       // 货币名称
	char	  ChCurrencySymbol;          // currency symbol
	UINT32	  uiDecimalPos;              // 货币小数点位
	UINT32    uiTranAmtLen;              // 输入金额最大长度
	UINT32    uiPrintNums;               // 打印票据张数
	UINT32	  uiScrGray;				 // LCD亮度, 默认为4
	ulong	  ulAcceptTimeout;		     // 显示交易成功信息的时间
	ulong	  ulTMSTimeOut;			     // 下载参数通讯超时
	ulong     ulMaxSettleAmt;            // 当天允许的最大总计金额
	char	  szPabx[10+1];			     // 号码前缀，例如外线号码，国际长途
	char	  szDownLoadTID[8+1];		 // 下载参数终端号
	char	  szTMSNii[3+1];			 // 下载参数的NII
	UINT8     ucLanguage;                // 英文，简体中文，繁体中文
	UINT8     ucTcpMsgHeadLenType;       // 
	char      sTmkRefNumber[8];          // TMK Ref number
	char      szAutoSettleTime[10+1];    // 自动结算的时间
}ST_EDC_INFO;

// 卡表信息
typedef struct _tag_ST_CARD_INFO
{
	char	 sPanRangeLow[5];		// 卡号起始号
	char	 sPanRangeHigh[5];		// 卡号终止号
	uint8_t	 ucIssuerKey;			// 卡表指向的发卡表号
	uint8_t	 ucAcqKey;				// 卡表指向的收单表号
	char	 ChOption;				// b1-allow payment
	ulong	 ulPanLength;			// 卡号长度, 00为不检查长度
}ST_CARD_INFO;

// 发卡行信息
typedef struct _tag_ST_ISSUER_INFO
{
	uint8_t	ucKey;					// 表号
	char	szName[11+1];			// 发卡行名称
	char	szRefTelNo[12*2+1];		// phone # for voice referral
	// sOption[0]:
		// b7-allow adjust, b6-descriptor required, b5-voice referral allowed, b4-allow offline
		// b3-expiry required, b2-allow manual pan, b1-pin required, b0-account type selection
		// sOption[1]:
		// b7-inhibit pre-auth, b6-inhibit refund, b4-check expiry for offline
		// b3-capture txn, b2-print, b1-roc input required, b0-pan MOD 10 check
	// sOption[2]:
		// b7-pan Robinson's MOD 11 check, b4-check expiry, b3-omit auth_code display and input for offline
		// b2-extended pay, b1-additional data allowed, b0-inhibit void
	// sOption[3]:
		// b6-security code entry
	char	sOption[4];
	uint8_t	ucDefAccount;		// reserved ?
	char	sPanMask[3];		// for print slips
	ulong	ulFloorLimit;
	uint8_t	ucAdjustPercent;
}ST_ISSUER_INFO;


// 收单行信息
typedef struct _tag_ST_ACQUIRE_INFO
{
	uint8_t	 ucKey;						// 表号
	char 	 szPrgName[20+1];			// 别名？主机名称？reserved?
	char	 szName[20+1];				// 收单行名称
	uint8_t	 sIssuerKey[MAX_ISSUER];		// 发卡行信息
	uint8_t	 ucIndex;					// 该收单行的数组下标
	// sOption[0]:
		// b7-visa choice,b6-print audit trail after settle,b5-express payment prompt
		// b4-disable business date,b3-ignore host time,b2-request open suspended batch
		// b1-clear batch after initial,b0-amex specific feature
	// sOption[1]:
		// b5-prompt auth_code on receipt 1,b4-disable trick feed, b3-block auth prompt for offline
		// b2-use track, b1-no rrn on receipt, b0-additional prompt
	// sOption[2]:
		// b3-online void, b2-online refund
	// sOption[3]: // unused
	char	sOption[4];
	char	szNii[3+1];
	char	szTermID[8+1];
	char	szMerchantID[15+1];
	char    szTxnTelNo1[25];
	char    szTxnTelNo2[25];
	char	szRemoteIP[15+1];
	char	szRemotePort[5+1];
	char	szRemoteIP2[15+1];
	char	szRemotePort2[5+1];
	ulong	ulTimeOut;			        // timeout for wait resp
	ulong	ulCurBatchNo;
	ulong	ulNextBatchNo;
	ST_COMM_CONFIG	stTxnCommCfg;	// 交易通讯配置
}ST_ACQUIRER_INFO;

// 分期付款计划信息
typedef struct _tagST_INSTALMENT_PLAN
{
	uint8_t	    ucIndex;		// 1~MAX_PLAN
	uint8_t  	ucAcqIndex;
	char	    szName[10+1];
	UINT32	    uiMonths;
	ulong	    ulLeastAmt;	// 最低金额
}ST_INSTALMENT_PLAN;

// 商品描述信息
typedef struct _tagST_DESCRIPTOR
{
	uint8_t	 ucKey;
	char	 szCode[2+1];		// ANS format
	char	 szText[20+1];		// for display and print
}ST_DESCRIPTOR_INFO;

// 详细卡表信息(RFU for HK)
#define LEN_MAX_CARBIN_NAME		30
typedef struct _tagISSUER_NAME
{
	uint8_t	szChineseName[16+1];
	uint8_t	szEnglishName[LEN_MAX_CARBIN_NAME+1];
}ISSUER_NAME;

typedef struct _tagCARD_BIN
{
	uint8_t	ucIssuerIndex;		// 与IssuerName关联的索引号
	uint8_t	ucPanLen;			// 卡号长度(reserved)
	uint8_t	ucMatchLen;			// 匹配时候的长度
	uint8_t	sStartNo[10];
	uint8_t	sEndNo[10];
}CARD_BIN;

// 选项参数信息
typedef struct _tagOPTION_INFO
{
	void	*pText;
	UINT32	uiOptVal;
}OPTION_INFO;


// 交易日志
#define	MAX_GET_DESC		4
//
#define	MODE_NO_INPUT			0x00
#define	MODE_MANUAL_INPUT		0x01	// 手输卡号
#define	MODE_SWIPE_INPUT		0x02	// 磁卡刷卡
#define	MODE_CHIP_INPUT			0x04	// EMV卡插卡
#define	MODE_FALLBACK_SWIPE		0x08	// EMV刷卡
#define	MODE_PIN_INPUT			0x10	// online PIN input
#define MODE_OFF_PIN			0x20	// for AMEX
#define MODE_4DBC_INPUT         0x40    // for AMEX
#define MODE_CONTLESS_INPUT     0x40    // 非接触式卡
//
#define TS_OK			0x0000		// txn accepted
#define TS_NOSEND		0x0001		// txn not sent to host
#define TS_ADJ			0x0002		// txn adjusted
#define TS_REVD			0x0004		// txn reversed (reserved)
#define TS_VOID			0x0008		// txn voided
#define TS_CHANGE_APPV	0x0010		// 替换Approval Code, reserved
#define TS_FLOOR_LIMIT	0x0020		// 交易金额低于Floor Limit
#define TS_OFFLINE_SEND	0x0040		//
#define TS_NOT_UPLOAD	0x0080		// 不需要上送(由NOSEND/ADJ判断)
#define TS_NEED_TC		0x0100		// 需要上送TC
#define TS_ALREADY_TC   0x0200		// 已经上送TC
#define TS_ELECSIGN_HAVE   0x0400    // 有电子签名
#define TS_ALL_LOG		0xFFFF

typedef struct _tagTRAN_LOG
{
	uint8_t	  ucAcqKey;					// 收单行表号
	uint8_t	  ucIssuerKey;				// 发卡行表号
	uint8_t	  ucTranType;				// 交易类型
	uint8_t	  ucOrgTranType;			// 原交易类型
	uint8_t	  szPan[19+1];				// 卡号
	uint8_t	  szExpDate[4+1];			// 有效期
	uint8_t	  szAmount[12+1];			// 交易金额
	uint8_t	  szTipAmount[12+1];		// 小费金额
	uint8_t	  szOrgAmount[12+1];		// 原交易金额
//	uint8_t	  szFrnAmount[12+1];		// 外币金额
	uint8_t	  szDateTime[14+1];			// YYYYMMDDhhmmss
	uint8_t	  szRRN[13+1];				// RRN, system ref. no
	uint8_t	  szCondCode[2+1];			// Bit 25
	uint8_t	  szRspCode[2+1];				// 响应码
	uint8_t	  szHolderName[26+1];
	uint8_t	  szAuthCode[6+1];
	uint8_t	  ucInstalment;
//	uint8_t	  szFrnCurcyCode[3+1];
	char	  szTrack2[40+1];               // ICC卡的2磁道信息数据
	// EMV related data
	uint8_t	  bPanSeqOK;					// TRUE: PAN Seq read OK
	uint8_t	  ucPanSeqNo;
	uint8_t	  sAppCrypto[8];
	uint8_t	  sTVR[5];
	uint8_t	  sTSI[2];
	uint8_t	  sATC[2];
	uint8_t	  szAppLabel[16+1];
	uint8_t	  ucAidLen;
	uint8_t	  sAID[16];
	UINT32	  uiIccDataLen;
	uint8_t	  sIccData[LEN_ICC_DATA];
	uint8_t  ucEntryMode;					// 输入模式, 可计算出Bit 22
	ulong	 ulInvoiceNo;			        // 票据号
	ulong	 ulSTAN;					    // STAN
	ulong	 ulOrgSTAN;				        // 原交易流水
	UINT32	 uiStatus;				        // 交易状态
	UINT32   ucElecSignatureflag;   // 是否有电子签名 -- 前4bit表示是否是电子签名，后面表示是否成功上送
	uint8_t	 szElecfilename[40];  // 电子签名文件的名称
}ST_TRAN_LOG;


// 系统交易处理信息,不需要存储到文件系统
typedef struct _tagSYS_TRANS_INFO
{
	ST_TRAN_LOG	  stTranLog;
	char		  szTrack1[79+1];
//	char		  szTrack2[40+1];
	char		  szTrack3[107+1];

	// 其他数据要素,请根据交易处理需求来增加
	uint8_t		  bIsFallBack;
	uint8_t		  bIsOnlineTxn;
	uint8_t		  bIsFirstGAC;			 // 是否是第一次GAC
	uint8_t		  ucEcrCtrl;			 // ECR标志(RFU)
	uint8_t		  bExpiryError;			 // TRUE: 卡号有效期错误
	uint8_t		  szSecurityCode[4+1];	 // CVV2/4DBC
// 	uint8_t	  	  bPinEnter;			 // 是否输入了PIN
	uint8_t		  sPinBlock[8];			 // PIN block (RFU)
	uint8_t		  ucConnectCnt;          // 连接次数
	uint8_t		  szSettleMsg[30+1];	 // 结算信息(用于显示/打印等)
	UINT32		  uiRecNo;				 // 原交易记录号(for VOID)
	INT32		  iFallbackErrCode;		 // 造成fallback的原因(for AMEX)
	ST_ISO8583	  stSendPack;			 // 发送报文备份
}ST_TRANSINFO;

// 交易冲正信息
typedef struct _tagST_REVERSAL_INFO
{
	INT8			bNeedReversal;		// TRUE：需要冲正, FALSE: 不需要
	UINT8			ucEntryMode;		// 输入模式
	ST_ISO8583		stRevPack;			// 原交易请求报文
}ST_REVERSAL_INFO;

// 通讯数据
#define	LEN_MAX_COMM_DATA	1024
typedef struct _tagST_COMM_DATA
{
	UINT32		uiLength;
	char		sContent[LEN_MAX_COMM_DATA+10];
}ST_COMM_DATA;

// 交易汇总信息
typedef struct _tagST_TOTAL_INFO
{
	UINT32		uiSaleCnt;
	UINT32		uiTipCnt;
	UINT32		uiRefundCnt;
	UINT32		uiVoidSaleCnt;
	UINT32		uiVoidRefundCnt;
	UINT32      uiTcSendCnt;

	uint8_t		szSaleAmt[12+1];	// base amount + tip amount
	uint8_t		szTipAmt[12+1];
	uint8_t		szRefundAmt[12+1];
	uint8_t		szVoidSaleAmt[12+1];
	uint8_t		szVoidRefundAmt[12+1];
	uint8_t		szTcSendAmt[12+1];
}ST_TOTAL_INFO;

// reprint settle information
typedef struct _tagST_REPRN_SETTL_INFO
{
	char		    bValid[MAX_ACQ];
	char		    szSettleMsg[MAX_ACQ][30];
	ulong		    ulBatchNo[MAX_ACQ];
	ulong		    ulSOC[MAX_ACQ];
	ST_TOTAL_INFO	stAcqTotal[MAX_ACQ];
	ST_TOTAL_INFO	stIssTotal[MAX_ISSUER];
}ST_REPRN_SETTL_INFO;

#define INIT_STATUS		0x01    // 初始化状态
#define LOGON_STATUS	0x02    // 签到状态
#define LOCK_STATUS	    0x04    // 锁机状态
#define TRANS_STATUS	0x08    // 正常交易状态
// 系统交易相关参数配置信息
typedef struct _tagSYSTEM_PARAM
{
	UINT32				uiAppStatus;		// 终端状态
	char				szPassword[MAX_PWD][16+1];	// 密码数据,密码长度最长16字节

	ST_COMM_CONFIG		stDownParamCommCfg;		// 终端参数下载通讯配置
 
	ST_EDC_INFO			stEdcInfo;			// 终端参数

	uint				uiAcqNum;			// 收单行数目
	ST_ACQUIRER_INFO	stAcqList[MAX_ACQ];

	uint				uiIssuerNum;		// 发卡行数目
	ST_ISSUER_INFO		stIssuerList[MAX_ISSUER];

	uint				uiCardNum;			// 卡表数目
	ST_CARD_INFO		stCardTable[MAX_CARD];

	uint				uiDescNum;			// 商品描述个数
	ST_DESCRIPTOR_INFO	stDescList[MAX_DESCRIPTOR];

	uint				uiPlanNum;			// 分期付款计划个数
	ST_INSTALMENT_PLAN	stPlanList[MAX_PLAN];

	ST_WIFI_AP_INFO     stWIFIAPInfo;       // wifi AP 配置信息 目前只考虑一台机器只连接一个热点

	// for HK
	UINT32				uiIssuerNameNum;
	ISSUER_NAME			stIssuerNameList[MAX_CARDBIN_ISSUER];
	UINT32				uiCardBinNum;
	CARD_BIN			stCardBinTable[MAX_CARDBIN_NUM];
	char				bTextAdData;		// False: bitmap TRUE: text
	char				sAdData[LEN_MAX_AD_DATA];	// 广告信息
	char                szAutoSettlementTime[14+1];    // 自动结算时间
}ST_SYSTEM_PARAM;

// RFU for HK
typedef struct _tagST_EMV_FIELD56
{
	UINT32		uiLength;
	uint8_t		sData[LEN_ICC_DATA2];
}ST_EMV_FIELD56;

typedef struct _tagST_WRITE_INFO
{
	uint8_t		  bNeedSave;		// TRUE: should save stTranLog
	uint8_t		  ucAcqIndex;		// acquirer index, must < 255
	UINT32		  uiRecNo;		    // record # of stTranLog
	ST_TRAN_LOG	  stTranLog;		// txn information to save
}ST_WRITE_INFO;


// 系统运行参数在每次交易都可能进行修改,存储在文件系统
enum {S_RESET, S_SET, S_LOAD, S_INS, S_OUT, S_USE ,S_PENDING, S_CLR_LOG};
typedef struct _tagSYS_CONTROL
{
	char            szLastSettlementDate[14+1];     // 上次结算时间
	ulong           ulTotalTranNum; // POS保存的所有交易数量
	ulong			ulSTAN;			// 当前终端流水号
	ulong			ulInvoiceNo;	// 当前票据号
	UINT32			uiLastRecNo;	// 最后一笔交易记录#
	UINT32			uiErrLogNo;		// 当前记录EMV错误日志的记录#
	UINT32			uiLastRecNoList[MAX_ACQ];		// for bea
	UINT32			sAcqStatus[MAX_ACQ];			// 收单行状态信息
	UINT32			sAcqKeyList[MAX_TRANLOG];		// 交易记录表
	UINT32			sIssuerKeyList[MAX_TRANLOG];	// 交易记录表
	ST_WRITE_INFO	stWriteInfo;	// information for saving txn log

#define LEN_SYSCTRL_BASE	((INT32)OFFSET(ST_SYSCONTROL_PARAM, stRevInfo))
	ST_REVERSAL_INFO	  stRevInfo[MAX_ACQ];		// 冲正信息

#define LEN_SYSCTRL_NORMAL	((INT32)OFFSET(ST_SYSCONTROL_PARAM, stRePrnStlInfo))
	ST_REPRN_SETTL_INFO	  stRePrnStlInfo;           // 结算统计信息

}ST_SYSCONTROL_PARAM;


extern ST_SYSTEM_PARAM       gstSysParam;		    // 系统配置参数
extern ST_SYSCONTROL_PARAM	 gstSysCtrlParam;		// 系统控制参数
extern ST_POS_CONFIG_INFO    gstPosCapability;      // 终端配置的性能

extern ST_TRANSINFO	         gstTransInfo;       // 交易处理信息
extern ST_COMM_DATA	       	 gstSendData;        // 发送的通讯数据
extern ST_COMM_DATA	       	 gstRecvData;	     // 接收通讯数据
extern ST_ISO8583		     gstSendPack;		 // 发送报文
extern ST_ISO8583		     gstRecvPack;		 // 接收报文

extern ST_ACQUIRER_INFO		 gstCurAcq;		     // 当前收单行
extern ST_ISSUER_INFO		 gstCurIssuer;	     // 当前发卡行
extern ST_COMM_CONFIG		 gstCurCommCfg;		 // 当前通讯配置参数
extern ST_CARD_INFO          gstCurCard;         // 当前的卡表

extern ST_TOTAL_INFO		 gstAcqTotal[MAX_ACQ];		    // 收单行汇总信息
extern ST_TOTAL_INFO		 gstIssuerTotal[MAX_ISSUER];	// 发卡行汇总信息
extern ST_TOTAL_INFO		 gstEdcTotal;		            // 终端汇总信息
extern ST_TOTAL_INFO		 gstTransTotal;	                // 交易处理使用的汇总信息
extern ST_TOTAL_INFO		 gstPrnTotal;		            // 单据打印使用的汇总信息

extern unsigned char         Bmp_Prn[];

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _GLOBAL_H

// end of file
