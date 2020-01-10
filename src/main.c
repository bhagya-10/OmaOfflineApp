
#include "global.h"
#include "Param.h"
#include "seos.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void CheckTermSatus(void);
static void AppFirstRunProc(void);
static void CheckInitTerminal(void);
static void GetPosCapablity(void);
static int  CheckICCCard(void);


/********************** Internal variables declaration *********************/
ST_MSG_INFO 	glstMsgInfo;	// 事件消息
/********************** external reference declaration *********************/

const APPINFO AppInfo =
{
	EDCAPP_AID,
	EDC_VER_STRING,
	"NEW POS TECHNOLOGY",
	"20111008171123",
	"APPLICATION PROGRAM",
	0x01
};

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/
int    imif_fd = -1;


int contactness_poweredon(int ifd)
{
	int status = 0;
	int lasterror;

	emv_contactless_get_lasterror(ifd, &lasterror);

	if (NFC_ERR_ENODATA == lasterror)
		return MIF_RET_NOCARD;
	else if (NFC_ERR_EMULTIHOP == lasterror)
		return MIF_RET_MULTICARD;
	else if (NFC_ERR_NONE == lasterror)
	{
		emv_contactless_obtain_status(ifd, &status);
		if (NFC_OP_EXCHANGE_APDU == status)
		{
			return MIF_RET_SUCCESS;
		}
		else
		{
			return MIF_RET_NOCARD;
		}
	}
	return MIF_RET_NOCARD;
}

int contactness_close(int icc_fd)
{
	int  iRet;

	if (icc_fd < 0)
	{
		return 0;
	}

	iRet = mif_close(icc_fd);
	return iRet;
}

int contactness_exchangeapdu(int icc_fd, uint32_t sendlen, const void *senddata, uint32_t *recvlen, void *recvdata)
{
	return emv_contactless_exchange_apdu(icc_fd, sendlen, senddata, recvlen, recvdata);
}

int contactness_open(void)
{
	return mif_open("/dev/nfc");
}

int contactness_poll(int icc_fd)
{
	return emv_contactless_active_picc(icc_fd);
}

int contactness_poweroff(int fd)
{
	return emv_contactless_deactive_picc(fd);
}


int main(int argc, char *argv[])
{
	int       IfRedraw;
	int   ibacklight_set_time_bak;
	int	iLength;
	int	iRet;
	char	    szUID[32];
	char  sTempBuff[64];
	// 程序开始必须进行初始化directfb--如果程序已经在运行就退出
	IfRedraw = BootInit(argc,argv);
	if (IfRedraw == 1)
	{
		exit(0);
	}
	lcdCls();
	PubDisplayTitle(TRUE,"NEW8210 APP");
	lcdFlip();

	GetPosCapablity();  // 获取终端配置信息
	memset(&glstMsgInfo, 0, sizeof(ST_MSG_INFO));
	glstMsgInfo.iMessage = APPINIT_EVENT;
	ProcAppEvent();  // 开机需初始化APP,加载参数等
//	gstSysParam.uiAppStatus |= LOGON_STATUS;
//	SaveAppTransParam();

#ifdef OFFLINE_TEST_VERSION
	memcpy(sTempBuff, "\x11\x11\x11\x11\x11\x11\x11\x11\x44\x44\x44\x44\x44\x44\x44\x44", 16);
	iRet = PedWritePinKey(PARITY_NONE,MASTER_KEY_ID,PIN_KEY_ID,16, (UINT8 *)sTempBuff);

	memcpy(sTempBuff, "\x11\x11\x11\x11\x11\x11\x11\x11\x22\x22\x22\x22\x22\x22\x22\x22", 16);
	iRet = PedWriteMacKey(PARITY_NONE,MASTER_KEY_ID,MAC_KEY_ID,16, (UINT8 *)sTempBuff);

	memcpy(sTempBuff, "\x11\x11\x11\x11\x11\x11\x11\x11\x33\x33\x33\x33\x33\x33\x33\x33", 16);
	iRet = PedWriteMacKey(PARITY_NONE,MASTER_KEY_ID,DATA_KEY_ID,16, (UINT8 *)sTempBuff);
#endif


	IfRedraw = TRUE;
	while( 1 )
	{
		IfRedraw = DispLayIdleUI(IfRedraw);
//射频卡
#ifdef 	_POS_TYPE_6210
		if(0)
		{
			led_set_brightness("led_red", 255);
			iRet=emv_test();
			led_set_brightness("led_red", 0);
			 if(iRet==OK)
			 {
			 	InitTransInfo();
				PubBeepOk();
			 	gstTransInfo.stTranLog.ucTranType=SALE;
				DisplayTransName();
			 	iRet = GetAmount();
				if( iRet!=0 )
				{
					continue;
				}
				SetCommReqField();
				sprintf((char *)gstSendPack.szPan,	   "%.*s", LEN_PAN,   (char *)gstTransInfo.stTranLog.szPan);
				iLength=strlen(gstTransInfo.stTranLog.szHolderName);
				memcpy( &gstSendPack.sICCData2[2],gstTransInfo.stTranLog.szHolderName,iLength);
				PubLong2Char((ulong)iLength, 2, gstSendPack.sICCData2);

				TranProcess();
				IfRedraw = TRUE;
				continue;
			 }
			 IfRedraw = TRUE;
		}
#else
//新接口,EMV卡
		if(1)
		{
			if(imif_fd == -1)
			{
				contactness_close(imif_fd);
				imif_fd = -1;
				imif_fd = contactness_open();
				contactness_poll(imif_fd);
			}

			iRet = contactness_poweredon(imif_fd);
			if(iRet==OK)
			 {
			 	contactness_close(imif_fd);
				imif_fd = -1;
			 	PubBeepOk();
				InitTransInfo();
			 	gstTransInfo.stTranLog.ucTranType=SALE;
				gstTransInfo.stTranLog.ucEntryMode =MODE_CONTLESS_INPUT;
				sprintf((char *)gstTransInfo.stTranLog.szPan,"1111111111111111");
				sprintf((char *)gstTransInfo.stTranLog.szExpDate,"5012");
				DisplayTransName();
				ValidCard();
				DisplayTransName();
			 	iRet = GetAmount();
				if( iRet!=0 )
				{
					continue;
				}
				DisplayTransName();
//				SetCommReqField();
//				sprintf((char *)gstSendPack.szPan,	   "%.*s", LEN_PAN,   "1234567890123456789");
				iRet=TranProcess();
				CommOnHook(FALSE);

//				DisplayResult(iRet);

//				CheckInitTerminal();
				IfRedraw = TRUE;
				continue;
			 }
			 IfRedraw = TRUE;
		}
//非接，旧接口，能判断Type B 和Type A 卡.
		if (0)
		{
			mifOpen();
			iRet = ChkContactlessCardInsert();
			if (iRet == MIF_RET_SUCCESS)
			{
				InitTransInfo();
			 	gstTransInfo.stTranLog.ucTranType=SALE;
				gstTransInfo.stTranLog.ucEntryMode =MODE_CONTLESS_INPUT;
				sprintf((char *)gstTransInfo.stTranLog.szPan,"1111111111111111");
				sprintf((char *)gstTransInfo.stTranLog.szExpDate,"5012");
				DisplayTransName();
				ValidCard();
//				iRet=ContactlessCardProc();
			}

			 mifClose();

			 if(iRet==OK)
			 {
			 	PubBeepOk();
			 	iRet = GetAmount();
				if( iRet!=0 )
				{
					continue;
				}
				lcdClrLine(2,11);
				lcdDisplay(0,2,1|DISP_MEDIACY,"Contactless Card Info");
				memset(szUID,0,sizeof(szUID));
				PubBcd2Asc(glstMsgInfo.stMagInfo.szTrack1,glstMsgInfo.stMagInfo.iRetCode,szUID);
				lcdDisplay(0,4,1|DISP_MEDIACY,"UID:%s",szUID);
				lcdFlip();
				kbGetKey();
				SetCommReqField();
				sprintf((char *)gstSendPack.szPan,	   "%.*s", LEN_PAN,   (char *)gstTransInfo.stTranLog.szPan);
				iLength=strlen((char *)gstTransInfo.stTranLog.szHolderName);
				memcpy( &gstSendPack.sICCData2[2],gstTransInfo.stTranLog.szHolderName,iLength);
				PubLong2Char((ulong)iLength, 2, gstSendPack.sICCData2);

				TranProcess();
				CommOnHook(FALSE);  //
				IfRedraw = TRUE;
				continue;
			 }
			 IfRedraw = TRUE;
		}
#endif
		if( PubChkKeyInput() )
		{
			contactness_close(imif_fd);
			imif_fd = -1;
			memset(&glstMsgInfo, 0, sizeof(ST_MSG_INFO));
			glstMsgInfo.iMessage  = KEYINPUT_EVENT;
			glstMsgInfo.wParam = kbGetKey();
			if(glstMsgInfo.wParam==KEY_CANCEL)
			{
				lcdClrLine(2,11);
				lcdDisplay(0, 3, DISP_CFONT|DISP_MEDIACY, "Exit ?");
				PubDispString("[CANCEL]  [ENTER]", DISP_LINE_CENTER|6);
				if(PubYesNo(USER_OPER_TIMEOUT))
				{
					exit(0);
				}
				else
				{
					IfRedraw = TRUE;
					continue;
				}
			}
			ProcAppEvent();
			IfRedraw = TRUE;
			continue;
		}

		if( magSwiped()==YES )
		{
			contactness_close(imif_fd);
			imif_fd = -1;
			sysGetbacklighttime(&ibacklight_set_time_bak);
			sysSetbacklighttime(-1); // 常亮
			sysSetbacklighttime(ibacklight_set_time_bak); //

			memset(&glstMsgInfo, 0, sizeof(ST_MSG_INFO));
			glstMsgInfo.iMessage = MAGCARD_EVENT;
			glstMsgInfo.stMagInfo.iRetCode = magRead(glstMsgInfo.stMagInfo.szTrack1,
												glstMsgInfo.stMagInfo.szTrack2,
												glstMsgInfo.stMagInfo.szTrack3);
			ProcAppEvent();
			IfRedraw = TRUE;
			continue;
		}

		if( ChkIfEmvEnable() && CheckICCCard())
		{
			contactness_close(imif_fd);
			imif_fd = -1;
			sysGetbacklighttime(&ibacklight_set_time_bak);
			sysSetbacklighttime(-1); // 常亮
			sysSetbacklighttime(ibacklight_set_time_bak); //

			memset(&glstMsgInfo, 0, sizeof(ST_MSG_INFO));
			glstMsgInfo.iMessage = ICCARD_EVENT;
			ProcAppEvent();
			IfRedraw = TRUE;
			continue;
		}
	}

	return 0;
}

// EDC  事件处理函数
int ProcAppEvent()
{
	INT32	iRet;

	CheckTermSatus();

	if (!PubCheckPosPower())  // 检测电量
	{
		return 0;
	}

	// version 06后,不需要自动签到和结算
/*	if (glstMsgInfo.iMessage != APPINIT_EVENT && glstMsgInfo.wParam != KEY_FN &&
		glstMsgInfo.wParam != KEY_DOWN && glstMsgInfo.wParam != KEY_UP )
	{
		iRet = CheckOpereateLogon();
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}

// 		iRet = CheckAutoSettlement();
// 		if (iRet != 0)
// 		{
// 			return ERR_NO_DISP;
// 		}
	}*/

	iRet = ERR_NO_DISP;
	switch( glstMsgInfo.iMessage )
	{
	case APPINIT_EVENT:
		ProcAppInitEvent();
		break;

	case MAGCARD_EVENT:
		iRet = ProcMagEvent();
		break;

	case ICCARD_EVENT:
		iRet = ProcICCEvent();
		PromptRemoveICC();
	    break;

	case KEYINPUT_EVENT:
		iRet = ProcKeyEvent();
		break;

	default:
	    break;
	}
	if( iRet!=0 )
	{
		DisplayResult(iRet);
	}

	CheckInitTerminal();
//	UnLockTerminal();

//	CommOnHook(FALSE);  //
	return 0;
}

// 应用初始化事件
int ProcAppInitEvent(void)
{
	int		iCnt, iRet,iWireInit;
	char	szEngTime[16+1];
	unsigned char	ucAcqIndex, ucTotalNum;
//	ST_ACQUIRER_INFO *pstAcq;

	GetCurTime(szEngTime);
	PubDisplayTitle(TRUE, szEngTime);
	Display2StringInRect("系统初始化...", "SYSTEM INIT...");
	lcdFlip();

	InitSysAllPrnFont(); // 初始化字库

	iWireInit = iCnt = 0;
	// this is the first time called by manager
/*	for(iCnt = 0; iCnt < gstSysParam.uiAcqNum;iCnt++)
	{
		pstAcq = &gstSysParam.stAcqList[iCnt];
		if ( (iWireInit!=1 ) && ((gstPosCapability.uiCommType&CT_GPRS) || (gstPosCapability.uiCommType&CT_CDMA))
			&& (pstAcq->stTxnCommCfg.ucCommType == CT_GPRS || pstAcq->stTxnCommCfg.ucCommType == CT_CDMA))
		{
			iWireInit = 1;
			GetCurTime(szEngTime);
			PubDisplayTitle(TRUE, szEngTime);
			Display2StringInRect("系统初始化...", "INIT WIRELESS...");

			iRet = CommInitModule(&pstAcq->stTxnCommCfg);
			if( iRet==0 )
			{
				SetCurAcq(iCnt);
				PreDial();
				break;
			}
			else
			{
				lcdClrLine(2,7);
				iRet = iRet & MASK_ERR_CODE;
				DispCommErrMsg(iRet);
				PubLongBeep();
				PubWaitKey(5);
				break;
			}
		}
	}*/
	iWireInit = 0;
	while(iWireInit<=2)
	{
		SetCurAcq(0);
		if (gstCurAcq.stTxnCommCfg.ucCommType==CT_MODEM)
		{
			break;
		}
		//iRet = CommInitModule(&gstCurAcq.stTxnCommCfg);
		iRet = 	ThreadInitModule();
		if (iRet == 0)
		{
			break;
		}

		iWireInit++;
		sysDelayMs(500); // 延时再检查一次

/*	if( iRet!=0 )
	{
		lcdClrLine(2,7);
		iRet = iRet & MASK_ERR_CODE;
		DispCommErrMsg(iRet);
	}*/
	}

	GetCurTime(szEngTime);
	PubDisplayTitle(TRUE, szEngTime);
	Display2StringInRect("系统初始化...", "SYSTEM INIT...");
	lcdFlip();

	if( ChkIfEmvEnable() )
	{
		sprintf((char *)glstMsgInfo.stMagInfo.szTrack1, "%s", "EMV");
		InitEMVCore();
		LoadTestKeys();
		LoadTestApps();
		LoadAllAidParam();
		LoadAllCapkParam();
		ClearExpireCAPK();
		iccSetPara(ICC_USER,ICC_VOL_5V,ICC_EMV);
	}

	// erase transaction log of last settlement
	ucTotalNum =0;
	for(ucAcqIndex=0; ucAcqIndex<gstSysParam.uiAcqNum; ucAcqIndex++)
	{
		if( gstSysCtrlParam.sAcqStatus[ucAcqIndex]==S_CLR_LOG )
		{
			ClearRecord(gstSysParam.stAcqList[ucAcqIndex].ucKey);
			ucTotalNum++;
		}
	}
	if (ucTotalNum>=gstSysParam.uiAcqNum)
	{
		ClearElecSignaturefile();
		fileMakeDir(STR_ELECSIGNATURE_FILE_PATH);  // 创建用于保存电子签名图片的目录,没有就创建
	}

	return 0;
}

// 处理磁卡刷卡事件
int ProcMagEvent(void)
{
	int		iRet;

	gstTransInfo.stTranLog.ucTranType = SALE_OR_AUTH;
	iRet = SwipeCardProc(TRUE);
	if( iRet!=0 )
	{
		return ERR_NO_DISP;
	}

	iRet = ValidCard();
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = TransCapture();
	CommOnHook(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	return 0;
}

// 处理IC卡插卡事件
int ProcICCEvent(void)
{
	int		iRet;

	gIsFirstSelectApp = 0;
/*	iRet = InitEMVCore();
	if (iRet!= EMV_RET_SUCCESS)
	{
		lcdClrLine(2,7);
		Display2Strings("EMV初始化失败","INIT EMV FAILED!");
		PubWaitKey(5);
		return ERR_NO_DISP;
	} */

	gstTransInfo.stTranLog.ucTranType = SALE;

	iRet = GetCardInfo(SKIP_DETECT_ICC|CARD_INSERTED);
	if( iRet!=0 )
	{
		return iRet;
	}

	iRet = TransCapture();
	CommOnHook(FALSE);
	if( iRet!=0 )
	{
		return iRet;
	}

	return 0;
}


// 处理普通按键事件
int ProcKeyEvent(void)
{
	int		iRet, iFuncNo;
	int     iKeyValue, iFlag = 0;
 //   UINT32 dwStart, dwEnd;

	iKeyValue = glstMsgInfo.wParam;
	iRet = ERR_NO_DISP;
	switch( iKeyValue )
	{
	case KEY_FN:
		iFuncNo = FunctionInput();
		if( iFuncNo>=0 )
		{
			FunctionExe(FALSE, iFuncNo);
		}
		break;

	case KEY_UP:
		FunctionEngMenu();
		break;

	case KEY_DOWN:
		FunctionChnMenu();
		break;

	case KEY_MENU:
	case KEY_ENTER:
		iRet = TransOther();
		PromptRemoveICC();
		break;

	case KEY1:
		iRet = TransVoid();
        iFlag = 1;
		break;

	case KEY2:
		iRet = TransSettle();
        iFlag = 1;
		break;

	case KEY3:
		PrnLastTrans();
		break;

	case KEY4:
		RePrnSpecTrans();
		break;

	case KEY5:
		iRet = TransOffSale();
        iFlag = 1;
		break;

	case KEY6:
		iRet = TransAuth(PREAUTH);
        iFlag = 1;
		break;

	case KEY7:
		iRet = TransSaleManualEntry();
        iFlag = 1;
		break;

	case KEY8:
		iRet = TransLogon();
        iFlag = 1;
		break;

	case KEY9:
		iRet = SetExtLineNo();
		SaveAppTransParam();  // 保存
		break;

	default:
		break;
	}
//    dwStart = sysGetTimerCount();
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
        iFlag = 0;
	}
//    dwEnd = sysGetTimerCount();
//    lcdCls();lcdDisplay(0,0,1,"time=%dms,", dwEnd-dwStart);kbGetKey();
	if( iRet!=0 )
	{
		return iRet;
	}

	return 0;
}


// 读取配置信息、检查终端参数/状态(如,是否下载参数、是否锁定等)、
// 恢复冲正标志等
void CheckTermSatus(void)
{
	AppFirstRunProc();

	CheckInitTerminal();
//	UnLockTerminal();

	RecoverTranLog();	// must called after system initialization
	InitTransInfo();
}

// 第一次运行时候处理
void AppFirstRunProc(void)
{
	char	szCurTime[16+1];
	static  int  iFirstFlag = 1;

	if( ExistAppTransParam())
	{
		LoadAppTransParam();
		LoadAppCtrlParam();

		if (gstSysParam.uiAppStatus == INIT_STATUS)
		{
			GetCurTime(szCurTime);
			PubDisplayTitle(TRUE, szCurTime);
			Display2StringInRect("系统初始化...", "SYSTEM INIT...");
			lcdFlip();

			if (ExistParamFile())
			{
				if (!ExistCtrlParam())
					LoadCtrlDefault();
				else
					LoadAppCtrlParam();

				ResetAllPara(TRUE);
				memset(&gstEmvStatus, 0, sizeof(EMV_STATUS));
				SaveEmvStatus();
				GetParamFile();
				RemoveParamFile();
				AdjustParam();
				SaveAppTransParam();
			}
		}

		// set default EMV library parameters
		if (ChkIfEmvEnable() && iFirstFlag)
		{
			InitEMVCore();
			LoadTestKeys();
			LoadTestApps();
			LoadAllAidParam();
			LoadAllCapkParam();
			ClearExpireCAPK();
			iFirstFlag = 0;
		}

		if (ExistParamFile())
			RemoveParamFile();

		fileMakeDir(STR_ELECSIGNATURE_FILE_PATH);  // 创建用于保存电子签名图片的目录,没有就创建

		return;
	}

	GetCurTime(szCurTime);
	PubDisplayTitle(TRUE, szCurTime);
	Display2StringInRect("系统初始化...", "SYSTEM INIT...");
	lcdFlip();

	// set default values
	if (ExistParamFile())
	{
		if (!ExistCtrlParam())
			LoadCtrlDefault();
		else
			LoadAppCtrlParam();

		ResetAllPara(TRUE);
		memset(&gstEmvStatus, 0, sizeof(EMV_STATUS));
		SaveEmvStatus();
		GetParamFile();
		RemoveParamFile();
		AdjustParam();
		SaveAppTransParam();
	}
	else
	{
		LoadEdcDefault();
	}

	InitTranLogFile();

	// set default EMV library parameters
	if (ChkIfEmvEnable())
	{
		InitEMVCore();
		LoadTestKeys();
		LoadTestApps();
		LoadAllAidParam();
		LoadAllCapkParam();
		ClearExpireCAPK();
	}

	fileMakeDir(STR_ELECSIGNATURE_FILE_PATH);  // 创建用于保存电子签名图片的目录,没有就创建
}


void CheckInitTerminal(void)
{
	char	szCurTime[16+1];

	if( !(gstSysParam.uiAppStatus & INIT_STATUS) )
	{
		return;
	}

	sysTimerSet(0, 0);
	while( gstSysParam.uiAppStatus & INIT_STATUS )
	{
		if( sysTimerCheck(0)==0 )
		{
			GetCurTime(szCurTime);
			PubDisplayTitle(TRUE, szCurTime);
			sysTimerSet(0, 60000);
			Display2StringInRect("请下装参数", "PLEASE INIT");
		}

		if( PubWaitKey(0)==KEY_FN )
		{
			InitTransInfo();
			FunctionInit();
			sysTimerSet(0, 0);
		}
	}
}


void GetPosCapablity(void)
{
	char    szPosConfigInfo[40];
	int     iRet;

	memset(szPosConfigInfo,0,sizeof(szPosConfigInfo));
	iRet = sysReadConfig((uint8_t *)szPosConfigInfo);
	if(iRet<=0)
	{
		return;
	}

	memset((char*)&gstPosCapability,0,sizeof(gstPosCapability));
	gstPosCapability.uiPosType = szPosConfigInfo[0];
	gstPosCapability.uiPrnType = szPosConfigInfo[1];
	gstPosCapability.uiSecuType = szPosConfigInfo[5];
	gstPosCapability.uiCommType = CT_RS232;
//	gstPosCapability.uiCommType |= CT_MODEM|CT_TCPIP|CT_GPRS|CT_WIFI|CT_CDMA;
	if (szPosConfigInfo[2]!=0)
	{
		gstPosCapability.uiCommType |= CT_MODEM;
	}

	if (szPosConfigInfo[8]!=0)
	{
		gstPosCapability.uiCommType |= CT_TCPIP;
	}

	if (szPosConfigInfo[9]!=0)
	{
		gstPosCapability.uiCommType |= CT_GPRS;
	}

	if (szPosConfigInfo[10]!=0)
	{
		gstPosCapability.uiCommType |= CT_CDMA;
	}

	if (szPosConfigInfo[11]!=0)
	{
		gstPosCapability.uiCommType |= CT_WIFI;
	}

	switch (szPosConfigInfo[3])
	{
	case 1:		gstPosCapability.uiMaxSyncPSTNRate = 1200;		break;
	case 2:		gstPosCapability.uiMaxSyncPSTNRate = 2400;		break;
	case 3:		gstPosCapability.uiMaxSyncPSTNRate = 9600;		break;
	case 4:		gstPosCapability.uiMaxSyncPSTNRate = 14400;		break;
	default:		break;
	}

	switch (szPosConfigInfo[4])
	{
	case 1:		gstPosCapability.uiMaxAsynPSTNRate = 1200;		break;
	case 2:		gstPosCapability.uiMaxAsynPSTNRate = 2400;		break;
	case 3:		gstPosCapability.uiMaxAsynPSTNRate = 4800;		break;
	case 4:		gstPosCapability.uiMaxAsynPSTNRate = 7200;		break;
	case 5:		gstPosCapability.uiMaxAsynPSTNRate = 9600;		break;
	case 6:		gstPosCapability.uiMaxAsynPSTNRate = 12000;		break;
	case 7:		gstPosCapability.uiMaxAsynPSTNRate = 14400;		break;
	case 8:		gstPosCapability.uiMaxAsynPSTNRate = 19200;		break;
	case 9:		gstPosCapability.uiMaxAsynPSTNRate = 24000;		break;
	case 10:	gstPosCapability.uiMaxAsynPSTNRate = 26400;		break;
	case 11:	gstPosCapability.uiMaxAsynPSTNRate = 28800;		break;
	case 12:	gstPosCapability.uiMaxAsynPSTNRate = 31200;		break;
	case 13:	gstPosCapability.uiMaxAsynPSTNRate = 33600;		break;
	case 14:	gstPosCapability.uiMaxAsynPSTNRate = 48000;		break;
	case 15:	gstPosCapability.uiMaxAsynPSTNRate = 56000;		break;
	default:
		break;
	}

	gstPosCapability.uiUnContactlessCard = szPosConfigInfo[12];
	gstPosCapability.uipostsScreen = szPosConfigInfo[13];

	if (gstPosCapability.uiPosType == 0x02) // 如果是6110
	{
		memset(szPosConfigInfo,0,sizeof(szPosConfigInfo));
		sysReadVer((uint8_t *)szPosConfigInfo);
		if (!(szPosConfigInfo[6] & 0x30))
		{
			gstPosCapability.uiPrnType = 0;
		}
	}

}

int  CheckICCCard(void)
{
	int iRet,iCnt;

	iRet = iccDetect(ICC_USER);
	if (iRet!=ICC_SUCCESS)
	{
		return 0;
	}

	iCnt = 5;
	while(iCnt--)
	{
		sysDelayMs(10);
		if (iccDetect(ICC_USER)!=ICC_SUCCESS)
		{
			return 0;
		}
	}

	return 1;
}


int  CheckOpereateLogon(void)
{
	int  iRet ;

	if (gstSysParam.uiAppStatus & LOGON_STATUS)
	{
/*		PubDisplayTitle(TRUE,"Logon");
		iRet = CheckTermPWD();
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}*/

		PubDisplayTitle(TRUE,"Logon");
		Display2Strings("请先签到", "PLS LOGON");
		if(!PubYesNo(USER_OPER_TIMEOUT))
		{
			return ERR_NO_DISP;
		}

		PubDisplayTitle(TRUE,"Logon");
		iRet = TransLogon();
		if (gstSysParam.stAcqList[0].stTxnCommCfg.ucCommType == CT_MODEM)
		{
			CommOnHook(TRUE);
		}
		else
		{
			CommOnHook(FALSE);
		}
		if (iRet != 0)
		{
			return ERR_NO_DISP;
		}
	}

	return 0;
}


