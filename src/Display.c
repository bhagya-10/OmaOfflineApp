
#include "global.h"
#include "seos.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
HOST_ERR_MSG	glHostErrMsg[] =
{
	{"00", "TXN. ACCEPTED",         "�� �� �� ��"},
	{"01", "Call your bank",        "�� ѯ �� �� ��"},
	{"02", "Refer to Issuer",       "�� ѯ �� ��"},
	{"03", "Invalid Merchant",      "�� Ч �� ��"},
	{"04", "Pick Up Card",          "û �� �� ��"},
	{"05", "Do Not Honour",         "�� �� ��"},
	{"07", "Pick Up Card",          "û �� �� ��"},
	{"08", "Check Signature",       "�� �� ֤ ǩ ��"},
	{"12", "call issure bank",      "�� ѯ �� �� ��"},
	{"13", "Invalid Amount",        "�� �� �� Ч"},
	{"14", "Invalid Card No",       "�� �� �� ��"},
	{"19", "Re-enter TRANS.",       "�� �� �� �� ��"},
	{"21", "No Transaction",        "�� Ч �� ��"},
	{"25", "Invalid Terminal",      "��Ч�ն˺�"},
	{"30", "Format Error",          "�� ʽ �� ��"},
	{"31", "Bank ID Not Found",     "�� ʽ �� ��"},
	{"41", "Card Reported Lost",    "��������,û��"},
	{"43", "Stolen Card",           "���Կ�,û��"},
	{"44", "PIN Change Require",    "���Կ�,û��"},
	{"45", "Card Not Activated",    "For Use"},
	{"51", "Insufficient Fund",     "�� �� ��"},
	{"52", "No Checking Count",     "��Ч���ѹ�"},
	{"53", "No Savings Accmount",   "�� �� �� ��"},
	{"54", "Expired Card",          "��Ч���ѹ�"},
	{"55", "Invalid PIN",           "�� �� �� ��"},
	{"56", "Invalid Card",          "�� Ч ��"},
	{"58", "Invalid TXN",           "�� �� �� ��"},
	{"61", "Terminal Over Limit",   "�� ѯ �� �� ��"},
	{"63", "Security Violation",    "�� ѯ �� �� ��"},
	{"75", "PIN Tries Exceed",      "�� ѯ �� �� ��"},
	{"76", "Invalid Desc code",     "�� ѯ �� �� ��"},
	{"77", "Reconcile Error",       "�� �� �� ��"},
	{"78", "Invalid Trace No",      "�� �� �� ��"},
	{"79", "Bad Batch Number",      "�� �� �� ��"},
	{"80", "Invalid Batch No",      "�� �� �� ��"},
	{"85", "Batch Not Found",       "�� ѯ �� �� ��"},
	{"88", "Call Issuer",           "�� ѯ �� �� ��"},
	{"89", "Bad Terminal ID",       "�� ѯ �� �� ��"},
	{"91", "Issuer Error",          "�� ѯ �� �� ��"},
	{"93", "Declined",              "�� �� ȡ ��"},
	{"94", "Duplicate Trace",       "�� ѯ �� �� ��"},
	{"95", "Total Mismatch",        "�� �� �� ��"},
	{"96", "System Error",          "�� Ч �� Ϣ"},
	{"99", "Declined",              "�� �� ȡ ��"},
	{"A1", "Invalid Region",        "�� ѯ �� �� ��"},
	{"A2", "Invalid Coordinate",    "�� ѯ �� �� ��"},
	{"A3", "Invalid Area Code",     "�� ѯ �� �� ��"},
	{"A4", "Invalid Unit Code",     "�� ѯ �� �� ��"},
	{"A5", "Invalid Territory",     "�� ѯ �� �� ��"},
	{"A6", "Invalid Agent Code",    "�� ѯ �� �� ��"},
	{"N1", "Not DCC Eligible",      "DCC �� Ч"},
	{"Q1", "CARD AUTH FAIL",        "CARD AUTH FAIL"},
	{"Y1", "OFFLINE APPROVAL",      "������Ȩ"},
	{"Z1", "OFFLINE DECLINE",       "���߾ܾ�����"},
	{"Y2", "APPROVED",              "APPROVED"},
	{"Z2", "DECLINED",              "DECLINED"},
	{"Y3", "GO ONLINE FAIL",        "GO ONLINE FAIL"},
	{"Z3", "GO ONLINE FAIL",        "GO ONLINE FAIL"},
	{"NA", "NO UPDATE",             "�� Ч �� Ϣ"},
	{"P0", "ERROR SERIAL NO.",      "�� Ч �� Ϣ"},
	{"XY", "Duplicate Trans",       "�� Ч �� Ϣ"},
	{"XX", "NO DCC SGD REQ",        "�� Ч �� Ϣ"},
	{"**", "NO RESPONSE CODE",      "�� �� Ӧ ��"},
	{"\0\0","PLS CALL BANK",        "�� ѯ �� �� ��"},
};

