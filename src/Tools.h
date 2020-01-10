
/****************************************************************************
NAME
    Tools.h - 定义所有实用函数(针对应用封装基本库)

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.10      - created
****************************************************************************/

#ifndef _TOOLS_H
#define _TOOLS_H

// amount type 
#define AMOUNT		0
#define RFDAMOUNT	1
#define ORGAMOUNT	2
#define TIPAMOUNT	3

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void   InitTransInfo(void);
ulong  GetNewTraceNo(void);
ulong  GetNewBatchNo(ulong ulCurBatchNo);
ulong  GetNewInvoiceNo(void);
void   FindIssuer(uint8_t ucIssuerKey);
void   FindAcq(uint8_t ucAcqKey);
void   SetCurAcq(uint8_t ucAcqIndex);
void   SetCurIssuer(uint8_t ucIssuerIndex);
void   PrintOne(void);
void   GetDateTime(uint8_t *pszDateTime);
void   GetCurTime(char *pszCurTime);
void   ChangeToEngTime(char *pszDateTime, char *pszEngTime);
void   ConvIssuerName(uint8_t *pszOrgName, uint8_t *pszOutName);
int    GetAmount(void);
uint8_t  ValidAdjustAmount(uint8_t *pszBaseAmt, uint8_t *pszTotalAmt);
uint8_t  ConfirmAmount(uint8_t *pszAmount);
int    GetPIN(uint8_t ucFlag);
int    GetMAC(uint8_t *psData, UINT32 uiDataLen, uint8_t *psOutMAC);
void   Get8583ErrMsg(uint8_t bPackErr, int iErrCode, uint8_t *pszErrMsg);
int    GetPreAuthCode(void);
int    InputInvoiceNo(ulong *pulInvoiceNo);
void   GetStateText(UINT32 ucStatus, uint8_t *pszStaText);
uint8_t  AskYesNo(void);
int    SelectAcq(uint8_t bAllowSelAll, uint8_t *pszTitle, uint8_t *pucAcqIndex);
int    SelectIssuer(uint8_t *pucIssuerIndex);
void   ClearTotalInfo(void *pstTotalInfo);
uint8_t  IsNumStr(char *pszStr);
void   GetEngTranLabel(uint8_t *pszTranTitle, uint8_t *pszEngLabel);
void   GetChnTranLabel(uint8_t *pszTranTitle, uint8_t *pszChnLabel);
uint8_t  GetTranAmountInfo(void *pTranLog);
void   ModifyTermCapForPIN(void);
int   UpdateEMVTranType(void);
int   InputAmount(uint8_t ucAmtType);
int   GetTipAmount(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _UITL_H

// end of file
