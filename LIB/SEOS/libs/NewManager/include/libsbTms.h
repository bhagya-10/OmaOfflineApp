/* This is the file which defined for trans process which run in the background !*/

#ifndef __LIB_SB_TMS_H__
#define __LIB_SB_TMS_H__

#include "stdint.h"

#ifndef INT32S
#define INT32S      signed int
#endif


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum
{
    Ret_None        = 0, //none
    Comm_Error      = 1, //socket comm error
    
    //The follow 3 value is used for starting translation
    Accept_Trans    = 2, //accept translation
    Refuse_Trans    = 3, //translation was refused
    Req_Tag_Error   = 4, //the req tag is unsupported

    //The follow 4 value is used for checking translation result
    Trans_Do_None   = 5, //have no translation now
    Trans_Doing     = 6, //translation is doing now
    Trans_Done      = 7, //translation has been done
    Trans_Error     = 8, //like download app fail
}TRANS_RET_CODE;

typedef enum
{
    NO_UPDATE_TYPE  = 0,
    FORCE_UPDATE    = 1,
    SELECT_UPDATE   = 2,
    ALL_AUTO_UPDATE = 3,
}APP_UPDATE_TYPE;

#ifndef SBTMS_RET_OK
#define SBTMS_RET_OK    0
#endif

#ifndef SBTMS_RET_FAIL
#define SBTMS_RET_FAIL  -1
#endif


/////////////////////////////////////////////////////////////////////
/*
Input  : 
TRANS_RET_CODE *pRetCode, When it return from these functions, its value will be set.
Output :
0      : means this function excute success; you can get some info from (TRANS_RET_CODE *pRetCode)
-1     : means this function excute with some error ; 
         you can get some info from the input arg (TRANS_RET_CODE *pRetCode)
*/
//request server to return the result of a trans
INT32S Check_Auto_Inspect_Result(TRANS_RET_CODE *pRetCode, \
                APP_UPDATE_TYPE  *pAppUpdateType, char *pRemarksInfo);  
//request server do auto inspect autoly // it will check auto inspect time ffrist
INT32S Start_Auto_Inspect_Autoly(TRANS_RET_CODE *pRetCode);
//request server do auto inspect and show trans info at the same time
INT32S Start_Manu_Auto_Inspect(TRANS_RET_CODE *pRetCode, \
                void (*pDisplay_Trans_Info)(void *pArg), void *pArg);
//request server do auto inspect right now 
INT32S Start_Auto_Inspect_Now(TRANS_RET_CODE *pRetCode);

/////////////////////////////////////////////////////////////////////
/*
Input  : 
TRANS_RET_CODE *pRetCode, When it return from these functions, its value will be set.
Output :
0      : means this function excute success; you can get some info from (TRANS_RET_CODE *pRetCode)
-1     : means this function excute with some error ; 
         you can get some info from the input arg (TRANS_RET_CODE *pRetCode)
*/
//request server to return the result of a trans
//INT32S Check_Download_App_Result(TRANS_RET_CODE *pRetCode);
INT32S Check_Download_App_Result(TRANS_RET_CODE *pRetCode, APP_UPDATE_TYPE *pAppUpdateType);
//request server do download app autoly
INT32S Start_Download_App_Autoly(TRANS_RET_CODE *pRetCode);
//request server do download app and show trans info at the same time
INT32S Start_Manu_Download_App(TRANS_RET_CODE *pRetCode, \
                void (*pDisplay_Trans_Info)(void *pArg), void *pArg);
//request server do Download_App right now 
INT32S Start_Download_App_Now(TRANS_RET_CODE *pRetCode);

///////////////////////////////////////////////////////////////////
/*
Function : get the software version of this code project
Input    : A arg with the type char * (at least 20 bytes)
Output   : void
*/
void   Get_Ser_Cur_Software_Version(char *pVersion);

//////////////////////////////////////////////////////////////////
/*
Function : clear tmp app download files , and it will turn the original state just like nothing has happen.
Input    : void
Output   : 0 is Clr OK , other value is FAIL
*/
INT32S Clr_Tmp_App_Doanload_File(void);

//////////////////////////////////////////////////////////////////
/*
Function : Update new app file . It will cover the old app file.
Input    : void
Output   : 0 is Clr OK , other value is FAIL
*/
INT32S Update_App_Doanload_File(void);

