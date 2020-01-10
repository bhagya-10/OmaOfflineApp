
#include "global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal variables declaration *********************/
/********************** Internal functions declaration *********************/
static INT32  MatchCardTable(uint8_t *pszPAN);
static INT32  SelectAcqForCard(uint8_t *psAcqMatchFlag, uint8_t *pucCardIndex);
static void   GetHolderNameFromTrack1(uint8_t *pszHolderName);
static void   ConvertHolderName(uint8_t *pszOrgName, uint8_t *pszNormalName);
static INT32  GetEmvTrackData(void);
static INT32  GetPanFromTrack(uint8_t *pszPAN, uint8_t *pszExpDate);
static INT32  MatchTrack2AndPan(uint8_t *pszTrack2, uint8_t *pszPan);
static INT32  MatchCardBin(uint8_t *pszPAN);
static INT32  DetectCardEvent(uint8_t ucMode);
static INT32  GetExpiry(void);
static int    GetSecurityCode(void);
/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

/************************************************************************
* 刷卡事件处理函数
* bCheckICC:    TRUE  检查2磁道的service code(对EMV终端有效)
*               FALSE 不检查
************************************************************************/
INT32 SwipeCardProc(uint8_t bCheckICC)
{
	INT32		iRet;

	iRet = ReadMagCardInfo();
	if( iRet!=0 )
	{
		return iRet;
	}

	// 交易不要求判断卡片类型或者为非EMV终端,直接返回
	if( !bCheckICC || !ChkIfEmvEnable() )
	{
		return 0;
	}

	// EMV终端,继续检查
	if( gstTransInfo.bIsFallBack==TRUE )
	{
		if( IsChipCard((uchar*)gstTransInfo.stTranLog.szTrack2) )
		{	// fallback并且是IC卡,则返回成功
			gstTransInfo.stTranLog.ucEntryMode = MODE_FALLBACK_SWIPE;
			return 0;
		}
		else
		{
			DisplayTransName();
			Display2Strings("非EMV卡,请重刷", "NON EMV,RE-SWIPE");
			PubBeepErr();
			PubWaitKey(3);
			return ERR_NO_DISP;
		}
	}
	else if( IsChipCard((uchar*)gstTransInfo.stTranLog.szTrack2) )
	{
		if( gstTransInfo.stTranLog.ucTranType!=OFF_SALE &&
			gstTransInfo.stTranLog.ucTranType!=REFUND   &&
			gstTransInfo.stTranLog.ucInstalment==0 )
		{
			DisplayTransName();
			DisplayInsertCard();
			PubBeepErr();
			PubWaitKey(3);
			return ERR_NEED_INSERT;
		}
	}

	return 0;
}

// ICC插卡事件处理函数
INT32 InsertCardProc(void)
{
	int		iRet;
	char    szTotalAmt[12+1], sTemp[6];
	uint32_t    uiLen;
	char        Atr[500];
//	emvSetKernelType(KERNEL_TYPE_EMV); //设置成emv类型

	// 如果已经FALLBACK,忽略IC卡插卡操作
	if( gstTransInfo.bIsFallBack==TRUE )
	{
		return ERR_NEED_FALLBACK;
	}
	ModifyTermCapForPIN();

	gstTransInfo.stTranLog.ucEntryMode = MODE_CHIP_INPUT;

	DisplayTransName();
	DisplayProcessing();

	iccSetPara(ICC_USERCARD,ICC_VOL_5V,ICC_ISO);
	iRet = iccPowerUp(ICC_USERCARD, &uiLen, Atr);
	if( iRet )
	{

		return ERR_NEED_FALLBACK;		
	}
	
	icc_fd = iccGetSlotFd(ICC_USERCARD);//取句柄
	__ICCARD_EXCHANGE_APDU = iccCardExchangeAPDU;

	// 应用选择
	iRet = emv_application_select(icc_fd, gstSysCtrlParam.ulSTAN);
	iRet = iRet?errno:0;
	DisplayTransName();
	if( iRet==EMV_ERRNO_NO_APP || iRet==EMV_ERRNO_DATA || iRet == EMV_ERRNO_SW )
	{
		gstTransInfo.bIsFallBack = TRUE;
		gstTransInfo.iFallbackErrCode = iRet;
		return ERR_NEED_FALLBACK;
	}
	if( iRet==EMV_ERRNO_CANCEL )
	{
		return ERR_USERCANCEL;
	}
	if( iRet!=EMV_RET_SUCCESS )
	{
		return ERR_TRAN_FAIL;
	}

	// Read Track 2 and/or Pan
	lcdCls();
	DisplayTransName();
	iRet = GetEmvTrackData();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = ValidCard();	// 校验卡号
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = UpdateEMVTranType();
	if(iRet != EMV_RET_SUCCESS)
	{
		lcdClrLine(2,7);
		Display2StringInRect("POS处理失败","TRY IT AGAIN");
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	// 输入交易金额
	if(gstTransInfo.stTranLog.ucTranType!=VOID )
	{
		sysDelayMs(50);
		iRet = GetAmount();
		if( iRet!=0 )
		{
			return ERR_USERCANCEL;
		}
		PubAscAdd((char*)gstTransInfo.stTranLog.szAmount, (char*)gstTransInfo.stTranLog.szTipAmount, 12, szTotalAmt);
		PubAddHeadChars(szTotalAmt, 12, '0');
		PubAsc2Bcd(szTotalAmt, 12, sTemp);
		emv_set_data_element((uchar*)"\x9F\x02",2, 6, (uchar*)sTemp);
		PubLong2Char((ulong)atol((char *)szTotalAmt), 4, sTemp);
		emv_set_data_element((uchar*)"\x81",1, 4, (uchar*)sTemp);
		memset(sTemp,0,sizeof(sTemp));
//		PubAsc2Bcd(gstTransInfo.stTranLog.szTipAmount,12,sTemp);
		emv_set_data_element((uint8_t *)"\x9F\x03",2, 6, (uint8_t *)sTemp);
	}

	DisplayTransName();
	DisplayProcessing();
	// 卡片数据认证
	if( gstTransInfo.stTranLog.ucTranType==SALE ||
		gstTransInfo.stTranLog.ucTranType==AUTH ||
		gstTransInfo.stTranLog.ucTranType==PREAUTH )
	{
		iRet = emv_offline_data_authentication(icc_fd);

		if( iRet!=EMV_RET_SUCCESS )
		{
			lcdClrLine(2,7);
			Display2StringInRect("脱机数据认证失败","DECLINED");
			SaveTVRTSI(TRUE);
			PubWaitKey(5);
			return ERR_NO_DISP;
		}
	}

	return 0;
}


// 根据参数进行,刷卡/插卡/输入卡号
// ucMode: bit 8: 1=skipping check track 2 service code, 0=check
//         bit 7: 1=fallback swipe
//         bit 6: 1=skip detect ICC
INT32 GetCardInfo(UINT32 ucMode)
{
	INT32		iRet, iEventID;
	uint8_t	bCheckICC, ucKey;
	
	if( (gstTransInfo.stTranLog.ucEntryMode & 0x0F)!=MODE_NO_INPUT )
	{
		return 0;
	}
	
	if( ucMode & FALLBACK_SWIPE )
	{
		ucMode &= ~(SKIP_CHECK_ICC|CARD_INSERTED);	// clear bit 8, force to check service code
	}
	
	bCheckICC = !(ucMode & SKIP_CHECK_ICC);
	while( 1 )
	{
		iEventID = DetectCardEvent(ucMode);
		if( iEventID==CARD_KEYIN )
		{
			ucKey = kbGetKey();
			if( ucKey==KEY_CANCEL )
			{
				return ERR_USERCANCEL;
			}
			if( (ucMode & CARD_KEYIN) && ucKey>='0' && ucKey<='9' )
			{
				return ManualInputPan(ucKey);
			}
		}
		else if( iEventID==CARD_SWIPED )
		{
			iRet = SwipeCardProc(bCheckICC);
			if( iRet==0 )
			{
				return ValidCard();
			}
			else if( iRet==ERR_SWIPECARD )
			{
				DispGetMagInfoErr();
				PubBeepErr();
				PubWaitKey(3);
			}
			else if( iRet==ERR_NEED_INSERT )	// 是芯片卡
			{
				if( !(ucMode & CARD_INSERTED) )
				{	// 本身交易不允许插卡
					return iRet;
				}
				ucMode &= ~CARD_SWIPED;			// 去掉刷卡检查
			}
			else
			{
				return iRet;
			}
		}
		else if( iEventID==CARD_INSERTED )
		{
			iRet = InsertCardProc();
			if( iRet==0 )
			{
				return 0;
			}
			else if( iRet==ERR_NEED_FALLBACK )
			{
				PromptFallBack();
				PromptRemoveICC();
				ucMode = CARD_SWIPED|FALLBACK_SWIPE;
#ifdef JAY_TEST	
				gstTransInfo.bIsFallBack = TRUE;
				gstTransInfo.iFallbackErrCode = iRet;
#endif
			}
			else if( iRet==ERR_TRAN_FAIL )
			{
				Display2StringInRect("交易拒绝", "NOT ACCEPTED");
				PubBeepErr();
				PubWaitKey(3);
				PromptRemoveICC();
				return ERR_NO_DISP;
			}
			else
			{
				PromptRemoveICC();
				return iRet;
			}
		}
	}
}


// 手工输入PAN及其相关信息
INT32 ManualInputPan(uint8_t ucInitChar)
{
	INT32		iRet;
	
	Display2StringHigh("请按入账号", "ENTER ACCOUNT NO");
	
	memset(gstTransInfo.stTranLog.szPan, 0, sizeof(gstTransInfo.stTranLog.szPan));
	if( ucInitChar>='0' && ucInitChar<='9' )
	{
		gstTransInfo.stTranLog.szPan[0] = ucInitChar;
	}
	if( PubGetString(NUM_IN|CARRY_IN, 13, LEN_PAN, gstTransInfo.stTranLog.szPan, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_NO_DISP;
	}
	gstTransInfo.stTranLog.ucEntryMode = MODE_MANUAL_INPUT;
	
	DisplayTransName();
	iRet = MatchCardTable(gstTransInfo.stTranLog.szPan);
	if( iRet!=0 )
	{
		Display2StringInRect("不接纳此卡", "UNSUPPORTED CARD");
		PubBeepErr();
		PubWaitKey(3);
		return ERR_NO_DISP;
	}
	
	if( !ChkIssuerOption(ISSUER_EN_MANUL) )
	{
//		Display2StringInRect("不能手工输入", "NOT ALLOW MANUL");
//		PubWaitKey(3);
		DisplayBlockFunc();
		return ERR_NO_DISP;
	}
	
	iRet = ValidPanNo(gstTransInfo.stTranLog.szPan);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = GetExpiry();
	if( iRet!=0 )
	{
		return iRet;
	}
	
	CheckCapture();
	iRet = ConfirmPanInfo();
	if( iRet!=0 )
	{
		return iRet;
	}	

	iRet = GetSecurityCode();
	if( iRet!=0 )
	{
		return iRet;
	}
	
	return 0;
}

// 读取磁卡磁道及PAN
INT32 ReadMagCardInfo(void)
{
	INT32		iRet;

	if( glstMsgInfo.iMessage==MAGCARD_EVENT )
	{
		gstTransInfo.stTranLog.ucEntryMode = MODE_SWIPE_INPUT;
		sprintf((char *)gstTransInfo.szTrack1, "%.*s", LEN_TRACK1, glstMsgInfo.stMagInfo.szTrack1);
		sprintf((char *)gstTransInfo.stTranLog.szTrack2, "%.*s", LEN_TRACK2, glstMsgInfo.stMagInfo.szTrack2);
		sprintf((char *)gstTransInfo.szTrack3, "%.*s", LEN_TRACK3, glstMsgInfo.stMagInfo.szTrack3);
	}
	else
	{
		gstTransInfo.stTranLog.ucEntryMode = MODE_SWIPE_INPUT;
		magRead((uint8_t *)gstTransInfo.szTrack1, (uint8_t *)gstTransInfo.stTranLog.szTrack2, (uint8_t *)gstTransInfo.szTrack3);
	}

	iRet = GetPanFromTrack(gstTransInfo.stTranLog.szPan, gstTransInfo.stTranLog.szExpDate);
	if( iRet!=0 )
	{
		DispGetMagInfoErr();
		PubBeepErr();
		PubWaitKey(3);
		return ERR_NO_DISP;
	}
	sysBeep();

	return 0;
}


// 检测磁道信息是否为IC卡磁道信息
INT32 IsChipCard(uint8_t *pszTrack2)
{
	char	*pszSeperator;

	if( *pszTrack2==0 )
	{
		return FALSE;
	}

	pszSeperator = strchr((char *)pszTrack2, '=');
	if( pszSeperator==NULL )
	{
		return FALSE;
	}
	if( (pszSeperator[5]=='2') || (pszSeperator[5]=='6') )
	{
		return TRUE;
	}

	return FALSE;
}


// 检查卡号,并确定收单行/发卡行(必须在读出卡号后调用)
INT32 ValidCard(void)
{
	INT32		iRet;
	
	iRet = MatchCardTable(gstTransInfo.stTranLog.szPan);
	if( iRet!=0 )
	{
		Display2StringInRect("不接纳此卡", "UNSUPPORTED CARD");
		PubBeepErr();
		PubWaitKey(3);
		return ERR_NO_DISP;
	}

	if(!(gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT))
	{
		if( !ChkIssuerOption(ISSUER_EN_SWIPE) )
		{
			Display2StringInRect("不支持刷卡", "NOT ALLOW SWIPE");
			PubWaitKey(3);
			return ERR_NO_DISP;
		}
	}

	if( !ChkAcqOption(ACQ_EMV_FEATURE) )
	{	// 非EMV Acquirer不允许Fallback及插卡
		if( (gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT) ||
			(gstTransInfo.stTranLog.ucEntryMode & MODE_FALLBACK_SWIPE) )
		{
			Display2StringInRect("非EMV收单行", "NOT EMV ENABLE");
			PubBeepErr();
			PubWaitKey(3);
			return ERR_NO_DISP;
		}
	}
	
	iRet = ValidPanNo(gstTransInfo.stTranLog.szPan);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = ValidCardExpiry();
	if( iRet!=0 )
	{
		return iRet;
	}
	CheckCapture();
	
	if( gstTransInfo.stTranLog.ucInstalment!=0 )
	{
		if( gstTransInfo.bIsFallBack || glstMsgInfo.iMessage==ICCARD_EVENT )
		{
			Display2Strings("分期,只接受刷卡", "PLS SWIPE AGAIN");
			PubBeepErr();
			PubWaitKey(5);
			return ERR_NEED_SWIPE;
		}
	}
	
	GetCardHolderName((char*)gstTransInfo.stTranLog.szHolderName);
	if(gstTransInfo.stTranLog.ucEntryMode ==MODE_CONTLESS_INPUT)
		return OK;
	iRet = ConfirmPanInfo();
	if( iRet!=0 )
	{
		CommOnHook(FALSE);
		return iRet;
	}

	iRet = GetSecurityCode();
	if( iRet!=0 )
	{
		return iRet;
	}
	
	if( ChkIfAmex() )
	{
		if( gstTransInfo.bIsFallBack && gstTransInfo.iFallbackErrCode==EMV_RET_NOAPP )
		{	// if AMEX emv is not enabled in terminal, take it as mag swipe
			gstTransInfo.stTranLog.ucEntryMode &= ~MODE_FALLBACK_SWIPE;
			gstTransInfo.stTranLog.ucEntryMode |= MODE_SWIPE_INPUT;
		}
	}
		
	return 0;
}

// 校验卡号
INT32 ValidPanNo(uint8_t *pszPanNo)
{
	uint8_t	bFlag, ucTemp, ucResult;
	uint8_t	*pszTemp;
	
	// 是否检查卡号
	if( !ChkIssuerOption(ISSUER_CHKPAN_MOD10) )
	{
		return 0;
	}
	
	// (2121算法)
	bFlag    = FALSE;
	pszTemp  = &pszPanNo[strlen((char *)pszPanNo)-1];
	ucResult = 0;
	while( pszTemp>=pszPanNo )
	{
		ucTemp = (*pszTemp--) & 0x0F;
		if( bFlag )    ucTemp *= 2;
		if( ucTemp>9 ) ucTemp -= 9;
		ucResult = (ucTemp + ucResult) % 10;
		bFlag = !bFlag;
	}
	
	if( ucResult!=0 )
	{
		Display2StringInRect("卡号错误", "INVALID CARD");
		PubBeepErr();
		PubWaitKey(3);
		return ERR_NO_DISP;
	}
	
	return 0;
}

// 检查卡的有效期(YYMM)
INT32 ValidCardExpiry(void)
{
	uint8_t	szDateTime[14+1];
	ulong	ulCardYear, ulCardMonth;
	ulong	ulCurYear, ulCurMonth;
	
	gstTransInfo.bExpiryError = FALSE;
	ulCardYear  = PubAsc2Long((char*)gstTransInfo.stTranLog.szExpDate, 2, NULL);
	ulCardYear +=  2000;
	ulCardMonth = PubAsc2Long((char*)gstTransInfo.stTranLog.szExpDate+2, 2, NULL);
	
	GetDateTime(szDateTime);
	ulCurYear  = PubAsc2Long((char*)szDateTime, 4, NULL);
	ulCurMonth = PubAsc2Long((char*)szDateTime+4, 2, NULL);
	
	if( ulCardMonth>12 || ulCardMonth<1 )
	{
		if( gstTransInfo.stTranLog.ucEntryMode & MODE_MANUAL_INPUT )
		{
			Display2StringInRect("有效期月份错误", "ERR EXPIRED");
			PubBeepErr();
			PubWaitKey(3);
			return ERR_NO_DISP;
		}
	}

	if ( ulCardMonth>12 || ulCardMonth<1 || ulCardYear<ulCurYear || 
		 (ulCurYear==ulCardYear && ulCurMonth>ulCardMonth))
	{
		gstTransInfo.bExpiryError = TRUE;
		if( ChkIssuerOption(ISSUER_EN_EXPIRY) && ChkIssuerOption(ISSUER_CHECK_EXPIRY) )
		{
			Display2StringInRect("此卡已失效", "CARD EXPIRED");
			if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
			{	// if EMV expired card, let core to continue process(based upon TACs/IACs)
				lcdFlip();
				sysDelayMs(1500);
				return 0;
			}
			else
			{
				PubBeepErr();
				PubWaitKey(3);
				return ERR_NO_DISP;
			}
		}
	}
	
	return 0;
}

// 获得持卡人姓名(已经转换为打印格式)
void GetCardHolderName(char *pszHolderName)
{
	INT32		iRet;
	uint8_t   iTagLen;
	uchar	    szBuff[40];
	uchar        szTempName[40];
	
	*pszHolderName = 0;
	if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
	{
		memset(szBuff, 0, sizeof(szBuff));
		iRet = emv_get_data_element("\x5F\x20",2, &iTagLen, (char*)szBuff);
		if( iRet!=EMV_RET_SUCCESS )
		{
			return;
		}
		ConvertHolderName(szBuff, szTempName);
	}
	else	// other entry mode, just get it from track 1 data
	{
		GetHolderNameFromTrack1(szTempName);
	}
	sprintf((char *)pszHolderName, "%.26s", szTempName);
}

// 确认卡号信息
INT32 ConfirmPanInfo(void)
{
	INT32		iIndex,iRet;
	uint8_t	ucLine, szIssuerName[10+1];
	int     iScreenwidth, iScreenheight;
	UINT8   ucMode = DISP_ASCII, ucindex = 1;
	int	   small_font_size = 28,big_font_size = 30;
	
	iScreenwidth = iScreenheight = 0;
	lcdGetSize(&iScreenwidth, &iScreenheight);
	if (!(iScreenheight==64 && iScreenwidth == 128))
	{
		ucindex = 2;
		ucMode = DISP_CFONT;
	}	
	
	lcdCls();
	DisplayTransName();
	ucLine = 5;

/*	iIndex = MatchCardBin(gstTransInfo.stTranLog.szPan);
	if( iIndex>=0 )
	{
		lcdDisplay(0, ucLine, ucMode, "%.21s", gstSysParam.stIssuerNameList[iIndex].szEnglishName);
		ucLine += ucindex;
		if( strlen((char *)gstSysParam.stIssuerNameList[iIndex].szEnglishName)>21 )
		{
			lcdDisplay(0, ucLine, ucMode, "%.9s", gstSysParam.stIssuerNameList[iIndex].szEnglishName+21);
			ucLine += ucindex;
		}
	}
	else
	{
		ConvIssuerName((uchar*)gstCurIssuer.szName, szIssuerName);
		lcdDisplay(0, ucLine, ucMode, "%s", szIssuerName);
		ucLine += ucindex;
	}
*/
/*	if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
	{
		lcdDisplay(0,3,ucMode,"%.21s", gstTransInfo.stTranLog.szAppLabel);
	}
	else
		lcdDisplay(0,3,ucMode,"Other");
*/
	lcdDisplay(0, ucLine, ucMode, "%.21s", gstTransInfo.stTranLog.szHolderName);
	ucLine += ucindex;

	if (ucMode == DISP_CFONT)
	{
		if( strlen((char *)gstTransInfo.stTranLog.szPan)>18 )
		{
			lcdSetFont("/usr/share/fonts/wqy-microhei.ttf", "GBK", 0,  small_font_size, 0);
			lcdDisplay(0, ucLine, ucMode, "%s", gstTransInfo.stTranLog.szPan);
			lcdSetFont("/usr/share/fonts/wqy-microhei.ttf", "GBK", 0,  big_font_size, 0);
		}
		else
		{
			lcdDisplay(0, ucLine, ucMode, "%s", gstTransInfo.stTranLog.szPan);
		}
	}
	else
	{
		lcdDisplay(0, ucLine, ucMode, "%.19s", gstTransInfo.stTranLog.szPan);
	}
	ucLine += ucindex;
	
	if( ChkIssuerOption(ISSUER_EN_EXPIRY) )
	{
		if(!ChkIssuerOption(ISSUER_DISP_EXPIRY) )
			lcdDisplay(0, ucLine++, ucMode,"EXP. DATE: **/**");
		else
			lcdDisplay(0, ucLine++, ucMode, "EXP DATE:%2.2s/%2.2s",
			     &gstTransInfo.stTranLog.szExpDate[2], &gstTransInfo.stTranLog.szExpDate[0]);
	}
	
	if( !PubYesNo(USER_OPER_TIMEOUT) )
	{
		return ERR_USERCANCEL;
	}
	
	if( gstTransInfo.stTranLog.ucTranType==SALE_OR_AUTH ||
		gstTransInfo.stTranLog.ucTranType==SALE         ||
		gstTransInfo.stTranLog.ucTranType==AUTH         ||
		gstTransInfo.stTranLog.ucTranType==PREAUTH )
	{
#ifdef OFFLINE_TEST_VERSION
	return OK;
#endif		
		lcdCls();
		DisplayTransName();

		Display2Strings("预拨号...","PRE-DIALING...");
		lcdFlip();
		iRet = PreDial();
		iRet = CheckPreDial(iRet);
		if (iRet != 0)
		{
			return iRet;
		}
	}

	// set EMV library parameters
// 	if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
// 	{
// 		emvGetMCKConfig(&gstPosEmvConfig);
// 		memcpy(gstEmvCoreParam.MerchantID., gstCurAcq.szMerchantID, 15);
// 		memcpy(gstEmvCoreParam., gstCurAcq.szTermID, 8);
// 		emvSetMCKConfig(glEmvParam);
//  	}
	
	return 0;
}

// 根据参数设置对PAN进行掩码
void MaskPan(uint8_t *pszInPan, uint8_t *pszOutPan)
{
	int  iPanLen, iCnt, index;
	char  szBuffer[40], szTemp[50];
	
	iPanLen = strlen((char *)gstTransInfo.stTranLog.szPan);
	iCnt = index = 0;

	memset(szTemp,0,sizeof(szTemp));
	memset(szBuffer,0,sizeof(szBuffer));
	while(iCnt < iPanLen)
	{
		if (iCnt<6 || iCnt+4>= iPanLen)
		{
			szBuffer[index] = gstTransInfo.stTranLog.szPan[iCnt];
		}
		else
		{
			szBuffer[index] = '*';
		}
		iCnt++;
		index++;
	}
	iCnt = index = 0;
	while (iCnt < iPanLen)
	{
		if (iPanLen - iCnt >= 4)
		{
			sprintf(&szTemp[index],"%4.4s ",szBuffer+iCnt);
//			prnPrintf("%4.4s ",szBuffer+iCnt);
			iCnt += 4;
			index += 5;
		}
		else
		{
			sprintf(&szTemp[index],"%s ",szBuffer+iCnt);
//			prnPrintf("%s ",szBuffer+iCnt);
			iCnt = iPanLen;
		}
	} 

	if (pszOutPan != NULL)
	{
		sprintf((char *)pszOutPan,szTemp);
	}
/*	uint8_t	szBuff[30];
	INT32		iCnt, iPanLen, iOption;
	
	memset(szBuff, 0, sizeof(szBuff));
	iPanLen = strlen((char *)pszInPan);
	iOption = 0;
	if( !ChkOptionExt(gstCurIssuer.sPanMask, 0x0080) )	//the first bit of the 24 bits
	{	// right align
		for(iCnt=0; iCnt<iPanLen; iCnt++)
		{
			iOption = ((2-iCnt/8)<<8) + (1<<(iCnt%8));
			if( !ChkOptionExt(gstCurIssuer.sPanMask, (UINT32)iOption) )
			{
				szBuff[iPanLen-iCnt-1] = pszInPan[iPanLen-iCnt-1];
			}
			else
			{
				szBuff[iPanLen-iCnt-1] = '*';
			}
		}
	}
	else
	{	// left align
		for(iCnt=0; iCnt<iPanLen; iCnt++)
		{
			iOption = (((iCnt+2)/8)<<8) + (0x80>>((iCnt+2)%8));
			if( !ChkOptionExt(gstCurIssuer.sPanMask, (UINT32)iOption) )
			{
				szBuff[iCnt] = pszInPan[iCnt];
			}
			else
			{
				szBuff[iCnt] = '*';
			}
		}
	} 
	
	for(iCnt=0; iCnt<iPanLen; iCnt++)
	{

		if( iCnt >= iPanLen-4 )
		{
			szBuff[iCnt] = pszInPan[iCnt];
		}
		else
		{
			szBuff[iCnt] = '*';
		}
	}

	sprintf((char *)pszOutPan, "%.*s", LEN_PAN, szBuff);*/
}

// 从磁道信息分析出卡号(PAN)
INT32 GetPanFromTrack(uint8_t *pszPAN, uint8_t *pszExpDate)
{
	INT32		iPanLen;
	char	*p, *pszTemp;

	// 从2磁道开始到'＝'
	if( strlen((char *)gstTransInfo.stTranLog.szTrack2)>0 )
	{
		pszTemp = (char *)gstTransInfo.stTranLog.szTrack2;
	}
	else if( strlen((char *)gstTransInfo.szTrack3)>0 )
	{
		pszTemp = (char *)&gstTransInfo.szTrack3[2];
	}
	else
	{	// 2、3磁道都没有
		return ERR_SWIPECARD;
	}

	p = strchr((char *)pszTemp, '=');
	if( p==NULL )
	{
		return ERR_SWIPECARD;
	}
	iPanLen = p - pszTemp;
	if( iPanLen<13 || iPanLen>19 )
	{
		return ERR_SWIPECARD;
	}

	sprintf((char *)pszPAN, "%.*s", iPanLen, pszTemp);
	if( pszTemp==(char *)gstTransInfo.stTranLog.szTrack2 )
	{
		sprintf((char *)pszExpDate, "%.4s", p+1);
	}
	else
	{
		sprintf((char *)pszExpDate, "0000");
	}

	return 0;
}

// get holder name form track 1, which is identified by '^'
void GetHolderNameFromTrack1(uint8_t *pszHolderName)
{
	char	*p, *q;
	uint8_t	szOrgName[50];
	INT32		iLen;

	*pszHolderName = 0;
	if( gstTransInfo.szTrack1[0]==0 )
	{
		return;
	}

	p = strchr((char *)gstTransInfo.szTrack1, '^');
	if( p==NULL )
	{
		return;
	}
	p++;
	iLen = strlen(p);

	q = strchr(p, '^');
	if( q!=NULL )
	{
		iLen = min((INT32)(q-p), iLen);
	}

	sprintf((char *)szOrgName, "%.*s", (INT32)min((INT32)(sizeof(szOrgName)-1), iLen), p);
	ConvertHolderName(szOrgName, pszHolderName);
}

// 转换ISO7813格式人名为打印格式
// "Amex/F D.Mr" --> "Mr F D Amex"
void ConvertHolderName(uint8_t *pszOrgName, uint8_t *pszNormalName)
{
	char	*pszTitle, *pszMidName, *pszTemp, szBuff[50];

	sprintf((char *)pszNormalName, "%s", pszOrgName);
	if( *pszOrgName==0 )
	{
		return;
	}
	pszTemp = (char *)pszNormalName;

	pszMidName = strchr((char *)pszOrgName, '/');
	if( pszMidName==NULL )
	{
		return;
	}

	pszTitle = strrchr((char *)pszOrgName, '.');
	if( pszTitle!=NULL )
	{
		sprintf(szBuff, "%s ", pszTitle+1);
		PubTrimStr(szBuff);
		pszTemp += sprintf(pszTemp, "%s ", szBuff);

		sprintf(szBuff, "%.*s ", (INT32)(pszTitle-pszMidName-1), pszMidName+1);
		PubTrimStr(szBuff);
		pszTemp += sprintf(pszTemp, "%s ", szBuff);
	}
	else
	{
		sprintf(szBuff, "%s", pszMidName+1);
		PubTrimStr(szBuff);
		pszTemp += sprintf(pszTemp, "%s ", szBuff);
	}
	sprintf(pszTemp, "%.*s", (INT32)(pszMidName-(char *)pszOrgName), pszOrgName);
}


// 根据卡号匹配的收单行信息,提示用户选择收单行
INT32 SelectAcqForCard(uint8_t *psAcqMatchFlag, uint8_t *pucCardIndex)
{
	INT32			iMenuNo, iValidAcqCnt;
	uint		    ucCnt, uiAcqNum;
	char        szPrompt[16+1];
	MenuItem	stAcqMenu[MAX_ACQ+1];
	
	// build menu from candidate acquirer's list
	memset(&stAcqMenu[0], 0, sizeof(stAcqMenu));
	for(uiAcqNum=ucCnt=0; ucCnt<gstSysParam.uiAcqNum; ucCnt++)
	{
		if( psAcqMatchFlag[ucCnt]==0 )
		{
			continue;
		}
		stAcqMenu[uiAcqNum].bVisible = TRUE;
		stAcqMenu[uiAcqNum].pfMenuFunc = NULL;
		sprintf((char *)stAcqMenu[uiAcqNum].szMenuName, "%.10s", gstSysParam.stAcqList[ucCnt].szName);
		uiAcqNum++;
	}
	
	stAcqMenu[uiAcqNum].bVisible = FALSE;
	stAcqMenu[uiAcqNum].pfMenuFunc = NULL;
	stAcqMenu[uiAcqNum].szMenuName[0] = 0;

	// prompt use select acquirer
	if( uiAcqNum<=9 )
	{
		sprintf((char *)szPrompt, "选择中心    HOST");
	}
	else
	{
		sprintf((char *)szPrompt, "选择中心:");
	}
	iMenuNo = PubGetMenu((uchar *)szPrompt, stAcqMenu, MENU_AUTOSNO, USER_OPER_TIMEOUT);
	if( iMenuNo<0 )
	{
		return (iMenuNo==-3) ? ERR_USERCANCEL : ERR_SEL_ACQ;
	}
	
	// search selected acquirer index
	for(iValidAcqCnt=0,ucCnt=0; ucCnt<gstSysParam.uiAcqNum; ucCnt++)
	{
		if( psAcqMatchFlag[ucCnt]==0 )
		{
			continue;
		}
		if( iValidAcqCnt==iMenuNo )
		{
			*pucCardIndex = ucCnt;
			return 0;
		}
		iValidAcqCnt++;
	}
	
	return ERR_SEL_ACQ;
}


// 根据卡号匹配卡表,并最终确定收单行(glCurAca)和发卡行(gstCurIssuer)
INT32 MatchCardTable(uint8_t *pszPAN)
{
	INT32			iRet;
	uint8_t		    ucCnt, ucPanLen, ucLastAcqIdx;
	uint            uiAcqNum;
	uint8_t		    sPanHeader[5], sCardIndex[MAX_ACQ], sAcqMatchFlag[MAX_ACQ];
	ST_CARD_INFO	*pstCardTbl;

	memset(sCardIndex,    0, sizeof(sCardIndex));
	memset(sAcqMatchFlag, 0, sizeof(sAcqMatchFlag));

	ucLastAcqIdx = 0;
	// 建立收单行列表
	ucPanLen = strlen((char *)pszPAN);
	PubAsc2Bcd((char *)pszPAN, 10, (char *)sPanHeader);
	for(uiAcqNum=ucCnt=0; ucCnt<gstSysParam.uiCardNum; ucCnt++)
	{
		pstCardTbl = &gstSysParam.stCardTable[ucCnt];
		if( (memcmp(pstCardTbl->sPanRangeLow,  sPanHeader, 5)<=0  &&
			 memcmp(pstCardTbl->sPanRangeHigh, sPanHeader, 5)>=0) &&
			(pstCardTbl->ulPanLength==0 || pstCardTbl->ulPanLength==ucPanLen) )
		{
			FindIssuer(pstCardTbl->ucIssuerKey);
			FindAcq(pstCardTbl->ucAcqKey);
			sCardIndex[gstCurAcq.ucIndex] = ucCnt;
			if( gstSysCtrlParam.sAcqStatus[gstCurAcq.ucIndex]==S_USE  ||
				gstSysCtrlParam.sAcqStatus[gstCurAcq.ucIndex]==S_PENDING )
			{
// 				if( gstTransInfo.stTranLog.ucInstalment==1 )
// 				{
// 					return 0;
// 				}
				if( sAcqMatchFlag[gstCurAcq.ucIndex]==0 )
				{
					sAcqMatchFlag[gstCurAcq.ucIndex] = 1;
					uiAcqNum++;
					ucLastAcqIdx = gstCurAcq.ucIndex;
				}
			}
		}
	}
	if( uiAcqNum==0 )
	{
		return ERR_UNSUPPORT_CARD;
	}

	if( uiAcqNum==1 )	// 只有一个收单行
	{
		pstCardTbl = &gstSysParam.stCardTable[sCardIndex[ucLastAcqIdx]];
		FindIssuer(pstCardTbl->ucIssuerKey);
		FindAcq(pstCardTbl->ucAcqKey);
		return 0;
	}

	// 匹配到多个收单行
	if( ChkEdcOption(EDC_SELECT_ACQ_FOR_CARD) )
	{	// 手工选择收单行
		iRet = SelectAcqForCard(sAcqMatchFlag, &ucCnt);
		if( iRet!=0 )
		{
			return iRet;
		}
	}
	else
	{	// 自动选择一个收单行
		for(ucCnt=0; ucCnt<gstSysParam.uiAcqNum; ucCnt++)
		{
			if( sAcqMatchFlag[ucCnt]!=0 )
			{
				break;
			}
		}
	}
	pstCardTbl = &gstSysParam.stCardTable[sCardIndex[ucCnt]];
	FindIssuer(pstCardTbl->ucIssuerKey);
	FindAcq(pstCardTbl->ucAcqKey);

	return 0;
}


// 读取IC卡磁道信息/卡号信息等
INT32 GetEmvTrackData()
{
	INT32		iRet, iLength = 0;
	uint8_t	sTemp[50], szCardNo[20+1];
	INT32		i, bReadTrack2, bReadPan;

	// 读取应用数据
	DisplayProcessing();
	sysDelayMs(100);
	iRet = emv_read_app_data(icc_fd);
	iRet = iRet?errno:0;
	if( iRet!=EMV_RET_SUCCESS )
	{
		return ERR_TRAN_FAIL;
	}

	// Read Track 2 Equivalent Data
	bReadTrack2 = FALSE;
	memset(sTemp, 0, sizeof(sTemp));
	iLength = 0;
	iRet = emv_get_data_element((uchar *)"\x57",1, (uint8_t*)&iLength, (char*)sTemp);
	if( iRet==EMV_RET_SUCCESS )
	{
		bReadTrack2 = TRUE;
		PubBcd2Asc0((char *)sTemp, iLength, gstTransInfo.stTranLog.szTrack2);
		PubTrimTailChars(gstTransInfo.stTranLog.szTrack2, 'F');	// erase padded 'F' chars
		for(i=0; gstTransInfo.stTranLog.szTrack2[i]!='\0'; i++)		// convert 'D' to '='
		{
			if( gstTransInfo.stTranLog.szTrack2[i]=='D' )
			{
				gstTransInfo.stTranLog.szTrack2[i] = '=';
				break;
			}
		}
	}

	// read PAN
	bReadPan = FALSE;
	memset(sTemp, 0, sizeof(sTemp));
	iLength = 0;
	iRet =emv_get_data_element((uchar *)"\x5A",1, (uint8_t*)&iLength, (char*)sTemp); 
	if( iRet==EMV_RET_SUCCESS )
	{
		PubBcd2Asc0((char *)sTemp, iLength, (char *)szCardNo);
		PubTrimTailChars((char *)szCardNo, 'F');		// erase padded 'F' chars
		if( bReadTrack2 && !MatchTrack2AndPan((uint8_t *)gstTransInfo.stTranLog.szTrack2, szCardNo) )
		{	// 如果Track2 & PAN 同时存在,则必须匹配
			Display2StringInRect("卡号错误", "CARD ERROR");
			PubBeepErr();
			PubWaitKey(3);
			PromptRemoveICC();
			return ERR_NO_DISP;
//			return ERR_TRAN_FAIL;
		}
		sprintf((char *)gstTransInfo.stTranLog.szPan, "%.19s", szCardNo);
		bReadPan = TRUE;
	}
	else if ( !bReadTrack2 )	// 如果Track 2 和 PAN 都没有,则交易失败
	{
		return ERR_TRAN_FAIL;
	}
	if( !bReadPan )	// 没有读取PAN，但是有track 2
	{
		iRet = GetPanFromTrack(gstTransInfo.stTranLog.szPan, gstTransInfo.stTranLog.szExpDate);
		if( iRet!=0 )
		{
			return ERR_TRAN_FAIL;
		}
	}

	// read PAN sequence number
	gstTransInfo.stTranLog.bPanSeqOK = FALSE;
	iRet = emv_get_data_element((uchar *)"\x5F\x34",2, (uint8_t*)&iLength, (char*)&gstTransInfo.stTranLog.ucPanSeqNo);
	if( iRet==EMV_RET_SUCCESS )
	{
		gstTransInfo.stTranLog.bPanSeqOK = TRUE;
	}

	// read Application Expiration Date
	if( bReadPan )
	{
		memset(sTemp, 0, sizeof(sTemp));
		iRet = emv_get_data_element((uchar *)"\x5F\x24",2, (uint8_t*)&iLength, (char*)sTemp);
		if( iRet==EMV_RET_SUCCESS )
		{
			PubBcd2Asc0((char *)sTemp, 2, (char *)gstTransInfo.stTranLog.szExpDate);
		}
	}

	// read other data for print slip
	emv_get_data_element((uchar *)"\x50",1, (uint8_t*)&iLength, (char*)gstTransInfo.stTranLog.szAppLabel);	// application label
	iRet = emv_get_data_element((uchar *)"\x4F",1, (uint8_t*)&iLength, (char*)gstTransInfo.stTranLog.sAID);	// AID
	if( iRet==EMV_RET_SUCCESS )
	{
		gstTransInfo.stTranLog.ucAidLen = (uint8_t)iLength;
	}

	// read cardholder name
	memset(sTemp, 0, sizeof(sTemp));
	iRet = emv_get_data_element((uchar *)"\x5F\x20",2, (uint8_t*)&iLength, (char*)sTemp);
	if( iRet==EMV_RET_SUCCESS )
	{
		sprintf((char *)gstTransInfo.stTranLog.szHolderName, "%.20s", sTemp);
	}

	return 0;
}

// 比较2磁道信息和PAN是否一致(For ICC)
INT32 MatchTrack2AndPan(uint8_t *pszTrack2, uint8_t *pszPan)
{
	INT32		i;
	uint8_t	szTemp[19+1];

	for(i=0; i<19 && pszTrack2[i]!='\0'; i++)
	{
		if( pszTrack2[i]=='=' )
		{
			break;
		}
		szTemp[i] = pszTrack2[i];
	}
	szTemp[i] = 0;
	if (i >= strlen((char *)pszPan))
	{
		i = strlen((char *)pszPan);
	}
	if (i <= 16)
	{
		i = 16;  // 卡号不小于16字节
	}

	if(memcmp((char *)pszPan,(char *)szTemp,i)==0 )
	{
		return TRUE;
	}

	return FALSE;
}

// RFU for HK
INT32 MatchCardBin(uint8_t *pszPAN)
{
	char	szStartNo[20+1], szEndNo[20+1];
	UINT32	i;

	for(i=0; i<gstSysParam.uiCardBinNum; i++)
	{
		PubBcd2Asc((char *)gstSysParam.stCardBinTable[i].sStartNo, 10, szStartNo);
		PubBcd2Asc((char *)gstSysParam.stCardBinTable[i].sEndNo,   10, szEndNo);
		if( memcmp(pszPAN, szStartNo, gstSysParam.stCardBinTable[i].ucMatchLen)>=0 &&
			memcmp(pszPAN, szEndNo,   gstSysParam.stCardBinTable[i].ucMatchLen)<=0 )
		{
			return (INT32)gstSysParam.stCardBinTable[i].ucIssuerIndex;
		}
	}

	return -1;
}


// 用户输入事件检测(按键/刷卡/插卡)
INT32 DetectCardEvent(uint8_t ucMode)
{
	//磁头上电、打开、清缓冲
	if( ucMode & CARD_SWIPED )
	{
// 		magClose();  // 8210 不要频繁的打开磁头只打开一次
// 		magOpen();
		magReset();
	}

	if( ucMode & FALLBACK_SWIPE )
	{
		DispFallBackSwipeCard();
	}
	else if( (ucMode & CARD_SWIPED) && (ucMode & CARD_INSERTED) )
	{
		DisplaySwipeCard();
	}
	else if( (ucMode & CARD_INSERTED)  )
	{
		if( !(ucMode & SKIP_DETECT_ICC) )
		{
			DisplayInsertCard();
		}
	}
	else if( (ucMode == CARD_KEYIN)  )
	{
		DisplayKeyInOnly();
	}
	else
	{
		DisplaySwipeOnly();
	}

	lcdFlip();
    kbFlush();
	while( 1 )
	{
		if( PubChkKeyInput() )
		{
			return CARD_KEYIN;		// 有按键事件
		}
		if( (ucMode & CARD_SWIPED) && (magSwiped()==YES) )
		{
			return CARD_SWIPED;		// 有刷卡事件
		}
		if( (ucMode & CARD_INSERTED) && ChkIfEmvEnable() )
		{
			if( ucMode & SKIP_DETECT_ICC )
			{
				return CARD_INSERTED;	// 有插入IC卡事件
			}
			else if( iccDetect(ICC_USER)==ICC_SUCCESS )
			{
				return CARD_INSERTED;	// 有插入IC卡事件
			}
		}
	}
}


// 输入有效期
INT32 GetExpiry(void)
{
	INT32		iRet;
	uint8_t	szExpDate[4+1];

	if( !ChkIssuerOption(ISSUER_EN_EXPIRY) )
	{
		sprintf((char *)gstTransInfo.stTranLog.szExpDate, "1111");
		return 0;
	}

	while( 1 )
	{
		Display2StringHigh("有效限期", "EXP DATE: MMYY");
		if( PubGetString(NUM_IN, 4, 4, szExpDate, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}

		sprintf((char *)gstTransInfo.stTranLog.szExpDate, "%.2s%.2s", &szExpDate[2], szExpDate);
		iRet = ValidCardExpiry();
		if( iRet==0 )
		{
			break;
		}
	}

	return 0;
}

// get partial pan for PIN entry
INT32 ExtractPAN(uint8_t *pszPAN, uint8_t *pszOutPan)
{
	INT32		iLength;
	
	iLength = strlen((char*) pszPAN);
	if( iLength<13 || iLength>19 )
	{
		return ERR_SWIPECARD;
	}
	
	memset(pszOutPan, '0', 16);
	memcpy(pszOutPan+4, pszPAN+iLength-13, 12);
	pszOutPan[16] = 0;
	
	return 0;
}


// input CVV2 or 4DBC
int GetSecurityCode(void)
{
	UINT8	ucLen;
	
	if( !ChkIfNeedSecurityCode() )
	{
		return 0;
	}
	
/*	if( !(gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT) )
	{
		return 0;
	}*/

	lcdClrLine(2,7);
//	ucLen = ChkIfAmex() ? 4 : 3;
//	lcdDisplay(0, 2, DISP_CFONT, "%s", ChkIfAmex() ? "ENTER 4DBC" : "SECURITY CODE");
	ucLen = 3;
	lcdDisplay(0, 2, DISP_CFONT, "SECURITY CODE");
	if( PubGetString(NUM_IN, ucLen, ucLen, gstTransInfo.szSecurityCode, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}
	
	gstTransInfo.stTranLog.ucEntryMode |= MODE_4DBC_INPUT;
	return 0;
}


// end of file
