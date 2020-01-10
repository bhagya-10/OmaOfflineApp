
#include "global.h"


// 设置系统参数
void SetSystemParam(void)
{
	int	iKey;

	PubDisplayTitle(TRUE,"Setup param");
	if( CheckBankPWD()!=0 )
	{
		return;
	}

	while( 1 )
	{
		PubDisplayTitle(TRUE,"Setup param");
		PubDispString("1.SETUP EDC",      DISP_LINE_LEFT|2);
		PubDispString("2.SETUP ACQUIRER", DISP_LINE_LEFT|4);

		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			return;
		}
		else if( iKey==KEY1 )
		{
			SetEdcParam();
		}
		else if( iKey==KEY2 )
		{
			SetAcqParam();
		}
	}
}


void DeleteAllDataFile(void)
{
    FILE_INFO asFileInfo[256];
    int iRet, i;

    lcdCls();
    lcdDisplay(0, 0, 1, "Delete data file!");
    lcdDisplay(0, 2, 1, "Are you sure?");
    lcdDisplay(0, 4, 1, "[ENTER]/[CANCEL]");
	lcdFlip();
    if (KEY_ENTER != kbGetKey())
    {
        return ;
    }
    iRet = fileGetInfo(asFileInfo);
    if (iRet > 0)
    {
        for (i=0; i<iRet; i++)
        {
//            lcdDisplay(0, 2, 1, "%d,", i);
 //           lcdDisplay(0, 4, 1, "%s", asFileInfo[i].name);
            if (   (FILE_TYPE_DATA == asFileInfo[i].type)
                || (FILE_TYPE_PARA == asFileInfo[i].type))
            {
                fileRemove(asFileInfo[i].name);
            }
            if (FILE_TYPE_APP == asFileInfo[i].type)
            {
                fileRemoveExt(asFileInfo[i].name, 0xff, FILE_TYPE_APP);
            }
        }
    }
    sysReset();
}


// 设置系统参数
void SetExternParam(void)
{
	int	iKey;

	PubDisplayTitle(TRUE,"Setup param2");
	if( CheckBankPWD2()!=0 )
	{
		return;
	}

	while( 1 )
	{
		PubDisplayTitle(TRUE,"Setup param2");
		PubDispString("1.SETUP ISSUER",   DISP_LINE_LEFT|2);
		PubDispString("2.Initial All", DISP_LINE_LEFT|4);

		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			return;
		}
		else if( iKey==KEY1 )
		{
			SetIssuerParam();
		}
		else if( iKey==KEY2 )
		{
			DeleteAllDataFile();
		}
	}
}

