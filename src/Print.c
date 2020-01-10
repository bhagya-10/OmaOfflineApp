
#include "global.h"
//#include "font.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
PRNFONT_INFO  gSongGB2312CHn;
PRNFONT_INFO  gSongGB18030CHn;
PRNFONT_INFO  gCourierNewAsciiEn;
PRNFONT_INFO  gArialAsciiEn32;
PRNFONT_INFO  gArialAsciiEn24;
PRNFONT_INFO  gArialAsciiEn20;
PRNFONT_INFO  gSystemAsciiEn16;
/********************** Internal functions declaration *********************/
//static void   PrnHead(void);
static void   PrnAmount(uint8_t *pszIn);
static void   PrnTotalInfo(void *pstInfo);
static INT32  PrnParaAcq(uint8_t ucAcqIndex);
static void   PrnParaIssuer(uint8_t ucAcqIndex);
static void   PrnParaIssuerSub(uint8_t ucIssuerKey);
static void   PrnIssuerOption(uint8_t *psOption);
static void   PrnCardTable(uint8_t ucIssuerKey);
/*static*/ INT32  PrnInstalmentPara(void);
static INT32  PrnEmvPara(void);
static void   PrnHexString(char *pszTitle, uint8_t *psHexStr, INT32 iLen);
static void   DispPrnError(uint8_t ucErrCode);
//static void   PrintfStrigMidOfLine(char *pszString);
/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

void  InitSysAllPrnFont()
{
	memset((char*)&gSongGB2312CHn,0,sizeof(gSongGB2312CHn));
	memset((char*)&gSongGB18030CHn,0,sizeof(gSongGB18030CHn));
	memset((char*)&gCourierNewAsciiEn,0,sizeof(gCourierNewAsciiEn));
	memset((char*)&gArialAsciiEn32,0,sizeof(gArialAsciiEn32));
	memset((char*)&gArialAsciiEn24,0,sizeof(gArialAsciiEn24));
	memset((char*)&gSystemAsciiEn16,0,sizeof(gSystemAsciiEn16));
	memset((char*)&gArialAsciiEn20,0,sizeof(gArialAsciiEn20));

	memcpy(gSongGB2312CHn.szFontName,FONT_SONG,sizeof(FONT_SONG));
	memcpy(gSongGB2312CHn.szFontCharacter,FONT_CHARSET_GB2312,sizeof(FONT_CHARSET_GB2312));
	gSongGB2312CHn.iStyle = 0;
	gSongGB2312CHn.iSize = 12;
	gSongGB2312CHn.iPriority = SET_PRIMARY_FONT;
	
	memcpy(gSongGB18030CHn.szFontName,FONT_SONG,sizeof(FONT_SONG));
	memcpy(gSongGB18030CHn.szFontCharacter,FONT_CHARSET_GB18030,sizeof(FONT_CHARSET_GB18030));
	gSongGB18030CHn.iStyle = 0;
	gSongGB18030CHn.iSize = 24;
	gSongGB18030CHn.iPriority = SET_PRIMARY_FONT;
	
	memcpy(gCourierNewAsciiEn.szFontName,FONT_COURIER_NEW,sizeof(FONT_COURIER_NEW));
	memcpy(gCourierNewAsciiEn.szFontCharacter,FONT_CHARSET_ASCII,sizeof(FONT_CHARSET_ASCII));
	gCourierNewAsciiEn.iStyle = 0;
	gCourierNewAsciiEn.iSize = 12;
	gCourierNewAsciiEn.iPriority = SET_PRIMARY_FONT;
	
//	memcpy(gArialAsciiEn32.szFontName,FONT_ARIAL,sizeof(FONT_ARIAL));
	strcpy(gArialAsciiEn32.szFontName,"/usr/share/fonts/arialnb.ttf");
	dfb_create_font(gArialAsciiEn32.szFontName, 32, 0);
	memcpy(gArialAsciiEn32.szFontCharacter,FONT_CHARSET_ASCII,sizeof(FONT_CHARSET_ASCII));
	gArialAsciiEn32.iStyle = 0;
	gArialAsciiEn32.iSize = 32;
	gArialAsciiEn32.iPriority  = SET_PRIMARY_FONT;

//	memcpy(gArialAsciiEn24.szFontName,FONT_ARIAL,sizeof(FONT_ARIAL));
	strcpy(gArialAsciiEn24.szFontName,"/usr/share/fonts/arial.ttf");
//	strcpy(gArialAsciiEn24.szFontName,"/usr/share/fonts/wqy-microhei.ttf");
	dfb_create_font(gArialAsciiEn24.szFontName, 20, 0);
	memcpy(gArialAsciiEn24.szFontCharacter,FONT_CHARSET_ASCII,sizeof(FONT_CHARSET_ASCII));
	gArialAsciiEn24.iStyle = 0;
	gArialAsciiEn24.iSize = 20;
	gArialAsciiEn24.iPriority  = SET_PRIMARY_FONT;

	memcpy(gSystemAsciiEn16.szFontName,FONT_SYSTEM,sizeof(FONT_SYSTEM));
	memcpy(gSystemAsciiEn16.szFontCharacter,FONT_CHARSET_ASCII,sizeof(FONT_CHARSET_ASCII));
	gSystemAsciiEn16.iStyle = 0;
	gSystemAsciiEn16.iSize = 24;
	gSystemAsciiEn16.iPriority  = SET_PRIMARY_FONT;

//	memcpy(gArialAsciiEn20.szFontName,FONT_ARIAL,sizeof(FONT_ARIAL));
	strcpy(gArialAsciiEn20.szFontName,"/usr/share/fonts/arial.ttf");
	dfb_create_font(gArialAsciiEn20.szFontName, 18, 0);
	memcpy(gArialAsciiEn20.szFontCharacter,FONT_CHARSET_ASCII,sizeof(FONT_CHARSET_ASCII));
	gArialAsciiEn20.iStyle = 0;
	gArialAsciiEn20.iSize = 18;
	gArialAsciiEn20.iPriority  = SET_PRIMARY_FONT;

	return;
}

void GetCurrentPrnFont(PRNFONT_INFO *PstCurPrnFont)
{
	prnGetCurFont(PstCurPrnFont->szFontName,PstCurPrnFont->szFontCharacter,
		          &PstCurPrnFont->iStyle,&PstCurPrnFont->iSize,PstCurPrnFont->iPriority);
}

