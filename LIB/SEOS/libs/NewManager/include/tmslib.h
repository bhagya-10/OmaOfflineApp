#ifndef     _TMSLIB_H_
#define     _TMSLIB_H_

#include <stdint.h>
#include <modem_iface.h>
#include <modem.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C"{
#endif

////////////////////////////////////////////////////////////////

//TMS��API����ֵö��
typedef enum
{
    TMS_FAIL    = -1, //ʧ��
    TMS_OK      = 0 //�ɹ�
}TMSStatus;

//ģ������ö��
typedef enum
{
    UMODULE_FILE        = 0, //�ļ���2-5���ļ�����ʹ��0��������������
    UMODULE_PARAFILE    = 1, //�����ļ�
    UMODULE_TRADE       = 2, //ҵ��ģ��
    UMODULE_PUBLIC      = 3, //����ģ��
    UMODULE_API         = 4, //APIģ��
    UMODULE_PRIVATE     = 5, //����˽�п�
    UMODULE_DATA        = 6, //�������ļ�(Ԥ��)
    UMODULE_DRIVER      = 7, //Ӳ������(Ԥ��)
    UMODULE_OS          = 8, //�ײ�OS(Ԥ��)
    UMODULE_APP         = 9        //Ӧ�ã���ִ���ļ� add andy.wang 
}UModuleType;

#define UMAX_MODULE_COUNT   100 //�����ļ��������

//ģ����Ϣ�ṹ��
typedef struct
{
    char ModuleName[50]; //ģ����
    int ModuleType; //�μ�enum UModuleType
    char AppName[20+1];      //Ӧ�������������ļ�����ĳ��Ӧ�� add andy.wang
}UMODULEINFO;

//ģ����Ϣ�ܽṹ��
typedef struct
{
    uint8_t bCount; //ʵ�������ļ���
    UMODULEINFO ModuleInfo[UMAX_MODULE_COUNT];//�μ�struct UMODULEINFO
}UMODULEINFOLIST;

#define UMAX_APP_COUNT      30 //Ӧ���������

//Ӧ����Ϣ�ṹ��
typedef struct
{
    char AppName[20+1]; //Ӧ����
    char AppDispName[20+1]; //Ӧ����ʾ��,�������01000006��ǩ,���ͱ�ǩ����
    char AppVar[15+1];        // Ӧ�ð汾�� tmsϵͳֻ֧��15λ�汾�� add wgz
    char TerminalID[8+1]; //�ն˺�
    char ShopId[15+1]; //�̻���
    uint8_t bisMainApp; //�Ƿ�����Ӧ�ã�0-��1-��
}UAPPINFO;

//Ӧ����Ϣ�ܽṹ��
typedef struct
{
    uint8_t bCount; //ʵ��Ӧ����
    UAPPINFO AppInfo[UMAX_APP_COUNT]; //�μ�struct UAPPINFO
}UAPPINFOLIST;

typedef struct
{
    char AppName[20+1];         //Ӧ��ģ����
    char AppDispName[20+1];     //Ӧ����ʾ��
    char AppVar[15+1];          // Ӧ�ð汾�� tmsϵͳֻ֧��15λ�汾��
    char TerminalID[8+1];       //�ն˺�
    char ShopId[15+1];          //�̻���
    char ParamPath[256];       //���ǲ����ļ�·��
    unsigned int AppRunNum;     // Ӧ������
    char AppRunFlag;            // �Ƿ�����
    char AppUpdateTime[15];     // ģ�����ʱ��
    char AppUpdateTimeEnd[15];  // ģ����½�ֹʱ��
    unsigned char bisMainApp;   //�Ƿ�����Ӧ�ã�0-��1-��
    uint8_t REF[104];//Ԥ��
}UAPPINFOmain;

typedef struct
{
    unsigned char  bCount;                       //ʵ��Ӧ����
    UAPPINFOmain AppInfoMain[UMAX_APP_COUNT];//�μ�struct UAPPINFO
}UAPPINFOLISTmain;

////////////////////////////////////////////////////////////////

