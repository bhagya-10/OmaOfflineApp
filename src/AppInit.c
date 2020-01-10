
#include "global.h"
#include "Param.h"

/* ********************* Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
#ifdef _NO_TMS_
void InitEdcParam(void);
#endif

/********************** Internal variables declaration *********************/
/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

// 设置EDC缺省参数
void LoadEdcDefault(void)
{

	ResetAllPara(TRUE);

#ifdef _NO_TMS_
	InitEdcParam();
	SaveAppTransParam();
#endif

	LoadCtrlDefault();
		
	memset(&gstEmvStatus, 0, sizeof(EMV_STATUS));
	SaveEmvStatus();
}


void LoadCtrlDefault()
{
	int			iCnt;
	
	memset(&gstSysCtrlParam, 0, sizeof(ST_SYSCONTROL_PARAM));
	gstSysCtrlParam.ulInvoiceNo = 1L;
	gstSysCtrlParam.ulSTAN      = 1L;
	gstSysCtrlParam.uiLastRecNo = 0xFFFF;
	gstSysCtrlParam.uiErrLogNo  = 0;
	for(iCnt=0; iCnt<MAX_ACQ; iCnt++)
	{
		gstSysCtrlParam.sAcqStatus[iCnt] = S_USE;
		gstSysCtrlParam.uiLastRecNoList[iCnt] = 0xFFFF;
	}
	
	for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
	{
		gstSysCtrlParam.sAcqKeyList[iCnt]    = INV_ACQ_KEY;		// set to invalid acquirer key
		gstSysCtrlParam.sIssuerKeyList[iCnt] = INV_ISSUER_KEY;	// set to invalid issuer key
	}
	gstSysCtrlParam.stWriteInfo.bNeedSave = FALSE;

	GetDateTime((uint8_t *)gstSysCtrlParam.szLastSettlementDate);
		
	SaveSysCtrlAll();	
}

void AdjustParam(void)
{
	if (gstSysParam.stEdcInfo.bClearBatch)
	{
		ClearRecord(ACQ_ALL);
		ClearElecSignaturefile();
		fileMakeDir(STR_ELECSIGNATURE_FILE_PATH);  // 创建用于保存电子签名图片的目录,没有就创建
		if(gstSysParam.stEdcInfo.ulAcceptTimeout<5)
			SetElecSignTimeOut(40*1000);
		else
			SetElecSignTimeOut(gstSysParam.stEdcInfo.ulAcceptTimeout*1000);
	}
	
	if (memcmp(gstSysParam.stEdcInfo.sInitSTAN,"\x00\x00\x00",3)==0)
	{
		memcpy(gstSysParam.stEdcInfo.sInitSTAN,"\x00\x00\x10",3);
	}
	PubBcd2Long(gstSysParam.stEdcInfo.sInitSTAN,3,&gstSysCtrlParam.ulSTAN);
	
	if (gstSysParam.uiAcqNum >=1 &&
		gstSysParam.uiIssuerNum >= 1 &&
		gstSysParam.uiCardNum >=1)
	{
		gstSysParam.uiAppStatus = LOGON_STATUS;
	}

	// 如果是modem通讯，并且没有设置外线号码，默认添加"w"--马来西亚客户要求
	if ( (strlen(gstSysParam.stEdcInfo.szPabx)==0) && (gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType&CT_MODEM))
	{
		strcpy(gstSysParam.stEdcInfo.szPabx,"W");
	}
}


