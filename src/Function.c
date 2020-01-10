
#include "global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void   ShowExchangePack(void);
//void   NacTxd(uint8_t *psTxdData, UINT32 uiDataLen);
static void   ResetPosSystem(void);
/********************** Internal variables declaration *********************/
static MenuItem sgChnFuncMenu[] =
{
	{FALSE, "0.远程下载",		RemoteDownLoad},
	{FALSE, "1.系统参数设置",	SetSystemParam},
	{TRUE,  "2.设置系统时间",	SetSysTime},
	{TRUE,  "3.参数设置",		SetExternParam},
//	{TRUE,  "3.锁定终端",		LockTerm},
	{TRUE,  "4.修改密码",		ChangePassword},
	{FALSE, "5.下载密钥",		DownMasterKey},
	{FALSE, "9.删除记录",		DoClear},
	{TRUE,  "10.显示版本",		DisplayVersion},
	{TRUE,  "11.查阅交易记录",	ViewTranList},
	{TRUE,  "12.查阅指定记录",	ViewSpecList},
	{TRUE,  "13.查阅交易总额",  ViewTotal},
	{FALSE, "14.调整 ADJUST",	TransAdjust},
	{FALSE, "15.LAST TSI TVR",	ViewTVR_TSI},
	{FALSE, "20.打印参数",		PrintParam},
	{TRUE,  "21.重印结账单据",	RePrnSettle},
	{TRUE,  "22.重印上笔交易",	PrnLastTrans},
	{TRUE,  "23.重印指定单据",	RePrnSpecTrans},
	{TRUE,  "24.打印汇总",		PrnTotal},
	{TRUE,  "25.打印流水",		PrnAllList},
	{FALSE, "34.发送通讯报文",	ShowExchangePack},
	{FALSE, "35.打印EMV交易",	PrintEmvErrLog},
	{FALSE, "95.系统参数设置",  SetParamForManager},
	{FALSE, "96.回复出厂设置",  ClearConfig},
	{FALSE, "97.重启系统",      ResetPosSystem},
	{FALSE, "99.远程管理",      RemoteManager},
	{TRUE,  "", NULL},
};

static MenuItem sgEngFuncMenu[] =
{
	{FALSE, "0.REMOTE DOWN",	RemoteDownLoad},
	{FALSE, "1.SETUP PARAM",	SetSystemParam},
	{TRUE,  "2.SET SYS_TIME",	SetSysTime},
	{TRUE,  "3.SET PARAM2",		SetExternParam},
//	{TRUE,  "3.LOCK TERM",		LockTerm},
	{TRUE,  "4.MODIFY PWD",	    ChangePassword},
	{FALSE, "5.DOWNLOAD KEY",	DownMasterKey},
	{FALSE, "9.CLEAR",			DoClear},
	{TRUE,  "10.DISP VERSION",	DisplayVersion},
	{TRUE,  "11.VIEW RECORD",	ViewTranList},
	{TRUE,  "12.TXN REVIEW",	ViewSpecList},
	{TRUE,  "13.VIEW TOTAL",	ViewTotal},
	{FALSE, "14.ADJUST",		TransAdjust},
	{FALSE, "15.LAST TSI TVR",	ViewTVR_TSI},
	{FALSE, "20.PRINT PARA",	PrintParam},
	{TRUE,  "21.REPRN SETTLE",	RePrnSettle},
	{TRUE,  "22.REPRINT LAST",	PrnLastTrans},
	{TRUE,  "23.REPRINT TRACE",	RePrnSpecTrans},
	{TRUE,  "24.PRINT TOTAL",	PrnTotal},
	{TRUE,  "25.PRINT LOG",		PrnAllList},
	{FALSE, "34.SHOW PACKAGE",	ShowExchangePack},
	{FALSE, "35.PRINT ERR LOG",	PrintEmvErrLog},
	{FALSE, "95.SET PARAMETER", SetParamForManager},
	{FALSE, "96.Load Default",  ClearConfig},
	{FALSE, "97.RESET SYSTEM",  ResetPosSystem},
	{FALSE, "99.REMOTE MANAGE", RemoteManager},
	{TRUE,  "", NULL},
};

