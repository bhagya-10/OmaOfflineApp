
/****************************************************************************
NAME
    AppInit.h - 实现终端应用参数初始化

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.13      - created
****************************************************************************/

#ifndef _APPINIT_H
#define _APPINIT_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void LoadEdcDefault(void);
void LoadDefCommPara(uint8_t ucAcqIndex);
void ResetAllPara(uint8_t bFirstTime);
void RemoteDownLoad(void);
void LoadCtrlDefault(void);
void AdjustParam(void);
void ResetSysAfterRemoteDown(void); // 远程更新后重启系统
void RemoteManager(void);
void DeleteRemoteTask(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _INITPOS_H

// end of file
