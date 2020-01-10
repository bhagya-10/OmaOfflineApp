#include "PosLib.h"
#include "posapi.h"
#include "MathLib.h"
#include <ctype.h>
#include "Display.h"
#include "Tools.h"
#include "global.h"
#include "New8210.h"
#include "ped.h"

/********************** Internal macros declaration ************************/
#ifdef _POS_DEBUG
	#define LEN_DBGDATA         1024    // 显示数据最大长度
	#define MAX_CHARS           5       // 每行可以格式化的字符数目
	#define CHARS_PERLINE       21      // 每行的字符数目
	#define MAX_LOG_MSG         (((LEN_DBGDATA+MAX_CHARS-1)/MAX_CHARS)*CHARS_PERLINE)
#endif /* _POS_DEBUG */

#define LEN_GETSTRING		512	// for PubGetString()

/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
#ifdef _POS_DEBUG
static void GetBaseName(UINT8 *pszFullPath, UINT8 *pszBaseName);
#endif

static void MapChar2Index(UINT8 ch, int *piRow, int *piCol);
static void ShowWorkBuf(UINT8 ucMode, UINT8 *pszWorkBuff, int iLen);
/********************** Internal variables declaration *********************/
#ifdef _POS_DEBUG
static UINT8 glszDbgInfo[MAX_LOG_MSG+128];
#endif

static UINT8 sgMinLineNo;
static char *pszAlphaTbl[10] =
{
	"0.,*# ~`!@$%^&-+=(){}[]<>_|\\:;\"\'?/",
	"1QZqz ", "2ABCabc", "3DEFdef",
	"4GHIghi", "5JKLjkl", "6MNOmno",
	"7PRSprs", "8TUVtuv", "9WXYwxy",
};

/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/
/*********************************************************************************************/
/*********************************************************************************************/
int  PubCheckPosPower(void)
{
	int iRet;
	
	iRet = sysGetPowerStatus();
	if (iRet==1 || iRet ==0)
	{
		PubBeepOk();
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDisplayTitle(TRUE,"System warning");
			PubDispString("low battery", 2|DISP_LINE_CENTER);
			PubDispString("please recharge", 4|DISP_LINE_CENTER);
		}
		else
		{
			PubDisplayTitle(TRUE,"系统自测");
			PubDispString("电量低,请充电!", 2|DISP_LINE_CENTER);
			PubDispString("low battery !", 4|DISP_LINE_CENTER);
		}
		PubWaitKey(5);
		return FALSE;
	}
	
	return TRUE;
}


void PubBeepOk(void)
{
	int	  iCnt;

	for (iCnt=0; iCnt<3; iCnt++)
	{
		sysBeef(6, 60);
		sysDelayMs(80);
	}
}

// 按键音
void PubBeepErr(void)
{
	sysBeef(6, 200);
	sysDelayMs(200);
}

void PubLongBeep(void)
{
	sysBeef(6, 150);
}

//  检测用户是有按下按钮的操作.
int PubChkKeyInput(void)
{
	if (kbhit()==YES)
	{
		return TRUE;
	}

	return FALSE;
}

// 等待按键输入，由于PCI原因，该函数有一定的限制。
// 在未注册界面，输入的数字按键可能是无效值--0xFF
int PubWaitKey(ulong ulWaitTime)
{
	int   iKey;

	kbFlush();
	lcdFlip();
	if (ulWaitTime>0)
	{
		iKey = kbGetKeyMs((int)(ulWaitTime*1000));
	}
	else
	{
		iKey = kbGetKey();
	}
		
	return iKey;
}

UINT8 PubYesNo(ulong ulWaitTime)
{
	int   iKeyValue;
	
	lcdFlip();
	while (1)
	{
		iKeyValue = PubWaitKey(ulWaitTime);
		if (iKeyValue==KEY_ENTER)
		{
			return  TRUE;
		}
		if (iKeyValue==KEY_CANCEL || iKeyValue==KEY_TIMEOUT)
		{
			return  FALSE;
		}
	}
}

//...
void PubHalt(UINT8 *pszfile, uint uiLine)
{
	int iMode,iErr;

	lcdCls();
	sysGetLastError(&iMode,&iErr);

	lcdDisplay(0, 0, DISP_ASCII, "File:%s,\nLine:%d,\nRet:%d", (char *)pszfile, uiLine, iErr);
	lcdDisplay(0, 4, DISP_CFONT, "PLS RECORD");
	lcdDisplay(0, 4, DISP_CFONT, "THEN RESTART POS");
	lcdFlip();

	while (1)
	{
	};
}


////////////////////////////////////// 显示输出 //////////////////////////////////////
void PubDisplayTitle(int IfClrScr, char *pszTitle)
{
	int ilen,iBegindex;
	char  buffer[16+1];

	PubASSERT(pszTitle!=NULL);
	if (pszTitle==NULL)
	{
		return;
	}

	if (IfClrScr)
	{
		lcdCls();
	}

	memset(buffer,0,sizeof(buffer));
	ilen = strlen(pszTitle);
	if (ilen>=16)
	{
		ilen = 16;
	}

	iBegindex = (16-ilen)/2;
	memset(buffer,' ',sizeof(buffer)-1);
	memcpy(buffer+iBegindex,pszTitle,ilen);

	lcdDisplay(0, 0, DISP_CFONT|DISP_INVLINE|DISP_MEDIACY,"%-16.16s", buffer);

	return;
}


void PubDisplayMsg(UINT8 ucLine, UINT8 *pszMsg)
{
//	int		iLen;
	UINT8	ucMode;

	PubASSERT(pszMsg!=NULL);
	if (pszMsg==NULL)
	{
		return;
	}

	ucMode =  (ucLine & DISP_REVERSE) | DISP_CFONT |DISP_MEDIACY;
	ucLine &= ~DISP_REVERSE;
	lcdClrLine(ucLine, (UINT8)(ucLine+1));

// 	iLen = strlen((char *)pszMsg);
// 	if (iLen > ucMaxZHChars)
// 	{
// 		iLen = ucMaxZHChars;
// 	}

// 	lcdDisplay((UINT8)((NUM_MAXCOLS-16*iLen)/2), ucLine, ucMode,
// 			"%.*s", iLen, (char *)pszMsg);
	lcdDisplay((UINT8)0, ucLine, ucMode,"%s",(char *)pszMsg);

	return;
}


void PubShowTwoMsg(UINT8 ucLine, UINT8 *pszMsg1, UINT8 *pszMsg2)
{
	if (ucLine>4)
	{
		ucLine = 4;
	}
	PubDisplayMsg(ucLine, pszMsg1);
	PubDisplayMsg((UINT8)(ucLine+2), pszMsg2);
}

//api.c--apiDispString
//...
void PubDispString(void *pszStr, UINT8 ucPosition)
{
	UINT8	mode, x, y, ucLen, buffer[60];
	UINT8   ucMode;
	int     iScreenwidth, iScreenheight;
	UINT8   ucMaxZHChars = 16;
	
	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
		ucMaxZHChars = 18;
	}

	mode  = ucPosition & 0xf0;
	y     = ucPosition & 0x0f;
	ucLen = strlen(pszStr);
	if (ucLen > ucMaxZHChars)
	{
		ucLen = ucMaxZHChars;
	}

	ucMode =0;
	switch(mode)
	{
		case DISP_SCR_CENTER:
			lcdCls();
			x = (64 - ucLen * 4);
			y = 3;
			break;

		case DISP_LINE_CENTER:
			lcdClrLine(y, (UINT8) (y + 1));
//			x = (64 - ucLen * 4);
			x = 0;
			ucMode = DISP_MEDIACY;
			break;

		case DISP_LINE_RIGHT:
			x = (128 - ucLen * 8);
			break;

		case DISP_LINE_LEFT:
		default:
			x = 0;
			break;
	}

	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, pszStr, ucLen);
	lcdClrLine(y, (UINT8)(y+1));
	lcdDisplay(x, y, DISP_CFONT|ucMode, "%s", (char *)buffer);

	return;
}



