
#include "global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void DispPedErrMsg(int iErrCode);
/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/
// 初始化交易参数
void InitTransInfo(void)
{
	memset(&gstTransInfo, 0, sizeof(ST_TRANSINFO));
	gstTransInfo.uiRecNo     = 0xFFFF;
	gstTransInfo.bIsFirstGAC = TRUE;
	sprintf((char *)gstTransInfo.stTranLog.szTipAmount, "%012ld", 0L);

	gstTransInfo.stTranLog.ucEntryMode = MODE_NO_INPUT;
	gstTransInfo.stTranLog.bPanSeqOK   = FALSE;
	gstTransInfo.stTranLog.ulInvoiceNo = gstSysCtrlParam.ulInvoiceNo;
	GetDateTime(gstTransInfo.stTranLog.szDateTime);	// set default txn time

//	memcpy(&gstCurCommCfg, &gstSysParam.stTxnCommCfg, sizeof(ST_COMM_CONFIG));
}

// 获得新的流水号
ulong GetNewTraceNo(void)
{
	gstSysCtrlParam.ulSTAN++;
	if( !(gstSysCtrlParam.ulSTAN>0 && gstSysCtrlParam.ulSTAN<=999999L) )
	{
		gstSysCtrlParam.ulSTAN = 1L;
	}
	SaveSysCtrlBase();

	return (gstSysCtrlParam.ulSTAN);
}

// calculate new batch number
ulong GetNewBatchNo(ulong ulCurBatchNo)
{
	ulCurBatchNo++;
	if( !(ulCurBatchNo>0 && ulCurBatchNo<=999999L) )
	{
		return 1L;
	}

	return ulCurBatchNo;
}

// 获得新的票据号
ulong GetNewInvoiceNo(void)
{
	gstSysCtrlParam.ulInvoiceNo++;
	if( !(gstSysCtrlParam.ulInvoiceNo>0 && gstSysCtrlParam.ulInvoiceNo<=999999L) )
	{
		gstSysCtrlParam.ulInvoiceNo = 1L;
	}
	SaveSysCtrlBase();

	return (gstSysCtrlParam.ulInvoiceNo);
}


void PrintOne(void)
{
	static uint8_t sPrinterLogo[137] =
	{
		0x04,
		0x00,0x20,
		0x0,0xf8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,
		0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf8,

		0x00,0x20,
		0x0,0xff,0x0,0x0,0x0,0x0,0x80,0x40,0x20,0x70,0x4c,0x44,0x54,0x54,0x54,0x54,
		0x54,0x54,0x44,0x44,0x44,0x74,0x2c,0x20,0xa0,0xe0,0xe0,0x0,0x0,0x0,0x0,0xff,

		0x00,0x20,
		0x0,0xff,0x0,0x0,0x0,0x3f,0x21,0xe1,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,
		0x21,0x21,0x21,0x25,0x25,0xe1,0xe1,0x7f,0x1f,0xf,0x7,0x0,0x0,0x0,0x0,0xff,

		0x00,0x20,
		0x0,0xf,0x8,0x8,0x8,0x8,0x8,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,
		0x9,0x9,0x9,0x9,0x9,0x9,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf
	};

	lcdGoto(86, 3);
	lcdDrLogo(86,3,sPrinterLogo);
}

// 根据Issuer Key设置gstCurIssuer
void FindIssuer(uint8_t ucIssuerKey)
{
	uint8_t	ucIndex;

	for(ucIndex=0; ucIndex<gstSysParam.uiIssuerNum; ucIndex++)
	{
		if( gstSysParam.stIssuerList[ucIndex].ucKey==ucIssuerKey )
		{
			SetCurIssuer(ucIndex);
			gstTransInfo.stTranLog.ucIssuerKey = ucIssuerKey;
			break;
		}
	}
}

// 根据Key设置gstCurAcq
void FindAcq(uint8_t ucAcqKey)
{
	uint8_t	ucIndex;

	for(ucIndex=0; ucIndex<gstSysParam.uiAcqNum; ucIndex++)
	{
		if( gstSysParam.stAcqList[ucIndex].ucKey==ucAcqKey )
		{
			SetCurAcq(ucIndex);
			gstTransInfo.stTranLog.ucAcqKey = ucAcqKey;
			break;
		}
	}
}

