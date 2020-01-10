
/****************************************************************************
NAME
	Poslib.h - 实现公共函数

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	pyming		2008.03.14		- add precompile process
****************************************************************************/

#ifndef _POSLIB_H
#define _POSLIB_H

#include "posapi.h"
#include "New8210.h"

// #define _POS_DEBUG
#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef INT32
#define INT32  int
#endif

#ifndef INT8
#define INT8 char
#endif

#ifndef TRUE
	#define TRUE	1
	#define FALSE	0
#endif

// macros for vDes()
#define ONE_DECRYPT     0           // 单DES解密
#define ONE_ENCRYPT     1           // 单DES加密
#define TRI_DECRYPT     2           // 三DES解密(16 bytes key)
#define TRI_ENCRYPT     3           // 三DES加密(16 bytes key)
#define TRI_DECRYPT3    4           // 三DES解密(24 bytes key)
#define TRI_ENCRYPT3    5           // 三DES加密(24 bytes key)

// macros for vCalcMac()
#define MAC_ANSIX99     0           // ANSI9.9 standard MAC algorithm
#define MAC_FAST        1           // HyperCom fast MAC algorithm

#define ERR_OPENFILE     -1          //打开文件错误
#define ERR_READFILE     -2          //读文件错误
#define ERR_SEEKFILE     -3          //定位文件错误
#define ERR_WRITEFILE    -4          //写文件错误


// macros for PubGetString()
#define NUM_IN			0x01	// 数字输入
#define ALPHA_IN		0x02	// 字母数字混合输入
#define PASS_IN			0x04	// 秘密输入
#define CARRY_IN		0x08	// 有传入数据
#define ECHO_IN			0x10	// 回显传入数据,按键则清除原数据
#define HEX_IN          0x20    // 十六进制输入      
#define AUTO_FONT		0x80	// RFU


// macros for screen functions
//#define NUM_MAXZHCHARS      18          // 每行最多字符数目(大字体)
#define NUM_MAXCOLS         128         // 每行最多列数
#define NUM_MAXCOLNO        (NUM_MAXCOLS-1)     // 最大列号
#define NUM_MAXLINES        8           // 最多行数
#define NUM_MAXLINENO       (NUM_MAXLINES-1)    // 最大行号
#define NUM_MAXCHARS		21

#define DISP_LINE_LEFT		0x80
#define DISP_LINE_RIGHT		0x40
#define DISP_LINE_CENTER	0x20
#define DISP_SCR_CENTER		0x10

// menu display mode
#define MENU_NOSPEC		0x00	// 自动显示
#define MENU_USESNO		0x01	// 使用菜单自己的编号(优先于MENU_AUTOSNO)
#define MENU_AUTOSNO	0x02	// 自动编号
#define MENU_CENTER		0x10	// 中间对齐显示
#define MENU_PROMPT		0x20	// 强制提示信息在底部
#define MENU_ASCII		0x40	// 强制显示小字体
#define MENU_CFONT		0x80	// 强制显示大字体(优先于MENU_ASCII)

// types for PubGetMenu()
typedef void (*MenuFunc)(void);
typedef struct _tagMenuItem{
	UINT8		bVisible;						// TRUE: show it, FALSE: hide
    char        szMenuName[NUM_MAXCHARS+1];		// menu name  ENGLISH
    MenuFunc    pfMenuFunc;						// callback function(if set)
}MenuItem;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************
Function:      检测系统电量，是否允许操作
****************************************************************************/
int  PubCheckPosPower(void);

/****************************************************************************
 Function:      以6频率发音60ms，然后停止80ms. 连续3次
****************************************************************************/
void PubBeepOk(void);		//

/****************************************************************************
 Function:       以1频率发音200ms，然后停止200ms. 
****************************************************************************/
void PubBeepErr(void);		//

/****************************************************************************
 Function:      以6频率发音800ms
****************************************************************************/
void PubLongBeep(void);	//

/****************************************************************************
 Function:      检测用户是有按下按钮的操作.
 Return Code:
    TRUE        有按键按下.
    FALSE       无按键按下.
****************************************************************************/
int PubChkKeyInput(void);

/****************************************************************************
 Function:      在限制的时间内，等待用户输入.
 Param In:
    ulWaitTimeMs   >0:  等待的输入键值的时间，以秒为单位.
                  <＝0:  一直等待,直到有按键输入
 Return Code:
                用户输入的按键值，或者超时退出.
注意：由于PCI原因，在未注册界面，输入的数字按键可能是无效值--0xFF
****************************************************************************/
int PubWaitKey(ulong ulWaitTime);


