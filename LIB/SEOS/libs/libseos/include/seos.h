/******************************************************************************
 * seos.h
 * 
 * Copyright (c) 2011 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2011-01-19   1.0.0    Li.Hua       written
 ******************************************************************************/

#ifndef __SEOS_H__
#define __SEOS_H__

#include <stdint.h>
#include <assert.h>
#include <netinet/in.h>

#include "led.h"
#include "posapi.h"

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */


#ifndef     INT8
		typedef     char                    INT8;
#endif
#ifndef     INT16
		typedef     short                   INT16;
#endif
#ifndef     INT32
		typedef     int                     INT32;
#endif
#ifndef     INT64
		typedef     long long               INT64;
#endif
		
#ifndef     UINT8
		typedef     unsigned char           UINT8;
#endif
#ifndef     UINT16
		typedef     unsigned short          UINT16;
#endif
#ifndef     UINT32
		typedef     unsigned int            UINT32;
#endif
#ifndef     UINT64
		typedef     unsigned long long      UINT64;
#endif
		
#ifndef uchar
#define uchar unsigned char
#endif
		
#ifndef uint
#define uint  unsigned int
#endif

#ifndef     YES
#define     YES                     0x01
#define     NO                      0x00
#endif
		
#ifndef     OK
#define     OK						0x00
#endif
		
#define     ON                      0x01
#define     OFF						0x00

		
typedef struct
{
    UINT8       AppName[32];
    UINT8       AppVer[16];
    UINT8       AppProvider[32];
    UINT8       BuildTime[14];
    UINT8       Descript[64];
    INT32       AppNo;
}APPINFO;


int application_init(int *argc, char **argv[]);
int application_exit(void);



typedef struct
{
    int             dhcp;       //  YES: enable dhcp, NO: disable dhcp
    struct in_addr  localip;    //  local ip
    struct in_addr  subnet;     //  net subnet addr;
    struct in_addr  gateway;    //  gate way addr;
    struct in_addr  dns1;       //  DNS1;
    struct in_addr  dns2;       //  DNS2;
}tcpipcfg_t;


typedef struct
{
    uint8_t       fid;
    uint8_t       attr;
    uint8_t       type;
    char		  name[17];
    uint32_t      length;
} FILE_INFO;


typedef struct{
#define STK_MAX_ITEM        50          //  max item in one menu
#define STK_MAX_NAMELEN     255         //  max word len in an item

	int     cur_cmdtype;            //universal cmd type
	int     numofitem;	            //for 36,37 as no of item, 33,34,35 as command detail
	char    title[STK_MAX_NAMELEN]; //for 36,37 as title, 33,34,35 as text
	char    content[STK_MAX_ITEM][STK_MAX_NAMELEN]; //for 36,37
	int     content_idx[STK_MAX_ITEM];  //for 36,37
	int     inputMin;               //for 34,35
	int     inputMax;	            //for 34,35
}tStkMenu;

typedef struct{
    int     index;          // the index in the sms memory
    int     status;         // 0:"REC UNREAD", 1:"REC READ", 2:"STO UNSENT", 3:"STO SENT", 4:"ALL"
    char    daoa[64];       // the telephone number of sms, like "10086000"
    char    scts[64];       // the time of sms, like "10/05/11,18:25:00+32"
    int     length;         // the length of sms content
    char    content[512];   // the content of sms
} tSms;



/* System API */
void sysReadSN(void *SerialNo);
int sysReadConfig(uint8_t *ConfigInfo);
void sysGetLastError(int *ModInfo, int *ErrorNum);
void sysBeef(uint32_t Freq, uint32_t BeepTime);
void sysBeep(void);
int sysSetTime(const void *TimeStr);
void sysGetTime(void *TimeStr);
uint32_t sysGetTimerCount(void);
void sysTimerSet(uint32_t TimerNo, uint32_t CountMs);
uint32_t sysTimerCheck(uint32_t TimerNo);
void sysDelayMs(uint32_t CountMs);
void sysPowerOff(void);
void sysReset(void);
int sysGetPowerStatus(void);
int sysGetVolt(void);
void sysAutoPowerSave(int mode);
uint32_t sysPowerSave(uint32_t Event, uint32_t TimeOutMs);
int sysEncode2bmp(const void *image, uint32_t width, uint32_t height, void *bmpfilebuffer);
int accGetMotion(int *Xaxis, int *Yaxis, int *Zaxis);
int sysSetPowerKey(int32_t DelayMs);
int sysGetPowerKeyStatus(void);
void   sysReadVer(uint8_t *VerInfo);


/* 定义按键值 */
#undef	KEY0
#define	KEY0                    DIKS_0	//  '0'

#undef	KEY1
#define	KEY1                    DIKS_1  //  '1'

#undef	KEY2
#define	KEY2                    DIKS_2  //  '2'

#undef	KEY3
#define	KEY3                    DIKS_3	//  '3'

#undef	KEY4
#define	KEY4                    DIKS_4	//  '4'

#undef	KEY5
#define	KEY5                    DIKS_5	//  '5'

#undef	KEY6
#define	KEY6                    DIKS_6	//  '6'

#undef	KEY7
#define	KEY7                    DIKS_7	//  '7'

#undef	KEY8
#define	KEY8                    DIKS_8	//  '8'

#undef	KEY9
#define	KEY9                    DIKS_9	//  '9'

#undef	KEY_BACKSPACE
#define	KEY_BACKSPACE           DIKS_BACKSPACE	//  退格