// 设置当前收单行信息
void SetCurAcq(uint8_t ucAcqIndex)
{
	memcpy(&gstCurAcq, &gstSysParam.stAcqList[ucAcqIndex], sizeof(ST_ACQUIRER_INFO));
	gstCurAcq.ucIndex = ucAcqIndex;
}

// 设置当前发卡行信息
void SetCurIssuer(uint8_t ucIssuerIndex)
{
	memcpy(&gstCurIssuer, &gstSysParam.stIssuerList[ucIssuerIndex], sizeof(ST_ISSUER_INFO));
}


// 获取终端当前时间,格式:YYYYMMDDhhmmss
void GetDateTime(uint8_t *pszDateTime)
{
	uint8_t	sCurTime[7];

	sysGetTime(sCurTime);
	sprintf((char *)pszDateTime, "%02X%02X%02X%02X%02X%02X%02X",
			0x20, sCurTime[0], sCurTime[1],
			sCurTime[2], sCurTime[3], sCurTime[4], sCurTime[5]);
}

// 获取当前系统英文日期时间(16 bytes, eg: "OCT07,2008 11:22")
void   GetCurTime(char *pszCurTime)
{
	char	szEngName[13][4] = {"   ","JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	UINT8	sCurTime[7+1],ucTemp;
	INT32   index;

	if (pszCurTime==NULL)
	{
		return;
	}

	memset(sCurTime,0,sizeof(sCurTime));
	sysGetTime(sCurTime);

 	ucTemp = sCurTime[1];
 	index = (ucTemp>>4) * 10 ;
 	ucTemp = sCurTime[1];
 	index += (ucTemp & 0x0F);
 	index %= 13;

	sprintf((char *)pszCurTime, "%.3s%02X,%02X%02X %02X:%02X", szEngName[index],
			sCurTime[2], 0x20,
			sCurTime[0], sCurTime[3], sCurTime[4]);
}

// 转换YYYYMMDDhhmmss 到 OCT 07, 2008  11:22
void ChangeToEngTime(char *pszDateTime, char *pszEngTime)
{
	char	szEngName[13][4] = {"   ","JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	ulong	ulindex = 0;

	PubAsc2Long(&pszDateTime[4],2, &ulindex);
	ulindex = ulindex % 13;
	sprintf((char *)pszEngTime, "%3.3s %2.2s, %4.4s  %2.2s:%2.2s", szEngName[ulindex],
			pszDateTime+6, pszDateTime, pszDateTime+8, pszDateTime+10);
}

void ConvIssuerName(uint8_t *pszOrgName, uint8_t *pszOutName)
{
	char	*p;

	sprintf((char *)pszOutName, "%.10s", pszOrgName);
	p = strchr((char *)pszOutName, '_');
	if( p!=NULL )
	{
		*p = 0;
	}
}


// 输入金额及小费
INT32 GetAmount(void)
{
	INT32		iRet;
	uint8_t	szTotalAmt[12+1];

	if( gstTransInfo.stTranLog.szAmount[0]!=0 )
	{
		return 0;
	}

	while( 1 )
	{
		iRet = InputAmount(AMOUNT);
		if( iRet!=0 )
		{
			return iRet;
		}

		iRet = GetTipAmount();
		if( iRet!=0 )
		{
			return iRet;
		}

		PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, (char *)szTotalAmt);
		if( ConfirmAmount(szTotalAmt) )
		{
			break;
		}
	}

	if( !AllowDuplicateTran() )
	{
		return ERR_USERCANCEL;
	}

	return 0;
}

// 输入交易金额
int InputAmount(uint8_t ucAmtType)
{
	uint8_t	ucRet, ucMinLen, ucMaxLen, ucFlag, szCurrName[4+1];
	uint8_t	*pszAmt;

	PubASSERT( ucAmtType==AMOUNT    || ucAmtType==RFDAMOUNT ||
			   ucAmtType==ORGAMOUNT || ucAmtType==TIPAMOUNT);

	switch( ucAmtType )
	{
	case AMOUNT:
		if( ChkIfNeedTip() )
		{
			Display2StringHigh("请输入金额", "BASE AMOUNT ?");
		}
		else
		{
			Display2StringHigh("请输入金额", "ENTER AMOUNT ?");
		}
		break;

	case RFDAMOUNT:	// RFU
		Display2StringHigh("请输入退款金额", "ENTER AMOUNT ?");
		break;

	case ORGAMOUNT:	// RFU
		Display2StringHigh("请输入原金额", "ENTER AMOUNT ?");
		break;

	case TIPAMOUNT:
		Display2StringHigh("请输入小费", "ENTER TIP ?");
		break;
	}

	sprintf((char *)szCurrName, "%.3s ", gstSysParam.stEdcInfo.szCurrencyName);
// 	sprintf((char *)szCurrName, "%.3s%1.1s", gstSysParam.stEdcInfo.szCurrencyName, &gstSysParam.stEdcInfo.ChCurrencySymbol);
	ucMinLen = (ucAmtType==TIPAMOUNT) ? 0 : 1;
	ucMaxLen = min(gstSysParam.stEdcInfo.uiTranAmtLen, (UINT32)12);
	ucFlag   = 0;
	if( gstTransInfo.stTranLog.ucTranType==REFUND || ucAmtType==RFDAMOUNT )
	{
		ucFlag = GA_NEGATIVE;
	}

	if( ucAmtType==TIPAMOUNT )
	{
		pszAmt = gstTransInfo.stTranLog.szTipAmount;
	}
	else
	{
		pszAmt = gstTransInfo.stTranLog.szAmount;
	}
	sysDelayMs(50);
	ucRet = PubGetAmount(szCurrName, 2, ucMinLen, ucMaxLen, pszAmt, USER_OPER_TIMEOUT, ucFlag);
	if( ucRet!=0 )
	{
		return ERR_USERCANCEL;
	}
	PubAddHeadChars((char *)pszAmt, 12, '0');

	return 0;
}

// 输入TIP金额
INT32 GetTipAmount(void)
{
	INT32		iRet;
	uint8_t	szTotalAmt[12+1];

	if( !ChkIfNeedTip() )
	{
		return 0;
	}

	while( 1 )
	{
		iRet = InputAmount(TIPAMOUNT);
		if( iRet!=0 )
		{
			return iRet;
		}

		PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, (char *)szTotalAmt);
		if( ValidAdjustAmount(gstTransInfo.stTranLog.szAmount, szTotalAmt) )
		{
			break;
		}
	}

	return 0;
}

