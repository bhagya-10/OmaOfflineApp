
#include "global.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void PubSysHalt(void);
/********************** Internal variables declaration *********************/
static  char SYSPARAM_FILE[]    = "SYSParam.dat";		// 系统配置参数文件
static  char SYSCTRL_FILE[]     = "SYSCtrl.dat";		// 系统运行控制参数文件
static  char TRANLOG_FILE[]     = "TransLog.dat";		// 交易日志文件
static  char EMV_STATUS_FILE[]  = "EMVStatus.dat";      // 最后一次EMV交易状态
static  char EMV_ERRLOG_FILE[]  = "EMVErrLog.dat";		// 错误日志文件
static  char EMV_CAPK_FILE[]    = "EMVCAPK.dat";        // CAPK文件
static  char EMV_AID_FILE[]     = "EMVAID.dat";         // AID文件
/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

// 初始化交易日志文件
int InitTranLogFile(void)
{
	int			    fd, iWriteBytes, iCnt;
	ST_TRAN_LOG	    stTranLog;
//	EMV_ERR_LOG	    stErrLog;

	fd = fileOpen((char *)TRANLOG_FILE, O_CREAT|O_RDWR);
	if( fd<0 )
	{
		PubTRACE0("InitTranLogFile");
		return ERR_FILEOPER;
	}

	memset(&stTranLog, 0, sizeof(stTranLog));
	for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
	{
		iWriteBytes = PubFWriteN(fd, &stTranLog, sizeof(stTranLog));
		if( iWriteBytes!=(int)sizeof(stTranLog) )
		{
			PubTRACE0("InitTranLogFile");
			fileClose(fd);
			return ERR_FILEOPER;
		}
	}
	fileClose(fd);

	// init error log file
	fd = fileOpen((char *)EMV_ERRLOG_FILE, O_CREAT|O_RDWR);
	if( fd<0 )
	{
		PubTRACE0("InitTranLogFile");
		return ERR_FILEOPER;
	}

// 	memset(&stErrLog, 0, sizeof(stErrLog));
// 	for(iCnt=0; iCnt<MAX_ERR_LOG; iCnt++)
// 	{
// 		iWriteBytes = PubFWriteN(fd, &stErrLog, sizeof(stErrLog));
// 		if( iWriteBytes!=(int)sizeof(stErrLog) )
// 		{
// 			PubTRACE0("InitTranLogFile");
// 			fileClose(fd);
// 			return ERR_FILEOPER;
// 		}
// 	}
	fileClose(fd);

	return 0;
}