void LoadDefCommPara(uint8_t ucAcqIndex)
{
	ST_ACQUIRER_INFO   *pstAcq;

	if (ucAcqIndex == ACQ_ALL)
	{
		gstSysParam.stDownParamCommCfg.ucCommType = gstPosCapability.uiCommType;
		gstSysParam.stDownParamCommCfg.stRS232Para.ucPortNo   = PORT_COM1;
		gstSysParam.stDownParamCommCfg.stRS232Para.ucSendMode = CM_SYNC;
		sprintf((char *)gstSysParam.stDownParamCommCfg.stRS232Para.szAttr, "9600,8,n,1");
		return ;
	}

	if (ucAcqIndex >= MAX_ACQ)
	{
		return;
	}

	pstAcq = &gstSysParam.stAcqList[ucAcqIndex];
	// 默认通讯类型
	pstAcq->stTxnCommCfg.ucCommType  = gstPosCapability.uiCommType;
	pstAcq->stTxnCommCfg.stRS232Para.ucPortNo   = PORT_COM1;
	pstAcq->stTxnCommCfg.stRS232Para.ucSendMode = CM_SYNC;
	sprintf((char *)pstAcq->stTxnCommCfg.stRS232Para.szAttr, "9600,8,n,1");
	// TCP/IP缺省参数
	sprintf((char *)pstAcq->stTxnCommCfg.stTcpIpPara.szGatewayIP, "192.168.18.1");
	sprintf((char *)pstAcq->stTxnCommCfg.stTcpIpPara.szRemoteIP,  "192.168.18.234");
	sprintf((char *)pstAcq->stTxnCommCfg.stTcpIpPara.szRemotePort, "9000");

	sprintf((char *)pstAcq->stTxnCommCfg.stWirlessPara.szAPN, "cmnet");
	sprintf((char *)pstAcq->stTxnCommCfg.stWirlessPara.szRemoteIP,   "192.168.18.34");
	sprintf((char *)pstAcq->stTxnCommCfg.stWirlessPara.szRemotePort, "9000");
	sprintf((char *)pstAcq->stTxnCommCfg.stWirlessPara.szUID, "card");
	sprintf((char *)pstAcq->stTxnCommCfg.stWirlessPara.szPwd, "card");
	pstAcq->stTxnCommCfg.stPSTNPara.ucSendMode = MODEM_COMM_SYNC;
    pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.connect_mode = MODEM_COMM_SYNC;
	pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.connect_speed = 1200;
	pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.dial_mode = MODEM_DAIL_DTMF;
	pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.dial_pause = 1;
	pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.dial_timeo = 30;
	pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.extension_chk = 1;
	pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.region = ModemRegion(Malaysia);
	pstAcq->stTxnCommCfg.stPSTNPara.stDialPara.idle_timeo = 60;
	strcpy(pstAcq->szTxnTelNo1,"0629");
	strcpy(pstAcq->szTxnTelNo2,"0629");
}

