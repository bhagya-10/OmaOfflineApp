#ifndef NEW8210_H
#define NEW8210_H

#ifdef _POS_TYPE_8210

#include <ped.h>
#include <ppp.h>
#include <wnet.h>
#include <iccard.h>
#include <iso14443.h>
#include "seos.h"
#include "emvapi.h"
#include <openssl/des.h>
#include <openssl/sha.h>
#include "postslib.h"

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef INT32
#define INT32  int
#endif

#ifndef INT8
#define INT8 char
#endif

#define CAPKLIST struct issuer_ca_public_key
#define AIDLIST  struct terminal_aid_info
// #define EMV_RET_SUCCESS  0


// 软件版本V+银联规定的软件版本号(2byte) + 自定义的版本号(4byte). 每次修改后都需要重新修改版本号,目前我们定义的是"年份+月份"
#define VERSION_STRING     "V311204"  
#define CERTIFICATE_NUMBER "123125"   //8210的证书编号是P3125
#define SRC_VERSION        "V2.0.121206"

#define MAX_LINE_SCREEN        6   // 6行每页

//  返回值定义
#define     FILE_EXIST          1
#define     FILE_NOEXIST        2
#define     MEM_OVERFLOW        3
#define     TOO_MANY_FILES      4
#define     INVALID_HANDLE      5
#define     INVALID_MODE        6
#define     FILE_NOT_OPENED     8
#define     FILE_OPENED         9
#define     END_OVERFLOW        10
#define     TOP_OVERFLOW        11
#define     NO_PERMISSION       12
#define     FS_CORRUPT          13

#define     KB_REVDISP              0x00

#define     EMV_PIN_KEY_ID          31    // 脱机明文PIN KEY ID
#define     NET_ERR_USER_CANCEL     1043    //  User press key cancel
#define     EMV_RET_CANCEL          EMV_ERRNO_CANCEL
#define     EMV_RET_SUCCESS         0
#define     EMV_RET_NOAPP           EMV_ERRNO_NO_APP
#define     EMV_RET_ICCERR          EMV_ERRNO_SW
#define     ONLINE_DENIAL           EMV_ERRNO_ONLINE_ERROR
/****************************************************************************
 功    能:      计算两个整数数字/表达式最大(小)值
 输入参数:
    a,b         待比较的整数/表达式
 输出参数:
    none
 返 回 值:      最大(小)值
****************************************************************************/

#ifndef  MAX
#define MAX(a, b)       ( (a)>=(b) ? (a) : (b) )
#endif

#ifndef  MIN
#define MIN(a, b)       ( (a)<=(b) ? (a) : (b) )
#endif 

enum {
	NFC_ERR_NONE				= 0,
	NFC_ERR_TIMEOUT				= ETIME,		/* 超时无返回 */
	NFC_ERR_EIO					= EIO,			/* 读卡芯片错误 */
	NFC_ERR_EPROTO				= EPROTO,		/* 卡片返回数据不符合ISO14443规范 */
	NFC_ERR_ECOMM				= ECOMM,		/* 卡片返回数据奇偶错或者CRC校验错 */
	NFC_ERR_EBADE				= EBADE,		/* 卡片返回数据长度错误 */
	NFC_ERR_EMULTIHOP			= EMULTIHOP,	/* 感应区内多卡存在 */
	NFC_ERR_ENODATA 			= ENODATA,		/* 感应区内无T=CL卡 */
};

void CheckPinPad(void);
int  BootInit(int argc, char *argv[]);

#define  PARITY_NONE        0x00
#define  PED_RET_APPTSSAKERR     2009        //  No TSSAK for this APP in APP
#define  PED_RET_PEDTSSAKERR     0x07        //  No TSSAK for this APP in PED
#define  KEY_TYPE_PIN       KEY_TYPE_PINK
#define  KEY_TYPE_MAC       KEY_TYPE_MACK
#define  FORMAT_0           PIN_BLOCK_FORMAT_0
#define  TYPE_MACKEY        KEY_TYPE_MACK
#define  TYPE_MASTERKEY     KEY_TYPE_MASTK