typedef struct ST_PUB_APP_INFO
{
    char gbOnlineReportBegin[16];   //������ʼʱ��
    char gbOnlineReportEnd[16];     //��������ʱ��
    char gbOnlineReportLastTime[16];//ͬһ���������һ���Զ�Ѳ��ʱ��
    int  gbOnlineReportNextPassDate;//�´��Զ���������ʱ��,������ڣ���
    int  iAutoReportNum;
    int  iManualSetReportFlag;      //�ֶ������Զ�Ѳ�쿪��
    int  iAutoReportFlag;           //��Ӧ���л����غ���Ҫ���Զ�Ѳ�쿪��
    int  iIntervalTime;             //�ֹ������Զ�����ʱ�䣬��λ:Сʱ
    char TmsDownLoadStartTime[16];   //�Զ����ؿ�ʼʱ��
    char TmsDownLoadEndTime[16];     //�Զ����ؽ���ʱ��
    char TmsDownLoadLastTime[16];    //ͬһ���������һ���Զ�����ʱ��
    int  iAutoDownloadNum;           
    int  iAutoDownloadFlag;         //��Ӧ���л����غ���Ҫ���������ؿ���
    char szManageVersion[20];   //lileitang 2014.08.01 10:59 ����֧��Դ��汾
    char cRegistRslt;                   //lilt 2014.08.22 11:47 �ն˲��ǽ�� 1-�ɹ� 0-ʧ�� 3-δִ��(��Ӧ��Ҫ��ִ�в���)

    char ucActivateFlag;
    char ucDownloadFlag;   // 1 ǿ�Ƹ���   0 ѡ����� 

    // 1 byte which save pos cur use common comm type //use for sync all sub app comm type
    char szCommType; // 'C'->CDMA 'G'->GPRS 'T'->TCP/IP 'W'->WIFI 'R'->RS232 'M'->MODEM  'A'->ALL
    char szREFU[298];

}ST_PUB_APP_INFO;

/////////////////////////////////////////////////////////////////

#ifndef TRUE
    #define TRUE                1
    #define FALSE               0
    #define USER_CANCEL         2
#endif

#define     TMS_PROMPT_EN       0       //  prompt in English
#define     TMS_PROMPT_CHN      1       //  prompt in Chinese
#define     TMS_PROTO_NEWTMS    0       //  tms protocol: newtms

#define     TMS_COMM_GPRS       1       //  uses socket comm(gprs, cdma, lan...)
#define     TMS_COMM_CDMA       2       //  uses socket comm(gprs, cdma, lan...)
#define     TMS_COMM_PPP        3       //  uses PPP comm
#define     TMS_COMM_MDMASY     4       //  uses MODEM ASYN
#define     TMS_COMM_UART       5       //  uses RS232 comm
#define     TMS_COMM_TCPIP      6       //  uses LAN comm
#define     TMS_COMM_WIFI       7       //  uses LAN comm

typedef struct
{
    int32_t     protocol;       //  tms protocol
    int32_t     prompt;         //  prompt control on LCD
    int32_t     enbreak;        //  whether CANCEL key is enabled to terminate downloading, YES/NO
    int32_t     commtype;       //  communication type
    int32_t     commport;       //  the handle of communication(port No., sockfd)
    uint8_t     termID[16];     //  terminal ID
    int32_t     specifiedtmk;   //  whether the tmk is given by application, YES/NO
    int32_t     tmklen;         //  the tmk length
    uint8_t     tmk[24];        //  the tmk content
    uint8_t     sztpdu[6];        // tms tpdu ����һ��tpdu add andy.wang 2013-8-25 14:18:23
    uint8_t     szNetCtrlFlag;  // flag for net contrller enable ,  add  by liluchang 2015-11-25 
    uint8_t     szModemMode;    //modem dial mode // 1->SYNC  0->ASYNC
    int32_t     szNetCtrlMaxPckLen;  //max length of a packet with net controller //0 means use default
}tmsctrl_t;