#undef	KEY_CLEAR
#define	KEY_CLEAR               DIKS_CLEAR		//  全部清除

#undef	KEY_ALPHA
#define	KEY_ALPHA               DIKS_SHIFT		//  字母

#undef	KEY_UP
#define	KEY_UP                  DIKS_CURSOR_UP	//  向上

#undef	KEY_DOWN
#define	KEY_DOWN                DIKS_CURSOR_DOWN	//  向下

#undef	KEY_FN
#define	KEY_FN                  DIKS_CONTROL	//  功能

#undef	KEY_MENU
#define	KEY_MENU                DIKS_ALT        //  菜单

#undef	KEY_ENTER
#define	KEY_ENTER               DIKS_ENTER		//  确认

#undef	KEY_CANCEL
#define	KEY_CANCEL              DIKS_ESCAPE		//  取消

#undef	KEY_PRNUP
#define	KEY_PRNUP               DIKS_PRINT		//  打印向上走纸

#undef	KEY_INVALID
#define	KEY_INVALID             0xFF            //DIKS_NULL值为0，和KEY_TIMEOUT值一样，KEY_TIMEOUT8110也为0，所以把这个键值强制改为0XFF //  无效按键

#undef	KEY_TIMEOUT
#define	KEY_TIMEOUT             0				//  超时无按键 20130412

#undef KEY_QINGCHU							
#define KEY_QINGCHU				0x08		//键盘上的清除键值

// 串输入控制和返回值
#define KB_MAXLEN               256         //  串输入最大允许输入的长度
#define KB_EN_REVDISP           0x01        //  1（0） 正 （反）显示
#define KB_EN_FLOAT             0x02        //  1（0） 有（否）小数点
#define KB_EN_SHIFTLEFT         0x04        //  1（0） 左（右）对齐输入
#define KB_EN_CRYPTDISP         0x08        //  1（0） 是（否）密码方式
#define KB_EN_CHAR              0x10        //  1（0） 能（否）输字符
#define KB_EN_NUM               0x20        //  1（0） 能（否）输数字
#define KB_EN_BIGFONT           0x40        //  1（0） 大（小）字体
#define KB_EN_PRESETSTR         0x80        //  1（0） 是（否）允许预设的字符串
#define KB_EN_NEWLINE           0x100       //  1（0） 是（否）允许左对齐输入换行显示
#define KB_EN_DECIMAL           0x200       //  1（0） 小数点位数3 (否)小数点位数2


// 预设几组常用的输入控制模式
#define KB_BIG_ALPHA            0x1F5       //  大字体字符串输入
#define KB_BIG_NUM              0x1E5       //  大字体数字串输入
#define KB_BIG_PWD              0x6D        //  大字体密码输入
#define KB_SMALL_ALPHA          0x1B5       //  小字体字符串输入
#define KB_SMALL_NUM            0x1A5       //  小字体数字串输入
#define KB_SMALL_PWD            0x2D        //  小字体密码输入

#define KB_CANCEL               -1          //  串输入被用户按取消键退出
#define KB_TIMEOUT              -2          //  串输入超时退出
#define KB_ERROR                -3          //  串输入参数非法


void kbSetSlipFW(int Mode);
int kbCheckKey(void);
int kbWaitKey(int status);

int kbhit(void);
int kbGetKey(void);
int kbGetKeyMs(int TimeOutMs);
void kbFlush(void);
void kbSound(int Mode, int DelayTimeMs);
int kbGetString(uint32_t mode, int minlen, int maxlen, uint32_t timeoutms, char *info);
int kbGetHZ(int MinLen, int MaxLen, int TimeOutMs, char *StrBuf);
void kbSetLightTime(uint32_t LightTime);
int kbWaitCodeKey(void);

#define kbLight(Mode)			lcdLight(Mode)


// 背光模式
#define LIGHT_ON_TEMP           0           //  背光临时开启(对于定时模式用)
#define LIGHT_TIMER_MODE        1           //  背光定时模式
#define LIGHT_ON_MODE           2           //  背光常开模式
#define LIGHT_OFF_MODE          3           //  背光常关模式

void lcdLight(int Mode);



/* DirectFB */
IDirectFB *dfb_get_directfb(void);
IDirectFBDisplayLayer *dfb_get_primay_layer(void);
IDirectFBFont *dfb_create_font(const char *fontfile, int fontsize, int attributes);
int dfb_destroty_font_by_file(const char *fontfile, int fontsize, int attributes);
int dfb_destroy_font(IDirectFBFont *font);
IDirectFBFont *dfb_get_font(int fontsize, int attributes);
IDirectFBFont *dfb_get_font_by_file(const char * fontfile, int fontsize, int attributes);
int dfb_get_font_info(const IDirectFBFont *font, char **fontfile, int *fontsize, int *attributes);
int dfb_putpixel(IDirectFBSurface *surface, int x, int y, int color);


/* LCD */
#define DISP_ASCII          0x00    //  display small font(6*8)
#define DISP_CFONT          0x01    //  display the pointed font (default 8*16)
#define DISP_HFONT16		 0x02		//显示黑体16号字
#define DISP_ZFONT29		 0x04		//真黑29号字
#define DISP_SONG24         0x08		//宋体24
#define DISP_HFONT28		 0x1000	//黑体28号
#define DISP_INVLINE		0x10	//  display inverse whole line
#define DISP_CLRLINE        0x20    //  clear the line before display
#define DISP_MEDIACY        0x40    //  display in the middle of line
#define DISP_REVERSE        0x80    //  display inverse character
#define DISP_LEFT			0x100   //  display in the LEFT of line
#define DISP_RIGHT			0x200   //  display in the RIGHT of line
#define DISP_ABSLINE		0x400   //  display at abs x point y point 2013.01.10
#define DISP_FLIP			0x800   //  display at abs x point y point 2013.01.10
#define DISP_COLOR			0x2000  // display locor


