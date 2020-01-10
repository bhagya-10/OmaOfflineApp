
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
	{FALSE, "0.Զ������",		RemoteDownLoad},
	{FALSE, "1.ϵͳ��������",	SetSystemParam},
	{TRUE,  "2.����ϵͳʱ��",	SetSysTime},
	{TRUE,  "3.��������",		SetExternParam},
//	{TRUE,  "3.�����ն�",		LockTerm},
	{TRUE,  "4.�޸�����",		ChangePassword},
	{FALSE, "5.������Կ",		DownMasterKey},
	{FALSE, "9.ɾ����¼",		DoClear},
	{TRUE,  "10.��ʾ�汾",		DisplayVersion},
	{TRUE,  "11.���Ľ��׼�¼",	ViewTranList},
	{TRUE,  "12.����ָ����¼",	ViewSpecList},
	{TRUE,  "13.���Ľ����ܶ�",  ViewTotal},
	{FALSE, "14.���� ADJUST",	TransAdjust},
	{FALSE, "15.LAST TSI TVR",	ViewTVR_TSI},
	{FALSE, "20.��ӡ����",		PrintParam},
	{TRUE,  "21.��ӡ���˵���",	RePrnSettle},
	{TRUE,  "22.��ӡ�ϱʽ���",	PrnLastTrans},
	{TRUE,  "23.��ӡָ������",	RePrnSpecTrans},
	{TRUE,  "24.��ӡ����",		PrnTotal},
	{TRUE,  "25.��ӡ��ˮ",		PrnAllList},
	{FALSE, "34.����ͨѶ����",	ShowExchangePack},
	{FALSE, "35.��ӡEMV����",	PrintEmvErrLog},
	{FALSE, "95.ϵͳ��������",  SetParamForManager},
	{FALSE, "96.�ظ���������",  ClearConfig},
	{FALSE, "97.����ϵͳ",      ResetPosSystem},
	{FALSE, "99.Զ�̹���",      RemoteManager},
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

// ִ��ָ�����ܺŵĺ���
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
		PubDisplayTitle(TRUE, "�� �� ��");
	Display2StringInRect("���ܺŴ���", "FUNC NUMBER ERR");
	PubBeepErr();
	PubWaitKey(3);
}

void FunctionChnMenu(void)
{
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubGetMenu((uint8_t *)"FUNCTION:", sgEngFuncMenu, MENU_USESNO|MENU_CFONT, USER_OPER_TIMEOUT);
	else
		PubGetMenu((uint8_t *)"���ܺ�:", sgChnFuncMenu, MENU_USESNO|MENU_CFONT, USER_OPER_TIMEOUT);
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

// ���빦�ܺ���
INT32 FunctionInput(void)
{
	uint8_t szFuncNo[2+1];
	
	if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "�� �� ��");
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


// ����ͨѶ���ĵ�COM1
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
			PubDisplayTitle(TRUE, "����ͨѶ����");
			lcdDisplay(0, 2, DISP_CFONT,"1.EDC���͵�����");
			lcdDisplay(0, 4, DISP_CFONT,"2.EDC���յ�����");
		}

		iKey = PubWaitKey(30);
		switch (iKey)
		{
		case KEY1:
			lcdClrLine(2,7);
			Display2StringInRect("������...", "SENDING...");
			NacTxd((uchar*)gstSendData.sContent+2, gstSendData.uiLength-2);
			Display2StringInRect("�������", "SEND OK");
			PubWaitKey(2);
			break;
		case KEY2:
			lcdClrLine(2,7);
			Display2StringInRect("������...", "SENDING...");
			NacTxd((uchar*)gstRecvData.sContent, gstRecvData.uiLength);
			Display2StringInRect("�������", "SEND OK");
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
		Display2StringInRect("���ݳ��ȷǷ�", "INVALID PACK");
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
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(30 *1000) )	// ��鶨ʱ��
			{
				if( uiReadCnt>0 )	// �Ѿ���ȡ������
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
		{	// ���ճɹ�,����
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
		{	// �ǳ�ʱ����,�˳�
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
		PubDisplayTitle(TRUE, "ϵͳ����");
		PubDispString("�Ƿ�����ϵͳ?",3|DISP_LINE_CENTER);
		PubDispString("[�˳�]   [ȷ��]",6|DISP_LINE_CENTER);
	}
	iKey = PubWaitKey(USER_OPER_TIMEOUT);
	if (iKey == KEY_ENTER)
	{
		sysReset();
	}
	
	return ;
}

// end of file
