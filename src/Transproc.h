
/****************************************************************************
NAME
    tranproc.h - 定义交易处理模块

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.01      - created
****************************************************************************/

#ifndef _TRANSPROC_H
#define _TRANSPROC_H

/************************************************************************
 * 定义交易类型 查询 批上送 签到 预授权 授权销售 退款 冲正 销售 结算 测试
 * 撤消预授权 撤消
************************************************************************/
enum
{
	DOWNLOAD,
	PREAUTH,
	AUTH,
	SALE,
	INSTALMENT,
	BALANCE_BOC,
	BALANCE_SCB,
	UPLOAD,
	LOGON,
	REFUND,
	REVERSAL,
	SETTLEMENT,
	VOID,
	OFFLINE_SEND,
	OFF_SALE,
	SALE_COMP,
	CASH,
	SALE_OR_AUTH,
	TC_SEND,
	ENQUIRE_BOC_RATE,
	MAX_TRANTYPE,
};

// 交易配置信息
#define PRN_RECEIPT			0x80
#define WRT_RECORD			0x40
#define IN_SALE_TOTAL		0x20
#define IN_REFUND_TOTAL		0x10
#define VOID_ALLOW			0x08
#define NEED_REVERSAL		0x04
#define ACT_INC_TRACE		0x02
#define NO_ACT				0x00
typedef struct _tagTRAN_CONFIG
{
	uint8_t	szLabel[16+1];		// 显示的交易类型
	uint8_t	szTxMsgID[4+1];		// 交易上送报文信息
	uint8_t	szProcCode[6+1];	// 交易处理码
	uint8_t	ucTranAct;		// 该交易需要的功能
}TRAN_CONFIG;

// 错误代码(具体定义待定!!!!!)
#ifndef ERR_BASE
#define ERR_BASE			0x10000
#endif
#define ERR_PINPAD			(ERR_BASE+0x01)
#define ERR_NO_TELNO		(ERR_BASE+0x03)
#define ERR_SWIPECARD		(ERR_BASE+0x05)
#define ERR_USERCANCEL		(ERR_BASE+0x06)
#define ERR_TRAN_FAIL		(ERR_BASE+0x07)
#define ERR_UNSUPPORT_CARD	(ERR_BASE+0x08)
#define ERR_SEL_ACQ			(ERR_BASE+0x09)
#define ERR_HOST_REJ		(ERR_BASE+0x0A)

#define ERR_NEED_INSERT		(ERR_BASE+101)
#define ERR_NEED_FALLBACK	(ERR_BASE+102)
#define ERR_NEED_SWIPE		(ERR_BASE+103)
#define ERR_NO_DISP			(ERR_BASE+999)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void SetCommReqField(void);
int  TransInit(uint8_t ucTranType);
int  TransCapture(void);
int  TransSale(void);
int  TransSaleManualEntry(void);
int  TransAuth(uint8_t ucTranType);
int  FinishOffLine(void);
int  TranReversal(void);
int  OfflineSend(void);
int  TranSaleComplete(void);
int  TransRefund(void);
int  TransOffSale(void);
int  TransVoid(void);
int  TransOther(void);
void TransAdjust(void);
int  TransSettle(void);
int  TransLogon(void);
int  TransSendTC(uint8_t ucUploadAll);

int  TransPrnLastTrans(void);
int  TransRePrnSpecTrans(void);
void SetParamForManager(void);

int  TransAutoSettlement(void);

int  TransGetElecSignature(void);          // 电子签名，内置签名板
void ClearElecSignaturefile(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


extern TRAN_CONFIG		glTranConfig[];
extern TRAN_CONFIG		glTranConfig_CH[];

#endif	// _TRANPROC_H

// end of file