/////////////////////////////////////////////////////////////////

//request server do terminal register right now 
//return 0 is ok , return -1 are fail
INT32S Start_Manu_Term_Reg(TRANS_RET_CODE *pRetCode, INT32S *pTrsRslt, \
                                void (*pDisplay_Trans_Info)(void *pArg), void *pArg);

//request server do manager log on trans right now 
//return 0 is ok , return -1 are fail
INT32S Start_Manu_Manage_Log_On(TRANS_RET_CODE *pRetCode, INT32S *pTrsRslt, \
                                void (*pDisplay_Trans_Info)(void *pArg), void *pArg);

/////////////////////////////////////////////////////////////////
typedef struct
{
    void (*pFunc)(void *pArg);
    void *pArg;
}CALL_BACK_FUNC;


////////////////////////////////////////////////////////////////
//Here are some definition or macro for config sbTms process

//sbTms comm config file define
#ifndef FILE_APP_LOG
#define FILE_APP_LOG            "SysParam.log"
#endif

#include "seos.h"           //which define UINT8
#include "modem_iface.h"  //which define struct ModemDialParms_t

// PSTN���ò���
typedef struct _tagST_PSTN_PARA_MAIN_
{
    uint8_t             ucSendMode;          // ���ݷ��ͷ�ʽ
    uint8_t             szTxnTelNo[100+1];   // ���ź���
    ModemDialParms_t    stDialPara;          // modem���Ų���
}ST_PSTN_PARA_MAIN; 

// RS232���ò���
typedef struct _tagRS232_INFO_MAIN_
{
    uint8_t     ucPortNo;           // Port #, COM1, COM2 ....
    uint8_t     ucSendMode;         // ���ݷ��ͷ�ʽ
    uint8_t     szAttr[20+1];       // ���ڴ򿪲���, "9600,8,n,1", ....
}ST_RS232_INFO_MAIN;

// TCP/IP���ò���
typedef struct _tagST_TCPIP_INFO_MAIN_
{
    uint8_t     szNetMask[15+1];
    uint8_t     szGatewayIP[15+1];
    uint8_t     szLocalIP[15+1];
    uint8_t     szRemoteIP[15+1];
    uint8_t     szRemotePort[5+1];
    uint8_t     szDNS[32+1];
}ST_TCPIP_INFO_MAIN;

// GPRS/CDMA configurations
typedef struct _tagST_WIRELESS_INFO_MAIN_
{
    uint8_t     szAPN[64+1];  ///���ź��� CDMA: #777; GPRS: cmnet
    uint8_t     szUID[64+1];
    uint8_t     szPwd[64+1];
    uint8_t     szSimPin[16+1];     // SIM card PIN
    uint8_t     szDNS[32+1];
    uint8_t     szRemoteIP[20+1];
    uint8_t     szRemotePort[5+1];
}ST_WIRELESS_INFO_MAIN;

// WIFI  AP ���ò���
typedef struct _tagST_WIFI_AP_INFO_MAIN_
{
    int         iChannel;                  //  Wireless LAN communication channel(0 - 13)����ͨѶ�ŵ�
    int         iSecurityType;             //  Sets the Wireless LAN security type. ���� ����ģʽ
    int         iWEPKeyIdx;                //  WEP key index (1 - 4) -- WEP��Կ��
    char        szListWEPKey[4][32];       //  4 sets of WEP Key  --- ��Կ����Կ
    char        szWPAPSK[64];              //  Personal Shared Key Pass-Phrase,
    char        szSSID[64];                //  SSID
}ST_WIFI_AP_INFO_MAIN;

// ͨѶģ��������Ϣ
typedef struct _tagST_COMM_CONFIG_MAIN_
{
    uint8_t                 ucCommType;           // ͨѶ����(RS232/modem/tcp...)
    ulong                   ulSendTimeOut;        // ���ͳ�ʱʱ��
    ST_RS232_INFO_MAIN      stRS232Para;          // RS232 ���ò���
    ST_TCPIP_INFO_MAIN      stTcpIpPara;          // TCP ���ò���
    ST_WIRELESS_INFO_MAIN   stWirlessPara;        // GRPS/CDMA����ͨѶ���ò���
    ST_PSTN_PARA_MAIN       stPSTNPara;           // modem ���ò���
    ST_TCPIP_INFO_MAIN      stWIFIIpPara;         // WIFI ͨѶģʽ���ò���
}ST_COMM_CONFIG_MAIN;

