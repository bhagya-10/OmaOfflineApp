#ifndef     _DEFINE_H_
#define     _DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif
//#include <stdint.h>
#define DebugModuleName "ums_tms"


#define     ON                      0x01
#define     OFF                                         0x00

#define     YES                     0x01
#define     NO                      0x00

#define     OK                                          0x00

#define  ERROR                                  0x01


#define     TMS_PROMPT_EN       0       //  prompt in English
#define     TMS_PROMPT_CHN      1       //  prompt in Chinese
#define     TMS_PROTO_NEWTMS    0       //  tms protocol: newtms

#define     TMS_COMM_GPRS       1       //  uses socket comm(gprs, cdma, lan...)
#define     TMS_COMM_CDMA       2       //  uses socket comm(gprs, cdma, lan...)
#define     TMS_COMM_PPP        3       //  uses PPP comm
#define     TMS_COMM_MDMASY     4       //  uses MODEM ASYN
#define     TMS_COMM_UART       5       //  uses RS232 comm
#define     TMS_COMM_LAN                6                       //uses LAN


#define     NEWTMS_MAX_TASKNUM              127         //  任务的最大数目

//#define       FIRMWARE_DOWN_LINUX     0x11    //  7110内核下载
//#define       FILE_DOWN_LINUX         0x12    //  7110文件下载
//#define       DELETE_FILE_LINUX       0x13    //  7110文件删除
//#define     DYMATIC_FILE_LINUX		 0x14		//dongtaiku
#define     PARAM_FILE_LINUX                 0x01      //应用参数文件
#define     DYMATIC_FILE_LINUX               0x04      //api公共模块
#define     DATA_FILE_DOWN           0x06   //  数据文件下载
#define     FIRMWARE_DOWN_LINUX      0x08    //  7110内核下载
#define     FILE_DOWN_LINUX          0x09      //可执行程序

#define     DESKTOP_FILE_LINUX               0x0A     //desktop
#define     PORT_RET_BUSY           0x05        //  没有物理串口资源分配
#define     PORT_RET_PORTERR        0x02        //  逻辑串口号非法
#define     PORT_RET_PARAERR        0xfe        //  无效的通讯参数
#define     PORT_RET_OK             0x00        //  成功

/* UART */
#define     PORT_COM1                           0x00        //  下载口
#define     PORT_MODEM                          0x01        //  modem模块
#define     PORT_WNET                           0x02        //  无线模块和以太网
#define     PORT_PINPAD             0x03        //  PINPAD口
#define     PORT_GPS                0x04        //  GPS
#define         PORT_EXT                                0x05            //	扩展口

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


#define NET_OK              0       //  无错误，正常
#define NET_ERR_RSP         1024    //  "ERROR" is return from the module
#define NET_ERR_NOSIM       1025    //  SIM/UIM card is not inserted or not detected
#define NET_ERR_PIN         1026    //  SIM PIN is required
#define NET_ERR_PUK         1027    //  SIM PUK is required
#define NET_ERR_PWD         1028    //  SIM PIN is incorrect
#define NET_ERR_SIMDESTROY  1029    //  SIM card destroy
#define NET_ERR_CSQWEAK     1030    //  the signal is too weak, please check the antenna
#define NET_ERR_LINKCLOSED  1031    //  the module is offline
#define NET_ERR_LINKOPENING 1032    //  the module is connecting to the network
#define NET_ERR_DETTACHED   1033    //  Dettached
#define NET_ERR_ATTACHING   1034    //  the module is looking for the base station.
#define NET_ERR_EMERGENCY   1035    //  SIM/UIM is in emergency status
#define NET_ERR_RING        1036    //  detect ringing
#define NET_ERR_BUSY        1037    //  detect call in progress
#define NET_ERR_DIALING     1038    //  dialing
#define NET_ERR_UNKNOWN     1039    //  unknown module type
#define NET_ERR_ABNORMAL    1040    //  abnormal error
#define NET_ERR_NOMODULE    1041    //  no module in the unit

