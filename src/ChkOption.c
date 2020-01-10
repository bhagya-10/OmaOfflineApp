
#include "global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
//static uint8_t ChkCurAcqName(void *pszAcqName);

/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

// ɨ�������յ�����Ϣ,�ж��ն��Ƿ�ָ��EMV
uint8_t ChkIfEmvEnable(void)
{
	return TRUE;  // ֧��оƬ��
/*	uint8_t	i;

	for(i=0; i<gstSysParam.uiAcqNum; i++)
	{
		if( ChkOptionExt(gstSysParam.stAcqList[i].sOption, ACQ_EMV_FEATURE) )
		{
			return TRUE;
		}
	}

	return FALSE;*/
}

// ��鵱ǰ�����е�ѡ��
uint8_t ChkIssuerOption(UINT32 uiOption)
{
	PubASSERT( (uiOption>>8)<sizeof(gstCurIssuer.sOption) );
	return ChkOptionExt((uint8_t *)gstCurIssuer.sOption, uiOption);
//	return (gstCurIssuer.sOption[uiOption>>8] & (uiOption & 0xFF));
}

// ��鵱ǰ�յ���ѡ��
uint8_t ChkAcqOption(UINT32 uiOption)
{
	PubASSERT( (uiOption>>8)<sizeof(gstCurAcq.sOption) );
	return ChkOptionExt((uint8_t *)gstCurAcq.sOption, uiOption);
//	return (gstCurAcq.sOption[uiOption>>8] & (uiOption & 0xFF));
}

// ����ն�ѡ��
uint8_t ChkEdcOption(UINT32 uiOption)
{
	PubASSERT( (uiOption>>8)<sizeof(gstSysParam.stEdcInfo.sOption) );
	return ChkOptionExt((uint8_t *)gstSysParam.stEdcInfo.sOption, uiOption);
//	return (gstSysParam.stEdcInfo.sOption[uiOption>>8] & (uiOption & 0xFF));
}

// ��չѡ����
uint8_t ChkOptionExt(uint8_t *psExtOpt, UINT32 uiOption)
{
	return (psExtOpt[uiOption>>8] & (uiOption & 0xFF));
}

// ���ܣ��Ƿ������������ ��ڣ��� ���ڣ� TRUE ������ FALSE ��������
uint8_t ChkIfNeedPIN(void)
{
	return FALSE;
	return ChkIssuerOption(ISSUER_EN_PIN);
}

uint8_t ChkIfPinReqdAllIssuer(void)
{
/*	uint8_t	i;

	for(i=0; i<gstSysParam.uiIssuerNum; i++)
	{
		if( ChkOptionExt(gstSysParam.stIssuerList[i].sOption, ISSUER_EN_PIN) )
		{
			return TRUE;
		}
	} */

	return FALSE;
}


// ���ſ����׽���Ƿ����Floor Limit
uint8_t ChkIfBelowMagFloor(void)
{
	uint8_t	szBuff[12+1];

	sprintf((char *)szBuff, "%012lu", gstCurIssuer.ulFloorLimit);
	if( memcmp(gstTransInfo.stTranLog.szAmount, szBuff, 12)>=0 )
	{
		return FALSE;
	}

	return TRUE;
}

void CheckCapture(void)
{
	if( gstTransInfo.stTranLog.ucTranType!=SALE_OR_AUTH )
	{
		return;
	}

	gstTransInfo.stTranLog.ucTranType = SALE;
	return;

/*	if( ChkEdcOption(EDC_CASH_PROCESS) )
	{
		gstTransInfo.stTranLog.ucTranType = CASH;
	}
	else if( ChkIssuerOption(ISSUER_CAPTURE_TXN) )
	{
		gstTransInfo.stTranLog.ucTranType = SALE;
		if( ChkIfInstalment() && (gstTransInfo.stTranLog.ucInstalment!=0) )
		{
			gstTransInfo.stTranLog.ucTranType = INSTALMENT;
		}
	}
	else
	{
		if( ChkEdcOption(EDC_AUTH_PREAUTH) )
		{
			gstTransInfo.stTranLog.ucTranType = AUTH;
		}
		else
		{
			gstTransInfo.stTranLog.ucTranType = PREAUTH;
		}
	}*/
}