// 检查调整金额是否合法(TRUE: 合法, FALSE: 非法)
// 金额必须为12数字
uint8_t ValidAdjustAmount(uint8_t *pszBaseAmt, uint8_t *pszTotalAmt)
{
	uint8_t	szMaxAmt[15+1], szAdjRate[3+1];

	if( memcmp(pszTotalAmt, pszBaseAmt, 12)<0 )
	{
		DisplayAdjustTitle();
		Display2Strings("金额输入错误", "AMOUNT ERROR");
		PubBeepErr();
		PubWaitKey(3);
		return FALSE;
	}

	if( gstCurIssuer.ucAdjustPercent==0 || memcmp(pszTotalAmt, pszBaseAmt, 12)==0 )
	{
		return TRUE;
	}

	sprintf((char *)szAdjRate, "%ld", (ulong)(gstCurIssuer.ucAdjustPercent+100));
	PubAscMul((char *)pszBaseAmt, (char *)szAdjRate, (char *)szMaxAmt);
	PubAddHeadChars((char *)szMaxAmt, 15, '0');

	if( memcmp(pszTotalAmt, &szMaxAmt[1], 12)>0 )	// 最后两位为小数,不进行比较
	{
		DisplayAdjustTitle();
		Display2Strings("小费额度超限", "TIP OVR LIMIT");
		PubBeepErr();
		PubWaitKey(3);
		return FALSE;
	}

	return TRUE;
}

// 确认金额界面处理
uint8_t ConfirmAmount(uint8_t *pszAmount)
{
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "TOTAL           ");
	else
		PubDisplayTitle(TRUE, "总额       TOTAL");
	DisplayAmount(2, pszAmount);
	if (gstSysParam.stEdcInfo.ucLanguage != EDC_LANGUGE_EN)
		PubDispString("是否正确",      DISP_LINE_LEFT|4);
	PubDispString("CORRECT ? Y/N", DISP_LINE_LEFT|6);
	return PubYesNo(USER_OPER_TIMEOUT);
}