UINT8 PubConvAmount(UINT8 *pszPrefix, UINT8 *pszIn, UINT8 ucDeciPos, UINT8 *pszOut, UINT8 ucMisc)
{
	int		ii, iInLen, iIntegerLen;
	UINT8	bSeparator, bNegative, szBuff[40+1], szTemp[40];
	UINT8	*pRead, *pWr;

	PubASSERT(pszIn!=NULL && pszOut!=NULL && ucDeciPos<4);

// 	iInLen = strlen((char *)pszIn);		//输入金额长度
// 	pRead  = pszIn;

	sprintf((char *)szBuff, "%.40s", pszIn);
	PubTrimHeadChars((char *)szBuff, '0');
	iInLen = strlen((char *)szBuff);		//输入金额长度
	pRead  = szBuff;
	pWr = pszOut;

	bSeparator = ucMisc & GA_SEPARATOR;		//whether use ',' to separate huge amount
	bNegative  = ucMisc & GA_NEGATIVE;		//whether display negative amount

	if (pszPrefix!=NULL && *pszPrefix!=0)
	{
		pWr += sprintf((char *)pWr, "%.4s ", pszPrefix);
//		pWr += sprintf((char *)pWr, "%.3s ", pszPrefix);
//		pWr += MIN(strlen((char *)pszPrefix), 4);
	}
	if (bNegative)
	{
		*(pWr++) = '-';
	}

	//小数点前
	if (iInLen>ucDeciPos)	//小数点前非0
	{
		iIntegerLen = iInLen - ucDeciPos;
		ii = iIntegerLen;
		while (ii--)
		{
			*(pWr++) = *(pRead++);
			if (bSeparator && (ii%3==0) && ii)
			{
				*(pWr++) = ',';
			}
		}
	}
	else
	{
		*(pWr++) = '0';
	}

	//小数点及其后
	if (ucDeciPos!=0)
	{
		sprintf((char *)szTemp, "%.*s", ucDeciPos, pRead);
		PubAddHeadChars((char *)szTemp, ucDeciPos, '0');
		sprintf((char *)pWr, ".%s", szTemp);
		pWr += (ucDeciPos+1);
	}
	else
	{
		*(pWr++) = 0;
	}

	return 0;
}


// display menu
int PubGetMenu(UINT8 *pszPrompt, MenuItem *pstMenu, UINT8 ucMode, UINT32 nTimeOut)
{
	static UINT8 *pszDefPrompt = (UINT8 *)"PLS INPUT:";
	int     iKey,iLen, IsChnString,iScreenwidth, iScreenheight;
	UINT8   ucMaxLen, ucLen, ucCol, ucKeepChars, ucMenuFont, ucRever;
	UINT8   ucStartLineNo, ucLineNo, ucMenuLines, ucEndLineNo;
	UINT8   ucMenuNum, ucCurItem, ucPageNum, ucPageNo;
	UINT8	bOneKeySel, bAutoSno, szInput[2+1], ucInputCnt, ucMenuCnt;
	UINT8	ucSelNo, bBottomPrompt, bUseSno, ucCnt, ucShowItemNum;
	UINT8	szMenuNo[2+1], bMenuOk, ucActNum;
	char  *pDotSign;
	short	iCnt;
	UINT8   ucMaxLines = 4, ucMaxZHChars = 16;

	PubASSERT( pstMenu!=NULL );
	kbFlush();

	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
#ifdef  JAY_TEST
		ucMaxLines = 6;
#else
		ucMaxLines = 8;//显示8行?
#endif
		ucMaxZHChars = 18;
	}

	IsChnString =  FALSE;
	ucCurItem = 0;
	// Analyze menu content
	if( pstMenu==NULL )
	{
		return -1;
	}
	bUseSno = (ucMode & MENU_USESNO);
	for(ucShowItemNum=ucMaxLen=ucMenuNum=0; pstMenu[ucMenuNum].szMenuName[0]; ucMenuNum++)
	{
		ucLen	 = strlen((char *)pstMenu[ucMenuNum].szMenuName);

		if (!IsChnString) // 检查是否是中文字符串
		{
			IsChnString = PubCheckCHNString((char *)pstMenu[ucMenuNum].szMenuName,ucLen);
		}

		ucMaxLen = MAX(ucLen, ucMaxLen);
		if( pstMenu[ucMenuNum].bVisible )
		{
			ucShowItemNum++;
		}
		if( !bUseSno )
		{
			continue;
		}
		pDotSign = strchr(pstMenu[ucMenuNum].szMenuName, '.');
		if( pDotSign==NULL )
		{
			return -2;
		}
		iLen = pDotSign - pstMenu[ucMenuNum].szMenuName;
		if( iLen ==0  || iLen > 2  )
		{
			return -2;
		}
		// 暂时不检查菜单自带动编号是否为数字
	}
	if( ucMenuNum==0 || ucShowItemNum==0 )
	{
		return -3;
	}

	// process options
	ucMenuNum     = min(ucMenuNum, (UINT8)99);			// 最多99个菜单项可以被显示
	bBottomPrompt = bUseSno  ? TRUE  : (ucMode & MENU_PROMPT);
