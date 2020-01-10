#ifndef     _NEWTMSLIB_H_
#define     _NEWTMSLIB_H_

#include "tmslib.h"
#include "servpack.h"
#include "termpack.h"
#include "define.h"

#define  MODULE_FILE_NAME	"ͨ������"

//#define _ZL_DEBUG_   //���������������


//  �ļ�����
#define     FILE_TYPE_BOOTLOADER    0x80    //  BOOT LOADER���
#define     FILE_TYPE_BOOT          0x81    //  BOOT���
#define     FILE_TYPE_MONITOR       0x82    //  MONITOR���
#define     FILE_TYPE_CONFIG        0x90    //  ϵͳ�����ļ�
#define     FILE_TYPE_FONT          0xA0    //  �ֿ��ļ�
#define     FILE_TYPE_TMS           0xA1    //  tms��ʱ�ļ�
#define     FILE_TYPE_PED           0xB0    //  PED�ļ�

#define     FILE_TYPE_APP           0x01    //  0x01    //  Ӧ�ó���
#define     FILE_TYPE_DSRM          0x02    //  0x07    //  Ӧ�ó�����ʾע���ļ�
#define     FILE_TYPE_DATA          0x03    //  0x04    //  Ӧ�ó��򴴽��������ļ�
#define     FILE_TYPE_PARA          0x04    //  0x06    //  �����ļ�
#define     FILE_TYPE_ASPUBKEY      0x11    //  �յ���ǩ����Կ�ļ�
#define     FILE_TYPE_AAPUBKEY      0x12    //  �յ���KISSA��Ȩ��Կ�ļ�

#define     LD_OK                   0x00        //  �ɹ�
#define     LDERR_GENERIC           0x01        //  ʧ��
#define     LDERR_BAUDERR           0x03        //  �����ʲ�֧��
#define     LDERR_INVALIDTIME       0x04        //  �Ƿ�ʱ��
#define     LDERR_CLOCKHWERR        0x05        //  ʱ��Ӳ������
#define     LDERR_SIGERR            0x06        //  ��֤ǩ��ʧ��
#define     LDERR_TOOMANYAPP        0x07        //  Ӧ��̫�࣬�������ظ���Ӧ��
#define     LDERR_TOOMANYFILES      0x08        //  �ļ�̫�࣬�������ظ����ļ�
#define     LDERR_NOAPP             0x09        //  ָ��Ӧ�ò�����
#define     LDERR_UNKNOWNAPP        0x0A        //  ����ʶ���Ӧ������
#define     LDERR_SIGTYPEERR        0x0B        //  ǩ�����������ͺ������������Ͳ�һ��
#define     LDERR_SIGAPPERR         0x0C        //  ǩ��������������Ӧ��������������Ӧ������һ��
#define     LDERR_WRITEFILEFAIL     0x10        //  �ļ�д��ʧ��
#define     LDERR_NOSPACE           0x11        //  û���㹻�Ŀռ�
#define     LDERR_RESENDLASTPACK    0x20        //  У�����Ҫ�����´������һ����
#define     LDERR_UNSUPPORTEDCMD    0xFF        //  ��֧�ָ�����


#define     NEWTMS_STX                      0x02
#define   	NEWTMS_ETX						 0x03
#define     NEWTMS_HANDSHAKE_SEND           "HELLO"
#define     NEWTMS_HANDSHAKE_RECV           "READY"

#define     NEWTMS_INFO_FACTORY             "NEWPOSTECH"    //  16λ����������д

#define     NEWTMS_MAX_TASKNUM              127         //  ����������Ŀ


#define     NEWTMS_MAX_MODEM_FRMLEN         1024  //  1024        //  modem asynһ֡���ݵ����ֵ

