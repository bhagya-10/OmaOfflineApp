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

// PSTN配置参数
typedef struct _tagST_PSTN_PARA_MAIN_
{
    uint8_t             ucSendMode;          // 数据发送方式
    uint8_t             szTxnTelNo[100+1];   // 拨号号码
    ModemDialParms_t    stDialPara;          // modem拨号参数
}ST_PSTN_PARA_MAIN; 

// RS232配置参数
typedef struct _tagRS232_INFO_MAIN_
{
    uint8_t     ucPortNo;           // Port #, COM1, COM2 ....
    uint8_t     ucSendMode;         // 数据发送方式
    uint8_t     szAttr[20+1];       // 串口打开参数, "9600,8,n,1", ....
}ST_RS232_INFO_MAIN;

// TCP/IP配置参数
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
    uint8_t     szAPN[64+1];  ///拨号号码 CDMA: #777; GPRS: cmnet
    uint8_t     szUID[64+1];
    uint8_t     szPwd[64+1];
    uint8_t     szSimPin[16+1];     // SIM card PIN
    uint8_t     szDNS[32+1];
    uint8_t     szRemoteIP[20+1];
    uint8_t     szRemotePort[5+1];
}ST_WIRELESS_INFO_MAIN;

// WIFI  AP 配置参数
typedef struct _tagST_WIFI_AP_INFO_MAIN_
{
    int         iChannel;                  //  Wireless LAN communication channel(0 - 13)－－通讯信道
    int         iSecurityType;             //  Sets the Wireless LAN security type. －－ 加密模式
    int         iWEPKeyIdx;                //  WEP key index (1 - 4) -- WEP密钥组
    char        szListWEPKey[4][32];       //  4 sets of WEP Key  --- 密钥组密钥
    char        szWPAPSK[64];              //  Personal Shared Key Pass-Phrase,
    char        szSSID[64];                //  SSID
}ST_WIFI_AP_INFO_MAIN;

// 通讯模块配置信息
typedef struct _tagST_COMM_CONFIG_MAIN_
{
    uint8_t                 ucCommType;           // 通讯类型(RS232/modem/tcp...)
    ulong                   ulSendTimeOut;        // 发送超时时间
    ST_RS232_INFO_MAIN      stRS232Para;          // RS232 配置参数
    ST_TCPIP_INFO_MAIN      stTcpIpPara;          // TCP 配置参数
    ST_WIRELESS_INFO_MAIN   stWirlessPara;        // GRPS/CDMA无线通讯配置参数
    ST_PSTN_PARA_MAIN       stPSTNPara;           // modem 配置参数
    ST_TCPIP_INFO_MAIN      stWIFIIpPara;         // WIFI 通讯模式配置参数
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
    uint8_t szPhoneNum3[20];// 用于异步拨号
    uint8_t szPhoneNum4[20];
    //以太网
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
    uint8_t PPPUser[32+1];         //ppp拨号用户名
    uint8_t PPPPwd[32+1];          //ppp拨号密码
    uint8_t szPOSRemotePPPIp[15+1];
    uint8_t szPOSRemotePPPPort[5+1];

    uint8_t szExtNum[10];    //外线号码
    uint8_t sTpdu[6];           
    uint8_t szParamsPwd[9];   
        
    int     ucOprtLimitTime;  
    int     ucCommWaitTime;     
    int     ucDialRetryTimes;      
    int     iDispLanguage;   // 显示语言
    int     iCommType;       // 1-脱机 2-串口 3-联机无线
    
    ST_COMM_CONFIG_MAIN      stTxnCommCfg;   // 终端通讯配置 －－
    ST_WIFI_AP_INFO_MAIN     stWIFIAPInfo;   // wifi AP 配置信息 目前只考虑一台机器只连接一个热点

    uint8_t ucTelIndex;    // 当前首选电话号码的索引 初始化为0, 如果在实际使用中，某个索引的电话号码拨成功了，那么记录下来索引，下一次从这个开始
    uint8_t ucTCPIndex;    // 当前首选IP的索引 初始化为0, 如果在实际使用中，某个索引的IP拨成功了，那么记录下来索引，下一次从这个开始
    UINT8   szOrganNum[8 + 1];  //tms机构号码 add wgz
    ST_MULTI_APP_PARAM_MAIN staTagValue[40];

    uint8_t szInitPWD[32 + 1];//初始密码维护密码
    uint8_t szModifyPWD[32 + 1];//修改后的维护密码
    uint8_t szDownLoadTime[14+1];//上次下载时间，用于超过24小时删除未下载完成的文件
    uint8_t szDialNum[32];
    uint8_t ucWnetMode;     //无线拨号有两种模式
    int     iPPPAuth;       // ppp认证
    uint8_t szTpdu_Asc[11]; //asc表示的TPDU
    uint8_t szNetCtrlFlag;  // 1->net controller enable //0->net controller disable
    uint8_t szNetCtrlMaxPckLen[2]; // 2 bytes : for example : 0x01 0x35 => (0x01 << 8) | 0x35 = 309 bytes
    int8_t ucCommModule;      //无线通讯模块信息 //存入文件中，避免每次临时获取
    uint8_t ucDnsFlg;     // flag=1  DNS //flag=0 IP
    uint8_t ucChgCdmaParamFlg; //将原本cdma的uid和pwd修改
    uint8_t SslEnable;   //是否启用ssl模式，1启用，0不启用， 默认不启用，可通过ini配置参数文件导入默认的值
    uint8_t szREFU[195-1-1];   //备用，由于远程下载的时候为了保证参数不变，不能改结构体，所以加一些字节备用
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
    char AppName[20+1];         //应用模块名
    char AppDispName[20+1];     //应用显示名
    char AppVar[15+1];          // 应用版本号 tms系统只支持15位版本号
    char TerminalID[8+1];       //终端号
    char ShopId[15+1];          //商户号
    char ParamPath[256];       //补登参数文件路径
    unsigned int AppRunNum;     // 应用排序
    char AppRunFlag;            // 是否运行
    char AppUpdateTime[15];     // 模块更新时间
    char AppUpdateTimeEnd[15];  // 模块更新截止时间
    unsigned char bisMainApp;   //是否主控应用，0-否，1-是
    uint8_t REF[104];//预留    
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