uint8_t ChkInstalmentAllAcq(void)
{
	uint8_t	sBuff[256];
	uint8_t	i;

	memset(sBuff, 0, sizeof(sBuff));
	for(i=0; i<gstSysParam.uiPlanNum; i++)
	{
		sBuff[gstSysParam.stPlanList[i].ucAcqIndex] = 1;
	}

	for(i=0; i<gstSysParam.uiAcqNum; i++)
	{
		if( sBuff[i]==0 )
		{
			return FALSE;
		}
	}

	return TRUE;
}

uint8_t ChkIfCurAcqEMVEnable(void)
{
	return ChkAcqOption(ACQ_EMV_FEATURE);
}

uint8_t ChkIfDispMaskPan2(void)
{
	return ChkOptionExt((uint8_t *)gstCurIssuer.sPanMask, 0x0040);
}

uint8_t ChkIfInstalment(void)
{
	return FALSE;

/*	if( !ChkEdcOption(EDC_ENABLE_INSTALMENT) )
	{
		return FALSE;
	}

	return (gstSysParam.uiPlanNum > 0);*/
}

/*
// compare acquirer name in upper case
uint8_t ChkCurAcqName(void *pszAcqName)
{
	uint8_t	szBuff[10+1];

	sprintf((char *)szBuff, "%.10s", gstCurAcq.szName);
	PubStrUpper(szBuff);
	if( memcmp(szBuff, pszAcqName, strlen((char *)pszAcqName))==0 )
	{
		return TRUE;
	}

	return FALSE;
}
*/
uint8_t ChkIfTranAllow(uint8_t ucTranType)
{
	if( ucTranType==SETTLEMENT || ucTranType==UPLOAD || ucTranType == LOGON )
	{
		return TRUE;
	}

	if( GetTranLogNum(ACQ_ALL)>=MAX_TRANLOG )
	{
		PubDisplayTitle(TRUE, "BATCH FULL");
		Display2Strings("���Ƚ���", "PLS SETTLE BATCH");
		PubBeepErr();
		PubWaitKey(5);
		return FALSE;
	}

	if( gstTransInfo.stTranLog.ucInstalment!=0 && ChkIfInstalment() )
	{
		return TRUE;
	}

	return TRUE;
}

uint8_t ChkSettle(void)
{
	UINT32  uiTotalAmt;

	if( gstSysCtrlParam.sAcqStatus[gstCurAcq.ucIndex]==S_PENDING )
	{
		PubDisplayTitle(TRUE, "SETTLE PENDING");
		Display2Strings("���Ƚ���", "PLS SETTLE BATCH");
		PubLongBeep();
		PubWaitKey(5);
		return FALSE;
	}

	if (gstTransInfo.stTranLog.ucTranType == SALE ||
		gstTransInfo.stTranLog.ucTranType == OFF_SALE ||
		gstTransInfo.stTranLog.ucTranType == PREAUTH ||
		gstTransInfo.stTranLog.ucTranType == VOID)
	{
		CalcTotal(ACQ_ALL);
		uiTotalAmt = PubAsc2Long((char *)gstTransTotal.szSaleAmt,12,NULL);
		if (uiTotalAmt >= gstSysParam.stEdcInfo.ulMaxSettleAmt)
		{
			PubDisplayTitle(TRUE, "OUT OF MAX AMT");
			Display2Strings("���Ƚ���", "PLS SETTLE BATCH");
			PubLongBeep();
			PubWaitKey(5);
			return FALSE;
		}
	}

	return TRUE;
}

