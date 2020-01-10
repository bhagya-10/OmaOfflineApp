
/****************************************************************************
NAME
    manage.h - 定义管理类交易模块

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.10.25      - created
****************************************************************************/

#ifndef _MANAGE_H
#define _MANAGE_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void UnLockTerminal(void);
void LockTerm(void);
void ClearAllRecord(void);
void ClearConfig(void);
void ClearPassword(void);
void ClearReversal(void);
void ClearRecord(uint8_t ucAcqKey);
void DoClear(void);
void ViewTotal(void);
void ViewTranList(void);
void ViewSpecList(void);
void PrnLastTrans(void);
void RePrnSpecTrans(void);
void PrnTotal(void);
void RePrnSettle(void);
void PrintEmvErrLog(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _MANAGE_H

// end of file