// �ն˴�����Ϣ
TERM_ERR_MSG	glTermErrMsg[] = 
{
	// 	{ERR_COMM_MODEM_OCCUPIED,  "PHONE OCCUPIED", "���õ绰ռ��"},
	// 	{ERR_COMM_MODEM_NO_LINE,   "LINE READY ?",   "��Ӻõ绰��"},
	// 	{ERR_COMM_MODEM_LINE,      "LINE BREAK",     "ͨѶ����,����"},
	// 	{ERR_COMM_MODEM_NO_ACK,    "NO ACK",         "���������Ӧ"},
	// 	{ERR_COMM_MODEM_LINE_BUSY, "LINE BUSY",      "�� �� �� ͨ æ"},
	// 	{ERR_NO_TELNO,             "NO TEL NO",      "�绰����Ϊ��"},
	{ERR_USERCANCEL,           "USER CANCELED",  "�û�ȡ��"},
	{ERR_TRAN_FAIL,            "PROCESS FAILED", "����ʧ��"},
	{0, "", ""},
};
/********************** Internal functions declaration *********************/
/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/
void Display2StringHigh(void *pszString1, void *pszString2)
{
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "%-16.16s", pszString2);
	}
	else
	{
		lcdCls();
		lcdDisplay(0, 0, DISP_CFONT|DISP_MEDIACY|DISP_INVLINE, "%-16.16s", pszString1);
		lcdDisplay(0, 2, DISP_CFONT|DISP_INVLINE, "%-16.16s", pszString2);
	}
	lcdFlip();
}

void   Display2Strings(char *pszString1, char *pszString2)
{
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayMsg(4, (uint8_t *)pszString2);
	}
	else
	{
		PubDisplayMsg(3, (uint8_t *)pszString1);
		PubDisplayMsg(5, (uint8_t *)pszString2);
	}
	DrawRect(0, 17, 127, 63);
	lcdFlip();
}

void   Display2StringInRect(char *pszString1,  char *pszString2)
{
	Display2Strings(pszString1, pszString2);
	DrawRect(0, 17, 127, 63);
	lcdFlip();
}


void   DrawRect(UINT32 uiBegRow, UINT32 uiBegCol, UINT32 uiEndRow, UINT32 uiEndCol)
{
	UINT32	uiPointx, uiPointy;
	int     iScreenwidth, iScreenheight;
	
	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
		return;
	}

	if (uiBegRow>=127)
	{
		uiEndRow = 127;
	}
	if (uiEndRow>=127)
	{
		uiEndRow = 127;
	}
	if (uiEndRow<=uiBegRow)
	{
		uiEndRow = uiBegRow;
	}

	if (uiBegCol>=63)
	{
		uiBegCol = 63;
	}
	if (uiEndCol>=63)
	{
		uiEndCol = 63;
	}
	if (uiEndCol<=uiBegCol)
	{
		uiEndCol = uiBegCol;
	}

	for (uiPointx=uiBegRow; uiPointx<=uiEndRow;uiPointx++)
	{
		lcdStipple(uiPointx, uiBegCol, ON);
		lcdStipple(uiPointx, uiEndCol, ON);
	}

	for (uiPointy=uiBegCol; uiPointy<=uiEndCol; uiPointy++)
	{
		lcdStipple(uiBegRow, uiPointy, ON);
		lcdStipple(uiEndRow, uiPointy, ON);
	}
}