#define SET_PRIMARY_FONT                0   //  设置首选字库
#define SET_SECONDARY_FONT              1   //  设置备选字库(针对首选字库为汉字，通过这个备选字库选择ascii部分的字体)

/* 字体定义 */
#define FONT_COURIER_NEW				"CourierNew"
#define FONT_SONG						"宋体"
#define FONT_IMITATE_SONG				"仿宋"
#define FONT_SYSTEM						"System"
#define FONT_ARIAL						"Arial"
#define FONT_TAHOMA						"Tahoma"
#define FONT_VERDANA					"Verdana"
#define FONT_DOTUM						"Dotum"

/* 字符集定义 */
#define FONT_CHARSET_ASCII				"ASCII"
#define FONT_CHARSET_GB2312				"GB2312"
#define FONT_CHARSET_GB18030			"GB18030"


IDirectFBSurface *lcdGetSurface(void);
void lcdGetSize(int *width, int *height);
void lcdCls(void);
void lcdClrLine(int BeginLine, int EndLine);
void lcdGoto(int Col, int Line);
void lcdSetInverse(int Mode);
void lcdPrintf(const char *format, ...);
void lcdFlip(void);
void lcdDisplay(int Col, int Line, uint32_t Mode, const char *format, ...);
void lcdStipple(int X, int Y, int Color);
void lcdDrawPicture(int x, int y, int w, int h, const char *filename);
/*FontPriority 为DISP_CFONT, DISP_ASCII时设置相应的字体*/
int lcdSetFont(const char *pFontName, const char *pFontCharacter, 
			   int Style, int Size, int FontPriority);
void lcdGetCurFont(char *pFontName, char *pFontCharacter, 
				   int *pStyle, int *pSize, int FontPriority);
void lcdDrawFrame(int StartX, int StartY, int EndX, int EndY, int Mode);




#define lcdStore(pCol, pLine, pucBitMapBuf) \
	do { \
		lcdGoto(0, 0); \
		lcdSetInverse(0); \
		lcdPrintf("Notice: the function lcdStore() has not implemented."); \
		kbGetKey(); \
	} while (0);

#define lcdRestore(Col, Line, pucBitMapBuf) \
	do { \
		lcdGoto(0, 0); \
		lcdSetInverse(0); \
		lcdPrintf("Notice: the function lcdRestore() has not implemented."); \
		kbGetKey(); \
	} while (0);

void lcd_store(IDirectFBSurface **surface);
void lcd_restore(void *buf);



#define     ICON_PHONE          1       //  phone
#define     ICON_SIGNAL         2       //  wireless signal
#define     ICON_PRINTER        3       //  printer
#define     ICON_ICCARD         4       //  ICC
#define     ICON_LOCK           5       //  lock
#define     ICON_BATTERY        6       //  battery
#define     ICON_EXTPOWER       7       //  external power
#define     ICON_UP             8       //  up
#define     ICON_DOWN           9       //  down
#define     ICON_ETH            10      //  ethernet
#define     ICON_MAIL           11      //  mail

#define     CLOSEICON           0       //  close icon
#define     OPENICON            1       //  open icon[for printer,ICC, lock, bttery, extpower, up, down, eth and mail]

void lcdSetIcon(int IconNo, int Mode);


//  返回值
#define     PRN_OK              0x00
#define     PRN_BUSY            0x01
#define     PRN_PAPEROUT        0x02
#define     PRN_WRONG_PACKAGE   0x03
#define     PRN_FAULT           0x04
#define     PRN_TOOHEAT         0x08
#define     PRN_NOBATTERY       0x10 //7210没有装电池
#define     PRN_OTHERFAULT      0x20 //其他错误
#define     PRN_UNFINISHED      0xF0
#define     PRN_NOFONTLIB       0xFC
#define     PRN_OUTOFMEMORY     0xFE

IDirectFBSurface *prnGetSurface(void);
int  prnInit(void);
void prnClose(void);  //20130116
int  prnSetFont(const char *pFontName, const char *pFontCharacter, int Style, int Size, int FontPriority);
void prnGetCurFont(char *pFontName, char *pFontCharacter, int *pStyle, int *pSize, int FontPriority);
void prnSetFontZoom(int WidthZoom, int HeightZoom);
void prnSetSpace(int Width, int Height);
void prnLeftIndent(int Indent);
void prnSetGray(int Level);
int prnGetCurLine(void);
void prnStep(int Line);
int prnTemperature(void);
int prnPrintf(const char *format, ...);
void pntDrawPicture(int x, int y, int w, int h, const char *filename);
int prnStatus(void);
int prnStart(void);
void prnEnBlackMark(int Mode, int BlackMarkHeight);
int prnBlackMarkStep(int MaxStep, int BlackMarkNum);
int prnCut(uint32_t cut_mode);