uint8_t ChkIfNeedTip(void)
{
	if( ChkEdcOption(EDC_TIP_PROCESS) &&
		((gstTransInfo.stTranLog.ucTranType==SALE)     ||
		 (gstTransInfo.stTranLog.ucTranType==OFF_SALE) ||
		 (gstTransInfo.stTranLog.ucTranType==CASH))      )
	{
		return TRUE;
	}

	return FALSE;
}


uint8_t ChkIfIccTran(uint8_t ucEntryMode)
{
	if( (ucEntryMode & MODE_CHIP_INPUT) || (ucEntryMode & MODE_FALLBACK_SWIPE) )
	{
		return TRUE;
	}

	return FALSE;
}

uint8_t ChkIfPrnReceipt(void)
{
	PubASSERT( gstTransInfo.stTranLog.ucTranType<MAX_TRANTYPE );
	return (glTranConfig[gstTransInfo.stTranLog.ucTranType].ucTranAct & PRN_RECEIPT);
}

uint8_t ChkIfNeedReversal(void)
{
	PubASSERT( gstTransInfo.stTranLog.ucTranType<MAX_TRANTYPE );
	return (glTranConfig[gstTransInfo.stTranLog.ucTranType].ucTranAct & NEED_REVERSAL);
}

uint8_t ChkIfSaveLog(void)
{
	PubASSERT( gstTransInfo.stTranLog.ucTranType<MAX_TRANTYPE );
	return (glTranConfig[gstTransInfo.stTranLog.ucTranType].ucTranAct & WRT_RECORD);
}


uint8_t ChkIfNeedMac(void)
{
	return TRUE;
}

uint8_t ChkIfAmex(void)
{
	return FALSE;  // ��ʱ��֧��
 	return ChkAcqOption(ACQ_AMEX_SPECIFIC_FEATURE);
}

uint8_t ChkIfNeedSecurityCode(void)
{
	if( gstTransInfo.stTranLog.ucTranType==OFF_SALE || gstTransInfo.stTranLog.ucTranType==VOID )
	{
		return FALSE;
	}
	
	if( ChkIfAmex() )
	{
		if( gstTransInfo.stTranLog.ucTranType==REFUND && !ChkAcqOption(ACQ_ONLINE_REFUND) )
		{
			return FALSE;
		}
	}
	else if( gstTransInfo.stTranLog.ucTranType==REFUND )
	{
		return FALSE;
	}
	
	if( gstTransInfo.stTranLog.ucEntryMode & MODE_SWIPE_INPUT ||
		gstTransInfo.stTranLog.ucEntryMode & MODE_FALLBACK_SWIPE )
	{
		if( ChkIssuerOption(ISSUER_SECURITY_SWIPE) )
		{
			return TRUE;
		}
	}
	else if( gstTransInfo.stTranLog.ucEntryMode & MODE_MANUAL_INPUT )
	{
		if( ChkIssuerOption(ISSUER_SECURITY_MANUL) )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

uint8_t ChkIfNeedSettle(void)
{
	
	CalcTotal(ACQ_ALL);
	
	if( gstTransTotal.uiSaleCnt !=0   || gstTransTotal.uiVoidSaleCnt !=0 ||
		gstTransTotal.uiRefundCnt !=0 || gstTransTotal.uiVoidRefundCnt !=0 )
	{
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDispString("Not settlement", DISP_LINE_CENTER|2);
			PubDispString("Continue?", DISP_LINE_CENTER|4);
			PubDispString("[CANCEL] [ENTER]", DISP_LINE_CENTER|6);
		}
		else
		{
			PubDispString("�ն���δ�����¼", DISP_LINE_CENTER|2);
			PubDispString("�Ƿ��������?", DISP_LINE_CENTER|4);
			PubDispString("[ȡ��]  [ȷ��]", DISP_LINE_CENTER|6);
		}
		if(!PubYesNo(USER_OPER_TIMEOUT))
		{
			return FALSE;
		}
		
		return TRUE;
	}	

	return TRUE;
}

// end of file