typedef struct _MULTI_APP_PARAM_MAIN_ 
{
    char tag[10];
    char value[82];
    char rfu[20];
}ST_MULTI_APP_PARAM_MAIN; 

//define manage comm param struct
typedef struct _POS_PARAM_STRC_MAIN_
{
    uint8_t szPhoneNum1[20];
    uint8_t szPhoneNum2[20];
    uint8_t szPhoneNum3[20];// �����첽����
    uint8_t szPhoneNum4[20];
    //��̫��
    uint8_t szPOSLocalIP[15+1];
    uint8_t szPOSLocalPort[15+1];
    uint8_t szPOSLocalGate[15+1];
    
    uint8_t szPOSRemoteIP[15+1];
    uint8_t szPOSRemotePort[5+1];
    uint8_t szPOSRemoteIP2[15+1];
    uint8_t szPOSRemotePort2[5+1];

    //WIFI
    uint8_t szPOSRemoteWifiIp[15+1];
    uint8_t szPOSRemoteWifiPort[5+1];
    uint8_t szPOSRemoteWifiIp2[15+1];
    uint8_t szPOSRemoteWifiPort2[5+1];
    
    //GPRS
    uint8_t szPOSRemoteGprsIp[15+1];
    uint8_t szPOSRemoteGprsPort[5+1];
    uint8_t szPOSRemoteGprsIp2[15+1];
    uint8_t szPOSRemoteGprsPort2[5+1];
    uint8_t szPOSGprsApn[64+1];
    uint8_t szPOSGprsUID[64+1];
    uint8_t szPOSGprsPwd[16+1];
    
    //CDMA
    uint8_t szPOSRemoteCdmaIp[15+1];
    uint8_t szPOSRemoteCdmaPort[5+1];
    uint8_t szPOSRemoteCdmaIp2[15+1];
    uint8_t szPOSRemoteCdmaPort2[5+1];
    uint8_t szPOSCdmaApn[64+1];
    uint8_t szPOSCdmaUID[64+1];
    uint8_t szPOSCdmaPwd[16+1];

    //PPP
    uint8_t PPPNetTel[32+1];
    uint8_t PPPUser[32+1];         //ppp�����û���
    uint8_t PPPPwd[32+1];          //ppp��������
    uint8_t szPOSRemotePPPIp[15+1];
    uint8_t szPOSRemotePPPPort[5+1];

    uint8_t szExtNum[10];    //���ߺ���
    uint8_t sTpdu[6];           
    uint8_t szParamsPwd[9];   
        
    int     ucOprtLimitTime;  
    int     ucCommWaitTime;     
    int     ucDialRetryTimes;      
    int     iDispLanguage;   // ��ʾ����
    int     iCommType;       // 1-�ѻ� 2-���� 3-��������
    
    ST_COMM_CONFIG_MAIN      stTxnCommCfg;   // �ն�ͨѶ���� ����
    ST_WIFI_AP_INFO_MAIN     stWIFIAPInfo;   // wifi AP ������Ϣ Ŀǰֻ����һ̨����ֻ����һ���ȵ�

    uint8_t ucTelIndex;    // ��ǰ��ѡ�绰��������� ��ʼ��Ϊ0, �����ʵ��ʹ���У�ĳ�������ĵ绰���벦�ɹ��ˣ���ô��¼������������һ�δ������ʼ
    uint8_t ucTCPIndex;    // ��ǰ��ѡIP������ ��ʼ��Ϊ0, �����ʵ��ʹ���У�ĳ��������IP���ɹ��ˣ���ô��¼������������һ�δ������ʼ
    UINT8   szOrganNum[8 + 1];  //tms�������� add wgz
    ST_MULTI_APP_PARAM_MAIN staTagValue[40];

    uint8_t szInitPWD[32 + 1];//��ʼ����ά������
    uint8_t szModifyPWD[32 + 1];//�޸ĺ��ά������
    uint8_t szDownLoadTime[14+1];//�ϴ�����ʱ�䣬���ڳ���24Сʱɾ��δ������ɵ��ļ�
    uint8_t szDialNum[32];
    uint8_t ucWnetMode;     //���߲���������ģʽ
    int     iPPPAuth;       // ppp��֤
    uint8_t szTpdu_Asc[11]; //asc��ʾ��TPDU
    uint8_t szNetCtrlFlag;  // 1->net controller enable //0->net controller disable
    uint8_t szNetCtrlMaxPckLen[2]; // 2 bytes : for example : 0x01 0x35 => (0x01 << 8) | 0x35 = 309 bytes
    int8_t ucCommModule;      //����ͨѶģ����Ϣ //�����ļ��У�����ÿ����ʱ��ȡ
    uint8_t ucDnsFlg;     // flag=1  DNS //flag=0 IP
    uint8_t ucChgCdmaParamFlg; //��ԭ��cdma��uid��pwd�޸�
    uint8_t SslEnable;   //�Ƿ�����sslģʽ��1���ã�0�����ã� Ĭ�ϲ����ã���ͨ��ini���ò����ļ�����Ĭ�ϵ�ֵ
    uint8_t szREFU[195-1-1];   //���ã�����Զ�����ص�ʱ��Ϊ�˱�֤�������䣬���ܸĽṹ�壬���Լ�һЩ�ֽڱ���
}POS_PARAM_STRC_MAIN;