/* magstripe */
int  magOpen(void);
void magClose(void);
void magReset(void);
int magSwiped(void);
uint32_t magRead(void *Track1, void *Track2, void *Track3);


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
/* filesystem */
int fileOpen(const char *filename, int oflag, ...);
int fileSeek(int fd, off_t offset, int whence);
int fileRead(int fd, void *buf, size_t count);
int fileWrite(int fd, const void *buf, size_t count);
int fileTruncate(int fd, off_t length);
int fileClose(int fd);
int fileRemove(const char *filename);
int fileSize(const char *filename);
unsigned long long fileFreeSpace(void);
int fileExist(const char *filename);
int fileGetInfo(FILE_INFO *pFileInfo);
int fileRename(const char *OldFileName, const char *NewFileName);
int fileOpenExt(const char *FileName, uint32_t Mode, uint32_t BelongApp, uint32_t FileType);
int fileRemoveExt(const char *FileName, uint32_t BelongApp, uint32_t FileType);
int fileSizeExt(const char *FileName, uint32_t BelongApp, uint32_t FileType);
int fileExistExt(const char *FileName, uint32_t BelongApp, uint32_t FileType);
int fileRenameExt(const char *OldFileName, uint32_t OldBelongApp, uint32_t OldFileType,
				  const char *NewFileName, uint32_t NewBelongApp, uint32_t NewFileType);

int appDelete(uint8_t AppNo, int DelSubFile);
int fileToApp(uint8_t *srcFileName, uint32_t srcBelongApp, uint32_t srcFileType, uint8_t *RetAppNo);
int fileToAppData(uint8_t *srcFileName, uint32_t srcBelongApp, uint32_t srcFileType,
                            uint8_t *dstFileName, uint32_t dstBelongApp, uint32_t dstFileType);
int fileMakeDir(const char *pathname);
int fileRemoveDir(const char *pathname);

/* wireless */
int WnetSelectModule(int slotNo);
int WnetInit(int TimeoutMs);
int WnetGetInfo(int *NetType, int *ModType, void *ModVer);
int WnetReset(void);
int WnetSignal(int *Signal);
int WnetCheckSim(void);
int WnetInputSimPin(const char *Pin);
int WnetGetIMEI(char *IMEI);
int WnetDnsResolve(const char *name, char *result, int maxlen);
int WPhoneStatus(void);
int WPhoneAnswer(void);
int WPhoneHangUp(void);
int WPhoneCall(const char *PhoneNum);
int WPhoneSendDTMF(char DTMFNum, uint32_t DurationMs);

int Stk_Init(int timeout_ms, int *ret_cmd_type);
int Stk_GetCurCmd(int *ret_cmd_type);
int Stk_ListMenu(int cmd_type, tStkMenu *ret_menu);
int Stk_Handle(int cmd_type, int *status, int *item_id, const char *input_string, int *ret_cmd_type);
int Ussd_Cmd(int n, const void *sendstr, int dcs, int *m, void *recvstr, int *recvdcs);
int Sms_GetCSCA(void *sca);
int Sms_SetCSCA(const void *sca, int tosca);
int Sms_Cmgl(int listtype, size_t maxno, size_t *recvnum, tSms smsdata[]);
int Sms_Cmgr(int index, tSms *smsdata);
int Sms_Cmgd(int index);
int Sms_Cmgs(const void *de, const void *content);

/* getsockopt/setsockopt optval types. */
#define OPT_DEVNULL     0
#define OPT_DEVWNET     1
#define OPT_DEVETH      2
#define OPT_DEVWIFI     3
#define OPT_DEVMODEM	4

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

#define EWIFI_SOCKIPREG     2088    //  Socket IP registration failed
#define EWIFI_ONLINELOST    2094    //  In Always Online mode, connection was lost and re-established

/* PPP */
enum {
	PPP_DEV_MODEM	=	OPT_DEVMODEM,
	PPP_DEV_GPRS,
	PPP_DEV_CDMA,
};

int PPPLogin(int devtype, const char *APN, const char *user, 
			 const char *passwd,const uint32_t auth, int timeout);
int PPPLogout(int devtype);
int PPPCheck(int devtype);



/* network */
int NetGetTCPIPCfg(int devtype, tcpipcfg_t *cfgparms);
int NetSetTCPIPCfg(int devtype, const tcpipcfg_t *cfgparms);
int NetSocket(int family, int type, int protocol);
int NetBind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen);
int NetConnect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
int NetConnectURL(int sockfd, const char *servurl, in_port_t servport);
int NetListen(int sockfd, int backlog);
int NetAccept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
int NetSend(int sockfd, const void *buff, int nbytes, int flags);
int NetRecv(int sockfd, void *buff, int nbytes, int flags);
int NetSendto(int sockfd, const void *buff, int nbytes, int flags, 
			  const struct sockaddr *to, socklen_t addrlen);
int NetRecvfrom(int sockfd, void *buff, int nbytes, int flags, 
				struct sockaddr *from, socklen_t *addrlen);
int NetClose(int sockfd);
int NetSetsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int NetGetsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int SockAddrset(struct sockaddr *addr, const void *ipstr, in_port_t port);
int SockAddrget(const struct sockaddr *addr, void *ipstr, in_port_t *port);

/* WIFI */
typedef struct
{
    int32_t     NetType;        //  网络类型
    int32_t     Module;         //  模块型号
    uint8_t     Ver[64];        //  模块版本信息
}tWifi_Operations;

#define MAXSENDLEN_WIFI             256

#define WAITPREBUF_WIFI             0
#define WAITFIRST_WIFI              5000
#define WAITBLOCK_WIFI              10000
#define WAITATCMD_WIFI              20000

#define WAIT_NETREG_WIFI            60000
#define WIFI_TRYNUM                 5

