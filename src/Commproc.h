
/****************************************************************************
NAME
    commproc.h - 实现通讯控制

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.07.2      - created
****************************************************************************/

#ifndef _COMMPROC_H
#define _COMMPROC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

INT32  TranProcess(void);
INT32  SendRecvPacket(void);
INT32  SendPacket(void);
INT32  RecvPacket(void);
INT32  PreDial(void);
int    CheckPreDial(int iPreDialRet);
INT32  ConnectHost(void);
INT32  AfterTranProc(void);
INT32  ReferralDial(uint8_t *pszPhoneNo);
INT32  GenSendPacket(void);
void   DispCommErrMsg(INT32 iErrCode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _COMMPROC_H

// end of file
