
#include "global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void ClearReversalSub(void);
static void ViewTranSub(INT32 iStartRecNo);

/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

void UnLockTerminal(void)
{
	if( ChkEdcOption(EDC_KEYBOARD_LOCKED) )
	{
		return;
	}

	while( !ChkEdcOption(EDC_KEYBOARD_LOCKED) )
	{
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDisplayTitle(TRUE, "TERMINAL  LOCKED");
		else
			PubDisplayTitle(TRUE, "�ն�����  LOCKED");
		Display2StringInRect("�������ն�����", "TERMINAL PWD");
		lcdFlip();
		PubWaitKey(-1);
		if( CheckTermPWD()==0 )
		{
			gstSysParam.stEdcInfo.sOption[EDC_KEYBOARD_LOCKED>>8] |= (EDC_KEYBOARD_LOCKED & 0xFF);
			SaveEdcParam();
			PubBeepOk();
		}
	}
}

void LockTerm(void)
{
	if(gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayTitle(TRUE,"Lock terminal");
		PubDispString(" TERMINAL LOCK? ", DISP_LINE_CENTER|3);
		PubDispString("[CANCEL] [ENTER]", DISP_LINE_CENTER|6);
	}
	else
	{
		PubDisplayTitle(TRUE," �� �� ��");
		PubDispString("   �Ƿ����ն�   ", DISP_LINE_CENTER|3);
		PubDispString("[ȡ��]  [ȷ��]", DISP_LINE_CENTER|6);
	}
	if(PubYesNo(USER_OPER_TIMEOUT))
	{
		gstSysParam.stEdcInfo.sOption[EDC_KEYBOARD_LOCKED>>8] &= ~(EDC_KEYBOARD_LOCKED & 0xFF);
		SaveEdcParam();
		PubBeepOk();
		UnLockTerminal();
	}
}

void ClearAllRecord(void)
{
	PubDisplayTitle(TRUE, "CLR BATCH");
	if( !AskYesNo() )
	{
		return;
	}

	ClearRecord(ACQ_ALL);
	ClearElecSignaturefile();
	fileMakeDir(STR_ELECSIGNATURE_FILE_PATH);  // �������ڱ������ǩ��ͼƬ��Ŀ¼,û�оʹ���
	DisplayClearOk();
}

void ClearConfig(void)
{
	char	szPasswordBak[MAX_PWD][16+1];	// ��������,���볤���16�ֽ�

	PubDisplayTitle(TRUE, "CLR CONFIG");
	if( !AskYesNo())
	{
		return;
	}

	PubDisplayTitle(TRUE, "CLR CONFIG");
	if( CheckBankPWD()!=0 )
	{
		return;
	}

	memcpy(&szPasswordBak[0][0],gstSysParam.szPassword,sizeof(gstSysParam.szPassword));
	LoadEdcDefault();
	memcpy(gstSysParam.szPassword,&szPasswordBak[0][0],sizeof(gstSysParam.szPassword));
	SaveAppTransParam();
	DisplayClearOk();
}

void ClearPassword(void)
{
	PubDisplayTitle(TRUE, "CLR PASSWORD");
	if( !AskYesNo())
	{
		return;
	}

	memset(gstSysParam.szPassword, '0', sizeof(gstSysParam.szPassword));
	SavePassword();
	DisplayClearOk();
}

void ClearReversal(void)
{
	PubDisplayTitle(TRUE, "CLR REVERSAL");
	if( !AskYesNo())
	{
		return;
	}

	ClearReversalSub();
	DisplayClearOk();
}

void ClearReversalSub(void)
{
	INT32		iCnt;

	for(iCnt=0; iCnt<(INT32)gstSysParam.uiAcqNum; iCnt++)
	{
		gstSysCtrlParam.stRevInfo[iCnt].bNeedReversal = FALSE;
		if( gstSysCtrlParam.sAcqStatus[iCnt]==S_PENDING )	// Ϊ��ɾ�������־???
		{
			gstSysCtrlParam.sAcqStatus[iCnt] = S_USE;
		}
	}
	SaveSysCtrlNormal();
}