void ResetAllPara(uint8_t bFirstTime)
{
	INT32				iCnt;
	ST_COMM_CONFIG      stCommCfgBak;
	unsigned long       ulTimeOut;
	char                szDownTID[9];

	stCommCfgBak = gstSysParam.stDownParamCommCfg;
	ulTimeOut = gstSysParam.stEdcInfo.ulTMSTimeOut;
	sprintf(szDownTID,"%s",gstSysParam.stEdcInfo.szDownLoadTID);

	memset(&gstSysParam, 0, sizeof(ST_SYSTEM_PARAM));
	LoadDefCommPara(ACQ_ALL);

	if (!(stCommCfgBak.ucCommType & CT_ALLCOMM) || (stCommCfgBak.ucCommType > CT_ALLCOMM))
	{
		gstSysParam.stDownParamCommCfg.ucCommType = gstPosCapability.uiCommType;
		sprintf((char *)gstSysParam.stDownParamCommCfg.stWirlessPara.szAPN, "CMNET");
		sprintf((char *)gstSysParam.stDownParamCommCfg.stWirlessPara.szRemoteIP,   "192.168.0.1");
		sprintf((char *)gstSysParam.stDownParamCommCfg.stWirlessPara.szRemotePort, "9006");
		sprintf((char *)gstSysParam.stDownParamCommCfg.stWirlessPara.szUID, "CARD");
		sprintf((char *)gstSysParam.stDownParamCommCfg.stWirlessPara.szPwd, "CARD");
		gstSysParam.stDownParamCommCfg.stRS232Para.ucPortNo   = PORT_COM1;
		gstSysParam.stDownParamCommCfg.stRS232Para.ucSendMode = CM_SYNC;
		sprintf((char *)gstSysParam.stDownParamCommCfg.stRS232Para.szAttr, "9600,8,n,1");
		gstSysParam.stDownParamCommCfg.stPSTNPara.ucSendMode = MODEM_COMM_SYNC;
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.connect_mode = MODEM_COMM_SYNC;
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.connect_speed = 1200;
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.dial_mode = MODEM_DAIL_DTMF;
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.dial_pause = 1;
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.dial_timeo = 30;
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.extension_chk = 1;
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.region = ModemRegion(Malaysia);
		gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara.idle_timeo = 60;
	}
	else
	{
		gstSysParam.stDownParamCommCfg = stCommCfgBak;
		gstSysParam.stEdcInfo.ulTMSTimeOut = ulTimeOut;
		sprintf(gstSysParam.stEdcInfo.szDownLoadTID,"%s",szDownTID);
	}

#ifdef _NO_TMS_
	gstSysParam.uiAppStatus              = TRANS_STATUS;
#else
	gstSysParam.uiAppStatus              = LOGON_STATUS;
#endif
	gstSysParam.stEdcInfo.bPreDial        = TRUE;
	gstSysParam.stEdcInfo.ulAcceptTimeout = 30;
	gstSysParam.stEdcInfo.ulTMSTimeOut    = 60;
	gstSysParam.stEdcInfo.uiPrintNums     = 2;
	gstSysParam.uiAcqNum = 0;
	gstSysParam.uiCardNum = 0;
	gstSysParam.uiIssuerNum = 0;
	gstSysParam.uiCardNum = 0;
	gstSysParam.uiPlanNum = 0;
	gstSysParam.uiDescNum = 0;
	gstSysParam.uiCardBinNum = 0;
	strcpy(gstSysParam.stEdcInfo.szPabx,"W");

	memset(gstSysParam.szPassword, '0', sizeof(gstSysParam.szPassword));
	if( !bFirstTime )
	{
//		sprintf((char *)gstSysParam.stEdcInfo.szDownTelNo,   "%.25s", szDownTelNo);
//		sprintf((char *)gstSysParam.stEdcInfo.szDownLoadTID, "%.8s",  szDownLoadTID);
	}

	for(iCnt=0; iCnt<MAX_ACQ; iCnt++)
	{
		gstSysParam.stAcqList[iCnt].ucKey = INV_ACQ_KEY;
	}
	for(iCnt=0; iCnt<MAX_ISSUER; iCnt++)
	{
		gstSysParam.stIssuerList[iCnt].ucKey = INV_ISSUER_KEY;
	}

	SaveAppTransParam();

}