//	bBottomPrompt = bUseSno  ? FALSE : (ucMode & MENU_PROMPT);
	bAutoSno      = bUseSno  ? FALSE : (ucMode & MENU_AUTOSNO);
	bOneKeySel    = bAutoSno ? (ucMenuNum<=9) : FALSE;	// 一键选择
	ucKeepChars   = 0;
	if( ucMode & MENU_AUTOSNO )
	{
		ucKeepChars = (ucMenuNum<=9) ? 2 : 3;	// 计算用于显示编号的保留字符数目
	}

	// analyze menu font
	ucMenuFont = (ucMaxLen+ucKeepChars>ucMaxZHChars) ? DISP_ASCII: DISP_CFONT;
	if( (ucMode & MENU_CFONT) || (ucMode & MENU_ASCII) )
	{
		ucMenuFont = (ucMode & MENU_CFONT) ? DISP_CFONT : DISP_ASCII;
	}
	// 如果是中文字符串，则强制为中文显示
	if (IsChnString)
	{
		ucMenuFont = DISP_CFONT;
	}

	// calculate max. chars per lines and the start column
	if( ucMenuFont== DISP_ASCII )
	{
		ucMaxLen = min(ucMaxLen, (UINT8)(NUM_MAXCHARS-ucKeepChars));
		ucCol    = (ucMode & MENU_CENTER) ? (6*(NUM_MAXCHARS-ucKeepChars-ucMaxLen)/2)   : 0;
	}
	else
	{
		ucMaxLen = min(ucMaxLen, (UINT8)(ucMaxZHChars-ucKeepChars));
		ucCol    = (ucMode & MENU_CENTER) ? (8*(ucMaxZHChars-ucKeepChars-ucMaxLen)/2) : 0;
	}

	// analyze menu start/end lines
	ucStartLineNo = 0;
	if( pszPrompt==NULL || *pszPrompt==0 )
	{
		if( bUseSno )
		{
			pszPrompt   = pszDefPrompt;
			ucMenuLines = (ucMenuFont==DISP_ASCII) ? NUM_MAXLINES-2 : ucMaxLines-1;
		}
		else if( bAutoSno )
		{
			if( ucMenuFont==DISP_ASCII )
			{
				ucMenuLines = bOneKeySel ? NUM_MAXLINES : NUM_MAXLINES-2;
			}
			else
			{
				ucMenuLines = bOneKeySel ? ucMaxLines : ucMaxLines-1;
			}
			pszPrompt = bOneKeySel ? NULL : pszDefPrompt;
		}
		else
		{
			ucMenuLines = (ucMenuFont==DISP_ASCII) ? NUM_MAXLINES : ucMaxLines;
			pszPrompt   = NULL;
		}
		if( pszPrompt!=NULL )
		{
			ucStartLineNo = bBottomPrompt ? 0 : 2;
		}
	}
	else
	{
		if( !bBottomPrompt )
		{
			ucStartLineNo = (bOneKeySel || !bAutoSno) ? 2 : 0;
		}
		ucMenuLines = (ucMenuFont==DISP_ASCII) ? NUM_MAXLINES-2 : ucMaxLines-1;
	}
	ucEndLineNo = (ucMenuFont==DISP_ASCII) ? (ucStartLineNo+ucMenuLines-1) : (ucStartLineNo+ucMenuLines*2-1);
	ucPageNum   = (ucShowItemNum+ucMenuLines-1)/ucMenuLines;

	// show title/prompt message
	if( ucStartLineNo>0 )
	{
		PubDisplayTitle(TRUE, (char *)pszPrompt);
	}
	else if( pszPrompt!=NULL )
	{
		PubDispString(pszPrompt, 6|DISP_LINE_LEFT);
	}

	for(ucMenuCnt=0; ucMenuCnt<ucMenuNum; ucMenuCnt++)
	{
		if( pstMenu[ucMenuCnt].bVisible )
		{
			break;
		}
	}
	ucSelNo = ucMenuCnt;
	// Do menu loops
	ucPageNo = ucInputCnt = 0;
	memset(szInput, 0, sizeof(szInput));
	while( 1 )
	{
		// Light/Close up & down icon
		if( bUseSno || bAutoSno )
		{
			lcdSetIcon(ICON_UP,   (UINT8)(ucPageNo>0		   ? OPENICON : CLOSEICON));
			lcdSetIcon(ICON_DOWN, (UINT8)(ucPageNo<ucPageNum-1 ? OPENICON : CLOSEICON));
		}
		else
		{
			lcdSetIcon(ICON_UP,   (UINT8)(ucSelNo>0		      ? OPENICON : CLOSEICON));
			lcdSetIcon(ICON_DOWN, (UINT8)(ucSelNo<ucMenuNum-1 ? OPENICON : CLOSEICON));
		}

		lcdClrLine(ucStartLineNo, ucEndLineNo);
		if( ucStartLineNo==0 && pszPrompt!=NULL )
		{
			if (ucMaxZHChars != 16)
			{
				lcdDisplay(260, 10, DISP_CFONT, "%2s", szInput);
			}
			else
			{
				lcdDisplay(112, 6, DISP_CFONT, "%2s", szInput);
			}
		}

		for(ucActNum=ucMenuCnt=0; ucMenuCnt<ucMenuNum; ucMenuCnt++)
		{
			if( !pstMenu[ucMenuCnt].bVisible )
			{
				continue;
			}
			if( ucActNum>=ucPageNo*ucMenuLines )
			{
				ucCurItem = ucMenuCnt;
				break;
			}
			ucActNum++;
		}
//		ucCurItem = ucPageNo * ucMenuLines;
		// Display menu
// 		lcdClrLine(ucStartLineNo, ucEndLineNo);
		for(ucMenuCnt=0,ucLineNo=ucStartLineNo; ucMenuCnt<ucMenuLines;)
		{
			if( pstMenu[ucCurItem].bVisible )
			{
				if( bAutoSno )
				{
					lcdDisplay(ucCol, ucLineNo, ucMenuFont, "%*d.%.*s",
							(int)(ucKeepChars>0 ? ucKeepChars-1 : 0),
							(int)(ucCurItem+1), ucMaxLen, pstMenu[ucCurItem].szMenuName);
				}
				else
				{
					ucRever = 0;
					if( !bUseSno )
					{
						ucRever = (ucCurItem==ucSelNo) ? DISP_INVLINE: 0;
					}
					lcdDisplay(ucCol, ucLineNo, (UINT8)(ucMenuFont|ucRever), "%.*s",
							ucMaxLen, pstMenu[ucCurItem].szMenuName);
				}
				ucLineNo += ((ucMenuFont==DISP_ASCII) ? 1 : 2);
				ucMenuCnt++;
			}
			ucCurItem++;
			if( ucCurItem>=ucMenuNum )
			{
				break;
			}
		}

		lcdFlip();
		iKey = PubWaitKey(nTimeOut);		// Waiting for user input
		lcdSetIcon(ICON_UP,   CLOSEICON);
		lcdSetIcon(ICON_DOWN, CLOSEICON);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID || iKey == KEY_TIMEOUT)
		{
			return -4;
		}
		else if( iKey==KEY_UP )
		{
			if( bUseSno || bAutoSno )
			{
				ucPageNo = (ucPageNo==0) ? (ucPageNum-1) : (ucPageNo-1);
			}
			else
			{
				ucSelNo  = (ucSelNo>0) ? ucSelNo-1 : ucMenuNum-1;
				for(iCnt=(int)ucSelNo; iCnt>=0; iCnt--)
				{
					if( pstMenu[iCnt].bVisible )
					{
						break;
					}
				}
				if( iCnt<0 )
				{
					for(iCnt=(int)(ucMenuNum-1); iCnt>=0; iCnt--)
					{
						if( pstMenu[iCnt].bVisible )
						{
							break;
						}
					}
				}
				ucSelNo = (UINT8)iCnt;
				for(ucActNum=ucMenuCnt=0; ucMenuCnt<ucSelNo; ucMenuCnt++)
				{
					if( pstMenu[ucMenuCnt].bVisible )
					{
						ucActNum++;
					}
				}
				ucPageNo = ucActNum/ucMenuLines;
			}
		}
		else if( iKey==KEY_DOWN )
		{
			if( bUseSno || bAutoSno )
			{
				ucPageNo = (ucPageNo+1>=ucPageNum) ? 0 : ucPageNo+1;
			}
			else
			{
				ucSelNo  = (ucSelNo+1<ucMenuNum) ? ucSelNo+1 : 0;
				for(ucMenuCnt=ucSelNo; ucMenuCnt<ucMenuNum; ucMenuCnt++)
				{
					if( pstMenu[ucMenuCnt].bVisible )
					{
						break;
					}
				}
				if( ucMenuCnt>=ucMenuNum )
				{
					for(ucMenuCnt=0; ucMenuCnt<ucMenuNum; ucMenuCnt++)
					{
						if( pstMenu[ucMenuCnt].bVisible )
						{
							break;
						}
					}
				}
				ucSelNo = ucMenuCnt;
				for(ucActNum=ucMenuCnt=0; ucMenuCnt<ucSelNo; ucMenuCnt++)
				{
					if( pstMenu[ucMenuCnt].bVisible )
					{
						ucActNum++;
					}
				}
				ucPageNo = ucActNum/ucMenuLines;
			}
		}
		else if( (bUseSno || bAutoSno) && iKey>=KEY0 && iKey<=KEY9 )
		{
			if( bUseSno )
			{
				if( ucInputCnt<2 )
				{
					szInput[ucInputCnt++] = iKey;
					szInput[ucInputCnt]   = 0;
				}
			}
			else if( bOneKeySel )
			{
				if( iKey>=KEY1 && iKey<ucMenuNum+KEY1 )
				{
					ucSelNo = (UINT8)(iKey - KEY1);
					break;
				}
			}
			else if( ucInputCnt<(UINT8)(ucKeepChars-1) )
			{
				szInput[ucInputCnt++] = iKey;
				szInput[ucInputCnt]   = 0;
				ucSelNo = (UINT8)atoi((char *)szInput);
				if( ucSelNo>ucMenuNum )
				{
					szInput[--ucInputCnt] = 0;
				}
			}
		}
		else if( (bUseSno || bAutoSno) && iKey==KEY_CLEAR )
		{
			szInput[0] = 0;
			ucInputCnt = 0;
		}
		else if( (bUseSno || bAutoSno) && iKey==KEY_BACKSPACE )
		{
			if( ucInputCnt>0 )
			{
				szInput[--ucInputCnt] = 0;
			}
		}
		else if( iKey==KEY_ENTER )
		{
			if( !bAutoSno && !bUseSno )
			{
				break;
			}

			if( bOneKeySel || ucInputCnt==0 )
			{
				ucPageNo = (ucPageNo>=ucPageNum-1) ? 0 : ucPageNo+1;
			}
			else if( ucInputCnt>0 )
			{
				ucSelNo = (UINT8)atoi((char *)szInput);
				if( bUseSno )
				{
					for(bMenuOk=FALSE,ucCnt=0; ucCnt<ucMenuNum; ucCnt++)
					{
						pDotSign = strchr((char *)pstMenu[ucCnt].szMenuName, '.');
						PubASSERT( pDotSign!=NULL );
						sprintf((char *)szMenuNo, "%.*s",
								(int)min(2, pDotSign-pstMenu[ucCnt].szMenuName),
								pstMenu[ucCnt].szMenuName);
						if( atoi((char *)szMenuNo)==(int)ucSelNo )
						{
							ucSelNo = ucCnt;
							bMenuOk = TRUE;
							break;
						}
					}
					if( bMenuOk )
					{
						break;
					}
					szInput[0] = 0;
					ucInputCnt = 0;
				}
				else
				{
					if( ucSelNo>0 && ucSelNo<=ucMenuNum )
					{
						ucSelNo--;
						break;
					}
				}
			}
		}
	}	// end of while( 1

	if( pstMenu[ucSelNo].pfMenuFunc!=NULL )
	{
		(*pstMenu[ucSelNo].pfMenuFunc)();
	}

	return (int)ucSelNo;
}