#define WIFI_SECURITY_NONE          0       //  No security;
#define WIFI_SECURITY_WEP64         1       //  WEP-64;
#define WIFI_SECURITY_WEP128        2       //  WEP-128;
#define WIFI_SECURITY_WPATKIP       3       //  WPA-TKIP;
#define WIFI_SECURITY_WPA2AES       4       //  WPA2-AES;
#define WIFI_SECURITY_WPATKIP_E     5       //  WPA-TKIP Enterprise PEAP-MSCHAPv2;
#define WIFI_SECURITY_WPA2AES_E     6       //  WPA2-AES Enterprise PEAP-MSCHAPv2;

//  要启用省电模式，.IchipPowerSave=1, .WLANPowerSave=5是最优化的设置
typedef struct
{
    int     IchipPowerSave;     //  Wi-Fi module (iChip) power save (0..255, 0:Disable Power Save mode, 1..255:seconds without any activity)
    int     Channel;            //  Wireless LAN communication channel(0 - 13)
    int     WLANPowerSave;      //  Wireless LAN Power Save. (0(default): WiFi chipset Power Save mode is disabled. 1-5:The number of beacon periods during chipset remains in Power Save mode.)
    int     RoamingMode;        //  0(default):disable roaming mode, 1:enable roaming mode
    int     PeriodicScanInt;    //  Periodic WiFi Scan Interval(1~3600, default=5)
    int     RoamingLowSNR;      //  Sets a low SNR threshold for iChip in Roaming mode.(0 - 255dB, default:10dB)
    int     RoamingHighSNR;     //  Sets a high SNR threshold for iChip in Roaming mode.(0 - 255dB, default:30dB)
}WiFiConfig_t;      //  Wi-Fi通用参数设置

typedef struct
{
    char    SSID[64];           //  SSID
    int     WEPMode;            //  WEP mode: 0-disable; 1-enable 64-bit; 2-enable 128-bit
    int     WEPKeyIdx;          //  WEP key index (1 - 4)
    char    WEPKey[4][32];      //  4 sets of WEP Key
    char    WPAPSK[64];         //  Personal Shared Key Pass-Phrase, ("":WPA security is disabled, else enable for WPA-PSK encryption key)
}WiFiDefAP_t;

typedef struct
{
    char    SSID[64];           //  SSID
    int     SecurityType;       //  Sets the Wireless LAN security type.(0:No security; 1:WEP-64; 2:WEP-128; 3:WPA-TKIP; 4:WPA2-AES)
    char    WPAPSK[64];         //  Personal Shared Key Pass-Phrase.
    char    WEPKey[32];         //  WEP Key
}WiFiAPx_t;       //  can be configured up to 10 sets(0~9), the default set is No.0.


#define WIFI_STATUS_NOTPRESENT          0       //  Wlan adapter not present
#define WIFI_STATUS_DISABLED            1       //  Wlan adapter disabled
#define WIFI_STATUS_SEARCHING           2       //  Searching for initial connection
#define WIFI_STATUS_CONNECTED           4       //  Connected
#define WIFI_STATUS_OUTOFRANGE          5       //  Out of range

typedef struct
{
    int     Status;             /*  port status:    0:Wlan adapter not present;
                                                    1:Wlan adapter disabled;
                                                    2:Searching for initial connection;
                                                    4:Connected;
                                                    5:Out of range      */
    int     XferRate;           //  Transfer rate in the range 1..54
    int     SigLevel;           //  signal level [%], in the range 0..100
    int     LnkQual;            //  Link quality [%], in the range 0..100
    char    SSID[64];           //  SSID
    int     SecurityType;       //  The Wireless LAN security type.(0:No security; 1:WEP-64; 2:WEP-128; 3:WPA-TKIP; 4:WPA2-AES)
    int     Channel;            //  Current communication channel
}WiFiStatus_t;  // Wi-Fi连接状态

typedef struct
{
    char    SSID[64];           //  SSID
    int     SecurityType;       //  The Wireless LAN security type.(0:No security; 1:WEP-64; 2:WEP-128; 3:WPA-TKIP; 4:WPA2-AES)
    int     SigStrength;        //  Signal strength, 0-low, 1-good, 2-excellent
}WiFiAPInfo_t;      //  available ap info in surrounding area

int WifiGetCurConnect(WiFiStatus_t *CurStatus);
int WifiScanAP(int maxlistnum, WiFiAPInfo_t APlist[]);
int WifiGetLocalIP(char *ipstr);
int WifiSetParms(const WiFiConfig_t *parms);
int WifiSetDefAP(const WiFiDefAP_t *defAPParms);
int WifiSetAP(int index, const WiFiAPx_t *APxParms);
int WifiSoftReset(int mode);
int WifiPing(char *ipstr, int timeoutms);