#ifdef _NO_TMS_
void InitEdcParam(void)
{
	sprintf((char *)gstSysParam.stEdcInfo.szMerchantName, "EDC DEFAULT PARAMEER");
	sprintf((char *)gstSysParam.stEdcInfo.szMerchantAddr, "TEST MERCHANT ADDR1");
	sprintf((char *)&gstSysParam.stEdcInfo.szMerchantAddr[40], "ADDRESS LINE 2");
	sprintf((char *)&gstSysParam.stEdcInfo.szDownLoadTID, "00000000");
	memcpy(gstSysParam.stEdcInfo.sOption, "\xE6\xFF\x08\x09\x00", 5);
	sprintf((char *)gstSysParam.stEdcInfo.szCurrencyName, "RM");
	gstSysParam.stEdcInfo.uiTranAmtLen     = 10;
	gstSysParam.stEdcInfo.uiDecimalPos     = 2;
	gstSysParam.stEdcInfo.ChCurrencySymbol = '$';
	strcpy(gstSysParam.stEdcInfo.szPabx,"W");

	// set acquirer parameters
	gstSysCtrlParam.sAcqStatus[0] = S_USE;
	gstSysParam.stAcqList[0].ucKey   = 0x01;
	gstSysParam.stAcqList[0].ucIndex = 0;
	sprintf((char *)gstSysParam.stAcqList[0].szNii, "019");
	sprintf((char *)gstSysParam.stAcqList[0].szName, "VISA_ACQ");
	sprintf((char *)gstSysParam.stAcqList[0].szPrgName, "VISA");
	sprintf((char *)gstSysParam.stAcqList[0].szTermID, "00000001");
	sprintf((char *)gstSysParam.stAcqList[0].szMerchantID, "123456789012345");
	memcpy(gstSysParam.stAcqList[0].sOption, "\x06\x20\x0E\x00", 4);
	gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType  = gstPosCapability.uiCommType & ~CT_RS232;
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stTcpIpPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stTcpIpPara.szRemotePort, "8000");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stWirlessPara.szAPN,  "CMNET");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stWirlessPara.szUID, "CARD");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stWirlessPara.szPwd, "CARD");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stWirlessPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stWirlessPara.szRemotePort, "8000");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stWIFIIpPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stWIFIIpPara.szRemotePort, "8000");
	strcpy((char *)gstSysParam.stAcqList[0].stTxnCommCfg.stRS232Para.szAttr,"115200,8,n,1");
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.ucSendMode = MODEM_COMM_SYNC;
    gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.connect_mode = MODEM_COMM_SYNC;
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.connect_speed = 1200;
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.dial_mode = MODEM_DAIL_DTMF;
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.dial_pause = 1;
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.dial_timeo = 30;
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.extension_chk = 1;
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.region = ModemRegion(Malaysia);
	gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara.stDialPara.idle_timeo = 60;
	strcpy(gstSysParam.stAcqList[0].szTxnTelNo1,"0629");
	strcpy(gstSysParam.stAcqList[0].szTxnTelNo2,"0629");
	gstSysParam.stAcqList[0].ulCurBatchNo  = 1L;
	gstSysParam.stAcqList[0].ulNextBatchNo = 2L;
	gstSysParam.stAcqList[0].ulTimeOut = 30;
	memcpy(gstSysParam.stAcqList[0].sIssuerKey, "\x01", 1);// visa master

	gstSysCtrlParam.sAcqStatus[1] = S_USE;
	gstSysParam.stAcqList[1].ucKey   = 0x02;
	gstSysParam.stAcqList[1].ucIndex = 1;
	sprintf((char *)gstSysParam.stAcqList[1].szNii, "003");
	sprintf((char *)gstSysParam.stAcqList[1].szName, "Master");
	sprintf((char *)gstSysParam.stAcqList[1].szPrgName, "Master");
	sprintf((char *)gstSysParam.stAcqList[1].szTermID, "00000002");
	sprintf((char *)gstSysParam.stAcqList[1].szMerchantID, "222222222222222");
	memcpy(gstSysParam.stAcqList[1].sOption, "\xC6\x20\x0E\x00", 4);
	gstSysParam.stAcqList[1].stTxnCommCfg.ucCommType  = gstPosCapability.uiCommType & ~CT_RS232;
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stTcpIpPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stTcpIpPara.szRemotePort, "8000");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stWirlessPara.szAPN,  "CMNET");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stWirlessPara.szUID, "CARD");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stWirlessPara.szPwd, "CARD");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stWirlessPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stWirlessPara.szRemotePort, "8000");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stWIFIIpPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stWIFIIpPara.szRemotePort, "8000");
	strcpy((char *)gstSysParam.stAcqList[1].stTxnCommCfg.stRS232Para.szAttr,"115200,8,n,1");
	gstSysParam.stAcqList[1].stTxnCommCfg.stPSTNPara = gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara;
	strcpy(gstSysParam.stAcqList[1].szTxnTelNo1,"0629");
	strcpy(gstSysParam.stAcqList[1].szTxnTelNo2,"0629");

	gstSysParam.stAcqList[1].ulCurBatchNo = 1L;
	gstSysParam.stAcqList[1].ulNextBatchNo = 2L;
	gstSysParam.stAcqList[1].ulTimeOut = 30;
	memcpy(gstSysParam.stAcqList[1].sIssuerKey, "\x02", 1); // AMEX

	gstSysCtrlParam.sAcqStatus[2] = S_USE;
	gstSysParam.stAcqList[2].ucKey   = 0x03;
	gstSysParam.stAcqList[2].ucIndex = 1;
	sprintf((char *)gstSysParam.stAcqList[2].szNii, "003");
	sprintf((char *)gstSysParam.stAcqList[2].szName, "Other");
	sprintf((char *)gstSysParam.stAcqList[2].szPrgName, "Other");
	sprintf((char *)gstSysParam.stAcqList[2].szTermID, "00000003");
	sprintf((char *)gstSysParam.stAcqList[2].szMerchantID, "111111111111111");
	memcpy(gstSysParam.stAcqList[2].sOption, "\xC6\x20\x0E\x00", 4);
	gstSysParam.stAcqList[2].stTxnCommCfg.ucCommType  = gstPosCapability.uiCommType & ~CT_RS232;
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stTcpIpPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stTcpIpPara.szRemotePort, "8000");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stWirlessPara.szAPN,  "CMNET");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stWirlessPara.szUID, "CARD");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stWirlessPara.szPwd, "CARD");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stWirlessPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stWirlessPara.szRemotePort, "8000");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stWIFIIpPara.szRemoteIP,  "192.168.0.1");
	sprintf((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stWIFIIpPara.szRemotePort, "8000");
	strcpy((char *)gstSysParam.stAcqList[2].stTxnCommCfg.stRS232Para.szAttr,"115200,8,n,1");
	gstSysParam.stAcqList[2].stTxnCommCfg.stPSTNPara = gstSysParam.stAcqList[0].stTxnCommCfg.stPSTNPara;
	strcpy(gstSysParam.stAcqList[2].szTxnTelNo1,"0629");
	strcpy(gstSysParam.stAcqList[2].szTxnTelNo2,"0629");
	gstSysParam.stAcqList[2].ulCurBatchNo = 1L;
	gstSysParam.stAcqList[2].ulNextBatchNo = 2L;
	gstSysParam.stAcqList[2].ulTimeOut = 30;
	memcpy(gstSysParam.stAcqList[2].sIssuerKey, "\x03", 1); // AMEX

	gstSysParam.uiAcqNum = 3;

	// set issuer parameters
	gstSysParam.stIssuerList[0].ucKey = 0x01;
	sprintf((char *)gstSysParam.stIssuerList[0].szRefTelNo, "86");
	sprintf((char *)gstSysParam.stIssuerList[0].szName, "VISA");
	memcpy(gstSysParam.stIssuerList[0].sOption, "\xFC\x1E\x11\x00", 4);
	memcpy(gstSysParam.stIssuerList[0].sPanMask, "\xFF\xFF\xFF", 3);
	gstSysParam.stIssuerList[0].ulFloorLimit    = 1000;
	gstSysParam.stIssuerList[0].ucAdjustPercent = 10;

	gstSysParam.stIssuerList[1].ucKey = 0x02;
	sprintf((char *)gstSysParam.stIssuerList[1].szRefTelNo, "86");
	sprintf((char *)gstSysParam.stIssuerList[1].szName, "MASTER");
	memcpy(gstSysParam.stIssuerList[1].sOption, "\xFC\x1E\x11\x00", 4);
	memcpy(gstSysParam.stIssuerList[1].sPanMask, "\xFF\xFF\xFF", 3);
	gstSysParam.stIssuerList[1].ulFloorLimit    = 1000;
	gstSysParam.stIssuerList[1].ucAdjustPercent = 10;

	gstSysParam.stIssuerList[2].ucKey = 0x03;
	sprintf((char *)gstSysParam.stIssuerList[2].szRefTelNo, "86");
	sprintf((char *)gstSysParam.stIssuerList[2].szName, "Other");
	memcpy(gstSysParam.stIssuerList[2].sOption, "\xFE\x1F\x11\x00", 4);
	memcpy(gstSysParam.stIssuerList[2].sPanMask, "\xFF\xFF\xFF", 3);
	gstSysParam.stIssuerList[2].ulFloorLimit    = 1000;
	gstSysParam.stIssuerList[2].ucAdjustPercent = 10;

	gstSysParam.uiIssuerNum = 3;

	// set card range
	memcpy(gstSysParam.stCardTable[0].sPanRangeLow,  "\x40\x00\x00\x00\x00", 5);
	memcpy(gstSysParam.stCardTable[0].sPanRangeHigh, "\x49\x99\x99\x99\x99", 5);
	gstSysParam.stCardTable[0].ucIssuerKey = 0x01;
	gstSysParam.stCardTable[0].ucAcqKey    = 0x01;
	gstSysParam.stCardTable[0].ulPanLength = 0;
	gstSysParam.stCardTable[0].ChOption    = 0x02;

	memcpy(gstSysParam.stCardTable[1].sPanRangeLow,  "\x50\x00\x00\x00\x00", 5);
	memcpy(gstSysParam.stCardTable[1].sPanRangeHigh, "\x59\x99\x99\x99\x99", 5);
	gstSysParam.stCardTable[1].ucIssuerKey = 0x02;
	gstSysParam.stCardTable[1].ucAcqKey    = 0x02;
	gstSysParam.stCardTable[1].ulPanLength = 0;
	gstSysParam.stCardTable[1].ChOption    = 0x02;

	memcpy(gstSysParam.stCardTable[2].sPanRangeLow,  "\x10\x00\x00\x00\x00", 5);
	memcpy(gstSysParam.stCardTable[2].sPanRangeHigh, "\x39\x99\x99\x99\x99", 5);
	gstSysParam.stCardTable[2].ucIssuerKey = 0x03;
	gstSysParam.stCardTable[2].ucAcqKey    = 0x03;
	gstSysParam.stCardTable[2].ulPanLength = 0;
	gstSysParam.stCardTable[2].ChOption    = 0x02;

	memcpy(gstSysParam.stCardTable[3].sPanRangeLow,  "\x60\x00\x00\x00\x00", 5);
	memcpy(gstSysParam.stCardTable[3].sPanRangeHigh, "\x99\x99\x99\x99\x99", 5);
	gstSysParam.stCardTable[3].ucIssuerKey = 0x03;
	gstSysParam.stCardTable[3].ucAcqKey    = 0x03;
	gstSysParam.stCardTable[3].ulPanLength = 0;
	gstSysParam.stCardTable[3].ChOption    = 0x02;
	gstSysParam.uiCardNum = 4;

	gstSysParam.uiAppStatus         = LOGON_STATUS;
}
#endif