//#define PED_RET_NOKEY           (PED_RET_BASE_NO +  9)	//  No designated key in PED
INT32 PedWriteMasterKey(UINT32 Mode, UINT16  srcKeyID, UINT16 destKeyID, UINT16 KeyLen, UINT8 *KeyBuf);
INT32 PedWritePinKey(UINT32 Mode, UINT16  MasterKeyID, UINT16 PinKeyID, UINT16 PinKeyLen, UINT8 *PinKey);
INT32 PedWriteMacKey(UINT32 Mode, UINT16  MasterKeyID, UINT16 MacKeyID, UINT16 MacKeyLen, UINT8 *MacKey);
INT32 PedGetPin(UINT16 PinKeyID, UINT8 *ExpectPinLen, UINT8 *CardNo, INT32 Mode, UINT8 *PinBlock);
INT32 PedGetMac(UINT16 MacKeyID, UINT8 *DataIn, UINT16 InLen, INT32 Mode, UINT8 *MacOut);
INT32 PedTDEA(UINT32 Mode, INT32 KeyType, UINT16  KeyID, UINT32 DataLen, UINT8 *InBuf, UINT8 *OutBuf);
#define  PedDelKey          ped_delete_key

int32_t PedGetSensitiveTimer(int32_t SensitiveType);

void  lcdDrLogo(INT32 X, INT32 Y, UINT8 *pucBitMap);
INT32  sysGetPowerStatus(void);

int start_timer(time_t sec, suseconds_t usec);
int stop_timer(void);
int    iccCardExchangeAPDU(int fd, uint32_t sendlen, const void *senddata, uint32_t *recvlen, void *recvdata);

int    icc_fd;
extern void *glbEpp;
extern int  g_epp_Port_Num;
extern int(*__ICCARD_EXCHANGE_APDU)(int fd, uint32_t sendlen, const void *senddata, uint32_t *recvlen, void *recvdat);

int sysSetbacklighttime(int keep);

int sysGetbacklighttime(int *keep);

int    ThreadPreDial(void);
int    ThreadInitModule(void);
int    ThreadJoinInitModule(void);
void   timer_thread(union sigval v); 
int    Init_timer(void);

enum EPP_ERROR_CODE
{
    EPP_SUCCESS = 0,
	EPP_RECV_PACKET_ERROR=2001,
	EPP_PORT_OPEN_ERROR,
	EPP_SEND_PACKET_ERROR,  // 3
	EPP_PACKET_LEN_ERROR,
	EPP_RECV_TIMEOUT,  // 5
	EPP_PACKET_LEN_TOO_LONG,
	EPP_CRC_CHECK_ERROR,  // 7
	EPP_OPEN_FILE_ERROR,
	EPP_SEEK_FILE_ERROR,  // 9
	EPP_WRITE_FILE_ERROR,
	EPP_READ_FILE_ERROR,  // 11
	EPP_CONFIGURE_INVALID,
	EPP_CONFIGURE_MAC_ERROR,  // 13
	EPP_NO_PIN,
	EPP_SEND_CMD_ERROR,  // 15
	EPP_RECV_CMD_ERROR,
	EPP_RECV_RET_ERROR,  // 17
	EPP_RECV_LEN_ERROR,
	EPP_MAC_CHECK_ERROR,  // 19
	EPP_AUTHEN_FAILED,
	EPP_INPUT_PARAM_ERROR,  // 21
	EPP_USER_PRESS_CANCEL,
	EPP_INPUT_CMD_ERROR,  // 23
	EPP_INPUT_KEY_INDEX_ERROR,
	EPP_INPUT_MAC_LEN_ERROR, // 25
	EPP_INPUT_MODE_ERROR,
	EPP_KEY_TYPE_ERROR,  // 27
	EPP_KEY_INDEX_ERROR,
	EPP_MASTER_KEY_INDEX_ERROR,  // 29
	EPP_BMP_ERROR,
	EPP_PIN_CFG_LANGE_TYPE_ERROR,  // 31
	EPP_PIN_CFG_SYMBOL_TYPE_ERROR,
	EPP_INPUT_DATA_LEN_ERROR, // 33
	EPP_SET_PORT_ERROR,
	EPP_CFG_ERROR, // 35
	EPP_CLOSE_FAILED,
	EPP_KSN_LEN_ERROR,
};