INT32 SetCurrentPrnFont(PRNFONT_INFO stCurPrnFont)
{
	return prnSetFont(stCurPrnFont.szFontName,stCurPrnFont.szFontCharacter,stCurPrnFont.iStyle,
		              stCurPrnFont.iSize,stCurPrnFont.iPriority);
}

INT32 PrintReceipt(uint8_t ucPrnFlag)
{
		
#ifdef 	_POS_TYPE_6210
	return OK;
#endif

	int     iRet;
	uint8_t	ucNum;
	uint8_t	szBuff[50];
	uint8_t	szIssuerName[10+1], szTranName[16+1];
	int     iDatalen;
	char  buf[200], buf1[200];
	
	for(ucNum=0; ucNum<gstSysParam.stEdcInfo.uiPrintNums; ucNum++)
	{
		PrnHead();
		// print txn name
		GetEngTranLabel(glTranConfig[gstTransInfo.stTranLog.ucTranType].szLabel, szTranName);
		if( gstTransInfo.stTranLog.ucTranType==VOID || (gstTransInfo.stTranLog.uiStatus & TS_VOID) )
		{
			GetEngTranLabel(glTranConfig[gstTransInfo.stTranLog.ucOrgTranType].szLabel, szTranName);
			sprintf((char *)szBuff, "VOID(%s)", szTranName);
		}
		else if( gstTransInfo.stTranLog.uiStatus & TS_ADJ )
		{
			sprintf((char *)szBuff, "%s(ADJ)", szTranName);
		}
		else
		{
			sprintf((char *)szBuff, "%s", szTranName);
		}

		iDatalen = strlen((char *)szBuff);
		iDatalen = (15-iDatalen);
		prnPrintf("%*.*s",iDatalen,iDatalen,"                                        ");
		SetCurrentPrnFont(gSystemAsciiEn16);
		prnPrintf("%s\n",szBuff);
		

//		SetCurrentPrnFont(gArialAsciiEn24);
		SetCurrentPrnFont(gSystemAsciiEn16);
		ChangeToEngTime((char *)gstTransInfo.stTranLog.szDateTime, (char *)szBuff);  //DATE/TIME
		prnPrintf("DATE:%12.12s      \nTIME:%6.6s:%2.2s\n", szBuff,szBuff+14,gstTransInfo.stTranLog.szDateTime+12);

		MaskPan(gstTransInfo.stTranLog.szPan, szBuff);
		if( ucNum==1 ) // Customer Copy 只显示最后4位卡号
		{
			memcpy(szBuff,"**** **",7);
		}
//		memcpy(szBuff,gstTransInfo.stTranLog.szPan,sizeof(gstTransInfo.stTranLog.szPan));
		if( gstTransInfo.stTranLog.ucEntryMode & MODE_SWIPE_INPUT )// ||  gstTransInfo.stTranLog.ucEntryMode & MODE_CONTLESS_INPUT)
		{
			prnPrintf("Card  No: %s \nSwipe\n", gstTransInfo.stTranLog.szPan);
		}
		else if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
		{
			prnPrintf("Card  No: %s \nChip\n", gstTransInfo.stTranLog.szPan);
		}
		else if( gstTransInfo.stTranLog.ucEntryMode & MODE_FALLBACK_SWIPE )
		{
			prnPrintf("Card  No: %s \nFall\n", gstTransInfo.stTranLog.szPan);
		}
		else	if(gstTransInfo.stTranLog.ucEntryMode &MODE_CONTLESS_INPUT)
		{
			prnPrintf("Card  No: %s \nContless\n",gstTransInfo.stTranLog.szPan);
		}
		else
		{
			prnPrintf("Card  No: %s \nManual\n", gstTransInfo.stTranLog.szPan);
		}

		
		if (strlen((char *)gstTransInfo.stTranLog.szHolderName) )
		{
			prnPrintf("HOLDER: %s\n", gstTransInfo.stTranLog.szHolderName);
		}
		// print expiry	
		if( ChkIssuerOption(ISSUER_DISP_EXPIRY))
		{
//			prnPrintf("EXPY DATE: **/**\n");
			prnPrintf("EXPY DATE: %2.2s/%2.2s\n", &gstTransInfo.stTranLog.szExpDate[2],
					gstTransInfo.stTranLog.szExpDate);
		}
		else
		{
			prnPrintf("EXPY DATE: %2.2s/%2.2s\n", &gstTransInfo.stTranLog.szExpDate[2],
					gstTransInfo.stTranLog.szExpDate);
		}
		
		if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
		{
			prnPrintf("App : %.16s \n", gstTransInfo.stTranLog.szAppLabel);
			PubBcd2Asc0((char *)gstTransInfo.stTranLog.sAID, gstTransInfo.stTranLog.ucAidLen, (char *)szBuff);
			PubTrimTailChars((char *)szBuff, 'F');
			prnPrintf("AID : %s\n", szBuff);
			PubBcd2Asc0((char *)gstTransInfo.stTranLog.sAppCrypto, 8, (char *)szBuff);
			prnPrintf(" TC : %s\n",szBuff);
		}
//		else
//		{
//			ConvIssuerName((uint8_t *)gstCurIssuer.szName, szIssuerName);
//			prnPrintf("TYPE: %s\n", szIssuerName); // issuer Name
//		}

		prnPrintf("REF. NO:%12s \nAPPR.CODE: %-6.6s\n", gstTransInfo.stTranLog.szRRN, gstTransInfo.stTranLog.szAuthCode);
		prnPrintf("BATCH NO:%06ld   \nTRACE NO:%06ld\n", gstCurAcq.ulCurBatchNo, gstTransInfo.stTranLog.ulInvoiceNo);
 
		// amount
		PrnAmount((uint8_t *)" ");  //Print amount

		SetCurrentPrnFont(gSystemAsciiEn16);
		prnSetSpace(1,1);
		if( ucNum==0 )
		{
			prnPrintf("CARDHOLDER SIGNATURE\n");
//			prnPrintf(" NO REFUND\n");
#ifdef _APP_SUPP_ELECSIGN  // 电子签名  

			if ( ChkEdcOption(EDC_ENABLE_ELECSIGN))
			{
				if (gstTransInfo.stTranLog.ucElecSignatureflag & TS_ELECSIGN_HAVE)
				{
					strcpy(buf,(char *)gstTransInfo.stTranLog.szElecfilename);
					strcpy(buf1,"/var/tmp/prttemp.png");
					iRet = jbg2png(buf,buf1);
					if (iRet == 0)
					{
						pntDrawPicture(0,0,0,0,buf1);
					}
					else
					{
						prnPrintf("\n\n\n\n");
					}
					fileRemove(buf1);
				}
				else
				{
					prnPrintf("\n\n\n\n");
				}
			}	
#else
			prnPrintf("\n\n\n\n");			
#endif
			prnPrintf("--------------------------------\n");
			PrintfStrigMidOfLine("  I ACKNOWLEDGE SATISFACTORY\n");
			PrintfStrigMidOfLine("     RECEIPT OF RELATIVE \n");
			PrintfStrigMidOfLine("        GOODS/SERVICE\n");
		}
		else
		{
			prnPrintf("--------------------------------\n");
			SetCurrentPrnFont(gSystemAsciiEn16);
			prnPrintf(" * Cardholder Signature Verify *\n");
			SetCurrentPrnFont(gSystemAsciiEn16);
			prnSetSpace(1,1);
			PrintfStrigMidOfLine("  I ACKNOWLEDGE SATISFACTORY \n");
			PrintfStrigMidOfLine("     RECEIPT OF RELATIVE \n");
			PrintfStrigMidOfLine("        GOODS/SERVICE\n");
		}

		if( ucPrnFlag==PRN_REPRINT )
		{
			PrintfStrigMidOfLine("     ******REPRINT*******\n");
		}
		
		if( ucNum==0 )
		{
			PrintfStrigMidOfLine("    **** MERCHANT COPY ****\n");
		}
		else if( ucNum==1 )
		{
			PrintfStrigMidOfLine("    **** CUSTOMER COPY ****\n");
		}
		else if( ucNum==2 )
		{
			prnPrintf("         **** BANK COPY ****\n");
		}

//		PrintfStrigMidOfLine("Powered By CPU Solutions Sdn. Bhd.\n");
		PrintfStrigMidOfLine("  ***OFFLINE TEST VERSION***\n");
		prnPrintf("\n\n\n\n");

		iRet = StartPrinter();
		if ( iRet != 0)
		{
			return iRet;
		}
		 
		if( ucNum==0 || (ucNum+1 < gstSysParam.stEdcInfo.uiPrintNums))
		{
			if (!(gstTransInfo.stTranLog.ucElecSignatureflag & TS_ELECSIGN_HAVE))
			{
				lcdClrLine(2,7);
				PubDispString("I HEREBY VERIFY",3|DISP_LINE_CENTER);
				PubDispString("SIGNATURE",5|DISP_LINE_CENTER);
				DrawRect(0, 17, 127, 63);
				PubWaitKey(0);
			}
//			Display2Strings("请撕纸/按任意键", "PRESS ANY KEY");
//			PubWaitKey(USER_OPER_TIMEOUT);  // USER_OPER_TIMEOUT);
		}
	}

	return 0;
}

