#include "PedSignature.h"
#include "global.h"
TRAN_CONFIG		glTranConfig[] =
{
	{"DOWNLOAD",       "0800", "000000", ACT_INC_TRACE},
	{"PRE-AUTH",       "0100", "300000", PRN_RECEIPT+ACT_INC_TRACE+NEED_REVERSAL+WRT_RECORD},
	{"AUTH",           "0100", "300000", PRN_RECEIPT+ACT_INC_TRACE+NEED_REVERSAL+WRT_RECORD},
	{"SALE",           "0200", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"INSTAL",         "0200", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"ENQUIRE",        "0800", "970000", ACT_INC_TRACE},
	{"ENQUIRE",        "0100", "000000", ACT_INC_TRACE},
	{"UPLOAD",         "0320", "000000", ACT_INC_TRACE},
	{"LOGON",          "0800", "920000", ACT_INC_TRACE},
	{"REFUND",         "0200", "200000", PRN_RECEIPT+ACT_INC_TRACE+IN_REFUND_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"REVERSAL",       "0400", "000000", 0},
	{"SETTLEMENT",     "0500", "920000", ACT_INC_TRACE},
	{"VOID",           "0200", "020000", PRN_RECEIPT+NEED_REVERSAL+ACT_INC_TRACE},
	{"UPLOAD OFFLINE", "0220", "000000", PRN_RECEIPT+ACT_INC_TRACE},
	{"OFFLINE",        "0220", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+WRT_RECORD+VOID_ALLOW},
	{"SALECOMP",       "0220", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"CASH",           "0200", "010000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"SALE_OR_AUTH",   "0200", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"TC-ADVICE",      "0320", "940000", ACT_INC_TRACE},
	{"ENQUIRE",        "0800", "930000", ACT_INC_TRACE},
};

TRAN_CONFIG		glTranConfig_CH[] =
{
	{"远程下载DOWNLOAD", "0800", "000000", ACT_INC_TRACE},
	{"预授权  PRE-AUTH", "0100", "300000", PRN_RECEIPT+ACT_INC_TRACE+NEED_REVERSAL+WRT_RECORD},
	{"授权        AUTH", "0100", "300000", PRN_RECEIPT+ACT_INC_TRACE+NEED_REVERSAL+WRT_RECORD},
	{"销售        SALE", "0200", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"分期        INST", "0200", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"汇率查询 ENQUIRE", "0800", "970000", ACT_INC_TRACE},
	{"汇率查询 ENQUIRE", "0100", "000000", ACT_INC_TRACE},
	{"批上送          ", "0320", "000000", ACT_INC_TRACE},
	{"签到       LOGON", "0800", "920000", ACT_INC_TRACE},
	{"退款      REFUND", "0200", "200000", PRN_RECEIPT+ACT_INC_TRACE+IN_REFUND_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"冲正    REVERSAL", "0400", "000000", 0},
	{"结算            ", "0500", "920000", ACT_INC_TRACE},
	{"取消        VOID", "0200", "020000", PRN_RECEIPT+NEED_REVERSAL+ACT_INC_TRACE},
	{"离线交易上送    ", "0220", "000000", PRN_RECEIPT+ACT_INC_TRACE},
	{"离线     OFFLINE", "0220", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+WRT_RECORD+VOID_ALLOW},
	{"销售    SALECOMP", "0220", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"现金        CASH", "0200", "010000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"销售SALE_OR_AUTH", "0200", "000000", PRN_RECEIPT+ACT_INC_TRACE+IN_SALE_TOTAL+NEED_REVERSAL+WRT_RECORD+VOID_ALLOW},
	{"TC-ADVICE       ", "0320", "940000", ACT_INC_TRACE},
	{"汇率查询 ENQUIRE", "0800", "930000", ACT_INC_TRACE},
};

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void ModifyProcessCode(void);
static void SetEntryMode(ST_TRAN_LOG *pstLog);
static void SetStdEntryMode(ST_TRAN_LOG *pstLog);
static void SetAmexEntryMode(ST_TRAN_LOG *pstLog);
static void SetCondCode(void);
 int  TransCashSub(void);
static int  TransSaleSub(void);
static int  TransAuthSub(uint8_t ucTranType);
static int  GetOfflineTrans(void);
static int  AdjustInput(void);
static int  TransSettleSub(void);
static int  TransUpLoad(void);
static void SetField57(void);
static int  GetNeedTCTrans(void);

/********************** Internal variables declaration *********************/

/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

// 设置请求包的公共bits
void SetCommReqField(void)
{
	uint8_t	szTotalAmt[12+1];
	char    szBuffer[400];
	int		iLength;

	iLength = 0;
	// set common bits
	memset(&gstSendPack, 0, sizeof(ST_ISO8583));
	sprintf((char *)gstSendPack.szMsgCode,    "%.*s", LEN_MSG_CODE,
			glTranConfig[gstTransInfo.stTranLog.ucTranType].szTxMsgID);

	sprintf((char *)gstSendPack.szProcCode,   "%.*s", LEN_PROC_CODE,
			glTranConfig[gstTransInfo.stTranLog.ucTranType].szProcCode);
	sprintf((char *)gstSendPack.szNii,        "%.*s", LEN_NII,         gstCurAcq.szNii);
	sprintf((char *)gstSendPack.szTermID,     "%.*s", LEN_TERM_ID,     gstCurAcq.szTermID);
	sprintf((char *)gstSendPack.szMerchantID, "%.*s", LEN_MERCHANT_ID, gstCurAcq.szMerchantID);

	sprintf((char *)gstSendPack.szSTAN, "%06lu", gstSysCtrlParam.ulSTAN);  //??

	// 下面的交易不要再赋值给交易流水号
	if (gstTransInfo.stTranLog.ucTranType != TC_SEND)
	{
		gstTransInfo.stTranLog.ulSTAN = gstSysCtrlParam.ulSTAN;
	}

	// modify bit 3, process code
	ModifyProcessCode();

	if( gstTransInfo.stTranLog.ucTranType==SETTLEMENT || gstTransInfo.stTranLog.ucTranType==UPLOAD ||
		gstTransInfo.stTranLog.ucTranType==LOGON )
	{
		if (gstTransInfo.stTranLog.ucTranType==SETTLEMENT )
		{
			// bit 62, ROC/SOC
			memcpy(gstSendPack.sField62, "\x00\x06", 2);
			sprintf((char *)&gstSendPack.sField62[2], "%06lu", gstTransInfo.stTranLog.ulInvoiceNo);
			SetField57();
		}
		else
		{
			sprintf((char *)gstSendPack.szLocalTime, "%.6s",  &gstTransInfo.stTranLog.szDateTime[8]);
			sprintf((char *)gstSendPack.szLocalDate, "%.4s",  &gstTransInfo.stTranLog.szDateTime[4]);
		}

		if( ChkIfNeedMac() )
		{
			PubLong2Char((ulong)LEN_MAC, 2, gstSendPack.sMac);
		}

		return;
	}

	// bit 4, transaction amount
	if( gstTransInfo.stTranLog.szTipAmount[0]!=0 &&
		memcmp(gstTransInfo.stTranLog.szTipAmount, "000000000000", 12)!=0 )
	{
		PubAscAdd((char*)gstTransInfo.stTranLog.szAmount, (char*)gstTransInfo.stTranLog.szTipAmount, 12, (char*)szTotalAmt);
		PubAddHeadChars((char*)szTotalAmt, 12, '0');
		sprintf((char *)gstSendPack.szTranAmt,   "%.*s", LEN_TRAN_AMT,   szTotalAmt);
		sprintf((char *)gstSendPack.szExtAmount, "%.*s", LEN_EXT_AMOUNT, gstTransInfo.stTranLog.szTipAmount);
		if( gstTransInfo.stTranLog.ucTranType==REFUND && ChkIfAmex() )
		{
			gstSendPack.szExtAmount[0] = 0;
		}
	}
	else
	{
		sprintf((char *)gstSendPack.szTranAmt, "%.*s", LEN_TRAN_AMT, gstTransInfo.stTranLog.szAmount);
	}

	if( gstTransInfo.stTranLog.ucTranType==OFFLINE_SEND )
	{
		sprintf((char *)gstSendPack.szLocalTime, "%.6s",  &gstTransInfo.stTranLog.szDateTime[8]);
		sprintf((char *)gstSendPack.szLocalDate, "%.4s",  &gstTransInfo.stTranLog.szDateTime[4]);
		sprintf((char *)gstSendPack.szRRN,       "%.12s", gstTransInfo.stTranLog.szRRN);
		sprintf((char *)gstSendPack.szAuthCode,  "%.6s",  gstTransInfo.stTranLog.szAuthCode);
	}
	else if( gstTransInfo.stTranLog.ucTranType==VOID )
	{
		sprintf((char *)gstSendPack.szLocalTime, "%.6s",  &gstTransInfo.stTranLog.szDateTime[8]);
		sprintf((char *)gstSendPack.szLocalDate, "%.4s",  &gstTransInfo.stTranLog.szDateTime[4]);
		sprintf((char *)gstSendPack.szRRN,       "%.12s", gstTransInfo.stTranLog.szRRN);      
		sprintf((char *)gstSendPack.szAuthCode,  "%.6s",  gstTransInfo.stTranLog.szAuthCode);	
	}

	// PAN/track 1/2/3/expiry etc
	if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
	{
		if( gstTransInfo.stTranLog.szTrack2[0]==0 || 
			gstTransInfo.stTranLog.ucTranType==OFFLINE_SEND ||
			gstTransInfo.stTranLog.ucTranType == TC_SEND)
		{
			memset(szBuffer,0,sizeof(szBuffer));
			memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szPan));
			sprintf((char *)gstSendPack.szPan,     "%.*s", LEN_PAN,      szBuffer);
			memset(szBuffer,0,sizeof(szBuffer));
			memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szExpDate));
			sprintf((char *)gstSendPack.szExpDate, "%.*s", LEN_EXP_DATE, szBuffer);
		}
		else
		{
			memset(szBuffer,0,sizeof(szBuffer));
			memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szTrack2));
			sprintf((char *)gstSendPack.szTrack2,  "%.*s", LEN_TRACK2,   szBuffer);

		}

		if (gstTransInfo.stTranLog.bPanSeqOK)
		{
			sprintf((char *)gstSendPack.szPanSeqNo, "%0*X", LEN_PAN_SEQ_NO, gstTransInfo.stTranLog.ucPanSeqNo);
		}
	}
	else if( gstTransInfo.stTranLog.ucEntryMode & MODE_SWIPE_INPUT ||
			 gstTransInfo.stTranLog.ucEntryMode & MODE_FALLBACK_SWIPE )
	{
		if( gstTransInfo.stTranLog.ucTranType==OFFLINE_SEND || gstTransInfo.stTranLog.ucTranType==VOID )
		{
			memset(szBuffer,0,sizeof(szBuffer));
			memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szPan));
			sprintf((char *)gstSendPack.szPan,     "%.*s", LEN_PAN,      szBuffer);
			memset(szBuffer,0,sizeof(szBuffer));
			memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szExpDate));
			sprintf((char *)gstSendPack.szExpDate, "%.*s", LEN_EXP_DATE, szBuffer);
		}
		else
		{
			memset(szBuffer,0,sizeof(szBuffer));
			memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szTrack2));
			sprintf((char *)gstSendPack.szTrack2,  "%.*s", LEN_TRACK2,   szBuffer);
			memset(szBuffer,0,sizeof(szBuffer));
			memset(szBuffer,'0',strlen((char *)gstTransInfo.szTrack1));
			sprintf((char *)gstSendPack.szTrack1, "%.*s", LEN_TRACK1, szBuffer);
		}
	}
	else if( gstTransInfo.stTranLog.ucEntryMode & MODE_MANUAL_INPUT )
	{
		memset(szBuffer,0,sizeof(szBuffer));
		memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szPan));
		sprintf((char *)gstSendPack.szPan,     "%.*s", LEN_PAN,      szBuffer);
		memset(szBuffer,0,sizeof(szBuffer));
		memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szExpDate));
		sprintf((char *)gstSendPack.szExpDate, "%.*s", LEN_EXP_DATE, szBuffer);
	}

	SetEntryMode(&gstTransInfo.stTranLog);		// bit 22, entry mode
	SetCondCode();		// bit 25, service condition code

	// bit 48 CVV2 or 4DBC
	if( gstTransInfo.stTranLog.ucEntryMode & MODE_4DBC_INPUT )
	{
//		sprintf((char *)gstSendPack.sField48, "%-3.3s", gstTransInfo.szSecurityCode);
		sprintf((char *)gstSendPack.sField48, "%-3.3s", "000");
	}

	if( gstTransInfo.stTranLog.ucEntryMode & MODE_PIN_INPUT )
	{
		PubLong2Char((ulong)LEN_PIN_DATA, 2, gstSendPack.sPINData);
		memcpy(&gstSendPack.sPINData[2], gstTransInfo.sPinBlock, LEN_PIN_DATA);
	}

	// process bit 55,56