enum N20_RETURN_CODE{
    RET_ERROR_BASE = 2300,
	RET_PASSWORD_ERROR,  // 1
	RET_BOOT_LEN_ERROR,
	RET_BOOT_HASH_ERROR, // 3
	RET_BOOT_MAC_ERROR,
	RET_NOT_UBSK,  // 5
	RET_CHECK_UBSK_FAILED,
	RET_CHECK_KSK_FAILED,  // 7
	RET_NOT_LCK,
	RET_NOT_AUTH,  // 9
	RET_NOT_LOGON,
	RET_RECV_FAILED,  // 11
	RET_AUTH_CHECK_FAILED,
	RET_MAC_CHECK_FAILED,  // 13
	RET_CRC_CHECK_ERROR,
	RET_KVC_CHECK_ERROR,  // 15
	RET_NOT_APP_FREE_SPACE,
	RET_APP_NOT_EXIST,  // 17
	RET_KEY_TYPE_ERROR,
	RET_KEY_TYPE_NOT_SUPPORT,  // 19
	RET_MAC_LEN_ERROR,
	RET_MAIN_KEY_INDEX_ERROR,  // 21
	RET_MAIN_KEY_NOT_EXIST,
	RET_KEY_INDEX_ERROR,  // 23
	RET_KEY_LEN_ERROR,
	RET_KEY_BE_USE,  // 25
	RET_MAIN_KEY_BE_USE,  // 26
	RET_MAC_ALG_MODE_ERROR,
	RET_KEY_GET_ERROR,
	RET_KEY_OVER_FLOW,  // 29
	RET_KEY_SAME_ERROR,
	RET_KEY_NOT_EXIST,  // 31
	RET_KEY_MAC_CHECK_FAILED,
	RET_KEY_KSN_LEN_ERROR,  // 33
	RET_KEY_BDK_LEN_ERROR,
	RET_USER_PRESS_CANCEL_EXIT,  // 35
	RET_USER_INPUT_TIME_OUT,
	RET_KSN_LEN_RERROR,  // 37
	RET_APP_NUM_OVER_FLOW,
	RET_RW_LENGTH_ERROR,  // 39
	RET_PIN_KEY_TIME_LOCK,
	RET_MAC_KEY_TIME_LOCK,  // 41
	RET_SET_PIN_TIME_OUT_ERROR,
	RET_WRITE_EEPROM_FAILED,  // 43
	RET_RECV_LEN_ERROR,
	RET_PARAM_ERROR  // 45
};
  
#define KEY_TYPE_FIXED_PIN   0x11
int   EppLight(unsigned long dwMode, unsigned long dwTimeMs);
int   EppDisplayLogo(int X, int Y, const uint8_t *pbyBmpLogoIn);
int   EppDisplayString(int X, int Y, int iMode, const void *str, int iStrLen);
int   EppClearScreen(void);
int   EppSetPinInputTimeout(unsigned long wTimeout100Ms);
int   EppGetPin(uint32_t byKeyType, uint32_t byKeyIndex, uint32_t byDisplayMode,
            uint32_t byMode, const uint8_t *pbyCardInfo, const uint8_t *pbyLenInfo, uint8_t *pbyPinBlockOut);
int   EppRestoreDefaultIdleLogo(void);
int   EppGetMac(uint32_t byKeyType, uint32_t byKeyIndex, const uint8_t *pbyMacDataIn,
                        unsigned long wMacLen, uint32_t byMode, uint8_t *pbyMacOut);
int   EppDownloadAppKey(uint32_t byKeyType, uint32_t byMode, uint32_t byKeyIndex,
                        const uint8_t *pbyKeyDataIn, uint32_t byKeyLen, uint32_t byMasterKeyIndex);
int   EppClose(void);
int   EppBeep(unsigned long dwFrequency, unsigned long dwTimeMs);
int   EppClearAKey(uint32_t byKeyType, uint32_t byKeyIndex);


#endif

#endif