/****************************************************************************
 Function:      在设定的之间内等待用户输入 "确认" 或者 "取消".
 Param In:
    nWaitTime   >0: 等待的输入键值的时间，以秒为单位.
                =0:  一直等待,直到有按键输入
 Param Out:
    none
 Return Code:
    TRUE        输入"确认".
    FALSE       输入"取消",或者超时退出.
****************************************************************************/
UINT8 PubYesNo(ulong ulWaitTime);


/****************************************************************************
 Function:      应用程序出现错误时候，显示错误并强制关机.
 Param In:
    pszfile     发生错误所在的文件名.
    uiLine      发生错误所在的行.
****************************************************************************/
void PubHalt(UINT8 *pszfile, uint uiLine);


//显示
/****************************************************************************
 Function:      Show title at line zero & Clear screen first, if needed.
 Param In:
    bClrScreen  TRUE:  Clear screen before show title.
                FALSE: Keep original screen.
    pszTitle    Pointer of title to be displayed. string should end with '\0'.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void PubDisplayTitle(int IfClrScr, char *pszTitle);	//

/****************************************************************************
 Function:      Show message(CFONT) at ucLine, center-aligned. If the text
                is out of right-border, extra characters will be truncated.
 Param In:
    ucLine      Line number to show message. If ucLine & DISP_REVERSE is true, the
                background of the text is reversed.
    pszMsg      Pointer of string to be displayed. string should end with '\0'.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void PubDisplayMsg(UINT8 ucLine, UINT8 *pszMsg);			//

/****************************************************************************
 Function:      Show two message(CFONT) at ucLine and ucLine+2, center-aligned.
                If each text is out of right-border, extra characters will be
                truncated.
 Param In:
    ucLine      Line number to show first message. If ucLine & DISP_REVERSE is true,
                the background of the text is reversed. Since 2 lines of CFONT will
                be displayed, ucLine should be 0 to 4.
    pszMsg1     Pointer of 1st string to be displayed. string should end with '\0'.
    pszMsg2     Pointer of 2nd string to be displayed. string should end with '\0'.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void PubShowTwoMsg(UINT8 ucLine, UINT8 *pszMsg1, UINT8 *pszMsg2);		//


/****************************************************************************
 Function:      Display a string(CFONT) at specified line and backgroud.
 Param In:
    pszStr      Pointer of string to be displayed. string should end with '\0'.
	            get rid of bRever
    ucPosition  the value : (dispMode & 0x0F) indicates the line where to display,
                while the value : (dispMode & 0x0F) indicates the align rule.
                Line number should be 0 to 6, and align value should be DISP_SCR_CENTER,
                DISP_LINE_CENTER, DISP_LINE_RIGH and DISP_LINE_LEFT
 Param Out:
    none
 Return Code:   none
 Example:
                PubDispString("This is a sample", TRUE, DISP_LINE_LEFT+3);
                  will display the string in CFONT with black background, left align
                on the 4th line (use 0x00 to 0x07 to indicate line 1 to 8)
****************************************************************************/
void PubDispString(void *pszStr, UINT8 ucPosition);


/****************************************************************************
 Function:      Convert a numeric string(no dot) to the format of amount.
 Param In:
    pszPrefix   Prefix to the amount output, such as "HKD" in "HKD*****.**",
                   allow 4 character at most. if NULL, no prefix will be attached.
	pszIn		Pointer to input amount string (without dot), e.g "314277".
	ucDeciPos   Decimal position of the numeric input string. should be less than 4.
	               e.g. if ucDeciPos==2, "14277" may output "142.77"
				   if ucDeciPos==0, "14277" may output "14,277" or "14277"
    ucMisc      Misc parameter,
                   GA_SEPARATOR : use ',' to separater large amount: "12,345.67"
				   GA_NEGATIVE	: display negative sign: "SGD-123,45.67"
 Param Out:
	pszOut      Pointer to output amount string, the result may be like
	               "SGD$-3,142.77". (depending on call parameter)
 Return Code:
    TRUE        Keyboard is pressed.
    FALSE       No key is pressed.
****************************************************************************/
UINT8 PubConvAmount(UINT8 *pszPrefix, UINT8 *pszIn, UINT8 ucDeciPos, UINT8 *pszOut, UINT8 ucMisc);