// 打印明细
void PrnAllList(void)
{
		
	INT32		iRet;
	uint8_t	szTitle[16+1], ucIndex;
#ifdef 	_POS_TYPE_6210
		return OK;
#endif

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szTitle, "PRINT LOG       ");
	else
		sprintf((char *)szTitle, "打印流水  PrnLog");
	PubDisplayTitle(TRUE,(char *)szTitle);
	lcdFlip();

	iRet = SelectAcq(FALSE, szTitle, &ucIndex);
	if( iRet!=0 )
	{
		return;
	}

	if( ucIndex!=ACQ_ALL )
	{
		PrnCurAcqTransList();
		return;
	}

	for(ucIndex=0; ucIndex<gstSysParam.uiAcqNum; ucIndex++)
	{
		SetCurAcq(ucIndex);
		PrnCurAcqTransList();
	}
}

// print list of transaction of current acquirer
INT32 PrnCurAcqTransList(void)
{
		//return OK;
	int		iCnt, iNumOfOnePage;
	char	szBuff[100], szIssuerName[10+1];
	char    szTranName[16+1],szDateTime[30];
#ifdef 	_POS_TYPE_6210
	return OK;
#endif	
	PrnHead();
	
	prnStep(3);
	PrintfStrigMidOfLine("     TRANSACTION LIST\n");
	memset(szDateTime,0,sizeof(szDateTime));
	GetDateTime((uint8_t *)szDateTime);
	ChangeToEngTime(szDateTime, szBuff);  //DATE/TIME
	prnPrintf("DATE: %12.12s     TIME: %6.6s:%2.2s\n", szBuff,szBuff+14,szDateTime+12);
	
	prnPrintf("HOST NII: %.3s   %.10s\n", gstCurAcq.szNii, gstCurAcq.szName);
	prnPrintf("================================\n");
	iNumOfOnePage = 0;
	for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
	{
		if( gstSysCtrlParam.sAcqKeyList[iCnt]!=gstCurAcq.ucKey )
		{
			continue;
		}
		
		memset(&gstTransInfo.stTranLog, 0, sizeof(ST_TRAN_LOG));
		LoadTranLog(&gstTransInfo.stTranLog, (UINT32)iCnt);
		FindIssuer(gstTransInfo.stTranLog.ucIssuerKey);  //Alex add
		ConvIssuerName((uint8_t *)gstCurIssuer.szName, (uint8_t *)szIssuerName);
		prnPrintf("ISSUER: %.10s\n", szIssuerName);
		
		MaskPan(gstTransInfo.stTranLog.szPan, (uint8_t *)szBuff);
		//		memcpy(szBuff,gstTransInfo.stTranLog.szPan,sizeof(gstTransInfo.stTranLog.szPan));
		if( gstTransInfo.stTranLog.ucEntryMode & MODE_SWIPE_INPUT )// ||  gstTransInfo.stTranLog.ucEntryMode & MODE_CONTLESS_INPUT)
		{
			prnPrintf("Card No: %s Swipe\n", szBuff);
		}
		else if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
		{
			prnPrintf("Card No: %s Chip\n", szBuff);
		}
		else if( gstTransInfo.stTranLog.ucEntryMode & MODE_FALLBACK_SWIPE )
		{
			prnPrintf("Card No: %s Fall\n", szBuff);
		}
		else
		{
			prnPrintf("Card No: %s Manual\n", szBuff);
		}
		
		GetEngTranLabel(glTranConfig[gstTransInfo.stTranLog.ucTranType].szLabel, (uint8_t *)szTranName);
		if( gstTransInfo.stTranLog.uiStatus & TS_VOID )
		{
			GetEngTranLabel(glTranConfig[gstTransInfo.stTranLog.ucOrgTranType].szLabel, (uint8_t *)szTranName);
			sprintf((char *)szBuff, "%s(VOID)", szTranName);
		}
		else if( gstTransInfo.stTranLog.uiStatus & TS_ADJ )
		{
			sprintf((char *)szBuff, "%s(ADJUST)", szTranName);
		}
		else
		{
			sprintf((char *)szBuff, "%s", szTranName);
		}
		prnPrintf("TXN: %-21.21s\nTRACE: %06lu\n", szBuff, gstTransInfo.stTranLog.ulInvoiceNo);
		PrnAmount((uint8_t *)"");
		prnPrintf("=============================\n");
		
		iNumOfOnePage++;
		if( (iNumOfOnePage%5)==0 )
		{
			if( StartPrinter()!=0 )
			{
				return 1;
			}
			iNumOfOnePage = 0;
			prnInit();
			prnSetSpace(0, 0);
			prnSetGray(150);
//			prnSetGray(200);
			prnStep(2);
		}
	}
	PrintfStrigMidOfLine("****END OF LIST****");
	prnPrintf("\n\n\n\n");
	return StartPrinter();	
}


void PrnHead(void)
{
		//return OK;
	char  szBuffer[40];
#ifdef _POS_TYPE_6210
	return OK;
#endif
	prnInit();
	InitSysAllPrnFont();
/*	SetCurrentPrnFont(gArialAsciiEn24);	
	prnSetSpace(0,0);
	prnStep(1);
	pntDrawPicture(0,0,300,86,(char*)Bmp_Prn);
	prnPrintf("\n\n\n\n");
//	prnLogo(EM_BMP_ROTATE0,Bmp_Prn);
	prnSetGray(150);
*/
	memset(szBuffer,0,sizeof(szBuffer));
//	SetCurrentPrnFont(gArialAsciiEn24);
	SetCurrentPrnFont(gSystemAsciiEn16);

//	SetCurrentPrnFont(gSystemAsciiEn16);
	sprintf(szBuffer,"%s\n", gstSysParam.stEdcInfo.szMerchantName);
	PrintfStrigMidOfLine(szBuffer);
	memset(szBuffer,0,sizeof(szBuffer));
	
	sprintf(szBuffer,"%s\n", gstSysParam.stEdcInfo.szMerchantAddr);
	PrintfStrigMidOfLine(szBuffer);
	memset(szBuffer,0,sizeof(szBuffer));
	sprintf(szBuffer,"%s\n", &gstSysParam.stEdcInfo.szMerchantAddr[40]);
//	PrintfStrigMidOfLine(szBuffer);
	prnPrintf("\n");
	
	prnPrintf("TID: %s \nMID: %s\n", gstCurAcq.szTermID, gstCurAcq.szMerchantID);
}

void PrnAmount(uint8_t *pszIn)
{
	uint8_t	szBuff[50], szTotalAmt[12+1];
	int     iDatalen;

	iDatalen = strlen((char *)pszIn);
	SetCurrentPrnFont(gSystemAsciiEn16);
	prnSetSpace(0,0);
	if( ChkEdcOption(EDC_TIP_PROCESS) && gstTransInfo.stTranLog.ucTranType!=REFUND &&
		gstTransInfo.stTranLog.ucTranType!=VOID && iDatalen != 0 &&
		!(gstTransInfo.stTranLog.uiStatus &TS_VOID) )
	{
		PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, gstTransInfo.stTranLog.szAmount, 2, szBuff, 0);
		prnPrintf("%sBASE%28.28s\n", pszIn, szBuff);
		if( memcmp(gstTransInfo.stTranLog.szTipAmount, "000000000000", 12)!=0 )
		{
			PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, gstTransInfo.stTranLog.szTipAmount, 2, szBuff, 0);
			prnPrintf("%sTIPS%28.28s\n", pszIn, szBuff);
		}
		else
		{
			prnPrintf("%sTIPS\n", pszIn);
		}
		if( *pszIn==' ' )
		{
			prnPrintf("                             --------------\n");
		}
		if( memcmp(gstTransInfo.stTranLog.szTipAmount, "000000000000", 12)!=0 )
		{
			PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, (char *)szTotalAmt);
			PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, szTotalAmt, 2, szBuff, 0);
			prnPrintf("%sTOTAL:%17.17s\n", pszIn, szBuff);
		}
		else
		{
			prnPrintf("%sTOTAL:\n", pszIn);
		}
		if( *pszIn==' ' )
		{
			prnPrintf("                             --------------\n");
		}
	}
	else
	{
		if (gstTransInfo.stTranLog.ucTranType == VOID &&
			memcmp(gstTransInfo.stTranLog.szTipAmount, "000000000000", 12)!=0  )
		{
			PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, (char *)szTotalAmt);
		}
		else
		{
			memcpy(szTotalAmt,gstTransInfo.stTranLog.szAmount,sizeof(gstTransInfo.stTranLog.szAmount));
		}
		PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, szTotalAmt, 2,
			szBuff, GetTranAmountInfo(&gstTransInfo.stTranLog));
		prnPrintf("%sTOTAL:%17.17s\n", pszIn, szBuff);
	}

	SetCurrentPrnFont(gSystemAsciiEn16);
//	prnSetSpace(2,2);
}


// print total information of ucIssuerKey
INT32 PrnTotalIssuer(uint8_t ucIssuerKey)
{
		//return OK;
	uint8_t	ucIndex, szBuff[20];
#ifdef 	_POS_TYPE_6210
		return OK;
#endif

	for(ucIndex=0; ucIndex<gstSysParam.uiIssuerNum; ucIndex++)
	{
		if( gstSysParam.stIssuerList[ucIndex].ucKey==ucIssuerKey )
		{
			break;
		}
	}
	memcpy(&gstPrnTotal, &gstIssuerTotal[ucIndex], sizeof(ST_TOTAL_INFO));

	if( gstPrnTotal.uiSaleCnt==0  && gstPrnTotal.uiRefundCnt==0 &&
		gstPrnTotal.uiVoidSaleCnt==0 && gstPrnTotal.uiVoidRefundCnt==0 )
	{
		return 0;
	}
	
	prnPrintf("-----------------------------------------------------\n");
	ConvIssuerName((uint8_t *)gstSysParam.stIssuerList[ucIndex].szName, szBuff);
	prnPrintf("ISSUER  : %-10.10s\n", szBuff);
	PrnTotalInfo(&gstPrnTotal);

	return 0;
}

// print total information
void PrnTotalInfo(void *pstInfo)
{
	uint8_t		szBuff[50], szBaseAmt[20];
	ST_TOTAL_INFO	*pstTotal;

	pstTotal = (ST_TOTAL_INFO *)pstInfo;
	PubAscSub((char *)pstTotal->szSaleAmt, (char *)pstTotal->szTipAmt, 12, (char *)szBaseAmt);
	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, szBaseAmt, 2, szBuff, 0);
	prnPrintf("BASE :%13d\n%s\n", pstTotal->uiSaleCnt, szBuff);

	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, pstTotal->szTipAmt, 2, szBuff, 0);
	prnPrintf("TIPS : %13d\n%s\n", pstTotal->uiTipCnt, szBuff);

	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, pstTotal->szSaleAmt, 2, szBuff, 0);
	prnPrintf("SALES: %13d\n%s\n", pstTotal->uiSaleCnt, szBuff);

	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, pstTotal->szVoidSaleAmt, 2, szBuff, GA_NEGATIVE);
//	prnPrintf("VOIDED SALES: %6d  %27.27s\n", pstTotal->uiVoidSaleCnt, szBuff);
	prnPrintf("VOIDED:%13d\n%s\n", pstTotal->uiVoidSaleCnt, szBuff);

}

INT32 PrintSettle(uint8_t ucPrnFlag)
{
		//return OK;
#ifdef 	_POS_TYPE_6210
		return OK;
#endif

	ulong	ulTraceNo;
	int     iIssureIndex,iCnt; 
	int     iDatalen;
	char    szBuff[100],szDateTime[30];

	if( ucPrnFlag==PRN_NORMAL )
	{
		// save settle information for reprint
		gstSysCtrlParam.stRePrnStlInfo.bValid[gstCurAcq.ucIndex]    = TRUE;
		gstSysCtrlParam.stRePrnStlInfo.ulSOC[gstCurAcq.ucIndex]     = gstSysCtrlParam.ulInvoiceNo;
		gstSysCtrlParam.stRePrnStlInfo.ulBatchNo[gstCurAcq.ucIndex] = gstCurAcq.ulCurBatchNo;
		sprintf((char *)gstSysCtrlParam.stRePrnStlInfo.szSettleMsg[gstCurAcq.ucIndex], "%s", gstTransInfo.szSettleMsg);
		memcpy(&gstSysCtrlParam.stRePrnStlInfo.stAcqTotal[gstCurAcq.ucIndex], &gstAcqTotal[gstCurAcq.ucIndex], sizeof(ST_TOTAL_INFO));
//		memcpy(gstSysCtrlParam.stRePrnStlInfo.stIssTotal[gstCurAcq.ucIndex], gstIssuerTotal, sizeof(gstIssuerTotal));
		for(iIssureIndex=0;iIssureIndex<MAX_ISSUER;iIssureIndex++)
		{ // 保存当前ACQ的所以ISSURE的结算信息
			for(iCnt=0;iCnt<MAX_ISSUER;iCnt++)
			{
				if (gstSysParam.stIssuerList[iIssureIndex].ucKey == gstCurAcq.sIssuerKey[iCnt])
				{
					memcpy(&gstSysCtrlParam.stRePrnStlInfo.stIssTotal[iIssureIndex], &gstIssuerTotal[iIssureIndex], sizeof(ST_TOTAL_INFO));
				}
			}
		}

		SaveRePrnStlInfo();
	}

	PrnHead();

	strcpy(szBuff,"SETTLEMENT");
	iDatalen = strlen(szBuff);
	iDatalen = (20-iDatalen);
	prnPrintf("%*.*s",iDatalen,iDatalen,"                                        ");
	SetCurrentPrnFont(gSystemAsciiEn16);
//	SetCurrentPrnFont(gArialAsciiEn32);
	prnPrintf("%s\n","SETTLEMENT");
//	SetCurrentPrnFont(gArialAsciiEn24);
	
	memset(szDateTime,0,sizeof(szDateTime));
	GetDateTime((uint8_t *)szDateTime);
	ChangeToEngTime(szDateTime, szBuff);  //DATE/TIME
	prnPrintf("DATE: %12.12s     \nTIME: %6.6s:%2.2s\n", szBuff,szBuff+14,szDateTime+12);
	PrintfStrigMidOfLine("***SUMMARY OF CHARGE***\n");
	PrintfStrigMidOfLine("*CLOSE BATCH CONFIRMED*\n\n");
	ulTraceNo = (ucPrnFlag==PRN_NORMAL) ? gstSysCtrlParam.ulInvoiceNo : gstSysCtrlParam.stRePrnStlInfo.ulSOC[gstCurAcq.ucIndex];
	prnPrintf("BATCH NO:%06ld   \nTRACE NO:%06ld\n", gstCurAcq.ulCurBatchNo, ulTraceNo);
	sprintf(szBuff,"%s\n", gstTransInfo.szSettleMsg);
	PrintfStrigMidOfLine(szBuff);

	PrnTotalInfo(&gstTransTotal);

	if( ucPrnFlag==PRN_REPRINT )
	{
		prnPrintf("\n");
		PrintfStrigMidOfLine(" ******REPRINT*******\n");
	}
	prnPrintf("\n\n\n\n");

	return StartPrinter();
}

void PrnEngTime(void)
{
	uint8_t	szDateTime[14+1], szBuff[30];

	GetDateTime(szDateTime);
	ChangeToEngTime((char *)szDateTime, (char *)szBuff);
	prnPrintf("%s\n", szBuff);
}

// 打印终端参数
void PrintParam(void)
{
		//return OK;
	uint8_t	ucCnt;
#ifdef 	_POS_TYPE_6210
		return OK;
#endif

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "PRINT PARAMETER");
	else
		PubDisplayTitle(TRUE, "打印参数 PrnPara");
	prnInit();
//	SetCurrentPrnFont(gArialAsciiEn20);
	SetCurrentPrnFont(gSystemAsciiEn16);

	prnSetSpace(1, 4);
	prnSetGray(150);

	prnPrintf("\n\n%.30s\n", gstSysParam.stEdcInfo.szMerchantName);
	prnPrintf("%.40s\n",     gstSysParam.stEdcInfo.szMerchantAddr);
	prnPrintf("%.40s\n",     gstSysParam.stEdcInfo.szMerchantAddr+40);
//	prnPrintf("INIT TID: %.8s\n", gstSysParam.stEdcInfo.szDownLoadTID);
	PrnHexString("EDC OPTION:", (uint8_t *)gstSysParam.stEdcInfo.sOption, 5);

	PrnEngTime();
	prnPrintf("VERSION: %s\n", EDC_VER_STRING);

	for(ucCnt=0; ucCnt<gstSysParam.uiDescNum; ucCnt++)
	{
		prnPrintf("   %.2s:%.20s\n", gstSysParam.stDescList[ucCnt].szCode,
				gstSysParam.stDescList[ucCnt].szText);
	}
//	PrnInstalmentPara();

	for(ucCnt=0; ucCnt<gstSysParam.uiAcqNum; ucCnt++)
	{
		if( PrnParaAcq(ucCnt)==0 )
		{
			if( StartPrinter()!=0 )
			{
				return;
			}
			prnInit();
			prnSetGray(150);
			prnPrintf("\n\n\n");
			prnSetSpace(1, 4);
		}
	}
	if( StartPrinter()!=0 )
	{
		return;
	}

	PrnEmvPara();
}

INT32 PrnParaAcq(uint8_t ucAcqIndex)
{
	PubASSERT(gstSysParam.stAcqList[ucAcqIndex].ucKey!=INV_ACQ_KEY);
	if( gstSysParam.stAcqList[ucAcqIndex].ucKey==INV_ACQ_KEY )
	{
		return 1;
	}

	prnPrintf("\n========================\n");
	prnPrintf("%.3s  %.10s\n", gstSysParam.stAcqList[ucAcqIndex].szNii,
			gstSysParam.stAcqList[ucAcqIndex].szName);
	prnPrintf("TID: xxxxxx%2.2s\n",   gstSysParam.stAcqList[ucAcqIndex].szTermID+6);
	prnPrintf("MID: %.15s\n",  gstSysParam.stAcqList[ucAcqIndex].szMerchantID);
	PrnHexString("ACQ OPTION:", (uint8_t *)gstSysParam.stAcqList[ucAcqIndex].sOption, 4);
/*	prnPrintf("PRI TEL: %.24s\n", gstSysParam.stAcqList[ucAcqIndex].stTxnCommCfg.stPSTNPara.TxnTelNo1);
	prnPrintf("SEC TEL: %.24s\n", gstSysParam.stAcqList[ucAcqIndex].stTxnCommCfg.stPSTNPara.TxnTelNo2);
	prnPrintf("SET TEL1: %.24s\n", gstSysParam.stAcqList[ucAcqIndex].stTxnCommCfg.stPSTNPara.StlTelNo1);
	prnPrintf("SET TEL2: %.24s\n", gstSysParam.stAcqList[ucAcqIndex].stTxnCommCfg.stPSTNPara.StlTelNo2);
*/

	PrnParaIssuer(ucAcqIndex);

	return 0;
}

void PrnParaIssuer(uint8_t ucAcqIndex)
{
	uint8_t	ucCnt;

	for(ucCnt=0; ucCnt<gstSysParam.uiIssuerNum; ucCnt++)
	{
		if( gstSysParam.stAcqList[ucAcqIndex].sIssuerKey[ucCnt]!=INV_ISSUER_KEY )
		{
			PrnParaIssuerSub(gstSysParam.stAcqList[ucAcqIndex].sIssuerKey[ucCnt]);
		}
	}
}

void PrnParaIssuerSub(uint8_t ucIssuerKey)
{
	uint8_t	ucCnt, szBuff[20], szBuff1[40];

	if( ucIssuerKey==INV_ISSUER_KEY )
	{
		return;
	}

	for(ucCnt=0; ucCnt<gstSysParam.uiIssuerNum; ucCnt++)
	{
		if( gstSysParam.stIssuerList[ucCnt].ucKey==ucIssuerKey )
		{
			break;
		}
	}
	PubASSERT( ucCnt<gstSysParam.uiIssuerNum );
	if( ucCnt>=gstSysParam.uiIssuerNum )
	{
		return;
	}

	prnPrintf("---ISSUER: %-10.10s---\n", gstSysParam.stIssuerList[ucCnt].szName);
	PrnHexString("ISSUER OPTION:", (uint8_t *)gstSysParam.stIssuerList[ucCnt].sOption, 4);
	prnPrintf("PAN MASK: %02x %02x %02x\n",
			(uint8_t)(0xFF^gstSysParam.stIssuerList[ucCnt].sPanMask[0]),
			(uint8_t)(0xFF^gstSysParam.stIssuerList[ucCnt].sPanMask[1]),
			(uint8_t)(0xFF^gstSysParam.stIssuerList[ucCnt].sPanMask[2]));

	sprintf((char *)szBuff, "%lu%02lu", gstSysParam.stIssuerList[ucCnt].ulFloorLimit/100,
			gstSysParam.stIssuerList[ucCnt].ulFloorLimit%100);
	PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, szBuff, 2, szBuff1, 0);
	prnPrintf("FLOOR LIMIT: %s\n", szBuff1);
	prnPrintf("ADJUSTMENT PERCENT: %d%%\n", gstSysParam.stIssuerList[ucCnt].ucAdjustPercent);

	PrnIssuerOption((uint8_t *)gstSysParam.stIssuerList[ucCnt].sOption);
	PrnCardTable(ucIssuerKey);
}