// 输入拨号号码前缀
int SetExtLineNo(void)
{
	PubDisplayTitle(TRUE, "PABX NO.");
	PubDispString("PABX NO.", DISP_LINE_LEFT|2);
	if( PubGetString(ALPHA_IN|ECHO_IN, 0, 10, (uint8_t *)gstSysParam.stEdcInfo.szPabx, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}

	return 0;
}

// 修改Modem参数
int SetModemParam(void)
{
	int   iKey, iTemp, iCnt;
	char  szBuff[20], ucTemp;
	static	MenuItem stBateRateMenu[20];
	int   sBateRateList[20];

/*	PubDisplayTitle(TRUE, (uint8_t *)"DIAL PARA");
	ucTemp = gstSysParam.stEdcInfo.bPreDial; 
	while( 1 )
	{
		lcdClrLine(2, 7);
		PubDispString("PRE DIAL", DISP_LINE_CENTER|2);
		sprintf((char *)szBuff, "(%c)", ucTemp ? '1' : '0');
		PubDispString(szBuff,       DISP_LINE_CENTER|4);
		PubDispString("1.ON 0.OFF", DISP_LINE_CENTER|6);
		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			return ERR_USERCANCEL;
		}
		else if( iKey==KEY_ENTER )
		{
			break;
		}
		else if ( iKey==KEY0 )
		{
			ucTemp = FALSE;
		}
		else if ( iKey==KEY1 )
		{
			ucTemp = TRUE;
		}
	}
	gstSysParam.stEdcInfo.bPreDial = ucTemp; */

	SetCurAcq(0);
	PubDisplayTitle(TRUE, "DIAL PARA");
	ucTemp = gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.connect_mode;
	while( 2 )
	{
		lcdClrLine(2, 7);
		sprintf((char *)szBuff, "(%c)",  ucTemp ? '1' : '0');
		PubDispString("CONNECT MODE",      DISP_LINE_CENTER|2);
		PubDispString(szBuff,           DISP_LINE_CENTER|4);
		PubDispString("0.ASYNC 1.SYNC", DISP_LINE_CENTER|6);
		
		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			return ERR_USERCANCEL;
		}
		else if( iKey== KEY_ENTER )
		{
			break;
		}
		else if ( iKey==KEY0 )
		{
			ucTemp = MODEM_COMM_ASYNC;
		}
		else if ( iKey==KEY1 )
		{
			ucTemp = MODEM_COMM_SYNC;
		}
	}
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.connect_mode = ucTemp;

	iTemp = gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.connect_speed;
	sBateRateList[0] = MODEM_CONNECT_1200BPS;		
	sBateRateList[1] = MODEM_CONNECT_2400BPS;
	sBateRateList[2] = MODEM_CONNECT_4800BPS;
	sBateRateList[3] = MODEM_CONNECT_7200BPS;
	sBateRateList[4] = MODEM_CONNECT_9600BPS;
	sBateRateList[5] = MODEM_CONNECT_12000BPS;
	sBateRateList[6] = MODEM_CONNECT_14400BPS;
	sBateRateList[7] = MODEM_CONNECT_16800BPS;
	sBateRateList[8] = MODEM_CONNECT_19200BPS;
	sBateRateList[9] = MODEM_CONNECT_21600BPS;
	sBateRateList[10] = MODEM_CONNECT_24000BPS;
	sBateRateList[11] = MODEM_CONNECT_26400BPS;
	sBateRateList[12] = MODEM_CONNECT_28800BPS;
	sBateRateList[13] = MODEM_CONNECT_31200BPS;
	sBateRateList[14] = MODEM_CONNECT_33600BPS;
	sBateRateList[15] = MODEM_CONNECT_48000BPS;
	sBateRateList[16] = MODEM_CONNECT_56000BPS;
	memset(stBateRateMenu,0,sizeof(stBateRateMenu));
	for (iCnt=0; iCnt<= 16; iCnt++)
	{
		stBateRateMenu[iCnt].bVisible = TRUE;
		if (sBateRateList[iCnt] == iTemp)
			strcpy(szBuff,">>");
		else
			strcpy(szBuff,"  ");

		sprintf(stBateRateMenu[iCnt].szMenuName,"%s%d",szBuff,sBateRateList[iCnt]);
		stBateRateMenu[iCnt].pfMenuFunc = NULL;
	}
	stBateRateMenu[iCnt].bVisible = FALSE;
	stBateRateMenu[iCnt].pfMenuFunc = NULL;
	stBateRateMenu[iCnt].szMenuName[0] = 0;
	sprintf(szBuff,"RATE(%d)",iTemp);
	iKey = PubGetMenu((uchar*)szBuff, stBateRateMenu, MENU_AUTOSNO|MENU_ASCII, USER_OPER_TIMEOUT);
	if((iKey>=0 && iKey < iCnt))
	{
		gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.connect_speed = sBateRateList[iKey];
	}

	PubDisplayTitle(TRUE, "DIAL PARA");
	ucTemp = gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_mode;
	while( 2 )
	{
		lcdClrLine(2, 7);
		sprintf((char *)szBuff, "(%c)",  ucTemp ? '1' : '0');
		PubDispString("DIAL MODE",      DISP_LINE_CENTER|2);
		PubDispString(szBuff,           DISP_LINE_CENTER|4);
		PubDispString("0.DTMF 1.PULSE", DISP_LINE_CENTER|6);

		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			return ERR_USERCANCEL;
		}
		else if( iKey== KEY_ENTER )
		{
			break;
		}
		else if ( iKey==KEY0 )
		{
			 ucTemp = 0;
		}
		else if ( iKey==KEY1 )
		{
			ucTemp = 1;
		}
	}
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_mode = ucTemp;

	PubDisplayTitle(TRUE, "DIAL PARA");
	lcdClrLine(2, 7);
	sprintf((char *)szBuff, "OLD:%d(s)", gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_pause);
	PubDispString(szBuff,        DISP_LINE_LEFT|2);
	PubDispString("PABX PAUSE:", DISP_LINE_LEFT|4);
	sprintf((char *)szBuff,"%d", gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_pause);
	if( PubGetString(NUM_IN|ECHO_IN, 0, 2, (uchar*)szBuff, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_pause = atoi((char *)szBuff);

	lcdClrLine(2, 7);
	sprintf((char *)szBuff, "OLD:%d(s)", gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_timeo);
	PubDispString(szBuff,       DISP_LINE_LEFT|2);
	PubDispString("DIAL WAIT:", DISP_LINE_LEFT|4);
	sprintf((char *)szBuff, "%d", gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_timeo);
	if( PubGetString(NUM_IN|ECHO_IN, 0, 2, (uchar*)szBuff, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_timeo = atoi((char *)szBuff);

	lcdClrLine(2, 7);
	sprintf((char *)szBuff, "OLD:%d(s)", gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.idle_timeo);
	PubDispString(szBuff,           DISP_LINE_LEFT|2);
	PubDispString("IDLE TIME:", DISP_LINE_LEFT|4);
	sprintf((char *)szBuff, "%d", gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.idle_timeo);
	if( PubGetString(NUM_IN|ECHO_IN, 0, 3, (uchar*)szBuff, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.idle_timeo = atoi((char *)szBuff);

	ucTemp = gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.extension_chk;
	while( 2 )
	{
		lcdClrLine(2, 7);
		sprintf((char *)szBuff, "(%c)",  ucTemp ? '1' : '0');
		PubDispString("CHECK LINE",      DISP_LINE_CENTER|2);
		PubDispString(szBuff,           DISP_LINE_CENTER|4);
		PubDispString("1.YES 2.NO", DISP_LINE_CENTER|6);

		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( iKey==KEY_CANCEL || iKey==KEY_INVALID )
		{
			return ERR_USERCANCEL;
		}
		else if( iKey== KEY_ENTER )
		{
			break;
		}
		else if ( iKey==KEY1 )
		{
			ucTemp = 1;
		}
		else if ( iKey==KEY2 )
		{
			ucTemp = 0;
		}
	}
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.extension_chk = ucTemp;
	gstSysParam.stDownParamCommCfg.stPSTNPara.stDialPara = gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara;
//	CommInitModule(&gstCurAcq.stTxnCommCfg); -- 先不初始化模块，设置通讯类型再设置

	for(iCnt=0; iCnt<gstSysParam.uiAcqNum; iCnt++)
	{
		gstSysParam.stAcqList[iCnt].stTxnCommCfg.stPSTNPara = gstCurAcq.stTxnCommCfg.stPSTNPara;
	}

	return 0;
}

// 设置TCP/IP参数
int SetTcpIpParam(int iMode)
{
	int		iRet;

	PubDisplayTitle(TRUE, "TCP/IP PARA");
	
	if (iMode == DOWNLAOD_COMMTYPE)
	{
		iRet = GetIPAddress((uint8_t *)"REMOTE IP", FALSE, gstCurAcq.stTxnCommCfg.stTcpIpPara.szRemoteIP);
		if( iRet!=0 )
		{
			return iRet;
		}
		
		iRet = GetIPPort((uint8_t *)"PORT", gstCurAcq.stTxnCommCfg.stTcpIpPara.szRemotePort);
		if( iRet<0 )
		{
			return iRet;
		}

		return 0;
	}

	iRet = GetIPAddress((uint8_t *)"PRIMARY IP", FALSE, (uint8_t *)gstCurAcq.szRemoteIP);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = GetIPPort((uint8_t *)"PRIMARY PORT", (uint8_t *)gstCurAcq.szRemotePort);
	if( iRet<0 )
	{
		return iRet;
	}

	iRet = GetIPAddress((uint8_t *)"SECOND IP", FALSE, (uint8_t *)gstCurAcq.szRemoteIP2);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = GetIPPort((uint8_t *)"SECOND PORT", (uint8_t *)gstCurAcq.szRemotePort2);
	if( iRet<0 )
	{
		return iRet;
	}

/*	iRet = GetIPAddress((uint8_t *)"LOCAL IP", TRUE, gstCurAcq.stTxnCommCfg.stTcpIpPara.szLocalIP);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetIPAddress((uint8_t *)"IP MASK", TRUE, gstCurAcq.stTxnCommCfg.stTcpIpPara.szNetMask);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = GetIPAddress((uint8_t *)"GATEWAY IP", TRUE, gstCurAcq.stTxnCommCfg.stTcpIpPara.szGatewayIP);
	if( iRet!=0 )
	{
		return iRet;
	}*/

	return 0;
}

// 输入IP地址
int GetIPAddress(uint8_t *pszPrompts, uint8_t bAllowNull, uint8_t *pszIPAddress)
{
	uint8_t	szTemp[20];

	sprintf((char *)szTemp, "%.15s", pszIPAddress);
	while( 1 )
	{
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "%.16s", pszPrompts);
		if( PubGetString(ALPHA_IN|ECHO_IN, 0, 15, szTemp, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}
		if( bAllowNull && szTemp[0]==0 )
		{
			*pszIPAddress = 0;
			break;
		}
		if( ValidIPAddress((char *)szTemp) )
		{
			sprintf((char *)pszIPAddress, "%.15s", szTemp);
			break;
		}

		Display2StringInRect("无效IP地址", "INV IP ADDR");
		PubBeepErr();
		PubWaitKey(4);
	}

	return 0;
}

// 检查IP地址
int ValidIPAddress(char *pszIPAddr)
{
	int		i;
	char	*p, *q, szBuf[5+1];

	PubTrimStr((char *)pszIPAddr);
	if( *pszIPAddr==0 )
	{
		return FALSE;
	}

	p = strchr(pszIPAddr, ' ');
	if( p!=NULL )
	{
		return FALSE;
	}
	if( strlen(pszIPAddr)>15 )
	{
		return FALSE;
	}

	// 1st --- 3rd  part
	for(q=pszIPAddr, i=0; i<3; i++)
	{
		p = strchr(q, '.');
		if( p==NULL || p==q || p-q>3 )
		{
			return FALSE;
		}
		sprintf(szBuf, "%.*s", (int)(p-q), q);
		if( !IsNumStr(szBuf) || atoi(szBuf)>255 )
		{
			return FALSE;
		}
		q = p + 1;
	}

	// final part
	p = strchr((char *)q, '.');
	if( p!=NULL || !IsNumStr(q) || strlen(q)==0 || strlen(q)>3 || atoi(q)>255 )
	{
		return FALSE;
	}

	return TRUE;
}

// 输入端口
int GetIPPort(uint8_t *pszPrompts, uint8_t *pszPortNo)
{
	int		iTemp;
	uint8_t	szTemp[15];

	sprintf((char *)szTemp, "%.5s", pszPortNo);
	while( 1 )
	{
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "%.16s", pszPrompts);
		if( PubGetString(NUM_IN|ECHO_IN, 1, 5, szTemp, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}
		iTemp = atoi((char *)szTemp);
		if( iTemp>0 && iTemp<65535 )
		{
			sprintf((char *)pszPortNo, "%.5s", szTemp);
			break;
		}

		Display2StringInRect("无效端口号", "INV PORT #");
		PubBeepErr();
		PubWaitKey(4);
	}

	return 0;
}

// 选择发卡行并修改参数
void SetIssuerParam(void)
{
	int		iRet;
	uint8_t	ucIndex;

	iRet = SelectIssuer(&ucIndex);
	if( iRet!=0 )
	{
		return;
	}

	PubDisplayTitle(TRUE, gstCurIssuer.szName);
	ModifyOptList((uchar*)gstSysParam.stIssuerList[ucIndex].sOption, 'I');
	SaveAppTransParam();
}

// 设置收单行参数
void SetAcqParam(void)
{
	int		iRet;
	uint8_t	ucKey, szTitle[16+1];

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		sprintf((char *)szTitle, "%.16s",  "SELECT ACQUIRER");
	else
		sprintf((char *)szTitle, "%.16s",  "选择收单行  NII");

	iRet = SelectAcq(FALSE, szTitle, NULL);
	if( iRet!=0 )
	{
		return;
	}

	while( 1 )
	{
		DispAcqPama();
		ucKey = PubWaitKey(USER_OPER_TIMEOUT);
		if( ucKey==KEY_CANCEL || ucKey==KEY_INVALID )
		{
			break;
		}

		lcdCls();
		PubDisplayTitle(TRUE, gstCurAcq.szName);
		if( ModifyOptList((uchar*)gstCurAcq.sOption, 'A')!=0 )
		{
			break;
		}

#ifndef FUN2_READ_ONLY
		PubDisplayTitle(TRUE, "SETUP ACQUIRER ");
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("ACQUIRER NAME", DISP_LINE_CENTER|4);
		else
			PubDispString("收单行名称",      DISP_LINE_CENTER|4);
		if( PubGetString(ECHO_IN|ALPHA_IN, 2, 20, (uint8_t *)gstCurAcq.szName, USER_OPER_TIMEOUT)!=0 )
		{
			break;
		}

		PubDisplayTitle(TRUE, gstSysParam.stAcqList[gstCurAcq.ucIndex].szName);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("TERMINAL ID", DISP_LINE_CENTER|4);
		else
			PubDispString("终端号",      DISP_LINE_CENTER|4);
		if( PubGetString(ECHO_IN, 8, 8, (uchar*)gstCurAcq.szTermID, USER_OPER_TIMEOUT)!=0 )
		{
			break;
		}

		lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("MERCHANT ID", DISP_LINE_CENTER|4);
		else
			PubDispString("商户号",      DISP_LINE_CENTER|4);
		if( PubGetString(ECHO_IN, 15, 15, (uchar*)gstCurAcq.szMerchantID, USER_OPER_TIMEOUT)!=0 )
		{
			break;
		}

		lcdClrLine(2, 7);
		PubDispString("NII", DISP_LINE_CENTER|4);
		if( PubGetString(ECHO_IN, 3, 3, (uchar*)gstCurAcq.szNii, USER_OPER_TIMEOUT)!=0 )
		{
			break;
		}

		SetAcqCommParam(ACQ_COMMTYPE);  // 设置通讯参数

#endif
		break;
	}

	gstSysParam.stAcqList[gstCurAcq.ucIndex] = gstCurAcq;
	SaveAppTransParam();
}

// 显示当前收单行参数信息
void DispAcqPama(void)
{
	WiFiStatus_t    stWiFiStatus;

	lcdCls();
	lcdDisplay(0, 0, DISP_CFONT, "%-10.10s   %3.3s", gstCurAcq.szName, gstCurAcq.szNii);
	lcdDisplay(0, 2, DISP_ASCII, "%s", gstCurAcq.szTermID);
	lcdDisplay(0, 3, DISP_ASCII, "%s", gstCurAcq.szMerchantID);

	switch( gstCurAcq.stTxnCommCfg.ucCommType )
	{
	case CT_RS232:
		lcdDisplay(0, 4, DISP_ASCII, "COMM TYPE: COM%d",gstCurAcq.stTxnCommCfg.stRS232Para.ucPortNo);
		lcdDisplay(0, 5, DISP_ASCII, "%s", gstCurAcq.stTxnCommCfg.stRS232Para.szAttr);		
		break;
		
	case CT_MODEM:
		lcdDisplay(0, 4, DISP_ASCII, "COMM TYPE: MODEM");
		lcdDisplay(0, 5, DISP_ASCII, "TXN1:%s", gstCurAcq.szTxnTelNo1);
		lcdDisplay(0, 6, DISP_ASCII, "TXN2:%s", gstCurAcq.szTxnTelNo2);
		break;
		
	case CT_TCPIP:
		lcdDisplay(0, 4, DISP_ASCII, "COMM TYPE: TCPIP");
		lcdDisplay(0, 5, DISP_ASCII, "%s:%s", gstCurAcq.szRemoteIP,gstCurAcq.szRemotePort);
		lcdDisplay(0, 6, DISP_ASCII, "%s:%s", gstCurAcq.szRemoteIP2,gstCurAcq.szRemotePort2);
		break;
		
	case CT_GPRS:
	case CT_CDMA:
		if (gstCurAcq.stTxnCommCfg.ucCommType==CT_CDMA)
			lcdDisplay(0, 4, DISP_ASCII, "COMM TYPE: CDMA");
		else
			lcdDisplay(0, 4, DISP_ASCII, "COMM TYPE: GPRS");

		lcdDisplay(0, 5, DISP_ASCII, "APN:%s", gstCurAcq.stTxnCommCfg.stWirlessPara.szAPN);
		lcdDisplay(0, 6, DISP_ASCII, "%s:%s", gstCurAcq.szRemoteIP,gstCurAcq.szRemotePort);
		lcdDisplay(0, 7, DISP_ASCII, "%s:%s", gstCurAcq.szRemoteIP2,gstCurAcq.szRemotePort2);
		break;
	
	case CT_WIFI:
		if (gstPosCapability.uiPosType == 0x02) // 6110 的WIFI和TCP一样处理
		{
			lcdDisplay(0, 4, DISP_ASCII, "COMM TYPE: WIFI");
			lcdDisplay(0, 5, DISP_ASCII, "%s:%s", gstCurAcq.szRemoteIP,gstCurAcq.szRemotePort);
			lcdDisplay(0, 6, DISP_ASCII, "%s:%s", gstCurAcq.szRemoteIP2,gstCurAcq.szRemotePort2);
		}
		else
		{
			lcdDisplay(0, 4, DISP_ASCII, "COMM TYPE: WIFI");
			WifiGetCurConnect(&stWiFiStatus);
			if (stWiFiStatus.Status != WIFI_STATUS_CONNECTED)
				lcdDisplay(0, 5, DISP_ASCII, "STATUS: DISCONNECT");
			else
				lcdDisplay(0, 5, DISP_ASCII, "STATUS: CONNECT");
			
			if (strlen((char *)gstCurAcq.stTxnCommCfg.stWIFIIpPara.szLocalIP)==0 || 
				strcmp((char *)gstCurAcq.stTxnCommCfg.stWIFIIpPara.szLocalIP,"0.0.0.0")==0 )
				lcdDisplay(0, 6, DISP_ASCII, "LOCAL IP: 0.0.0.0");
			else
				lcdDisplay(0, 6, DISP_ASCII, "LOCAL:%s",gstCurAcq.stTxnCommCfg.stWIFIIpPara.szLocalIP);
			
			lcdDisplay(0, 7, DISP_ASCII, "%s:%s", gstCurAcq.szRemoteIP,
				gstCurAcq.szRemotePort);
		}


		break;
		
	default:
		break;
	}
}

void   SetAcqCommType(void)
{
	INT32  iKey;
	UINT32 ucCommType,ucFlag, ucCommTypeBak;

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE,"Comm Type");
	else
		PubDisplayTitle(TRUE, "通讯类型");
	ucCommType = gstCurAcq.stTxnCommCfg.ucCommType;
	if ( !(ucCommType & CT_ALLCOMM) || ucCommType>CT_ALLCOMM )
	{
		ucCommType = gstPosCapability.uiCommType;
	}

	ucCommTypeBak = ucCommType;  // 先备份原来的通讯类型
	ucFlag = 1;
	while (ucFlag)
	{
		lcdClrLine(2,7);
		lcdDisplay(0, 2, DISP_ASCII, "%s%s", (ucCommType==CT_RS232 ? "=>" : "  "), "RS232");
		lcdDisplay(0, 3, DISP_ASCII, "%s%s", (ucCommType==CT_MODEM ? "=>" : "  "), "Modem");
		lcdDisplay(0, 4, DISP_ASCII, "%s%s", (ucCommType==CT_TCPIP ? "=>" : "  "), "TCPIP");
		lcdDisplay(0, 5, DISP_ASCII, "%s%s", (ucCommType==CT_CDMA  ? "=>" : "  "), "CDMA");
		lcdDisplay(0, 6, DISP_ASCII, "%s%s", (ucCommType==CT_GPRS  ? "=>" : "  "), "GPRS");
		lcdDisplay(0, 7, DISP_ASCII, "%s%s", (ucCommType==CT_WIFI  ? "=>" : "  "), "WIFI");	
		
		iKey = PubWaitKey(30);
		switch (iKey)
		{
		case KEY_CANCEL:			
			return;			
			break;
		case KEY_ENTER:	
			ucFlag = 0;  // 退出while
			break;
		case KEY1 :			       
			ucCommType = CT_RS232;
			break;
		case KEY2 :			       
			ucCommType = CT_MODEM;
			break;			       
		case KEY3 :			       
			ucCommType = CT_TCPIP;
			break;	
		case KEY4 :			       
			ucCommType = CT_CDMA;
			break;
		case KEY5 :			       
			ucCommType = CT_GPRS;
			break;
		case KEY6:
			ucCommType = CT_WIFI;
			break;
		case KEY_UP :
			ucCommType = ucCommType /  2 ;
			if(ucCommType==0)
			{
				ucCommType = CT_WIFI;
			}
			break;	
		case KEY_DOWN :
			ucCommType = ucCommType * 2 ;
			if (ucCommType > CT_WIFI)
			{
				ucCommType = CT_RS232;
			}
			break;
		default:
			break;
			
		}

		if (ucFlag==0)
		{
			if (!(gstPosCapability.uiCommType & ucCommType))
			{
				lcdClrLine(2,7);
				Display2StringInRect("不支持该通讯类型","NOT SUPPORT!");
				PubWaitKey(2);
				ucFlag = 1;
			}
		}
	}

	if ( (ucCommType >= CT_RS232) && (ucCommType <= CT_WIFI))
	{
		if (gstPosCapability.uiCommType & ucCommType)
		{
			gstCurAcq.stTxnCommCfg.ucCommType = ucCommType;
		}

		printf("setacq: %d %d \n",ucCommType, ucCommTypeBak);
		// 先挂断原来的通讯模块
		if (ucCommTypeBak != ucCommType) 
		{
			CommOnHook(TRUE);
			if (ucCommType == CT_GPRS || ucCommType == CT_CDMA)
			{
				ThreadInitModule();  // 新设置的通讯如果是无线的就启用模块
			}
		}
	}

	return;
}

// 设置当前终端通讯类型以及通讯参数
int  SetAcqCommParam(int iMode)
{
	int  iRet;
	ST_ACQUIRER_INFO  stTempAcq;
	char  szbuffer[4];

	if (iMode == DOWNLAOD_COMMTYPE)
	{
		FindAcq(1);  // 先找到ACQ
		stTempAcq = gstCurAcq;
		gstCurAcq.stTxnCommCfg = gstSysParam.stDownParamCommCfg;
		gstCurAcq.ulTimeOut = gstSysParam.stEdcInfo.ulTMSTimeOut;
	}

	memset(szbuffer,0,sizeof(szbuffer));
	sprintf((char *)szbuffer, "%lu", gstCurAcq.ulTimeOut);
	while( 1 )
	{
		lcdClrLine(2, 7);
		PubDispString("Comm Timeout", DISP_LINE_LEFT|2);
		PubDispString("[0..120]",       DISP_LINE_LEFT|4);
		if( PubGetString(NUM_IN|ECHO_IN, 1, 3, (uchar*)szbuffer, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}
		if( atoi((char *)szbuffer)<=120 )
		{
			break;
		}
		
		Display2StringInRect("输入值无效", "INVALID INPUT");
		PubBeepErr();
		PubWaitKey(3);
	}
	gstCurAcq.ulTimeOut = (ulong)atoi((char *)szbuffer);	

	SetAcqCommType();

	switch( gstCurAcq.stTxnCommCfg.ucCommType )
	{
	case CT_RS232:
		iRet = SetRS232Param();
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}
		break;

	case CT_MODEM:
		iRet = SetAcqTransTelNo(iMode);
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}
		CommInitModule(&gstCurAcq.stTxnCommCfg);
		break;
		
	case CT_TCPIP:
		iRet = SetTcpIpParam(iMode);
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}
		break;
		
	case CT_GPRS:
	case CT_CDMA:
		iRet = SetWirelessParam(iMode);
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}
//		CommOnHook(TRUE);
//		CommInitModule(&gstCurAcq.stTxnCommCfg); // 调整交易类型的时候已经初始化，在此不需要
		break;
		
	case CT_WIFI:
		iRet = SetWIFIParam(iMode);
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}
		CommOnHook(TRUE);
//		CommInitModule(&gstCurAcq.stTxnCommCfg);
		break;

	default:
		break;
	}

	if (iMode == DOWNLAOD_COMMTYPE)
	{
		gstSysParam.stEdcInfo.ulTMSTimeOut = gstCurAcq.ulTimeOut;
		gstSysParam.stDownParamCommCfg = gstCurAcq.stTxnCommCfg;
		gstCurAcq = stTempAcq;
		// 如果再次之前调用了SaveAppTransParam，会清除acq key =1的ACQ。则必须调用下面的恢复处理
		//memcpy(&gstSysParam.stAcqList[gstCurAcq.ucIndex], &gstCurAcq, sizeof(ST_ACQUIRER_INFO));
		SaveAppTransParam();
	}
	else
	{
		memcpy(&gstSysParam.stAcqList[gstCurAcq.ucIndex], &gstCurAcq, sizeof(ST_ACQUIRER_INFO));	
		SaveAppTransParam();
	}

	return 0;
}

