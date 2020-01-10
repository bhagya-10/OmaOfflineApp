
/****************************************************************************
NAME
    main.h - 定义管理器事件处理模块

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.01      - created
****************************************************************************/

#ifndef _MAIN_H
#define _MAIN_H

typedef struct _tag_MAGCARD_INFO
{
	char  szTrack1[256];
	char  szTrack2[256];
	char  szTrack3[256];
	int   iRetCode;
}ST_MAGCARD_INFO;

#define APPINIT_EVENT     0x01     // 开机系统初始化事件消息
#define MAGCARD_EVENT     0x02     // 刷卡事件消息
#define ICCARD_EVENT      0x03     // IC卡插卡事件消息
#define KEYINPUT_EVENT    0x04     // 按键事件消息

typedef struct _tag_MSG_INFO
{
	int              iMessage;		 // APPINIT_EVENT....
	int              wParam;         // 消息类型的附加值，例如按键消息的键值等
	ST_MAGCARD_INFO  stMagInfo;      // 磁卡消息的磁道信息
}ST_MSG_INFO;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int  ProcAppEvent(void);
int  ProcAppInitEvent(void);
int  ProcMagEvent(void);
int  ProcICCEvent(void);
int  ProcKeyEvent(void);

int  CheckOpereateLogon(void);
int  CheckAutoSettlement(void);

//射频卡
int    GetOneSectorData(int iSectorNum, char *psKeyA, char*psUid);
int    GetAllDataFromM1Card(void);
INT32  ContactlessCardProc(void);
int     ChkContactlessCardInsert(void);
int emv_test(void);

#define KEYA_SECTOR0        "\xFF\xFF\xFF\xFF\xFF\xFF\x00"



#ifdef __cplusplus
}
#endif /* __cplusplus */


extern ST_MSG_INFO 	glstMsgInfo;	// 事件消息

#endif	// _EVENTMSG_H

// end of file