// ɾ���ض�acquirer�Ľ��׼�¼
void ClearRecord(uint8_t ucAcqKey)
{
	INT32		iCnt;

	if( ucAcqKey==ACQ_ALL )
	{
		gstSysCtrlParam.uiLastRecNo = 0xFFFF;
		// ɾ��������־
		for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
		{
			gstSysCtrlParam.sAcqKeyList[iCnt]    = INV_ACQ_KEY;
			gstSysCtrlParam.sIssuerKeyList[iCnt] = INV_ISSUER_KEY;
		}

		// �ָ��յ���״̬�����������־
		for(iCnt=0; iCnt<(INT32)gstSysParam.uiAcqNum; iCnt++)
		{
			gstSysCtrlParam.sAcqStatus[iCnt]         = S_USE;
			gstSysCtrlParam.stRevInfo[iCnt].bNeedReversal = FALSE;
			gstSysCtrlParam.uiLastRecNoList[iCnt]    = 0xFFFF;
			if( !(gstSysParam.stAcqList[iCnt].ulCurBatchNo>0 &&
				  gstSysParam.stAcqList[iCnt].ulCurBatchNo<=999999L) )
			{
				gstSysParam.stAcqList[iCnt].ulCurBatchNo = 1L;
			}
		}
		SaveSysCtrlAll();
	}
	else
	{
		if( gstSysCtrlParam.uiLastRecNo<MAX_TRANLOG &&
			gstSysCtrlParam.sAcqKeyList[gstSysCtrlParam.uiLastRecNo]==ucAcqKey )
		{
			gstSysCtrlParam.uiLastRecNo = 0xFFFF;
		}

		// ɾ��������־
		for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
		{
			if( gstSysCtrlParam.sAcqKeyList[iCnt]==ucAcqKey )
			{
				gstSysCtrlParam.sAcqKeyList[iCnt]    = INV_ACQ_KEY;
				gstSysCtrlParam.sIssuerKeyList[iCnt] = INV_ISSUER_KEY;
			}
		}

		// �ָ��յ���״̬�����������־
		for(iCnt=0; iCnt<(INT32)gstSysParam.uiAcqNum; iCnt++)
		{
			if( gstSysParam.stAcqList[iCnt].ucKey==ucAcqKey )
			{
				gstSysCtrlParam.sAcqStatus[iCnt] = S_USE;
				gstSysCtrlParam.stRevInfo[iCnt].bNeedReversal = FALSE;
				gstSysCtrlParam.uiLastRecNoList[iCnt] = 0xFFFF;

				// increase batch no
				gstSysParam.stAcqList[iCnt].ulCurBatchNo = GetNewBatchNo(gstSysParam.stAcqList[iCnt].ulCurBatchNo);

				break;
			}
		}
		SaveSysCtrlNormal();
	}

	SaveAppTransParam();
}