// 输入PIN, ucFlag: bit 8, ICC online PIN
INT32 GetPIN(uint8_t ucFlag)
{
	int		iRet , iRemainTimes, iLastTimes;
	char	szPAN[16+1], szTotalAmt[12+1];
	char	ucAmtFlag, szBuff[25];

	if( !(ucFlag & 0x80) )
	{
 		if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
 		{
			return 0;
 		}
	}

	if( !ChkIssuerOption(ISSUER_EN_PIN) )
	{
		return 0;
	} 

	// 循环等待直到允许输入密钥
	iLastTimes = PedGetSensitiveTimer(TYPE_PIN_INTERVAL);
	iLastTimes /= 1000;
	if (iLastTimes)
	{
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Wait a Sec.(%dS)",iLastTimes);
			lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Wait for Input");
		}
		else
		{
			lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请 稍 候 (%dS)",iLastTimes);
			lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"请等待输入");
		}
		lcdFlip();
	}
	while(PedGetSensitiveTimer(TYPE_PIN_INTERVAL))
	{
		iRemainTimes = PedGetSensitiveTimer(TYPE_PIN_INTERVAL);
		iRemainTimes /= 1000;
		if (iRemainTimes < iLastTimes)
		{
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Wait a Sec.(%dS)",iLastTimes);
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Wait for Input");
			}
			else
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请 稍 候 (%dS)",iLastTimes);
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"请等待输入");
			}
			iLastTimes = iRemainTimes;
			lcdFlip();
		}
	}

	iRet = ExtractPAN(gstTransInfo.stTranLog.szPan, (uint8_t *)szPAN);
	if( iRet!=0 )
	{
		return iRet;
	}

	PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, szTotalAmt);

	ucFlag &= 0x7F;
	lcdClrLine(2, 7);
	DisplayAmount(2, (uint8_t *)szTotalAmt);
	printf("454ucFlag=%d\n",ucFlag);
	if( ucFlag==0 )
	{
	
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("PLS ENTER PIN", 4|DISP_LINE_CENTER);
		else
			PubDispString("请输入密码",    4|DISP_LINE_CENTER);
	}
	else
	{
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("PIN ERR, RETRY", 4|DISP_LINE_CENTER);
		else
			PubDispString("密码错，请重输", 4|DISP_LINE_CENTER);
	}
	printf("470\n");
	lcdFlip();
	ucAmtFlag = 0;
	if( gstTransInfo.stTranLog.ucTranType==VOID || gstTransInfo.stTranLog.ucTranType==REFUND )
	{
		ucAmtFlag |= GA_NEGATIVE;
	}
	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, (uint8_t *)szTotalAmt, 2, (uint8_t *)szBuff, ucFlag);
	// show amount on PINPAD

	memset(gstTransInfo.sPinBlock, 0, sizeof(gstTransInfo.sPinBlock));
	lcdGoto(60,6);
	iRet = PedGetPin(PIN_KEY_ID,(uint8_t *)"0,4,6",(uint8_t *)szPAN,FORMAT_0,gstTransInfo.sPinBlock);
	printf("483=%d",iRet);
	if( iRet==0 )
	{
		gstTransInfo.stTranLog.ucEntryMode |= MODE_PIN_INPUT;
		return 0;
	}
	else if( iRet== PED_RET_NOPIN )
	{
		return 0;	// PIN by pass
	}
	else if( iRet== PED_RET_CANCEL || iRet== PED_RET_TIMEOUT )
	{
		DispPedErrMsg(iRet);
		return ERR_NO_DISP;
	}

	

	return iRet;
}

// calculate mac
INT32 GetMAC(uint8_t *psData, UINT32 uiDataLen, uint8_t *psOutMAC)
{
	int	  iRet, index;
	char  szHashValue[40],szHashVal2[40];
	char  szBuffer[8];

	if( !ChkIfNeedMac() )
	{
		return 0;
	}

	memset(szHashValue,0,sizeof(szHashValue));
	memset(szHashVal2,0,sizeof(szHashVal2));
	Hash(psData,uiDataLen,szHashValue);
	PubBcd2Asc(szHashValue,20,szHashVal2);
	uiDataLen = 40;

	// 0: ANSI x9.9, 1: fast arithm
	if (gstTransInfo.stTranLog.ucTranType == LOGON)
	{
		index = 0;
		memset(szBuffer,0,sizeof(szBuffer));
		while(index<uiDataLen)
		{
			PubXor(szBuffer,&szHashVal2[index],8);
			iRet = PedTDEA(TDEA_ENCRYPT,TYPE_MASTERKEY,TAK_KEY_ID,8,(uint8_t *)szBuffer,psOutMAC);
			memcpy(szBuffer,psOutMAC,8);
			index += 8;
		}
	}
	else
	{
		iRet = PedGetMac(MAC_KEY_ID,(uint8_t *)szHashVal2, uiDataLen,  0x00, psOutMAC);
	}
	if( iRet!=0 )
	{
		DispPedErrMsg(iRet);
		return ERR_NO_DISP;
	}

	return 0;
}

// show PED/PINPAD error message
void DispPedErrMsg(int iErrCode)
{
	char   szBuffer[16+1];

	switch( iErrCode )
	{
	case PED_RET_COMMERR:
		Display2StringInRect("PED通讯错误", "CONNECT PED ERR");
		break;		
	case PED_RET_NEEDAUTH:
		Display2StringInRect("授权超时", "AUTH TIMEOUT");
		break;		
	case PED_RET_AUTHERR:
		Display2StringInRect("授权校验错误", "AUTH VERIFY ERR");
		break;
//	case PED_RET_APPTSSAKERR:
	case PED_RET_PEDTSSAKERR:
	case PED_RET_KEYINDEXERR:
	case PED_RET_NOKEY:
		Display2StringInRect("密钥索引非法", "INVALID KEY ID");
		break;		
	case PED_RET_NOPIN:
		Display2StringInRect("无密钥输入", "NO PIN INPUT");
		break;
	
	case PED_RET_CANCEL:
		Display2StringInRect("用户取消", "USER CANCEL");
		break;
		
	case PED_RET_TIMEOUT:
		Display2StringInRect("输入等待超时", "TIME OUT");
		break;
		
	default:
		lcdClrLine(2, 7);
		sprintf(szBuffer,"PED ERROR:%02X", iErrCode);
		Display2StringInRect("PED 错误", szBuffer);
		break;
	}
	PubBeepErr();
	PubWaitKey(3);
}


// 获取8583打包/解包错误信息
void Get8583ErrMsg(uint8_t bPackErr, INT32 iErrCode, uint8_t *pszErrMsg)
{
	PubASSERT( iErrCode<0 );
	if( bPackErr )
	{
		if( iErrCode<-2000 )
		{
			sprintf((char *)pszErrMsg, "BIT %d DEF ERR", -iErrCode-2000);
		}
		else if( iErrCode<-1000 )
		{
			sprintf((char *)pszErrMsg, "BIT %d PACK ERR", -iErrCode-1000);
		}
		else
		{
			sprintf((char *)pszErrMsg, "PACK HEADER ERR");
		}
	}
	else
	{
		if( iErrCode<-2000 )
		{
			sprintf((char *)pszErrMsg, "BIT %d DEF ERR", -iErrCode-2000);
		}
		else if( iErrCode<-1000 )
		{
			sprintf((char *)pszErrMsg, "UNPACK %d ERR", -iErrCode-1000);
		}
		else if( iErrCode==-1000 )
		{
			sprintf((char *)pszErrMsg, "DATA LENGTH ERR");
		}
		else
		{
			sprintf((char *)pszErrMsg, "UNPACK HEAD ERR");
		}
	}
}


// 获取预授权号码
INT32 GetPreAuthCode(void)
{
	uint8_t	ucRet, szBuff[LEN_AUTH_CODE+1];

	Display2StringHigh("授权号码", "APPV CODE ?");
	memset(szBuff, 0, sizeof(szBuff));
	ucRet = PubGetString(ALPHA_IN, 2, LEN_AUTH_CODE, szBuff, USER_OPER_TIMEOUT);
	if( ucRet!=0 )
	{
		return ERR_USERCANCEL;
	}

	sprintf((char *)gstTransInfo.stTranLog.szAuthCode, "%-*s", LEN_AUTH_CODE, szBuff);

	return 0;
}

// 获取票据号
INT32 InputInvoiceNo(ulong *pulInvoiceNo)
{
	uint8_t	ucRet, szBuff[LEN_INVOICE_NO+1];

	lcdClrLine(2, 7);
	Display2StringHigh("账 单 号", "TRACE NO ?");
// 	Display2StringHigh("账 单 号", "INVOICE NO ?");

	memset(szBuff, 0, sizeof(szBuff));
	ucRet = PubGetString(NUM_IN, 1, LEN_INVOICE_NO, szBuff, USER_OPER_TIMEOUT);
	if( ucRet!=0 )
	{
		return ERR_USERCANCEL;
	}
	*pulInvoiceNo = (ulong)atol((char *)szBuff);

	return 0;
}

// 获取要显示的交易状态信息
void GetStateText(UINT32 uiStatus, uint8_t *pszStaText)
{
	*pszStaText = 0;
	if( uiStatus & TS_VOID)
	{
		sprintf((char *)pszStaText, "VOIDED");
	}
	else if( uiStatus & TS_ADJ)
	{
		sprintf((char *)pszStaText, "ADJUSTED");
	}
	else if( uiStatus & TS_NOSEND)
	{
		sprintf((char *)pszStaText, "NOT_SEND");
	}
}


uint8_t AskYesNo(void)
{
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDispString("[ NO ]  [YES ]", DISP_LINE_CENTER|5);
	}
	else
	{
		PubDispString("[取消]  [确认]", DISP_LINE_CENTER|4);
		PubDispString("[ NO ]  [YES ]", DISP_LINE_CENTER|6);
	}
	return PubYesNo(USER_OPER_TIMEOUT);
}

// 选择收单行(for settle/reprint ....)
INT32 SelectAcq(uint8_t bAllowSelAll, uint8_t *pszTitle, uint8_t *pucAcqIndex)
{
	uint8_t		ucCnt, ucIndex;
	MenuItem	stAcqMenu[MAX_ACQ+1+1];
	INT32			iSelMenu, iMenuNum;

	if( gstSysParam.uiAcqNum==0 )
	{
		lcdCls();
		Display2StringInRect("无收单行信息", "NO ACQUIRER");
		PubBeepErr();
		PubWaitKey(3);
		return ERR_NO_DISP;
	}
	if( gstSysParam.uiAcqNum==1 )
	{
		SetCurAcq(0);
		if( pucAcqIndex!=NULL )
		{
			*pucAcqIndex = 0;
		}
		return 0;
	}

	// here, gstSysParam.ucAcqNum must >= 2
	memset(&stAcqMenu[0], 0, sizeof(stAcqMenu));
	iMenuNum = 0;
	if( bAllowSelAll )
	{
		stAcqMenu[iMenuNum].bVisible = TRUE;
		sprintf((char *)stAcqMenu[iMenuNum].szMenuName, "SETTLE ALL");
		iMenuNum++;
	}

	for(ucCnt=0; ucCnt<gstSysParam.uiAcqNum; ucCnt++)
	{
		stAcqMenu[iMenuNum].bVisible = TRUE;
		stAcqMenu[iMenuNum].pfMenuFunc = NULL;
		sprintf((char *)stAcqMenu[iMenuNum].szMenuName, "%3.3s %.10s",
				gstSysParam.stAcqList[ucCnt].szNii, gstSysParam.stAcqList[ucCnt].szName);
		iMenuNum++;
	}
	stAcqMenu[iMenuNum].bVisible = FALSE;
	stAcqMenu[iMenuNum].pfMenuFunc = NULL;
	stAcqMenu[iMenuNum].szMenuName[0] = 0;

	iSelMenu = PubGetMenu(pszTitle, stAcqMenu, MENU_AUTOSNO|MENU_ASCII, USER_OPER_TIMEOUT);
	if( iSelMenu<0 )
	{
		return ERR_USERCANCEL;
	}
	if( bAllowSelAll )
	{
		ucIndex = (uint8_t)((iSelMenu==0) ? MAX_ACQ : iSelMenu-1);
	}
	else
	{
		ucIndex = (uint8_t)iSelMenu;
	}

	if( ucIndex!=MAX_ACQ )
	{
		SetCurAcq(ucIndex);
	}
	if( pucAcqIndex!=NULL )
	{
		*pucAcqIndex = ucIndex;
	}

	return 0;
}