//GA_SEPARATOR:	0x80
UINT8 PubGetAmount(UINT8 *pszPrefix, UINT8 ucDeciPos,
				  UINT8 ucMinLen, UINT8 ucMaxLen, UINT8 *pszData,
				  UINT8 ucTimeout, UINT8 ucMisc)
{
	UINT8	*pszNowChar, bSeparator, bNegative;
	UINT8	szBuff[20], szDispBuf[20], szDispBuf2[30];
	int		iKeyVal , iTotalLen;
	UINT32  uiOldTime, uiNewTime;

	if (ucDeciPos>5 ||
		ucMinLen>ucMaxLen || ucMaxLen>12 ||
		pszData==NULL)
	{
		return 0xFE;		//parameter illegal
	}

	memset(szBuff, 0, sizeof(szBuff));
	memset(szDispBuf, 0, sizeof(szDispBuf));
	memset(szDispBuf2, 0, sizeof(szDispBuf2));

	bSeparator = ucMisc & GA_SEPARATOR;		//whether use ',' to separate huge amount
	bNegative  = ucMisc & GA_NEGATIVE;		//whether display negative amount
	if (ucMisc & GA_CARRY)
	{
		strcpy((char *)szBuff,(char *)pszData);
	}
	pszNowChar = szBuff;

	while (1)
	{
		/* 显示 */
		memset(szDispBuf2, 0, sizeof(szDispBuf2));
		if (pszPrefix!=NULL && strlen((char *)pszPrefix)>0)
		{
			sprintf((char *)szDispBuf2, "%.4s", (char *)pszPrefix);
		}

		//protocol
		//PubConvAmount(UINT8 *pszPrefix, UINT8 *pszIn, UINT8 *pszOut,
		//				UINT8 ucDeciPos, UINT8 ucMisc)
		PubConvAmount(NULL, szBuff, ucDeciPos, szDispBuf, ucMisc);

		lcdClrLine(6, 7);
		iTotalLen = strlen((char *)szDispBuf) + strlen((char *)szDispBuf2);
		if (iTotalLen < 17)
		{
			strcat((char *)szDispBuf2, (char *)szDispBuf);
		}

		/* 处理按键 */
		uiOldTime = sysGetTimerCount();
		while(1)
		{
			if (iTotalLen < 17)
			{
				lcdDisplay(0, 6, DISP_CFONT, "%16.16s", szDispBuf2);
			}
			else
			{
				lcdDisplay(0, 6, DISP_CFONT, "%s..%s", szDispBuf2, &szDispBuf[iTotalLen-16+2]);
			}
			lcdFlip();
			iKeyVal = kbGetKeyMs(100);
			if (iKeyVal != KEY_TIMEOUT )
			{
				break;
			}
			uiNewTime = sysGetTimerCount();
			if (uiOldTime + ucTimeout*1000 < uiNewTime)
			{
				break;
			}
			
			DrawRect(120,62,127,63);
			lcdFlip();
			sysDelayMs(150);
		}
//		iKeyVal = PubWaitKey(ucTimeout);
		switch(iKeyVal)
		{
			case KEY0:
				if (pszNowChar==szBuff || pszNowChar>=szBuff+ucMaxLen)
				{
					PubBeepErr();
				}
				else
				{
					*pszNowChar++ = iKeyVal;
					*pszNowChar   = 0;
				}
				break;
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
				if (pszNowChar>=szBuff+ucMaxLen)
				{
					PubBeepErr();
				}
				else
				{
					*pszNowChar++ = iKeyVal;
					*pszNowChar   = 0;
				}
				break;

			case KEY_CLEAR:   // 清除
				pszNowChar  = szBuff;
				*pszNowChar = 0;
				break;

			case KEY_BACKSPACE:	// 退格
				if (pszNowChar<=szBuff)
				{
					PubBeepErr();
				}
				else
				{
					*(--pszNowChar) = 0;
				}
				break;

			case KEY_ENTER:
				if (pszNowChar>=szBuff+ucMinLen)
				{
					*pszData = 0;
					strcpy((char *)pszData, (char *)szBuff);
					if (strlen((char *)szBuff)==0)
					{
						strcpy((char *)pszData, "0");
					}
					return 0;
				}
				else
				{
					PubBeepErr();
				}
				break;
			case KEY_CANCEL:
			case KEY_TIMEOUT:
				return 0xFF;
			default:
				PubBeepErr(); //可能是这里发出的声音
				break;
		}
	}
}


