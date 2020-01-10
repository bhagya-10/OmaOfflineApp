/****************************************************************************
NAME
    setup.h - 定义终端参数设置、查询模块

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.01      - created
****************************************************************************/

#ifndef _SETUP_H
#define _SETUP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define  ACQ_COMMTYPE          0x01
#define  DOWNLAOD_COMMTYPE     0x02

void   SetSystemParam(void);
int    SetExtLineNo(void);
int    SetModemParam(void);
void   SetAcqParam(void);
int    SetTcpIpParam(int iMode);
int    GetIPAddress(uint8_t *pszPrompts, uint8_t bAllowNull, uint8_t *pszIPAddress);
int    ValidIPAddress(char *pszIPAddr);
int    GetIPPort(uint8_t *pszPrompts, uint8_t *pszPortNo);
void   SetIssuerParam(void);
void   DispAcqPama(void);
void   SetAcqCommType(void);
int    SetAcqCommParam(int iMode);
int    SetAcqTransTelNo(int iMode); 
int    SetRS232Param(void);
int    SetTel(uint8_t *pszTelNo, uint8_t *psDisp);
int    SetWirelessParam(int iMode);
void   SetEdcParam(void);
int    SetGetSysTraceNo(void);
int    SetAcceptTimeOut(void);
int    SetNumOfReceipt(void);
int    SetPreDial(void);
int    ModifyOptList(uint8_t *psOption, uint8_t ucMode);
void   ChangePassword(void);
void   SetSysTime(void);
void   SetLanguage(void);
int    SetMsgHeadLenType(void);
int    SelectAndSetWIFIAP(void);
int    SetWIFIParam(int iMode);
int    SetWiFiLoaclIPCfg(void);
void   SetMerchantInfo(void);
void   SetExternParam(void);
int    SetAutoSettlmentTime(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _SETUP_H

// end of file