int    SetRS232Param(void)
{
	int iKey ,ucFlag ;
	uint8_t ucCommType;

	ucCommType = gstCurAcq.stTxnCommCfg.stRS232Para.ucSendMode ;
	ucFlag = 1;
	while(ucFlag)
	{
		lcdCls();
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDisplayTitle(TRUE,"COM TYPE:");
			if (ucCommType==CM_RAW)
			{
				lcdDisplay(0, 2, DISP_CFONT, "%.18s",  "=>Org. Msg");
				lcdDisplay(0, 4, DISP_CFONT, "%.18s",  "  Pack Msg");
			}
			else
			{
				lcdDisplay(0, 2, DISP_CFONT, "%.18s",  "  Org. Msg");
				lcdDisplay(0, 4, DISP_CFONT, "%.18s",  "=>Pack Msg");
			}
		}
		else
		{
			PubDisplayTitle(TRUE,"串口通讯方式:");
			if (ucCommType==CM_RAW)
			{
				lcdDisplay(0, 2, DISP_CFONT, "%.18s",  "=>原报文发送");
				lcdDisplay(0, 4, DISP_CFONT, "%.18s",  "  打包发送");
			}
			else
			{
				lcdDisplay(0, 2, DISP_CFONT, "%.18s",  "  原报文发送");
				lcdDisplay(0, 4, DISP_CFONT, "%.18s",  "=>打包发送");
			}
		}	
		iKey = PubWaitKey(30);
		switch (iKey)
		{
		case KEY_CANCEL:			
		case KEY_ENTER:	
			ucFlag = 0;  // 退出while
			break;
		case KEY1 :			       
			ucCommType = CM_RAW;
			ucFlag = 0;
			break;
		case KEY2 :			       
			ucCommType = CM_SYNC;
			ucFlag = 0;
			break;			       			       
		case KEY_UP :
			if(ucCommType != CM_RAW)
			{
				ucCommType = CM_RAW;
			}
			else
			{
				ucCommType = CM_SYNC;
			}
			break;	
		case KEY_DOWN :
			if(ucCommType != CM_RAW)
			{
				ucCommType = CM_RAW;
			}
			else
			{
				ucCommType = CM_SYNC;
			}
			break;
		default:
			break;
		}
	}
	gstCurAcq.stTxnCommCfg.stRS232Para.ucSendMode = ucCommType ;

	lcdCls();
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE,"COM param");
	else
		PubDisplayTitle(TRUE,"串口通讯参数");
	if( PubGetString(ALPHA_IN|ECHO_IN, 0, 24, gstCurAcq.stTxnCommCfg.stRS232Para.szAttr, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}
	
	gstCurAcq.stTxnCommCfg.stRS232Para.ucPortNo = PORT_COM1;

	return 0;		
}