UINT8 PubGetBoolParam(UINT8 ucLine, UINT8 *pszPrompt, UINT8 *pucResult)
{
	UINT8	ucFlag;
	int     iKeyValue,iScreenwidth, iScreenheight;
	UINT8   ucMaxZHChars = 16;
	
	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
		ucMaxZHChars = 18;
	}

	if (pszPrompt!=NULL && *pszPrompt!=0)
	{
		lcdClrLine(2, NUM_MAXLINENO);
		lcdDisplay(0, ucLine, DISP_CFONT, "%.*s", ucMaxZHChars, (char *)pszPrompt);
	}

	ucFlag = *pucResult;
	while(1)
	{
		PubDisplayMsg(NUM_MAXLINENO-1, (UINT8 *)(ucFlag ? "[NO]" : "[YES]"));
		lcdFlip();
		iKeyValue = PubWaitKey(60);
		if (iKeyValue==KEY_ENTER)
		{
			*pucResult = ucFlag;
			return 0;
		}
		if (iKeyValue==KEY_CANCEL || iKeyValue==KEY_TIMEOUT)
		{
			return 0xFF;
		}
		if ((iKeyValue==KEY_BACKSPACE)|| (iKeyValue==KEY_ALPHA)
			|| (iKeyValue ==KEY_INVALID) || (iKeyValue>=KEY0 && iKeyValue<=KEY9))
		{
			ucFlag = !ucFlag;
		}
	}   // end of while(1)
}

void MapChar2Index(UINT8 ch, int *piRow, int *piCol)
{
	int		i;
	char	*p;

	for(i=0; i<sizeof(pszAlphaTbl)/sizeof(pszAlphaTbl[0]); i++)
	{
		for(p=pszAlphaTbl[i]; *p; p++)
		{
			if( *p==toupper(ch) )
			{
				*piRow = i;
				*piCol = p-pszAlphaTbl[i];
				break;
			}
		}
	}
}

void ShowWorkBuf(UINT8 ucMode, UINT8 *pszWorkBuff, int iLen)
{
	int		iLineNum, iCnt, iLeftBytes, iScreenwidth, iScreenheight;
	UINT8	ucLineNo, sTemp[100];
	UINT8   ucMaxLines = 4, ucMaxZHChars = 16;
		
	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
		ucMaxLines = 8;
		ucMaxZHChars = 18;
	}

	iLineNum = (iLen+ucMaxZHChars-1)/ucMaxZHChars;
	if (iLineNum<=1)
	{
		iLineNum = 1;
	}
	if( iLineNum>=ucMaxLines )
	{
		lcdCls();
	}
	else
	{
		sgMinLineNo = min(sgMinLineNo, (UINT8)(NUM_MAXLINENO-iLineNum*2+1));
		lcdClrLine(sgMinLineNo, NUM_MAXLINENO);
	}

	memset(sTemp,  0, sizeof(sTemp));
	memset(sTemp, '*', ucMaxZHChars);
	ucLineNo = NUM_MAXLINENO-1;
	for(iCnt=1; iCnt<=3; iCnt++)
	{
		if( (iCnt*ucMaxZHChars)<=iLen )
		{
			iLeftBytes = ucMaxZHChars;
			if( ucMode & PASS_IN )
			{
				lcdDisplay(0, ucLineNo, DISP_CFONT, "%.*s", iLeftBytes, sTemp);
			}
			else
			{
				lcdDisplay(0, ucLineNo, DISP_CFONT, "%.*s", iLeftBytes, pszWorkBuff + iLen - iCnt*ucMaxZHChars);
			}
		}
		else
		{
			iLeftBytes = iLen - (iCnt-1)*ucMaxZHChars;
			if( iLeftBytes<=0 && iCnt>1 )
			{
				break;
			}
			if( ucMode & PASS_IN )
			{
				lcdDisplay(0, ucLineNo, DISP_CFONT, "%*.*s", ucMaxZHChars, iLeftBytes, sTemp);
			}
			else
			{
				lcdDisplay(0, ucLineNo, DISP_CFONT, "%*.*s", ucMaxZHChars, iLeftBytes, pszWorkBuff);
			}
			break;
		}
		ucLineNo -= 2;
	}
}

// 输入字母/数字
UINT8 PubGetString(UINT8 ucMode, UINT8 ucMinlen, UINT8 ucMaxlen, UINT8 *pszOut, UINT8 ucTimeOut)
{
	UINT8	bClearInData, szWorkBuff[LEN_GETSTRING+1];
	int		iLen, iIndex, iLastKey,iKeyValue,iCnt;
	UINT32  uiOldTime, uiNewTime;
	int     iScreenwidth, iScreenheight;
	UINT8   ucMaxZHChars = 16;
	
	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
		ucMaxZHChars = 18;
	}

	PubASSERT( (ucMode & NUM_IN) || (ucMode & PASS_IN) || (ucMode & ALPHA_IN) || (ucMode & HEX_IN) );
	PubASSERT( !((ucMode & CARRY_IN) && (ucMode & ECHO_IN)) );
	if( (ucMode & CARRY_IN) || (ucMode & ECHO_IN) )
	{
		sprintf((char *)szWorkBuff, "%.*s", LEN_GETSTRING, pszOut);
		if (ucMode & HEX_IN)
		{
			iLen = strlen((char *)szWorkBuff);
			for (iCnt=0; iCnt<iLen;iCnt++)
			{
				if ( ( ('0' <= szWorkBuff[iCnt]) && (szWorkBuff[iCnt] <= '9') ) ||
					 ( ('A' <= szWorkBuff[iCnt]) && (szWorkBuff[iCnt] <= 'F') ) || 
					 ( ('a' <= szWorkBuff[iCnt]) && (szWorkBuff[iCnt] <= 'f') ) )
				{
				   continue;	
				}
				else
				{
					memset(szWorkBuff, 0, sizeof(szWorkBuff));
				}
			}
		}
	}
	else
	{
		memset(szWorkBuff, 0, sizeof(szWorkBuff));
	}

	iLastKey = -1;
	iIndex = 0;
	iLen = strlen((char *)szWorkBuff);
	if( iLen>0 )
	{
		MapChar2Index(szWorkBuff[iLen-1], &iLastKey, &iIndex);
	}
	else
	{
		iLastKey = -1;
		iIndex   = 0;
	}

	sgMinLineNo  = 0xFF;
	bClearInData = (ucMode & ECHO_IN);
	while( 1 )
	{
		uiOldTime = sysGetTimerCount();
		while(1)
		{
			ShowWorkBuf(ucMode, szWorkBuff, iLen);
			if (ucMaxZHChars != 16)
			{
				lcdClrRect(150,144,170,145);
			}
			else
			{
				lcdClrRect(120,62,127,63);
			}
			lcdFlip();
			iKeyValue = kbGetKeyMs(100);
//			if (iKeyValue != KEY_TIMEOUT )
			if (iKeyValue != 0 )
			{
				break;
			}
			uiNewTime = sysGetTimerCount();
			if (uiOldTime + ucTimeOut*1000 < uiNewTime)
			{
				break;
			}

			if (ucMaxZHChars != 16)
			{
				lcdDrawFrame(150,144,170,145,ON);
			}
			else
			{
				lcdDrawFrame(120,62,127,63,ON);
			}
			lcdFlip();
			sysDelayMs(150);
		}
		if( iKeyValue==KEY_CANCEL ||iKeyValue==KEY_TIMEOUT )
		{
			return 1;
		}
		else if( iKeyValue==KEY_ENTER )
		{
			bClearInData = FALSE;
			if( iLen<(int)ucMinlen )
			{
				continue;
			}
			sprintf((char *)pszOut, "%s", szWorkBuff);
			break;
		}
		else if( iKeyValue>=KEY0 && iKeyValue<=KEY9 )
		{
			if( bClearInData )
			{	// clear in buffer
				szWorkBuff[0] = 0;
				iLen          = 0;
				iLastKey      = -1;
				bClearInData  = FALSE;
			}
			// save key in data
			if( iLen<(int)ucMaxlen )
			{
				szWorkBuff[iLen++] = iKeyValue;
				szWorkBuff[iLen]   = 0;
				iLastKey = iKeyValue - KEY0;
				iIndex   = 0;
			}
		}
		else if( iKeyValue==KEY_CLEAR )
		{
			szWorkBuff[0] = 0;
			iLen          = 0;
			iLastKey      = -1;
		}
		else if( iKeyValue==KEY_BACKSPACE )	// backspace
		{
			bClearInData = FALSE;
			if( iLen<=0 )
			{
				continue;
			}
			szWorkBuff[--iLen] = 0;
			if( iLen>0 )
			{
				MapChar2Index(szWorkBuff[iLen-1], &iLastKey, &iIndex);
			}
			else
			{
				iLastKey = -1;
				iIndex   = 0;
			}
		}
		else if( iKeyValue==KEY_ALPHA )
		{
			bClearInData = FALSE;
			if( !( (ucMode & ALPHA_IN) || (ucMode & HEX_IN)) || iLastKey<0 || iLen<1 )
			{
				continue;
			}
			iIndex = (iIndex+1)%strlen(pszAlphaTbl[iLastKey]);
			if (ucMode&HEX_IN)
			{
				if ( ( ('0' <= pszAlphaTbl[iLastKey][iIndex]) && (pszAlphaTbl[iLastKey][iIndex] <= '9') ) ||
					 ( ('A' <= pszAlphaTbl[iLastKey][iIndex]) && (pszAlphaTbl[iLastKey][iIndex] <= 'F') ) || 
					 ( ('a' <= pszAlphaTbl[iLastKey][iIndex]) && (pszAlphaTbl[iLastKey][iIndex] <= 'f') ))
				{
					szWorkBuff[iLen-1] = pszAlphaTbl[iLastKey][iIndex];
				}
				else
				{
					PubBeepErr();
					continue;
				}
			}
			else
			{
				szWorkBuff[iLen-1] = pszAlphaTbl[iLastKey][iIndex];
			}
		}
	}

	return 0;
}