/* UART */
#define     PORT_COM1				0x00        //  下载口
#define     PORT_MODEM				0x01        //  modem模块
#define     PORT_WNET				0x02        //  无线模块和以太网
#define     PORT_PINPAD             0x03        //  PINPAD口
#define     PORT_GPS                0x04        //  GPS
#define		PORT_EXT				0x05		//	扩展口
#define     PORT_XX               0x06        //  6210下载口
#define     PORT_6210LINK8210     0x07        //  6210跟8210连接USB0
#define     PORT_6210LINK8210ACM  0x08        //  6210跟8210连接ACM(USB)
#define     PORT_HID              0x09
#define     PORT_6210LINK821001   0x0A        //  6210跟8210连接USB1
#define     PORT_6210LINK821002   0x0B        //  6210跟8210连接USB2
#define     PORT_6210LINK821003   0x0C        //  6210跟8210连接USB3
#define     PORT_6210LINK821004   0x0D        //  6210跟8210连接USB4
#define     PORT_6210LINK821005   0x0E        //  6210跟8210连接USB5
#define     PORT_6210LINK821006   0x0F        //  6210跟8210连接USB6
#define     PORT_6210LINK821007   0x10        //  6210跟8210连接USB7
#define     PORT_6210LINK821008   0x11        //  6210跟8210连接USB8
#define     PORT_6210LINK821009   0x12        //  6210跟8210连接USB9
#define     PORT_6210LINKACM01    0x89        //  6210跟8210连接ACM1
#define     PORT_6210LINKACM02    0x8A        //  6210跟8210连接ACM2
#define     PORT_6210LINKACM03    0x8B        //  6210跟8210连接ACM3
#define     PORT_6210LINKACM04    0x8C        //  6210跟8210连接ACM4
#define     PORT_6210LINKACM05    0x8D        //  6210跟8210连接ACM5
#define     PORT_6210LINKACM06    0x8E        //  6210跟8210连接ACM6
#define     PORT_6210LINKACM07    0x8F        //  6210跟8210连接ACM7
#define     PORT_6210LINKACM08    0x90        //  6210跟8210连接ACM8
#define     PORT_6210LINKACM09    0x91        //  6210跟8210连接ACM9

#define     PORT_6210MISUSB       0x80        //  6210MIS纯USB口



#define     PORT_RET_OK             0x00        //  成功
#define     PORT_RET_NOTEMPTY       0x01        //  发送缓冲还有数据
#define     PORT_RET_PORTERR        0x02        //  逻辑串口号非法
#define     PORT_RET_NOTOPEN        0x03        //  串口未打开
#define     PORT_RET_BUSY           0x05        //  没有物理串口资源分配
#define     PORT_RET_MODEM_INUSE    0xf0		//  MODEM通道正被系统占用（仅针对PortNum=PORT_MODEM）
#define     PORT_RET_PARAERR        0xfe        //  无效的通讯参数
#define     PORT_RET_TIMEOUT        0xff        //  超时

int portOpen(int PortNum, const char *PortPara);
int portClose(int PortNum);
int portSends(int PortNum, const void *SendBuf, size_t SendLen);
int portPrintf(int PortNum, const char *FmtBuf, ...);
int portRecv(int PortNum, uint8_t *RecvBuf, uint32_t TimeOutMs);
int portReset(int PortNum);
int portCheckSendBuf(int PortNum);
int portCheckRecvBuf(int PortNum);


// IC 卡
//============================ ICC MODULE ======================================
// ICC数据通信结构
// 卡座号
#define     ICC_USERCARD          0x00        // 半埋卡座
#define     ICC_SAM1              0x11        // SAM卡座1
#define     ICC_SAM2              0x12        // SAM卡座2
#define     ICC_SAM3              0x13        // SAM卡座1
#define     ICC_SAM4              0x14        // SAM卡座2

// ICC卡通信参数
#define     ICC_VOL_5V            0x03        //  5V卡
#define     ICC_VOL_3V            0x02        //  3V卡
#define     ICC_VOL_1_8V          0x01        //  1.8V卡

#define     ICC_ISO               0x01        // 符合ISO7816规范,上电初始化通信速度为372CLK/Bit
#define     ICC_EMV               0x00        // 符合EMV v4.1规范,上电初始化通信速度为372CLK/Bit
#define     ICC_19200             0x02        // 上电初始化通信速度为186CLK/Bit
#define     ICC_38400             0x03        // 上电初始化通信速度为93CLK/Bit

// ICC数据通信结构
typedef struct
{
    UINT8       CLA;
    UINT8       INS;
    UINT8       P1;
    UINT8       P2;
    UINT8       Lc;
    UINT8       DataIn[300];
    UINT8       LeFlag;
    UINT8       Le;
}APDU_SEND;

typedef struct
{
    UINT32      LenOut;
    UINT8       DataOut[300];
    UINT8       SWA;
    UINT8       SWB;
}APDU_RESP;

//  返回值定义
#define     ICC_SUCCESS          0x00      // 卡操作成功
#define     ICC_TIMEOUT          0x01      // 卡超时响应
#define     ICC_COMMERR          0x02      // 卡通信错误(奇偶错或LRC错)
#define     ICC_RESPERR          0x03      // 卡片返回信息错误
#define     ICC_CARDOUT          0x04      // 卡不在位
#define     ICC_NOPOWER          0x05      // 卡未上电
#define     ICC_ERRORSLOT        0x06      // 卡座错误
#define     ICC_ERRPARA          0x07      // 参数错误

// API
int iccSetPara(int CardSlot, uint32_t CardVol, uint32_t CardType);
int iccDetect(int CardSlot);
int iccPowerUp(int CardSlot, uint32_t *pAtrLen, void *pAtrData);
int iccExchangeAPDU(int CardSlot, const APDU_SEND *pSendApdu, APDU_RESP *pRespApdu);
int iccPowerDown(int CardSlot);
int iccGetSlotFd(int CardSlot); //20130122

typedef struct
{
	uint32_t WaitTime_PollTypeA;		// TypeA寻卡期间单条指令等待时间
	uint32_t WaitTime_PollTypeB;		// TypeB寻卡期间单条指令等待时间
	uint32_t WaitTime_Mifare;			// Mifare One卡操作期间指令等待时间
	uint32_t WaitTime_RATS;				// 激活Pro卡指令等待时间
	uint32_t WaitTime_CardResponse;		// T=CL卡操作等待卡响应超时时间
	uint32_t WaitTime_CompleteCmd;		// 等待终端完成本次数据发送接收总时间
	uint32_t WaitTime_DelayBefore;		// 发送下一条指令前的等待时间
	uint32_t WaitTime_IdleRF;			// 复位载波的等待时间
} MIF_TIME_STRUCT;