#define     NEWTMS_MAX_UART_FRMLEN          2048  //  8192        //  uartһ֡���ݵ����ֵ
#define     NEWTMS_MAX_LAN_FRMLEN           2048  //  8192        //  LANһ֡���ݵ����ֵ
#define     NEWTMS_MAX_SOCK_FRMLEN          2048  //  1024        //  sockһ֡���ݵ����ֵ

#define     NEWTMS_BAGS                     20     //  һ�����յİ�����

#define     NEWTMS_MAX_RECVSIZE             409600         //  һ��������������ֵ

#define     NEWTMS_SENDPACK_TIMEOUT         50       //  �����͵���ʱʱ��(ms)
#define     NEWTMS_RECPACK_TIMEOUT          50       //  �����͵���ʱʱ��(ms)
#define     NEWTMS_REC_TIMEOUT              30       //  �����͵���ʱʱ��(ms)

//����ʱ��
#define		NEWPOS_SHAKEHAND_TOTALTIME		180000		//������ʱ��
#define		NEWPOS_MODEM_SHAKEHAND_TIME	    20000        //  modem�����ط�ʱ��(ms)
#define		NEWPOS_TIMEOUT_TIME		20000		//�����쳣ʱ��


//  ������
#define     NEWTMS_CMD_HANDSHAKE            0x70        //  ����
#define     NEWTMS_CMD_REQAUTH              0x71        //  ������֤
#define     NEWTMS_CMD_ANSAUTH              0x72        //  Ӧ����֤
#define     NEWTMS_CMD_DLDATA               0x73        //  ��������
#define     NEWTMS_CMD_UPLDATA              0x74        //  ��������
#define     NEWTMS_CMD_EXIT                 0x75        //  �˳�����


#define TMS_ERROR_UNKOWN_TID			9001		//δ֪TID
#define TMS_ERROR_UNKOWN_SN				9002		//δ֪SN��
#define TMS_ERROR_UNKOWN_MANUFACTURES	9003		//δ֪����
#define TMS_ERROR_UNSPT_PACK_LEN		9004		//��֧�ֵ����ݰ�����
#define TMS_ERROR_UNSPT_COMPRESS_VER	9005		//��֧�ֵ�ѹ���㷨
#define TMS_ERROR_UNSPT_ENCRYPT_VER		9006		//��֧�ֵļ����㷨
#define TMS_ERROR_UNSPT_VERIFY_VER		9007		//��֧�ֵ�У���㷨
#define TMS_ERROR_UNSPT_COM_MODEL		9008		//��֧�ֵ�ͨѶ��ʽ
#define TMS_ERROR_UNSPT_AUTH_VER		9009		//��֧�ֵ������֤��ʽ
#define TMS_ERROR_AUTH_ERROR			9010		//�����֤����
#define TMS_ERROR_TASK_ERROR			9011		//ȡ�����б����
#define TMS_ERROR_UNKOWN_TASK_INDEX		9012		//�����������
#define TMS_ERROR_UNKOWN_FILE_ADDR		9013		//������ļ�ƫ�Ƶ�ַ
#define TMS_ERROR_UNKOWN				9014		//λ�ô���
#define TMS_ERROR_TIMEOUT				9015		//��ʱ��
#define TMS_ERROR_VERIFY_ERROR			9016		//У���
#define TMS_TASKLIST_NULL				9017		//�����б�Ϊ��
#define TMS_TASK_FORRBIDEN				9018		//�ն˱���ֹ����

#define ums_api_enter(fmt, arg...)	printf(fmt, ##arg);

#define MIN(a, b)       ( (a)<=(b) ? (a) : (b) )

typedef struct
{
    int32_t     cmdcode;
    int32_t     retcode;
    int32_t     len;
    uint8_t     data[NEWTMS_MAX_RECVSIZE*2];
}newtms_pack_t;

