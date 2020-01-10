
/****************************************************************************
NAME
    Function.h - POS功能设置模块

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.01      - created
****************************************************************************/

#ifndef _FUNCTION_H
#define _FUNCTION_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void   FunctionExe(uint8_t bUseInitMenu, INT32 iFuncNo);
void   FunctionChnMenu(void);
void   FunctionEngMenu(void);
void   FunctionInit(void);
int    FunctionInput(void);
void   PrintKeys(void);

void   NacTxd(uint8_t *psTxdData, UINT32 uiDataLen);
int    NacRxd(uint8_t *psRxdData, UINT32 *uiDataLen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _FUNCTION_H

// end of file