void RemoteDownLoad(void)
{
	int iRet;
    tmsctrl_t stDownCtrl;
	char   szPwd[50],szTID[17];
	int   ibacklight_set_time_bak;

	iRet = TransInit(DOWNLOAD);
	if( iRet!=0 )
	{
		return;
	}

	lcdCls();
	DisplayTransName();
	if( CheckTermPWD()!=0 )
	{
		return ;
	}

	lcdCls();
	DisplayTransName();
	if (!ChkIfNeedSettle())  // 提示警告信息
	{
		return;
	}

    memset(&stDownCtrl, 0x00, sizeof(stDownCtrl));
    stDownCtrl.protocol = TMS_PROTO_NEWTMS;
    stDownCtrl.prompt = OFF;
    stDownCtrl.enbreak = YES;

    lcdCls();
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		lcdDisplay(0, 0, DISP_CFONT|DISP_INVLINE|DISP_MEDIACY, "   PLS SELECT   ");
		lcdDisplay(0, 2, DISP_CFONT, "0:DEFAULT KEY  ");
		lcdDisplay(0, 4, DISP_CFONT, "1:MANUAL KEY  ");
	}
	else
	{
		lcdDisplay(0, 0, DISP_CFONT|DISP_INVLINE|DISP_MEDIACY, " 请选择下载密钥 ");
		lcdDisplay(0, 2, DISP_CFONT, "0:使用默认密钥  ");
		lcdDisplay(0, 4, DISP_CFONT, "1:输入下载密钥  ");
	}
	lcdFlip();
    while(1)
    {
        iRet = kbGetKey();
        if( iRet==KEY0 || iRet==KEY1 )
        {
            break;
        }
        else if( iRet==KEY_CANCEL )
        {
            return ;
        }
    }
    if( iRet==KEY0 )
    {
        stDownCtrl.specifiedtmk = NO;
    }
    else if( iRet==KEY1 )
    {
        stDownCtrl.specifiedtmk = YES;
        while(1)
        {
			lcdCls();
			DisplayTransName();
			lcdDisplay(0, 2, DISP_CFONT, "Input Key(32/48):");
            memset(szPwd, 0x00, sizeof(szPwd));
			iRet = PubGetString(NUM_IN|ECHO_IN, 32,48, (uint8_t *)szPwd, USER_OPER_TIMEOUT);
//            iRet = kbGetString(KB_BIG_ALPHA, 32, 48, (USER_OPER_TIMEOUT*1000), szPwd);
            if( iRet != 0 )
            {
                return ;
            }
            if( iRet==32 || iRet==48 )
            {
                stDownCtrl.tmklen = iRet/2;
                PubAsc2Bcd(szPwd, iRet, (char *)stDownCtrl.tmk);
                break;
            }
        }
    }

	lcdCls();
	DisplayTransName();
	PubDispString("Terminal ID:",      DISP_LINE_CENTER|4);
	memset(szTID,0,sizeof(szTID));
	sprintf(szTID,"%s",gstSysParam.stEdcInfo.szDownLoadTID);
	if (strlen(szTID)==0)
	{
		sysReadSN(szTID);
	}
	PubGetString(NUM_IN|ECHO_IN, 0, 8, (uint8_t *)szTID, USER_OPER_TIMEOUT);
	if (strlen(szTID)==8)
	{
		sprintf(gstSysParam.stEdcInfo.szDownLoadTID,"%s",szTID);
		sprintf((char *)stDownCtrl.termID,"%s",szTID);
	}

	lcdCls();
	DisplayTransName();		
	iRet = SetAcqCommParam(DOWNLAOD_COMMTYPE);
	if (iRet != 0)
	{
		Display2StringInRect("用户取消", "USER CANCEL");
		PubWaitKey(3);
		return ;
	}

	switch (gstSysParam.stDownParamCommCfg.ucCommType)
	{
	case CT_RS232:
		stDownCtrl.commtype = TMS_COMM_UART;
		stDownCtrl.commport = gstSysParam.stDownParamCommCfg.stRS232Para.ucPortNo;
		break;
	case CT_GPRS:
	case CT_CDMA:
		if (gstSysParam.stDownParamCommCfg.ucCommType==CT_CDMA)
			stDownCtrl.commtype = TMS_COMM_CDMA;
		else
			stDownCtrl.commtype = TMS_COMM_GPRS;		
		break;
	case CT_TCPIP:
	case CT_WIFI:
			stDownCtrl.commtype = TMS_COMM_GPRS;		
		break;
	default:
		lcdClrLine(2,7);
		Display2StringInRect("不支持该通讯方式", "NOT SUPPORT!");
		PubBeepErr();
		PubWaitKey(3);
		return;
		break;
	}

	gstCurAcq.stTxnCommCfg = gstSysParam.stDownParamCommCfg;
	iRet = ConnectHost();
	if (iRet != 0)
	{
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDispString("connecet failure", 3|DISP_LINE_CENTER);
			PubDispString("Try it again!", 5|DISP_LINE_CENTER);
		}
		else
		{
			PubDispString("连接NEWTMS失败", 3|DISP_LINE_CENTER);
			PubDispString("请重试!", 5|DISP_LINE_CENTER);
		}
		PubWaitKey(5);
		return;
	}

	if (gstSysParam.stDownParamCommCfg.ucCommType != CT_RS232 && 
		gstSysParam.stDownParamCommCfg.ucCommType != CT_MODEM )
	{
		stDownCtrl.commport = GetCurSocket();
	}
	
	start_timer(30, 0);
	sysGetbacklighttime(&ibacklight_set_time_bak);
	sysSetbacklighttime(-1); // 常亮
    iRet = tmsDownload(&stDownCtrl);
	stop_timer();
	sysSetbacklighttime(ibacklight_set_time_bak);
	if (iRet<0)
	{
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDispString("Update failure", 3|DISP_LINE_CENTER);
			PubDispString("Try it again!", 5|DISP_LINE_CENTER);
		}
		else
		{
			PubDispString("远程下载失败", 3|DISP_LINE_CENTER);
			PubDispString("请重试!", 5|DISP_LINE_CENTER);
		}
		PubWaitKey(5);
		CommOnHook(TRUE);
		return;
	}
	else
	{
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("Update successful", 3|DISP_LINE_CENTER);
		else
			PubDispString("远程下载成功", 3|DISP_LINE_CENTER);
		PubWaitKey(5);
	}
	CommOnHook(TRUE);

	ResetSysAfterRemoteDown();

	return ;
}