/*	if( gstTransInfo.stTranLog.ucTranType==AUTH || gstTransInfo.stTranLog.ucTranType==PREAUTH ||
		gstTransInfo.stTranLog.ucTranType==SALE || gstTransInfo.stTranLog.ucTranType==CASH    ||
		gstTransInfo.stTranLog.ucTranType==INSTALMENT )
	{
		if( gstTransInfo.stTranLog.ucEntryMode & MODE_FALLBACK_SWIPE )
		{
			SetDE55(FALSE, &gstSendPack.sICCData[2], &iLength);
			PubLong2Char((ulong)iLength, 2, gstSendPack.sICCData);
			memcpy(gstTransInfo.stTranLog.sIccData, &gstSendPack.sICCData[2], iLength);
			gstTransInfo.stTranLog.uiIccDataLen = (UINT32)iLength;
		}
	}*/

	if(gstTransInfo.stTranLog.ucTranType != LOGON &&
	   gstTransInfo.stTranLog.ucTranType != SETTLEMENT)
	{
		SetField57();
	}

	// bit 62, ROC/SOC
	memcpy(gstSendPack.sField62, "\x00\x06", 2);
	sprintf((char *)&gstSendPack.sField62[2], "%06lu", gstTransInfo.stTranLog.ulInvoiceNo);

	if( ChkIfNeedMac() )
	{
		PubLong2Char((ulong)LEN_MAC, 2, gstSendPack.sMac);
	}
}

// Bit 3 definition:
// AMEX 0200:
// For a sale transaction									00 40 0x
// For a refund transaction									20 40 0x
// For an on line void of on line sale						02 40 0x
// For an on line void of on line refund					22 40 0x
// For an on line void of off line sale seen by host		02 40 0x
// For an on line void of off line sale not seen by host	00 40 0x
//
// AMEX 0220:
// For a sale transaction (referred/sale comp or off line sale)
//													00 40 0x
// For an sale adjustment (i.e. add tip or void of sale):
//		When original sale not seen by host			00 40 0x
//		When original sale seen by host				02 40 0x
// For an off line refund transaction				20 40 0x
// For a void refund transaction:
//		When Trickle fed refund to host				22 40 0x
//		When Void off line refund not trickle fed	20 40 0x
void ModifyProcessCode(void)
{
	if ( ChkIfAmex() )
	{
		gstSendPack.szProcCode[2] = '4';
		if( gstTransInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			if( gstTransInfo.stTranLog.ucOrgTranType==SALE     ||
				gstTransInfo.stTranLog.ucOrgTranType==OFF_SALE ||
				gstTransInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				if( gstTransInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID) )
				{
					if( gstTransInfo.stTranLog.szRRN[0]!=0 )
					{
						gstSendPack.szProcCode[1] = '2';
					}
				}
			}
			else if( gstTransInfo.stTranLog.ucOrgTranType==REFUND )
			{
				gstSendPack.szProcCode[0] = '2';
				if( gstTransInfo.stTranLog.uiStatus & TS_VOID )
				{
					if( gstTransInfo.stTranLog.ucTranType!=SETTLEMENT )
					{	// trickle feed
						gstSendPack.szProcCode[1] = '2';
					}
				}
			}
		}
		else if( gstTransInfo.stTranLog.ucTranType==VOID )
		{
			if( gstTransInfo.stTranLog.ucOrgTranType==SALE )
			{
				gstSendPack.szProcCode[1] = '2';
			}
			else if( gstTransInfo.stTranLog.ucOrgTranType==REFUND )
			{
				gstSendPack.szProcCode[0] = '2';
				gstSendPack.szProcCode[1] = '2';
			}
			else if( gstTransInfo.stTranLog.ucOrgTranType==OFF_SALE ||
				gstTransInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				if( gstTransInfo.stTranLog.szRRN[0]!=0 )
				{
					gstSendPack.szProcCode[1] = '2';
				}
			}
		}
	} 
	else
	{
		if( gstTransInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			if( gstTransInfo.stTranLog.ucOrgTranType==SALE     ||
				gstTransInfo.stTranLog.ucOrgTranType==OFF_SALE ||
				gstTransInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				if( gstTransInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID) )
				{
					if( gstTransInfo.stTranLog.szRRN[0]!=0 )
					{
						gstSendPack.szProcCode[1] = '2';
					}
				}
			}
			else if( gstTransInfo.stTranLog.ucOrgTranType==REFUND )
			{
				gstSendPack.szProcCode[0] = '2';
				if( gstTransInfo.stTranLog.uiStatus & TS_ADJ )
				{
					if( gstTransInfo.stTranLog.szRRN[0]!=0 )
					{
						gstSendPack.szProcCode[1] = '2';
					}
				}
			}
		}
		else if( gstTransInfo.stTranLog.ucTranType==VOID )
		{
			if( gstTransInfo.stTranLog.ucOrgTranType==SALE )
			{
				gstSendPack.szProcCode[1] = '2';
			}
			else if( gstTransInfo.stTranLog.ucOrgTranType==REFUND )
			{
				gstSendPack.szProcCode[0] = '2';
				gstSendPack.szProcCode[1] = '2';
			}
		}
	}
}

void SetEntryMode(ST_TRAN_LOG *pstLog)
{
/*	if( pstLog->ucEntryMode & MODE_PIN_INPUT )
	{
		sprintf((char *)gstSendPack.szEntryMode, "0001");
	}
	else
	{
		sprintf((char *)gstSendPack.szEntryMode, "0002");
	} */
// 按照BOC的需求定义，我们机器是带PIN功能的机器，FIELD22最后byte置1
	sprintf((char *)gstSendPack.szEntryMode, "0001");

	if( ChkIfAmex() )
	{
		SetAmexEntryMode(pstLog);
	}
	else
	{
		SetStdEntryMode(pstLog);
	}
}

void SetAmexEntryMode(ST_TRAN_LOG *pstLog)
{
	if( ChkAcqOption(ACQ_EMV_FEATURE) )
	{
		gstSendPack.szEntryMode[1] = '5';
//		gstSendPack.szEntryMode[3] = '3';	// HK need offline pin
	}
	
	if( pstLog->ucEntryMode & MODE_CHIP_INPUT )
	{
		gstSendPack.szEntryMode[2] = '5';
	}
	else if( pstLog->ucEntryMode & MODE_SWIPE_INPUT )
	{
		gstSendPack.szEntryMode[2] = (pstLog->ucEntryMode & MODE_4DBC_INPUT) ? '6' : '2';
	}
	else if( pstLog->ucEntryMode & MODE_FALLBACK_SWIPE )
	{
		gstSendPack.szEntryMode[1] = '6';
		gstSendPack.szEntryMode[2] = (pstLog->ucEntryMode & MODE_4DBC_INPUT) ? '6' : '2';
	}
	else if( pstLog->ucEntryMode & MODE_MANUAL_INPUT )
	{
		gstSendPack.szEntryMode[2] = (pstLog->ucEntryMode & MODE_4DBC_INPUT) ? '7' : '1';
	}
}

void SetStdEntryMode(ST_TRAN_LOG *pstLog)
{
/*	if( ChkAcqOption(ACQ_EMV_FEATURE) )
	{
		gstSendPack.szEntryMode[0] = '5';
		if( ChkIfIccTran(pstLog->ucEntryMode) )
			gstSendPack.szEntryMode[3] = '1';
	}*/

	if( pstLog->ucEntryMode & MODE_CHIP_INPUT )
	{
		gstSendPack.szEntryMode[2] = '5';
	}
	else if( pstLog->ucEntryMode & MODE_SWIPE_INPUT || pstLog->ucEntryMode & MODE_CONTLESS_INPUT)
	{
		gstSendPack.szEntryMode[2] = '2';
	}
	else if( pstLog->ucEntryMode & MODE_FALLBACK_SWIPE )
	{
		gstSendPack.szEntryMode[1] = '8';
/*		if( pstLog->szPan[0]=='4' )
		{
			memcpy(gstSendPack.szEntryMode, "590", 3);
		}
		else if( pstLog->szPan[0]=='5' )
		{
			memcpy(gstSendPack.szEntryMode, "580", 3);
		}
		else if ( memcmp(pstLog->szPan, "35", 2)==0 )
		{
			memcpy(&gstSendPack.szEntryMode[1], "971", 3);
		}*/
	}
	else if( pstLog->ucEntryMode & MODE_MANUAL_INPUT )
	{
		gstSendPack.szEntryMode[2] = '1';
	}
}

// set bit 25
void SetCondCode(void)
{
	if (ChkIfAmex())
	{
		if( gstTransInfo.stTranLog.ucTranType==PREAUTH || gstTransInfo.stTranLog.ucTranType==AUTH)
		{
			sprintf((char *)gstTransInfo.stTranLog.szCondCode, "06");
		}
		else if( gstTransInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			sprintf((char *)gstTransInfo.stTranLog.szCondCode, "00");
			if( !(gstTransInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID)) &&
				gstTransInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				sprintf((char *)gstTransInfo.stTranLog.szCondCode, "06");
			}
/*			if( !(gstTransInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID)) &&
				(gstTransInfo.stTranLog.ucOrgTranType==SALE_COMP || gstTransInfo.stTranLog.ucOrgTranType==OFF_SALE) )
			{
				sprintf((char *)gstTransInfo.stTranLog.szCondCode, "06");
			} */
		}
		else
		{
			sprintf((char *)gstTransInfo.stTranLog.szCondCode, "00");
		}
	} 
	else
	{
		sprintf((char *)gstTransInfo.stTranLog.szCondCode, "00");
		if( gstTransInfo.stTranLog.ucTranType==PREAUTH )
		{
			sprintf((char *)gstTransInfo.stTranLog.szCondCode, "06");
		}
		else if( gstTransInfo.stTranLog.ucTranType==VOID || gstTransInfo.stTranLog.ucTranType==OFFLINE_SEND )
		{
			if( gstTransInfo.stTranLog.ucOrgTranType==SALE_COMP )
			{
				sprintf((char *)gstTransInfo.stTranLog.szCondCode, "06");
			}
		}
	}

	sprintf((char *)gstSendPack.szCondCode, "%.2s", gstTransInfo.stTranLog.szCondCode);
}


void SetField57(void)
{
	int   index, iBitmap;
	char  szData[1024], szbuffer[1024];

	memset(szbuffer,0,sizeof(szbuffer));
	memset(szData,0,sizeof(szData));
	index = 8;
	if (strlen((char *)gstSendPack.szPan))
	{
		PubAsc2Bcd((char *)gstTransInfo.stTranLog.szPan,strlen((char *)gstTransInfo.stTranLog.szPan),&szData[index]);
		index += (strlen((char *)gstTransInfo.stTranLog.szPan)+1)/2;
		iBitmap = 2 - 1;
		szData[iBitmap/8] |= (0x80>>(iBitmap%8));
	}

	if (strlen(gstSendPack.szExpDate))
	{
		PubAsc2Bcd((char *)gstTransInfo.stTranLog.szExpDate,strlen((char *)gstTransInfo.stTranLog.szExpDate),&szData[index]);
		index += strlen((char *)gstTransInfo.stTranLog.szExpDate)/2;
		iBitmap = 14 - 1;
		szData[iBitmap/8] |= (0x80>>(iBitmap%8));
	}

	if (strlen(gstSendPack.szTrack2))
	{
//		PubAsc2Bcd(gstTransInfo.stTranLog.szTrack2,strlen(gstTransInfo.stTranLog.szTrack2),&szData[index]);
		PubNoAsc2Bcd(gstTransInfo.stTranLog.szTrack2,strlen(gstTransInfo.stTranLog.szTrack2),&szData[index]);
		index += (strlen(gstTransInfo.stTranLog.szTrack2)+1)/2;
		iBitmap = 35 - 1;
		szData[iBitmap/8] |= (0x80>>(iBitmap%8));
	}

	if (strlen(gstSendPack.szTrack1))
	{
//		PubAsc2Bcd(gstTransInfo.szTrack1,strlen(gstTransInfo.szTrack1),&szData[index]);
		PubNoAsc2Bcd(gstTransInfo.szTrack1,strlen(gstTransInfo.szTrack1),&szData[index]);

		index += (strlen(gstTransInfo.szTrack1)+1)/2;
		iBitmap = 45 - 1;
		szData[iBitmap/8] |= (0x80>>(iBitmap%8));
	}

	if (strlen(gstSendPack.sField48))
	{
		sprintf(szbuffer,"0%s",gstTransInfo.szSecurityCode);
		PubAsc2Bcd(szbuffer,strlen(szbuffer),&szData[index]);
		index += (strlen(szbuffer)+1)/2;
		iBitmap = 48 - 1;
		szData[iBitmap/8] |= (0x80>>(iBitmap%8));
	}

//	NacTxd(gstTransInfo.szTrack1,strlen(gstTransInfo.szTrack1));
//	NacTxd(szData,index);

	if (index % 8)
	{
		memcpy(&szData[index],"\x80\x00\x00\x00\x00\x00\x00\x00",(8-(index%8)));
		index += (8 - (index%8));
	}

//	NacTxd(szData,index);

	// 先转换成	ASCII码后再进行补充
	memset(szbuffer,0,sizeof(szbuffer));
	PubBcd2Asc(szData,index,szbuffer);
	index = index * 2;
	memcpy(szData,szbuffer,index);
/*	if (index % 8)
	{
		memcpy(&szData[index],"\x80\x00\x00\x00\x00\x00\x00\x00",(8-(index%8)));
		index += (8 - (index%8));
	}*/
	memset(szbuffer,0,sizeof(szbuffer));

	iBitmap = 0;
	while(iBitmap < index)
	{
		PubXor(szbuffer,&szData[iBitmap],8);
		PedGetMac(DATA_KEY_ID,(uint8_t*)szbuffer,8,0x00,(uint8_t*)&szData[iBitmap]);
		memcpy(szbuffer,&szData[iBitmap],8);
		iBitmap += 8;
	}

	memset(gstSendPack.sField57,0,sizeof(gstSendPack.sField57));
	PubLong2Char(index+2,2,gstSendPack.sField57);
	PubAsc2Bcd("0003",4,gstSendPack.sField57+2);
	memcpy(gstSendPack.sField57+4,szData,index);
}


// 交易初始化:检查交易是否允许,显示交易标题
int TransInit(uint8_t ucTranType)
{
	lcdCls();
	gstTransInfo.stTranLog.ucTranType = ucTranType;
	gIsFirstSelectApp = 0;
	DisplayTransName();

	if( !ChkIfTranAllow(ucTranType) )
	{
		return ERR_NO_DISP;
	}

	return 0;
}

int TransCapture(void)
{
	int	iRet;

	iRet = TransSaleSub();
	if (gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType == CT_MODEM)
	{
		CommOnHook(TRUE);
	}
	else
	{
		CommOnHook(FALSE);
	}

	return iRet;

/*	if( gstTransInfo.stTranLog.ucTranType==CASH )
	{
		iRet = TransCashSub();
	}
	else if ( gstTransInfo.stTranLog.ucTranType==SALE ||
		gstTransInfo.stTranLog.ucTranType==INSTALMENT )
	{
		iRet = TransSaleSub();
	}
	else
	{
		if( ChkEdcOption(EDC_AUTH_PREAUTH) )
		{
			iRet = TransAuthSub(AUTH);
		}
		else
		{
			iRet = TransAuthSub(PREAUTH);
		}
	}

	return iRet; */
}

int TransSaleManualEntry(void)
{
	int		iRet;

	iRet = TransInit(SALE);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetCardInfo(CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	return TransSaleSub();
}

int TransCashSub(void)
{
	int		iRet;

	if( !ChkIfTranAllow(gstTransInfo.stTranLog.ucTranType) )
	{
		return ERR_NO_DISP;
	}
	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	lcdCls();
	DisplayTransName();
	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	iRet = TranProcess();
	if( iRet!=ERR_NEED_FALLBACK )
	{
		return iRet;
	}

	// continue fallback process
	gstTransInfo.bIsFallBack = TRUE;
	gstTransInfo.stTranLog.ucEntryMode &= 0xF0;
	iRet = GetCardInfo(FALLBACK_SWIPE|CARD_SWIPED);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	return TranProcess();
}

// 消费交易
int TransSale(void)
{
	int		iRet;

	iRet = TransInit(SALE);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetCardInfo(CARD_INSERTED|CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	return TransSaleSub();
}

int TransSaleSub(void)
{
	int		iRet;

	if( !ChkIfTranAllow(gstTransInfo.stTranLog.ucTranType) )
	{
		return ERR_NO_DISP;
	}
	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	lcdCls();
	DisplayTransName();
	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	iRet = TranProcess();
	if( iRet!=ERR_NEED_FALLBACK )
	{
		return iRet;
	}

	// continue fallback process
	gstTransInfo.bIsFallBack = TRUE;
	gstTransInfo.stTranLog.ucEntryMode &= 0xF0;
	iRet = GetCardInfo(FALLBACK_SWIPE|CARD_SWIPED);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	return TranProcess();
}

// 授权/预授权交易
int TransAuth(uint8_t ucTranType)
{
	int		iRet;

	PubASSERT(ucTranType==AUTH || ucTranType==PREAUTH);

	iRet = TransInit(ucTranType);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetCardInfo(CARD_INSERTED|CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkIssuerOption(ISSUER_EN_PREAUTH) )
	{
		DisplayBlockFunc();
		return ERR_NO_DISP;
	}

/*	if( CheckTransAdjustPWD()!=0 )
	{
		return ERR_NO_DISP;
	}*/

	return TransAuthSub(ucTranType);
}

int TransAuthSub(uint8_t ucTranType)
{
	int		iRet;

	if( !ChkIfTranAllow(ucTranType) )
	{
		return ERR_NO_DISP;
	}
	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	lcdCls();
	DisplayTransName();
	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	iRet = TranProcess();
	if( iRet!=ERR_NEED_FALLBACK )
	{
		return iRet;
	}

	// continue fallback process
	gstTransInfo.bIsFallBack = TRUE;
	gstTransInfo.stTranLog.ucEntryMode &= 0xF0;
	iRet = GetCardInfo(FALLBACK_SWIPE|CARD_SWIPED);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCommReqField();
	return TranProcess();
}

int FinishOffLine(void)
{
	uint8_t	ucTranAct;

	DisplayTransName();
	DisplayProcessing();

	if( !(gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT) &&
		(gstTransInfo.stTranLog.ucTranType!=SALE_COMP) )
	{
		sprintf((char *)gstTransInfo.stTranLog.szRspCode, "00");
	}
	gstTransInfo.stTranLog.ulInvoiceNo = gstSysCtrlParam.ulInvoiceNo;
	ucTranAct = glTranConfig[gstTransInfo.stTranLog.ucTranType].ucTranAct;
	if( ucTranAct & WRT_RECORD )
	{
		if( (gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT))
		{
			gstTransInfo.stTranLog.uiStatus |= TS_NEED_TC; //需要TC上送
		}
		gstTransInfo.stTranLog.uiStatus |= TS_NOSEND;
		SaveTranLog(&gstTransInfo.stTranLog);
	}

	if( ucTranAct & PRN_RECEIPT )	// print slip
	{
		CommOnHook(FALSE);
		GetNewInvoiceNo();
		if (gstPosCapability.uiPrnType != 0)
		{
#ifdef _APP_SUPP_ELECSIGN  // 电子签名  
 			if(ChkEdcOption(EDC_ENABLE_ELECSIGN))
 			{
				if(gstPosCapability.uipostsScreen)
					TransGetElecSignature();
				else
					TransElecSignature();
 			}
		 
#endif
			PrintReceipt(PRN_NORMAL);
		}
	}

	DisplayResult(0);
	PubWaitKey(gstSysParam.stEdcInfo.ulAcceptTimeout);

	return 0;
}

int TranReversal(void)
{
	int				iRet;
	ST_TRANSINFO	stProcInfoBak;
	char            szBuffer[200];

	if( !gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].bNeedReversal )
	{
		return 0;
	}

	// backup current process information
	memcpy(&gstTransInfo.stSendPack, &gstSendPack, sizeof(ST_ISO8583));
	memcpy(&stProcInfoBak, &gstTransInfo, sizeof(ST_TRANSINFO));
	gstTransInfo.stTranLog.ucTranType = REVERSAL;

	lcdCls();
	DisplayTransName();

	memcpy(&gstSendPack, &gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].stRevPack, sizeof(ST_ISO8583));
	sprintf((char *)gstSendPack.szMsgCode, "0400");
	memset(gstSendPack.sPINData, 0, sizeof(gstSendPack.sPINData));	// erase PIN block
	memset(gstSendPack.szRRN, 0, sizeof(gstSendPack.szRRN));
	memset(gstSendPack.szAuthCode, 0, sizeof(gstSendPack.szAuthCode));
	memset(gstSendPack.sField48,0,sizeof(gstSendPack.sField48));
	memset(gstSendPack.szTrack1,0,sizeof(gstSendPack.szTrack1));
	if (strlen((char *)gstSendPack.szPan))
	{
		strcpy((char *)gstTransInfo.stTranLog.szPan,gstSendPack.szPan);
		memset(szBuffer,0,sizeof(szBuffer));
		memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szPan));
		sprintf((char *)gstSendPack.szPan,     "%.*s", LEN_PAN,      szBuffer);
	}

	if (strlen(gstSendPack.szExpDate))
	{
		strcpy((char *)gstTransInfo.stTranLog.szExpDate,gstSendPack.szExpDate);
		memset(szBuffer,0,sizeof(szBuffer));
		memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szExpDate));
		sprintf((char *)gstSendPack.szExpDate,     "%.*s", LEN_EXP_DATE,      szBuffer);		
	}

	if (strlen(gstSendPack.szTrack2))
	{
		strcpy(gstTransInfo.stTranLog.szTrack2,gstSendPack.szTrack2);
		memset(szBuffer,0,sizeof(szBuffer));
		memset(szBuffer,'0',strlen(gstTransInfo.stTranLog.szTrack2));

		sprintf((char *)gstSendPack.szTrack2,     "%.*s", LEN_TRACK2,      szBuffer);
		memset(gstSendPack.szPan,0,sizeof(gstSendPack.szPan));	
		memset(gstSendPack.szExpDate,0,sizeof(gstSendPack.szExpDate));
	}
	SetField57();
	
/*	if (strlen(gstSendPack.szTrack2))
	{	
		memset(gstSendPack.szPan,0,sizeof(gstSendPack.szPan));	
		memset(gstSendPack.szExpDate,0,sizeof(gstSendPack.szExpDate));
	}*/

/*	memset(szBuffer,0,sizeof(szBuffer));
	GetDateTime(szBuffer);
	sprintf((char *)gstSendPack.szLocalTime, "%.6s",  &szBuffer[8]);
	sprintf((char *)gstSendPack.szLocalDate, "%.4s",  &szBuffer[4]);*/

	while( 1 )
	{
		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			break;
		}
		if( memcmp(gstRecvPack.szRspCode, "00", 2)==0 )
		{
			break;
		}

		if( ChkIfAmex() && (memcmp(gstRecvPack.szRspCode, "08", 2)==0 || memcmp(gstRecvPack.szRspCode, "88", 2)==0) )
		{
			break;
		}

		sprintf((char *)gstTransInfo.stTranLog.szRspCode, "%.2s", gstRecvPack.szRspCode);
		DisplayResult(ERR_HOST_REJ);
// 		iRet = ERR_NO_DISP;
		iRet = ERR_TRAN_FAIL;
		break;
	}
	if( iRet==0 )
	{	// clear reversal flag
		SaveRevInfo(FALSE);
	}

	// restore process information
	memcpy(&gstTransInfo, &stProcInfoBak, sizeof(ST_TRANSINFO));
	memcpy(&gstSendPack, &gstTransInfo.stSendPack, sizeof(ST_ISO8583));

	lcdCls();
	DisplayTransName();

	return iRet;
}

int GetOfflineTrans(void)
{
	int		iRet;
	UINT32	uiCnt;

	for(uiCnt=0; uiCnt<MAX_TRANLOG; uiCnt++)
	{
		if( gstSysCtrlParam.sAcqKeyList[uiCnt]!=gstCurAcq.ucKey )
		{
			continue;
		}

		memset(&gstTransInfo.stTranLog, 0, sizeof(ST_TRAN_LOG));
		iRet = LoadTranLog(&gstTransInfo.stTranLog, uiCnt);
		if( iRet!=0 )
		{
			return FALSE;
		}
		if( gstTransInfo.stTranLog.uiStatus & TS_NOSEND )
		{
			gstTransInfo.uiRecNo = uiCnt;
			return TRUE;
		}
	}

	return FALSE;
}

int OfflineSend(void)
{
	int				iRet;
	ST_TRANSINFO	stProcInfoBak;

	if( gstTransInfo.stTranLog.ucTranType!=SETTLEMENT )
	{	// the INT32 to load 400 txn log is about 1-2 seconds
		if( ChkAcqOption(ACQ_DISABLE_TRICK_FEED) || GetTranLogNum(gstCurAcq.ucKey)>100 )
		{
			return 0;
		}
	}

	memcpy(&gstTransInfo.stSendPack, &gstSendPack, sizeof(ST_ISO8583));
	memcpy(&stProcInfoBak, &gstTransInfo,  sizeof(ST_TRANSINFO));
	while( 1 )
	{
		InitTransInfo();
		if( !GetOfflineTrans() )
		{
			iRet = 0;
			break;
		}

		gstTransInfo.stTranLog.ucOrgTranType = gstTransInfo.stTranLog.ucTranType;
		gstTransInfo.stTranLog.ucTranType    = OFFLINE_SEND;
		DisplayTransName();

		SetCommReqField();
		// bit 4
		if( gstTransInfo.stTranLog.uiStatus & TS_VOID )
		{//VOID 时 金额是0，但是需要上送  pym
//			gstSendPack.szTranAmt[0] = 0;
			strcpy(gstSendPack.szTranAmt,"000000000000");			
		}

		// bit 37, 39
		if( !ChkIfAmex() )
		{
			if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
			{
				sprintf((char *)gstSendPack.szRspCode, "%.2s", gstTransInfo.stTranLog.szRspCode);
				if( gstTransInfo.stTranLog.szRRN[0]!=0 )	// seen by host
				{
					gstSendPack.szRspCode[0] = 0;
				}
			}
		}

		if( ChkIfAmex() && (gstTransInfo.stTranLog.uiStatus & TS_FLOOR_LIMIT) )
		{
			if( !(gstTransInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID)) )
			{
				sprintf((char *)gstSendPack.szAddlRsp, "03");
			}
		}

		// bit 55
		if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
		{
			if( ChkIfAmex() )
			{
				if( !(gstTransInfo.stTranLog.uiStatus & TS_ADJ) &&
					((gstTransInfo.stTranLog.ucOrgTranType==OFF_SALE) ||
					(gstTransInfo.stTranLog.ucOrgTranType==SALE_COMP) ||
					(gstTransInfo.stTranLog.ucOrgTranType==SALE && (gstTransInfo.stTranLog.uiStatus & TS_NOSEND))) )
				{
					PubLong2Char((ulong)gstTransInfo.stTranLog.uiIccDataLen, 2, gstSendPack.sICCData);
					memcpy(&gstSendPack.sICCData[2], gstTransInfo.stTranLog.sIccData, gstTransInfo.stTranLog.uiIccDataLen);
				}
			}
			else
			{
		// only send ICC sale below floor
				if( gstTransInfo.stTranLog.szRRN[0]==0 )
				{
					PubLong2Char((ulong)gstTransInfo.stTranLog.uiIccDataLen, 2, gstSendPack.sICCData);
					memcpy(&gstSendPack.sICCData[2], gstTransInfo.stTranLog.sIccData, gstTransInfo.stTranLog.uiIccDataLen);
				}
	// 				if( gstTransInfo.stTranLog.ucOrgTranType==SALE  &&
	// 					!(gstTransInfo.stTranLog.uiStatus & TS_ADJ) &&
	// 					(gstTransInfo.stTranLog.uiStatus & TS_NOSEND) )
	// 				{
	// 					PubLong2Char((ulong)gstTransInfo.stTranLog.uiIccDataLen, 2, gstSendPack.sICCData);
	// 					memcpy(&gstSendPack.sICCData[2], gstTransInfo.stTranLog.sIccData, gstTransInfo.stTranLog.uiIccDataLen);
	// 				}
			}

		}

		// bit 60
		if( !ChkIfAmex() && (gstTransInfo.stTranLog.uiStatus & (TS_ADJ|TS_VOID)) )
		{
			PubLong2Char(12,2,gstSendPack.szField60);
			if( gstTransInfo.stTranLog.uiStatus & TS_VOID )
			{
				sprintf((char *)gstSendPack.szField60+2, "%.12s", gstTransInfo.stTranLog.szAmount);
			}
			else
			{
				sprintf((char *)gstSendPack.szField60+2, "%.12s", gstTransInfo.stTranLog.szOrgAmount);
			}
		}

		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			break;
		}

		// update txn status
		gstTransInfo.stTranLog.uiStatus &= ~TS_NOSEND;
		gstTransInfo.stTranLog.uiStatus |= TS_OFFLINE_SEND;
		gstTransInfo.stTranLog.ucTranType = gstTransInfo.stTranLog.ucOrgTranType;
		UpdateTranLog(&gstTransInfo.stTranLog, gstTransInfo.uiRecNo);

		if( stProcInfoBak.stTranLog.ucTranType!=SETTLEMENT )
		{	// is trickle feed, only need send one txn
			break;
		}
	}
	memcpy(&gstTransInfo, &stProcInfoBak, sizeof(ST_TRANSINFO));
	memcpy(&gstSendPack, &gstTransInfo.stSendPack, sizeof(ST_ISO8583));

	return iRet;
}

// transaction complete for voice referral
int TranSaleComplete(void)
{
	int		iRet;
	int  	iKey;

	if( !ChkIssuerOption(ISSUER_EN_VOICE_REFERRAL) )
	{
		return ERR_TRAN_FAIL;
	}

	if( gstTransInfo.stTranLog.ucTranType>INSTALMENT &&
		gstTransInfo.stTranLog.ucTranType!=CASH )
	{
		return ERR_TRAN_FAIL;
	}

	if( gstTransInfo.stTranLog.ucTranType>=SALE )
	{
		gstTransInfo.stTranLog.ucTranType = SALE_COMP;
	}

	CommOnHook(FALSE);
	lcdDisplay(0, 0, DISP_CFONT|DISP_INVLINE|DISP_MEDIACY, "CALL:%-11.11s", gstCurIssuer.szRefTelNo);
	Display2StringInRect("查 询 卡 中 心", "PLS CALL BANK");

//	if( ChkEdcOption(EDC_REFERRAL_DIAL) )
	if (0)
	{
		iRet = ReferralDial((uint8_t *)gstCurIssuer.szRefTelNo);
		if( iRet!=0 )
		{
			return iRet;
		}
	}
	else
	{
		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			Display2Strings("操 作 取 消", "TRANS CANCELED");
			PubWaitKey(3);
			return ERR_NO_DISP;
		}
	}

	iRet = GetPreAuthCode();
	if( iRet!=0 )
	{
		Display2Strings("操 作 取 消", "TRANS CANCELED");
		PubWaitKey(3);
		return ERR_NO_DISP;
	}

	// 香港星展银行,sale_complete上送时, VISA/MASTER需要上送 BIT37 BIT39
	memmove(&gstTransInfo.stTranLog.szRRN[1], gstTransInfo.stTranLog.szRRN, 12);
	gstTransInfo.stTranLog.szRRN[0] = 0;
//	sprintf((char *)gstTransInfo.stTranLog.szCondCode, "06");

	return FinishOffLine();
}

// 退货
int TransRefund(void)
{
	int		iRet;

	iRet = TransInit(REFUND);
	if( iRet!=0 )
	{
		return iRet;
	}

/*	if( !ChkEdcOption(EDC_REFUND_PWD) )
	{
		if( CheckTransRefundPWD()!=0 )
		{
			return ERR_NO_DISP;
		}
	}*/

	lcdCls();
	DisplayTransName();
	iRet = GetCardInfo(SKIP_CHECK_ICC|CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	if(! ChkIssuerOption(ISSUER_EN_REFUND) )
	{
		DisplayBlockFunc();
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	lcdCls();
	DisplayTransName();
	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( ChkAcqOption(ACQ_ONLINE_REFUND) )
	{
		SetCommReqField();
		return TranProcess();
	}
	else
	{
		return FinishOffLine();
	}
}

// 离线
int TransOffSale(void)
{
	int		iRet;

	InitTransInfo();
	iRet = TransInit(OFF_SALE);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetCardInfo(SKIP_CHECK_ICC|CARD_SWIPED|CARD_KEYIN);
	if( iRet!=0 )
	{
		return iRet;
	}

	if( !ChkIssuerOption(ISSUER_EN_OFFLINE) )
	{
		DisplayBlockFunc();
		return ERR_NO_DISP;
	}
	
/*	if( CheckTransAdjustPWD()!=0 )
	{
		return ERR_NO_DISP;
	}*/

	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	iRet = GetAmount();
	if( iRet!=0 )
	{
		return iRet;
	}

	lcdCls();
	DisplayTransName();
	iRet = GetPreAuthCode();
	if( iRet!=0 )
	{
		return iRet;
	}

/*	lcdCls();
	DisplayTransName();
	iRet = GetPIN(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}*/

	// 赋值流水号
	gstTransInfo.stTranLog.ulSTAN = gstSysCtrlParam.ulSTAN;
	GetNewTraceNo();

	return FinishOffLine();
}

int TransVoid(void)
{
	int			iRet;
	uint8_t		ucTranAct, bOnlineFlag, szTempAmt[12+1];

	iRet = TransInit(VOID);
	if( iRet!=0 )
	{
		return iRet;
	}

/*	if( !ChkEdcOption(EDC_VOID_PWD) )
	{
		if( CheckTransVoidPWD()!=0 )
		{
			return ERR_NO_DISP;
		}
	}*/
	printf("1391%d\n",gstTransInfo.stTranLog.ulSTAN);
	while( 1 )
	{
		iRet = GetRecord(TS_OK|TS_NOSEND|TS_ADJ, &gstTransInfo.stTranLog);
		if( iRet!=0 )
		{
			return iRet;
		}

		ucTranAct = glTranConfig[gstTransInfo.stTranLog.ucTranType].ucTranAct;
		if( ucTranAct & VOID_ALLOW )
		{
			gstTransInfo.stTranLog.ulOrgSTAN = gstTransInfo.stTranLog.ulSTAN;
			gstTransInfo.stTranLog.ucOrgTranType = gstTransInfo.stTranLog.ucTranType;
			gstTransInfo.stTranLog.ucTranType    = VOID;
			break;
		}

		Display2StringInRect("该交易不能撤消", "NOT ALLOW VOID");
		PubBeepErr();
		PubWaitKey(3);
	}
	printf("1413%d\n",gstTransInfo.stTranLog.ulSTAN);
	FindAcq(gstTransInfo.stTranLog.ucAcqKey);
	FindIssuer(gstTransInfo.stTranLog.ucIssuerKey);

	lcdCls();
	DisplayTransName();
/*	if( !ChkIssuerOption(ISSUER_EN_VOID) )
	{
		DisplayBlockFunc();
		return ERR_NO_DISP;
	}*/

	if( !ChkSettle() )
	{
		return ERR_NO_DISP;
	}

	if( ChkAcqOption(ACQ_ONLINE_VOID) )
	{
		iRet = PreDial();
		iRet = CheckPreDial(iRet);
		if (iRet != 0)
		{
			return iRet;
		}
	}
	if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "总额       TOTAL");

	lcdClrLine(2,7);
	PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, (char *)szTempAmt);
	DisplayAmount(2, szTempAmt);	// show total amount
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDispString("CORRECT ? Y/N", DISP_LINE_LEFT|6);
	else
		PubDispString("是否取消",      DISP_LINE_LEFT|4);
	if( !PubYesNo(USER_OPER_TIMEOUT) )
	{
		return ERR_USERCANCEL;
	}

	lcdCls();
	DisplayTransName();

 	bOnlineFlag = TRUE;
  	if( gstTransInfo.stTranLog.uiStatus & TS_NOSEND )
 	{	// offsale/sale below/sale comp.../adjust
 		if( gstTransInfo.stTranLog.uiStatus & TS_ADJ )
 		{	// 该调整还未上送,作废调整后的金额
			PubAscSub((char *)gstTransInfo.stTranLog.szOrgAmount, (char *)gstTransInfo.stTranLog.szAmount, 12, (char *)szTempAmt);
			PubAddHeadChars((char *)szTempAmt, 12, '0');
			sprintf((char *)gstTransInfo.stTranLog.szTipAmount, "%.12s", szTempAmt);
 		}
		if( gstTransInfo.stTranLog.szRRN[0]==0 )
		{
			bOnlineFlag = FALSE;
		}
 	}

	bOnlineFlag = TRUE;
/* 	if( !ChkAcqOption(ACQ_ONLINE_VOID) ) // online void
	{
		bOnlineFlag = FALSE;
	}*/

	if( bOnlineFlag )
	{
		SetCommReqField();
		PubLong2Char((ulong)gstTransInfo.stTranLog.uiIccDataLen, 2, gstSendPack.sICCData);
		memcpy(&gstSendPack.sICCData[2], gstTransInfo.stTranLog.sIccData, gstTransInfo.stTranLog.uiIccDataLen);
		return TranProcess();
	}
	else	// offline void
	{
		gstTransInfo.stTranLog.uiStatus |= (TS_VOID|TS_NOSEND);
		if( gstTransInfo.stTranLog.szRRN[0]==0 )
		{	// not seen by host(orginal txn is offsale/sale below/salecomp/ ...)
			gstTransInfo.stTranLog.uiStatus &= ~(TS_NOSEND);
		}
		GetDateTime(gstTransInfo.stTranLog.szDateTime);
		UpdateTranLog(&gstTransInfo.stTranLog, gstTransInfo.uiRecNo);
		if (gstPosCapability.uiPrnType != 0)
		{
#ifdef _APP_SUPP_ELECSIGN  // 电子签名  
 			if(ChkEdcOption(EDC_ENABLE_ELECSIGN))
 			{
				if(gstPosCapability.uipostsScreen)
					TransGetElecSignature();
				else
					TransElecSignature();
 			}
		 
#endif
			PrintReceipt(PRN_NORMAL);
		}
		DisplayResult(0);

		return 0;
	}
}

int TransOther(void)
{
	int		iRet, iMenuNo, iFlag = 0;
	uint8_t	ucTranType;
	MenuItem stTranMenu_EN[] =
	{
		{TRUE, "VOID",          NULL},
		{TRUE, "SETTLEMENT",    NULL},
		{TRUE, "REPRINT LAST",  NULL},
		{TRUE, "REPRINT TRACE", NULL},
		{TRUE, "OFFLINE SALE",  NULL},
		{TRUE, "PREAUTH",       NULL},
		{TRUE, "MANUAL ENTRY",  NULL},
		{TRUE, "LOGON",         NULL}, 
		{TRUE, "SET PARAMETER", NULL},	
		{TRUE, "", NULL},
	};
	static	MenuItem * pstMenuItemList;
	static  uint8_t pszPrmpt[]       = "PLS SELECT:";

	pstMenuItemList = stTranMenu_EN;
	iMenuNo = PubGetMenu(pszPrmpt, pstMenuItemList, MENU_AUTOSNO|MENU_PROMPT, USER_OPER_TIMEOUT);
	switch( iMenuNo )
	{
	case 0:
		iRet = TransVoid();
        iFlag = 1;
		break;

	case 1:
		iRet = TransSettle();
        iFlag = 1;
		break;

	case 2:
		iRet = TransPrnLastTrans();
		break;
		
	case 3:
		iRet = TransRePrnSpecTrans();
		break;

	case 4:
		iRet = TransOffSale();
        iFlag = 1;
		break;

	case 5:
		ucTranType = PREAUTH;
		iRet = TransAuth(ucTranType);
        iFlag = 1;
		break;

	case 6:
		iRet = TransSaleManualEntry();
        iFlag = 1;
		break;

	case 7:
		iRet = TransLogon();
        iFlag = 1;
		break;

	case 8:
		SetParamForManager(); 
		iRet = ERR_NO_DISP;
		break;

	default:
		return ERR_NO_DISP;
		break;
	}

    if (1 == iFlag)
	{
		if (gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType == CT_MODEM)
		{
			CommOnHook(TRUE);
		}
		else
		{
			CommOnHook(FALSE);
		}
    }

	return iRet;
}