// ��ʾ����ʱ�û�����
int   DispLayIdleUI(int IfRedraw)
{
	static	char	szOldTime[5+1] = {"00000"};
	char	szCurTime[16+1];
	int     iRet;
	static  int      imagOpenflag = 0;
	
	GetCurTime(szCurTime);
	if(IfRedraw || memcmp(szOldTime, &szCurTime[11], 4)!=0 )	// ÿ�β��������ÿ10���ӶԴ�ͷ��λһ��
	{
		if (imagOpenflag == 0)
		{	
			magClose();
			magOpen();
			imagOpenflag = 1; 
		}

		magReset();

		// ��ʾ�źţ���������ϵ縴λһ��
		if (gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType == CT_CDMA || 
			gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType == CT_GPRS)
		{
			iRet = DispWirelessSignal();
			if (iRet == -ETIMEDOUT || iRet == -WNET_RECV_TIMEOUT_ERR || 
				iRet == -WNET_NO_FUN_ERR || iRet == -WNET_MOD_RESP_ERR)
			{
				CommResetModule(&gstSysParam.stAcqList[0].stTxnCommCfg);
			}
		}
		else
		{
//			CommCheckModemModule(&gstSysParam.stAcqList[0].stTxnCommCfg);
		}
	}

	if( IfRedraw || memcmp(szOldTime, &szCurTime[11], 5)!=0 ) // 
	{
		sprintf((char *)szOldTime, "%.5s", &szCurTime[11]);
		PubDisplayTitle(TRUE, szCurTime);
		DisplaySwipeCard();
		lcdFlip();
		IfRedraw = FALSE;
	}

	return IfRedraw;
}


void DisplayVersion(void)
{
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "VERSION");
	else
		PubDisplayTitle(TRUE, "�� �� ��");

	lcdDisplay(0, 2, DISP_CFONT, "%.16s", EDC_VER_STRING);
	lcdDisplay(0, 4, DISP_CFONT, "FreeROM:%-8ld", fileFreeSpace());	
	PubWaitKey(USER_OPER_TIMEOUT);
}


void DisplayTransName(void)
{
	uint8_t	szTitle[16+1];
	
	if( gstTransInfo.stTranLog.ucTranType==SALE_OR_AUTH )
	{
		GetCurTime((char*)szTitle);
		lcdDisplay(0, 0, DISP_CFONT|DISP_MEDIACY|DISP_INVLINE, "%-16.16s", szTitle);
	}
	else
	{
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{	
			lcdDisplay(0, 0, DISP_CFONT|DISP_MEDIACY|DISP_INVLINE, "%-16.16s", glTranConfig[gstTransInfo.stTranLog.ucTranType].szLabel);
		}
		else
		{
			lcdDisplay(0, 0, DISP_CFONT|DISP_MEDIACY|DISP_INVLINE, "%-16.16s", glTranConfig_CH[gstTransInfo.stTranLog.ucTranType].szLabel);
		}
	}
}


// ��ָ������ʾ��ʽ���Ľ����Ϣ
void DisplayAmount(uint8_t ucLine, uint8_t *pszAmount)
{
	uint8_t	ucFlag, szDispAmt[30];
	UINT32  uiDispMode;
	
	ucFlag = 0;
	memset(szDispAmt,0,sizeof(szDispAmt));
	if( gstTransInfo.stTranLog.ucTranType==VOID || gstTransInfo.stTranLog.ucTranType==REFUND ||
		*pszAmount=='D' )
	{
		ucFlag |= GA_NEGATIVE;
		if( *pszAmount=='D' )
		{
			*pszAmount = '0';
		}
	}
	
	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, pszAmount, 2, szDispAmt, ucFlag);
	if (strlen((char *)szDispAmt)>16)
	{
		uiDispMode = DISP_ASCII;
		szDispAmt[21] = 0;
	}
	else
	{
		uiDispMode = DISP_CFONT;
		szDispAmt[16] = 0;
	}

	lcdDisplay(0, ucLine, uiDispMode, (char *)szDispAmt);
}


void DisplayAdjustTitle(void)
{
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "ADJUST          ");
	else
		PubDisplayTitle(TRUE, "����      ADJUST");
}