static MenuItem sgInitMenu[] =
{
	{FALSE, "0.REMOTE DOWN",	RemoteDownLoad},
	{FALSE, "1.SETUP PARAM",	SetSystemParam},
	{TRUE,  "2.SET SYS_TIME",	SetSysTime},
	{TRUE,  "3.SET PARAM2",		SetExternParam},
	{TRUE,  "4.MODIFY PWD",	    ChangePassword},
	{FALSE, "9.CLEAR",			DoClear},
	{TRUE,  "10.DISP VER",		DisplayVersion},
	{FALSE, "95.SET PARAMETER", SetParamForManager},
	{FALSE, "96.Load Default",  ClearConfig},
	{FALSE, "97.RESET SYSTEM",  ResetPosSystem},
	{FALSE, "99.REMOTE MANAGE", RemoteManager},
	{TRUE,  "", NULL},
};

/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

// 执行指定功能号的函数
void FunctionExe(uint8_t bUseInitMenu, int iFuncNo)
{
	int			iCnt, iTempNo;
	MenuItem	*pstMenu;

	pstMenu = bUseInitMenu ? sgInitMenu : sgEngFuncMenu;
	for(iCnt=0; pstMenu[iCnt].szMenuName[0]!=0; iCnt++)
	{
		if( isdigit(pstMenu[iCnt].szMenuName[1]) )
		{
			iTempNo = (int)PubAsc2Long(pstMenu[iCnt].szMenuName, 2, NULL);
		}
		else
		{
			iTempNo = (int)PubAsc2Long(pstMenu[iCnt].szMenuName, 1, NULL);
		}
		if( iTempNo==iFuncNo )
		{
			if( pstMenu[iCnt].pfMenuFunc==NULL )
			{
				break;
			}
			(*pstMenu[iCnt].pfMenuFunc)();
			return;
		}
	}

	if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "功 能 号");
	Display2StringInRect("功能号错误", "FUNC NUMBER ERR");
	PubBeepErr();
	PubWaitKey(3);
}

void FunctionChnMenu(void)
{
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubGetMenu((uint8_t *)"FUNCTION:", sgEngFuncMenu, MENU_USESNO|MENU_CFONT, USER_OPER_TIMEOUT);
	else
		PubGetMenu((uint8_t *)"功能号:", sgChnFuncMenu, MENU_USESNO|MENU_CFONT, USER_OPER_TIMEOUT);
}

void FunctionEngMenu(void)
{
	PubGetMenu((uint8_t *)"FUNCTION:", sgEngFuncMenu, MENU_USESNO|MENU_CFONT, USER_OPER_TIMEOUT);
}

void FunctionInit(void)
{
	int		iFuncNo;

	iFuncNo = FunctionInput();
	if( iFuncNo>=0 )
	{
		FunctionExe(TRUE, iFuncNo);
	}
}

// 输入功能号码
INT32 FunctionInput(void)
{
	uint8_t szFuncNo[2+1];
	
	if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "功 能 号");
	else
		PubDisplayTitle(TRUE, "FUNTION");
	lcdClrLine(2,7);
	PubDispString("FUNCTION ?", 2|DISP_LINE_LEFT);
	memset(szFuncNo, 0, sizeof(szFuncNo));
	if( PubGetString(NUM_IN, 1, 2, szFuncNo, USER_OPER_TIMEOUT)!=0 )
	{
		return -1;
	}
	
	return  atoi((char *)szFuncNo);
}


// 发送通讯报文到COM1
void ShowExchangePack(void)
{
	int iKey;
	while(1)
	{
		lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDisplayTitle(TRUE, "Get Comm Data");
			lcdDisplay(0, 2, DISP_CFONT,"1. Send data");
			lcdDisplay(0, 4, DISP_CFONT,"2. Recv data");
		}
		else
		{
			PubDisplayTitle(TRUE, "发送通讯报文");
			lcdDisplay(0, 2, DISP_CFONT,"1.EDC发送的数据");
			lcdDisplay(0, 4, DISP_CFONT,"2.EDC接收的数据");
		}

		iKey = PubWaitKey(30);
		switch (iKey)
		{
		case KEY1:
			lcdClrLine(2,7);
			Display2StringInRect("发送中...", "SENDING...");
			NacTxd((uchar*)gstSendData.sContent+2, gstSendData.uiLength-2);
			Display2StringInRect("发送完成", "SEND OK");
			PubWaitKey(2);
			break;
		case KEY2:
			lcdClrLine(2,7);
			Display2StringInRect("发送中...", "SENDING...");
			NacTxd((uchar*)gstRecvData.sContent, gstRecvData.uiLength);
			Display2StringInRect("发送完成", "SEND OK");
			PubWaitKey(2);
			break;
		case KEY_TIMEOUT:
		case KEY_CANCEL:
			return;
			break;
		default:
			break;
		}
	}
}