// 设置当前收单行交易电话号码
int SetAcqTransTelNo(int iMode)
{

	if (iMode == DOWNLAOD_COMMTYPE)
	{
		if( SetTel((uchar*)gstSysParam.stDownParamCommCfg.stPSTNPara.szTxnTelNo, (uint8_t *)"REMOTE DOWN TEL")!=0 )
		{
			return ERR_USERCANCEL;
		}

		return 0;
	}

	if( SetTel((uchar*)gstCurAcq.szTxnTelNo1, (uint8_t *)"TXN TEL:")!=0 )
	{
		return ERR_USERCANCEL;
	}

	if( SetTel((uint8_t *)gstCurAcq.szTxnTelNo2, (uint8_t *)"TXN TEL2:")!=0 )
	{
		return ERR_USERCANCEL;
	}

	return 0;
}

// 设置电话号码
int SetTel(uint8_t *pszTelNo, uint8_t *pszPromptInfo)
{
	lcdClrLine(2, 7);
	PubDispString(pszPromptInfo, DISP_LINE_LEFT|4);
	if( PubGetString(ALPHA_IN|ECHO_IN, 0, 24, pszTelNo, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}

	return 0;
}

int SetWirelessParam(int iMode)
{
	int   iRet;
	ST_WIRELESS_INFO	*pstParam;

	PubDisplayTitle(TRUE, "WIRELESS PARA");
	pstParam = &gstCurAcq.stTxnCommCfg.stWirlessPara;

	if (iMode == DOWNLAOD_COMMTYPE)
	{
		lcdClrLine(2, 7);
		if( GetIPAddress((uint8_t *)"REMOTE IP", FALSE, pstParam->szRemoteIP)!=0 )
		{
			return ERR_USERCANCEL;
		}
		
		lcdClrLine(2, 7);
		if( GetIPPort((uint8_t *)"PORT", pstParam->szRemotePort)!=0 )
		{
			return ERR_USERCANCEL;
		}
		
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "APN");
		if( PubGetString(ALPHA_IN|ECHO_IN, 0, 32, pstParam->szAPN, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}
		
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "LOGIN NAME");
		if( PubGetString(ALPHA_IN|ECHO_IN, 0, 32, pstParam->szUID, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}
		
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "LOGIN PWD");
		if( PubGetString(ALPHA_IN|ECHO_IN, 0, 16, pstParam->szPwd, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}
		
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "SIM PIN");
		if( PubGetString(ALPHA_IN|ECHO_IN, 0, 16, pstParam->szSimPin, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}

		return 0;
	}

	lcdClrLine(2, 7);
	iRet = GetIPAddress((uint8_t *)"PRIMARY IP", FALSE, (uint8_t *)gstCurAcq.szRemoteIP);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	lcdClrLine(2, 7);
	iRet = GetIPPort((uint8_t *)"PRIMARY PORT", (uint8_t *)gstCurAcq.szRemotePort);
	if( iRet<0 )
	{
		return iRet;
	}
	
	lcdClrLine(2, 7);
	iRet = GetIPAddress((uint8_t *)"SECOND IP", FALSE, (uint8_t *)gstCurAcq.szRemoteIP2);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	lcdClrLine(2, 7);
	iRet = GetIPPort((uint8_t *)"SECOND PORT", (uint8_t *)gstCurAcq.szRemotePort2);
	if( iRet<0 )
	{
		return iRet;
	}

	lcdClrLine(2, 7);
	lcdDisplay(0, 2, DISP_CFONT, "APN");
	if( PubGetString(ALPHA_IN|ECHO_IN, 0, 32, pstParam->szAPN, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}

	lcdClrLine(2, 7);
	lcdDisplay(0, 2, DISP_CFONT, "LOGIN NAME");
	if( PubGetString(ALPHA_IN|ECHO_IN, 0, 32, pstParam->szUID, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}

	lcdClrLine(2, 7);
	lcdDisplay(0, 2, DISP_CFONT, "LOGIN PWD");
	if( PubGetString(ALPHA_IN|ECHO_IN, 0, 16, pstParam->szPwd, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}

	lcdClrLine(2, 7);
	lcdDisplay(0, 2, DISP_CFONT, "SIM PIN");
	if( PubGetString(ALPHA_IN|ECHO_IN, 0, 16, pstParam->szSimPin, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}
/*
	if( GetIPAddress((uint8_t *)"REMOTE IP", FALSE, pstParam->szRemoteIP)!=0 )
	{
		return ERR_USERCANCEL;
	}
	
	if( GetIPPort((uint8_t *)"PORT", pstParam->szRemotePort)!=0 )
	{
		return ERR_USERCANCEL;
	}
*/
	return 0;
}

// 设置EDC参数
void SetEdcParam(void)
{
	PubDisplayTitle(TRUE, "EDC PARAM");
	while( 1 )
	{
		SetMerchantInfo();

		if( SetAcceptTimeOut()!=0 )
		{
			break;
		}
		if( SetNumOfReceipt()!=0 )
		{
			break;
		}
		if( SetGetSysTraceNo()!=0 )
		{
			break;
		}

		// version 06后,不需要设置自动结算时间
//		SetAutoSettlmentTime();
// 		if (SetAutoSettlmentTime()!=0)
// 		{
// 			break;
// 		}
//		SetLanguage();
//		SetMsgHeadLenType();
		SetPreDial(); 
		ModifyOptList((uchar*)gstSysParam.stEdcInfo.sOption, 'E');

		if (gstPosCapability.uiCommType & CT_MODEM)
		{
			SetExtLineNo();
			SetModemParam();
		}

		break;
	}

	SaveEdcParam();
}

int SetGetSysTraceNo(void)
{
	uint8_t	szBuff[6+1];

	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDispString("TRACE NO", DISP_LINE_LEFT|4);
	else
		PubDispString("流水号",   DISP_LINE_LEFT|4);
	sprintf((char *)szBuff, "%06ld", gstSysCtrlParam.ulSTAN);

#ifndef	FUN2_READ_ONLY
	if( PubGetString(NUM_IN|ECHO_IN, 1, 6, szBuff, USER_OPER_TIMEOUT)!=0 )
	{
		return ERR_USERCANCEL;
	}
	gstSysCtrlParam.ulSTAN = (ulong)atol((char *)szBuff);
	SaveSysCtrlBase();
#else
	PubDispString(szBuff, DISP_LINE_RIGHT|6);
	PubYesNo(USER_OPER_TIMEOUT);
#endif

	return 0;
}

// 输入交易成功时确认信息显示时间
int SetAcceptTimeOut(void)
{
	uint8_t	szBuff[2+1];
	
	memset(szBuff,0,sizeof(szBuff));
	sprintf((char *)szBuff, "%lu", gstSysParam.stEdcInfo.ulAcceptTimeout);
	while( 1 )
	{
		lcdClrLine(2, 7);
		PubDispString("Confirm Timeout", DISP_LINE_LEFT|2);
		PubDispString("[0..60]",       DISP_LINE_LEFT|4);
		if( PubGetString(NUM_IN|ECHO_IN, 1, 2, szBuff, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}
		if( atoi((char *)szBuff)<=60 )
		{
			break;
		}
		
		Display2StringInRect("输入值无效", "INVALID INPUT");
		PubBeepErr();
		PubWaitKey(3);
	}
	gstSysParam.stEdcInfo.ulAcceptTimeout = (ulong)atoi((char *)szBuff);
	
	return 0;
}

int    SetPreDial(void)
{
	int  iKey; 

	while( 1 )
	{
		lcdClrLine(2, 7);
		lcdDisplay(0,2,DISP_CFONT|DISP_MEDIACY,"ENABLE PREDIAL");
		if (gstSysParam.stEdcInfo.bPreDial==1)
			lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"(1)");
		else
			lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"(0)");
		lcdDisplay(0,6,DISP_CFONT|DISP_MEDIACY,"1.ON 0.OFF");
		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		switch (iKey)
		{
		case KEY1:
			gstSysParam.stEdcInfo.bPreDial = 1;
			break;
		case KEY0:
			gstSysParam.stEdcInfo.bPreDial = 0;
			break;
		case KEY_ENTER:
		case KEY_CANCEL:
			return 0;
			break;
		default:
			PubBeepErr();
			break;
		}
	}
	
	return 0;
}

// 输入热敏打印单据张数
int SetNumOfReceipt(void)
{
	char 	szBuff[1+1];
	UINT32  ulNums;

	ulNums = gstSysParam.stEdcInfo.uiPrintNums;
	memset(szBuff,0,sizeof(szBuff));
	szBuff[0] = gstSysParam.stEdcInfo.uiPrintNums + 0x30;
	while( 1 )
	{
		lcdClrLine(2, 7);
		PubDispString("Receipt pages", DISP_LINE_LEFT|2);
		PubDispString("[1..3]Thermal",   DISP_LINE_LEFT|4);
		if( PubGetString(NUM_IN|ECHO_IN, 1, 1, (uchar*)szBuff, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}

		ulNums = szBuff[0] - 0x30;
		if( ulNums>=1 && ulNums<=3 )
		{
			break;
		}

		Display2StringInRect("输入值无效", "INVALID INPUT");
		PubBeepErr();
		PubWaitKey(3);
	}

	gstSysParam.stEdcInfo.uiPrintNums = ulNums;

	return 0;
}


// 修改或者查看开关选项
int ModifyOptList(uint8_t *psOption, uint8_t ucMode)
{
	static OPTION_INFO stIssuerOptList[] =
	{
// 		{"CAPTURE CASH?",		ALLOW_EXTEND_PAY},
//		{"CAPTURE TXN?",		ISSUER_CAPTURE_TXN},
//		{"ENABLE BALANCE?",		ISSUER_EN_BALANCE},
		{"ENABLE ADJUST?",		ISSUER_EN_ADJUST},
		{"ENABLE OFFLINE?",		ISSUER_EN_OFFLINE},
		{"ENABLE PREAUTH?",		ISSUER_EN_PREAUTH},
		{"ENABLE MANUL?",		ISSUER_EN_MANUL},
		{"ENABLE SWIPE?",		ISSUER_EN_SWIPE},
//		{"ENABLE REFUND?",		ISSUER_EN_REFUND},
//		{"ENABLE VOID?",		ISSUER_EN_VOID},
		{"ENABLE EXPIRY?",		ISSUER_EN_EXPIRY},
		{"CHECK EXPIRY?",		ISSUER_CHECK_EXPIRY},
		{"DISP EXPIRY?",		ISSUER_DISP_EXPIRY},
		{"CHKEXP OFFLINE?",		ISSUER_CHECK_EXPIRY_OFFLINE},
		{"CHECK PAN?",			ISSUER_CHKPAN_MOD10},
// 		{"CHECK PAN11",			ISSUER_CHKPAN_MOD11},
//		{"EN DISCRIPTOR?",		ISSUER_EN_DISCRIPTOR},
//		{"ENABLE PRINT?",		ISSUER_EN_PRINT},
//		{"VOICE REFERRAL?",		ISSUER_EN_VOICE_REFERRAL},
//		{"PIN REQUIRED?",		ISSUER_EN_PIN},
//		{"ACCOUNT SELECT?",		ISSUER_EN_ACCOUNT_SELECTION},
//		{"ROC INPUT REQ?",		ISSUER_ROC_INPUT_REQ},
//		{"DISP AUTH CODE?",		ISSUER_AUTH_CODE},
//		{"ADDTIONAL DATA?",		ISSUER_ADDTIONAL_DATA},
		{"SECURITY CODE?",		ISSUER_SECURITY_SWIPE},
//		{"SECU. CODE MANUL?",	ISSUER_SECURITY_MANUL},
		{NULL, 0},
	};

	static OPTION_INFO stAcqOptList[] =
	{
//		{"ONLINE VOID?",		ACQ_ONLINE_VOID},
//		{"ONLINE REFUND?",		ACQ_ONLINE_REFUND},
//		{"DIS. TRICK FEED",		ACQ_DISABLE_TRICK_FEED},
//		{"ADDTION PROMPT",		ACQ_ADDTIONAL_PROMPT},
//		{"AIR TICKET",			ACQ_AIR_TICKET},
//		{"BOC INSTALMENT",		ACQ_BOC_INSTALMENT_FEATURE},
//		{"ENABLE EMV",			ACQ_EMV_FEATURE},
//		{"AMEX FEATURE?",		ACQ_AMEX_SPECIFIC_FEATURE},
		{NULL, 0},
	};

	static OPTION_INFO stEdcOptList[] =
	{
//		{"AUTH PAN MASKING",	EDC_AUTH_PAN_MASKING},
//		{"SELECT ACQ_CARD",		EDC_SELECT_ACQ_FOR_CARD},
//		{"OFFLINE TXN?",		EDC_OFFLINE_TXN},
//		{"EN. INSTALMENT?",		EDC_ENABLE_INSTALMENT},
//		{"CAPTURE CASH?",		EDC_CASH_PROCESS},
//		{"REFERRAL DIAL",		EDC_REFERRAL_DIAL},
//		{"AUTH OR PREAUTH",		EDC_AUTH_PREAUTH},
//		{"PRINT TIME?",			EDC_PRINT_TIME},
		{"TIP PROCESSING?",		EDC_TIP_PROCESS},
//		{"USE PRINTER?",		EDC_USE_PRINTER},
//		{"NOT ADJUST PWD?",		EDC_ADJUST_PWD},
//		{"NOT SETTLE_PWD?",		EDC_SETTLE_PWD},
//		{"NOT REFUND PWD?",		EDC_REFUND_PWD},
//		{"NOT VOID PWD?",		EDC_VOID_PWD},
//		{"NOT MANUAL PWD?",		EDC_MANUAL_PWD},
//		{"NOT LOCKED EDC?",		EDC_KEYBOARD_LOCKED},
		{"AUTO SETTLEMENT?",	EDC_AUTO_SETTLEMENT},
		{"ENABLE ELECSIGN?",    EDC_ENABLE_ELECSIGN},
		{NULL, 0},
	};
	OPTION_INFO		*pstCurOpt;
	uint8_t			ucCnt, szBuff[4], ucOptIndex, ucOptBitNo;

	if( ucMode=='I' )
	{
		pstCurOpt = (OPTION_INFO *)stIssuerOptList;
	}
	else if( ucMode=='E' )
	{
		pstCurOpt = (OPTION_INFO *)stEdcOptList;
	}
	else
	{
		pstCurOpt = (OPTION_INFO *)stAcqOptList;
	}
	if( pstCurOpt->pText==NULL )
	{
		return 0;
	}

	ucCnt = 0;
	while( 1 )
	{
		ucOptIndex = (uint8_t)(pstCurOpt[ucCnt].uiOptVal>>8);
		ucOptBitNo = (uint8_t)(pstCurOpt[ucCnt].uiOptVal & 0xFF);

		lcdClrLine(2, 7);
		PubDispString(pstCurOpt[ucCnt].pText, DISP_LINE_CENTER|2);
		sprintf((char *)szBuff, "(%c)", (psOption[ucOptIndex] & ucOptBitNo) ? '1' : '0');
		PubDispString(szBuff,       DISP_LINE_CENTER|4);
		PubDispString("1.ON 0.OFF", DISP_LINE_CENTER|6);
		switch( PubWaitKey(USER_OPER_TIMEOUT) )
		{
#ifndef FUN2_READ_ONLY
		case KEY1:
			psOption[ucOptIndex] |= ucOptBitNo;
			break;

		case KEY0:
			psOption[ucOptIndex] &= ~ucOptBitNo;
			break;
#else
		case KEY1:
			if( (ucMode=='E') && (pstCurOpt[ucCnt].uiOptVal==EDC_AUTH_PAN_MASKING) )
			{
				psOption[ucOptIndex] |= ucOptBitNo;
			}
			if( (ucMode=='E') && (pstCurOpt[ucCnt].uiOptVal==EDC_SELECT_ACQ_FOR_CARD) )
			{
				psOption[ucOptIndex] |= ucOptBitNo;
			}
			break;

		case KEY0:
			if( (ucMode=='E') && (pstCurOpt[ucCnt].uiOptVal==EDC_AUTH_PAN_MASKING) )
			{
				psOption[ucOptIndex] &= ~ucOptBitNo;
			}
			if( (ucMode=='E') && (pstCurOpt[ucCnt].uiOptVal==EDC_SELECT_ACQ_FOR_CARD) )
			{
				psOption[ucOptIndex] &= ~ucOptBitNo;
			}
			break;
#endif

		case KEY_UP:
			if( ucCnt>0 )
			{
				ucCnt--;
			}
			break;

		case KEY_DOWN:
		case KEY_ENTER:
			if( pstCurOpt[ucCnt+1].pText==NULL )
			{
				return 0;
			}
			ucCnt++;
			break;

		case KEY_INVALID:
		case KEY_CANCEL:
			return ERR_USERCANCEL;
		}
	}
}


// 修改口令
void ChangePassword(void)
{
	static MenuItem stChgPwdMenu[] =
	{
		{TRUE, "SYSTEM   PWD",   ModifyBankPWD},
		{TRUE, "TERMINAL PWD",   ModifyTermPWD},
/*		{TRUE, "VOID     PWD",   ModifyTransVoidPWD},
		{TRUE, "REFUND   PWD",   ModifyTransRefundPWD},
		{TRUE, "ADJUST   PWD",   ModifyTransAdjustPWD},
		{TRUE, "SETTLEMENT PWD", ModifyTransSettlePWD},*/
		{TRUE, "", NULL},
	};

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubGetMenu((uint8_t *)"CHANGE PWD",       stChgPwdMenu, MENU_AUTOSNO|MENU_CFONT, USER_OPER_TIMEOUT);
	else
		PubGetMenu((uint8_t *)"改密码CHANGE PWD", stChgPwdMenu, MENU_AUTOSNO|MENU_CFONT, USER_OPER_TIMEOUT);
}

// 手工设置系统时间
void SetSysTime()
{
	char	szBuff[14+1], sInputTime[6];

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, (char *)"SET TIME");
	else
		PubDisplayTitle(TRUE, (char *)"重设时间 SETTIME");
	while( 1 )
	{
		lcdClrLine(2, 7);
		PubDispString("YYYYMMDDHHMMSS", DISP_LINE_RIGHT|2);
		GetDateTime((uint8_t *)szBuff);
		PubDispString(szBuff, DISP_LINE_RIGHT|4);

		if( PubGetString(NUM_IN|ECHO_IN, 14, 14, (uchar*)szBuff, USER_OPER_TIMEOUT)!=0 )
		{
			return;
		}
		PubAsc2Bcd(&szBuff[2], 12, sInputTime);
		if( sysSetTime((uchar*)sInputTime)==0 )
		{
			return;
		}
		Display2StringInRect("输入时间非法", "INVALID TIME");
		PubBeepErr();
		PubWaitKey(3);
	}

	return;
}


void   SetLanguage(void)
{
#ifndef _POS_TYPE_8210
	int    iKey, iFlag;
	char   chLanguage; 
	
	PubDisplayTitle(TRUE,"SET LANGUAGE");
	chLanguage = gstSysParam.stEdcInfo.ucLanguage;
	iFlag = 1;
	while (iFlag)
	{
		lcdClrLine(2,7);
		switch(chLanguage)
		{
		case EDC_LANGUGE_SM:
			font_set_trans_func(NULL,NULL);
			lcdDisplay(0, 2, DISP_CFONT, "   英文");
			lcdDisplay(0, 4, DISP_CFONT, "=> 简体中文");
			lcdDisplay(0, 6, DISP_CFONT, "   繁体中文");
			break;
		case EDC_LANGUGE_CH:
			font_set_trans_func(NULL,font_simplified2tradition);
			lcdDisplay(0, 2, DISP_CFONT, "   英文");
			lcdDisplay(0, 4, DISP_CFONT, "   简体中文");
			lcdDisplay(0, 6, DISP_CFONT, "=> 繁体中文");
			break;
		case EDC_LANGUGE_EN:
		default:
			lcdDisplay(0, 2, DISP_CFONT, "=> ENGLISH");
			lcdDisplay(0, 4, DISP_CFONT, "   CHINESE(SM)");
			lcdDisplay(0, 6, DISP_CFONT, "   CHINESE(GB)");
			break;
		}
		
		iKey = PubWaitKey(30);
		switch (iKey)
		{
		case KEY_CANCEL:
			gstSysParam.stEdcInfo.ucLanguage = chLanguage;
			if (chLanguage==0)
			{
				gstSysParam.stEdcInfo.ucLanguage = EDC_LANGUGE_EN;
				SaveAppTransParam();
			}
			return;
			break;
		case KEY_ENTER:	
			gstSysParam.stEdcInfo.ucLanguage = chLanguage;
			if (chLanguage==0)
			{
				gstSysParam.stEdcInfo.ucLanguage = EDC_LANGUGE_EN;
			}
			SaveAppTransParam();
			return;
			break;
		case KEY1 :			       
			chLanguage = EDC_LANGUGE_EN;
			break;
		case KEY2 :			       
			chLanguage = EDC_LANGUGE_SM;
			break;			       
		case KEY3 :	
			chLanguage = EDC_LANGUGE_CH;
			break;				       
		case KEY_UP :
			chLanguage--;
			if (chLanguage < EDC_LANGUGE_EN)
			{
				chLanguage = EDC_LANGUGE_CH;
			}
			break;	
		case KEY_DOWN :
			chLanguage++;
			if (chLanguage > EDC_LANGUGE_CH)
			{
				chLanguage = EDC_LANGUGE_EN;
			}
			break;
		default:
			break;
		}
	}
#endif	
	return ;
}

int    SelectAndSetWIFIAP(void)
{
#ifndef _POS_TYPE_8210
	int   iRet, iKeyIndex, iApNum,iMenuNo;
	char  szBuffer[200];
    WiFiConfig_t    stWIFIConfig;
    WiFiAPInfo_t    stApInfoList[64];
    WiFiAPx_t       stApxInfo;   // 
    WiFiDefAP_t     stDefApInfo;
	MenuItem        stSelectMenu[64+1];

	memset(szBuffer,0,sizeof(szBuffer));
	memset((char*)&stWIFIConfig,0,sizeof(stWIFIConfig));
	memset((char*)&stApxInfo,0,sizeof(stApxInfo));
	memset((char*)&stDefApInfo,0,sizeof(stDefApInfo));

	PubDisplayTitle(TRUE,"SETUP AP");
    lcdDisplay(0, 2, DISP_CFONT, "1.SCAN AP");
    lcdDisplay(0, 4, DISP_CFONT, "2.MANUAL SETUP");
    iRet = PubWaitKey(USER_OPER_TIMEOUT);
	if (iRet != KEY1 && iRet != KEY2)
	{
		return ERR_NO_DISP;
	}
    if(iRet == KEY2)
    {
        lcdClrLine(2, 7);
        lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, "INPUT SSID:");
        memset(szBuffer,0,sizeof(szBuffer));
		strcpy(szBuffer,gstSysParam.stWIFIAPInfo.szSSID);
		iRet = PubGetString(ALPHA_IN|ECHO_IN,1,32,(uchar*)szBuffer,USER_OPER_TIMEOUT);
        if(iRet != 0)
        {
			return ERR_NO_DISP;
        }
        else
        {
            strcpy(stApxInfo.SSID,szBuffer);
        }
    }
    else
    {
        lcdClrLine(2, 7);
		Display2StringInRect("PLS WAIT...", "SCAN...");
        iApNum = WifiScanAP(64, stApInfoList);
        if(iApNum < 0)
        {
            lcdClrLine(2, 7);
			PubDispString("CAN'T SACN AP",3|DISP_LINE_CENTER);
			PubDispString("PLS TRY IT AGAIN",5|DISP_LINE_CENTER);
            lcdDisplay(96, 7, DISP_ASCII, "%d", iApNum);
			PubWaitKey(3);
			return ERR_NO_DISP;
        }

        if(iApNum == 0)
        {
            lcdClrLine(2, 7);
			PubDispString("CAN'T SACN AP",3|DISP_LINE_CENTER);
			PubDispString("PLS TRY IT AGAIN",5|DISP_LINE_CENTER);
            lcdDisplay(96, 7, DISP_ASCII, "%d", iApNum);
			PubWaitKey(3);
			return ERR_NO_DISP;
        }

        for(iMenuNo=0; iMenuNo< iApNum; iMenuNo++)
        {
			stSelectMenu[iMenuNo].bVisible = TRUE;
			stSelectMenu[iMenuNo].pfMenuFunc = NULL;
			strcpy(stSelectMenu[iMenuNo].szMenuName,stApInfoList[iMenuNo].SSID);
        }
		stSelectMenu[iMenuNo].bVisible = FALSE;
		stSelectMenu[iMenuNo].pfMenuFunc = NULL;
		memset(stSelectMenu[iMenuNo].szMenuName,0,sizeof(stSelectMenu[iMenuNo].szMenuName));

        iMenuNo = PubGetMenu((uchar*)"SELECT AP", stSelectMenu, MENU_AUTOSNO, USER_OPER_TIMEOUT);
        if(iMenuNo < 0 || iMenuNo>= iApNum)
        {
            return ERR_NO_DISP;
        }
        strcpy(stApxInfo.SSID, stApInfoList[iMenuNo].SSID);
    }

	PubDisplayTitle(TRUE,"SETUP AP");
	while(1)
	{
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "CHANNEL(0-13):");
		memset(szBuffer,0,sizeof(szBuffer));
		sprintf(szBuffer,"%d",gstSysParam.stWIFIAPInfo.iChannel);
		iRet = PubGetString(NUM_IN|ECHO_IN,1,2,(uchar*)szBuffer,USER_OPER_TIMEOUT);
		if(iRet != 0)
		{
			return ERR_NO_DISP;
		}

		iRet = atoi(szBuffer);
		if (iRet>=13)
		{
            lcdClrLine(2, 7);
			PubDispString("INVALID VALUE",3|DISP_LINE_CENTER);
			PubDispString("RANGE:0--13",5|DISP_LINE_CENTER);
			PubWaitKey(2);
			continue;
		}

		stWIFIConfig.Channel = iRet;
		break;
	}

    PubDisplayTitle(TRUE, "SET SECURITY:");
    lcdDisplay(0, 2, DISP_ASCII, "1.NONE *");
    lcdDisplay(0, 3, DISP_ASCII, "2.WEP64 ");
    lcdDisplay(0, 4, DISP_ASCII, "3.WEP128");
    lcdDisplay(0, 5, DISP_ASCII, "4.WPA-TKIP");
    lcdDisplay(0, 6, DISP_ASCII, "5.WPA2-AES");
    iRet = PubWaitKey(USER_OPER_TIMEOUT);
    switch(iRet)
    {
    case KEY_CANCEL:
        return(KB_CANCEL);
    case KEY1:
        stApxInfo.SecurityType = WIFI_SECURITY_NONE;
        break;
    case KEY2:
        stApxInfo.SecurityType = WIFI_SECURITY_WEP64;
        break;
    case KEY3:
        stApxInfo.SecurityType = WIFI_SECURITY_WEP128;
        break;
    case KEY4:
        stApxInfo.SecurityType = WIFI_SECURITY_WPATKIP;
        break;
    case KEY5:
        stApxInfo.SecurityType = WIFI_SECURITY_WPA2AES;
        break;
    default:
        break;
    }

	PubDisplayTitle(TRUE,"SET PASSWORD");
	switch (stApxInfo.SecurityType)
	{
    case WIFI_SECURITY_WEP64:
    case WIFI_SECURITY_WEP128:
        strncpy(stDefApInfo.SSID, stApxInfo.SSID, sizeof(stApxInfo.SSID));
        stDefApInfo.WEPMode = stApxInfo.SecurityType;

		while(1)
		{
			lcdClrLine(2, 7);
			PubDispString("Set WEPKEY INDEX",2|DISP_LINE_CENTER);
			PubDispString("RANGE:[1..4]",4|DISP_LINE_CENTER);
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer,"%d",gstSysParam.stWIFIAPInfo.iWEPKeyIdx);
			iRet = PubGetString(NUM_IN|ECHO_IN,1,1,(uchar*)szBuffer,USER_OPER_TIMEOUT);
			if(iRet != 0)
			{
				return ERR_NO_DISP;
			}

			iRet = atoi(szBuffer);
			if (iRet>5)
			{
				lcdClrLine(2, 7);
				Display2Strings("INVALID WEPKEY", "RANGE:1--4");
				PubWaitKey(2);
				continue;
			}

			stDefApInfo.WEPKeyIdx = iRet;
			break;
		}
		
        lcdClrLine(2, 7);
        lcdDisplay(0, 2, DISP_CFONT, "WEP KEY[%d]:", stDefApInfo.WEPKeyIdx);
        memset(szBuffer, 0, sizeof(szBuffer));
		strcpy(szBuffer,gstSysParam.stWIFIAPInfo.szListWEPKey[stDefApInfo.WEPKeyIdx]);
		iRet = PubGetString(ALPHA_IN|ECHO_IN,1,13,(uchar*)szBuffer,USER_OPER_TIMEOUT);
        if(iRet != 0)
        {
			return ERR_NO_DISP;
        }
		strcpy(stApxInfo.WEPKey, szBuffer);
		strcpy(stDefApInfo.WEPKey[stDefApInfo.WEPKeyIdx-1], szBuffer);
		break;
    case WIFI_SECURITY_WPATKIP:
    case WIFI_SECURITY_WPA2AES:
        lcdClrLine(2, 7);
        lcdDisplay(0, 2, DISP_CFONT, "WPA PSK:");
        memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer,"%s",gstSysParam.stWIFIAPInfo.szWPAPSK);
        iRet = PubGetString(ALPHA_IN|ECHO_IN,1,32,(uchar*)szBuffer,USER_OPER_TIMEOUT);
        if(iRet != 0)
        {
			return ERR_NO_DISP;
        }
		strcpy(stApxInfo.WPAPSK, szBuffer);
        break;

	case WIFI_SECURITY_NONE:
	default:
		break;
	}

    stWIFIConfig.IchipPowerSave = 5;
    stWIFIConfig.WLANPowerSave = 5;
    stWIFIConfig.RoamingMode = 0;
    stWIFIConfig.PeriodicScanInt = 5;
    stWIFIConfig.RoamingLowSNR = 10;
    stWIFIConfig.RoamingHighSNR = 30;

	PubDisplayTitle(TRUE,"SETUP AP");
	Display2StringInRect("PLS WAIT...","ACTIVE SETTING..");
	if(stApxInfo.SecurityType == WIFI_SECURITY_WEP64 || stApxInfo.SecurityType == WIFI_SECURITY_WEP128)
    {
		iRet = WifiSetDefAP(&stDefApInfo);
    }
    else
    {
 	    iRet = WifiSetAP(0, &stApxInfo);
    }
    if(iRet != NET_OK)
    {
		lcdClrLine(2, 7);
		PubDispString("Set AP FAILURE",3|DISP_LINE_CENTER);
		PubDispString("TRY IT AGAIN",5|DISP_LINE_CENTER); 
        lcdDisplay(96, 7, DISP_ASCII, "%d", iRet);
		PubWaitKey(5);
	    return ERR_NO_DISP;
    }

    iRet = WifiSetParms(&stWIFIConfig);
    if(iRet != NET_OK)
    {
		lcdClrLine(2, 7);
		PubDispString("Set WIFI FAILURE",3|DISP_LINE_CENTER);
		PubDispString("TRY IT AGAIN",5|DISP_LINE_CENTER);
        lcdDisplay(96, 7, DISP_ASCII, "%d", iRet);
		PubWaitKey(5);
		return ERR_NO_DISP;
    }

	SetWiFiLoaclIPCfg();

	Display2StringInRect("请稍候...","RESET WI-FI");
    iRet = WifiSoftReset(0);
    if(iRet != NET_OK)
    {
		lcdClrLine(2, 7);
		PubDispString("Set WIFI FAILURE",3|DISP_LINE_CENTER);
		PubDispString("TRY IT AGAIN",5|DISP_LINE_CENTER);
        lcdDisplay(96, 7, DISP_ASCII, "%d", iRet);
		PubWaitKey(5);
		return ERR_NO_DISP;
    }

	gstSysParam.stWIFIAPInfo.iChannel = stWIFIConfig.Channel;
	gstSysParam.stWIFIAPInfo.iSecurityType = stApxInfo.SecurityType;
	iKeyIndex = gstSysParam.stWIFIAPInfo.iWEPKeyIdx = stDefApInfo.WEPKeyIdx; 
	strcpy(gstSysParam.stWIFIAPInfo.szListWEPKey[iKeyIndex],stDefApInfo.WEPKey[iKeyIndex]);
	strcpy(gstSysParam.stWIFIAPInfo.szWPAPSK,stApxInfo.WPAPSK);
	strcpy(gstSysParam.stWIFIAPInfo.szSSID,stApxInfo.SSID);
	SaveAppTransParam();
#endif
	return 0;
}

