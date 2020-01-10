

//#include "tranfunclist.h"
//#include "posparams.h"
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include "PedSignature.h"


uint8_t CalcLRC(uint8_t *psData, UINT32 uiLength, uint8_t ucInit)
{
	while( uiLength>0 )
	{
		ucInit ^= *psData++;
		uiLength--;
	}

	return ucInit;
}
//int RS232RawTxd(uint8_t *psTxdData, uint32_t uiDataLen, uint32_t uiTimeOut)


static int RS232NacRxdForSign(uint8_t *psRxdData, UINT32 uiExpLen, UINT32 uiTimeOut, UINT32 *puiOutLen)
{
	INT32   iRet ,iFlag;
	UINT32	uiReadCnt, uiLength;
	UINT32  iOldTimeMs,iNewTimeMs,uiTemp;
	uint8_t		sWorkBuf[MAX_PACKET_LEN+50];

	if( uiExpLen>MAX_PACKET_LEN )
	{
		return -1;
	}

	iFlag = 0;
	uiTemp = 5000;
	uiReadCnt = uiLength = 0;
	memset(sWorkBuf, 0x00, sizeof(sWorkBuf));
	iOldTimeMs = sysGetTimerCount();
	while( 1 )
	{
		if (iFlag==0)
		{
			iNewTimeMs= sysGetTimerCount();
			if( (iNewTimeMs-iOldTimeMs) >= (UINT32)(uiTimeOut *1000) )	// 检查定时器
			{
				if( uiReadCnt>0 )	// 已经读取到数据
				{
					break;
				}
				return -1;
			}

			if( kbhit())
			{
				if (kbGetKey() == KEY_CANCEL)
				{
					return KEY_CANCEL;
				}
			}

//			lcdDisplay(87,4,DISP_CFONT,"(%d) ",(iNewTimeMs-iOldTimeMs)/1000);
			iRet = portCheckRecvBuf(PORT_COM1);
			if (iRet != PORT_RET_NOTEMPTY)
			{
				if (iRet != PORT_RET_OK )
				{
					return iRet;
				}
				continue;
			}
		}

		iRet = portRecv(PORT_COM1, &sWorkBuf[uiReadCnt], uiTemp);//uiTemp
		if( iRet!=0 )
		{
			if( iRet==0xFF )
			{
				continue;
			}
			return iRet;
		}
		uiTemp = 100;
		if (0 == iFlag)
		{
			if( sWorkBuf[0]!=STX )
			{
				continue;
			}

			iFlag = 1;
		}

		uiReadCnt++;
		if (uiReadCnt > uiExpLen)
		{
			return -1;
		}

		if( uiReadCnt==3 )
		{
			uiLength = (sWorkBuf[1]<<8)|sWorkBuf[2];

		}
		if( uiReadCnt==uiLength+4 ) //签字板协议有状态位
		{	// read data ok, verify it ...
			if( sWorkBuf[uiReadCnt-2]==ETX &&
				CalcLRC(&sWorkBuf[1], (UINT32)(uiReadCnt-1), 0)==0 )
			{
				break;
			}
			return -1;
		}
	}

	memcpy(psRxdData, &sWorkBuf[3], uiLength);
	if( puiOutLen!=NULL )
	{
		*puiOutLen = uiLength;
	}

	return 0;
}

static INT32 RS232NacTxd1(uint8_t *psTxdData, UINT32 uiDataLen, UINT32 uiTimeOut)
{
	INT32		iRet;
	uint8_t		sWorkBuf[MAX_PACKET_LEN+50];
//	char sz1[25], sz2[55];

	if( uiDataLen>MAX_PACKET_LEN )
	{
		return -1;
	}

	sWorkBuf[0] = STX;

	sWorkBuf[1] = (uiDataLen>>8)&0xFF;
	sWorkBuf[2] = uiDataLen&0xFF;

	uiDataLen -=1; //carel
	memcpy(&sWorkBuf[3], psTxdData, uiDataLen);
	sWorkBuf[3+uiDataLen]   = ETX;
	sWorkBuf[3+uiDataLen+1] = CalcLRC(psTxdData, uiDataLen, (uint8_t)(sWorkBuf[1] ^ sWorkBuf[2] ^ ETX));


	portClose(PORT_COM1);
	iRet = portOpen(PORT_COM1, (void *)"115200,8,N,1");
	if(iRet!=0)
	{
		return iRet;
	}
	iRet = RS232RawTxd(sWorkBuf, (UINT32)(uiDataLen+5), uiTimeOut);	// data
	if( iRet!=0 )
	{
		return iRet;
	}

//	portClose(PORT_COM1);

	return 0;
}

int SaveImageFile(char *filename, uint8_t *filebuf, uint32_t filelen)
{
	int fd;
	int ret;
	
	fd = fileOpen(filename, O_RDWR|O_CREAT);
	if( fd<0 ) 
	{
		return (ERR_OPENFILE);
	}
	
	ret = fileSeek(fd, (int)0, SEEK_SET);
	if( ret<0 ) 
	{
		fileClose(fd);
		return (ERR_SEEKFILE);
	}
	ret = fileWrite(fd, (uint8_t *)filebuf, filelen);
	if( ret!=filelen )
	{
		fileClose(fd);
		return (ERR_WRITEFILE);
	}
	fileClose(fd);
	return (OK);
}

int SetElecSignTimeOut(uint16_t usTimeOutMs)
{
	int iRet, uiTimeOut;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;

	memset(szSendBuf, 0, sizeof(szSendBuf));
	szSendBuf[0] = 0xF7;
	usSendLen = 1;

	szSendBuf[1] = (usTimeOutMs/100)/256;
	szSendBuf[2] = (usTimeOutMs/100)%256;
	usSendLen += 2;
	
	usSendLen += 1;
	iRet = RS232NacTxd1(szSendBuf, usSendLen, 5);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	memset(szRecvBuf, 0x00, sizeof(szRecvBuf));
	uiTimeOut = 3;
	iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, uiTimeOut, &usRecvLen);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	if(memcmp(szRecvBuf, "\xF7\x00",2)==0)
	{
		return 0;
	}
	
	return 1;
}

int ShakeHands(void)
{
	int iRet, uiTimeOut;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;

	memset(szSendBuf, 0, sizeof(szSendBuf));
	szSendBuf[0] = 0xA0;
	usSendLen = 1;
	usSendLen += 1;
	iRet = RS232NacTxd1(szSendBuf, usSendLen, 5);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	memset(szRecvBuf, 0x00, sizeof(szRecvBuf));
	uiTimeOut = 3;
	iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, uiTimeOut, &usRecvLen);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	if(memcmp(szRecvBuf, "\xB0\x01",2)==0)
	{
		return 0;
	}
	
	return 1;
}

int InputSign(char *TxnCode, char *ImagefileName, uint8_t ImageType, uint32_t timeOutMs)
{
	int iRet;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;
	unsigned long ulCompressLen=0;
	uint8_t  BMP_SIGN[1024*10+20];
	UINT8 sBuf[1024*10+20];
	
	unsigned long ulBmpSignLen = 0;		   // 所获取到的bmp签名的长度

	memset(szSendBuf, 0, sizeof(szSendBuf));
	szSendBuf[0] = 0xA2;
	usSendLen = 1;

	sprintf((char*)&szSendBuf[1], "%.8s", TxnCode);
	usSendLen += 8;

	usSendLen += 1; //该终端需要把etx算上
	iRet = RS232NacTxd1(szSendBuf, usSendLen, 10);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	memset(szRecvBuf, 0, sizeof(szRecvBuf));
	iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, timeOutMs/1000, &usRecvLen);
	if(iRet !=0)
	{
		portClose(PORT_COM1);
		return iRet;
	}

	if (szRecvBuf[0] == (uint8_t)'\xB2')
	{
		memset(BMP_SIGN, 0x00, sizeof(BMP_SIGN));
		memcpy(BMP_SIGN, &szRecvBuf[4], usRecvLen - 5);
		ulBmpSignLen = usRecvLen - 5;
		portClose(PORT_COM1);
		if(ImageType==IMAGE_BMP)
		{
			return SaveImageFile(ImagefileName, BMP_SIGN, ulBmpSignLen);
		}
		else
		{
			memcpy(sBuf,BMP_SIGN,ulBmpSignLen);
//输出的bmp文件是黑底白字，转换成白底黑字，只需转换文件头的像素			
			memcpy(&sBuf[54],"\x00\x00\x00\x00\xFF\xFF\xFF\x00",8);
			iRet=SaveImageFile("temp.bmp", sBuf, ulBmpSignLen);
			if(iRet!=OK)
				return iRet;
			iRet=bmp2jbg("temp.bmp", ImagefileName);			
			fileRemove("temp.bmp");
			return iRet;


		}
	}
	else if (memcmp(szRecvBuf, "\xC2\x03",2)==0)
	{
		portClose(PORT_COM1);
		return 1;
	}
	else
	{
		portClose(PORT_COM1);
		return 1;
	}

	return 0;
}

int AfterSignReply(UINT8 ucFlag)
{
	int iRet, uiTimeOut, iTry;
	uint8_t	szSendBuf[MAX_PACKET_LEN], szRecvBuf[MAX_PACKET_LEN];
	uint32_t usSendLen=0, usRecvLen;

	for (iTry = 0; iTry < 3; iTry++) // 最多尝试3次发起签名结束请求报文
	{
		memset(szSendBuf, 0, sizeof(szSendBuf));
		szSendBuf[0] = 0xA3;
		usSendLen = 1;
		usSendLen += 1; //该终端需要把etx算上
		iRet = RS232NacTxd1(szSendBuf, usSendLen, 10);
		if(iRet !=0)
		{
			continue;
		}

		if (TRUE == ucFlag) // 经过测试证明，如果是POS端主动取消签名，则pos端发出结束签名请求，而无需等待回应，手写板也不会有回应
		{
			return 0;
		}

		memset(szRecvBuf, 0, sizeof(szRecvBuf));
		uiTimeOut = 3;
		iRet = RS232NacRxdForSign(szRecvBuf, MAX_PACKET_LEN, uiTimeOut, &usRecvLen);
		if(iRet !=0)
		{
			continue;
		}

		if(memcmp(szRecvBuf, "\xB3", 1)==0)
		{
			return 0;
		}

	}
	portClose(PORT_COM1);
	return 1;
}

int PedSignature(char *TxnCode, char *ImagefileName, uint8_t ImageType, uint32_t timeOutMs)
{
    int iRet;

	//参数检查
	if(TxnCode==NULL || strlen(TxnCode)==0 
		|| ImagefileName==NULL || strlen(ImagefileName)==0
		|| (ImageType!=IMAGE_BMP && ImageType!=IMAGE_JBG))
	{
		return 1;
	}

	//默认40秒
	if(timeOutMs==0)
	{
		timeOutMs = 40000;
	}
	
	iRet = ShakeHands();
	if(iRet !=0)
	{
		return iRet;
	}
	
	iRet = InputSign(TxnCode, ImagefileName, ImageType, timeOutMs);
	if (iRet == ERR_USERCANCEL)
	{
		AfterSignReply(TRUE);
		return iRet;
	}
	else
	{
		AfterSignReply(FALSE); // 不管是否成功获取签名，也得发起结束签名命令
	}
	if(iRet !=0)
	{
		return iRet;
	}

	return 0;
}


int P_GetElecSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo)
{
	lcdClrLine(2, 7);	
	lcdDisplay(0, 2, DISP_CFONT, "%s", pstElecSignInfo->szPrompt);
	lcdFlip();
	return PedSignature(pstElecSignInfo->szTxnInfoCode 
		,pstElecSignInfo->szImagefileName, pstElecSignInfo->ucImageType
		,pstElecSignInfo->ulTimeout);
}


uint8_t TransElecSignature(void)          // 电子签名
{	
	char     szBuffer[1024],szTmp[30];
	int      iRet;
	ST_TS_ELEC_SIGNATURE  stElecSignaInfo;
//	IDirectFBFont * font;
	
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

//	font = lcdGetFontExt(DISP_ASCII);
//	PosTsSetFont(font,DISP_ASCII);
//	PosTsSetFont(font,DISP_CFONT);
	iRet = P_GetElecSignature(&stElecSignaInfo);
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



//end
