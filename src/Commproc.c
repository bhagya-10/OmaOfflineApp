
#include "global.h"
#include "PedSignature.h"
/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
//INT32  GenSendPacket(void);
static INT32  ProcRecvPacket(void);
static void SaveRecvPackData(void);
static INT32  AdjustCommParam(void);
/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

// ���״���
// process transaction
INT32 TranProcess(void)
{
	INT32		iRet, iRetryPIN;
//	char        szbuff[8],szTemp[8];

	// EMV�����״���
	if( (gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT) &&
		(gstTransInfo.stTranLog.ucTranType==SALE || gstTransInfo.stTranLog.ucTranType==CASH ||
		 gstTransInfo.stTranLog.ucTranType==AUTH || gstTransInfo.stTranLog.ucTranType==PREAUTH ||
		 gstTransInfo.stTranLog.ucTranType==INSTALMENT) )
	{
		iRet = FinishEmvTran();
		if (iRet == ERR_USERCANCEL)
		{
			DisplayResult(iRet);
			return ERR_NO_DISP;
		}

		// оƬ�����׺�����г���������Ҫ��������
		TranReversal();
		return iRet;
	}

	if (gstTransInfo.stTranLog.ucTranType != LOGON)
	{
		// �ж��ϴν����Ƿ���Ҫ���г�����
		iRet = TranReversal();
		if( iRet!=0 )
		{
			return iRet;
		}
	}

	iRetryPIN = 0;
	while( 1 )
	{
		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			// ����ʧ�ܺ�����г���������Ҫ��������
			TranReversal();

			return iRet;
		}

		if( memcmp(gstRecvPack.szRspCode, "55", 2)!=0 || ++iRetryPIN>3 || !ChkIfNeedPIN() )
		{
			break;
		}

		// ��������PIN
		iRet = GetPIN(TRUE);
		if( iRet!=0 )
		{
			return iRet;
		}
		sprintf((char *)gstSendPack.szSTAN, "%06lu", gstSysCtrlParam.ulSTAN);
		memcpy(&gstSendPack.sPINData[0], "\x00\x08", 2);
		memcpy(&gstSendPack.sPINData[2], gstTransInfo.sPinBlock, 8);
	}

	return AfterTranProc();
}

// ����ͨѶ����
// exchange package(send request & wait response)
INT32 SendRecvPacket(void)
{
	INT32		iRet;
	int   ibacklight_set_time_bak;

	start_timer(30, 0);
	sysGetbacklighttime(&ibacklight_set_time_bak);
	sysSetbacklighttime(-1); // ����
	iRet = SendPacket();
	if( iRet!=0 )
	{
		stop_timer();
		sysSetbacklighttime(ibacklight_set_time_bak);
		return iRet;
	}

	// �������״̬/��������
	if( gstTransInfo.stTranLog.ucTranType==SETTLEMENT )
	{
		gstSysCtrlParam.sAcqStatus[gstCurAcq.ucIndex] = S_PENDING;
		SaveSysCtrlBase();
	}
	SaveRevInfo(TRUE);

	iRet = RecvPacket();
	if( iRet!=0 )
	{
		stop_timer();
		sysSetbacklighttime(ibacklight_set_time_bak);
		return iRet;
	}

	stop_timer();
	sysSetbacklighttime(ibacklight_set_time_bak);
	return 0;
}

// ���ɷ��ͱ���
// generate request package
INT32 GenSendPacket(void)
{
	INT32		iRet;
	uint	uiLength;
	char	szBuff[20], sMacOut[LEN_MAC];

	// prepare TPDU header
	memset(&gstSendData, 0, sizeof(ST_COMM_DATA));
	sprintf(szBuff, "600%3.3s0000", gstCurAcq.szNii);
	PubAsc2Bcd(szBuff, 10, gstSendData.sContent);

	// generate iso8583 data
	iRet = PubPack8583(glEdcDataDef, &gstSendPack, &gstSendData.sContent[5], &uiLength);
	if( iRet<0 )
	{
		Get8583ErrMsg(TRUE, iRet, (uchar*)szBuff);
		Display2StringInRect("�������", szBuff);
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}
	gstSendData.uiLength = (UINT32)(uiLength + 5);

	// ���DEBIT����MAC
	if( !ChkIfNeedMac() )
	{
		return 0;
	}

	// fill mac data
	memset(sMacOut, 0, sizeof(sMacOut));
	iRet = GetMAC((uchar*)&gstSendData.sContent[5], (UINT32)(uiLength-LEN_MAC), (uchar*)sMacOut);
	if( iRet!=0 )
	{
		return iRet;
	}
	memcpy(&gstSendData.sContent[gstSendData.uiLength-LEN_MAC], sMacOut, LEN_MAC);

	return 0;
}