//return -1 is fail , return 0 is OK
INT32S Get_Ser_Comm_Param(POS_PARAM_STRC_MAIN *pSaveParam);
INT32S Set_Ser_Comm_Param(POS_PARAM_STRC_MAIN *pSaveParam);

//about Net controller status
// 1-> net controller open  // 0->net controller close
INT32S Get_Ser_Net_Ctrl_Status(INT32S *pNewStatus);
INT32S Set_Ser_Net_Ctrl_Status(INT32S *pNewStatus);
////////////////////////////////////////////////////////////////

//new extern functions for manu request trans
//request server do terminal register manu 
//return 0 is ok , return -1 are fail
//INT32S DispLanType : 1->chinese // 0->english
INT32S Start_Manu_Auto_Inspect_New(TRANS_RET_CODE *pRetCode, INT32S DispLanType);
INT32S Start_Manu_Download_App_New(TRANS_RET_CODE *pRetCode, INT32S DispLanType);
INT32S Start_Manu_Term_Reg_New(TRANS_RET_CODE *pRetCode, INT32S *pRegistRslt, INT32S DispLanType);
INT32S Start_Manu_Manage_Log_On_New(TRANS_RET_CODE *pRetCode, INT32S *pRegistRslt, INT32S DispLanType);
//pHostRetCode : must more than 3 bytes space
INT32S Get_Last_Trans_Host_Ret_Code(char *pHostRetCode);
INT32S Get_Cur_Tms_Task_Ver_Info(char *pVerInfo);
INT32S Get_Tms_Task_Download_Per(char *pPerStr);  //download percentage  
////////////////////////////////////////////////////////////////

typedef struct __STR_APP_INFO_LIST_MAIN__
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
}STR_APP_INFO_LIST_MAIN; //it is the same as strcut UAPPINFOmain which defined in tmslib.h;


void  Test_LibsbTms(void);

//pStrAppInfoListMain : store app info 
//AddManageInfoFlg    : 1 => add manage app info together; 0 => only add sub app info
INT32S Install_Sub_App_Info_List(STR_APP_INFO_LIST_MAIN *pStrAppInfoListMain, int AddManageInfoFlg);

/*
Function : library init , must call it in the main function before while(1)
           It will start sbTmsMain process which run in the background
           It's used for Manager and sbTmsMain processing together !
Input    : void
Output   : return -1 is fail , others are OK
*/
//return -1 is fail , return 0 are OK
INT32S LibsbTms_Init(void);

/*
Function : library init , must call it in the main function before while(1)
           It will start sbTmsMain process which run in the background.
           New input param, add sbTmsMain excute path and tms download tmp path !
           It's used for only sbTmsMain without Manager in tms process !
Input    : pExcutePath,  sbTmsMain excute path (sub path)in /home/user0/Manage/
Output   : return -1 is fail , others are OK
*/
//return -1 is fail , return 0 are OK
INT32S LibsbTms_Init_New(char *pExcutePath);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif



