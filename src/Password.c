
#include "global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
typedef struct _tagPASSWD_INFO
{
	char	szChnMsg[16+1];
	char	szEngMsg[16+1];
	int  	iMinPwdLen; 
	int     iMaxPwdLen;
}PASSWD_INFO;

/********************** Internal functions declaration *********************/
static int     CheckPassWord(int iPwdType);
static int     InputNewPwd(char *psOutPwd, int iPwdLen);
static void    ModifyPassWord(int iPwdType);
static void    InputMasterKey(void);
static void    InputZMKey(void);
static void    DownMasterKeyFromHost(void);
static int  MainPosRS232Rxd(unsigned char *psRxdData, unsigned int uiExpLen, unsigned int uiTimeOut, unsigned int *puiOutLen);
/********************** Internal variables declaration *********************/
static PASSWD_INFO sgPwdInfo[] =
{
	{"请输入银行密码", "SYSTEM  PWD",      6, 6},
	{"请输入终端密码", "TERMINAL PWD",     4, 4},
	{"请输入银行密码", "SYSTEM  PWD2",     6, 6},
	{"请输入取消密码", "VOID PASSWORD",    4, 4},
	{"请输入退款密码", "REFUND PASSWORD",  4, 4},
	{"请输入调整密码", "ADJUST PASSWORD",  4, 4},
	{"请输入结算密码", "SETTLEMENT PWD",   4, 4},
};

/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/
int CheckBankPWD(void)
{
	return CheckPassWord(BANK_PWD);
}

int CheckBankPWD2(void)
{
	return CheckPassWord(BANK_PWD2);
}

int CheckTermPWD(void)
{
	return CheckPassWord(TERM_PWD);
}


int CheckTransVoidPWD(void)
{
//	return CheckPassWord(VOID_PWD);
	return CheckPassWord(TERM_PWD);
}

int CheckTransRefundPWD(void)
{
	return CheckPassWord(REFUND_PWD);
}

int CheckTransAdjustPWD(void)
{
//	return CheckPassWord(ADJUST_PWD);
	return CheckPassWord(TERM_PWD);
}

int CheckTransSettlePWD(void)
{
//	return CheckPassWord(SETTLE_PWD);
	return CheckPassWord(TERM_PWD);
}

void ModifyBankPWD(void)
{
	ModifyPassWord(BANK_PWD);
}

void ModifyTermPWD(void)
{
	ModifyPassWord(TERM_PWD);
}


void ModifyTransVoidPWD(void)
{
	ModifyPassWord(VOID_PWD);
}

void ModifyTransRefundPWD(void)
{
	ModifyPassWord(REFUND_PWD);
}

void ModifyTransAdjustPWD(void)
{
	ModifyPassWord(ADJUST_PWD);
}

void ModifyTransSettlePWD(void)
{
	ModifyPassWord(SETTLE_PWD);
}

int CheckPassWord(int iPwdType)
{
	int   iRet;
	uchar  szBuff[20];

	if( iPwdType>sizeof(sgPwdInfo)/sizeof(sgPwdInfo[0]) )
	{
		return 1;
	}

	while( 1 )
	{
		Display2StringHigh(sgPwdInfo[iPwdType].szChnMsg, sgPwdInfo[iPwdType].szEngMsg);
		memset(szBuff, 0, sizeof(szBuff));

		iRet = PubGetString(PASS_IN, sgPwdInfo[iPwdType].iMinPwdLen,sgPwdInfo[iPwdType].iMaxPwdLen, szBuff, USER_OPER_TIMEOUT);
		if( iRet!=0 )
		{
			return 1;
		}

		if( memcmp(gstSysParam.szPassword[iPwdType], szBuff,sgPwdInfo[iPwdType].iMinPwdLen)==0)
		{
			return 0;
		}

		Display2StringInRect("密码错误", "PWD ERROR!");
		PubBeepErr();
		PubWaitKey(3);
	}
}

void ModifyPassWord(int iPwdType)
{
	if( CheckPassWord(iPwdType)!=0 )
	{
		return;
	}

	if( InputNewPwd(gstSysParam.szPassword[iPwdType], sgPwdInfo[iPwdType].iMaxPwdLen)!=0 )
	{
		return;
	}

	SavePassword();
	PubDisplayTitle(TRUE, "CHANGE PWD");
	Display2StringInRect("改密码成功", "PWD CHANGED!");
	PubBeepOk();
	PubWaitKey(3);
}

int InputNewPwd(char *psOutPwd, int iPwdLen)
{
	uchar	sBuff1[20], sBuff2[20];
	int     iRet;

	while( 1 )
	{
		Display2StringHigh("请输入新密码", "INPUT NEW PWD");

		iRet = PubGetString(PASS_IN, iPwdLen, iPwdLen, sBuff1, USER_OPER_TIMEOUT);
		if( iRet!=0 )
		{
			return 1;
		}

		Display2StringHigh("请再输入一次", "REINPUT NEW PWD");

		iRet = PubGetString(PASS_IN, iPwdLen, iPwdLen, sBuff2, USER_OPER_TIMEOUT);
		if( iRet!=0 )
		{
			return 1;
		}

		if( memcmp(sBuff1, sBuff2, iPwdLen)==0 )
		{
			memcpy(psOutPwd, sBuff1, iPwdLen);
			return 0;
		}

		PubDisplayTitle(TRUE, "CHANGE PWD");
		Display2StringInRect("密码不一致", "NOT CONSISTENT");
		PubBeepErr();
		PubWaitKey(3);
	}
}


void    DownMasterKey(void)
{
	int  iKey,iInputType;

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "DOWNLOAD KEY");
	else
		PubDisplayTitle(TRUE, "下载密钥");
	if( CheckTermPWD()!=0 )
	{
		return;
	}

	iInputType = 0;
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDisplayTitle(TRUE, "DOWNLOAD KEY");
	else
		PubDisplayTitle(TRUE, "下载密钥");
	while(1)
	{
		lcdClrLine(2,7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDisplayTitle(TRUE, "DOWNLOAD KEY");
			lcdDisplay(0, 2, DISP_CFONT, "1.DOWNLOAD KEY");
			lcdDisplay(0, 4, DISP_CFONT, "2.INPUT ZMK");
			lcdDisplay(0, 6, DISP_CFONT, "3.INPUT TMK");
		}
		else
		{
			PubDisplayTitle(TRUE, "下载密钥");
			lcdDisplay(0, 2, DISP_CFONT, "1.下载主密钥");
			lcdDisplay(0, 4, DISP_CFONT, "2.输入加密主密钥");
			lcdDisplay(0, 6, DISP_CFONT, "3.输入主密钥");
		}

		iKey = PubWaitKey(60);
		switch (iKey)
		{
		case KEY1:
			DownMasterKeyFromHost();
			return;
			break;
		case KEY2:
			InputZMKey();
			return;
			break;
		case KEY3:
			InputMasterKey();
			return;
			break;
		case KEY_DOWN:
			if (iInputType < 2)
				iInputType++;
			else
				iInputType = 0;
			break;
		case KEY_UP:
			if (iInputType > 0)
				iInputType--;
			else
				iInputType = 2;
			break;
		case KEY_CANCEL:
			return;
			break;
		case KEY_ENTER:
			if (iInputType==0)
			{
				DownMasterKeyFromHost();
			}
			else if (iInputType==1)
			{
				InputZMKey();
			}
			else
			{
				InputMasterKey();
			}
			return;
			break;
		default:
			PubBeepErr();
			break;
		}
	}

	return;
}


void    InputZMKey(void)
{ 
	int   iRet;
	char  szAsciiKey[50],sBcdKey[50];

	memset(szAsciiKey,0,sizeof(szAsciiKey));
	memset(sBcdKey,0,sizeof(sBcdKey));

	strcpy(szAsciiKey,"C20E79851A8C868C9E29D55EE63D831F");
	PubDisplayTitle(TRUE,"DOWNLOAD ZMK");
	lcdDisplay(0,2,DISP_CFONT,"Input ZMK(32):");
	if( PubGetString(HEX_IN|ECHO_IN, 32, 32, (uint8_t *)szAsciiKey, 60)!=0 )
	{
		return ;
	}

	lcdClrLine(2,7);
	DisplayProcessing();
	PubAsc2Bcd(szAsciiKey,32,sBcdKey);
	iRet = PedWriteMasterKey(TDEA_NONE,ZMK_KEY_ID,ZMK_KEY_ID,16,(uint8_t *)sBcdKey);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存ZMK密钥错误", "SAVE ZMK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	return ;
}


void    InputMasterKey(void)
{
	int   iRet;
	char  szAsciiKey[50],sBcdKey[50];
	
	memset(szAsciiKey,0,sizeof(szAsciiKey));
	strcpy(szAsciiKey,"0E33F894995C94BEE8327C7B086365BF");
	memset(sBcdKey,0,sizeof(sBcdKey));	
	PubDisplayTitle(TRUE,"DOWNLOAD TMK");
	lcdDisplay(0,2,DISP_CFONT,"Input TMK(32):");
	if( PubGetString(HEX_IN|ECHO_IN, 32, 32, (uint8_t *)szAsciiKey, 60)!=0 )
	{
		return ;
	}
	
	lcdClrLine(2,7);
	DisplayProcessing();	
	PubAsc2Bcd(szAsciiKey,32,sBcdKey);
	memset(szAsciiKey,0,sizeof(szAsciiKey));
/*	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sBcdKey,(uint8_t *)szAsciiKey);
	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sBcdKey+8,(uint8_t *)szAsciiKey+8);	
//	PubXor(szAsciiKey+8,sBcdKey,8);
	iRet = PedWriteMasterKey(TDEA_NONE,MASTER_KEY_ID,MASTER_KEY_ID,16,(uint8_t *)szAsciiKey);*/
	iRet = PedWriteMasterKey(TDEA_DECRYPT,ZMK_KEY_ID,MASTER_KEY_ID,16,(uint8_t *)sBcdKey);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存PKEY密钥错误", "SAVE TMK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	PubDisplayTitle(TRUE,"DOWNLOAD TAK");
	lcdDisplay(0,2,DISP_CFONT,"Input TAK(32):");
	strcpy(szAsciiKey,"0EDADB8DB87FD5AA64886856E727CA86");
	if( PubGetString(HEX_IN|ECHO_IN, 32, 32, (uint8_t *)szAsciiKey, 60)!=0 )
	{
		return ;
	}
	
	lcdClrLine(2,7);
	DisplayProcessing();	
	PubAsc2Bcd(szAsciiKey,32,sBcdKey);
	memset(szAsciiKey,0,sizeof(szAsciiKey));
/*	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sBcdKey,(uint8_t *)szAsciiKey);
	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sBcdKey+8,(uint8_t *)szAsciiKey+8);	
//	PubXor(szAsciiKey+8,sBcdKey,8);
	iRet = PedWriteMasterKey(TDEA_NONE,TAK_KEY_ID,TAK_KEY_ID,16,(uint8_t *)szAsciiKey);*/
	iRet = PedWriteMasterKey(TDEA_DECRYPT,ZMK_KEY_ID,TAK_KEY_ID,16,(uint8_t *)sBcdKey);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存PKEY密钥错误", "SAVE TMK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	PubDisplayTitle(TRUE,"Reference Number");
	lcdDisplay(0,2,DISP_CFONT,"Input Ref. No:");
	strcpy(szAsciiKey,"3030303030313331");
	if( PubGetString(HEX_IN, 16, 16, (uint8_t *)szAsciiKey, 60)!=0 )
	{
		return ;
	}
		
	PubAsc2Bcd(szAsciiKey,16,gstSysParam.stEdcInfo.sTmkRefNumber);
	SaveAppTransParam();
	
	return ;
}