// Ԥ����
// pre-connect to host
INT32 PreDial(void)
{
	INT32		iRet;

	if( !gstSysParam.stEdcInfo.bPreDial )
	{
		return 0;
	}

	lcdClrLine(2, 7);
	lcdDisplay(0, 4, DISP_CFONT, " PREDIALING..."); 
	DrawRect(0, 17, 127, 63);
	lcdFlip();

	iRet = AdjustCommParam();
	if( iRet!=0 )
	{
		return iRet;
	}

	return CommDial(PREDIAL_MODE);
}


int    CheckPreDial(int iPreDialRet)
{
	int iRet;

	if (iPreDialRet == 0)
	{
		return 0;
	}

	iRet = iPreDialRet;
	iPreDialRet = iPreDialRet & MASK_ERR_CODE;

	if (iRet == ERR_COMM_INV_TYPE)
	{
		DispCommErrMsg(iRet);
		return ERR_NO_DISP; 
	}

	switch( gstCurCommCfg.ucCommType )
	{
	case CT_RS232:		
	case CT_MODEM:		
	case CT_TCPIP:
	case CT_WIFI:
		if (iPreDialRet != 0)
		{
			DispCommErrMsg(iRet);
			return ERR_NO_DISP;
		}
		break;
		
	case CT_CDMA:
	case CT_GPRS:
		if (iPreDialRet== NET_ERR_NOSIM ||
			iPreDialRet == NET_ERR_SIMDESTROY)
		{
			DispCommErrMsg(iRet);
			return ERR_NO_DISP;
		}
		break;
		
	default:
		DispCommErrMsg(ERR_COMM_INV_TYPE);
		return ERR_NO_DISP;
		break;
	}

	return 0;
}

// ��������
// connect to host
INT32 ConnectHost(void)
{
	INT32		iRet, iRetryCnt, iHostNum;
	uint8_t	    ucKey;
	
	ucKey = 0;
	iHostNum = 1;
	if (gstCurAcq.stTxnCommCfg.ucCommType == CT_MODEM)
	{
		if (gstCurAcq.szTxnTelNo2[0] != 0)
		{
			iHostNum = 2;
		}
	}
	else
	{
		if (gstCurAcq.szRemoteIP2[0] != 0)
		{
			iHostNum = 2;
		}
	}

	gstTransInfo.ucConnectCnt = 0;
	iRet = ERR_NO_DISP;
	while(gstTransInfo.ucConnectCnt < iHostNum)
	{
		iRet = AdjustCommParam();
		if( iRet!=0 )
		{
			if( iRet!=ERR_NO_TELNO )
			{
				DispCommErrMsg(iRet);
				return ERR_NO_DISP;
			}
			return iRet;
		}

		iRetryCnt = 1;  // ÿ��HOST������ַֻ����һ��
		iRet = ERR_NO_DISP;
		while( iRetryCnt < 2 )
		{
			iRetryCnt++;
			DisplayDialing();
			lcdFlip();
			iRet = CommDial(ACTDIAL_MODE);
			if( iRet==0 )
			{
				return 0;
			}
			// ��ѡ��ѡ������Ȼ��ѡ�����Ĵ������ʧ�ܵ�ʱ��Ҫֱ���˳������򲻻���б��ݴ���
/*			if( (iRet & MASK_COMM_TYPE)!=ERR_COMM_ALL_BASE )
			{
				DispCommErrMsg(iRet);
				return ERR_NO_DISP;
			}
			
			PubBeepErr();
			Display2Strings("����ʧ��,����?  ", "DAIL FAIL,RETRY?");
			lcdDisplay(114, 7, DISP_ASCII, "%02X", (uint8_t)(iRet & MASK_ERR_CODE));
			ucKey = PubWaitKey(USER_OPER_TIMEOUT/2);
			if( ucKey==KEY_CANCEL || ucKey==KEY_INVALID )
			{
				return ERR_USERCANCEL;
			}*/
		}

		gstTransInfo.ucConnectCnt++;
	}

	if (iRet != 0)
	{
		if( (iRet & MASK_COMM_TYPE)!=ERR_COMM_ALL_BASE )
		{
			DispCommErrMsg(iRet);
			return ERR_NO_DISP;
		}
	}
	
	return ERR_NO_DISP;
}