#define UMAX_APP_COUNT		30 //应用最大数量
#define UMAX_MODULE_COUNT	100 //下载文件最大数量
//应用信息结构体
typedef struct
{
	char AppName[20+1]; //应用名
	char AppDispName[20+1]; //应用显示名,如果存在01000006标签,则送标签内容
	char AppVar[15+1];        // 应用版本号 tms系统只支持15位版本号 add wgz
	char TerminalID[8+1]; //终端号
	char ShopId[15+1]; //商户号
	uint8_t bisMainApp; //是否主控应用，0-否，1-是
}UAPPINFO;
//应用信息总结构体
typedef struct
{
	uint8_t bCount; //实际应用数
	UAPPINFO AppInfo[UMAX_APP_COUNT]; //参见struct UAPPINFO
}UAPPINFOLIST;

//模块类型枚举
typedef enum
{
	UMODULE_FILE = 0, //文件，2-5的文件都可使用0，由主控来区分
	UMODULE_PARAFILE = 1, //参数文件
	UMODULE_TRADE = 2, //业务模块
	UMODULE_PUBLIC = 3, //公共模块
	UMODULE_API = 4, //API模块
	UMODULE_PRIVATE = 5, //厂家私有库
	UMODULE_DATA = 6, //数据类文件(预留)
	UMODULE_DRIVER = 7, //硬件驱动(预留)
	UMODULE_OS = 8, //底层OS(预留)
	UMODULE_APP = 9        //应用，可执行文件 add andy.wang 
}UModuleType;

//模块信息结构体
typedef struct
{
	char ModuleName[50]; //模块名
	int ModuleType; //参见enum UModuleType
	char AppName[20+1];      //应用名，表明该文件属于某个应用 add andy.wang
}UMODULEINFO;


//模块信息总结构体
typedef struct
{
	uint8_t bCount; //实际下载文件数
	UMODULEINFO ModuleInfo[UMAX_MODULE_COUNT];//参见struct UMODULEINFO
}UMODULEINFOLIST;


//TMS下载通讯类型枚举
typedef enum
{
	TMS_BYRS232 = 1, //串口
	TMS_BYMODEM = 2, //MODEM异步
	TMS_BYETHENET = 3, //有线以太网
	TMS_BYWIFI = 4, //WIFI
	TMS_BYGPRS = 5, //GPRS，WCDMA（wcdma和gprs参数相同）
	TMS_BYCDMA = 6, //CDMA
	TMS_BYMODEMPPP = 7, //MODEMPPP拨号后走TCPIP
	TMS_BYUSB = 8 //USB
}TMSComType;

//TMS下载通讯结构
typedef struct
{
	unsigned char ucCOMMType; /*通信类型：参见 enum UTMSCOMTYPE*/
	unsigned char ucTPDUFlag; /*TPDU存在标识: 0-无，1-有*/
	unsigned char aucTPDU[5]; /*TPDU号码（需标识含TPDU）*/
	unsigned int uiSockID; /*tcpip通讯Socket ID UTMSCOMTYPE中3,4,5,6,7*/
	unsigned long hCommHandle; /*通讯句柄 UTMSCOMTYP中 1,2,8*/
	/*以下部分为联迪POS必须用到的参数，其他厂家不用可丢弃，有用到或者需要添加参数请及时反应*/
	unsigned char ucPackNum; /*下载阶段服务器端连续发送下载包个数：最小1，最大8*/
	unsigned int uiFrameLen; /*下载阶段传输数据帧的长度（单位：字节）*/
	unsigned char ucSyncFlag; /*强制终端与服务端执行一收一发的报文交互模式：0-关闭 1-打开*/
	unsigned char ucIfCheck; /*下载阶段数据校验标识：0-不校验 1-校验*/
	unsigned char ucBreakType; /*下载阶段是否允许按键（取消/清除键）中断下载0-不允许 1-允许*/
	unsigned int uiReTime; /*通讯报文接收超时后，重新接收次数(即服务器重发报文次数）*/
	unsigned int uiTimeoutMs; /*单个通讯报文接收超时时间（单位：毫秒）*/
	unsigned int uiDelayTime; /*终端每个发送数据包之前的延时（单位：毫秒）*/
	unsigned int uiSlotTime; /*服务器端连续发包间隔时间（单位：毫秒）*/
	unsigned char auReserver[100]; /*预留域*/
	unsigned int uiRFULen; /*扩展空间长度*/
	void *pvRFU; /*扩展空间指针*/
}TMSPARAMETERS;


//TMS类API返回值枚举
typedef enum
{
	TMS_FAIL = -1, //失败
	TMS_OK = 0 //成功
}TMSStatus;

#ifdef __cplusplus
}
#endif


#endif