typedef struct
{
    uint8_t     termID[16];             //  16λ�����ն˺�
    uint8_t     sn[16];                 //  16λSN��
    uint8_t     vender[16];             //  16λ����������д
    uint8_t     postype[16];            //  16λ���ն�������
    uint16_t    packlen;                //  2λ��ͨѶ���ݰ�����
    uint8_t     Compress;               //  1byte֧��ѹ���㷨
    uint8_t     encrypt;                //  1byte֧�ּ����㷨
    uint8_t     datacheck;              //  1byte֧��У���㷨
    uint8_t     commtype;               //  1λͨѶ��ʽ
    uint8_t     hardver[16];            //  16λӲ���汾��
    uint8_t     softver[16];            //  16λ����汾��
    uint8_t     authtype;               //  1λ�����֤��ʽ
    uint8_t     randdata[8];            //  8λ�����R1
}newtms_tinfo_t;
typedef struct
{
    int32_t     tasklen;                //  �����ļ�����
    uint8_t     checksum[20];           //  �����ļ���hashУ��ֵ
    uint8_t     filecompress;           //  �Ƿ����ļ�ѹ��
    uint8_t     fileencrypt;            //  �Ƿ����ļ�����
    uint8_t     flag[NEWTMS_MAX_TASKNUM+1]; //  ÿһ������ĸ���״��, 0:��������,1:�Ѿ��������,2:������� 
    uint8_t     RemoveSameTask;         //  �Ѿ���������ص�������������е��ļ���һ����
    uint8_t     reserve[1];             //  Ԥ��
}newtms_taskfile_t;

typedef struct   //add andy.wang 2013-9-22 19:14:40 ��Ҫ��Ϊ����ʾһ��Ҫ���ص�һ���ĸ����ʹ���
{
	int32_t 	itotaldownsize;  //�ܹ����صĴ�С
	int32_t     icurrdownsize;		//��ǰ���صĴ�С
}newtms_down_order;

//  �������Ͷ���
#define CREATE_FILE_SYS         0x01	//  �����ļ�ϵͳ
#define KERNEL_FILE_DOWN        0x02	//  Kernel�ļ�
#define USPUK_FILE_DOWN         0x03	//  US PUK�ļ�
#define FONT_FILE_DOWN          0x05	//  �ֿ��ļ�
#define APP_FILE_DOWN           0x06	//  Ӧ�ó���
#define DSRM_FILE_DOWN          0x07	//  DSRM�ļ�
#define DATA_FILE_UP            0x09	//  �����ļ��ϴ�
#define DELETE_ALL_APP          0x0A	//  ɾ������Ӧ��
#define DELETE_SOME_APP         0x0B	//  ɾ��ָ��Ӧ��
#define DELETE_FILE_LINUX       0x13    //  7110�ļ�ɾ��

#define NEWCONFIG_NAME     "NewConfig.ini"  //add andy.wang 2014-1-10 13:04:15


typedef struct
{
    uint8_t     identifier[2];      //  ֵΪ"TH"
    uint8_t     file_version;       //  ��ʽ�汾����ǰ����0x01
    uint8_t     impress;            //  �Ƿ�ǿ�Ƹ���
    uint8_t     file_headLen[4];    //  �ļ�ͷ�Ĵ�С
    uint8_t     task_number[4];     //  ������Ŀ
    uint8_t     reserve[4];         //  Ϊ�˴�16λ��
}T_Task_FileHead;     //16

typedef struct
{
    uint8_t     task_no;            //  ������
    uint8_t     task_type;          //  ��������
    uint8_t     parity_type;        //  У������
    uint8_t     download_control;   //  ���ؿ��ƣ�bit0��ʾǿ������
    uint8_t     file_len[4];        //  �ļ���С
    uint8_t     app_name[32];       //  ����Ӧ����
    uint8_t     file_name[32];      //  �洢�ļ��� //����Ӧ�����ʹ洢�ļ����ϲ�Ϊ7110���ļ������ܹ�64���ֽڣ�app_name��file_name�����������м䲻�������ӱ���
    uint8_t     parity[20];         //  У��ֵ
  //  uint8_t     szversion[4];        //
    uint8_t     enable_time[6];     //  ����ʱ��
    uint8_t     reserve[6];         //  �����ֽ�
}T_Task_Info;       //104 bytes

