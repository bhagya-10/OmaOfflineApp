
/****************************************************************************
NAME
    Disp.h - 定义所有实用函数(针对应用封装基本库)

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.10      - created
****************************************************************************/

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "posapi.h"
#include "New8210.h"

typedef struct _tagHOST_ERR_MSG
{
	uint8_t	szRspCode[2+1];		// 主机返回码
	uint8_t	szEngMsg[32+1];
	uint8_t	szChnMsg[32+1];
}HOST_ERR_MSG;

typedef struct _tagTERM_ERR_MSG
{
	INT32		iErrCode;
	uint8_t	szEngMsg[16+1];
	uint8_t	szChnMsg[16+1];
}TERM_ERR_MSG;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void   Display2StringHigh(void *pszString1, void *pszString2);  // 在标题上显示(0－2)行显示 
void   Display2Strings(char *pszString1, char *pszString2);     // 在银幕中间显示3－5行显示  
void   Display2StringInRect(char *pszString1,  char *pszString2);
void   DrawRect(UINT32 uiBegRow, UINT32 uiBegCol, UINT32 uiEndRow, UINT32 uiEndCol);

int    DispLayIdleUI(int IfRedraw);
void   DisplayVersion(void);
void   DisplayTransName(void);
void   DisplayAmount(uint8_t ucLine, uint8_t *pszAmount);
void   DisplayAdjustTitle(void);
INT32  DisplayTransTotal(uint8_t bShowVoidTrans);
void   DisplayTranLog(UINT32 uiActRecNo, void *pTranLog);
void   DisplayResult(INT32 iErrCode);

void   DisplaySwipeCard(void);
void   DisplaySwipeOnly(void);
void   DispGetMagInfoErr(void);
void   DisplayInsertCard(void);
void   DispFallBackSwipeCard(void);
void   PromptRemoveICC(void);
void   PromptFallBack(void);
void   DisplayKeyInOnly(void);

void   DisplayBlockFunc(void);
void   DisplayProcessing(void);
void   DisplayDialing(void);
void   DisplaySending(void);
void   DisplayReceiving(void);
void   DispAccepted(void);
void   DisplayClearOk(void);
void   DisplayTransLogState(UINT32 uiStatus);
void   DisplayOperOk(void *pszChnMsg, void *pszEngMsg);
int    DisplayWarningPromt(void);

void   DisaptEmvTransFail(int iErrCode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _UITL_H

// end of file