//  return code from terminal:
#define TMS_ERROR_CANCEL                1100    //  user cancel
#define TMS_ERROR_DATAERR               1101    //  data error
#define TMS_ERROR_HANDSHAKE             1102    //  handshake error
#define TMS_ERROR_AUTH                  1103    //  auth key error
#define TMS_ERROR_CHECKSUM              1104    //  check sum error
#define TMS_ERROR_NOSPACE               1105    //  no space to save
#define TMS_ERROR_TASKNUM               1106    //  too many task num
#define TMS_ERROR_NOTASK                1107    //  no task to update
#define TMS_ERROR_UNCOMPLETE            1108    //  download not complete
#define TMS_ERROR_SIGERR                1109    //  signature error
#define TMS_ERROR_WRITEFAIL             1110    //  write file error
#define TMS_ERROR_NOFILE                1111    //  can not find file
#define TMS_ERROR_TOOMANYAPP            1112    //  too many app
#define TMS_ERROR_UNKNOWNAPP            1113    //  unknown app format
#define TMS_ERROR_SIGTYPEERR            1114    //  signature type error
#define TMS_ERROR_NOAPP                 1115    //  no app
#define TMS_ERROR_SIGAPPERR             1116    //  app's signature error
#define TMS_ERROR_TASKTYPE              1117    //  unsupport task type.
#define TMS_ERROR_REMOTE_CLOSE          1118    //remote service close socket  add andy.wang 2013-9-14 18:00:27
#define TMS_ERROR_PARAM                 1119    //input param invalid

#ifndef ENOTASK
#define ENOTASK                         TMS_ERROR_NOTASK
#endif

#define E_TIMEOUT                       2       //time out

#define E_CANCEL                        1100    //  user cancel
#define E_DATAERR                       1101    //  data error
#define E_HANDSHAKE                     1102    //  handshake error
#define E_AUTH                          1103    //  auth key error
#define E_CHECKSUM                      1104    //  check sum error
#define E_NOSPACE                       1105    //  no space to save
#define E_TASKNUM                       1106    //  too many task num
#define E_NOTASK                        1107    //  no task to update
#define E_UNCOMPLETE                    1108    //  download not complete
#define E_SIGERR                        1109    //  signature error
#define E_WRITEFAIL                     1110    //  write file error
#define E_NOFILE                        1111    //  can not find file
#define E_TOOMANYAPP                    1112    //  too many app
#define E_UNKNOWNAPP                    1113    //  unknown app format
#define E_SIGTYPEERR                    1114    //  signature type error
#define E_NOAPP                         1115    //  no app
#define E_SIGAPPERR                     1116    //  app's signature error
#define E_TASKTYPE                      1117    //  unsupport task type.


//  return code from TMS server:
#define TMS_ERROR_UNKOWN_TID            9001        //  unknown TID
#define TMS_ERROR_UNKOWN_SN             9002        //  unknown SN
#define TMS_ERROR_UNKOWN_MANUFACTURES   9003        //  unknown vender name
#define TMS_ERROR_UNSPT_PACK_LEN        9004        //  unsupport data packet length
#define TMS_ERROR_UNSPT_COMPRESS_VER    9005        //  unsupport compress type
#define TMS_ERROR_UNSPT_ENCRYPT_VER     9006        //  unsupport encryption type
#define TMS_ERROR_UNSPT_VERIFY_VER      9007        //  unsupport checksum type
#define TMS_ERROR_UNSPT_COM_MODEL       9008        //  unsupport comm type
#define TMS_ERROR_UNSPT_AUTH_VER        9009        //  unsupport authentication type
#define TMS_ERROR_AUTH_ERROR            9010        //  authenticate failed
#define TMS_ERROR_TASK_ERROR            9011        //  get task error
#define TMS_ERROR_UNKOWN_TASK_INDEX     9012        //  task index error
#define TMS_ERROR_UNKOWN_FILE_ADDR      9013        //  unknown file addr
#define TMS_ERROR_UNKOWN                9014        //  unknown error
#define TMS_ERROR_TIMEOUT               9015        //  timeout
#define TMS_ERROR_VERIFY_ERROR          9016        //  checksum error
#define TMS_TASKLIST_NULL               9017        //  task is empty
#define TMS_TASK_FORRBIDEN              9018        //  the terminal is prevented to download
#define TMS_ERROR_FILE_NOTEXIST         9019        //  can not find the file to download
#define TMS_ERROR_SERVER_MEMORY_ERROR   9020        //  server memory error
#define TMS_ERROR_FILE_READ_ERROR       9021        //  reading file error
#define TMS_TRANDATA_MULTI_PACK         9022        //  return code of multi-packet(not an error code)
#define TMS_ERROR_APP_FILE_ERROR        9023        //  reading app file error
#define TMS_ERROR_PATH_ERROR            9024        //  can not find the path of downloading file
#define TMS_ERROR_FILE_CHANGED_ERROR    9025        //  the downloading file is updated

