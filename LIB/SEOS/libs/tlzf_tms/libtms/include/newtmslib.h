#ifndef     _NEWTMSLIB_H_
#define     _NEWTMSLIB_H_

#include "tmslib.h"
#include "servpack.h"
#include "termpack.h"
#include "define.h"

#define  MODULE_FILE_NAME	"通联主控"

//#define _ZL_DEBUG_   //配合王朝国测试用


//  文件类型
#define     FILE_TYPE_BOOTLOADER    0x80    //  BOOT LOADER软件
#define     FILE_TYPE_BOOT          0x81    //  BOOT软件
#define     FILE_TYPE_MONITOR       0x82    //  MONITOR软件
#define     FILE_TYPE_CONFIG        0x90    //  系统配置文件
#define     FILE_TYPE_FONT          0xA0    //  字库文件
#define     FILE_TYPE_TMS           0xA1    //  tms临时文件
#define     FILE_TYPE_PED           0xB0    //  PED文件

#define     FILE_TYPE_APP           0x01    //  0x01    //  应用程序
#define     FILE_TYPE_DSRM          0x02    //  0x07    //  应用程序显示注册文件
#define     FILE_TYPE_DATA          0x03    //  0x04    //  应用程序创建的数据文件
#define     FILE_TYPE_PARA          0x04    //  0x06    //  参数文件
#define     FILE_TYPE_ASPUBKEY      0x11    //  收单行签名公钥文件
#define     FILE_TYPE_AAPUBKEY      0x12    //  收单行KISSA授权公钥文件

#define     LD_OK                   0x00        //  成功
#define     LDERR_GENERIC           0x01        //  失败
#define     LDERR_BAUDERR           0x03        //  波特率不支持
#define     LDERR_INVALIDTIME       0x04        //  非法时间
#define     LDERR_CLOCKHWERR        0x05        //  时钟硬件错误
#define     LDERR_SIGERR            0x06        //  验证签名失败
#define     LDERR_TOOMANYAPP        0x07        //  应用太多，不能下载更多应用
#define     LDERR_TOOMANYFILES      0x08        //  文件太多，不能下载更多文件
#define     LDERR_NOAPP             0x09        //  指定应用不存在
#define     LDERR_UNKNOWNAPP        0x0A        //  不可识别的应用类型
#define     LDERR_SIGTYPEERR        0x0B        //  签名的数据类型和下载请求类型不一致
#define     LDERR_SIGAPPERR         0x0C        //  签名的数据所属的应用名和下载请求应用名不一致
#define     LDERR_WRITEFILEFAIL     0x10        //  文件写入失败
#define     LDERR_NOSPACE           0x11        //  没有足够的空间
#define     LDERR_RESENDLASTPACK    0x20        //  校验错误，要求重新传输最后一个包
#define     LDERR_UNSUPPORTEDCMD    0xFF        //  不支持该命令


#define     NEWTMS_STX                      0x02
#define   	NEWTMS_ETX						 0x03
#define     NEWTMS_HANDSHAKE_SEND           "HELLO"
#define     NEWTMS_HANDSHAKE_RECV           "READY"

#define     NEWTMS_INFO_FACTORY             "NEWPOSTECH"    //  16位的生产商缩写

#define     NEWTMS_MAX_TASKNUM              127         //  任务的最大数目


#define     NEWTMS_MAX_MODEM_FRMLEN         1024  //  1024        //  modem asyn一帧数据的最大值

#define     NEWTMS_MAX_UART_FRMLEN          2048  //  8192        //  uart一帧数据的最大值
#define     NEWTMS_MAX_LAN_FRMLEN           2048  //  8192        //  LAN一帧数据的最大值
#define     NEWTMS_MAX_SOCK_FRMLEN          2048  //  1024        //  sock一帧数据的最大值

#define     NEWTMS_BAGS                     20     //  一发多收的包数量

#define     NEWTMS_MAX_RECVSIZE             409600         //  一次数据请求的最大值

#define     NEWTMS_SENDPACK_TIMEOUT         50       //  包发送的延时时间(ms)
#define     NEWTMS_RECPACK_TIMEOUT          50       //  包发送的延时时间(ms)
#define     NEWTMS_REC_TIMEOUT              30       //  包发送的延时时间(ms)

//握手时间
#define		NEWPOS_SHAKEHAND_TOTALTIME		180000		//总握手时间
#define		NEWPOS_MODEM_SHAKEHAND_TIME	    20000        //  modem握手重发时间(ms)
#define		NEWPOS_TIMEOUT_TIME		20000		//下载异常时间


//  命令码
#define     NEWTMS_CMD_HANDSHAKE            0x70        //  握手
#define     NEWTMS_CMD_REQAUTH              0x71        //  请求认证
#define     NEWTMS_CMD_ANSAUTH              0x72        //  应答认证
#define     NEWTMS_CMD_DLDATA               0x73        //  下载数据
#define     NEWTMS_CMD_UPLDATA              0x74        //  上载数据
#define     NEWTMS_CMD_EXIT                 0x75        //  退出下载