void NacTxd(uint8_t *psTxdData, UINT32 uiDataLen)
{
	uint8_t	*psTemp, sWorkBuf[LEN_MAX_COMM_DATA*50+10];
	
	if( uiDataLen>LEN_MAX_COMM_DATA*50 )
	{
		Display2StringInRect("数据长度非法", "INVALID PACK");
		PubWaitKey(2);
		return;
	}

	sWorkBuf[0] = STX;
	sWorkBuf[1] = (uiDataLen/1000)<<4    | (uiDataLen/100)%10;	// convert to BCD
	sWorkBuf[2] = ((uiDataLen/10)%10)<<4 | uiDataLen%10;
	memcpy(&sWorkBuf[3], psTxdData, uiDataLen);
	sWorkBuf[3+uiDataLen]   = ETX;
	sWorkBuf[3+uiDataLen+1] = PubCalcLRC((char*)psTxdData, uiDataLen, (uint8_t)(sWorkBuf[1] ^ sWorkBuf[2] ^ ETX));
	uiDataLen += 5; 

	portClose(PORT_COM1);
	portOpen(PORT_COM1, (void *)"9600,8,n,1");
	psTemp = sWorkBuf;
	while( uiDataLen-->0 )
	{
		if( portSends(PORT_COM1, psTemp,1)!=0 )
		{
			psTemp++;
			break;
		}
		psTemp++;
	}
	portClose(PORT_COM1);
}

int    NacRxd(uint8_t *psRxdData, UINT32 *uiDataLen)
{
	int   iRet,iFlag;
	UINT32	uiReadCnt, uiTemp;
	UINT32  iOldTimeMs,iNewTimeMs;
	uint8_t    *psRecvData = psRxdData;
	uint8_t   ucLrcValue;
	
	uiReadCnt = iFlag = 0;
	uiTemp = 5000;
	iOldTimeMs = sysGetTimerCount();
	while( uiReadCnt< LEN_MAX_COMM_DATA )
	{
		if (iFlag==0)
		{
			iNewTimeMs= sysGetTimerCount();
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(30 *1000) )	// 检查定时器
			{
				if( uiReadCnt>0 )	// 已经读取到数据
				{
					break;
				}
				
				return ERR_NO_DISP;
			}
			
			if( PubChkKeyInput() )
			{
				if (kbGetKey() == KEY_CANCEL)
				{
					return ERR_USERCANCEL;
					
				}
			}
			
			iRet = portCheckRecvBuf(PORT_COM1);
			if (iRet != PORT_RET_NOTEMPTY)
			{
				if (iRet != PORT_RET_OK )
				{
					return ERR_NO_DISP;
				}
				
				continue;
			}
			else
			{
				iFlag = 1;
			}
		}
		
		iRet = portRecv(PORT_COM1, psRecvData, uiTemp);
		if( iRet==0x00 )
		{	// 接收成功,继续
			uiTemp = 80;
			psRecvData++;
			uiReadCnt++;
		}
		else if( iRet==0xFF )
		{
			if( uiReadCnt>0 )
			{
				break;
			}
			
			return ERR_NO_DISP;
		}
		else
		{	// 非超时错误,退出
			return ERR_NO_DISP;
		}
	}   // end of while( uiReadCnt<uiExpLen
	
	if (psRxdData[0] != STX || psRxdData[uiReadCnt-2] != ETX)
	{
		return ERR_NO_DISP;
	}
	
	ucLrcValue = PubCalcLRC((char *)psRxdData+1,uiReadCnt-2,0);
	if (psRxdData[uiReadCnt-1] != ucLrcValue)
	{
		return ERR_NO_DISP;
	}
	
	memcpy(psRxdData,psRxdData+3,uiReadCnt-5);
	psRxdData[uiReadCnt-5] = 0;
	
	*uiDataLen = uiReadCnt-5;
	
	return 0;
}

void   ResetPosSystem(void)
{
	int iKey;
	
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayTitle(TRUE, "RESET SYSTEM");
		PubDispString("  RESET SYSTEM ?",3|DISP_LINE_CENTER);
		PubDispString("[CANCEL] [ENTER]",6|DISP_LINE_CENTER);
	}
	else
	{
		PubDisplayTitle(TRUE, "系统重启");
		PubDispString("是否重启系统?",3|DISP_LINE_CENTER);
		PubDispString("[退出]   [确定]",6|DISP_LINE_CENTER);
	}
	iKey = PubWaitKey(USER_OPER_TIMEOUT);
	if (iKey == KEY_ENTER)
	{
		sysReset();
	}
	
	return ;
}

// end of file