void  ResetSysAfterRemoteDown(void)
{
	int iRet,iUpdateNum,iRemainNum;

	kbFlush();
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayTitle(TRUE,"Update system");
		lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "Are you sure");
		lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "update task?");
		PubDispString("[CANCEL] [ENTER]", DISP_LINE_CENTER|6);
	}
	else
	{
		PubDisplayTitle(TRUE,"系统更新");
		lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "是否更新已经");
		lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "下载任务? ");
		PubDispString("[取消]  [确认]", DISP_LINE_CENTER|6);
	}

	if(!PubYesNo(USER_OPER_TIMEOUT))
	{
		return ;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		iRet =  tmsUpdate(TMS_PROMPT_EN,&iUpdateNum,&iRemainNum);
	else
		iRet =  tmsUpdate(TMS_PROMPT_CHN,&iUpdateNum,&iRemainNum);
	if (iRet==0 && iUpdateNum >=1)
	{
		if (ExistParamFile())  // 如果下载了参数文件，则先清除原终端配置
		{
			LoadEdcDefault();
		}
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "%d task updated",iUpdateNum);
			lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "Pls restart system");
		}
		else
		{
			lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "更新(%d)个任务",iUpdateNum);
			lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "请重启机器");
		}
		PubWaitKey(USER_OPER_TIMEOUT);		
		sysReset();
	}
	else
	{
		if (iRet == -TMS_ERROR_NOTASK || iUpdateNum==0)
		{
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "Not need update");
			}
			else
			{
				lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "无更新任务");
			}
			PubWaitKey(5);
			return ;
		}

		if (iRet < 0 )
		{
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "Update failure");
				lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "Pls try it again");
				lcdDisplay(0, 6, DISP_CFONT|DISP_MEDIACY, "ERR CODE:%d",iRet);
			}
			else
			{
				lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "更新任务失败");
				lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "请重试更新");
				lcdDisplay(0, 6, DISP_CFONT|DISP_MEDIACY, "ERR CODE:%d",iRet);
			}
			PubWaitKey(5);
			return ;
		}
	}

	return ;
}