void PrnIssuerOption(uint8_t *psOption)
{
	static	OPTION_INFO	stIssuerOptList[] =
	{
// 		{"ENABLE BALANCE?",		ISSUER_EN_BALANCE},
		{"ENABLE ADJUST?",		ISSUER_EN_ADJUST},
		{"ENABLE OFFLINE?",		ISSUER_EN_OFFLINE},
		{"ENABLE PREAUTH?",		ISSUER_EN_PREAUTH},
//		{"ENABLE REFUND?",		ISSUER_EN_REFUND},
//		{"ENABLE VOID?",		ISSUER_EN_VOID},
// 		{"ENABLE EXPIRY?",		ISSUER_EN_EXPIRY},
// 		{"CHECK EXPIRY?",		ISSUER_CHECK_EXPIRY},
// 		{"CHKEXP OFFLINE?",		ISSUER_CHECK_EXPIRY_OFFLINE},
// 		{"CHECK PAN?",			ISSUER_CHKPAN_MOD10},
// 		{"EN DISCRIPTOR?",		ISSUER_EN_DISCRIPTOR},
		{"ENABLE MANUL?",		ISSUER_EN_MANUL},
		{"ENABLE SWIPE?",		ISSUER_EN_SWIPE},
// 		{"ENABLE PRINT?",		ISSUER_EN_PRINT},
//		{"VOICE REFERRAL?",		ISSUER_EN_VOICE_REFERRAL},
// 		{"PIN REQUIRED?",		ISSUER_EN_PIN},
// 		{"ACCOUNT SELECT?",		ISSUER_EN_ACCOUNT_SELECTION},
// 		{"ROC INPUT REQ?",		ISSUER_ROC_INPUT_REQ},
// 		{"DISP AUTH CODE?",		ISSUER_AUTH_CODE},
// 		{"ADDTIONAL DATA?",		ISSUER_ADDTIONAL_DATA},
// 		{"SECURITY CODE?",		ISSUER_SECURITY_SWIPE},
// 		{"SECU. CODE MANUL?",	ISSUER_SECURITY_MANUL},
		{NULL, 0},
	};
	uint8_t	ucCnt;

	for(ucCnt=0; stIssuerOptList[ucCnt].pText!=NULL; ucCnt++)
	{
		if( ChkOptionExt(psOption, stIssuerOptList[ucCnt].uiOptVal) )
		{
			prnPrintf("    %-16.16s[ on]\n", stIssuerOptList[ucCnt].pText);
		}
		else
		{
			prnPrintf("    %-16.16s[off]\n", stIssuerOptList[ucCnt].pText);
		}
	}
}

void PrnCardTable(uint8_t ucIssuerKey)
{
	uint8_t	ucCnt, szBuff[30];

	if( ucIssuerKey==INV_ISSUER_KEY )
	{
		return;
	}

	for(ucCnt=0; ucCnt<gstSysParam.uiCardNum; ucCnt++)
	{
		if( gstSysParam.stCardTable[ucCnt].ucIssuerKey==ucIssuerKey )
		{
			PubBcd2Asc0(gstSysParam.stCardTable[ucCnt].sPanRangeLow, 5, (char *)szBuff);
			szBuff[10] = ' ';
			PubBcd2Asc0(gstSysParam.stCardTable[ucCnt].sPanRangeHigh, 5, (char *)&szBuff[11]);
			prnPrintf("%s\n", szBuff);
		}
	}
}

INT32 PrnInstalmentPara(void)
{
	uint8_t	ucCnt, szBuff[20], szBuff1[50], ucAcqIndex;

	if( !ChkIfInstalment() )
	{
		return 1;
	}

	prnPrintf("\n========INSTALMENT=======\n");
	for(ucCnt=0; ucCnt<gstSysParam.uiPlanNum; ucCnt++)
	{
		PubASSERT( gstSysParam.stPlanList[ucCnt].ucIndex!=0xFF );
		prnPrintf("%02d.%-7.7s - %dmths\n", ucCnt+1, gstSysParam.stPlanList[ucCnt].szName,
				gstSysParam.stPlanList[ucCnt].uiMonths);

		sprintf((char *)szBuff, "%lu", gstSysParam.stPlanList[ucCnt].ulLeastAmt);
		PubConvAmount((uint8_t *)gstSysParam.stEdcInfo.szCurrencyName, szBuff, 2, szBuff1, 0);
		ucAcqIndex = gstSysParam.stPlanList[ucCnt].ucAcqIndex;
		prnPrintf("%-10.10s  %s\n", gstSysParam.stAcqList[ucAcqIndex].szName, szBuff1);
	}

	return 0;
}

// 打印EMV参数
INT32 PrnEmvPara()
{
	INT32			iRet, iCnt,iAidNum,iCapkNum;
	AIDLIST	        stEmvApp;
	CAPKLIST	stEmvCapk;
	struct terminal_mck_configure   stEmvParam;

	prnInit();
	prnSetGray(150);
	SetCurrentPrnFont(gSystemAsciiEn16);
//	SetCurrentPrnFont(gArialAsciiEn20);
	prnSetSpace(1, 4);
	prnPrintf("\n\n\n=========EMV PARAMETER=======\n");
	emv_get_mck_configure(&stEmvParam);
	prnPrintf("TERMINAL TYPE: %02X\n", stEmvParam.terminal_type);
	PrnHexString("TERMINAL CAPA:",  stEmvParam.terminal_capabilities, 3);
	PrnHexString("TERM EX-CAPA :",  stEmvParam.additional_terminal_capabilities, 5);
	prnPrintf("SELECT PSE   : %s\n",  stEmvParam.support_pse_selection_method    ? "YES" : "NO");
	prnPrintf("CONFIRM      : %s\n",  stEmvParam.support_cardholder_confirmation   ? "YES" : "NO");
	prnPrintf("FORCE ONLINE : %s\n",  stEmvParam.support_forced_online_capability  ? "YES" : "NO");

	iAidNum = emv_get_aid_info_num();
	prnPrintf("\n\n\n=========EMV APP LIST========\n");
	if (iAidNum<=0)
	{
		iAidNum = 0;
	}
	prnPrintf("TOTAL AID : %d\n",iAidNum);
	for(iCnt=0; iCnt<iAidNum; iCnt++)
	{
		memset(&stEmvApp, 0, sizeof(AIDLIST));
		iRet = emv_get_one_aid_info(iCnt, &stEmvApp);
		if( iRet<=0)
		{
			continue;
		}
		PrnHexString("AID:",  stEmvApp.aid, (INT32)stEmvApp.aid_len);
		PrnHexString("VERSION:",  stEmvApp.application_version, 2);
		prnPrintf("SELECT FLAG   : %s MATCH\n", stEmvApp.support_partial_aid_select==FULL_MATCH ? "FULL" : "PARTIAL");
		prnPrintf("PRIORITY      : %d\n", stEmvApp.application_priority);
		prnPrintf("TARGET PER    : %d\n", stEmvApp.target_percentage);
		prnPrintf("MAX TARGET PER: %d\n", stEmvApp.maximum_target_percentage);
		prnPrintf("FLOOR LIMIT   : %lu\n", stEmvApp.terminal_floor_limit);
		prnPrintf("THRESHOLD     : %lu\n", stEmvApp.threshold_value);
		PrnHexString("TAC DENIAL :",  stEmvApp.terminal_action_code_denial,  5);
		PrnHexString("TAC ONLINE :",  stEmvApp.terminal_action_code_online,  5);
		PrnHexString("TAC DEFAULT:",  stEmvApp.terminal_action_code_default, 5);
		prnPrintf("=============================\n");
		if( (iCnt%5)==0 )
		{
			if( StartPrinter()!=0 )
			{
				return 1;
			}
			prnInit();
			prnSetSpace(1, 4);
			prnSetGray(150);
		}
	}

	iCapkNum = emv_get_ca_public_key_num();
	prnPrintf("\n\n=========EMV CAPK LIST=======\n");
	if (iCapkNum<=0)
	{
		iCapkNum = 0;
	}
	prnPrintf("TOTAL CAPK : %d\n",iCapkNum);
	for(iCnt=0; iCnt<iCapkNum; iCnt++)
	{
		memset(&stEmvCapk, 0, sizeof(CAPKLIST));
		iRet = emv_get_one_ca_public_key_by_no(iCnt, &stEmvCapk);
		if( iRet!=EMV_RET_SUCCESS )
		{
			continue;
		}
		PrnHexString("RID    :",  stEmvCapk.RID, 5);
		prnPrintf("KEY ID : %02X\n",  stEmvCapk.Index);
		PrnHexString("EXP DATE:",  stEmvCapk.expiration_date, 3);
		prnPrintf("MODUL LEN: %d bits\n",  (INT32)(8 * stEmvCapk.len_of_modulus));
		prnPrintf("=============================\n");
		if( (iCnt%5)==0 )
		{
			if( StartPrinter()!=0 )
			{
				return 1;
			}
			prnInit();
			prnSetSpace(1, 4);
			prnSetGray(150);
		}
	}
	
 	prnPrintf("\f");
 	return StartPrinter();
}