/****************************************************************************
 Function:      Display menu & call menu process function, the max # of menu
                item is 99. Max characters of menu item name is 19 bytes.
 Param In:
    pszPrompt   Title/prompt of menu group(optional), NULL or zero length
                means none. string should end with '\0'.
    pstMenu     Array of menu structure, null string of menu name means end
                of menu items.
    ucMode      Mode for show menu:MENU_NOSPEC,MENU_CENTERR,MENU_AUTOSNO,
                MENU_USESNO,MENU_PROMPT,MENU_CFONT,MENU_ASCII
    nTimeOut    >0: Seconds before user select menu.
                =0:  Infinite.
 Param Out:
    none
 Return Code:
    >=0         Index of menu user selected. In the most time, user only need
                to set the callback function member and let PubGetMenu() call
                it automatically. But in some other unusual time user need set
                callback function member to NULL, and use this returned index
                to call the process function. This is useful for dynamically
                generated menus or applications which need pass parameters to
                the process function. For example, application manager can
                read information of applications and build menu, show it, then
                run application by the returned index of the menu.
    -1          NULL menu pointer.
    -2          Invalid menu content.
    -3          No menu content.
    -4          User cancel or timeout.
****************************************************************************/
int PubGetMenu(UINT8 *pszPrompt, MenuItem *pstMenu, UINT8 ucMode, UINT32 nTimeOut);


/****************************************************************************
 Function:      Read boolean parameter value.
 Param In:
    ucLine      Line number where to display prompt message.
    pszPrompt   Prompt message.
 Param Out:
    pucResult   Pointer of boolean parameter value.
 Return Code:
    0           OK.
    0xFF        User cancelled.
****************************************************************************/
UINT8 PubGetBoolParam(UINT8 ucLine, UINT8 *pszPrompt, UINT8 *pucResult);


/****************************************************************************
 Function:      Input amount value.
 Notice:        This function will use whole line6-7 and the last 4 character
                  of line4-5 to display the amount string and prefix, so do not
				  place prompt information at these places.
 Param In:
    pszPrefix   Pointer to the prefix string. e.g "HKD", "SGD$". max allow
                  4 bytes. if NULL of empty content, prefix won't be displayed.
    ucDeciPos   decimal position of the amount, should be same with tims/Protims para.
                  allow 0 to 5.
    ucMinLen    minimal amount length(digits).
    ucMaxLen    Maximal amount length(digits).
    ucTimeout   waiting timeout(seconds).
    ucMisc      Misc parameter,
                   GA_SEPARATOR : use ',' to separater large amount: "HKD$12,345.67"
				   GA_NEGATIVE	: display negative sign: "SGD-123,45.67"
 Param Out:
	pszData     Pointer to output string (end with '\0')
    pszData     Pointer of parameter string value.
 Return Code:
    0           OK.
    0xFE        Illegal parameter.
    0xFF        User cancelled.
****************************************************************************/
#define GA_SEPARATOR	0x80
#define GA_NEGATIVE		0x40
#define GA_CARRY        0x20
UINT8 PubGetAmount(UINT8 *pszPrefix, UINT8 ucDeciPos,
				  UINT8 ucMinLen, UINT8 ucMaxLen, UINT8 *pszData,
				  UINT8 ucTimeout, UINT8 ucMisc);

// get string of number/password/alpha
UINT8 PubGetString(UINT8 ucMode, UINT8 ucMinlen, UINT8 ucMaxlen, UINT8 *pszOut, UINT8 ucTimeOut);


//UINT8 PubGetBetString(UINT8 ucMode, UINT8 ucMinlen, UINT8 ucMaxlen, UINT8 ucMaxValue, UINT8 *pszOut, UINT8 ucTimeOut);

//文件
/****************************************************************************
 Function:     从当前文件偏移位置处,读出数据
 Param In:
    fd          文件句柄--读出的文件
	psData      数据缓冲区
    iNeedByte   希望读出的数据长度.
 Return Code:
    实际读出的数据长度.
****************************************************************************/
int PubFReadN(int fd, void *psData, int iNeedBytes);

/****************************************************************************
 Function:      从指定的文件名中读出数据.
 Param In:
    pszFileName 文件名.
    lOffset     偏移位置.
    iLen        读取的文件数据长度.
 Param Out:
    pszData     数据缓冲区.
 Return Code:
    0:          OK.
    -1:         打开文件错误.
    -3:         定位文件错误.
    -2:         读文件错误.
****************************************************************************/
int PubFileRead(char *pszFileName, long lOffset, void *psData, int iLen);		//

/****************************************************************************
 Function:      扩展权限的文件读出函数，该函数类似与PubFileRead.
                但是该函数可以读取其他应用的文件数据，或者是内核数据
****************************************************************************/
int PubFileReadEX(uint AppID,uint FileType,UINT8 *pszFileName, long lOffset, void *psData, int iLen);		//

/****************************************************************************
 Function:      将数据保存到文件中指定偏移位置
 Param In:
    fd          写入的文件句柄
	psData      保存的数据
    iNeedBytes  保存数据的长度
 Return Code:
    共计写入的数据长度.
****************************************************************************/
int PubFWriteN(int fd, void *psData, int iNeedBytes);