/*
UINT8 PubGetBetString(UINT8 ucMode, UINT8 ucMinlen, UINT8 ucMaxlen,UINT8 ucMaxValue, UINT8 *pszOut, UINT8 ucTimeOut)
{
	INT8    ucRet, ucCurIndex, ucstrLen, ucKeyCnt, ucLastCurIndex;
	UINT8   ucBegRow, ucBedCol;
	char    szBuffer[100], szInputKey[20],szWorkBuff[20];
	UINT32  uiOldTime, uiNewTime;
	INT32   iKeyValue , iRet, iDatalen;

	memset(szBuffer,0,sizeof(szBuffer));
	memset(szInputKey,0,sizeof(szInputKey));
	memset(szWorkBuff,0,sizeof(szWorkBuff));
	if (strlen(pszOut)!=0)
		strcpy(szWorkBuff,pszOut);
    ucKeyCnt = ucCurIndex = 0;
	ucLastCurIndex = 0x7F;   // 无效标记
	if (strlen(pszOut) == 1)
	{
		if (*pszOut <= ucMaxValue/10)
		{
			memset(szWorkBuff,0,sizeof(szWorkBuff));
			szInputKey[0] = *pszOut + 0x30;
			ucKeyCnt = 1;
		}
	}
	if (ucMode&CARRY_IN && *pszOut == 0)
	{
		memset(szWorkBuff,0,sizeof(szWorkBuff));
		szInputKey[0] = *pszOut + 0x30;
		ucKeyCnt = 1;
	}
	while( 1 )
	{
		ucstrLen = strlen(szWorkBuff);
		memset(szBuffer,0,sizeof(szBuffer));
		if (ucstrLen > 0)
		{
			sprintf(szBuffer,"%02d",szWorkBuff[0]);
			for (ucRet = 1; ucRet < ucstrLen; ucRet++)
			{
				sprintf(&szBuffer[ucRet*3-1]," %02d",(int)szWorkBuff[(int)ucRet]);
			}
		}
		lcdClrLine(2,7);
		if (strlen(szBuffer) >= 16)
		{
			lcdDisplay(0,2,DISP_CFONT,"%15.15s",szBuffer);
			lcdDisplay(0,4,DISP_CFONT,szBuffer+15);
		}
		else
		{
			lcdDisplay(0,2,DISP_CFONT,szBuffer);
		} 

		uiOldTime = sysGetTimerCount();
		sgMinLineNo  = 7;
		while(1)
		{
			lcdClrLine(6,7);
			ShowWorkBuf(CARRY_IN,szInputKey,strlen(szInputKey));
			iKeyValue = kbGetKeyMs(100);
			if (iKeyValue != KEY_TIMEOUT )
			{
				break;
			}
			uiNewTime = sysGetTimerCount();
			if (uiOldTime + ucTimeOut*1000 < uiNewTime)
			{
				break;
			}
			
			ucBegRow = (ucCurIndex%5)*3*8;
			ucBedCol = (ucCurIndex/5 + 2)*2*8 -1;
			DrawRect(ucBegRow,ucBedCol,ucBegRow+15,ucBedCol);
			DrawRect(120,62,127,63);
			sysDelayMs(150);
		}

		if( iKeyValue==KEY_CANCEL ||iKeyValue==KEY_TIMEOUT )
		{
			return 1;
		}
		else if( iKeyValue==KEY_ENTER )
		{
			if( ucstrLen<(int)ucMinlen )
			{
				GetRandomBetString(ucMinlen-ucstrLen,ucMaxValue,szWorkBuff);
			}
			sprintf((char *)pszOut, "%s", szWorkBuff);
			break;
		}
		else if( iKeyValue>=KEY0 && iKeyValue<=KEY9 )
		{
			if (ucLastCurIndex != 0x7F)
			{
				memcpy(&szWorkBuff[(int)ucLastCurIndex],&szWorkBuff[(int)ucLastCurIndex+1],ucstrLen-ucCurIndex);
				szWorkBuff[ucstrLen-1] = 0;
				ucLastCurIndex = 0x7F;
			}
			else
			{
				if (ucCurIndex + 1 >= ucMaxlen)
				{
					PubBeepErr();
					continue;
				}
			}

			if (ucKeyCnt >= 2)
			{
				ucKeyCnt = 0;
			}

			szInputKey[(int)ucKeyCnt] = iKeyValue;
			ucKeyCnt++;
			szInputKey[(int)ucKeyCnt] = 0;

			// save key in data
			if( ucKeyCnt >= 2 )
			{
				ucRet = (UINT8)atoi(szInputKey);
				if (ucRet > ucMaxValue)
				{
					ucKeyCnt = 0;
					memset(szInputKey,0,sizeof(szInputKey));
					continue;
				}
				
				iRet = InsertBetToCombination(ucRet,szWorkBuff);
				if (iRet < 0) //插入bets，如果不能插入，则上次输入无效
				{
					ucKeyCnt = 0;
					memset(szInputKey,0,sizeof(szInputKey));
					continue;
				}

				ucKeyCnt = 0;
//				ucCurIndex++;
				ucCurIndex = (UINT8)iRet;
				memset(szInputKey,0,sizeof(szInputKey));
			}
		}
		else if( iKeyValue==KEY_CLEAR )
		{
			memset(szInputKey,0,sizeof(szInputKey));
			memset(szWorkBuff,0,sizeof(szWorkBuff));
			ucCurIndex = ucKeyCnt = 0;
		}
		else if( iKeyValue==KEY_BACKSPACE )	// backspace
		{
			if (ucKeyCnt >= 1)
			{
				szInputKey[(int)ucKeyCnt-1] = 0;
				ucKeyCnt--;
			}
			else
			{
				if (ucCurIndex > 0)
				{
					iDatalen = strlen(szWorkBuff);
					strcpy(&szWorkBuff[(int)ucCurIndex],&szWorkBuff[(int)ucCurIndex+1]);
					szWorkBuff[(int)iDatalen-1] = 0;
//					szWorkBuff[(int)ucCurIndex] = 0;
					ucCurIndex--;
				}
				else
				{
					iDatalen = strlen(szWorkBuff);
					ucCurIndex = 0;
//					szWorkBuff[(int)ucCurIndex] = 0;
					strcpy(&szWorkBuff[(int)ucCurIndex],&szWorkBuff[(int)ucCurIndex+1]);
					szWorkBuff[(int)iDatalen-1] = 0;
				}
				memset(szInputKey,0,sizeof(szInputKey));
				ucKeyCnt  = 0;
				ucLastCurIndex = 0x7F;
				if (szWorkBuff[(int)ucCurIndex] != 0)
				{
					sprintf(szInputKey,"%02d",szWorkBuff[(int)ucCurIndex]);
					ucKeyCnt  = 2;
				}
			}
		}
		else if( iKeyValue==KEY_UP )
		{
			if (ucCurIndex > 0)
			{
				ucCurIndex --;
			}
			else
			{
				PubBeepErr();
			}
			ucLastCurIndex = ucCurIndex;
			sprintf(szInputKey,"%02d",szWorkBuff[(int)ucCurIndex]);
			ucKeyCnt  = 2;
		}
		else if( iKeyValue==KEY_DOWN )
		{
			if (ucCurIndex+1 >= ucMaxlen)
			{
				PubBeepErr();
				continue;
			}
			else
			{
				if (szWorkBuff[(int)ucCurIndex+1] ==0)
				{
					PubBeepErr();
					continue;
				}

				ucCurIndex++;
			}
			ucLastCurIndex = ucCurIndex;
			sprintf(szInputKey,"%02d",szWorkBuff[(int)ucCurIndex]);
			ucKeyCnt  = 0;
		}
	}

	return 0;
} */