// ��ʾ���׻�����Ϣ(gstTransTotal)
INT32 DisplayTransTotal(uint8_t bShowVoidTrans)
{
	uint8_t	szBuff[25];
	
	lcdCls();
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szBuff, "SALE%12d",    gstTransTotal.uiSaleCnt);
	else
		sprintf((char *)szBuff, "����SALE%8d", gstTransTotal.uiSaleCnt);
	PubDispString(szBuff, 0|DISP_LINE_LEFT);
	DisplayAmount(2, gstTransTotal.szSaleAmt);
	
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szBuff, "REFUND%10d",    gstTransTotal.uiRefundCnt);
	else
		sprintf((char *)szBuff, "�˿�REFUND%6d", gstTransTotal.uiRefundCnt);
	PubDispString(szBuff, 4|DISP_LINE_LEFT);
	
	sprintf((char *)szBuff, "%.12s", gstTransTotal.szRefundAmt);
	szBuff[0] = 'D';
	DisplayAmount(6, szBuff);
	
	if( !PubYesNo(USER_OPER_TIMEOUT) )
	{
		return ERR_USERCANCEL;
	}
	if( !bShowVoidTrans )
	{
		return 0;
	}
	
	lcdCls();
	sprintf((char *)szBuff, "VOIDED SALE%5d", gstTransTotal.uiVoidSaleCnt);
	PubDispString(szBuff, 0|DISP_LINE_LEFT);
	
	sprintf((char *)szBuff, "%.12s", gstTransTotal.szVoidSaleAmt);
	szBuff[0] = 'D';
	DisplayAmount(2, szBuff);
	
	sprintf((char *)szBuff, "VOIDED REFD%5d", gstTransTotal.uiVoidRefundCnt);
	PubDispString(szBuff, 4|DISP_LINE_LEFT);
	DisplayAmount(6, gstTransTotal.szVoidRefundAmt);
	
	if( PubYesNo(USER_OPER_TIMEOUT) )
	{
		return 0;
	}
	
	return ERR_USERCANCEL;
}

// ��ʾ������־
void DisplayTranLog(UINT32 uiActRecNo, void *pTranLog)
{
	uint8_t		szBuff[25], szEngLabel[16+1], szTotalAmt[12+1];
	ST_TRAN_LOG	*pstLog;
	
	lcdCls();
	pstLog = (ST_TRAN_LOG *)pTranLog;
	GetStateText(pstLog->uiStatus, szBuff);
	if( szBuff[0]!=0 )
	{
		GetEngTranLabel(glTranConfig[pstLog->ucTranType].szLabel, szEngLabel);
		PubDispString(szEngLabel, DISP_LINE_RIGHT|0);
		lcdDisplay(0,1, DISP_ASCII, "%-10.10s", szBuff);
	}
	else
	{
		PubDispString(glTranConfig[pstLog->ucTranType].szLabel, DISP_LINE_CENTER|0);
	}
	
	sprintf((char *)szBuff, "%03d/%03d  TRACE:%06lu", uiActRecNo, GetTranLogNum(ACQ_ALL),
		pstLog->ulInvoiceNo);
	
	lcdDisplay(0, 2, DISP_ASCII, "%.21s", szBuff);
	
	if( ChkIfDispMaskPan2() )
	{
		//		MaskPan(pstLog->szPan, szBuff);
		memcpy(szBuff,pstLog->szPan,sizeof(pstLog->szPan));
		lcdDisplay(0, 3, DISP_ASCII, "%.21s", szBuff);
	}
	else
	{
		lcdDisplay(0, 3, DISP_ASCII, "%.21s", pstLog->szPan);
	}
	
	PubAscAdd((char *)pstLog->szAmount, (char *)pstLog->szTipAmount, 12, (char *)szTotalAmt);
	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, szTotalAmt, 2, szBuff, GetTranAmountInfo(pstLog));
	lcdDisplay(0, 4, DISP_ASCII, "%.21s", szBuff);
	lcdDisplay(0, 5, DISP_ASCII, "APPR CODE :%6.6s", pstLog->szAuthCode);
	ChangeToEngTime((char *)pstLog->szDateTime, (char *)szBuff);
	lcdDisplay(0, 6, DISP_ASCII, "%.21s", szBuff);
	lcdDisplay(0, 7, DISP_ASCII, "RRN     :%12.12s", pstLog->szRRN);
}