int  CheckAutoSettlement(void)
{
	int  iRet, iNeedSettlement;
	char  szDateTime[20];

	if (!GetTranLogNum(ACQ_ALL))
	{// 无交易不需要结算
		return 0;
	}

	memset(szDateTime,0,sizeof(szDateTime));
	GetDateTime((uint8_t *)szDateTime);

	iNeedSettlement = FALSE;
	if (memcmp(szDateTime,gstSysCtrlParam.szLastSettlementDate,8) != 0)
	{// 隔日需要结算
		iNeedSettlement = TRUE;
	}
	else
	{ // 当天有结算过，再判断结算的时间是否大于自动结算时间,如果是在自动结算时间前也要提示结算,之后则不提示
		if (memcmp(gstSysCtrlParam.szLastSettlementDate+8,gstSysParam.szAutoSettlementTime,4)<0
			&& memcmp(szDateTime+8,gstSysParam.szAutoSettlementTime,4) >= 0)
		{
			iNeedSettlement = TRUE;
		}
	}

	if (iNeedSettlement)
	{
		PubDisplayTitle(TRUE, "AUTO SETTLEMENT ");
		Display2Strings("请先结账", "PLS SETTLE BATCH");
		PubBeepErr();
		PubWaitKey(5);

		iRet = TransSettle();
		CommOnHook(FALSE);
		if (iRet != 0)
		{
			return iRet;
		}
	}

	return 0;
}
//射频卡
int     ChkContactlessCardInsert(void)
{
	int iRet;
	char      szbuff[300];
	char	    szUID[32];
	memset(&glstMsgInfo, 0, sizeof(ST_MSG_INFO));
	InRelease(); //12.14 add
/*
	iRet = InListPassiveTarget(0,0,NULL,&glstMsgInfo.stMagInfo.iRetCode,glstMsgInfo.stMagInfo.szTrack1);
	if (iRet == MIF_RET_SUCCESS)
	{
		if (MIF_M1_S50 != glstMsgInfo.stMagInfo.szTrack1[0])
		// 激活卡片内已处于ACTIVE态的T=CL卡片，并使之进入ISO14443-4协议态
			iRet = InATR(&glstMsgInfo.stMagInfo.iRetCode,glstMsgInfo.stMagInfo.szTrack1);
	}
*/

	memset(szbuff,0,sizeof(szbuff));
	memset(szUID,0,sizeof(szUID));
	iRet = InListPassiveTarget(0,0,szbuff,&glstMsgInfo.stMagInfo.iRetCode,glstMsgInfo.stMagInfo.szTrack1);
	if (iRet == MIF_RET_SUCCESS)
	{
		glstMsgInfo.stMagInfo.iRetCode = glstMsgInfo.stMagInfo.szTrack1[4];
		memcpy(glstMsgInfo.stMagInfo.szTrack1,&glstMsgInfo.stMagInfo.szTrack1[5],glstMsgInfo.stMagInfo.iRetCode);
		glstMsgInfo.stMagInfo.szTrack1[glstMsgInfo.stMagInfo.iRetCode] = 0;
		PubBcd2Asc(glstMsgInfo.stMagInfo.szTrack1,glstMsgInfo.stMagInfo.iRetCode,szUID);
		printf("895=len=%d,%s\n",glstMsgInfo.stMagInfo.iRetCode,szUID);
		return OK;
	}

	memset(szbuff,0,sizeof(szbuff));
	memset(szUID,0,sizeof(szUID));
	iRet = InListPassiveTarget(3,1,szbuff,&glstMsgInfo.stMagInfo.iRetCode,glstMsgInfo.stMagInfo.szTrack1);
	if (iRet == MIF_RET_SUCCESS)
	{
		glstMsgInfo.stMagInfo.iRetCode = 4;
		memcpy(glstMsgInfo.stMagInfo.szTrack1,&glstMsgInfo.stMagInfo.szTrack1[2],4);
		glstMsgInfo.stMagInfo.szTrack1[4] = 0;
		PubBcd2Asc(glstMsgInfo.stMagInfo.szTrack1,glstMsgInfo.stMagInfo.iRetCode,szUID);
		printf("906=len=%d,%s\n",glstMsgInfo.stMagInfo.iRetCode,szUID);
		return OK;
	}

	sysDelayMs(50);
	memset(szbuff,0,sizeof(szbuff));
	memset(szUID,0,sizeof(szUID));
	memcpy(szbuff,"\x00\xFF\xFF\x01\x00",5);
	memset((char*)&glstMsgInfo, 0, sizeof(glstMsgInfo));
	iRet = InListPassiveTarget(1,5,szbuff,&glstMsgInfo.stMagInfo.iRetCode,glstMsgInfo.stMagInfo.szTrack1);
	if (iRet == MIF_RET_SUCCESS)
	{
		glstMsgInfo.stMagInfo.iRetCode = glstMsgInfo.stMagInfo.szTrack1[1];
		memcpy(glstMsgInfo.stMagInfo.szTrack1,&glstMsgInfo.stMagInfo.szTrack1[3],glstMsgInfo.stMagInfo.iRetCode);
		glstMsgInfo.stMagInfo.szTrack1[glstMsgInfo.stMagInfo.iRetCode] = 0;
		PubBcd2Asc(glstMsgInfo.stMagInfo.szTrack1,glstMsgInfo.stMagInfo.iRetCode,szUID);
		printf("920len=%d,=%s\n",glstMsgInfo.stMagInfo.iRetCode,szUID);
		return OK;
	}
//	mifResetRF(0);

	return -1;
}
INT32  ContactlessCardProc(void)
{
	int  iRet;
	gstTransInfo.stTranLog.ucTranType = SALE;

	lcdCls();

	DisplayTransName();
//	DisplayReadICC();

 	iRet = GetAllDataFromM1Card();
	return iRet;

}