void    DownMasterKeyFromHost(void)
{
	int   iRet;
	char  szBuffer[20], szMid[20];
	UINT32 uiPackLenTag, uiDataLen;
	unsigned short usCRC;
	char  *psTemp;

	SetCurAcq(0);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayTitle(TRUE,"DOWNLOAD KEY");
		PubDispString("MERCHANT ID:", DISP_LINE_CENTER|4);
	}
	else
	{
		PubDisplayTitle(TRUE,"密钥下载");
		PubDispString("商户号:",      DISP_LINE_CENTER|4);
	}
	memset(szMid,0,sizeof(szMid));
	strcpy(szMid,gstCurAcq.szMerchantID);
	if( PubGetString(ECHO_IN|ALPHA_IN, 15, 15, (uint8_t *)szMid, USER_OPER_TIMEOUT)!=0 )
	{
		return;
	}

	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayTitle(TRUE,"DOWNLOAD KEY");
		PubDispString("TERMINAL ID", DISP_LINE_CENTER|4);
	}
	else
	{
		PubDisplayTitle(TRUE,"密钥下载");
		PubDispString("终端号:",4|DISP_LINE_CENTER);
	}

	memset(szBuffer,0,sizeof(szBuffer));
	strcpy(szBuffer,gstCurAcq.szTermID);
	if( PubGetString(ECHO_IN|ALPHA_IN, 8, 8, (uint8_t *)szBuffer, USER_OPER_TIMEOUT)!=0 )
	{
		return;
	}
	
	memset(gstSendData.sContent, 0, sizeof(gstSendData.sContent));
	gstSendData.uiLength = 0;
	gstSendData.sContent[0] = 0x02;//begin
	gstSendData.uiLength += 1;
	PubLong2Char(CMD_DOWNLOAD_KEY,2,&gstSendData.sContent[gstSendData.uiLength]);
	gstSendData.uiLength += 2;	
	uiPackLenTag = gstSendData.uiLength;//长度信息
	gstSendData.uiLength += 2;
	memcpy(&gstSendData.sContent[gstSendData.uiLength], szMid, 15);
	gstSendData.uiLength += 15;
	memcpy(&gstSendData.sContent[gstSendData.uiLength], szBuffer, 8);
	gstSendData.uiLength += 8;	
	PubLong2Char(gstSendData.uiLength - 5, 2, &gstSendData.sContent[uiPackLenTag]);//长度信息	
	usCRC = PubCalcCRC((uint8_t *)&gstSendData.sContent[1],(gstSendData.uiLength-1),0);
	PubLong2Char(usCRC,2,&gstSendData.sContent[gstSendData.uiLength]);
	gstSendData.uiLength += 2;

	portClose(PORT_COM1);
	portOpen(PORT_COM1, (void *)"115200,8,n,1");
	psTemp = gstSendData.sContent;
	uiDataLen = gstSendData.uiLength;
	while( uiDataLen-->0 )
	{
		if( portSends(PORT_COM1, psTemp,1)!=0 )
		{
			psTemp++;
			break;
		}
		psTemp++;
	}

	memset(gstRecvData.sContent,0,sizeof(gstRecvData.sContent));
	gstRecvData.uiLength = 0;
	iRet = MainPosRS232Rxd((uint8_t *)gstRecvData.sContent, LEN_MAX_COMM_DATA, 60, &gstRecvData.uiLength);
	portClose(PORT_COM1);
	if (iRet != ERR_RS232_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("接收密钥数据错误", "GET KEY ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	if (memcmp(gstSendData.sContent,gstRecvData.sContent,3) != 0)
	{
		lcdClrLine(2,7);
		Display2Strings("接收密钥数据错误", "GET KEY ERR");
		lcdDisplay(0,7,DISP_ASCII,"DATA ERROR");
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	uiDataLen = PubChar2Long(gstRecvData.sContent+5,4,NULL);
	if (uiDataLen != 1)
	{
		lcdClrLine(2,7);
		Display2Strings("接收密钥数据错误", "NOT GET KEY");
		lcdDisplay(0,7,DISP_ASCII,"No valid key");
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	iRet = PedWriteMasterKey(TDEA_NONE,MASTER_KEY_ID,MASTER_KEY_ID,16,(uint8_t *)gstRecvData.sContent+5+4+15+8);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存PKEY密钥错误", "SAVE TMK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return;
	}

	iRet = PedWriteMasterKey(TDEA_NONE,TAK_KEY_ID,TAK_KEY_ID,16,(uint8_t *)gstRecvData.sContent+5+4+15+8+16);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存PKEY密钥错误", "SAVE TMK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return;
	}
	
	memcpy(gstSysParam.stEdcInfo.sTmkRefNumber,gstRecvData.sContent+5+4+15+8+16+16,8);
	SaveAppTransParam();
	
	lcdClrLine(2,7);
	Display2Strings("成功写入密钥", "PROCESSING OK!");
	PubWaitKey(5);	
	return;
}


//   串口接收数据
int  MainPosRS232Rxd(unsigned char *psRxdData, unsigned int uiExpLen, unsigned int uiTimeOut, unsigned int *puiOutLen)
{
	int             iRet;
	unsigned int 	uiReadCnt, uiLength;
	unsigned int    iOldTimeMs,iNewTimeMs;
	unsigned short  usCRCRet;
	
	uiReadCnt = uiLength = 0;
	iOldTimeMs = sysGetTimerCount();
	while( 1 )
	{
		// 检查是否超时
		iNewTimeMs= sysGetTimerCount();
		if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
		{
			if( uiReadCnt>0 )	// 已经读取到数据
			{
				break;
			}			
			return ERR_RS232_RX_TIMEOUT;
		}
		// 检查接收缓冲区是否有数据		
		iRet = portCheckRecvBuf(PORT_COM1);
		if (iRet != PORT_RET_NOTEMPTY)
		{
			if (iRet != PORT_RET_OK )
			{
				return ERR_RS232_RX_DATA;
			}			
			continue;
		}
		
		uiReadCnt = 0;
		// 报文头
		iRet = portRecv(PORT_COM1, &psRxdData[uiReadCnt], 100);
		if( iRet!=0 )
		{
			if( iRet==0xFF )
			{
				continue;
			}
			return ERR_RS232_RX_DATA;
		}
		if( psRxdData[0]!=STX )
		{
			continue;
		}
		uiReadCnt++;
		
		// 接收2个字节的命令码 和 2 字节数据长度
		for (; uiReadCnt < 5; uiReadCnt++)
		{
			iRet = portRecv(PORT_COM1, &psRxdData[uiReadCnt], 100);
			if( iRet!=0 )
			{
				return ERR_RS232_RX_DATA;
			}
		}
		
		// 数据长度
		uiLength = PubChar2Long((char *)&psRxdData[uiReadCnt-2],2,NULL);
		if (uiLength > uiExpLen)
		{
			return ERR_RS232_RX_LENGTH; // 数据超长
		}
		
		// 接收N个数据长度 和 2字节 CRC校验数据
		uiLength += 5+2;
		for (; uiReadCnt < uiLength; uiReadCnt++)
		{
			iRet = portRecv(PORT_COM1, &psRxdData[uiReadCnt], 100);
			if( iRet!=0 )
			{
				return ERR_RS232_RX_DATA;
			}
		}
		
		// 计算CRC结果: 从命令码开始到CRC数据之前的所有数据
		usCRCRet = PubCalcCRC(psRxdData+1,uiReadCnt-3,0x0000);
		uiLength = PubChar2Long((char *)psRxdData+uiReadCnt-2,2,NULL);
		if (usCRCRet != uiLength)
		{
			return ERR_RS232_RX_CHECKSUM;
		}
		
		break;
	}
	
	if( puiOutLen!=NULL )
	{
		*puiOutLen = uiReadCnt;
	}
	
	return 0;
}

// end of file