int    SetWIFIParam(int iMode)
{
#ifdef _POS_TYPE_8210
	int		iRet;
	
	if (!(gstPosCapability.uiCommType & CT_WIFI))
	{
		return ERR_COMM_INV_TYPE;
	}
		
	if (iMode == DOWNLAOD_COMMTYPE)
	{
		PubDisplayTitle(TRUE, "WIFI PARA");	
		iRet = GetIPAddress((UINT8 *)"REMOTE IP", FALSE, gstCurAcq.stTxnCommCfg.stWIFIIpPara.szRemoteIP);
		if( iRet!=0 )
		{
			return iRet;
		}
		
		iRet = GetIPPort((UINT8 *)"PORT", gstCurAcq.stTxnCommCfg.stWIFIIpPara.szRemotePort);
		if( iRet<0 )
		{
			return iRet;
		}
		
		return 0;
	}
	
	PubDisplayTitle(TRUE, "WIFI PARA");	
	iRet = GetIPAddress((uint8_t *)"PRIMARY IP", FALSE, (uint8_t *)gstCurAcq.szRemoteIP);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = GetIPPort((uint8_t *)"PRIMARY PORT", (uint8_t *)gstCurAcq.szRemotePort);
	if( iRet<0 )
	{
		return iRet;
	}
	
	iRet = GetIPAddress((uint8_t *)"SECOND IP", FALSE, (uint8_t *)gstCurAcq.szRemoteIP2);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = GetIPPort((uint8_t *)"SECOND PORT", (uint8_t *)gstCurAcq.szRemotePort2);
	if( iRet<0 )
	{
		return iRet;
	}
	
	return 0;

#else

	int		iRet, iFlag;
	WiFiStatus_t    stWiFiStatus;
	char    szlocalIP[20], sVersion[10];
	UINT32  uiOldTime;
	
	if (!(gstPosCapability.uiCommType & CT_WIFI))
	{
		return ERR_COMM_INV_TYPE;
	}

	if (gstPosCapability.uiPosType == 0x02) // 6110 的WIFI和TCP一样处理
	{
		PubDisplayTitle(TRUE, (UINT8 *)"WIFI PARA");	
		iRet = GetIPAddress((UINT8 *)"REMOTE IP", FALSE, gstCurAcq.stTxnCommCfg.stWIFIIpPara.szRemoteIP);
		if( iRet!=0 )
		{
			return iRet;
		}
		
		iRet = GetIPPort((UINT8 *)"PORT", gstCurAcq.stTxnCommCfg.stWIFIIpPara.szRemotePort);
		if( iRet<0 )
		{
			return iRet;
		}
		
		return 0;
	}

	memset(sVersion,0,sizeof(sVersion));
	sysReadVer(sVersion);
	if (sVersion[6]== 0x30 || sVersion[6]==0x32)
	{
		uiOldTime = sysGetTimerCount();
		while(1)
		{
			if ( sysGetTimerCount() >= (uiOldTime + 20000) )
			{ // 拨号连接超时10s，则返回
				return ERR_NO_DISP;
			}
			
			if( PubChkKeyInput() )
			{
				if (kbGetKey() == KEY_CANCEL)
				{
					return ERR_USERCANCEL;
				}
			}
			
			iRet = WnetSelectModule(1);	
			if (iRet < 0)
			{
				sysDelayMs(200);
				continue;
			}
			
			break;
		}
	}
	
	WnetInit(20000);
	iFlag = 1;
	while(iFlag)
	{
		iRet = WifiGetCurConnect(&stWiFiStatus);
		PubDisplayTitle(TRUE, "WIFI CONFIG");
		lcdDisplay(0,2,DISP_ASCII,"%16.16s",stWiFiStatus.SSID);
		if (stWiFiStatus.Status != WIFI_STATUS_CONNECTED)
			lcdDisplay(0, 3, DISP_ASCII, "STATUS: DISCONNECT");
		else
			lcdDisplay(0, 3, DISP_ASCII, "STATUS: CONNECTED");
		lcdDisplay(0, 4, DISP_ASCII, "SIG: %d%",stWiFiStatus.SigLevel);
		if (stWiFiStatus.LnkQual > 75)
		{
			lcdDisplay(0, 4, DISP_ASCII, "QUALITY: EXCELLED");
		}
		else if (stWiFiStatus.LnkQual > 50)
		{
			lcdDisplay(0, 4, DISP_ASCII, "QUALITY: GOOD");
		}
		else
		{
			lcdDisplay(0, 4, DISP_ASCII, "QUALITY: BAD");
		}
		
		memset(szlocalIP, 0, sizeof(szlocalIP));
		iRet = WifiGetLocalIP(szlocalIP);	
		if (iRet !=0 || strcmp(szlocalIP,"0.0.0.0")==0 )
			lcdDisplay(0, 5, DISP_ASCII, "LOCAL IP: 0.0.0.0");
		else
			lcdDisplay(0, 5, DISP_ASCII, "LOCAL:%s",szlocalIP);	
		lcdDisplay(0,6,DISP_CFONT,"[FN/MENU] MODIFY");
		iRet = PubWaitKey(10);
		switch (iRet)
		{
		case KEY_FN:
		case KEY_MENU:
			SelectAndSetWIFIAP();
			break;
		case KEY_CANCEL:
			iFlag = 0;
			break;
		default:
			break;
		}
	}

	iRet = InitWiFiModule();

	if (iMode == DOWNLAOD_COMMTYPE)
	{
		PubDisplayTitle(TRUE, (UINT8 *)"WIFI PARA");	
		iRet = GetIPAddress((UINT8 *)"REMOTE IP", FALSE, gstCurAcq.stTxnCommCfg.stWIFIIpPara.szRemoteIP);
		if( iRet!=0 )
		{
			return iRet;
		}
		
		iRet = GetIPPort((UINT8 *)"PORT", gstCurAcq.stTxnCommCfg.stWIFIIpPara.szRemotePort);
		if( iRet<0 )
		{
			return iRet;
		}

		return 0;
	}

	PubDisplayTitle(TRUE, (UINT8 *)"WIFI PARA");	
	iRet = GetIPAddress((uint8_t *)"PRIMARY IP", FALSE, gstCurAcq.szRemoteIP);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = GetIPPort((uint8_t *)"PRIMARY PORT", gstCurAcq.szRemotePort);
	if( iRet<0 )
	{
		return iRet;
	}
	
	iRet = GetIPAddress((uint8_t *)"SECOND IP", FALSE, gstCurAcq.szRemoteIP2);
	if( iRet!=0 )
	{
		return iRet;
	}
	
	iRet = GetIPPort((uint8_t *)"SECOND PORT", gstCurAcq.szRemotePort2);
	if( iRet<0 )
	{
		return iRet;
	}
	
	return 0;
#endif
}


int    SetWiFiLoaclIPCfg(void)
{
#ifndef _POS_TYPE_8210
    int         iRet;
    tcpipcfg_t  stdevcfg;
    char        szIpaddr[64];

    PubDisplayTitle(TRUE,"SETUP LOACL IP");
    memset(&stdevcfg, 0, sizeof(stdevcfg));
    iRet = NetGetTCPIPCfg(OPT_DEVWIFI, &stdevcfg);
    if(iRet != NET_OK)
    {
        lcdDisplay(0, 2, DISP_CFONT|DISP_CLRLINE, "Get CFG error");
        lcdDisplay(0, 4, DISP_CFONT|DISP_CLRLINE, "RET = %d", iRet);
        PubWaitKey(USER_OPER_TIMEOUT);
        return(iRet);
    }

    lcdDisplay(0, 0, DISP_ASCII|DISP_CLRLINE, "DHCP: %s", stdevcfg.dhcp==NO ? "DISABLE" : "ENABLE");
    memset(szIpaddr, 0, sizeof(szIpaddr));
    inet_ntop(AF_INET, &(stdevcfg.localip), szIpaddr, sizeof(szIpaddr));
	lcdDisplay(0, 1, DISP_ASCII|DISP_CLRLINE, "IP  : %s", szIpaddr);

    memset(szIpaddr, 0, sizeof(szIpaddr));
    inet_ntop(AF_INET, &(stdevcfg.subnet), szIpaddr, sizeof(szIpaddr));
	lcdDisplay(0, 2, DISP_ASCII|DISP_CLRLINE, "MASK: %s", szIpaddr);

    memset(szIpaddr, 0, sizeof(szIpaddr));
    inet_ntop(AF_INET, &(stdevcfg.gateway), szIpaddr, sizeof(szIpaddr));
	lcdDisplay(0, 3, DISP_ASCII|DISP_CLRLINE, "GATE: %s", szIpaddr);

    memset(szIpaddr, 0, sizeof(szIpaddr));
    inet_ntop(AF_INET, &(stdevcfg.dns1), szIpaddr, sizeof(szIpaddr));
	lcdDisplay(0, 4, DISP_ASCII|DISP_CLRLINE, "DNS1: %s", szIpaddr);

    memset(szIpaddr, 0, sizeof(szIpaddr));
    inet_ntop(AF_INET, &(stdevcfg.dns2), szIpaddr, sizeof(szIpaddr));
	lcdDisplay(0, 5, DISP_ASCII|DISP_CLRLINE, "DNS2: %s", szIpaddr);

    lcdDisplay(0, 6, DISP_CFONT, "[0]--Modify");

    kbFlush();
	lcdFlip();
    iRet = kbGetKey();
    if(iRet == KEY0)
    {
		PubDisplayTitle(TRUE,"SETUP LOACL IP");
        lcdDisplay(0, 2, DISP_CFONT, "SET DHCP:");
        if(stdevcfg.dhcp == NO)
        {
            lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "DISABLE");
        }
        else
        {
            lcdDisplay(0, 4, DISP_CFONT|DISP_MEDIACY, "ENABLE");
        }
		lcdDisplay(0, 6, DISP_CFONT, "1:ENABLE*  0:DIS");
        kbFlush();
        iRet = PubWaitKey(USER_OPER_TIMEOUT);
        if(iRet == KEY0)
        {
            stdevcfg.dhcp = NO;
        }
        else if(iRet == KEY1)
        {
            stdevcfg.dhcp = YES;
        }
        else 
        {
            return(KB_CANCEL);
        }

        if(stdevcfg.dhcp == NO)
        {
            lcdClrLine(2, 7);
			iRet = GetIPAddress((uint8_t *)"LOCAL IP", TRUE, gstCurAcq.stTxnCommCfg.stWIFIIpPara.szLocalIP);
			if( iRet!=0 )
			{
				return iRet;
			}
			inet_pton(AF_INET,gstCurAcq.stTxnCommCfg.stWIFIIpPara.szLocalIP,&stdevcfg.localip);

            lcdClrLine(2, 7);
			iRet = GetIPAddress((uint8_t *)"MASK ADDR", TRUE, gstCurAcq.stTxnCommCfg.stWIFIIpPara.szNetMask);
			if( iRet!=0 )
			{
				return iRet;
			}
			inet_pton(AF_INET,gstCurAcq.stTxnCommCfg.stWIFIIpPara.szNetMask,&stdevcfg.subnet);

            lcdClrLine(2, 7);
			iRet = GetIPAddress((uint8_t *)"GATEWAY", TRUE, gstCurAcq.stTxnCommCfg.stWIFIIpPara.szGatewayIP);
			if( iRet!=0 )
			{
				return iRet;
			}
			inet_pton(AF_INET,gstCurAcq.stTxnCommCfg.stWIFIIpPara.szGatewayIP,&stdevcfg.gateway);

            lcdClrLine(2, 7);
			iRet = GetIPAddress((uint8_t *)"DNS", TRUE, gstCurAcq.stTxnCommCfg.stWIFIIpPara.szDNS);
			if( iRet!=0 )
			{
				return iRet;
			}
			inet_pton(AF_INET,gstCurAcq.stTxnCommCfg.stWIFIIpPara.szDNS,&stdevcfg.dns1);
        }
        lcdClrLine(2, 7);
        lcdDisplay(0, 3, DISP_CFONT, "SAVE CFG...");
        iRet = NetSetTCPIPCfg(OPT_DEVWIFI, &stdevcfg);
        if(iRet != NET_OK)
        {
            lcdDisplay(0, 3, DISP_CFONT|DISP_CLRLINE, "Save CFG error");
            lcdDisplay(0, 5, DISP_CFONT|DISP_CLRLINE, "RET = %d", iRet);
            PubWaitKey(USER_OPER_TIMEOUT);
            return(iRet);
        }
    }
#endif
    return(OK);
}


int    SetMsgHeadLenType(void)
{
	int     iKey , iFlag;
	
	iFlag = 1;
	while( iFlag )
	{
		lcdClrLine(2, 7);
		lcdDisplay(0,2,DISP_CFONT|DISP_MEDIACY,"MSG LEN TYPE");
		if (gstSysParam.stEdcInfo.ucTcpMsgHeadLenType != _TCP_BCD_LEN_BYTE)
			lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"(HEX)");
		else
			lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"(BCD)");
		lcdDisplay(0,6,DISP_CFONT|DISP_MEDIACY,"1.BCD  0.HEX");
		iKey = PubWaitKey(USER_OPER_TIMEOUT);
		switch (iKey)
		{
		case KEY1:
			gstSysParam.stEdcInfo.ucTcpMsgHeadLenType = _TCP_BCD_LEN_BYTE;
			break;
		case KEY0:
			gstSysParam.stEdcInfo.ucTcpMsgHeadLenType = 0;
			break;
		case KEY_ENTER:
		case KEY_CANCEL:
			iFlag = 0;
			break;
		default:
			PubBeepErr();
			break;
		}
	}
	
	return 0;
}


void   SetMerchantInfo(void)
{
	PubDisplayTitle(TRUE, "EDC PARAM");
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDispString("MERCHANT NAME", DISP_LINE_CENTER|2);
	else
		PubDispString("商户名称",      DISP_LINE_CENTER|2);
	if( PubGetString(ECHO_IN|ALPHA_IN, 2, 30, (uint8_t *)gstSysParam.stEdcInfo.szMerchantName, USER_OPER_TIMEOUT)!=0 )
	{
		return;
	}

	PubDisplayTitle(TRUE, "EDC PARAM");
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDispString("ADDRESS LINE1", DISP_LINE_CENTER|2);
	else
		PubDispString("商户地址一",      DISP_LINE_CENTER|2);
	if( PubGetString(ECHO_IN|ALPHA_IN, 2, 40, (uint8_t *)gstSysParam.stEdcInfo.szMerchantAddr, USER_OPER_TIMEOUT)!=0 )
	{
		return;
	}

	PubDisplayTitle(TRUE, "EDC PARAM");
	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDispString("ADDRESS LINE2", DISP_LINE_CENTER|2);
	else
		PubDispString("商户地址二",      DISP_LINE_CENTER|2);
	if( PubGetString(ECHO_IN|ALPHA_IN, 0, 40, (uint8_t *)&gstSysParam.stEdcInfo.szMerchantAddr[40], USER_OPER_TIMEOUT)!=0 )
	{
		return;
	}
}


int    SetAutoSettlmentTime(void)
{
	uint8_t	szBuff[20+1];
	unsigned long     ulTemp;
	
	memset(szBuff,0,sizeof(szBuff));	
	while(1)
	{
		lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("AUTO SETTLE TIME", DISP_LINE_LEFT|2);
		else
			PubDispString("自动结算时间",   DISP_LINE_LEFT|2);
		sprintf((char *)szBuff, "(HH:MM)%2.2s:%2.2s", gstSysParam.szAutoSettlementTime,gstSysParam.szAutoSettlementTime+2);
		PubDispString(szBuff,   DISP_LINE_CENTER|4);

		memset(szBuff,0,sizeof(szBuff));
		if( PubGetString(NUM_IN|ECHO_IN, 4, 4, szBuff, USER_OPER_TIMEOUT)!=0 )
		{
			return ERR_USERCANCEL;
		}

		ulTemp = PubAsc2Long((char *)szBuff,2,NULL);
		if (ulTemp>=24)
		{
			lcdClrLine(2,7);
			PubDispString("Hour range error",3|DISP_LINE_CENTER);
			PubDispString("range: 0--23",    5|DISP_LINE_CENTER);
			PubBeepErr();
			PubWaitKey(2);
			continue;
		}

		ulTemp = PubAsc2Long((char *)szBuff+2,2,NULL);
		if (ulTemp>=60)
		{
			lcdClrLine(2,7);
			PubDispString("Min. range error",3|DISP_LINE_CENTER);
			PubDispString("range: 0--59",    5|DISP_LINE_CENTER);
			PubBeepErr();
			PubWaitKey(2);
			continue;
		}

		break;
	}

	memcpy(gstSysParam.szAutoSettlementTime,szBuff,4);
	SaveAppTransParam();	
	return 0;
}

//end of file