int32_t tmsSetDispLang(int32_t mode);
void    tmsShowRet(int32_t Col, int32_t Line, int32_t Mode, int32_t Retcode);

int32_t tmsDownload(tmsctrl_t *ctrlpara);
int32_t tmsSetModemHead(const void *head, uint32_t nbytes);
int32_t tmsSetTpdu(const void *tpdu, uint32_t nbytes);
int32_t tmsSetVersion(const void *info, uint32_t nbytes);
int32_t tmslibSend(int HandleId,char *Data,int DataLen);
int32_t tmslibRecv(int HandleId,char *Data,int DataLen,int TimeOut,int iFlag);
    
extern int32_t s_tmsGetDispLang(void);
#define tmsDisp(col, line, mode, ch, en, arg...)    \
    do {    \
        if((s_tmsGetDispLang() == TMS_PROMPT_CHN) && (((mode)&DISP_CFONT) == DISP_CFONT))\
            lcdDisplay(col, line, mode, ch, ## arg);\
        else    \
            lcdDisplay(col, line, mode, en, ## arg);\
    } while(0)
#define myprintf(fmt, arg...) 
//printf(fmt, ##arg)

/////////////////////////////////////////////////////////////////

//switch for socket send disp info
#define EN_SOKCET_DISP_INFO

#ifndef EN_SOKCET_DISP_INFO  //old methord 

int32_t TMS_DownLoadApp(tmsctrl_t *ctrlpara, UMODULEINFOLIST *st_ModuleList);
int32_t tmsUpdate(const UMODULEINFOLIST *st_ModuleList, int32_t prompt, int32_t *UpdateNum, int32_t *RemainNum);
int32_t tmsRemoveTask(int32_t prompt);

#else  //new methord //send disp info by socket

int32_t TMS_DownLoadApp(int Fd, tmsctrl_t *ctrlpara, UMODULEINFOLIST *st_ModuleList);
int32_t tmsUpdate(int Fd, const UMODULEINFOLIST *st_ModuleList, int32_t prompt, int32_t *UpdateNum, int32_t *RemainNum);
int32_t tmsRemoveTask(int Fd, int32_t prompt);


///////////////////////////////////////////about disp info

#define LCD_CLS             1000
#define LCD_NO_LINE         2000
#define LCD_COLOR_MODE      3000
#define KB_HIT_SOCKET       -1000

int32_t New_Lcd_Disp_Init(int ClsStartLine, int ClsEndLine, int KeyWaitTime, int SysBeef_a, int SysBeef_b);
int32_t New_Lcd_Disp_Add(int Col, int Line, uint32_t Mode, const char *format_ch, const char *format_en, ...);
int32_t New_Lcd_Disp_Flush(int *pFd);

int32_t Proc_Disp_Info(int *pFd, int (*pUpdate_Client_Info_Func)(int  Tag, void *MsgData));
void    Ctrl_Lib_Send_Trans_Info(int NewFd);
void    Get_Libtms_Ver(char *pVerOut);
int32_t Get_Cur_Download_Per_Str(char *pPerStr);
//////////////////////////////////////////////////////////////

#ifndef KB_HIT
#define KB_HIT          0
#endif

#define FRONT_PROCESS   0
#define BG_PROCESS      1

//front process which can't get key directly
void    LibTms_Key_Init(int ProcessType, void *pArg);
void    Clr_Old_Key_Input(void);
int32_t Get_Key_Input(int WaitTime);//unit : ms

//background process which can check key 
void    Test_Send_Key_Signal(void);
int32_t Check_Key_Input(int WaitTime); //unit : ms
//pNewDir, can't add space in the middle like this "123 tmp" !
int32_t Set_Tms_Define_Dir(char *pNewDir);

#endif

///////////////////////////////////////////////////////////////

//ssl function
void Comm_Ssl_Enable(int SslMode); // SslMode : 0, no ssl ; 1, ssl enable; default is 0
int  Comm_SSL_DisConnect(int SslHandle, int closeMode);
int  Comm_SSL_Connect(int SslHandle, int nTimeout);
int  Comm_SSL_Send(int SslHandle, const uint8_t *psBuf, int nBufLen);
int  Comm_SSL_Receive(int  SslHandle, uint8_t *psBuf, int uiExpLen, int nTimeout, int *pnBufLen);



#ifdef __cplusplus
}
#endif

#endif