void s_newtmsTaskRemove(int32_t taskid);
void s_newtmsTaskRead(void);
int32_t s_newtmsTaskWrite(int32_t mode);
void PrintFormat(char *prompt,uint8_t *src,int Len);
int32_t s_newtmsSendPack(newtms_pack_t *senddata);
int32_t s_newtmsRecvPack(ServPack_t *sp, TermPack_t *tp,newtms_pack_t *recvdata);
int32_t s_newtmsExchange(ServPack_t *sp, TermPack_t *tp,newtms_pack_t *senddata, newtms_pack_t *recvdata);
int32_t s_newtmsGetData(int32_t ctrlflag, uint8_t *srcData, uint8_t *dstData, uint32_t srcLen);
int32_t s_newtmsDlData(ServPack_t *sp, TermPack_t *tp,int32_t taskid, int32_t begaddr, int32_t endaddr, int32_t totallen, uint8_t *databuf,int32_t fileid);
int32_t s_newtmsExit(int32_t retcode);
int32_t s_newtmsSetTermInfo(void);
int32_t s_newtmsHandshake(int32_t  timeoutms);
int32_t s_newtmsVerify(newtms_taskfile_t *newtask);
int32_t s_newtmsDlTask(newtms_taskfile_t *newtask);
int32_t s_newtmsDlFile(int32_t taskid, int32_t totaltask, T_Task_Info *ptaskinfo);
int32_t s_newtmsDlAllFile(void);
int32_t s_newtmsRetConvert(int32_t ret);
int32_t s_newtmsUpdateTask(int32_t taskid, T_Task_Info *ptaskinfo);
int rm_file(const char* pathname);
int32_t newtmsDownload(tmsctrl_t *ctrlpara);
int32_t newtmsRemoveTask(int32_t prompt);
int32_t newtmsUpdate(const UMODULEINFOLIST * st_ModuleList,int32_t prompt, int32_t *UpdateNum, int32_t *RemainNum);
int32_t s_newtmsSaveFile(const char *pSrcName, const char *pDstName, const uint8_t TaskType, uint8_t uczipFlag);
int32_t s_newtmsGetElfType(const char *filename);
int32_t s_newtmsUpdateFirmware(void);
int s_newtmsRemoveFile(const char *srcFileName);
int tms_create_dir(const char *pathname);
int tms_dele_dir(const char *pathname);
void MakeUmsTmsProtocol(uint8_t *pszbuf, int *psLen);
int32_t GetDownSizeByCommType(void);
int s_newtmsCheckIfParaFile(const char *pszparaname);
int Utms_Get_Module_Name_type(T_Task_Info *ptaskinfo, char *modeName, char *appname);
int s_newtmsGetLibPath(char pszlibPath[][256], int *libpathNum);
int s_newtmsDeleteUnuseModule(void);
int s_newtmsDeletePrivateModule(void);
int tms_Get_ModuleInfo(uint8_t *pszVersion, uint8_t *pszAppPath);
int s_newtmsCheckIfDownPrivate(uint8_t *szappname, int8_t type);
int s_newtmsCheckIfPrivateLibDown(char *pszappname, T_Task_Info *ptasktemp, int32_t tasknum);
int tmsGetVersionByNewConfig(char *pszVersion, char *pszDir, char *pszAppName);
void s_newtmsClearRecvBuf(int32_t timeoutms);
int s_newtmsCheckIfNeedUzip(const char *pszparaname);
int GetCurrentDir(char *PszCurrentDir);
void tmsReadTask(void);
int tmsGetModuleInfo(UMODULEINFOLIST *st_ModuleList, char ucflag);

#endif