// 读取系统参数
int LoadAppTransParam(void)
{
	int		iRet;

	iRet = PubFileRead(SYSPARAM_FILE, 0L, &gstSysParam, sizeof(ST_SYSTEM_PARAM));
	if( iRet!=0 )
	{
		PubTRACE0("LoadAppTransParam");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 保存系统参数
int SaveAppTransParam(void)
{
	int		iRet;

	iRet = PubFileWrite(SYSPARAM_FILE, 0L, &gstSysParam, sizeof(ST_SYSTEM_PARAM));
	if( iRet!=0 )
	{
		PubTRACE0("SaveAppTransParam");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 保持EDC参数
int SaveEdcParam(void)
{
	int		iRet;

	iRet = PubFileWrite(SYSPARAM_FILE,
						OFFSET(ST_SYSTEM_PARAM, stEdcInfo),
						&gstSysParam.stEdcInfo,
						sizeof(gstSysParam.stEdcInfo));
	if( iRet!=0 )
	{
		PubTRACE0("SaveAppTransParam");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 保存系统密码
int SavePassword(void)
{
	int		iRet;

	iRet = PubFileWrite(SYSPARAM_FILE,
						OFFSET(ST_SYSTEM_PARAM, szPassword),
						gstSysParam.szPassword,
						sizeof(gstSysParam.szPassword));
	if( iRet!=0 )
	{
		PubTRACE0("SavePassword");
		PubSysHalt();
		return ERR_FILEOPER;
	}
	
	return 0;
}

// 判断系统参数文件是否存在
int ExistAppTransParam(void)
{
	if (fileExist((char *)SYSPARAM_FILE)>=0)
	{
		return TRUE;
	}

	return FALSE;
}


int    ExistCtrlParam(void)
{
	if (fileExist((char *)SYSCTRL_FILE)>=0)
	{
		return TRUE;
	}
	
	return FALSE;
}

// 读取全部运行控制参数
int LoadAppCtrlParam(void)
{
	int		iRet;

	iRet = PubFileRead(SYSCTRL_FILE, 0L, &gstSysCtrlParam, sizeof(ST_SYSCONTROL_PARAM));
	if( iRet!=0 )
	{
		PubTRACE0("LoadAppCtrlParam");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 保存全部运行控制参数(谨慎使用,比较耗时)
int SaveSysCtrlAll(void)
{
	int		iRet;

	// about 60K bytes
	iRet = PubFileWrite(SYSCTRL_FILE, 0L, &gstSysCtrlParam, sizeof(ST_SYSCONTROL_PARAM));
	if( iRet!=0 )
	{
		PubTRACE0("SaveSysCtrlAll");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 保存基本运行控制参数(STAN/Invoice No/....)
int SaveSysCtrlBase(void)
{
	int		iRet;

	// about 1.5K bytes
	iRet = PubFileWrite(SYSCTRL_FILE, 0L, &gstSysCtrlParam, LEN_SYSCTRL_BASE);
	if( iRet!=0 )
	{
		PubTRACE0("SaveSysCtrlBase");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// save basic information & reversal information
int SaveSysCtrlNormal(void)
{
	int		iRet;

	// about 20K bytes
	iRet = PubFileWrite(SYSCTRL_FILE, 0L, &gstSysCtrlParam, LEN_SYSCTRL_NORMAL);
	if( iRet!=0 )
	{
		PubTRACE0("SaveSysCtrlNormal");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 保存56域(只存储当前收单行对应的)
int SaveField56(void)
{
/*	int		iRet;

	iRet = PubFileWrite(SYSCTRL_FILE,
						OFFSET(ST_SYSCONTROL_PARAM, stField56[gstCurAcq.ucIndex]),
						&gstSysCtrlParam.stField56[gstCurAcq.ucIndex],
						sizeof(ST_EMV_FIELD56));
	if( iRet!=0 )
	{
		PubTRACE0("SaveField56");
		PubSysHalt();
		return ERR_FILEOPER;
	} */
	return 0;
}

// 保存冲正信息(只存储当前收单行对应的)
int SaveRevInfo(uint8_t bSaveRevInfo)
{
	int			iRet;
	ST_ISO8583	*pstRevPack;

	if( bSaveRevInfo )	// 保存冲正信息
	{
		if( !ChkIfNeedReversal() )
		{
			return 0;
		}

		gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].bNeedReversal = TRUE;
		pstRevPack = &gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].stRevPack;
		gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].ucEntryMode = gstTransInfo.stTranLog.ucEntryMode;
		memcpy(pstRevPack, &gstSendPack, sizeof(ST_ISO8583));

		// 冲正交易不允许保存磁道信息
		memset(pstRevPack->szTrack1, 0, sizeof(pstRevPack->szTrack1));
//		memset(pstRevPack->szTrack2, 0, sizeof(pstRevPack->szTrack2));
		memset(pstRevPack->szTrack3, 0, sizeof(pstRevPack->szTrack3));
		memset(pstRevPack->sField48, 0, sizeof(pstRevPack->sField48));  // cvv/4dbc 
		sprintf(pstRevPack->szTrack2, "%.*s", LEN_TRACK2, gstTransInfo.stTranLog.szTrack2);  // 保存track2,冲正继续上传
		sprintf((char *)pstRevPack->szPan,     "%.*s", LEN_PAN,      gstTransInfo.stTranLog.szPan);
		sprintf((char *)pstRevPack->szExpDate, "%.*s", LEN_EXP_DATE, gstTransInfo.stTranLog.szExpDate);
		iRet = PubFileWrite(SYSCTRL_FILE,
							OFFSET(ST_SYSCONTROL_PARAM, stRevInfo[gstCurAcq.ucIndex]),
							&gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex],
							sizeof(ST_REVERSAL_INFO));
	}
	else	// 清除冲正信息
	{
		if( !gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].bNeedReversal )
		{
			return 0;
		}
		gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].bNeedReversal = FALSE;
		iRet = PubFileWrite(SYSCTRL_FILE,
							OFFSET(ST_SYSCONTROL_PARAM, stRevInfo[gstCurAcq.ucIndex].bNeedReversal),
							&gstSysCtrlParam.stRevInfo[gstCurAcq.ucIndex].bNeedReversal,
							1);
	}
	if( iRet!=0 )
	{
		PubTRACE0("SaveRevInfo");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// save reprint settle information
int SaveRePrnStlInfo(void)
{
	int		iRet;

	// about 40K bytes
	iRet = PubFileWrite(SYSCTRL_FILE,
						OFFSET(ST_SYSCONTROL_PARAM, stRePrnStlInfo),
						&gstSysCtrlParam.stRePrnStlInfo,
						sizeof(ST_REPRN_SETTL_INFO));
	if( iRet!=0 )
	{
		PubTRACE0("SaveStlInfo");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 读取一个交易日志
int LoadTranLog(void *pstLog, uint uiIndex)
{
	int		iRet;

	PubASSERT( gstSysCtrlParam.sAcqKeyList[uiIndex]!=INV_ACQ_KEY );
	iRet = PubFileRead(TRANLOG_FILE, (long)(sizeof(ST_TRAN_LOG)*uiIndex),
						pstLog, sizeof(ST_TRAN_LOG));
	if( iRet!=0 )
	{
		PubTRACE0("LoadTranLog");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 更新一个交易日志
int UpdateTranLog(void *pstLog, uint uiIndex)
{
	int		iRet;

	PubASSERT( gstSysCtrlParam.sAcqKeyList[uiIndex]!=INV_ACQ_KEY );
	iRet = PubFileWrite(TRANLOG_FILE, (long)(sizeof(ST_TRAN_LOG)*uiIndex),
						pstLog, sizeof(ST_TRAN_LOG));
	if( iRet!=0 )
	{
		PubTRACE0("UpdateTranLog");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// 存储交易日志算法(避免结算时候的排序操作，删除交易记录也只是修改索引表)
// 交易日志文件的存储包括: 1.存储ACQ/Issuer Key索引表 2.交易记录
// 由于系统需要操作两次文件系统，这里采用了类似于数据库的两次提交方式:
// Step 1: 记录需要记录的信息（索引表信息、交易记录等）
// Step 2: 完成两次实际的日志信息记录
// 保存一个交易日志
int SaveTranLog(void *pstLog)
{
	int		iRet, iCnt;

	for(iCnt=0; iCnt<MAX_TRANLOG; iCnt++)
	{
		if( gstSysCtrlParam.sAcqKeyList[iCnt]==INV_ACQ_KEY )
		{
			break;
		}
	}

	if( iCnt>=MAX_TRANLOG )
	{
		PubTRACE0("SaveTranLog");
		return ERR_FILEOPER;
	}

	gstSysCtrlParam.sAcqKeyList[iCnt]      = gstCurAcq.ucKey;
	gstSysCtrlParam.sIssuerKeyList[iCnt]   = gstCurIssuer.ucKey;

	gstSysCtrlParam.uiLastRecNoList[gstCurAcq.ucIndex] = (UINT32)iCnt;

	gstSysCtrlParam.stWriteInfo.bNeedSave  = TRUE;
	gstSysCtrlParam.stWriteInfo.ucAcqIndex = gstCurAcq.ucIndex;
	gstSysCtrlParam.stWriteInfo.uiRecNo    = (UINT32)iCnt;
	memcpy(&gstSysCtrlParam.stWriteInfo.stTranLog, pstLog, sizeof(ST_TRAN_LOG));

	// Step 1: 记录需要记录的信息（索引表信息、交易记录等）
	iRet = SaveSysCtrlBase();
	if( iRet<0 )
	{
		return iRet;
	}

	// Step 2: 完成两次实际的日志信息记录
	return RecoverTranLog();
}

// 恢复日志记录的不一致性
int RecoverTranLog(void)
{
	int		fd, iRet;

	if( !gstSysCtrlParam.stWriteInfo.bNeedSave )
	{
		return 0;
	}

	fd = fileOpen((char *)TRANLOG_FILE, O_RDWR|O_CREAT);
	if( fd<0 )
	{
		PubTRACE0("RecoverTranLog");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	iRet = fileSeek(fd, (long)(gstSysCtrlParam.stWriteInfo.uiRecNo*sizeof(ST_TRAN_LOG)), SEEK_SET);
	if( iRet<0 )
	{
		fileClose(fd);
		PubSysHalt();
		return ERR_FILEOPER;
	}

	iRet = PubFWriteN(fd, &gstSysCtrlParam.stWriteInfo.stTranLog, sizeof(ST_TRAN_LOG));
	fileClose(fd);
	if( iRet!=(int)sizeof(ST_TRAN_LOG) )
	{
		PubTRACE0("RecoverTranLog");
		PubSysHalt();
		return iRet;
	}

	gstCurAcq.ucIndex = gstSysCtrlParam.stWriteInfo.ucAcqIndex;
	if( !(gstSysCtrlParam.stWriteInfo.stTranLog.uiStatus & TS_NOSEND) )
	{
		SaveRevInfo(FALSE);
	}

	gstSysCtrlParam.stWriteInfo.bNeedSave = FALSE;
	gstSysCtrlParam.uiLastRecNo = gstSysCtrlParam.stWriteInfo.uiRecNo;

	return SaveSysCtrlBase();
}


// 获取交易记录总数
int GetTranLogNum(uint8_t ucAcqKey)
{
	UINT32	uiCnt, uiTranNum;

	if( ucAcqKey==ACQ_ALL )
	{
		for(uiTranNum=uiCnt=0; uiCnt<MAX_TRANLOG; uiCnt++)
		{
			if( gstSysCtrlParam.sAcqKeyList[uiCnt]!=INV_ACQ_KEY )
			{
				uiTranNum++;
			}
		}
	}
	else
	{
		for(uiTranNum=uiCnt=0; uiCnt<MAX_TRANLOG; uiCnt++)
		{
			if( gstSysCtrlParam.sAcqKeyList[uiCnt]==ucAcqKey )
			{
				uiTranNum++;
			}
		}
	}

	return uiTranNum;
}

// 检查最后一笔交易是否与当前交易重复,并进行提示
int AllowDuplicateTran(void)
{
	ST_TRAN_LOG	stTranLog;
	UINT32		uiTranNum;

	uiTranNum = GetTranLogNum(ACQ_ALL);
	if( uiTranNum==0 || gstSysCtrlParam.uiLastRecNo>=MAX_TRANLOG )
	{
		return TRUE;
	}
	if( gstSysCtrlParam.sAcqKeyList[gstSysCtrlParam.uiLastRecNo]==INV_ACQ_KEY )
	{
		return TRUE;
	}

	memset(&stTranLog, 0, sizeof(ST_TRAN_LOG));
	LoadTranLog(&stTranLog, gstSysCtrlParam.uiLastRecNo);

	if( stTranLog.ucTranType!=gstTransInfo.stTranLog.ucTranType                       ||
		strncmp((char *)stTranLog.szPan, (char *)gstTransInfo.stTranLog.szPan, 19)!=0 ||
		memcmp(stTranLog.szAmount,       gstTransInfo.stTranLog.szAmount,      12)!=0 ||
		memcmp(stTranLog.szTipAmount,    gstTransInfo.stTranLog.szTipAmount,   12)!=0
		)
	{
		return TRUE;
	}

	lcdCls();
	DisplayTransName();
	Display2StringInRect("重复交易,继续?", "DUPLICATE? Y/N");

	return PubYesNo(USER_OPER_TIMEOUT);
}


// 输入交易流水号以获取该交易数据
// 入口： 限制的交易状态
// 出口： 0 获取成功
int GetRecord(uint uiStatus, void *pstOutTranLog)
{
	int			iRet;
	UINT32		uiCnt;
	ulong		ulInvoiceNo;
	ST_TRAN_LOG	*pstLog;

	pstLog = (ST_TRAN_LOG *)pstOutTranLog;
	while( 1 )
	{
		iRet = InputInvoiceNo(&ulInvoiceNo);
		if( iRet!=0 )
		{
			return iRet;
		}

		for(uiCnt=0; uiCnt<MAX_TRANLOG; uiCnt++)
		{
			if( gstSysCtrlParam.sAcqKeyList[uiCnt]==INV_ACQ_KEY )
			{
				continue;
			}

			memset(pstLog, 0, sizeof(ST_TRAN_LOG));
			iRet = LoadTranLog(pstLog, uiCnt);
			if( iRet!=0 )
			{
				return iRet;
			}
			if( pstLog->ulInvoiceNo!=ulInvoiceNo )
//			if( pstLog->ulSTAN !=ulInvoiceNo )
			{
				continue;
			}
			if((uint) ((pstLog->uiStatus) & 0x0F) <= uiStatus )
			{
				gstTransInfo.uiRecNo = uiCnt;
				return 0;
			}
			else
			{
				DisplayTransLogState(pstLog->uiStatus);
				break;
			}
		}
		if( uiCnt>=MAX_TRANLOG )
		{
			Display2StringInRect("无此账单", "INVALID INVOICE#");
		}
		PubBeepErr();
		PubWaitKey(2);
	}
}


void CalcTotal(uint8_t ucAcqKey)
{
	uint8_t		ucAcqIndex, ucIssIndex,ucTranAct;
	char		szTotalAmt[12+1];
	UINT32		uiIndex;
	ST_TRAN_LOG	stLog;

	// clear all total information
	ClearTotalInfo(&gstTransTotal);
	ClearTotalInfo(&gstEdcTotal);
	for(ucAcqIndex=0; ucAcqIndex<gstSysParam.uiAcqNum; ucAcqIndex++)
	{
		ClearTotalInfo(&gstAcqTotal[ucAcqIndex]);
	}
	for(ucIssIndex=0; ucIssIndex<gstSysParam.uiIssuerNum; ucIssIndex++)
	{
		ClearTotalInfo(&gstIssuerTotal[ucIssIndex]);
	}

	// 计算出所有acquirer和issuer,以及edc的统计
	for(uiIndex=0; uiIndex<MAX_TRANLOG; uiIndex++)
	{
		if( gstSysCtrlParam.sAcqKeyList[uiIndex]==INV_ACQ_KEY )
		{
			continue;
		}

		memset(&stLog, 0, sizeof(ST_TRAN_LOG));
		LoadTranLog(&stLog, uiIndex);

		for(ucAcqIndex=0; ucAcqIndex<gstSysParam.uiAcqNum; ucAcqIndex++)
		{
			if( gstSysParam.stAcqList[ucAcqIndex].ucKey==stLog.ucAcqKey )
			{
				break;
			}
		}
		PubASSERT( ucAcqIndex<gstSysParam.uiAcqNum );

		for(ucIssIndex=0; ucIssIndex<gstSysParam.uiIssuerNum; ucIssIndex++)
		{
			if( gstSysParam.stIssuerList[ucIssIndex].ucKey==stLog.ucIssuerKey )
			{
				break;
			}
		}
		PubASSERT( ucIssIndex<gstSysParam.uiIssuerNum );

		PubAscAdd((char*)stLog.szAmount, (char*)stLog.szTipAmount, 12, (char*)szTotalAmt);
		PubAddHeadChars((char*)szTotalAmt, 12, '0');

		if( stLog.ucTranType==VOID || (stLog.uiStatus & TS_VOID) )
		{
			ucTranAct = glTranConfig[stLog.ucOrgTranType].ucTranAct;
		}
		else
		{
			ucTranAct = glTranConfig[stLog.ucTranType].ucTranAct;
		}

		if( ucTranAct & IN_SALE_TOTAL )
		{
			if( stLog.uiStatus & TS_VOID )
			{
				gstEdcTotal.uiVoidSaleCnt++;
				SafeAscAdd((char*)gstEdcTotal.szVoidSaleAmt, (char*)szTotalAmt, 12);

				gstAcqTotal[ucAcqIndex].uiVoidSaleCnt++;
				SafeAscAdd((char*)gstAcqTotal[ucAcqIndex].szVoidSaleAmt, (char*)szTotalAmt, 12);

				gstIssuerTotal[ucIssIndex].uiVoidSaleCnt++;
				SafeAscAdd((char*)gstIssuerTotal[ucIssIndex].szVoidSaleAmt, (char*)szTotalAmt, 12);
			}
			else
			{
				gstEdcTotal.uiSaleCnt++;
				SafeAscAdd((char*)gstEdcTotal.szSaleAmt, szTotalAmt, 12);

				gstAcqTotal[ucAcqIndex].uiSaleCnt++;
				SafeAscAdd((char*)gstAcqTotal[ucAcqIndex].szSaleAmt, szTotalAmt, 12);

				gstIssuerTotal[ucIssIndex].uiSaleCnt++;
				SafeAscAdd((char*)gstIssuerTotal[ucIssIndex].szSaleAmt, szTotalAmt, 12);

				if( memcmp(stLog.szTipAmount, "000000000000",12)!=0)
				{
					gstEdcTotal.uiTipCnt++;
					SafeAscAdd((char*)gstEdcTotal.szTipAmt, (char*)stLog.szTipAmount, 12);

					gstAcqTotal[ucAcqIndex].uiTipCnt++;
					SafeAscAdd((char*)gstAcqTotal[ucAcqIndex].szTipAmt, (char*)stLog.szTipAmount, 12);

					gstIssuerTotal[ucIssIndex].uiTipCnt++;
					SafeAscAdd((char*)gstIssuerTotal[ucIssIndex].szTipAmt, (char*)stLog.szTipAmount, 12);
				}
			}
		}

		if( ucTranAct& IN_REFUND_TOTAL )
		{
			if( stLog.uiStatus & TS_VOID )
			{
				gstEdcTotal.uiVoidRefundCnt++;
				SafeAscAdd((char*)gstEdcTotal.szVoidRefundAmt, szTotalAmt, 12);

				gstAcqTotal[ucAcqIndex].uiVoidRefundCnt++;
				SafeAscAdd((char*)gstAcqTotal[ucAcqIndex].szVoidRefundAmt, szTotalAmt, 12);

				gstIssuerTotal[ucIssIndex].uiVoidRefundCnt++;
				SafeAscAdd((char*)gstIssuerTotal[ucIssIndex].szVoidRefundAmt, szTotalAmt, 12);
			}
			else
			{
				gstEdcTotal.uiRefundCnt++;
				SafeAscAdd((char*)gstEdcTotal.szRefundAmt, szTotalAmt, 12);

				gstAcqTotal[ucAcqIndex].uiRefundCnt++;
				SafeAscAdd((char*)gstAcqTotal[ucAcqIndex].szRefundAmt, szTotalAmt, 12);

				gstIssuerTotal[ucIssIndex].uiRefundCnt++;
				SafeAscAdd((char*)gstIssuerTotal[ucIssIndex].szRefundAmt, szTotalAmt, 12);
			}
		}

		if(stLog.uiStatus & TS_ALREADY_TC)
		{
			gstEdcTotal.uiTcSendCnt++;
			SafeAscAdd((char *)gstEdcTotal.szTcSendAmt, szTotalAmt, 12);
			
			gstAcqTotal[ucAcqIndex].uiTcSendCnt++;
			SafeAscAdd((char *)gstAcqTotal[ucAcqIndex].szTcSendAmt, szTotalAmt, 12);
			
			gstIssuerTotal[ucIssIndex].uiTcSendCnt++;
			SafeAscAdd((char *)gstIssuerTotal[ucIssIndex].szTcSendAmt, szTotalAmt, 12);			
		}
	}

	if( ucAcqKey==ACQ_ALL )
	{
		memcpy(&gstTransTotal, &gstEdcTotal, sizeof(ST_TOTAL_INFO));
		return;
	}

	for(ucAcqIndex=0; ucAcqIndex<gstSysParam.uiAcqNum; ucAcqIndex++)
	{
		if( gstSysParam.stAcqList[ucAcqIndex].ucKey==ucAcqKey )
		{
			break;
		}
	}
	memcpy(&gstTransTotal, &gstAcqTotal[ucAcqIndex], sizeof(ST_TOTAL_INFO));
}

// save last emv status
int SaveEmvStatus(void)
{
	int		iRet;

	iRet = PubFileWrite(EMV_STATUS_FILE, 0L, &gstEmvStatus, sizeof(EMV_STATUS));
	if( iRet!=0 )
	{
		PubTRACE0("SaveEmvStatus");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// save last emv status
int LoadEmvStatus(void)
{
	int		iRet;

	iRet = PubFileRead(EMV_STATUS_FILE, 0L, &gstEmvStatus, sizeof(EMV_STATUS));
	if( iRet!=0 )
	{
		PubTRACE0("LoadEmvStatus");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}

// save EMV error log message
int SaveEmvErrLog(void)
{
	int			iRet, iLength = 0;
	EMV_ERR_LOG	stErrLog;

	// collect message for log
	memset(&stErrLog, 0, sizeof(EMV_ERR_LOG));
	stErrLog.bValid = TRUE;
	stErrLog.ucAidLen = gstTransInfo.stTranLog.ucAidLen;
	memcpy(stErrLog.szAID, gstTransInfo.stTranLog.sAID, stErrLog.ucAidLen);
	sprintf((char *)stErrLog.szPAN, "%.19s", gstTransInfo.stTranLog.szPan);
	stErrLog.ucPANSeqNo = gstTransInfo.stTranLog.bPanSeqOK ? gstTransInfo.stTranLog.ucPanSeqNo : 0xFF;
	sprintf((char *)stErrLog.szAmount, "%.12s", gstSendPack.szTranAmt);
	sprintf((char *)stErrLog.szTipAmt, "%.12s", gstSendPack.szExtAmount);
	sprintf((char *)stErrLog.szRspCode, "%.2s", gstTransInfo.stTranLog.szRspCode);
	GetDateTime(stErrLog.szDateTime);
	sprintf((char *)stErrLog.szRRN, "%.12s",    gstTransInfo.stTranLog.szRRN);
	sprintf((char *)stErrLog.szAuthCode, "%.6s", gstTransInfo.stTranLog.szAuthCode);

	emv_get_data_element((uchar*)"\x95", 1, (uint8_t*)&iLength, stErrLog.sTVR);
	emv_get_data_element((uchar*)"\x9B", 1, (uint8_t*)&iLength, stErrLog.sTSI);

	stErrLog.uiReqICCDataLen = (UINT32)PubChar2Long(gstSendPack.sICCData, 2, NULL);
	if (stErrLog.uiReqICCDataLen <= 0)
	{
		// prepare online DE55 data
		iRet = SetDE55(TRUE, (uchar*)&gstSendPack.sICCData[2], &iLength);
		if( iRet!=0 )
		{
			return ONLINE_DENIAL;
		}
		PubASSERT( iLength<LEN_ICC_DATA );
		PubLong2Char((ulong)iLength, 2, gstSendPack.sICCData);
		memcpy(gstTransInfo.stTranLog.sIccData, &gstSendPack.sICCData[2], iLength);	// save for batch upload
		gstTransInfo.stTranLog.uiIccDataLen = (UINT32)iLength;	
		stErrLog.uiReqICCDataLen = iLength;
	}
	
	memcpy(stErrLog.sReqICCData, &gstSendPack.sICCData[2], stErrLog.uiReqICCDataLen);
	stErrLog.uiReqField56Len = (UINT32)PubChar2Long(gstSendPack.sICCData2, 2, NULL);
	memcpy(stErrLog.sReqField56, &gstSendPack.sICCData2[2], stErrLog.uiReqField56Len);
	if( memcmp(gstSendPack.szSTAN, gstRecvPack.szSTAN, 6)==0 )
	{
		stErrLog.uiRspICCDataLen = (UINT32)PubChar2Long(gstRecvPack.sICCData, 2, NULL);
		memcpy(stErrLog.sRspICCData, &gstRecvPack.sICCData[2], stErrLog.uiRspICCDataLen);
	}
	stErrLog.ulSTAN = gstTransInfo.stTranLog.ulSTAN;

	PubASSERT( gstSysCtrlParam.uiErrLogNo<MAX_ERR_LOG );
	if( gstSysCtrlParam.uiErrLogNo>=MAX_ERR_LOG )
	{
		gstSysCtrlParam.uiErrLogNo = 0;
	}
	iRet = PubFileWrite(EMV_ERRLOG_FILE, (long)(gstSysCtrlParam.uiErrLogNo * sizeof(EMV_ERR_LOG)),
						&stErrLog, sizeof(EMV_ERR_LOG));
	if( iRet!=0 )
	{
		PubTRACE0("SaveEmvErrLog");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	gstSysCtrlParam.uiErrLogNo++;
	if( gstSysCtrlParam.uiErrLogNo>=MAX_ERR_LOG )
	{
		gstSysCtrlParam.uiErrLogNo = 0;
	}
	SaveSysCtrlBase();

	return 0;
}

// load one error log
int LoadErrLog(UINT32 uiRecNo, void *pOutErrLog)
{
	int		iRet, iFilesize;

	iRet = fileExist(EMV_ERRLOG_FILE);
	if (iRet <0)
	{
		return ERR_NOT_FILE;
	}

	iFilesize = fileSize(EMV_ERRLOG_FILE);
	if (iFilesize < ((uiRecNo+1) * sizeof(EMV_ERR_LOG)))
	{
		return ERR_NOT_FILE;
	}

	iRet = PubFileRead(EMV_ERRLOG_FILE, (long)(uiRecNo * sizeof(EMV_ERR_LOG)),
						pOutErrLog, sizeof(EMV_ERR_LOG));
	if( iRet!=0 )
	{
		PubTRACE0("LoadEmvErrLog");
		PubSysHalt();
		return ERR_FILEOPER;
	}

	return 0;
}


// for BEA fallback process
int LastRecordIsFallback(void)
{
	int			iRet;
	ST_TRAN_LOG	stLog;

	if( gstSysCtrlParam.uiLastRecNoList[gstCurAcq.ucIndex]>=MAX_TRANLOG )
	{
		return FALSE;
	}

	memset(&stLog, 0, sizeof(ST_TRAN_LOG));
	iRet = LoadTranLog(&stLog, gstSysCtrlParam.uiLastRecNoList[gstCurAcq.ucIndex]);
	if( iRet!=0 )
	{
		return FALSE;
	}
	if( stLog.ucEntryMode & MODE_FALLBACK_SWIPE )
	{
		return TRUE;
	}

	return FALSE;
}

void PubSysHalt(void)
{
	lcdClrLine(2,7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		lcdDisplay(0,3,DISP_CFONT,"System error");
	else
		lcdDisplay(0,3,DISP_CFONT,"异常,请重启机器!");
	lcdDisplay(0,5,DISP_CFONT,"PLS RESTART POS");
	lcdFlip();
	while(1);
}


int    SaveCAPKLogFile(void *pstCapk)
{
	int	fd, iRet;
	
	fd = fileOpen((char *)EMV_CAPK_FILE, O_CREAT|O_RDWR);
	if( fd<0 )
	{
		PubTRACE0("SaveCapkFile");
		return ERR_FILEOPER;
	}

	iRet = fileSeek(fd, 0, SEEK_END);
	if( iRet<0 )
	{
		fileClose(fd);
		PubSysHalt();
		return ERR_FILEOPER;
	}
	
	iRet = PubFWriteN(fd, pstCapk, sizeof(CAPKLIST));
	fileClose(fd);
	if( iRet!=(int)sizeof(CAPKLIST) )
	{
		PubTRACE0("SaveCapkfile");
		PubSysHalt();
		return iRet;
	}

	return 0;
}


int    LoadCAPKLogFile(UINT32 uiRecNo, void *pstCapk)
{
	int		iRet, iFilesize;
	
	iRet = fileExist(EMV_CAPK_FILE);
	if (iRet <0)
	{
		return ERR_NOT_FILE;
	}
	
	iFilesize = fileSize(EMV_CAPK_FILE);
	if (iFilesize < ((uiRecNo+1) * sizeof(CAPKLIST)))
	{
		return ERR_NOT_FILE;
	}
	
	iRet = PubFileRead(EMV_CAPK_FILE, (long)(uiRecNo * sizeof(CAPKLIST)),pstCapk, sizeof(CAPKLIST));
	if( iRet!=0 )
	{
		PubTRACE0("LoadEmvCapkLog");
		PubSysHalt();
		return ERR_FILEOPER;
	}
	
	return 0;
}

int    LoadAllCapkParam(void)
{
	int  iRet, iFileSize, iCnt;
	CAPKLIST  stParamCapk;

	iFileSize = fileSize(EMV_CAPK_FILE); 
	if (iFileSize <= 0)
	{
		return 0;
	}

	iFileSize = iFileSize/(sizeof(CAPKLIST));
	for (iCnt=0; iCnt<iFileSize; iCnt++)
	{
		iRet = LoadCAPKLogFile(iCnt,&stParamCapk);
		if (iRet == 0)
		{
			emv_add_one_ca_public_key(&stParamCapk);
		}
	}

	return 0;
}


int    SaveAIDLogFile(void *pstAid)
{
	int	 fd, iRet;
	
	fd = fileOpen((char *)EMV_AID_FILE, O_CREAT|O_RDWR);
	if( fd<0 )
	{
		PubTRACE0("SaveAidFile");
		return ERR_FILEOPER;
	}
	
	iRet = fileSeek(fd, 0, SEEK_END);
	if( iRet<0 )
	{
		fileClose(fd);
		PubSysHalt();
		return ERR_FILEOPER;
	}
	
	iRet = PubFWriteN(fd, pstAid, sizeof(AIDLIST));
	fileClose(fd);
	if( iRet!=(int)sizeof(AIDLIST) )
	{
		PubTRACE0("SaveAidfile");
		PubSysHalt();
		return iRet;
	}
	
	return 0;
}


int    LoadAIDLogFile(UINT32 uiRecNo, void *pstAid)
{
	int		iRet, iFilesize;
	
	iRet = fileExist(EMV_AID_FILE);
	if (iRet <0)
	{
		return ERR_NOT_FILE;
	}
	
	iFilesize = fileSize(EMV_AID_FILE);
	if (iFilesize < ((uiRecNo+1) * sizeof(AIDLIST)))
	{
		return ERR_NOT_FILE;
	}
	
	iRet = PubFileRead(EMV_AID_FILE, (long)(uiRecNo * sizeof(AIDLIST)),pstAid, sizeof(AIDLIST));
	if( iRet!=0 )
	{
		PubTRACE0("LoadEmvAidLog");
		PubSysHalt();
		return ERR_FILEOPER;
	}
	
	return 0;
}


int    LoadAllAidParam(void)
{
	int  iRet, iFileSize, iCnt;
	AIDLIST   stParamAid;
	
	iFileSize = fileSize(EMV_AID_FILE); 
	if (iFileSize <= 0)
	{
		return 0;
	}
	
	iFileSize = iFileSize/(sizeof(AIDLIST));
	for (iCnt=0; iCnt<iFileSize; iCnt++)
	{
		iRet = LoadAIDLogFile(iCnt,&stParamAid);
		if (iRet == 0)
		{
			emv_add_one_aid_info(&stParamAid);
		}
	}
	
	return 0;
}

// end of file