// ���ͱ���
// send request package
INT32 SendPacket(void)
{
	INT32		iRet;
#ifdef OFFLINE_TEST_VERSION
		return OK;
#endif

	iRet = GenSendPacket();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = ConnectHost();
	if( iRet!=0 )
	{
		return iRet;
	}

	DisplaySending();
	if( gstCurCommCfg.ucCommType==CT_TCPIP ||
		gstCurCommCfg.ucCommType==CT_GPRS  ||
		gstCurCommCfg.ucCommType==CT_CDMA  ||
		gstCurCommCfg.ucCommType== CT_WIFI ||
		gstCurCommCfg.ucCommType==CT_RS232)// ||
//		gstCurCommCfg.ucCommType == CT_MODEM)
	{
		memmove(&gstSendData.sContent[2], gstSendData.sContent, gstSendData.uiLength);
		// BCD ��ʽ�ĳ����ֽ�
		if (gstSysParam.stEdcInfo.ucTcpMsgHeadLenType == _TCP_BCD_LEN_BYTE)
			PubLong2Bcd((ulong)gstSendData.uiLength,  2, gstSendData.sContent);
		else
			PubLong2Char((ulong)gstSendData.uiLength, 2, gstSendData.sContent);
		gstSendData.uiLength += 2;
	}
	
//	NacTxd(gstSendData.sContent, gstSendData.uiLength);

	iRet = CommTxd((uchar*)gstSendData.sContent, gstSendData.uiLength, -1);	// no timeout
	if( iRet!=0 )
	{
		DispCommErrMsg(iRet);
		return ERR_NO_DISP;
	}
	GetNewTraceNo();

	return 0;
}

// ���ձ���
INT32 RecvPacket(void)
{
	INT32		iRet;
	UINT32	uiTimeOut;
	ulong	ulTemp;

	DisplayReceiving();
#ifdef OFFLINE_TEST_VERSION
		if( gstTransInfo.stTranLog.ucTranType!=OFFLINE_SEND )
		{
			sprintf((char *)gstRecvPack.szRspCode,"00");
			sprintf((char *)gstTransInfo.stTranLog.szRspCode, "00");
		}
		if( gstTransInfo.stTranLog.ucTranType==SETTLEMENT ||
			gstTransInfo.stTranLog.ucTranType==UPLOAD	  ||
			gstTransInfo.stTranLog.ucTranType==REVERSAL )
		{
			return OK;
		}
	
		sprintf((char *)gstTransInfo.stTranLog.szAuthCode, "020038");
		if( gstTransInfo.stTranLog.szRRN[0]==0 )
		{
			sprintf((char *)gstTransInfo.stTranLog.szRRN, "201311028210");
		}
		sprintf((char *)gstTransInfo.stTranLog.szCondCode,	   "%.2s",	gstSendPack.szCondCode);
		return OK;
#endif	

	uiTimeOut = gstCurAcq.ulTimeOut ;
	memset(&gstRecvData, 0, sizeof(ST_COMM_DATA));
	iRet = CommRxd((uchar*)gstRecvData.sContent, LEN_MAX_COMM_DATA, uiTimeOut, &gstRecvData.uiLength);
	if( iRet!=0 )
	{
		DispCommErrMsg(iRet);
		return ERR_NO_DISP;
	}

//	NacTxd(gstRecvData.sContent,gstRecvData.uiLength);

	if( gstCurCommCfg.ucCommType==CT_TCPIP ||
		gstCurCommCfg.ucCommType==CT_GPRS  ||
		gstCurCommCfg.ucCommType==CT_CDMA  ||
		gstCurCommCfg.ucCommType== CT_WIFI ||
		gstCurCommCfg.ucCommType==CT_RS232)//||
//		gstCurCommCfg.ucCommType == CT_MODEM)
	{
		if (gstSysParam.stEdcInfo.ucTcpMsgHeadLenType == _TCP_BCD_LEN_BYTE)
		{
			PubBcd2Long(gstRecvData.sContent,  2, &ulTemp);
		}
		else
		{
			PubChar2Long(gstRecvData.sContent, 2, &ulTemp);
		}
		if( ulTemp+2!=(ulong)gstRecvData.uiLength )
		{
			Display2StringInRect("�������ݴ���", "RECV DATA ERR");
			PubBeepErr();
			PubWaitKey(2);
			return ERR_NO_DISP;
		}
		memmove(gstRecvData.sContent, &gstRecvData.sContent[2], (uint)ulTemp);
		gstRecvData.uiLength = (UINT32)ulTemp;
	}

	iRet = ProcRecvPacket();
	if( iRet!=0 )
	{
		return iRet;
	}

	return 0;
}