// 调整交易输入
int AdjustInput(void)
{
	int		iRet;

	while( 1 )
	{
		InitTransInfo();
		iRet = GetRecord(TS_NOSEND|TS_ADJ, &gstTransInfo.stTranLog);
		if( iRet!=0 )
		{
			return iRet;
		}

		DisplayAdjustTitle();
		if( memcmp(gstTransInfo.stTranLog.szAmount, "000000000000", 12)==0 &&
			memcmp(gstTransInfo.stTranLog.szTipAmount, "000000000000", 12)==0 )
		{
			Display2StringInRect("该交易不能调整", "DO NOT ADJUST");
			PubBeepErr();
			PubWaitKey(3);
			continue;
		}

		if( gstTransInfo.stTranLog.ucTranType==REFUND && ChkIfAmex() )
		{
			Display2StringInRect("退货不能调整", "DO NOT ADJUST");
			PubBeepErr();
			PubWaitKey(3);
			continue;
		}

		FindAcq(gstTransInfo.stTranLog.ucAcqKey);
		FindIssuer(gstTransInfo.stTranLog.ucIssuerKey);
		if( !ChkIssuerOption(ISSUER_EN_ADJUST) )
		{
			DisplayBlockFunc();
			continue;
		}
		if( ChkSettle() )
		{
			break;
		}
		return ERR_NO_DISP;
	}

	return 0;
}

// 调整
void TransAdjust( void )
{
	int		iRet;
	uint8_t	ucResult, szCurrName[4+1], szTotalAmt[12+1], szTemp[12+1];
	uint8_t	szCurTime[14+1];

/*	if( !ChkEdcOption(EDC_ADJUST_PWD) )
	{
		if( CheckTransAdjustPWD()!=0 )
		{
			return;
		}
	}*/

	if( GetTranLogNum(ACQ_ALL)==0 )
	{
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDisplayTitle(TRUE, "ADJUST          ");
		else
			PubDisplayTitle(TRUE, "调整      ADJUST");
		Display2StringInRect("无交易记录", "EMPTY BATCH");
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	while( 1 )
	{
		iRet = AdjustInput();
		if( iRet!=0 )
		{
			return;
		}

		while( 2 )
		{
			lcdCls();
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				PubDispString("BASE AMOUNT", 0|DISP_LINE_LEFT);
				PubDispString("TOTAL",       4|DISP_LINE_LEFT);
			}
			else
			{
				PubDispString("金额 BASE AMOUNT", 0|DISP_LINE_LEFT);
				PubDispString("总额       TOTAL", 4|DISP_LINE_LEFT);
			}
			DisplayAmount(2, gstTransInfo.stTranLog.szAmount);

			sprintf((char *)szCurrName, "%.3s", gstSysParam.stEdcInfo.szCurrencyName);
			memset(szTotalAmt, 0, sizeof(szTotalAmt));
			ucResult = PubGetAmount(szCurrName, 2, 1, 8, szTotalAmt, USER_OPER_TIMEOUT, 0);
			if( ucResult!=0 )
			{
				return;
			}
			PubAddHeadChars((char *)szTotalAmt, 12, '0');
			if( !ValidAdjustAmount(gstTransInfo.stTranLog.szAmount, szTotalAmt) )
			{
				continue;
			}
			if( !ConfirmAmount(szTotalAmt) )
			{
				break;
			}

			if( !(gstTransInfo.stTranLog.uiStatus & TS_NOSEND) )
			{
				PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, (char *)szTemp);
				PubAddHeadChars((char *)szTemp, 12, '0');
				sprintf((char *)gstTransInfo.stTranLog.szOrgAmount, "%.12s", szTemp);
				gstTransInfo.stTranLog.uiStatus |= (TS_ADJ|TS_NOSEND);
			}

			PubAscSub((char *)szTotalAmt, (char *)gstTransInfo.stTranLog.szAmount, 12, (char *)szTemp);
			PubAddHeadChars((char *)szTemp, 12, '0');
			sprintf((char *)gstTransInfo.stTranLog.szTipAmount, "%.12s", szTemp);
			GetDateTime(szCurTime);
			sprintf((char *)gstTransInfo.stTranLog.szDateTime, "%.14s", szCurTime);

			iRet = UpdateTranLog(&gstTransInfo.stTranLog, gstTransInfo.uiRecNo);
			if( iRet!=0 )
			{
				return;
			}

//			PrintReceipt(PRN_NORMAL);	// no receipt for adjust
			gstSysCtrlParam.uiLastRecNo = gstTransInfo.uiRecNo;		// use reprint to print adjust
			SaveSysCtrlBase();

			DisplayAdjustTitle();
			DispAccepted();
			PubBeepOk();
			PubWaitKey(3);
			break;	// continue to adjust
		}	// end of while( 2
	}	// end of while( 1
}

// 结算
int TransSettle(void)
{
	int		iRet;
	uint8_t	ucAcqIndex, szTitle[16+1];

	TransInit(SETTLEMENT);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szTitle, "%.16s",  "SELECT ACQUIRER");
	else
		sprintf((char *)szTitle, "%.16s",  "选择收单行  NII");

	iRet = SelectAcq(TRUE, szTitle, &ucAcqIndex);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	lcdCls();
	DisplayTransName();
	DisplayProcessing();
	if( ucAcqIndex==MAX_ACQ )
	{
		CalcTotal(ACQ_ALL);
	}
	else
	{
		CalcTotal(gstCurAcq.ucKey);
	}

	if( gstTransTotal.uiSaleCnt==0   && gstTransTotal.uiVoidSaleCnt==0 &&
		gstTransTotal.uiRefundCnt==0 && gstTransTotal.uiVoidRefundCnt==0 )
	{
		Display2StringInRect("无交易记录", "EMPTY BATCH");
		PubBeepErr();
		PubWaitKey(3);
		return ERR_NO_DISP;
	}

	lcdCls();
	DisplayTransName();
	iRet = DisplayTransTotal(FALSE);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	lcdCls();
	DisplayTransName();
/*	if( !ChkEdcOption(EDC_SETTLE_PWD) )
	{
		if( CheckTransSettlePWD()!=0 )
		{
			return ERR_NO_DISP;
		}
	}*/

	if( ucAcqIndex!=MAX_ACQ )
	{
		return TransSettleSub();
	}

	for(ucAcqIndex=0; ucAcqIndex<gstSysParam.uiAcqNum; ucAcqIndex++)
	{
		SetCurAcq(ucAcqIndex);
		memcpy(&gstTransTotal, &gstAcqTotal[ucAcqIndex], sizeof(ST_TOTAL_INFO));
		iRet = TransSettleSub();
		if( iRet!=0 )
		{
			if( iRet!=ERR_NO_DISP )
			{
				return iRet;
			}
		}
		ClearElecSignaturefile();
		fileMakeDir(STR_ELECSIGNATURE_FILE_PATH);  // 创建用于保存电子签名图片的目录,没有就创建
	}

	return 0;
}

// 结算当前收单行(gstCurAcq)
int TransSettleSub(void)
{
	int		iRet, bFirstSettle;
	uint8_t	szBuff[20];
	char  szCurTime[20], szDateTime[20];
	char  szLocalDate[10],szLocalTime[10];

	if( gstTransTotal.uiSaleCnt==0   && gstTransTotal.uiVoidSaleCnt==0 &&
		gstTransTotal.uiRefundCnt==0 && gstTransTotal.uiVoidRefundCnt==0 )
	{
		return 0;
	}

	TransInit(SETTLEMENT);
	lcdCls();
	DisplayTransName();

	sprintf((char*)gstTransInfo.szSettleMsg, "CLOSED");
	if( gstCurAcq.ulTimeOut<150 )
	{	// 结算时，timeout加长
		gstCurAcq.ulTimeOut += gstCurAcq.ulTimeOut/2;
	}
	else
	{
		gstCurAcq.ulTimeOut = 250;
	}

	bFirstSettle = 0;
	iRet = TranReversal();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = OfflineSend();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = TransSendTC(TRUE);
	if( iRet!=0 )
	{
		return iRet;
	}

	memset(szLocalTime,0,sizeof(szLocalTime));
	memset(szLocalDate,0,sizeof(szLocalDate));
	bFirstSettle = TRUE;
	while( 1 )
	{
		TransInit(SETTLEMENT);
		if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
			lcdDisplay(80, 1, DISP_ASCII|DISP_INVLINE, "%8.8s", gstCurAcq.szName);
		else
			lcdDisplay(48, 0, DISP_CFONT|DISP_INVLINE, "%10.10s", gstCurAcq.szName);
		SetCommReqField();
		sprintf((char *)gstSendPack.szProcCode, "%s", bFirstSettle ? "920000" : "960000");
		PubAddHeadChars((char *)gstTransTotal.szSaleAmt,   12, '0');
		PubAddHeadChars((char *)gstTransTotal.szRefundAmt, 12, '0');
		sprintf((char *)&gstSendPack.sField63[2], "%03d%12.12s%03d%12.12s%03d",
				gstTransTotal.uiSaleCnt,   gstTransTotal.szSaleAmt,
				gstTransTotal.uiRefundCnt, gstTransTotal.szRefundAmt,gstTransTotal.uiTcSendCnt);
//		memset(&gstSendPack.sField63[30+2], '0', 60); //should be 30+2
		PubLong2Char(33L, 2, gstSendPack.sField63);
		if( ChkIfAmex() )
		{	// AMEX settlement时, bit63 len = 36 byte
			gstSendPack.sField63[2+36] = 0;
		}
		PubLong2Char(6,2,gstSendPack.szField60);
		sprintf((char *)gstSendPack.szField60+2, "%06lu", gstCurAcq.ulCurBatchNo);
		memcpy(gstSendPack.sField62, "\x00\x06", 2);
		sprintf((char *)&gstSendPack.sField62[2], "%06lu", gstTransInfo.stTranLog.ulInvoiceNo);

		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			return iRet;
		}
		if( !bFirstSettle )
		{
			if( memcmp(gstRecvPack.szRspCode, "95", 2)==0 )
			{
				memcpy(gstRecvPack.szRspCode, "77", 2);
			}

			// 结算不平时再取最后一次对账的系统时间
			if (strlen(gstRecvPack.szLocalDate) != 0 && 
				strlen(gstRecvPack.szLocalTime) != 0)
			{
				strcpy(szLocalTime,gstRecvPack.szLocalTime);
				strcpy(szLocalDate,gstRecvPack.szLocalDate);
			}

			break;
		}

		if (strlen(gstRecvPack.szLocalDate) != 0 && 
			strlen(gstRecvPack.szLocalTime) != 0)
		{
			strcpy(szLocalTime,gstRecvPack.szLocalTime);
			strcpy(szLocalDate,gstRecvPack.szLocalDate);
		}

		if( memcmp(gstRecvPack.szRspCode, "95", 2)!=0 )
		{
			break;
		}

		// display error information
		DisplayResult(ERR_HOST_REJ);

		sprintf((char*)gstTransInfo.szSettleMsg, "CLOSED(95)");
		iRet = TransUpLoad();
		if( iRet!=0 )
		{
			return iRet;
		}
		bFirstSettle = FALSE;
	}
	if( memcmp(gstRecvPack.szRspCode, "77", 2)==0 )
	{
		sprintf((char*)gstTransInfo.szSettleMsg, "CLOSED(77 RECONCILE ERROR)");
	}

	if( memcmp(gstRecvPack.szRspCode, "00", 2)==0 ||
	    (!ChkIfAmex() && memcmp(gstRecvPack.szRspCode, "77", 2)==0) )
	{
		gstSysCtrlParam.sAcqStatus[gstCurAcq.ucIndex] = S_CLR_LOG;
		SaveSysCtrlBase();
		CommOnHook(FALSE);
		ClearRecord(gstCurAcq.ucKey);

		if (strlen(szLocalDate) != 0 && strlen(szLocalTime) != 0)
		{
			memset(szCurTime,0,sizeof(szCurTime));
			memset(szDateTime,0,sizeof(szDateTime));
			GetDateTime((uint8_t *)szCurTime);
			sprintf(szDateTime, "%.4s%.4s%.6s",szCurTime, szLocalDate, szLocalTime);
			memset(szCurTime,0,sizeof(szCurTime));
			PubAsc2Bcd(szDateTime+2,12,szCurTime);
			sysSetTime(szCurTime);
		}

		GetNewInvoiceNo();
		iRet = PrintSettle(PRN_NORMAL);	
		if( iRet!=0 )
		{
			Display2Strings("请进行重打印", "PLEASE REPRINT");
			PubWaitKey(3);
			iRet = ERR_NO_DISP;
		}

		if( memcmp(gstRecvPack.szRspCode, "77", 2)==0 )
		{
			Display2Strings("核 对 不 符", "RECONCILE ERROR");
		}
		else if( gstTransTotal.uiSaleCnt==0 && gstTransTotal.uiRefundCnt==0 )
		{
			Display2Strings("无交易总额", "NO TXN TOTAL");
		}
		else
		{
			lcdClrLine(2,7);
			sprintf((char *)szBuff, "BATCH %06ld", gstCurAcq.ulCurBatchNo);
			PubDispString(szBuff,   3|DISP_LINE_CENTER);
			PubDispString("CLOSED", 5|DISP_LINE_CENTER);
		}
		PubBeepOk();
		PubWaitKey(5);

		memset(gstSysCtrlParam.szLastSettlementDate,0,sizeof(gstSysCtrlParam.szLastSettlementDate));
		GetDateTime((uint8_t *)gstSysCtrlParam.szLastSettlementDate);
		SaveSysCtrlAll();
		return iRet;
	}

	if (memcmp(gstRecvPack.szRspCode, "00", 2)!=0)
	{
		DisplayResult(ERR_HOST_REJ);
	}

	return 0;
}