int    GetAllDataFromM1Card(void)
{
	int		iRet;

	uint8_t  Data[30];

//	for(iCnt=0;iCnt<50;iCnt++)
//	{

		sysDelayMs(50);

		iRet = ChkContactlessCardInsert();
		printf("1511=%d\n",iRet);

		if (iRet != MIF_RET_SUCCESS)
			return ERR_NO_DISP;

// 获取扇区0的数据

	iRet = GetOneSectorData(0,KEYA_SECTOR0,(char*)Data);
	if (iRet != 0)
	{
		return ERR_NO_DISP;
	}

// 获取扇区1的数据

/*	iRet = GetOneSectorData(1,KEYA_SECTOR0,(char*)Data);
	if (iRet != 0)
	{
		continue;
	}
*/

// 获取扇区2的数据
		iRet = GetOneSectorData(2,KEYA_SECTOR0,(char*)Data);
		if (iRet != 0)
		{
			printf("1571iRet=%d\n",iRet);
			return ERR_NO_DISP;
		}

//	}
	return 0;
}



int    GetOneSectorData(int iSectorNum, char *psKeyA, char*psUid)
{
	int		iRet;
	UINT32  iSendLen, iRecvLen, iCnt;
	UINT8   szSendData[300], szRecvData[300];
	char szCardInfo[100] = {0};
	//INT32 iCardInfolen = 0;
	char  szDispBuf[64];
	char  szDataBuf[64];
	//char szAmount[16+2];
	//int 	Amount;
	if (iSectorNum >= 16 || iSectorNum < 0 || psKeyA == NULL || psUid == NULL)
	{
		/*PubBeepErr();
		lcdClrLine(2,7);
		PubDispString("读卡参数错误",3|DISP_LINE_CENTER);
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"请 重 试");
		PubWaitKey(5);*/
		return ERR_NO_DISP;
	}
	memset(szCardInfo, 0, sizeof(szCardInfo));
/*	iRet = InListPassiveTarget(0, 0, NULL, &iCardInfolen, szCardInfo);
	if(iRet != OK)
	{
		printf("1433=%d\n",iRet);
		//fortest
		lcdClrLine(2, 7);
		lcdDisplay(0, 2, DISP_CFONT, "InListPassiveTarget 0 iRet = %d", iRet);
		kbGetKey();
		return iRet;
	}
*/
	iSendLen = iRecvLen = 0;
	memset(szSendData, 0x00, sizeof(szSendData));
	memset(szRecvData, 0x00, sizeof(szRecvData));
	// 发送命令认证命令
	szSendData[0] = 0x60;
	szSendData[1] = iSectorNum*4;
	memcpy(szSendData+2,psKeyA,6);
	memcpy(szSendData+8, glstMsgInfo.stMagInfo.szTrack1+5,4);
//	memcpy(szSendData+8, szCardInfo+5,4);
	iSendLen = 12;
	iRet = InDataExchange(iSendLen,szSendData,&iRecvLen,szRecvData);
	if (iRet != 0)
	{
		PubBeepErr();
		lcdClrLine(2,7);
		PubDispString("读卡失败",3|DISP_LINE_CENTER);
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"认证第%d块失败",(iSectorNum));
		lcdDisplay(0,7,DISP_ASCII|DISP_MEDIACY,"iRet=%d",iRet);
		PubWaitKey(5);
		PubWaitKey(5);
		return ERR_NO_DISP;
	}
	for (iCnt=0; iCnt<=3; iCnt++) // 读取前面3块，密钥数据块不读
	{
		iSendLen = iRecvLen = 0;
		memset(szSendData, 0x00, sizeof(szSendData));
		memset(szRecvData, 0x00, sizeof(szRecvData));
		// 读取N块数据
		szSendData[0] = 0x30;
		szSendData[1] = iSectorNum*4 + iCnt;
		iSendLen = 2;
		iRet = InDataExchange(iSendLen,szSendData,&iRecvLen,szRecvData);
		if (iRet != 0)
		{
			/*PubBeepErr();
			lcdClrLine(2,7);
			PubDispString("读卡失败",3|DISP_LINE_CENTER);
			lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"读第%d块数据失败",(iSectorNum*4 + iCnt));
			PubWaitKey(5);*/
			return ERR_NO_DISP;
		}
		memset(szDispBuf,0,sizeof(szDispBuf));
		memset(szDataBuf,0,sizeof(szDataBuf));
		PubBcd2Asc((char *)szRecvData, 16, szDispBuf);
		PubBcd2Asc(szDispBuf, 32, szDataBuf);
//Card no
		if(iSectorNum*4 + iCnt==1)
		{
			memset(gstTransInfo.stTranLog.szPan,0,sizeof(gstTransInfo.stTranLog.szPan));
			memcpy(gstTransInfo.stTranLog.szPan,szDispBuf,16);
			printf("%s\n",gstTransInfo.stTranLog.szPan);
		}
//Cardholdername
		if(iSectorNum*4 + iCnt==2)
			sprintf((char *)gstTransInfo.stTranLog.szHolderName,(char *)szRecvData);
//Amount
/*		if(iSectorNum*4 + iCnt==10)
		{
			sprintf(szAmount,"%",szDispBuf);
			Hex2Integer(szDispBuf,Amount);
		}
*/
		printf("%d\n",iSectorNum*4 + iCnt);
		printf("0:%s\n",szRecvData);
		printf("1:%s\n",szDispBuf);
		printf("2:%s\n\n",szDataBuf);
	}
	return 0;
}