void RemoteManager(void)
{
	MenuItem stRemoteMenu_EN[] =
	{
		{TRUE, "Update task",	ResetSysAfterRemoteDown},
		{TRUE, "Delete task",	DeleteRemoteTask},
		{TRUE, "",	NULL},
	};

	MenuItem stRemoteMenu_CH[] =
	{
		{TRUE, "远程下载更新",	ResetSysAfterRemoteDown},
		{TRUE, "删除下载任务",	DeleteRemoteTask},
		{TRUE, "",	NULL},
	};
	
	PubDisplayTitle(TRUE, "Update manager");
	if( CheckTermPWD()!=0 )
	{
		return;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubGetMenu((uint8_t *)"Update manager", stRemoteMenu_EN, MENU_AUTOSNO|MENU_CFONT, USER_OPER_TIMEOUT);
	else
		PubGetMenu((uint8_t *)"远程管理", stRemoteMenu_CH, MENU_AUTOSNO|MENU_CFONT, USER_OPER_TIMEOUT);
}


void DeleteRemoteTask(void)
{
	int iRet;

	kbFlush();
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayTitle(TRUE,"Delete task");
		lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "Are you sure");
		lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "delete task?");
		PubDispString("[CANCEL] [ENTER]", DISP_LINE_CENTER|6);
	}
	else
	{
		PubDisplayTitle(TRUE,"删除任务");
		lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "是否删除已经");
		lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "下载任务? ");
		PubDispString("[取消]  [确认]", DISP_LINE_CENTER|6);
	}
	if(!PubYesNo(USER_OPER_TIMEOUT))
	{
		return ;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		iRet = tmsRemoveTask(TMS_PROMPT_EN);
	else
		iRet = tmsRemoveTask(TMS_PROMPT_CHN);
	if (iRet < 0 )
	{
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "Delete failure");
			lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "Pls try it again");
			lcdDisplay(0, 6, DISP_CFONT|DISP_MEDIACY, "ERR CODE:%d",iRet);
		}
		else
		{
			lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "删除任务失败");
			lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "请重试操作");
			lcdDisplay(0, 6, DISP_CFONT|DISP_MEDIACY, "ERR CODE:%d",iRet);
		}
		PubWaitKey(5);
	}
	else
	{
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "Delete successful");
		}
		else
		{
			lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "删除任务成功");
		}
		PubWaitKey(5);
	}

	return;
}
// end of file