// 卡类型定义
#define MIF_M1_S50      		0x00	// S50存储结构的M1卡
#define MIF_M1_S70      		0x01	// S70存储结构的M1卡
#define MIF_ULTRALIGHT  		0x02	// mifare ultralight
#define MIF_DESFIRE     		0x03	// mifare desfire
#define MIF_PRO         		0x04	// 支持ISO14443-4协议的TypeA卡
#define MIF_PRO_S50     		0x05
#define MIF_PRO_S70     		0x06
#define MIF_TYPEB       		0x07
#define MIF_TYPEB_TCL   		0x08	// 支持ISO14443-4协议的TypeB卡
#define MIF_FELICA      		0x09	// SonyFeliCa
#define MIF_UNKNOWEN_TYPEA 		0x0A	// 未知类型的TypeA卡

// S50/S70扇区认证密钥类型
#define MIF_KEYA        		0x00
#define MIF_KEYB        		0x01

// 定义当前的载波类型
#define MIF_TYPE_A      		0x00	// TypeA
#define MIF_TYPE_B      		0x01	// TypeB
#define MIF_TYPE_C1      		0x02	// FeliCa, 212kbits/s
#define MIF_TYPE_C2      		0x03	// FeliCa, 424kbits/s

// 指示灯标示
#define MIF_RED_LED             0x00	// 红色灯
#define MIF_BLUE1_LED           0x01	// 蓝色一号灯
#define MIF_BLUE2_LED           0x02	// 蓝色二号灯
#define MIF_GREEN_LED           0x03	// 绿色灯

// 定义返回值
#define MIF_RET_SUCCESS         0		// 操作正确
#define MIF_RET_TRANSERR        -1		// 通信失败
#define MIF_RET_TIMEOUT         -2		// 超时错误
#define MIF_RET_PROTERR         -3		// 数据错
#define MIF_RET_MULTICARD       -4		// 多卡存在
#define MIF_RET_NOCARD          -5		// 无卡
#define MIF_RET_ERRPARA         -6		// 参数错误
#define MIF_RET_NOACTIVED       -7		// 卡片未激活
#define MIF_RET_NOAUTH          -8		// 扇区未验证
#define MIF_RET_OVERFLOW        -9		// 内部缓冲区溢出
#define MIF_RET_AUTHERR         -10		// 认证失败
#define MIF_RET_VALUEERR        -11		// 值错误
#define MIF_RET_NOSUPPORTTCL    -12		// 该Tag不支持T=CL协议
#define MIF_RET_NOSUPPORTCMD    -13		// 该命令Tag不支持
// API函数定义
void mifOpen(void);							// 打开射频卡模块
void mifClose(void);						// 关闭射频卡模块
void mifSelectMifType(uint32_t Type);
void mifResetRF(uint32_t dur_time);
int mifReqA(void *ATQA);
int mifWupA(void *ATQA);
int mifAnticollion(uint8_t SEL, void *UID);
int mifSelect(uint8_t SEL, const void *UID, uint8_t *SAK);
int mifHaltA(void);
int mifRATS(uint32_t *Len, uint8_t *Data);
int mifPPPS(uint8_t BRit, uint8_t BRti);
int mifAuthByKey(uint32_t BlockNo, uint32_t KeyType, uint8_t *Key, uint8_t *UID);
int mifAuthByE2(uint32_t BlockNo, uint32_t KeyType, uint8_t *UID);
int mifReadBlock(uint32_t BlockNo, uint8_t *BlockData);
int mifWriteBlock(uint32_t BlockNo, uint8_t *BlockData);
int mifSetValue(uint32_t BlockNo, uint32_t Value);
int mifIncrement(uint32_t BlockNo, uint32_t Value);
int mifDecrement(uint32_t BlockNo, uint32_t Value);
int mifRestore(uint32_t BlockNo);
int mifTransfer(uint32_t BlockNo);
int mifWriteKey(uint32_t KeyType, uint32_t Sector, uint8_t *Key);
int mifWriteE2(uint32_t StartAddr, uint32_t Len, uint8_t *Data);
int mifReadE2(uint32_t StartAddr, uint32_t Len, void *Data);
int mifReadULData(uint32_t PageAddr, void *Data);
int mifWriteULData(uint32_t PageAddr, const void *Data);
int mifWriteULData16(uint32_t PageAddr, uint8_t *Data);
int mifReqB(uint32_t AFI, uint32_t SlotNum, uint32_t *LenOfATQB, uint8_t *ATQB);
int mifWupB(uint32_t AFI, uint32_t SlotNum, uint32_t *LenOfATQB, uint8_t *ATQB);
int mifSlotMarker (uint32_t Slot, uint32_t *LenOfATQB, uint8_t *ATQB);
int mifAttriB(uint8_t *UID, uint8_t BRit, uint8_t BRti);
int mifHaltB(uint8_t *UID);
int mifProcess(uint32_t SendLen, uint8_t *SendData, uint32_t *RecvLen, uint8_t *RecvData, uint32_t TimeOut);
int mifExchangeAPDU(uint32_t SendLen, uint8_t *SendData, uint32_t *RecvLen, uint8_t *RecvData);
int mifFeliCaExchange(uint32_t SendLen, uint8_t *SendData, uint32_t *RecvLen, uint8_t *RecvData, uint32_t TimeOutValue);
void mifLEDControl(uint8_t LEDNo, uint8_t Mode);
void mifSetTimeOut(MIF_TIME_STRUCT *ptTimeOutStruct);
void mifGetTimeOut(MIF_TIME_STRUCT *ptTimeOutStruct);
int mifCheckOnlyOneCard(uint8_t Mode);
int InListPassiveTarget(uint8_t Mode, int InitLen, uint8_t *InitInfo, int *CardInfoLen, uint8_t *CardInfo);
int InATR(uint32_t *RespLen, uint8_t *ATRInfo);
int InPSL(uint8_t BRit, uint8_t BRti);
int InDataExchange(int SendLen, uint8_t *SendData, uint32_t *RecvLen, uint8_t *RecvData);
void InRelease(void);