// 选择发卡行
INT32 SelectIssuer(uint8_t *pucIssuerIndex)
{
	uint8_t		ucCnt, szTitle[16+1];
	INT32			iMenuNo;
	MenuItem	stMenuIssuer[MAX_ISSUER+1];

	if( gstSysParam.uiIssuerNum==0 )
	{
		lcdCls();
		Display2StringInRect("无发卡行信息", "NO ISSUER");
		PubBeepErr();
		PubWaitKey(3);
		return ERR_NO_DISP;
	}
	if( gstSysParam.uiIssuerNum==1 )
	{
		SetCurIssuer(0);
		if( pucIssuerIndex!=NULL )
		{
			*pucIssuerIndex = 0;
		}
		return 0;
	}

	memset((char*)stMenuIssuer, 0, sizeof(stMenuIssuer));
	for(ucCnt=0; ucCnt<gstSysParam.uiIssuerNum; ucCnt++)
	{
		stMenuIssuer[ucCnt].bVisible = TRUE;
		stMenuIssuer[ucCnt].pfMenuFunc = NULL;
		sprintf((char *)stMenuIssuer[ucCnt].szMenuName, "%.10s", gstSysParam.stIssuerList[ucCnt].szName);
	}
	stMenuIssuer[ucCnt].bVisible = FALSE;
	stMenuIssuer[ucCnt].pfMenuFunc = NULL;
	memset(stMenuIssuer[ucCnt].szMenuName,0,sizeof(stMenuIssuer[ucCnt].szMenuName));

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szTitle, "%.16s", gstSysParam.uiIssuerNum>9 ? "SELECT ISSUER:" : "SELECT ISSUER");
	else
		sprintf((char *)szTitle, "%.16s", gstSysParam.uiIssuerNum>9 ? "选择收单行:" : "选择收单行");
	iMenuNo = PubGetMenu(szTitle, stMenuIssuer, MENU_ASCII|MENU_AUTOSNO, USER_OPER_TIMEOUT);
	if(!(iMenuNo>=0 && iMenuNo < ucCnt))
	{
		return ERR_USERCANCEL;
	}
	SetCurIssuer((uint8_t)iMenuNo);
	if( pucIssuerIndex!=NULL )
	{
		*pucIssuerIndex = (uint8_t)iMenuNo;
	}

	return 0;
}

// 清除汇总信息
void ClearTotalInfo(void *pstTotalInfo)
{
	memset(pstTotalInfo, 0, sizeof(ST_TOTAL_INFO));

	memset(((ST_TOTAL_INFO *)pstTotalInfo)->szSaleAmt,       '0', 12);
	memset(((ST_TOTAL_INFO *)pstTotalInfo)->szTipAmt,        '0', 12);
	memset(((ST_TOTAL_INFO *)pstTotalInfo)->szRefundAmt,     '0', 12);
	memset(((ST_TOTAL_INFO *)pstTotalInfo)->szVoidSaleAmt,   '0', 12);
	memset(((ST_TOTAL_INFO *)pstTotalInfo)->szVoidRefundAmt, '0', 12);
	memset(((ST_TOTAL_INFO *)pstTotalInfo)->szTcSendAmt, '0', 12);
}

// 判断是否为数字串
uint8_t IsNumStr(char *pszStr)
{
	if( pszStr==NULL || *pszStr==0 )
	{
		return FALSE;
	}

	while( *pszStr )
	{
		if( !isdigit(*pszStr++) )
		{
			return FALSE;
		}
	}

	return TRUE;
}

// 获取交易的英文名称
void GetEngTranLabel(uint8_t *pszTranTitle, uint8_t *pszEngLabel)
{
	while( *pszTranTitle )
	{
		if( isalpha(*pszTranTitle) || *pszTranTitle=='-' )
		{
			*pszEngLabel++ = *pszTranTitle;
		}
		pszTranTitle++;
	}
	*pszEngLabel = 0;
}