static void dump_apdu(const char *prompt, uint32_t len, const void *apdu)
{
	const uint8_t	*papdu = (typeof(papdu))apdu;
	int		i;

	printf("%s", prompt);

	for (i=0; i<len; i++)
		printf("%s%02x", i==0 ? "" : " ", *papdu++);

	printf("\n");
}

static char *status_to_string(int status)
{
	 char *str;

	switch (status) {
		case NFC_OP_NULL:			str = "NULL";		break;
		case NFC_OP_POLL:			str = "POLL";		break;
		case NFC_OP_ANTICOLL:		str = "ANTICOLL";	break;
		case NFC_OP_REMOVE:			str = "REMOVE";		break;
		case NFC_OP_EXCHANGE_APDU:	str = "EXCHANGE";	break;
		default:					str = "UNKNOWN";	break;
	}

	return str;
}


int emv_test(void)
{
	uint8_t 	txbuf[256], rxbuf[256];
	uint32_t	txlen, rxlen;
	int 		fd, status, retval, error;
	int old_status,old_error;
	char *p;
	int tck = 0;
	//char UID[16+1];
	//int  length;
	//int iRet;
	int  time;
	//int lcd_fd=-1;
	//int i=0;

	printf("QPBOC LEVEL I\n");
	printf("Open: processing...\n");

	fd = mif_open("/dev/nfc");
	if (fd < 0) {
		printf("Open: failed with %d\n", fd);
		return fd;
	}

	do {
		usleep(10*1000);
		printf("QPBOC LEVEL I\n");
		printf("Polling: process...\n");

		tck++;

		if (tck > 200)
		{
			tck = 0;
			//break;
		}

#if 1
		//nfc_led_control( fd,0x0f, 1);//all open
		if (emv_contactless_active_picc(fd)) {
			printf("Polling: failed\n");
			goto exit_entry;
		}

		old_status=-1;
		old_error=-1;
//		while (1)
		for(time=0;time<10;time++)
		{
			if (emv_contactless_obtain_status(fd, &status)) {
				printf("Obtain: 'status' failed\n");
				goto exit_entry;
			}

			if (emv_contactless_get_lasterror(fd, &error)) {
				printf("Obtain: 'error' failed\n");
				goto exit_entry;
			}

			if(old_status !=status || old_error!=error)
			{
				p=status_to_string(status);
				printf("State:%s Err:%d\n",p, error);
				old_status=status;
				old_error=error;
			}
			if (status == NFC_OP_EXCHANGE_APDU)
				break;
			else
			{
				usleep(10*1000);
			}
		}
		if(status != NFC_OP_EXCHANGE_APDU)
			goto exit_entry;
		/*
		 * CLA INS P1 P2 LC ----------------DATA--------------------- LE
		 * 00  a4  04 00 0e 32 50 41 59 2e 53 59 53 2e 44 44 46 30 31 00
		 */
		txlen = 0;
		memcpy(&txbuf[txlen], "\x00\xA4\x04\x00", 4);	/* cla ins p1 p2 */
		txlen += 4;
		txbuf[txlen++] = 14;							/* lc */
		memcpy(&txbuf[txlen], "2PAY.SYS.DDF01", 14);	/* DATA */
		txlen += 14;
		txbuf[txlen++] = 0; 							/* le */

		while (1) {
			printf("Send: %d bytes\n", txlen);
			printf("Recv: \n");
			dump_apdu("\n[C-APDU] ==> ", txlen, txbuf);

			rxlen = 0;
			retval = emv_contactless_exchange_apdu(fd, txlen, txbuf, &rxlen, rxbuf);
			if (retval < 0) {
				printf("Exchange failed: %d\n", errno);
				break;
			}
			printf("Recv: %d bytes\n", rxlen);

			dump_apdu("[R-APDU] <== ", rxlen, rxbuf);

			if (rxlen < 2) {
				printf("bad receive length %d.\n", rxlen);
				break;
			}

			if (rxbuf[1] == 0x70)	{
				printf("test complete, normal quit\n");
				break;
			}

			if (rxlen < 5 || ((rxbuf[4] + 7) != rxlen && (rxbuf[4] + 8) != rxlen)) {
				printf("bad receive data\n");
				break;
			}
			/* loopback receive data */
			memcpy(txbuf, rxbuf, rxlen-2);		/* cla ins p1 p2 lc data [le] */
			txlen = rxlen-2;
		}
		//while(1)
		{
			mif_close(fd);
			printf("1178\n");
			return 0;
			printf("Please remove PICC\n");
			emv_contactless_deactive_picc(fd);
			printf("Remove:  OK\n");
			usleep(10*1000);
		}
#endif
	} while(0);

exit_entry:
	//nfc_led_control( fd,0x0f, (i++)%2);//all open
	printf("Close: processing...\n");
	mif_close(fd);
	return -1;

	//close(lcd_fd);

}




// end of file
