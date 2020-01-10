
/****************************************************************************
NAME
    checkopt.h - 定义选项检查函数

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.05.12      - created
****************************************************************************/

#ifndef _CHECKOPT_H
#define _CHECKOPT_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// 判断EDC是否支持EMV交易
uint8_t ChkIfEmvEnable(void);
uint8_t ChkIssuerOption(UINT32 uiOption);
uint8_t ChkAcqOption(UINT32 uiOption);
uint8_t ChkEdcOption(UINT32 uiOption);
uint8_t ChkOptionExt(uint8_t *psExtOpt, UINT32 uiOption);
uint8_t ChkIfNeedPIN(void);
uint8_t ChkIfPinReqdAllIssuer(void);
uint8_t ChkIfBelowMagFloor(void);
void    CheckCapture(void);
uint8_t ChkInstalmentAllAcq(void);
uint8_t ChkIfCurAcqEMVEnable(void);
uint8_t ChkIfDispMaskPan2(void);
uint8_t ChkIfInstalment(void);
uint8_t ChkIfTranAllow(uint8_t ucTranType);
uint8_t ChkSettle(void);
uint8_t ChkIfNeedTip(void);
uint8_t ChkIfIccTran(uint8_t ucEntryMode);
uint8_t ChkIfPrnReceipt(void);
uint8_t ChkIfNeedReversal(void);
uint8_t ChkIfSaveLog(void);
uint8_t ChkIfNeedMac(void);
uint8_t ChkIfAmex(void);
uint8_t ChkIfNeedSecurityCode(void);
uint8_t ChkIfNeedSettle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _CHECKOPT_H

// end of file