////////////////////////////////////// 文件 //////////////////////////////////////

int PubFReadN(int fd, void *psData, int iNeedBytes)
{
	int		iReadBytes, iLeftBytes;
	UINT8	*psTmp;

	PubASSERT( fd>=0 && psData!=NULL && iNeedBytes>=0 );
	psTmp      = (UINT8 *)psData;
	iLeftBytes = iNeedBytes;
	while(iLeftBytes>0)
	{
		iReadBytes = fileRead(fd, (char *)psTmp, iLeftBytes);
		if (iReadBytes<=0)
		{
			PubTRACE1("errno:%d", errno);
			break;
		}
		iLeftBytes -= iReadBytes;
		psTmp	   += iReadBytes;
	}

	return (iNeedBytes-iLeftBytes);
}


int PubFileRead(char *pszFileName, long lOffset, void *psData, int iLen)
{
	int		iRet, fd, iReadBytes;

	PubASSERT( pszFileName!=NULL&& *pszFileName!=0 && iLen>0 );
	fd = fileOpen((char *)pszFileName, O_RDWR);
	if (fd<0)
	{
		fileClose(fd);
		PubTRACE1("open:%s", pszFileName);
		PubTRACE1("errno:%d", errno);
		return ERR_OPENFILE;
	}

	iRet = fileSeek(fd, lOffset, SEEK_SET);
	if (iRet<0)
	{
		fileClose(fd);
		PubTRACE2("seek:%s,%ld", pszFileName, lOffset);
		PubTRACE1("errno:%d", errno);
		return ERR_SEEKFILE;
	}

	iReadBytes = PubFReadN(fd, psData, iLen);
	fileClose(fd);
	if (iReadBytes!=iLen)
	{
		return ERR_READFILE;
	}

	return 0;
}


int PubFileReadEX(uint AppID,uint FileType,UINT8 *pszFileName, long lOffset, void *psData, int iLen)
{
	int		iRet, fd, iReadBytes;
	
	PubASSERT( pszFileName!=NULL&& *pszFileName!=0 && iLen>0 );

	fd = fileOpenExt((char *)pszFileName, O_RDWR,AppID,FileType);
	if (fd<0)
	{
		PubTRACE1("open:%s", pszFileName);
		PubTRACE1("errno:%d", errno);
		return ERR_OPENFILE;
	}
	
	iRet = fileSeek(fd, lOffset, SEEK_SET);
	if (iRet<0)
	{
		PubTRACE2("seek:%s,%ld", pszFileName, lOffset);
		PubTRACE1("errno:%d", errno);
		return ERR_SEEKFILE;
	}
	
	iReadBytes = PubFReadN(fd, psData, iLen);
	fileClose(fd);
	if (iReadBytes!=iLen)
	{
		return ERR_READFILE;
	}
	
	return 0;
}


int PubFWriteN(int fd, void *psData, int iNeedBytes)
{
	int		iWriteBytes, iLeftBytes;
	UINT8   *psTmp;

	PubASSERT( fd>=0 && psData!=NULL && iNeedBytes>=0 );
	psTmp      = (UINT8 *)psData;
	iLeftBytes = iNeedBytes;

	while(iLeftBytes>0)
	{
		iWriteBytes = fileWrite(fd, (char*)psTmp, iLeftBytes);
		if (iWriteBytes<=0)
		{
			PubTRACE2("FWN:%d,%d", iWriteBytes, iLeftBytes);
			PubTRACE1("errno:%d", errno);
			break;
		}
		iLeftBytes -= iWriteBytes;
		psTmp	   += iWriteBytes;
	}

	return (iNeedBytes-iLeftBytes);
}


int PubFileWrite(char *pszFileName, long lOffset, void *psData, int iLen)
{
	int	 iRet, fd, iWriteBytes;

	PubASSERT( pszFileName!=NULL && *pszFileName!=0 && iLen>=0 );
	fd = fileOpen((char *)pszFileName, O_RDWR|O_CREAT);
	if (fd<0)
	{
		PubTRACE1("open:%s", pszFileName);
		PubTRACE1("errno:%d", errno);
		return ERR_OPENFILE;
	}

	iRet = fileSeek(fd, lOffset, SEEK_SET);
	if (iRet<0)
	{
		PubTRACE2("seek:%s,%ld", pszFileName, lOffset);
		PubTRACE1("errno:%d", errno);
		return ERR_SEEKFILE;
	}

	iWriteBytes = PubFWriteN(fd, psData, iLen);
	fileClose(fd);
	if (iWriteBytes!=iLen)
	{
		PubTRACE2("FW:%d,%d", iWriteBytes, iLen);
		return ERR_WRITEFILE;
	}

	return 0;
}

int PubFileWriteEX(uint AppID,uint FileType,UINT8 *pszFileName, long lOffset, void *psData, int iLen)
{
	int	 iRet, fd, iWriteBytes;
	
	PubASSERT( pszFileName!=NULL && *pszFileName!=0 && iLen>=0 );
	fd = fileOpenExt((char *)pszFileName, O_RDWR|O_CREAT,AppID,FileType);
	if (fd<0)
	{
		PubTRACE1("open:%s", pszFileName);
		PubTRACE1("errno:%d", errno);
		return ERR_OPENFILE;
	}
	
	iRet = fileSeek(fd, lOffset, SEEK_SET);
	if (iRet<0)
	{
		PubTRACE2("seek:%s,%ld", pszFileName, lOffset);
		PubTRACE1("errno:%d", errno);
		return ERR_SEEKFILE;
	}
	
	iWriteBytes = PubFWriteN(fd, psData, iLen);
	fileClose(fd);
	if (iWriteBytes!=iLen)
	{
		PubTRACE2("FW:%d,%d", iWriteBytes, iLen);
		return ERR_WRITEFILE;
	}
	
	return 0;
}

