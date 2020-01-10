
/****************************************************************************
NAME
    fileoper.h - 定义所有文件操作的函数

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.01      - created
****************************************************************************/

#ifndef _FILEOPER_H
#define _FILEOPER_H

#ifndef ERR_BASE		
#define ERR_BASE			0x10000
#endif

#define ERR_FILEOPER		(ERR_BASE+0x100)
#define ERR_NOT_FILE		(ERR_BASE+0x101)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int    InitTranLogFile(void);
int    ExistAppTransParam(void);
int    ExistCtrlParam(void);
int    LoadAppTransParam(void);
int    SaveAppTransParam(void);
int    SaveEdcParam(void);
int    SavePassword(void);
int    LoadAppCtrlParam(void);
int    SaveSysCtrlAll(void);
int    SaveSysCtrlBase(void);
int    SaveSysCtrlNormal(void);
int    SaveField56(void);
int    SaveRevInfo(uint8_t bSaveRevInfo);
int    SaveRePrnStlInfo(void);
int    LoadTranLog(void *pstLog, uint uiIndex);
int    UpdateTranLog(void *pstLog, uint uiIndex);
int    SaveTranLog(void *pstLog);
int    RecoverTranLog(void);
int    GetTranLogNum(uint8_t ucAcqKey);
int    AllowDuplicateTran(void);
int    GetRecord(uint uiStatus, void *pstOutTranLog);
void   CalcTotal(uint8_t ucAcqKey);
int    SaveEmvStatus(void);
int    LoadEmvStatus(void);
int    SaveEmvErrLog(void);
int    LoadErrLog(UINT32 uiRecNo, void *pOutErrLog);
int    LastRecordIsFallback(void);

int    SaveCAPKLogFile(void *pstCapk);
int    LoadCAPKLogFile(UINT32 uiRecNo, void *pstCapk);
int    LoadAllCapkParam(void);

int    SaveAIDLogFile(void *pstAid);
int    LoadAIDLogFile(UINT32 uiRecNo, void *pstAid);
int    LoadAllAidParam(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _FILEOPER_H

// end of file