void PrnHexString(char *pszTitle, uint8_t *psHexStr, INT32 iLen)
{
	INT32		iCnt;

	prnPrintf("%s", pszTitle);
	for(iCnt=0; iCnt<iLen; iCnt++)
	{
		prnPrintf(" %02X", psHexStr[iCnt]);
	}
	prnPrintf("\n");
}

// 打印错误提示
INT32 StartPrinter(void)
{
	int  iRet, iCnt;

	if (gstPosCapability.uiPrnType == 0)
	{
		Display2Strings("打印机缺纸", "OUT OF PAPER");
		prnClose();
		return ERR_NO_DISP;
	}

	iRet = iCnt = 0;
	while( 1 )
	{
		iCnt++;
		if (prnStatus()==PRN_PAPEROUT)
		{
			DispPrnError(PRN_PAPEROUT);
			if (iCnt >= 3)
			{
				Display2StringInRect("请重新打印", "PLS REPRINT");
				PubWaitKey(2);
				break;
			}

			continue;
		}

		if(gstTransInfo.stTranLog.ucTranType == SALE ||
			gstTransInfo.stTranLog.ucTranType == VOID ||
			gstTransInfo.stTranLog.ucTranType == OFF_SALE ||
			gstTransInfo.stTranLog.ucTranType == PREAUTH)
		{
			PubDispString("PLEASE VERIFY",3|DISP_LINE_CENTER);
			PubDispString("SIGNATURE",5|DISP_LINE_CENTER);
			DrawRect(0, 17, 127, 63);
			PubWaitKey(1);
		}
		else
		{
			Display2StringInRect(" 正在打印       ", " PRINTING       ");
		}
//		PrintOne();
		iRet = prnStart();
		if( iRet==PRN_OK )
		{
			prnClose();
			return 0;	// print success!
		}
		DispPrnError(iRet);
		if( iRet == PRN_PAPEROUT )
		{
			continue;
		}

		Display2StringInRect("请重新打印", "PLS REPRINT");
		PubWaitKey(2);
		break;
	}
	prnClose();
	return ERR_NO_DISP;
}

void DispPrnError(uint8_t ucErrCode)
{
	switch( ucErrCode )
	{
	case PRN_BUSY:
		Display2Strings("打印机忙", "PRINTER BUSY");
		break;

	case PRN_PAPEROUT:
		Display2Strings("打印机缺纸", "OUT OF PAPER");
		break;

	case PRN_DATA_ERR:
		Display2Strings("打印数据包错误", "PRN DATA ERROR");
		break;

	case PRN_ERR:
		Display2Strings("打印机故障", "PRINTER ERROR");
		break;

	case PRN_NO_DOT:
		Display2Strings("未装字库", "NOT PRINT DOT");
		break;

	case PRN_DATA_BIG:
		Display2Strings("数据包太大", "PRN DATA ERROR");
		break;

	default:
		Display2Strings("打印未成功", "PLEASE REPRINT");
		break;
	}
	PubBeepErr();
	PubWaitKey(5);
}


void   PrintfStrigMidOfLine(char *pszString)
{
	char  szSpaceBuffer[40];
	int   iDatalen, iMaxLine, iTotalLine;
	PRNFONT_INFO stCurPrnFont;

	memset(szSpaceBuffer,0,sizeof(szSpaceBuffer));
	memset(szSpaceBuffer,' ',sizeof(szSpaceBuffer)-1);
	iDatalen = strlen(pszString);

	memset(&stCurPrnFont,0,sizeof(stCurPrnFont));
	GetCurrentPrnFont(&stCurPrnFont);
	if (stCurPrnFont.iSize == 24)
	{
		
//		iMaxLine = 37;
		iMaxLine = 32;
		iTotalLine = 32;
	}
	else
	{
		iMaxLine = 43;
		iTotalLine = 36;
	}
	prnPrintf("%s",pszString);
/*
	if (iDatalen>=iMaxLine)
	{
		prnPrintf("%s",pszString);
	}
	else
	{
//		iDatalen = (43-iDatalen)/2;
		iDatalen = iTotalLine-iDatalen;
		szSpaceBuffer[iDatalen] = 0;
		prnPrintf("%s%s",szSpaceBuffer,pszString);
	}
*/
}

// end of file