////////////////////////////////////// 计算 //////////////////////////////////////

void PubDes(UINT8 ucMode, UINT8 *psData, UINT8 *psKey, UINT8 *psResult)
{
	PubASSERT(ucMode==ONE_ENCRYPT  || ucMode==ONE_DECRYPT ||
		ucMode==TRI_ENCRYPT  || ucMode==TRI_DECRYPT ||
		ucMode==TRI_ENCRYPT3 || ucMode==TRI_DECRYPT3);
	
	switch(ucMode)
	{
	case ONE_ENCRYPT:
		TDEA(psData, psResult, psKey, 8, TDEA_ENCRYPT);
		break;
		
	case ONE_DECRYPT:
		TDEA(psData, psResult, psKey, 8, TDEA_DECRYPT);
		break;
		
	case TRI_ENCRYPT:
		TDEA(psData, psResult, psKey, 16, TDEA_ENCRYPT);
		break;
		
	case TRI_DECRYPT:
		TDEA(psData, psResult, psKey, 16, TDEA_DECRYPT);
		break;
		
	case TRI_ENCRYPT3:
		TDEA(psData, psResult, psKey, 24, TDEA_ENCRYPT);
		break;
		
	case TRI_DECRYPT3:
		TDEA(psData, psResult, psKey, 24, TDEA_DECRYPT);
		break;
	}

	return ;
}


void PubCalcMac(UINT8 ucMode, UINT8 *psKey, UINT8 *psMsg, uint uiLen, UINT8 *psMac)
{
	UINT8   sOutMac[8];
	uint	uiOffset, i;

	PubASSERT(ucMode==MAC_FAST || ucMode==MAC_ANSIX99);
	memset(sOutMac, 0, sizeof(sOutMac));
	uiOffset = 0;

	while(uiLen>uiOffset)
	{
		if (uiLen-uiOffset<=8)
		{
			for (i=0; i<uiLen-uiOffset; i++)
			{
				sOutMac[i] ^= psMsg[uiOffset+i];
			}
			break;
		}
		for (i=0; i<8; i++)
		{
			sOutMac[i] ^= psMsg[uiOffset+i];
		}
		if (ucMode==MAC_ANSIX99)
		{
			PubDes(ONE_ENCRYPT, sOutMac, psKey, sOutMac);
		}
		uiOffset += 8;
	}

	PubDes(ONE_ENCRYPT, sOutMac, psKey, psMac);
}


int PubCheckCHNString(char *psString, int iLen)
{
	while(iLen > 0 && psString != NULL)
	{
		if (*psString & 0x80)
		{
			return TRUE;
		}

		iLen--;
		psString++;
	}
	
	return FALSE;
}

/*********************************************************************************************/
/*********************************************************************************************/
/***************************************************************************************
    For Debug use
***************************************************************************************/


#ifdef _POS_DEBUG
void DispHexMsg(UINT8 *pszTitle, UINT8 *psMsg, uint uiMsgLen, short nTimeOut)
{
	uint	i, iLineNum, iPageNo, iPageNum, iDispLine;
	UINT8	*pszBuff, ucStartLine;
	int    iKeyValue;

	// Format message
	uiMsgLen = min(uiMsgLen, LEN_DBGDATA);
	glszDbgInfo[0] = 0;
	for (pszBuff=glszDbgInfo,i=0; i<uiMsgLen; i+=MAX_CHARS)
	{
		if (uiMsgLen-i<MAX_CHARS)
		{
			pszBuff += DispHexLine(pszBuff, i, psMsg+i, uiMsgLen-i);
		}
		else
		{
			pszBuff += DispHexLine(pszBuff, i, psMsg+i, MAX_CHARS);
		}
	}   // end of for (pszBuff=

	// Display message
	if (pszTitle!=NULL && *pszTitle!=0)
	{
		PubDisplayTitle(TRUE, pszTitle);
		iDispLine   = NUM_MAXLINES-2;
		ucStartLine = 2;
	}
	else
	{
		iDispLine   = NUM_MAXLINES;
		ucStartLine = 0;
	}

	iLineNum = (strlen((char *)glszDbgInfo)+CHARS_PERLINE-1)/CHARS_PERLINE;
	iPageNum = (iLineNum+iDispLine-1)/iDispLine;
	iPageNo  = 0;

	while(1)
	{
		// Light/Close up & down icon
		lcdSetIcon(ICON_UP,   (UINT8)(iPageNo>0		  ? OPENICON : CLOSEICON));
		lcdSetIcon(ICON_DOWN, (UINT8)(iPageNo<iPageNum-1 ? OPENICON : CLOSEICON));

		lcdClrLine(ucStartLine, NUM_MAXLINENO);
		lcdSetIcon(0, ucStartLine, DISP_ASCII, "%.*s", CHARS_PERLINE*iDispLine,
				 &glszDbgInfo[CHARS_PERLINE*iDispLine*iPageNo]);

		lcdFlip();
		iKeyValue = PubWaitKey(nTimeOut);
		lcdSetIcon(ICON_UP,   CLOSEICON);
		lcdSetIcon(ICON_DOWN, CLOSEICON);
		lcdFlip();

		if (iKeyValue==KEY_UP)
		{
			iPageNo = (iPageNo==0) ? iPageNum-1 : iPageNo-1;
		}
		else if (iKeyValue==KEY_DOWN || iKeyValue==KEY_ENTER)
		{
			iPageNo = (iPageNo>=iPageNum-1) ? 0 : iPageNo+1;
		}
		else if (iKeyValue==KEY_CANCEL || iKeyValue==KEY_TIMEOUT)
		{
			return;
		}
	}   // end of while(1
}

// print a line as hexadecimal format
int DispHexLine(UINT8 *pszBuff, uint uiOffset, UINT8 *psMsg, uint uiMsgLen)
{
	uint	i;
	UINT8	*p = pszBuff;

	// Print line information
	pszBuff += sprintf((char *)pszBuff, "%04Xh:", uiOffset);

	for (i=0; i<uiMsgLen; i++)
	{
		pszBuff += sprintf((char *)pszBuff, " %02X", psMsg[i]);
	}
	for (; i<MAX_CHARS; i++)
	{   // append blank spaces, if needed
		pszBuff += sprintf((char *)pszBuff, "   ");
	}

	return (pszBuff-p);
}

// Debug 使用, 功能：显示文件名与行数
void DispAssert(UINT8 *pszFileName, ulong ulLineNo)
{
	UINT8	szFName[30];

	PubDisplayTitle(TRUE, (UINT8 *)"Assert Failure");
	GetBaseName(pszFileName, szFName);
	lcdDisplay(0, 2, DISP_CFONT, "FILE:%.11s", szFName);
	lcdDisplay(0, 4, DISP_CFONT, "LINE:%ld", ulLineNo);
	PubDisplayMsg(6, (UINT8 *)"PRESS ENTER EXIT");
	PubLongBeep();
	lcdFlip();

	while(!PubYesNo(60));
}

// get basename of a full path name
void GetBaseName(UINT8 *pszFullPath, UINT8 *pszBaseName)
{
	UINT8	*pszTmp;
	
	*pszBaseName = 0;
	if (!pszFullPath || !*pszFullPath)
	{
		return;
	}
	
	pszTmp = &pszFullPath[strlen((char *)pszFullPath)-1];
	while( pszTmp>=pszFullPath && *pszTmp!='\\' && *pszTmp!='/' )
	{
		pszTmp--;
	}
	sprintf((char *)pszBaseName, "%s", (char *)(pszTmp+1));
}

#else /* _POS_DEBUG */

#endif /* _POS_DEBUG */