#define TMS_ERROR_UNKOWN_TID			9001		//未知TID
#define TMS_ERROR_UNKOWN_SN				9002		//未知SN号
#define TMS_ERROR_UNKOWN_MANUFACTURES	9003		//未知厂商
#define TMS_ERROR_UNSPT_PACK_LEN		9004		//不支持的数据包长度
#define TMS_ERROR_UNSPT_COMPRESS_VER	9005		//不支持的压缩算法
#define TMS_ERROR_UNSPT_ENCRYPT_VER		9006		//不支持的加密算法
#define TMS_ERROR_UNSPT_VERIFY_VER		9007		//不支持的校验算法
#define TMS_ERROR_UNSPT_COM_MODEL		9008		//不支持的通讯方式
#define TMS_ERROR_UNSPT_AUTH_VER		9009		//不支持的身份验证方式
#define TMS_ERROR_AUTH_ERROR			9010		//身份验证错误
#define TMS_ERROR_TASK_ERROR			9011		//取任务列表出错
#define TMS_ERROR_UNKOWN_TASK_INDEX		9012		//错误的任务编号
#define TMS_ERROR_UNKOWN_FILE_ADDR		9013		//错误的文件偏移地址
#define TMS_ERROR_UNKOWN				9014		//位置错误
#define TMS_ERROR_TIMEOUT				9015		//超时了
#define TMS_ERROR_VERIFY_ERROR			9016		//校验错
#define TMS_TASKLIST_NULL				9017		//任务列表为空
#define TMS_TASK_FORRBIDEN				9018		//终端被禁止下载

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
    uint8_t     termID[16];             //  16位下载终端号
    uint8_t     sn[16];                 //  16位SN号
    uint8_t     vender[16];             //  16位的生产商缩写
    uint8_t     postype[16];            //  16位的终端类型名
    uint16_t    packlen;                //  2位的通讯数据包长度
    uint8_t     Compress;               //  1byte支持压缩算法
    uint8_t     encrypt;                //  1byte支持加密算法
    uint8_t     datacheck;              //  1byte支持校验算法
    uint8_t     commtype;               //  1位通讯方式
    uint8_t     hardver[16];            //  16位硬件版本号
    uint8_t     softver[16];            //  16位软件版本号
    uint8_t     authtype;               //  1位身份验证方式
    uint8_t     randdata[8];            //  8位随机数R1
}newtms_tinfo_t;
typedef struct
{
    int32_t     tasklen;                //  任务文件长度
    uint8_t     checksum[20];           //  任务文件的hash校验值
    uint8_t     filecompress;           //  是否整文件压缩
    uint8_t     fileencrypt;            //  是否整文件加密
    uint8_t     flag[NEWTMS_MAX_TASKNUM+1]; //  每一个任务的更新状况, 0:正在下载,1:已经下载完成,2:更新完毕 
    uint8_t     RemoveSameTask;         //  已经检查了下载的任务和正在运行的文件的一致性
    uint8_t     reserve[1];             //  预留
}newtms_taskfile_t;

typedef struct   //add andy.wang 2013-9-22 19:14:40 主要是为了显示一下要下载的一共的个数和次序
{
	int32_t 	itotaldownsize;  //总共下载的大小
	int32_t     icurrdownsize;		//当前下载的大小
}newtms_down_order;

//  任务类型定义
#define CREATE_FILE_SYS         0x01	//  创建文件系统
#define KERNEL_FILE_DOWN        0x02	//  Kernel文件
#define USPUK_FILE_DOWN         0x03	//  US PUK文件
#define FONT_FILE_DOWN          0x05	//  字库文件
#define APP_FILE_DOWN           0x06	//  应用程序
#define DSRM_FILE_DOWN          0x07	//  DSRM文件
#define DATA_FILE_UP            0x09	//  数据文件上传
#define DELETE_ALL_APP          0x0A	//  删除所有应用
#define DELETE_SOME_APP         0x0B	//  删除指定应用
#define DELETE_FILE_LINUX       0x13    //  7110文件删除

#define NEWCONFIG_NAME     "NewConfig.ini"  //add andy.wang 2014-1-10 13:04:15


typedef struct
{
    uint8_t     identifier[2];      //  值为"TH"
    uint8_t     file_version;       //  格式版本，当前采用0x01
    uint8_t     impress;            //  是否强制更新
    uint8_t     file_headLen[4];    //  文件头的大小
    uint8_t     task_number[4];     //  任务数目
    uint8_t     reserve[4];         //  为了凑16位的
}T_Task_FileHead;     //16

typedef struct
{
    uint8_t     task_no;            //  任务编号
    uint8_t     task_type;          //  任务类型
    uint8_t     parity_type;        //  校验类型
    uint8_t     download_control;   //  下载控制，bit0表示强制下载
    uint8_t     file_len[4];        //  文件大小
    uint8_t     app_name[32];       //  所属应用名
    uint8_t     file_name[32];      //  存储文件名 //所属应用名和存储文件名合并为7110的文件名，总共64个字节，app_name和file_name这两个变量中间不能再增加变量
    uint8_t     parity[20];         //  校验值
  //  uint8_t     szversion[4];        //
    uint8_t     enable_time[6];     //  更新时间
    uint8_t     reserve[6];         //  保留字节
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