// ����ն����ݽ���
void DoClear(void)
{
	static MenuItem stClearMenu[] =
	{
//		{TRUE, "CLR CONFIG",	ClearConfig},
		{TRUE, "CLR BATCH",		ClearAllRecord},
		{TRUE, "CLR REVERSAL",	ClearReversal},
//		{TRUE, "CLR PASSWORD",	ClearPassword},
		{TRUE, "",	NULL},
	};

	PubDisplayTitle(TRUE,"CLEAR");
	if( CheckTermPWD()!=0 )
	{
		return;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubGetMenu((uint8_t *)"CLEAR",            stClearMenu, MENU_AUTOSNO|MENU_CFONT, USER_OPER_TIMEOUT);
	else
		PubGetMenu((uint8_t *)"����       CLEAR", stClearMenu, MENU_AUTOSNO|MENU_CFONT, USER_OPER_TIMEOUT);
}

// �鿴���׻���
void ViewTotal(void)
{
	CalcTotal(ACQ_ALL);
	DisplayTransTotal(TRUE);
}

// �鿴���н��׼�¼
void ViewTranList(void)
{
	if( GetTranLogNum(ACQ_ALL)==0 )
	{
		PubDisplayTitle(TRUE, "EDC       REVIEW");
		Display2StringInRect("�޽��׼�¼", "EMPTY BATCH");
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	ViewTranSub(-1);	// ������н���
}

// �鿴ָ�����׼�¼
void ViewSpecList(void)
{
	INT32			iRet;
	ST_TRAN_LOG	stLog;

	if( GetTranLogNum(ACQ_ALL)==0 )
	{
		PubDisplayTitle(TRUE, "EDC       REVIEW");
		Display2StringInRect("�޽��׼�¼", "EMPTY BATCH");
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	iRet = GetRecord(TS_ALL_LOG, &stLog);
	if( iRet!=0 )
	{
		return;
	}

	ViewTranSub((INT32)gstTransInfo.uiRecNo);
}

// ���׼�¼�������
void ViewTranSub(INT32 iStartRecNo)
{
	INT32		iKey;
	INT32		iRecNo, iStep, iCnt, iActRecNo;
	ST_TRAN_LOG	stLog;

	iRecNo = iStartRecNo;
	iStep  = iStartRecNo<0 ? 1 : 0;
	while( 1 )
	{
		iRecNo = iRecNo + iStep;
		if( iRecNo>=MAX_TRANLOG )
		{
			lcdCls();
			Display2Strings("�� ¼ β", "END OF BATCH");
			lcdFlip();
			sysDelayMs(1000);
			iRecNo = 0;
		}
		else if( iRecNo<0 )
		{
			lcdCls();
			Display2Strings("�� ¼ ��", "START OF BATCH");
			lcdFlip();
			sysDelayMs(1000);
			iRecNo = MAX_TRANLOG-1;
		}
		if( gstSysCtrlParam.sAcqKeyList[iRecNo]==INV_ACQ_KEY )
		{
			continue;
		}
		memset(&stLog, 0, sizeof(ST_TRAN_LOG));
		LoadTranLog(&stLog, (UINT32)iRecNo);
		for(iActRecNo=iCnt=0; iCnt<=iRecNo; iCnt++)
		{
			if( gstSysCtrlParam.sAcqKeyList[iCnt]!=INV_ACQ_KEY )
			{
				iActRecNo++;
			}
		}
		DisplayTranLog((UINT32)iActRecNo, &stLog);

		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			break;
		}
		else if( iKey==KEY_DOWN || iKey== KEY_ENTER )
		{
			iStep = 1;
		}
		else if( iKey==KEY_UP || iKey==KEY_CLEAR )
		{
			iStep = -1;
		}
		else
		{
			iStep = 0;
		}
	}
}

void PrnLastTrans(void)
{
	INT32			iRet;

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "REPRINT LAST TRAN");
	else
		PubDisplayTitle(TRUE, "REPRINT ��ӡ����");
	lcdFlip();
	if( gstSysCtrlParam.uiLastRecNo>=MAX_TRANLOG )
	{
		Display2StringInRect("�޽��׼�¼", "EMPTY BATCH");
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	InitTransInfo();
	iRet = LoadTranLog(&gstTransInfo.stTranLog, gstSysCtrlParam.uiLastRecNo);
	if( iRet!=0 )
	{
		return;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "REPRINT         ");
	else
		PubDisplayTitle(TRUE, "REPRINT ��ӡ����");
	lcdFlip();
	FindAcq(gstTransInfo.stTranLog.ucAcqKey);
	FindIssuer(gstTransInfo.stTranLog.ucIssuerKey);
	if (gstPosCapability.uiPrnType != 0)
	{
		PrintReceipt(PRN_REPRINT);
	}
}

void RePrnSpecTrans(void)
{
	INT32			iRet;

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "REPRINT         ");
	else
		PubDisplayTitle(TRUE, "REPRINT ��ӡ����");
	lcdFlip();
	if( GetTranLogNum(ACQ_ALL)==0 )
	{
		Display2StringInRect("�޽��׼�¼", "EMPTY BATCH");
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	InitTransInfo();
	iRet = GetRecord(TS_ALL_LOG, &gstTransInfo.stTranLog);
	if( iRet!=0 )
	{
		return;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "REPRINT         ");
	else
		PubDisplayTitle(TRUE, "REPRINT ��ӡ����");
	lcdFlip();
	FindAcq(gstTransInfo.stTranLog.ucAcqKey);
	FindIssuer(gstTransInfo.stTranLog.ucIssuerKey);
	if (gstPosCapability.uiPrnType != 0)
	{
		PrintReceipt(PRN_REPRINT);
	}
}

void PrnTotal(void)
{
	INT32		iRet;
	uint8_t	szTitle[16+1], ucIndex, ucCnt;
	char    szDateTime[30], szBuff[100];

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szTitle, "PRINT TOTAL     ");
	else
		sprintf((char *)szTitle, "��ӡ�ܶ�PrnTotal");
	PubDisplayTitle(TRUE,(char *)szTitle);

	iRet = SelectAcq(FALSE, szTitle, &ucIndex);
	if( iRet!=0 )
	{
		return;
	}
	CalcTotal(gstCurAcq.ucKey);

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "PRINT TOTAL     ");
	else
		PubDisplayTitle(TRUE, "��ӡ�ܶ�PrnTotal");
	lcdFlip();
	if( gstTransTotal.uiSaleCnt==0     && gstTransTotal.uiRefundCnt==0 &&
		gstTransTotal.uiVoidSaleCnt==0 && gstTransTotal.uiVoidRefundCnt==0 )
	{
		Display2StringInRect("�޽��׼�¼", "EMPTY BATCH");
		PubBeepErr();
		PubWaitKey(3);
		return ;
	}

	PrnHead();

	prnStep(3);
	PrintfStrigMidOfLine("  TRANS TOTALS BY CARD\n");

	GetDateTime((uint8_t *)szDateTime);
	ChangeToEngTime(szDateTime, szBuff);  //DATE/TIME
	prnPrintf("DATE: %12.12s     TIME: %6.6s:%2.2s\n", szBuff,szBuff+14,szDateTime+12);
	prnPrintf("ACQUIRER: %-10.10s\n", gstCurAcq.szName);

	for(ucCnt=0; ucCnt<gstSysParam.uiIssuerNum; ucCnt++)
	{
		if( gstCurAcq.sIssuerKey[ucCnt]!=INV_ISSUER_KEY )
		{
			PrnTotalIssuer(gstCurAcq.sIssuerKey[ucCnt]);
		}
		if( (ucCnt%5)==4 )
		{
			if( StartPrinter()!=0 )
			{
				return;
			}
			prnInit();
			prnSetSpace(0, 0);
			prnSetGray(150);
//			prnSetGray(200);
		}
	}
	PrintfStrigMidOfLine("   END  OF  TOTAL\n");
	prnPrintf("\n\n\n\n");

	StartPrinter();
}