void DisplayResult(INT32 iErrCode)
{
	INT32		iCnt;

	if( iErrCode==ERR_NO_DISP )
	{
		return;
	}

	if( iErrCode!=0 )
	{
		lcdCls();
		DisplayTransName();
	}

	switch( iErrCode )
	{
	case 0:
		switch( gstTransInfo.stTranLog.ucTranType )
		{
		case UPLOAD:
			sysBeef(3, 60);
			break;

		case OFF_SALE:
		case VOID:
			lcdCls();
			DisplayTransName();
			DispAccepted();
			PubBeepOk();
			PubWaitKey(gstSysParam.stEdcInfo.ulAcceptTimeout);
			break;

		case LOGON:
			break;

		default:
			lcdCls();
			DisplayTransName();
			if( gstTransInfo.stTranLog.szAuthCode[0]==0 ||
				memcmp(gstTransInfo.stTranLog.szAuthCode, "       ", 6)==0 )
			{
				DispAccepted();
			}
			else
			{
				if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				{
					lcdDisplay(0, 3, DISP_CFONT, "  APPV CODE");
					lcdDisplay(0, 5, DISP_CFONT, "  %.6s", gstTransInfo.stTranLog.szAuthCode);
				}
				else
				{
					lcdDisplay(0, 2, DISP_CFONT, "  ��Ȩ����");
					lcdDisplay(0, 4, DISP_CFONT, "  APPV CODE");
					lcdDisplay(0, 6, DISP_CFONT, "  %.6s", gstTransInfo.stTranLog.szAuthCode);
				}
			}
			PubBeepOk();
			PubWaitKey(3);
		}
		break;

	case ERR_HOST_REJ:
		for(iCnt=0; glHostErrMsg[iCnt].szRspCode[0]!=0; iCnt++)
		{
			if( memcmp(gstTransInfo.stTranLog.szRspCode, glHostErrMsg[iCnt].szRspCode, 2)==0 )
			{
				break;
			}
		}


		Display2Strings((char *)glHostErrMsg[iCnt].szChnMsg, (char *)glHostErrMsg[iCnt].szEngMsg);
		lcdDisplay(112, 7, DISP_ASCII, "%.2s", gstTransInfo.stTranLog.szRspCode);
		PubBeepErr();
		PubWaitKey(5);
		break;

	default:
		for(iCnt=0; glTermErrMsg[iCnt].iErrCode!=0; iCnt++)
		{
			if( glTermErrMsg[iCnt].iErrCode==iErrCode )
			{
				Display2Strings((char *)glTermErrMsg[iCnt].szChnMsg, (char *)glTermErrMsg[iCnt].szEngMsg);
				break;
			}
		}
		if( glTermErrMsg[iCnt].iErrCode==0 )
		{
			Display2Strings("ϵͳ����", "SYSTEM ERROR");
			lcdDisplay(80, 7, DISP_ASCII, "%04x", iErrCode);
		}
		PubBeepErr();
		PubWaitKey(5);
	    break;
	}
}


// ��ʾ�γ�IC��
void PromptRemoveICC(void)
{
	if( !ChkIfEmvEnable() )
	{
		return;
	}
	
	iccPowerDown(ICC_USER);
	if( iccDetect(ICC_USER)==ICC_CARDOUT )	// ���IC���Ѱγ���ֱ�ӷ��ء�
	{
		return;
	}
	
	// ��ʾ���ȴ�IC���γ�
	DisplayTransName();
	Display2StringInRect("��γ�IC��", "PLS REMOVE CARD");
	lcdFlip();
	while( iccDetect(ICC_USER)!=ICC_CARDOUT )
	{
		sysBeep();
		sysDelayMs(500);
	}
}


// ��ʾˢ��/�忨����
void DisplaySwipeCard(void)
{
	if( ChkIfEmvEnable() )
	{
		Display2Strings("��ˢ����忨", "SWIPE/INSERT ...");
	}
	else
	{
		DisplaySwipeOnly();
	}
}

void DisplaySwipeOnly(void)
{
	Display2Strings("��  ˢ  ��", "PLS SWIPE CARD");
}

void DisplayInsertCard(void)
{
	Display2Strings("��  ��  ��", "PLS INSERT CARD");
}

void   DisplayKeyInOnly(void)
{
	Display2Strings("�����뿨��", "ENTER ACCOUNT NO");
}