// ������հ�������鹫������Ҫ��
INT32 ProcRecvPacket(void)
{
	INT32		iRet;
	long	lTemp1, lTemp2;
	uint8_t	bFailFlag, szBuff[20];

	DisplayProcessing();
	// �����յ������ݳ���(TPDU+MTI+Bitmap+stan+tid)
	if( gstRecvData.uiLength<5+2+8+8+3 )
	{
		Display2StringInRect("�������ݴ���", "RECV DATA ERR");
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}
	bFailFlag = FALSE;

	// ����TPDU(5)
	memset(&gstRecvPack, 0, sizeof(ST_ISO8583));
	iRet = PubUnPack8583(glEdcDataDef, &gstRecvData.sContent[5], gstRecvData.uiLength-5, &gstRecvPack);
	if( iRet<0 )
	{
		Get8583ErrMsg(FALSE, iRet, szBuff);
		Display2StringInRect("�������", (char*)szBuff);
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}
	
	// macУ��
	if( ChkIfNeedMac() && memcmp(gstRecvPack.sMac,"\x00\x00\x00\x00\x00\x00\x00\x00",8) != 0)
//	if( ChkIfNeedMac() )
	{
		memset(szBuff,0,sizeof(szBuff));
		iRet = GetMAC((uint8_t *)&gstRecvData.sContent[5],gstRecvData.uiLength-13,szBuff);
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}
		
		if (memcmp(szBuff,gstRecvPack.sMac+2,4) != 0)
		{
			PubDispString("INVALID MAC",3|DISP_LINE_CENTER);
			PubDispString("Pls logon",5|DISP_LINE_CENTER);
			PubBeepErr();
			PubWaitKey(5);
			return ERR_NO_DISP;
		}
	}


	// ��������/��Ӧ����
	SaveRecvPackData();

	// �����Ӧ���ݰ���Ҫ�ز��������ƥ��
	lTemp1 = atol((char *)gstSendPack.szMsgCode);
	lTemp2 = atol((char *)gstRecvPack.szMsgCode);
	if( lTemp2!=(lTemp1+10) )	// �����Ϣ��
	{
		Display2StringInRect("��Ϣ�벻��", "MSG ID MISMATCH");
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	// ��鴦����
	if( gstSendPack.szProcCode[0]!=0 &&
		memcmp(gstSendPack.szProcCode, gstRecvPack.szProcCode, LEN_PROC_CODE-1)!=0 )
	{
		bFailFlag = TRUE;
	}
	if( bFailFlag )
	{
		Display2StringInRect("�����벻��", "BIT 3 MISMATCH");
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	// ���STAN
	if( gstSendPack.szSTAN[0]!=0 &&
		memcmp(gstSendPack.szSTAN, gstRecvPack.szSTAN, LEN_STAN)!=0 )
	{
		bFailFlag = TRUE;
	}
	if( bFailFlag )
	{
		Display2StringInRect("��ˮ�Ų���", "STAN MISMATCH");
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	// ����ն˺�/�̻���
	if( memcmp(gstSendPack.szTermID, gstRecvPack.szTermID, LEN_TERM_ID)!=0 )
	{
		bFailFlag = TRUE;
	}
	if( bFailFlag )
	{
		Display2StringInRect("�ն˺Ų���", "TID MISMATCH");
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	return 0;
}

// ������ձ���Ҫ�ص�������Ϣ�ṹ
void SaveRecvPackData(void)
{
	uint8_t	szCurTime[14+1];

	if( gstTransInfo.stTranLog.ucTranType!=OFFLINE_SEND )
	{
		sprintf((char *)gstTransInfo.stTranLog.szRspCode, "%.2s", gstRecvPack.szRspCode);
	}
	if( gstTransInfo.stTranLog.ucTranType==SETTLEMENT ||
		gstTransInfo.stTranLog.ucTranType==UPLOAD     ||
		gstTransInfo.stTranLog.ucTranType==REVERSAL )
	{
		return;
	}

	GetDateTime(szCurTime);
	sprintf((char *)gstTransInfo.stTranLog.szDateTime, "%.4s%.4s%.6s",
			szCurTime, gstRecvPack.szLocalDate, gstRecvPack.szLocalTime);

	// ���ѽ���Ҳͬ��һ��ϵͳʱ��
	if (gstTransInfo.stTranLog.ucTranType == SALE)
	{
		if (strlen(gstRecvPack.szLocalDate) != 0 && 
			strlen(gstRecvPack.szLocalTime) != 0)
		{
			memset(szCurTime,0,sizeof(szCurTime));
			PubAsc2Bcd((char *)gstTransInfo.stTranLog.szDateTime+2,12,(char *)szCurTime);
			sysSetTime(szCurTime);
		}
	}

	if( gstRecvPack.szAuthCode[0]!=0 )
	{
		sprintf((char *)gstTransInfo.stTranLog.szAuthCode, "%.6s", gstRecvPack.szAuthCode);
	}
	if( gstTransInfo.stTranLog.szRRN[0]==0 )
	{
		sprintf((char *)gstTransInfo.stTranLog.szRRN, "%.12s", gstRecvPack.szRRN);
	}
	sprintf((char *)gstTransInfo.stTranLog.szCondCode,     "%.2s",  gstSendPack.szCondCode);
//	sprintf((char *)gstTransInfo.stTranLog.szFrnCurcyCode, "%.3s",  gstSendPack.szFrnCurcyCode);
//	sprintf((char *)gstTransInfo.stTranLog.szFrnAmount,    "%.12s", gstSendPack.szFrnAmt);
}

INT32 AfterTranProc(void)
{
	INT32			iRet;
	unsigned char   szStanBak[LEN_STAN+2];
	unsigned long   ulSTANBak;

	// ����ʧ�ܴ���
	if( memcmp(gstTransInfo.stTranLog.szRspCode, "00", 2)!=0 )
	{
		CommOnHook(FALSE);
		SaveRevInfo(FALSE);

		DisplayResult(ERR_HOST_REJ);
		if( memcmp(gstTransInfo.stTranLog.szRspCode, "01", 2)==0 ||
			memcmp(gstTransInfo.stTranLog.szRspCode, "02", 2)==0 ||
			(gstTransInfo.stTranLog.ucInstalment!=0 && ChkAcqOption(ACQ_BOC_INSTALMENT_FEATURE)) )
		{	// BOC ����instalment����ʱ���罻�׽������"00",������referrel����
			iRet = TranSaleComplete();
			if( iRet==0 )
			{
				return 0;
			}
		}
		return ERR_NO_DISP;
	}

	if( gstTransInfo.stTranLog.ucTranType==VOID )
	{
		gstTransInfo.stTranLog.uiStatus |= TS_VOID;
		gstTransInfo.stTranLog.uiStatus &= ~(TS_ADJ|TS_NOSEND|TS_OFFLINE_SEND);
		UpdateTranLog(&gstTransInfo.stTranLog, gstTransInfo.uiRecNo);
	}
	if( ChkIfSaveLog() )
	{
		if( (gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT) &&
			(gstTransInfo.stTranLog.ucTranType==SALE || gstTransInfo.stTranLog.ucTranType==PREAUTH ) )
		{
			gstTransInfo.stTranLog.uiStatus |= TS_NEED_TC; //��ҪTC����
		}

		SaveTranLog(&gstTransInfo.stTranLog);
		gstTransInfo.uiRecNo = gstSysCtrlParam.stWriteInfo.uiRecNo; // Ϊ�˵���ǩ��ʱ���µ���ǩ���ļ�

		// ����ǵ�һ�ʽ���,�����ϴν������ں͵�ǰ���ڲ�ͬ�����޸��ϴν�������Ϊ���ν��׽�������
		if (1 >= GetTranLogNum(ACQ_ALL))
		{
			if (memcmp(gstTransInfo.stTranLog.szDateTime,gstSysCtrlParam.szLastSettlementDate,8)!=0)
			{
				memcpy(gstSysCtrlParam.szLastSettlementDate,gstTransInfo.stTranLog.szDateTime,sizeof(gstTransInfo.stTranLog.szDateTime));
				SaveSysCtrlAll();
			}
		}
	}
	SaveRevInfo(FALSE);

#if 0
	if( gstTransInfo.stTranLog.ucTranType!=BALANCE_BOC &&
		gstTransInfo.stTranLog.ucTranType!=BALANCE_SCB )
	{
		// ����TC & �������������޸������������������ȱ����ٻָ�
		memcpy(szStanBak,gstSendPack.szSTAN,sizeof(gstSendPack.szSTAN));
		ulSTANBak = gstTransInfo.stTranLog.ulSTAN;
		OfflineSend();  // ������
		TransSendTC(TRUE);  // ��TC
		memcpy(gstSendPack.szSTAN,szStanBak,sizeof(gstSendPack.szSTAN));
		gstTransInfo.stTranLog.ulSTAN = ulSTANBak;
		DisplayTransName();
	}

	CommOnHook(FALSE);
	if( ChkIfPrnReceipt() )
	{
		DisplayTransName();
		if( gstTransInfo.stTranLog.ucTranType!=VOID )
		{
			GetNewInvoiceNo();
		}
		if (gstPosCapability.uiPrnType != 0)
		{
#ifdef _APP_SUPP_ELECSIGN  // ����ǩ��  
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

#else

	if( ChkIfPrnReceipt() )
	{
		DisplayTransName();
		if( gstTransInfo.stTranLog.ucTranType!=VOID )
		{
			GetNewInvoiceNo();
		}
		if (gstPosCapability.uiPrnType != 0)
		{
#ifdef _APP_SUPP_ELECSIGN  // ����ǩ��  
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

	if( gstTransInfo.stTranLog.ucTranType!=BALANCE_BOC &&
		gstTransInfo.stTranLog.ucTranType!=BALANCE_SCB )
	{
		// ����TC & �������������޸������������������ȱ����ٻָ�
		memcpy(szStanBak,gstSendPack.szSTAN,sizeof(gstSendPack.szSTAN));
		ulSTANBak = gstTransInfo.stTranLog.ulSTAN;
		OfflineSend();  // ������
		TransSendTC(TRUE);  // ��TC
		memcpy(gstSendPack.szSTAN,szStanBak,sizeof(gstSendPack.szSTAN));
		gstTransInfo.stTranLog.ulSTAN = ulSTANBak;
		DisplayTransName();
	}
	
	// �������Ҫ����Ľ��ײ��Ҵ��˽��׺��Զ����������
	if (ChkIfSaveLog() && ChkEdcOption(EDC_AUTO_SETTLEMENT))
	{
		TransAutoSettlement();
	}

	CommOnHook(FALSE);

	DisplayResult(0);
#endif

	return 0;
}

// voice referral dialing
INT32 ReferralDial(uint8_t *pszPhoneNo)
{
	return YES;
	/*
	uint8_t	ucRet, szTelNo[50];

	if( pszPhoneNo==NULL || *pszPhoneNo==0 )
	{
		Display2StringInRect("�绰�������", "TEL# ERROR");
		PubBeepErr();
		PubWaitKey(5);
		return ERR_NO_DISP;
	}

	sprintf((char*)szTelNo, "%s%s.", gstSysParam.stEdcInfo.szPabx, pszPhoneNo);
	while( 1 )
	{
		ucRet = ModemDial(NULL, szTelNo, 1);
		if( ucRet==0x83 )
		{	// ���õ绰�����ߵ绰������(�����ڷ���ת�˹�������ʽ)
			Display2StringInRect("������绰��  ", "PLS USE PHONE ");
			PubBeepErr();
		}
		sysDelayMs(1000);
		if( ucRet==0x06 || ucRet==0x00 || PubWaitKey(0)== KEY_ENTER )
		{
			return 0;
		}
		if( ucRet==0x0D )
		{
			Display2StringInRect("�Է���·æ", "LINE BUSY");
			OnHook();
			PubWaitKey(5);
			return ERR_NO_DISP;
		}
		PubWaitKey(1);
	}
	*/
}

// ����ͨѶ����(ֻ������Modem�绰����)
INT32 AdjustCommParam(void)
{
	char	szNewTelNo[100+1];
	char	szRemoteIP[15+1], szRemotePort[5+1];
	
	if (gstTransInfo.stTranLog.ucTranType == DOWNLOAD)
	{
		memcpy(&gstCurCommCfg,&gstSysParam.stDownParamCommCfg,sizeof(ST_COMM_CONFIG));
	}
	else
	{
		memcpy(&gstCurCommCfg,&gstCurAcq.stTxnCommCfg,sizeof(ST_COMM_CONFIG));

		if( gstCurCommCfg.ucCommType ==CT_MODEM )
		{
			switch(gstTransInfo.ucConnectCnt)
			{
			case 1:
				sprintf((char *)szNewTelNo, "%s%s", gstSysParam.stEdcInfo.szPabx, gstCurAcq.szTxnTelNo1);
				if (gstCurAcq.szTxnTelNo2[0] != 0)
				{
					sprintf((char *)szNewTelNo, "%s%s", gstSysParam.stEdcInfo.szPabx, gstCurAcq.szTxnTelNo2);
				}
				break;
			case 0:
			default:
				sprintf((char *)szNewTelNo, "%s%s", gstSysParam.stEdcInfo.szPabx, gstCurAcq.szTxnTelNo1);
				break;
			}
			sprintf((char *)gstCurCommCfg.stPSTNPara.szTxnTelNo, "%s", szNewTelNo);  
		}
		else
		{
			if (gstTransInfo.ucConnectCnt != 0 && gstCurAcq.szRemoteIP2[0] != 0)
			{
				memcpy(szRemoteIP,gstCurAcq.szRemoteIP2,sizeof(gstCurAcq.szRemoteIP2));
				memcpy(szRemotePort,gstCurAcq.szRemotePort2,sizeof(gstCurAcq.szRemotePort2));
			}
			else
			{
				memcpy(szRemoteIP,gstCurAcq.szRemoteIP,sizeof(gstCurAcq.szRemoteIP));
				memcpy(szRemotePort,gstCurAcq.szRemotePort,sizeof(gstCurAcq.szRemotePort));
			}
			
			sprintf((char *)gstCurCommCfg.stTcpIpPara.szRemoteIP,"%s",szRemoteIP);
			sprintf((char *)gstCurCommCfg.stTcpIpPara.szRemotePort,"%s",szRemotePort);
			sprintf((char *)gstCurCommCfg.stWIFIIpPara.szRemoteIP,"%s",szRemoteIP);
			sprintf((char *)gstCurCommCfg.stWIFIIpPara.szRemotePort,"%s",szRemotePort);
			sprintf((char *)gstCurCommCfg.stWirlessPara.szRemoteIP,"%s",szRemoteIP);
			sprintf((char *)gstCurCommCfg.stWirlessPara.szRemotePort,"%s",szRemotePort);
		}

	}
	
	return CommSetCfgParam(&gstCurCommCfg);
}

void DispCommErrMsg(INT32 iErrCode)
{
	COMM_ERR_MSG	stCommErrMsg;

	CommGetErrMsg(iErrCode, &stCommErrMsg);
	Display2Strings((char*)stCommErrMsg.szChnMsg, (char*)stCommErrMsg.szEngMsg);
	PubBeepErr();
	PubWaitKey(5);
}

// end of file