void RePrnSettle(void)
{
	INT32		iRet;
	uint8_t	szTitle[16+1];

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szTitle, "REPRINT SETTLE  ");
	else
		sprintf((char *)szTitle, "������ӡPrnSettl");
	iRet = SelectAcq(FALSE, szTitle, NULL);
	if( iRet!=0 )
	{
		return;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "REPRINT SETTLE  ");
	else
		PubDisplayTitle(TRUE, "������ӡPrnSettl");
	lcdFlip();
	if( !gstSysCtrlParam.stRePrnStlInfo.bValid[gstCurAcq.ucIndex] )
	{
		Display2Strings("�޽��˵���", "NO RECEIPT");
		PubWaitKey(3);
		return;
	}

	sprintf((char *)gstTransInfo.szSettleMsg, "%s", gstSysCtrlParam.stRePrnStlInfo.szSettleMsg[gstCurAcq.ucIndex]);
	memcpy(&gstTransTotal, &gstSysCtrlParam.stRePrnStlInfo.stAcqTotal[gstCurAcq.ucIndex], sizeof(ST_TOTAL_INFO));
//	memcpy(glIssuerTotal, gstSysCtrlParam.stRePrnStlInfo.stIssTotal[gstCurAcq.ucIndex], sizeof(glIssuerTotal));
	memcpy(gstIssuerTotal, gstSysCtrlParam.stRePrnStlInfo.stIssTotal, sizeof(gstIssuerTotal));
	gstCurAcq.ulCurBatchNo = gstSysCtrlParam.stRePrnStlInfo.ulBatchNo[gstCurAcq.ucIndex];

	if (gstPosCapability.uiPrnType != 0)
	{
		PrintSettle(PRN_REPRINT);
	}
}