// 批上送
int TransUpLoad(void)
{
	int			iRet;
	UINT32		uiIndex, uiTranNum, uiCnt;
	uint8_t		szTotalAmt[12+1],szBuffer[200];
	ST_TRAN_LOG	stLog;

	TransInit(UPLOAD);
	uiTranNum = GetTranLogNum(gstCurAcq.ucKey);
	for(uiCnt=uiIndex=0; uiIndex<MAX_TRANLOG; uiIndex++)
	{
		if( gstSysCtrlParam.sAcqKeyList[uiIndex]!=gstCurAcq.ucKey )
		{
			continue;
		}
		uiCnt++;
		lcdDisplay(70, 0, DISP_CFONT|DISP_INVLINE, "%03d/%03d", uiCnt, uiTranNum);

		memset(&stLog, 0, sizeof(ST_TRAN_LOG));
		iRet = LoadTranLog(&stLog, uiIndex);
		if( iRet!=0 )
		{
			return iRet;
		}
		if( stLog.ucTranType==PREAUTH || stLog.ucTranType== AUTH || (stLog.uiStatus & TS_NOT_UPLOAD) )
		{
			continue;
		}
		if( (stLog.uiStatus & TS_VOID) && !ChkIfAmex() )
		{	// AMEX 需要upload void，其它acquirer不用
			continue;
		}

		SetCommReqField();

		memset(szBuffer,0,sizeof(szBuffer));
		memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szPan));
			sprintf((char *)gstSendPack.szPan,     "%.*s", LEN_PAN,      szBuffer);
		memset(szBuffer,0,sizeof(szBuffer));
		memset(szBuffer,'0',strlen((char *)gstTransInfo.stTranLog.szExpDate));
		sprintf((char *)gstSendPack.szExpDate, "%.*s", LEN_EXP_DATE, szBuffer);
		SetField57();

		sprintf((char *)gstSendPack.szProcCode, "%.*s", LEN_PROC_CODE,
				glTranConfig[stLog.ucTranType].szProcCode);
		gstSendPack.szProcCode[LEN_PROC_CODE-1] = '1';

		PubAscAdd((char *)stLog.szAmount, (char *)stLog.szTipAmount, 12, (char *)szTotalAmt);
		PubAddHeadChars((char *)szTotalAmt, 12, '0');
		sprintf((char *)gstSendPack.szTranAmt, "%.12s", szTotalAmt);
		if( memcmp(stLog.szTipAmount, "000000000000", 12)!=0 )
		{
			sprintf((char *)gstSendPack.szExtAmount, "%.12s", stLog.szTipAmount);
		}

		sprintf((char *)gstSendPack.szLocalTime, "%.6s", &stLog.szDateTime[8]);
		sprintf((char *)gstSendPack.szLocalDate, "%.4s", &stLog.szDateTime[4]);	
		SetEntryMode(&stLog);
		sprintf((char *)gstSendPack.szCondCode, "%.2s",  stLog.szCondCode);
		sprintf((char *)gstSendPack.szRRN,      "%.12s", stLog.szRRN);
		sprintf((char *)gstSendPack.szAuthCode, "%.6s",  stLog.szAuthCode);

		if( !ChkIfAmex() )
		{
			sprintf((char *)gstSendPack.szRspCode, "%.2s", stLog.szRspCode);
			if( stLog.ucEntryMode & MODE_CHIP_INPUT )
			{
				PubLong2Char((ulong)stLog.uiIccDataLen, 2, gstSendPack.sICCData);
				memcpy(&gstSendPack.sICCData[2], stLog.sIccData, stLog.uiIccDataLen);
			}
		}

		memcpy(gstSendPack.sField62, "\x00\x06", 2);
		sprintf((char *)&gstSendPack.sField62[2], "%06ld", stLog.ulInvoiceNo);

		PubLong2Char(22,2,gstSendPack.szField60);
		memcpy(gstSendPack.szField60+2, glTranConfig[stLog.ucTranType].szTxMsgID, 4);
		if( stLog.uiStatus & TS_OFFLINE_SEND )
		{
			memcpy(gstSendPack.szField60+2, "0220", 4);
		}
		else if( stLog.uiStatus & TS_VOID )
		{
			memcpy(gstSendPack.szField60+2, "0200", 4);
		}
		if (stLog.szRRN[0] != 0)
		{
			sprintf((char *)&gstSendPack.szField60[6], "%06ld%12s", stLog.ulSTAN, stLog.szRRN);
		}
		else
		{
			sprintf((char *)&gstSendPack.szField60[6], "%06ld%12s", stLog.ulSTAN, "");
		}

		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			return iRet;
		}
		DisplayResult(0);
	}

	return 0;
}