/****************************************************************************
 Function:      保存文件.
 Param In:
    pszFileName 文件名.
    lOffset     数据保存的偏移位置.
    pszData     数据缓冲区.
    iLen        写入的数据长度.
 Return Code:
	0:          OK.
   -1:         打开文件错误.
   -3:         定位文件错误.
   -4:         写文件错误.
****************************************************************************/
int PubFileWrite(char *pszFileName, long lOffset, void *psData, int iLen);	//

/****************************************************************************
 Function:      扩展权限的文件保存函数，该函数类似与PubFileWrite.
 但是该函数可以保存其他应用的文件数据，或者是内核数据
****************************************************************************/
int PubFileWriteEX(uint AppID,uint FileType,UINT8 *pszFileName, long lOffset, void *psData, int iLen);	//


//计算
/****************************************************************************
Function:      多功能DES加密/解密运算
Param In:
    ucMode      DES加密/解密模式:
                ONE_ENCRYPT --> 单DES加密
                ONE_DECRYPT --> 单DES解密
                TRI_ENCRYPT --> 三DES加密
                TRI_DECRYPT --> 三DES解密
    psData:     待加密/解密的数据(8字节)
    psKey:      DES运算的密钥(单倍长度或者两倍长度,根据ucMode确定)
 Param Out:
    psResult    DES运算的结果(8字节)
 Return Code:   none
****************************************************************************/
void PubDes(UINT8 ucMode, UINT8 *psData, UINT8 *psKey, UINT8 *psResult);	//

/****************************************************************************
 Function:      报文MAC运算函数
 Param In:
    ucMode      报文MAC运算模式:
                MAC_FAST    --> 快速MAC运算(HyperCom快速方式)
                MAC_ANSIX99 --> ANSI x9.9 方式
    psKey:      MAC运算的密钥(8字节)
    psMsg:      计算MAC的数据
    uiLength:   计算MAC的数据的长度
 Param Out:
    psMAC       报文的MAC(8字节)
 Return Code:   none
****************************************************************************/
void PubCalcMac(UINT8 ucMode, UINT8 *psKey, UINT8 *psMsg, uint uiLen, UINT8 *psMAC);	//


/****************************************************************************
 Function:      检查字符串是否是中文字符串
 Param In:     
       psString:   待检测的字符串
	   uiLen:      字符串长度
 Param Out:
       无
 Return Code:   TRUE: 是中文字符串; FALSE: 非中文字符串
****************************************************************************/
int PubCheckCHNString(char *psString, int iLen);	//

#ifdef _POS_DEBUG

/****************************************************************************
 Function:      Show HEX message on screen.
 Param In:
    pszTitle    Title of the message.
    psMsg       Pointer of message to be displayed.
    uiMsgLen    Bytes of the message.
    nTimeOut    Seconds between user interaction.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void DispHexMsg(UINT8 *pszTitle, UINT8 *psMsg, uint uiMsgLen, short nTimeOut);

// print a line as hexadecimal format
int DispHexLine(UINT8 *pszBuff, uint uiOffset, UINT8 *psMsg, uint uiMsgLen);

// For Debug use, disp file name and line
void DispAssert(UINT8 *pszFileName, ulong ulLineNo);

    // debug macro for boolean expression
	#define _POS_DEBUG_WAIT		15
    #define PubASSERT(expr)                                           \
                if( !(expr) ){                                      \
                    DispAssert((UINT8 *)__FILE__, (ulong)(__LINE__));       \
                }

    // print string debug information
    #define PubTRACE0(sz)                                             \
            {                                                       \
                lcdCls();                                           \
                lcdDisplay(0, 0, DISP_CFONT, "%s", sz);                    \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACE1(sz, p1)                                         \
            {                                                       \
                lcdCls();                                           \
                lcdDisplay(0, 0, DISP_CFONT, sz, p1);                      \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACE2(sz, p1, p2)                                     \
            {                                                       \
                lcdCls();                                           \
                lcdDisplay(0, 0, DISP_CFONT, sz, p1, p2);                  \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACE3(sz, p1, p2, p3)                                 \
            {                                                       \
                lcdCls();                                           \
                lcdDisplay(0, 0, DISP_CFONT, sz, p1, p2, p3);              \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACEHEX(t, s, l)    DispHexMsg((t), (s), (l), _POS_DEBUG_WAIT)

#else /* _POS_DEBUG */

    #define PubASSERT(expr)
    #define PubTRACE0(sz)
    #define PubTRACE1(sz, p1)
    #define PubTRACE2(sz, p1, p2)
    #define PubTRACE3(sz, p1, p2, p3)
    #define PubTRACEHEX(t, s, l)

#endif /* _POS_DEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _UTIL_H */
