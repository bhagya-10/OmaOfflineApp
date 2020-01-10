
/****************************************************************************
NAME
    main.h - ����������¼�����ģ��

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

#define APPINIT_EVENT     0x01     // ����ϵͳ��ʼ���¼���Ϣ
#define MAGCARD_EVENT     0x02     // ˢ���¼���Ϣ
#define ICCARD_EVENT      0x03     // IC���忨�¼���Ϣ
#define KEYINPUT_EVENT    0x04     // �����¼���Ϣ

typedef struct _tag_MSG_INFO
{
	int              iMessage;		 // APPINIT_EVENT....
	int              wParam;         // ��Ϣ���͵ĸ���ֵ�����簴����Ϣ�ļ�ֵ��
	ST_MAGCARD_INFO  stMagInfo;      // �ſ���Ϣ�Ĵŵ���Ϣ
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

//��Ƶ��
int    GetOneSectorData(int iSectorNum, char *psKeyA, char*psUid);
int    GetAllDataFromM1Card(void);
INT32  ContactlessCardProc(void);
int     ChkContactlessCardInsert(void);
int emv_test(void);

#define KEYA_SECTOR0        "\xFF\xFF\xFF\xFF\xFF\xFF\x00"



#ifdef __cplusplus
}
#endif /* __cplusplus */


extern ST_MSG_INFO 	glstMsgInfo;	// �¼���Ϣ

#endif	// _EVENTMSG_H

// end of file