int  TransLogon(void)
{
	int   iRet, iDatalen;
	char  szCurTime[20], szDateTime[20];
	char  sTempBuff[64];
	iRet = TransInit(LOGON);
	if( iRet!=0 )
	{
		return iRet;
	}

	SetCurAcq(0);  // 设置

	lcdCls();
	DisplayTransName();
	iRet = PreDial();
	iRet = CheckPreDial(iRet);
	if (iRet != 0)
	{
		return iRet;
	}

	SetCommReqField();
	memset(gstSendPack.sField57,0,sizeof(gstSendPack.sField57));
	PubLong2Char(10,2,gstSendPack.sField57);
	PubLong2Char(0x03,2,gstSendPack.sField57+2);  
	memcpy(gstSendPack.sField57+4,gstSysParam.stEdcInfo.sTmkRefNumber,8);
	iRet = TranProcess();
	if (iRet != 0)
	{
		return iRet;
	}
#ifdef OFFLINE_TEST_VERSION
		memcpy(sTempBuff, "\x11\x11\x11\x11\x11\x11\x11\x11\x44\x44\x44\x44\x44\x44\x44\x44", 16);
		iRet = PedWritePinKey(PARITY_NONE,MASTER_KEY_ID,PIN_KEY_ID,16, sTempBuff);
	
		
	
		memcpy(sTempBuff, "\x11\x11\x11\x11\x11\x11\x11\x11\x22\x22\x22\x22\x22\x22\x22\x22", 16);
		iRet = PedWriteMacKey(PARITY_NONE,MASTER_KEY_ID,MAC_KEY_ID,16, sTempBuff);
	
		
		memcpy(sTempBuff, "\x11\x11\x11\x11\x11\x11\x11\x11\x33\x33\x33\x33\x33\x33\x33\x33", 16);
		iRet = PedWriteMacKey(PARITY_NONE,MASTER_KEY_ID,DATA_KEY_ID,16, sTempBuff);
#else

	iDatalen = PubChar2Long(gstRecvPack.sField57,2,NULL);
	if (iDatalen < 52)
	{
		lcdClrLine(2,7);
		PubDispString("Get WKEY error",3|DISP_LINE_CENTER);
		PubDispString("Less than 52Byte",5|DISP_LINE_CENTER);
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	iRet = PedWritePinKey(PARITY_NONE,MASTER_KEY_ID,PIN_KEY_ID,16,(uint8_t *)gstRecvPack.sField57+12);
	if (iRet != PED_RET_OK)
	{
		lcdClrLine(2,7);
		PubDispString("Save TPK error ",3|DISP_LINE_CENTER);
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	iRet = PedWriteMacKey(PARITY_NONE,MASTER_KEY_ID,MAC_KEY_ID,16,(uint8_t *)gstRecvPack.sField57+28);
	if (iRet != PED_RET_OK)
	{
		lcdClrLine(2,7);
		PubDispString("Save TPK error ",3|DISP_LINE_CENTER);
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	iRet = PedWriteMacKey(PARITY_NONE,MASTER_KEY_ID,DATA_KEY_ID,16,(uint8_t *)gstRecvPack.sField57+44);
	if (iRet != PED_RET_OK)
	{
		lcdClrLine(2,7);
		PubDispString("Save TPK error ",3|DISP_LINE_CENTER);
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}
#endif
	gstSysParam.uiAppStatus &= ~LOGON_STATUS;
	SaveAppTransParam();

	if (strlen(gstRecvPack.szLocalDate) != 0 && 
		strlen(gstRecvPack.szLocalTime) != 0)
	{
		memset(szCurTime,0,sizeof(szCurTime));
		memset(szDateTime,0,sizeof(szDateTime));
		GetDateTime((uint8_t *)szCurTime);
		sprintf(szDateTime, "%.4s%.4s%.6s",szCurTime, gstRecvPack.szLocalDate, gstRecvPack.szLocalTime);
		memset(szCurTime,0,sizeof(szCurTime));
		PubAsc2Bcd(szDateTime+2,12,szCurTime);
		sysSetTime(szCurTime);
	}

	lcdCls();
	DisplayTransName();
	PubBeepErr();
	lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Sign on OK");
	PubWaitKey(5);
	return 0;
}


int TransSendTC(uint8_t ucUploadAll)
{
	int				iRet;
	ST_TRANSINFO	stProcInfoBak;
	uint8_t	  ucOrgTranTypeBak;				// 原交易类型备份

	memcpy(&gstTransInfo.stSendPack, &gstSendPack, sizeof(ST_ISO8583));
	memcpy(&stProcInfoBak, &gstTransInfo,  sizeof(ST_TRANSINFO));
	while( 1 )
	{
		InitTransInfo();
		if( !GetNeedTCTrans() )
		{
			iRet = 0;
			break;
		}

		ucOrgTranTypeBak = gstTransInfo.stTranLog.ucOrgTranType;
		gstTransInfo.stTranLog.ucOrgTranType = gstTransInfo.stTranLog.ucTranType;
		gstTransInfo.stTranLog.ucTranType    = TC_SEND;
		DisplayTransName();

		SetCommReqField();

		sprintf((char *)gstSendPack.szLocalTime, "%.6s",  &gstTransInfo.stTranLog.szDateTime[8]);
		sprintf((char *)gstSendPack.szLocalDate, "%.4s",  &gstTransInfo.stTranLog.szDateTime[4]);
		// bit 37, 39
/*		sprintf((char *)gstSendPack.szRspCode, "%.2s", gstTransInfo.stTranLog.szRspCode);
		sprintf((char *)gstSendPack.szRRN,       "%.12s", gstTransInfo.stTranLog.szRRN);
		if (gstTransInfo.stTranLog.szAuthCode[0] != 0)
		{
			sprintf((char *)gstSendPack.szAuthCode,"%.6s", gstTransInfo.stTranLog.szAuthCode);
		}*/
		memset(gstSendPack.szLocalDate,0,sizeof(gstSendPack.szLocalDate));
		memset(gstSendPack.szLocalTime,0,sizeof(gstSendPack.szLocalTime));
		memset(gstSendPack.szRspCode,0,sizeof(gstSendPack.szRspCode));
		memset(gstSendPack.szRRN,0,sizeof(gstSendPack.szRRN));
		memset(gstSendPack.szAuthCode,0,sizeof(gstSendPack.szAuthCode));

		// bit 55
		PubLong2Char((ulong)gstTransInfo.stTranLog.uiIccDataLen, 2, gstSendPack.sICCData);
		memcpy(&gstSendPack.sICCData[2], gstTransInfo.stTranLog.sIccData, gstTransInfo.stTranLog.uiIccDataLen);

		// bit 60
		PubLong2Char(22,2,gstSendPack.szField60);
		if (gstTransInfo.stTranLog.ucOrgTranType == VOID)
		{// 如果交易被撤销了，则是上送原消费交易的消息类型和流水号
			sprintf((char *)gstSendPack.szField60+2, "%.4s%06lu%.12s", 
				glTranConfig[ucOrgTranTypeBak].szTxMsgID,gstTransInfo.stTranLog.ulOrgSTAN,gstTransInfo.stTranLog.szRRN);
		}
		else
		{
			sprintf((char *)gstSendPack.szField60+2, "%.4s%06lu%.12s", 
				glTranConfig[gstTransInfo.stTranLog.ucOrgTranType].szTxMsgID,gstTransInfo.stTranLog.ulSTAN,gstTransInfo.stTranLog.szRRN);
		}

		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			break;
		}

		// update txn status
		gstTransInfo.stTranLog.uiStatus &= ~TS_NEED_TC;
		gstTransInfo.stTranLog.uiStatus |= TS_ALREADY_TC; // 已经上送TC
		gstTransInfo.stTranLog.ucTranType = gstTransInfo.stTranLog.ucOrgTranType;
		gstTransInfo.stTranLog.ucOrgTranType = ucOrgTranTypeBak;
		UpdateTranLog(&gstTransInfo.stTranLog, gstTransInfo.uiRecNo);

//		if( stProcInfoBak.stTranLog.ucTranType!=SETTLEMENT )
		if (!ucUploadAll)
		{	// is trickle feed, only need send one txn
			break;
		}
	}
	memcpy(&gstTransInfo, &stProcInfoBak, sizeof(ST_TRANSINFO));
	memcpy(&gstSendPack, &gstTransInfo.stSendPack, sizeof(ST_ISO8583));

	return iRet;
}


int GetNeedTCTrans(void)
{
	int		iRet;
	UINT32	uiCnt;
	
	for(uiCnt=0; uiCnt<MAX_TRANLOG; uiCnt++)
	{
		if( gstSysCtrlParam.sAcqKeyList[uiCnt]!=gstCurAcq.ucKey )
		{
			continue;
		}
		
		memset(&gstTransInfo.stTranLog, 0, sizeof(ST_TRAN_LOG));
		iRet = LoadTranLog(&gstTransInfo.stTranLog, uiCnt);
		if( iRet!=0 )
		{
			return FALSE;
		}
		if( gstTransInfo.stTranLog.uiStatus & TS_NEED_TC )
		{
			gstTransInfo.uiRecNo = uiCnt;
			return TRUE;
		}
	}
	
	return FALSE;
}


int  TransPrnLastTrans(void)
{
	PrnLastTrans();
	return 0;
}


int  TransRePrnSpecTrans(void)
{
	RePrnSpecTrans();
	return 0;
}


void  SetParamForManager(void)
{
	PubDisplayTitle(TRUE, " SET PARAMETER  ");
	if (CheckTermPWD() != 0 )
	{
		return ;
	}

	PubDisplayTitle(TRUE, " SET PARAMETER  ");
	SetEdcParam();
	SetAcqParam();

	return ;
}


int  TransAutoSettlement(void)
{
	int		iRet;
	uint8_t	ucAcqIndex, szTitle[16+1];

	lcdClrLine(2,7);
	DisplayProcessing();

	CalcTotal(ACQ_ALL);  // 统计总计
	if( gstTransTotal.uiSaleCnt==0   && gstTransTotal.uiVoidSaleCnt==0 &&
		gstTransTotal.uiRefundCnt==0 && gstTransTotal.uiVoidRefundCnt==0 )
	{
		return 0;
	}

	TransInit(SETTLEMENT);
	lcdCls();
	DisplayTransName();

	for(ucAcqIndex=0; ucAcqIndex<gstSysParam.uiAcqNum; ucAcqIndex++)
	{
		SetCurAcq(ucAcqIndex);
		memcpy(&gstTransTotal, &gstAcqTotal[ucAcqIndex], sizeof(ST_TOTAL_INFO));
		iRet = TransSettleSub();
		if( iRet!=0 )
		{
			if( iRet!=ERR_NO_DISP )
			{
				return iRet;
			}
		}
	}

	return 0;
}

#ifdef _APP_SUPP_ELECSIGN  // 电子签名 
int  TransGetElecSignature(void)         // 电子签名
{
	char     szBuffer[1024],szTmp[30];
	int      iRet;
	ST_TS_ELEC_SIGNATURE  stElecSignaInfo;
	IDirectFBFont * font;
	
	gstTransInfo.stTranLog.ucElecSignatureflag=0;
	memset(gstTransInfo.stTranLog.szElecfilename,0,sizeof(gstTransInfo.stTranLog.szElecfilename));	
	memset(&stElecSignaInfo,0,sizeof(stElecSignaInfo));
	stElecSignaInfo.ullineTimes = 3;
	stElecSignaInfo.ulTimeout = gstSysParam.stEdcInfo.ulAcceptTimeout*1000;
	if (stElecSignaInfo.ulTimeout ==0){
		stElecSignaInfo.ulTimeout = 40000;
	}
	stElecSignaInfo.ucLanguage = LANGUAGE_ENG;           // 显示的语言
	stElecSignaInfo.ucImageType = IMAGE_JBG;          // 图片的格式
	strcpy(stElecSignaInfo.szPrompt,"Cardholder Signature:");
	memset(szBuffer,0,sizeof(szBuffer));
	memset(szBuffer,'0',16);

	if (strlen((char*)gstTransInfo.stTranLog.szRRN)){
		sprintf(szBuffer,"%4.4s%12.12s",gstTransInfo.stTranLog.szDateTime+4,gstTransInfo.stTranLog.szRRN);
	}
	else{
		sprintf(szBuffer,"%6.6s%06ld",gstTransInfo.stTranLog.szDateTime+4,gstTransInfo.stTranLog.ulInvoiceNo);
	}

	sprintf(stElecSignaInfo.szImagefileName,"%s/%s.jbg",STR_ELECSIGNATURE_FILE_PATH,szBuffer);
	PubAsc2Bcd(szBuffer,16,szTmp);
	PubXor(szTmp,szTmp+4,4);
	PubBcd2Asc0(szTmp,4,stElecSignaInfo.szTxnInfoCode);
	kbFlush();

	font = lcdGetFontExt(DISP_ASCII);
	PosTsSetFont(font,DISP_ASCII);
	PosTsSetFont(font,DISP_CFONT);
	iRet = GetElecSignature(&stElecSignaInfo);
	lcdCls();
	DisplayTransName();
	lcdFlip();
	if (iRet != 0)
	{
		return iRet;
	}
	
	if (fileSize(stElecSignaInfo.szImagefileName)>=1000)
	{
		return ERR_NO_DISP;
	}
	
	gstTransInfo.stTranLog.ucElecSignatureflag = TS_ELECSIGN_HAVE; // 有电子签名
	strcpy((char *)gstTransInfo.stTranLog.szElecfilename,stElecSignaInfo.szImagefileName);
	UpdateTranLog(&gstTransInfo.stTranLog, gstTransInfo.uiRecNo);
	
	return 0;
}

void   ClearElecSignaturefile(void)
{
	fileRemoveDir(STR_ELECSIGNATURE_FILE_PATH);
}
#endif
// end of file