// 获取交易的中文名称
void GetChnTranLabel(uint8_t *pszTranTitle, uint8_t *pszChnLabel)
{
	while( *pszTranTitle )
	{
		if( isalpha(*pszTranTitle) || isspace(*pszTranTitle) )
		{
			break;
		}
		*pszChnLabel++ = *pszTranTitle++;
	}
	*pszChnLabel = 0;
}

// 取得金额的符号
uint8_t GetTranAmountInfo(void *pTranLog)
{
	uint8_t		ucSignChar;
	ST_TRAN_LOG	*pstLog;

	pstLog = (ST_TRAN_LOG *)pTranLog;
	ucSignChar = 0;
	if( pstLog->ucTranType==REFUND || pstLog->ucTranType==VOID || pstLog->uiStatus &TS_VOID )
	{
		ucSignChar = GA_NEGATIVE;
	}

	if( (pstLog->ucTranType==VOID) && (pstLog->ucOrgTranType==REFUND) )
	{
		ucSignChar = 0;
	}

	if( (pstLog->ucTranType==REFUND) && ( pstLog->uiStatus &TS_VOID))
	{
		ucSignChar = 0;
	}

	return ucSignChar;
}


// if any issuer ticked PIN REQUIRED option, it open the EMV offline PIN
void ModifyTermCapForPIN(void)
{
/*	uint8_t	ucCnt, bNeedPIN;

	bNeedPIN = FALSE;
	for(ucCnt=0; ucCnt<gstSysParam.uiIssuerNum; ucCnt++)
	{
		if( ChkOptionExt(gstSysParam.stIssuerList[ucCnt].sOption, ISSUER_EN_PIN) )
		{
			bNeedPIN = TRUE;
			break;
		}
	}

	emvGetMCKConfig(&glEmvParam);
	if( bNeedPIN )	// mostly for AMEX
	{
		memcpy(glEmvParam.tTmCap, "\xE0\xB8\xC8", 3);
	}
	else
	{
		memcpy(glEmvParam.tTmCap, "\xE0\x20\xC8", 3);
	}
	emvSetMCKConfig(glEmvParam);  */
}

// set tag 9C for EMV
int UpdateEMVTranType(void)
{
	// 设置EMV参数
	int   iRet;

	iRet = emv_get_core_init_parameter(&gstEmvCoreParam);
	if (iRet != EMV_RET_SUCCESS)
	{
		return ERR_TRAN_FAIL;
	}

	switch( gstTransInfo.stTranLog.ucTranType )
	{
	case SALE:
		gstEmvCoreParam.transaction_type = EMV_GOODS;
//		gstEmvCoreParam.transaction_type = EMV_CASHBACK;
		emv_set_data_element((uint8_t *)"\x9C",1, 1, (uint8_t *)"\x00");
		break;

	case AUTH:
		emv_set_data_element((uint8_t *)"\x9C",1, 1, (uint8_t *)"\x00");
		gstEmvCoreParam.transaction_type = EMV_GOODS;
//		gstEmvCoreParam.transaction_type = EMV_CASHBACK;
		break;

	case PREAUTH:
		emv_set_data_element((uint8_t *)"\x9C",1, 1, (uint8_t *)"\x30");
		gstEmvCoreParam.transaction_type = EMV_GOODS;
//		gstEmvCoreParam.transaction_type = EMV_CASHBACK;
		break;

// 	case BALANCE:
// 		emv_set_data_element("\x9C",1, 1, (uint8_t *)"\x31");
//		gstEmvCoreParam.transaction_type = EMV_GOODS;
// 		break;

	default:
		return EMV_RET_SUCCESS;
		break;
	}	

	iRet = emv_set_core_init_parameter(&gstEmvCoreParam);
	if (iRet != EMV_RET_SUCCESS)
	{
		return ERR_TRAN_FAIL;
	}

/*	if (gstTransInfo.stTranLog.szPan[0] == '5')
	{
		emvSetKernelType(KERNEL_TYPE_MASTERCARD);
	}*/

	return EMV_RET_SUCCESS;
}

// end of file