// ��ʾFallback��ʾ����
void PromptFallBack(void)
{
	uint	iCnt;
	
	Display2StringInRect("��������,��ˢ��", "PLS SWIPE CARD");
	lcdFlip();
	iCnt = 0;
	while( iccDetect(0)==ICC_SUCCESS )
	{
		iCnt++;
		if( iCnt>4 )
		{
			sysBeep();
			lcdDisplay(16*5, 7, DISP_ASCII, "%8s", iCnt%2 ? "PULL OUT" : "");
			lcdFlip();
		}
		sysDelayMs(500);
	}
}


void DispFallBackSwipeCard(void)
{
	Display2Strings("��  ˢ  ��", "PLS SWIPE CARD");
	lcdDisplay(0, 7, DISP_ASCII|DISP_MEDIACY|DISP_REVERSE, "FALL BACK");
}


void DispGetMagInfoErr(void)
{
	Display2StringInRect("ˢ������", "READ CARD ERR.");
}


void DisplayBlockFunc(void)
{
    Display2StringInRect("��֧�ָý���", "TRANS NOT ALLOW");
    DrawRect(2, 0, 8, 21);
	PubBeepErr();
    PubWaitKey(3);
}

void DisplayProcessing(void)
{
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDispString(" PROCESSING....", 4|DISP_LINE_LEFT);
	}
	else
	{
		PubDispString(" �� �� ��",       3|DISP_LINE_LEFT);
		PubDispString(" PROCESSING....", 5|DISP_LINE_LEFT);
	}
	DrawRect(0, 17, 127, 63);
	lcdFlip();
}

void DisplayDialing(void)
{
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDispString(" DIALING....", 4|DISP_LINE_LEFT);
	}
	else
	{
		PubDispString(" �� �� ��",     3|DISP_LINE_LEFT);
		PubDispString(" DIALING....", 5|DISP_LINE_LEFT);
	}
	DrawRect(0, 17, 127, 63);
	lcdFlip();
}

void DisplaySending(void)
{
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDispString(" SENDING....", 4|DISP_LINE_LEFT);
	}
	else
	{
		PubDispString(" �� �� ��",    3|DISP_LINE_LEFT);
		PubDispString(" SENDING....", 5|DISP_LINE_LEFT);
	}
	DrawRect(0, 17, 127, 63);
	lcdFlip();
}

void DisplayReceiving(void)
{
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDispString(" RECEIVING...", 4|DISP_LINE_LEFT);
	}
	else
	{
		PubDispString(" �� �� ��",     3|DISP_LINE_LEFT);
		PubDispString(" RECEIVING...", 5|DISP_LINE_LEFT);
	}
	DrawRect(0, 17, 127, 63);
	lcdFlip();
}


void DispAccepted(void)
{
	lcdClrLine(2, 7);
	Display2StringInRect("�������", "TXN ACCEPTED");
}


void DisplayClearOk(void)
{
	DisplayOperOk("������     ", "CLEARED     ");
}

// ��ʾ����״̬��Ϣ
void DisplayTransLogState(UINT32 uiStatus)
{
	if( uiStatus & TS_VOID )
	{
		Display2StringInRect("�ѳ���", "ALREADY VOID");
	}
	else if( uiStatus & TS_ADJ )
	{
		Display2StringInRect("�ѵ���", "ALREADY ADJUST");
	}
	else if( uiStatus & TS_NOSEND )
	{
		Display2StringInRect("δ����", "NOT SEND");
	}
}