// Print EMV error log message
void PrintEmvErrLog(void)
{
	UINT32			uiCnt, uiActNum, uiTemp;
	uint8_t			szBuff[50];
	EMV_ERR_LOG		stErrLog;
	int     iRet;

	PubDisplayTitle(TRUE, "PRINT ERROR LOG");
	if( CheckTermPWD()!=0 )
	{
		return;
	}

	PubDisplayTitle(TRUE, "PRINT ERROR LOG");
	DisplayProcessing();

	prnInit();
	prnSetSpace(1, 4);
	prnSetGray(150);
	SetCurrentPrnFont(gSystemAsciiEn16);
	prnPrintf("  EMV ERROR LOG\n\n");
	
	for(uiActNum=uiCnt=0; uiCnt<MAX_ERR_LOG; uiCnt++)
	{
		memset(&stErrLog, 0, sizeof(EMV_ERR_LOG));
		iRet = LoadErrLog(uiCnt, &stErrLog);
		if (iRet == ERR_NOT_FILE)
		{
			return;
		}
		if( !stErrLog.bValid )
		{
			continue;
		}

		uiActNum++;
		prnPrintf("\nSTAN: %06lu\n", stErrLog.ulSTAN);
		PubBcd2Asc0((char*)stErrLog.szAID, stErrLog.ucAidLen, (char*)szBuff);
		prnPrintf("AID: %s\n", szBuff);
		prnPrintf("PAN: %s\n", stErrLog.szPAN);
		prnPrintf("PAN SEQ #: %02X\n", stErrLog.ucPANSeqNo);
		prnPrintf("AMT: %.12s\n", stErrLog.szAmount);
		prnPrintf("TIP: %.12s\n", stErrLog.szTipAmt);
		prnPrintf("RSP: %.2s\n",  stErrLog.szRspCode);
		prnPrintf("RRN: %.12s\n", stErrLog.szRRN);
		prnPrintf("AUT: %.6s\n",  stErrLog.szAuthCode);
		prnPrintf("TVR: %02X %02X %02X %02X %02X\n", stErrLog.sTVR[0], stErrLog.sTVR[1],
				stErrLog.sTVR[2], stErrLog.sTVR[3], stErrLog.sTVR[4]);
		prnPrintf("TSI: %02X %02X\n", stErrLog.sTSI[0], stErrLog.sTSI[1]);

		prnPrintf("REQ BIT 55:\n");
		for(uiTemp=0; uiTemp<stErrLog.uiReqICCDataLen; uiTemp++)
		{
			prnPrintf("%02X%s", stErrLog.sReqICCData[uiTemp], (uiTemp%14)==13 ? " " : "");
		}
		prnPrintf("%s", uiTemp>0 ? "\n" : "");

		prnPrintf("REQ BIT 56:\n");
		for(uiTemp=0; uiTemp<stErrLog.uiReqField56Len; uiTemp++)
		{
			prnPrintf("%02X%s", stErrLog.sReqField56[uiTemp], (uiTemp%14)==13 ? "\n" : "");
		}
		prnPrintf("%s", uiTemp>0 ? "\n" : "");

		prnPrintf("RSP BIT 55:\n");
		for(uiTemp=0; uiTemp<stErrLog.uiRspICCDataLen; uiTemp++)
		{
			prnPrintf("%02X", stErrLog.sRspICCData[uiTemp]);
		}
		prnPrintf("%s", uiTemp>0 ? "\n" : "");

		if( (uiActNum%5)== 4 )
		{
			prnPrintf("\n\n\n\n");
			if( StartPrinter()!=0 )
			{
				return;
			}

			prnInit();
			prnSetSpace(1, 4);
			prnSetGray(150);
			SetCurrentPrnFont(gSystemAsciiEn16);
		}
		else
		{
			prnPrintf("\n\n");
		}
	}

	prnPrintf("\n\n\n\n");

	StartPrinter(); 
}

// end of file