//======================= SLECARD卡模块========================================
// 定义卡片类型
#define     EM_SYNCARD_SLE4418             0x00             // 4418卡
#define     EM_SYNCARD_SLE4428             0x01             // 4428卡
#define     EM_SYNCARD_SLE4432             0x02             // 4432卡
#define     EM_SYNCARD_SLE4442             0x03             // 4442卡

// 函数返回值定义
#define     EM_SLECARD_SUCCESS             0x00            // 操作成功
#define     EM_SLECARD_ERROR               0x01            // 操作失败
#define     EM_SLECARD_CARDOUT             0x02            // 卡不在位
#define     EM_SLECARD_NOPOWER             0x03            // 卡未上电
#define     EM_SLECARD_NOVERIFY			   0x04			   // 卡片密码未校验
#define     EM_SLECARD_CHGABLE			   0x05			   // 该地址数据可修改
#define     EM_SLECARD_CHGDISABLE		   0x06			   // 该地址数据不可修改

// API函数定义
int sleCardPowerUp(uint8_t *pucCardType, uint8_t *pucATRLen, void *pvATRData);
void sleCardPowerDown(void);
int sleCardVerify (const void *PassWord , int PassWordLen);
int sleCardReadSCErr(uint8_t* pucSCErrCount);
int sleCardChangeSC(const void* pvNewSecCode, int Len);
int sleCardLock(void);
int sleCardRead(uint32_t uiAddr, uint32_t uiLen, void* pvRecData );
int sleCardReadStatus(uint32_t uiAddr, uint8_t* pucStatus);
int sleCardCheckData(uint32_t uiAddr, uint8_t ucData);
int sleCardWrite (uint32_t uiAddr,uint32_t uiLen, const void *pvDataIn);

// AT24CXX卡模块驱动
// 卡类型定义
#define AT24C01			1			// 128byte
#define AT24C02			2			// 256byte
#define AT24C04			4			// 512byte
#define AT24C08			8			// 1024byte
#define AT24C16			16			// 2048byte
#define AT24C32			32			// 4096byte
#define AT24C64			64			// 8192byte
#define AT24C128		128			// 16384byte
#define AT24C256		256			// 32768byte
#define AT24C512		512			// 65536byte
#define AT24C1024		1024	    // 131072byte

int at24cxxOpen(uint32_t CardType);
int at24cxxRead(int StartAddr, int Len, void *pucDataOut);
int at24cxxWrite(int StartAddr, int Len, const void *pucDataIn);
void at24cxxClose(void);


// 加解密API

uint16_t CalcCRC16(const void *Buf, uint32_t Len, uint16_t PreCrcValue);

#include "tdes.h"
#include "sha.h"
#include "md5.h"
#include "rsa.h"
#include "aes.h"


//add by justin
void lcdDrawRect(int x, int y, int width, int height, DFBColor BorderColor, DFBColor FillColor);
void lcdClrRect(int x, int y, int width, int height);
void lcdClrAbsLine(int BeginLine, int EndLine);
int lcdDrBgLogo(int ipointx, int ipointy,int iwidth, int ihigh, char *filename);
void lcdSetFgColor(DFBColor Color);
DFBColor lcdGetFgColor(DFBColor *Color);
void lcdSetBgColor(DFBColor Color);
DFBColor lcdGetBgColor(DFBColor *Color);
int lcdGetFontHeight(void);
void windowResize(int x, int y, int w, int h);
void lcdSurfaceResize(int w, int h);
IDirectFBWindow *lcdGetBgWindow(void);
IDirectFBWindow *lcdGetFgWindow(void);
void lcdFlipLine(int BeginLine, int EndLine);
IDirectFBFont *lcdGetFontExt(int mode);
int lcdSetFontExt(IDirectFBFont *font, int mode);
int GetPortFd(int PortNum);
void TouchDisplayTitle(int Col,int Line,uint32_t Mode,const char * format,...);
int TouchDisplayWhite(int Col, int Line, uint32_t Mode,  const char *format, ...);
void TouchDisplayTime(int Col, int Line, uint32_t Mode, const char *format, ...);
void lcdSetFgColorVariety(DFBColor Color);
DFBColor lcdGetFgColorVariety(DFBColor *Color);
void lcdSetBgColorVariety(DFBColor Color);
DFBColor lcdGetBgColorVariety(DFBColor *Color);

int seos_get_libversion(char *pszVer);
void prnClearSurface(void);
void SetCharType(uint8_t charType);
void cl_led_support(uint8_t IsSupport);//支持非接屏幕灯设置



#ifdef __cplusplus
	}
#endif /* __cplusplus */

#endif /* __SEOS_H__ */