void DispOkSub(uint8_t ucLogoNo)
{
	static uint8_t sLogoOkThree[117] =
	{
		0x03,
		0x00,0x24,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc0,0x40,0x0,0x40,0x0,0x40,0x0,0x40,0x0,
		0x40,0x0,0x40,0x0,0x40,0x0,0x80,0xc0,0xc0,0xc0,0xe0,0x60,0x60,0x30,0x30,0x30,
		0x10,0x10,0x10,0x0,

		0x00,0x24,
		0x0,0x4,0x4,0x4,0xc,0x18,0x18,0xba,0x70,0xe0,0xe0,0xc0,0xc0,0x80,0xc0,0xe0,
		0xf0,0xf8,0x7c,0x3e,0x1f,0xf,0x7,0xab,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0,

		0x00,0x24,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x4,0x0,0x5,0x3,0x7,0x3,0x7,0x3,
		0x5,0x0,0x4,0x0,0x4,0x0,0x4,0x6,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0
	};
	static uint8_t sLogoOkTwo[117] =
	{
		0x03,
		0x00,0x24,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc0,0x40,0x0,0x40,0x0,0x40,0x0,0x40,0x0,
		0x40,0x0,0x40,0x0,0x40,0x0,0x40,0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0,

		0x00,0x24,
		0x0,0x4,0x4,0x4,0xc,0x18,0x18,0xba,0x70,0xe0,0xe0,0xc0,0xc0,0x80,0xc0,0xe0,
		0xf0,0xf8,0x7c,0x38,0x10,0x0,0x0,0xaa,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0,

		0x00,0x24,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x4,0x0,0x5,0x3,0x7,0x3,0x7,0x3,
		0x5,0x0,0x4,0x0,0x4,0x0,0x4,0x6,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0
	};
	static uint8_t sLogoOkOne[117] =
	{
		0x03,
		0x00,0x24,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc0,0x40,0x0,0x40,0x0,0x40,0x0,0x40,0x0,
		0x40,0x0,0x40,0x0,0x40,0x0,0x40,0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0,

		0x00,0x24,
		0x0,0x4,0x4,0x4,0xc,0x18,0x18,0xba,0x70,0xe0,0xe0,0xc0,0xc0,0x80,0x0,0x0,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xaa,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0,

		0x00,0x24,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x4,0x0,0x5,0x3,0x7,0x3,0x6,0x0,
		0x4,0x0,0x4,0x0,0x4,0x0,0x4,0x6,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0
	};

	lcdGoto(76, 3);
	if( ucLogoNo==0 )
	{
		lcdDrLogo(76,3,sLogoOkOne);
	}
	else if( ucLogoNo==1 )
	{
		lcdDrLogo(76,3,sLogoOkTwo);
	}
	else
	{
		lcdDrLogo(76,3,sLogoOkThree);
	}
}

// ��ʾ�����ɹ��Ķ���
void DisplayOperOk(void *pszChnMsg, void *pszEngMsg)
{
//	uint8_t	ucLogoNo;
//	int   iOldTime,iNewTime;

	Display2Strings(pszChnMsg, pszEngMsg);

/*	iOldTime = sysGetTimerCount();
	for(ucLogoNo=0; ucLogoNo<3; )
	{
		iNewTime = sysGetTimerCount();
		if( iOldTime+200 > iNewTime )
		{
			DispOkSub(ucLogoNo);
			ucLogoNo++;
			iOldTime = iNewTime;
		}
	}*/
	lcdFlip();
	sysDelayMs(1500);
}

int    DisplayWarningPromt(void)
{
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDispString("CLEAR CONFIG?", DISP_LINE_CENTER|3);
		PubDispString("[CANCEL] [ENTER]", DISP_LINE_CENTER|6);
	}
	else
	{
		PubDispString("ȷ��ɾ���ն�����", DISP_LINE_CENTER|2);
		PubDispString("CLEAR CONFIG?", DISP_LINE_CENTER|4);
		PubDispString("[ȡ��]  [ȷ��]", DISP_LINE_CENTER|6);
	}
	if(!PubYesNo(USER_OPER_TIMEOUT))
	{
		return FALSE;
	}
	
	return TRUE;
}

void   DisaptEmvTransFail(int iErrCode)
{
	if (iErrCode == 0 || iErrCode == ERR_NO_DISP)
	{
		return;
	}

	if( !ChkIfEmvEnable() )
	{
		return;
	}

//  �����HOST��Ȩʧ�ܣ�ֱ���˳�
	if (gstTransInfo.bIsOnlineTxn &&
		memcmp(gstTransInfo.stTranLog.szRspCode,"00",2) !=0 )
	{
		return;
	}
	
	if (gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT)
	{
		if (gstTransInfo.bIsOnlineTxn &&
			memcmp(gstTransInfo.stTranLog.szRspCode,"00",2)==0)
		{
			lcdClrLine(2,7);
			Display2StringInRect("��Ƭ���ܾ�����", "ICC AUTH FAILED");
		}
		else
		{
			lcdClrLine(2,7);
			Display2StringInRect("EMV����ʧ��","DECLINED");
		}

		PubWaitKey(10);
	}

	return;
}

// END OF FILE



