
#include "global.h"
#include "Param.h"

/********************** Internal macros declaration ************************/
/********************** Internal structure declaration *********************/
ST_NODE_INFO   sgStNodeInfo[] =
{
	{"EDC",     EDC_NODE},
	{"ACQ",     ACQ_NODE},
	{"ISSURE",  ISSURE_NODE},
	{"CARD",    CARD_NODE},
	{"EMVAID",  EMVAID_NODE},
	{"CAPK",    EMVCAPK_NODE},
	{NULL,0},
};
char    sZmk[16];
char    sTmk[16];
char    sTak[16];
/********************** Internal functions declaration *********************/
ST_PARAM_FILE  stEDCConfg[] =
{
	{"MNAME",           gstSysParam.stEdcInfo.szMerchantName,     ATTR_CSTR,    30},
	{"MADDR1",          gstSysParam.stEdcInfo.szMerchantAddr,     ATTR_CSTR,    40},
	{"MADDR2",          &gstSysParam.stEdcInfo.szMerchantAddr[40],  ATTR_CSTR,    40},
	{"SYSTEM PWD",      gstSysParam.szPassword[0],                ATTR_NSTR,    16},
	{"TERMINAL PWD",    gstSysParam.szPassword[1],                ATTR_NSTR,    16},
	{"SYSTEM PWD2",      gstSysParam.szPassword[2],                ATTR_NSTR,    16},
/*	{"TIDPWD",          gstSysParam.szPassword[1],                ATTR_NSTR,    16},
	{"VOID PWD",        gstSysParam.szPassword[2],                ATTR_NSTR,    16},
	{"REFUND PWD",      gstSysParam.szPassword[3],                ATTR_NSTR,    16},
	{"ADJ PWD",         gstSysParam.szPassword[4],                ATTR_NSTR,    16},
	{"SETTLE PWD",      gstSysParam.szPassword[5],                ATTR_NSTR,    16},*/
	{"SETTLEMENT TIME", gstSysParam.szAutoSettlementTime,         ATTR_CSTR,     4},
	{"MAX SETTLE AMT",  &gstSysParam.stEdcInfo.ulMaxSettleAmt,    ATTR_NLONG,    9},
	{"INIT ISN",        gstSysParam.stEdcInfo.sInitSTAN,          ATTR_BCD,      6},
	{"CURRENCY NAME",   gstSysParam.stEdcInfo.szCurrencyName,     ATTR_CSTR,     3},
	{"CURRENCY SYMBOL", &gstSysParam.stEdcInfo.ChCurrencySymbol,  ATTR_CSTR,     1},
	{"DECIMAL POSITION",&gstSysParam.stEdcInfo.uiDecimalPos,      ATTR_NLONG,    1},
	{"MAX AMT LENGTH",  &gstSysParam.stEdcInfo.uiTranAmtLen,      ATTR_NLONG,    2},
	{"CHECK ADJ PWD",   &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_ADJUST_PWD},
	{"CHECK MANU PWD",  &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_MANUAL_PWD},
	{"CHECK SETT PWD",  &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_SETTLE_PWD},
	{"CHECK RFD PWD",   &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_REFUND_PWD},
	{"CHECK VOID PWD",  &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_VOID_PWD},
	{"KEY LOCKED",      &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_KEYBOARD_LOCKED},
	{"TIP PROCESS",     &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_TIP_PROCESS},
	{"ENABLE INSTAL",   &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_ENABLE_INSTALMENT},
	{"AUTH OR PREAUTH", &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_AUTH_PREAUTH},
	{"ENABLE CASH",     &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_CASH_PROCESS},
	{"OFFLINE TXN",     &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_OFFLINE_TXN},
	{"ENABLE DCC",      &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_ENABLE_DCC},
	{"REFERRAL DIAL",   &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_REFERRAL_DIAL},
	{"ENABLE ELECSIGN",&gstSysParam.stEdcInfo.sOption,	   ATTR_BIT,     EDC_ENABLE_ELECSIGN},
	{"AUTO SETTLEMENT", &gstSysParam.stEdcInfo.sOption,           ATTR_BIT,    EDC_AUTO_SETTLEMENT},
	{"PRE DIAL",        &gstSysParam.stEdcInfo.bPreDial,          ATTR_BIT,      1},
	{"CLEAR BATCH",     &gstSysParam.stEdcInfo.bClearBatch,       ATTR_BIT,      1},
	{"PRINT NUMBER",    &gstSysParam.stEdcInfo.uiPrintNums,       ATTR_NLONG,    1},
	{"ACCEPT TIMEOUT",  &gstSysParam.stEdcInfo.ulAcceptTimeout,   ATTR_NLONG,    2},
	{"PABX",            gstSysParam.stEdcInfo.szPabx,             ATTR_CSTR,     10},
	{"ADDTIONAL",       gstSysParam.stEdcInfo.szAddlPrompt,       ATTR_CSTR,     20},
	{"ZMK",             sZmk,                                     ATTR_BCD,      32},
	{"TMK",             sTmk,                                     ATTR_BCD,      32},
	{"TAK",             sTak,                                     ATTR_BCD,      32},
	{"REFERENCE NO",   gstSysParam.stEdcInfo.sTmkRefNumber,      ATTR_BCD,      16},
	{NULL,NULL,0,0},
};


ST_PARAM_FILE  stACQConfg[] =
{
	{"NAME",               gstCurAcq.szName,             ATTR_CSTR,    10},
	{"TID",                gstCurAcq.szTermID,           ATTR_NSTR,     8},
	{"MID",                gstCurAcq.szMerchantID,       ATTR_NSTR,    15},
	{"NII",                gstCurAcq.szNii,              ATTR_NSTR,     3},
	{"TIMEOUT",            &gstCurAcq.ulTimeOut,         ATTR_NLONG,    3},
	{"BATCH NO.",          &gstCurAcq.ulCurBatchNo,      ATTR_NLONG,    3},
	{"COMMINICATE TYPE",   &gstCurAcq.stTxnCommCfg.ucCommType,                 ATTR_CSTR,     1},
	{"RS232 PORT",         &gstCurAcq.stTxnCommCfg.stRS232Para.ucPortNo,       ATTR_NLONG,    1},
	{"RS232 ATTR",         gstCurAcq.stTxnCommCfg.stRS232Para.szAttr,          ATTR_CSTR,     20},
	{"PRIMARY TEL",        gstCurAcq.szTxnTelNo1,       ATTR_CSTR,      25},
	{"SECOND TEL",         gstCurAcq.szTxnTelNo2,       ATTR_CSTR,      25},
	{"PRIMARY PORT",       gstCurAcq.szRemotePort,      ATTR_NSTR,      5},
	{"PRIMARY IP",         gstCurAcq.szRemoteIP,        ATTR_NSTR,      15},
	{"SECOND PORT",        gstCurAcq.szRemotePort2,     ATTR_NSTR,      5},
	{"SECOND IP",          gstCurAcq.szRemoteIP2,       ATTR_NSTR,      15},
	{"WIRE APN",           gstCurAcq.stTxnCommCfg.stWirlessPara.szAPN,         ATTR_CSTR,     64},
	{"WIRE USER NAME",     gstCurAcq.stTxnCommCfg.stWirlessPara.szUID,         ATTR_CSTR,     64},
	{"WIRE USER PWD",      gstCurAcq.stTxnCommCfg.stWirlessPara.szPwd,         ATTR_CSTR,     16},
	{"ENABLE ADDTIONAL",   &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_ADDTIONAL_PROMPT},
	{"ENABLE EMV",         &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_EMV_FEATURE},
	{"ONLINE VOID",		   &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_ONLINE_VOID},
	{"ONLINE REFUND",	   &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_ONLINE_REFUND},
	{"DIS. TRICK FEED",	   &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_DISABLE_TRICK_FEED},
	{"AMEX FEATURE",	   &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_AMEX_SPECIFIC_FEATURE},
	{"AIR TICKET",		   &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_AIR_TICKET},
	{"BOC INSTALMENT",	   &gstCurAcq.sOption,                             ATTR_BIT,    ACQ_BOC_INSTALMENT_FEATURE},
	{NULL,NULL,0,0},
};


ST_PARAM_FILE  stIssureConfg[]=
{
	{"NAME",                 gstCurIssuer.szName,            ATTR_CSTR,    10},
	{"REFNO",                gstCurIssuer.szRefTelNo,        ATTR_NSTR,    24},
	{"MASKCARD",             gstCurIssuer.sPanMask,          ATTR_SBIT,    24},
	{"FLOORLIMIT",           &gstCurIssuer.ulFloorLimit,     ATTR_NLONG,     6},
	{"ADJ PERCENT",          &gstCurIssuer.ucAdjustPercent,  ATTR_NLONG,     2},
	{"ENABLE ADJUST",        &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_ADJUST},
	{"ENABLE DISCRIPTOR",    &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_DISCRIPTOR},
	{"ENABLE VOICE",         &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_VOICE_REFERRAL},
	{"ENABLE OFFLINE",       &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_OFFLINE},
	{"ENABLE EXPIRY",        &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_EXPIRY},
	{"CHKEXP OFFLINE",       &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_CHECK_EXPIRY_OFFLINE},
	{"ENABLE MANUL",         &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_MANUL},
	{"NEED PIN",             &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_PIN},
	{"ENABLE SELECE ACC",    &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_ACCOUNT_SELECTION},
//	{"ENABLE REFUND",        &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_REFUND},
	{"CAPTURE TXN",          &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_CAPTURE_TXN},
	{"ENABLE PRINT",         &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_PRINT},
	{"CHECK PAN MOD10",      &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_CHKPAN_MOD10},
	{"CHECK PAN MOD11",      &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_CHKPAN_MOD11},
	{"CHECK EXPIRY",         &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_CHECK_EXPIRY},
	{"CHECK AUTH CODE",      &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_AUTH_CODE},
	{"ENABLE VOID",          &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_VOID},
	{"DISPLAY EXPIRY",       &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_DISP_EXPIRY},
	{"ENABLE BALANCE",       &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_BALANCE},
	{"ENABLE PREAUTH",       &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_PREAUTH},
	{"ENABLE SWIPE",         &gstCurIssuer.sOption,          ATTR_BIT,   ISSUER_EN_SWIPE},
	{NULL,NULL,0,0},
};

ST_PARAM_FILE  stCardConfg[]=
{
	{"HIGH PAN",       gstCurCard.sPanRangeHigh,         ATTR_BCD,    10},
	{"LOW PAN",        gstCurCard.sPanRangeLow,          ATTR_BCD,    10},
	{"OPTION",         &gstCurCard.ChOption,             ATTR_BIT,     1},
	{"PAN LEN",        &gstCurCard.ulPanLength,          ATTR_NLONG,   2},
	{NULL,NULL,0,0},
};

AIDLIST   stParamAid;
CAPKLIST  stParamCapk;

ST_PARAM_FILE  stEmvAidConfg[]=
{
	{"AID LEN",        &stParamAid.aid_len,                                 ATTR_CLONG,   2},
	{"AID",            stParamAid.aid,                                      ATTR_BCD,     32},
	{"SELECT FLAG",    &stParamAid.support_partial_aid_select,              ATTR_CLONG,   1},
	{"PRIORTY",        &stParamAid.application_priority,                    ATTR_CLONG,   1},
	{"TARGETPER",      &stParamAid.target_percentage,                       ATTR_NLONG,   6},
	{"MAXTARGET",      &stParamAid.maximum_target_percentage,               ATTR_NLONG,   6},
	{"FLOORLIMIT",     &stParamAid.terminal_floor_limit,                    ATTR_NLONG,   6},
	{"THRESHOLD",      &stParamAid.threshold_value,                         ATTR_NLONG,   6},
	{"TACDENIAL",      stParamAid.terminal_action_code_denial,              ATTR_BCD,     10},
	{"TACONLINE",      stParamAid.terminal_action_code_online,              ATTR_BCD,     10},
	{"TACDEFAULT",     stParamAid.terminal_action_code_default,             ATTR_BCD,     10},
	{"ACQID",          stParamAid.acquirer_identifier,                      ATTR_BCD,     10},
	{"DDOL LEN",       &stParamAid.len_of_default_ddol,                     ATTR_NLONG,   3},
	{"DDOL VAL",       stParamAid.default_ddol,                             ATTR_BCD,     512},
	{"TDOL LEN",       &stParamAid.len_of_default_tdol,                     ATTR_NLONG,   3},
	{"TDOL VAL",       stParamAid.default_tdol,                             ATTR_BCD,     512},
	{"VERSION",        stParamAid.application_version,                      ATTR_BCD,     4},
	{"RISK LEN",       &stParamAid.len_of_terminal_risk_management_data,    ATTR_NLONG,   2},
	{"RISK DATA",      stParamAid.terminal_risk_management_data,            ATTR_BCD,     16},
	{NULL,NULL,0,0},
};

ST_PARAM_FILE  stEmvCapkConfg[]=
{
	{"RID",            stParamCapk.RID,                     ATTR_BCD,    10},
	{"RID INDEX",      &stParamCapk.Index,                  ATTR_BCD,    2},
	{"MOD LEN",        &stParamCapk.len_of_modulus,         ATTR_CLONG,   3},
	{"MOD",            stParamCapk.modulus,                 ATTR_BCD,    496},
	{"EXP LEN",        &stParamCapk.len_of_exponent,        ATTR_CLONG,    2},
	{"EXPONENT",       stParamCapk.exponent,                ATTR_BCD,     6},
	{"EXP DATA",       stParamCapk.expiration_date,         ATTR_BCD,     6},
	{"HASH",           stParamCapk.checksum,                ATTR_BCD,    40},
	{NULL,NULL,0,0},
};


int     gszStackNodeType[100];    //  对没有分析处理完成的段进行压栈，需要初始化
int     giStackNodeIndex;         //  堆栈段索引
char    gszParamFileName[50];      //  参数文件名
/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/

int  FilterHeadChars(char *psString,uint uiLen)
{ 
	uint  uiCnt;

	if (psString==NULL || uiLen == 0)
		return ERR_NOT_DATE;

	uiCnt = 0;
	while(uiCnt<uiLen || !psString)
	{
		switch (*psString)
		{
		case 0x0D:
			if (psString[uiCnt+1] == 0x0A)
				break;
			else
				return uiCnt;
			break;
		case 0x0A:
		case ' ' :
			break;
		default:
			return uiCnt;
		}
		uiCnt++;
		psString++;
	}

	return uiCnt;
}

//   从字符串的起始位置开始找特定的字符，并将特定字符的偏移位返回
int  FindOneChar(char *psString, uint uiStrLen,char CharSet)
{
	uint uiCnt;

	if (psString==NULL || uiStrLen == 0)
		return ERR_NOT_FIND;

	uiCnt = 0;
	while(uiCnt<uiStrLen || psString==NULL)
	{
		if (*psString==CharSet)
			return uiCnt;

		uiCnt++;
		psString++;
	}
	
	return ERR_NOT_FIND;
}

//  从字符串的最后一位开始找特定的字符
int  FindReverOneChar(char *psString, uint uiStrLen,char CharSet)
{	
	if (psString==NULL || uiStrLen ==0)
		return ERR_NOT_DATE;

	psString += uiStrLen-1;	
	while(uiStrLen || psString==NULL)
	{
		if (*psString==CharSet)
			return uiStrLen;

		uiStrLen--;
		psString--;
	}
	
	return ERR_NOT_FIND;
}

/************************************************************************************
功能：
   在限定的缓冲区内找到第一个段标志，包括起始段标志或者段的结束标志。
   如果找到段则输出段名(结束段名包括"\")，以及段名长度;
   如果找到关键段则返回该段所在行到起始位置的长度。
   如果没有找到则返回 ERR_NOT_FIND
************************************************************************************/
int  FindFirstNode(char *psParam, uint uiParamLen,char **psNodeName,uint *uiNameLen)
{
	int iCnt,iFirstIndex,iEndIndex,iSpecIndex,iStrlen;
	char *pTemp;
	
	if (psParam==NULL || uiParamLen ==0)
		return ERR_NOT_DATE;
		
	iCnt = 0;
	iSpecIndex = 0;
	while (iCnt<uiParamLen)
	{
		// 每次先分析获取一行数据，然后对该行数据进行分析
		iStrlen = GetStringLine(psParam+iCnt,uiParamLen-iCnt);
		if (iStrlen < 0)
			return iStrlen;   // 文件格式错误或者没有数据可供分析

		pTemp = psParam + iCnt;  // 当前行的起始点
		iCnt += iStrlen;         // 下一行的偏移长度

		iFirstIndex = FindOneChar(pTemp,iStrlen,'<');
		if (iFirstIndex==ERR_NOT_FIND)
		{
			continue;
		}
	
		iEndIndex = FindOneChar(pTemp+iFirstIndex+1,iStrlen,'>');
		if (iEndIndex==ERR_NOT_FIND)
		{
			continue;
		}

		if (iEndIndex-iFirstIndex<=1)  // 如果"< >"对中为空，则继续查找
		{
			continue;
		}

		if (psNodeName)
		{
			*uiNameLen = iEndIndex ;
			*psNodeName = pTemp+iFirstIndex+1;
		}

		return iCnt;
	}

	return ERR_NOT_DATE;
//	return ERR_NOT_FIND;
}

int  GetNodeType(char *pszNodeName, uint uiNameLen)
{
	int  iCnt = 0;

	while (sgStNodeInfo[iCnt].pszNodeName != NULL)
	{
		if (memcmp(sgStNodeInfo[iCnt].pszNodeName,pszNodeName,uiNameLen)==0)
		{
			return sgStNodeInfo[iCnt].iNodeType;
		}

		iCnt++;
	}

	return  ERR_NODE_TYPE;
}

//  从数据缓冲区中分析出一行数据,将数据行的长度返回
int  GetStringLine(char *psSource,uint uiLen)
{
	uint  uiCnt;
	int   iRet;

	if (psSource==NULL || uiLen <=0)
		return  ERR_NOT_DATE;

	uiCnt=0;
	while (uiCnt<uiLen)
	{
		iRet = FindOneChar(psSource,uiLen,0x0A);
		if (iRet == ERR_NOT_FIND)
			return ERR_FILE_FORMAT;  // 没有换行符号，文件格式错误

		return (iRet+1);   // 换行符号则返回数据行长度
	}

	return  ERR_FILE_FORMAT;
}


int  GetVarNameAndVaule(char * psStrLine,uint uiStrLen,char **psVarName,uint *uiNameLen,char **psVarValue,uint *uiVallen)
{
	int iEndLineIndex,iValueNameLen,iSpecIndex;

	if (psStrLine==NULL || uiStrLen <=0)
		return  0;

	*uiNameLen = 0;
	*uiVallen = 0;
	iSpecIndex = FilterHeadChars(psStrLine,uiStrLen);
	if ((iSpecIndex == ERR_NOT_DATE) || (iSpecIndex >= uiStrLen))  //所有数据虑掉就不需要下面的分析 
	{
		return uiStrLen;
	}
	psStrLine += iSpecIndex;
	uiStrLen -= iSpecIndex;

	iEndLineIndex = GetStringLine(psStrLine,uiStrLen);     // 获取一行有待分析的数据
	if (iEndLineIndex < 0 )
		return  iEndLineIndex;

	iSpecIndex = FindOneChar(psStrLine,iEndLineIndex,'=');   // 分析关键字“＝”
	if (iSpecIndex==ERR_NOT_FIND)
		return iEndLineIndex;

	iValueNameLen = FindOneChar(psStrLine,iSpecIndex,'(');  // 分析处理是否有属性
	if (iValueNameLen == ERR_NOT_FIND)
		*uiNameLen = iSpecIndex-1;
	else
		*uiNameLen = iValueNameLen-1;

	*psVarName = psStrLine;

// 如果允许行后面还能有注释，在此加多对注释的处理
	
	*psVarValue = psStrLine +iSpecIndex+1;

// 需要去掉2个换行符号,和由于iSpecIndex是下标索引而iEndLineIndex是长度所引起的差1
	if (psStrLine[iEndLineIndex-2] !=0x0D)
		*uiVallen = iEndLineIndex - iSpecIndex-2;  // linux系统下的文件系统换行符合只有1个字节
	else
		*uiVallen = iEndLineIndex - iSpecIndex-2-1;   
	
	return iEndLineIndex;
}


int  UnPackParamFile(char *psParam, uint uiParamLen)
{
	int iRet,iFNodeNamelen,iNextNodeNamelen,iFirstIndex,iNextIndex;
	int iNodeType,iPackDatelen,iUnpackDatelen,iCurDatelen;
	char *pszFirNodeName, *pszNextNodeName,*psTemp;

	if (psParam==NULL || uiParamLen==0)
		return ERR_NOT_DATE;

	iRet = 0;
	iUnpackDatelen = 0;  // 没有分析处理的数据长度
	iPackDatelen = 0;    // 已经分析处理过的长度
	iCurDatelen = 0;
	while(iPackDatelen<uiParamLen)
	{
		psTemp = psParam+iPackDatelen;
		iUnpackDatelen = uiParamLen - iPackDatelen;
		iFirstIndex = FindFirstNode(psTemp,iUnpackDatelen,&pszFirNodeName,(uint*)&iFNodeNamelen);
		if (iFirstIndex == ERR_NOT_DATE || iFirstIndex == ERR_FILE_FORMAT)
		{
			return iFirstIndex;
		}
		// 如果没有找到段或者找到了段，但是是段的结束标志则取段堆栈的最后一个段类型进行分析
		if (iFirstIndex==ERR_NOT_FIND || pszFirNodeName[0]=='\\')  
		{
			iNodeType = gszStackNodeType[giStackNodeIndex];
			if (iNodeType ==0) // 没有结点而且也没有前一次分析结点的类型则把数据作为不可知，直接返回
				return 0;  
			if (iFirstIndex==ERR_NOT_FIND)
			{
				iCurDatelen = iUnpackDatelen;
			}
			else
			{
				iCurDatelen = iFirstIndex;
			}
			
			switch(iNodeType)
			{
			case EDC_NODE:						
				iRet = UnPackEdcParam(psTemp,iCurDatelen);
				if(iRet == 0)
				{
					iRet = CheckEdcParam();  // 
				}
				break;
			case ACQ_NODE:
				iRet = UnPackAcqParam(psTemp,iCurDatelen,FALSE);
				if(iRet == 0)
				{
					iRet = CheckAcqParam();  // 
				}
				break;
			case ISSURE_NODE:
				iRet = UnPackIssureParam(psTemp,iCurDatelen,FALSE);
				if(iRet == 0)
				{
					iRet = CheckIssureParam();  // 
				}
				break;
			case CARD_NODE:
				iRet = UnPackCardParam(psTemp,iCurDatelen,FALSE);
				if(iRet == 0)
				{
					iRet = CheckCardParam();  // 
				}
				break;
			case EMVAID_NODE:
				iRet = UnpackEmvAidParam(psTemp,iCurDatelen,FALSE);
				if(iRet == 0)
				{
					iRet = CheckEmvAidParam();  // 
				}
				break;
			case EMVCAPK_NODE:
				iRet = UnpackEmvCapkParam(psTemp,iCurDatelen,FALSE);
				if(iRet == 0)
				{
					iRet = CheckEmvCapkParam();  // 
				}
				break;
			default:
				break;
			}
			
			if (iRet == ERR_NOT_DATE || iRet == ERR_FILE_FORMAT)
			{
				return iRet;
			}

			if (iRet < 0 )
			{
				DispUnpackErrMsg(iRet);
			}

			if (iFirstIndex==ERR_NOT_FIND)  // 如果没有找到段，则分析完所有数据后退出
			{
				return 0;
			}
			else   // 是段的结束标志，则先出栈然后继续处理
			{
				gszStackNodeType[giStackNodeIndex] = 0;  // 出栈
				giStackNodeIndex--;
				iPackDatelen += iCurDatelen;      // 对分析处理过的数据进行加总
				continue;
			}
		}		
		else// 下面是找到段的分析处
		{
			// 如果起始点到该段的起始点中间还有关键字“＝”则分析取堆栈的段类型然后分析
			if(FindOneChar(psTemp,iFirstIndex,'=')!=ERR_NOT_FIND)  
			{
				iNodeType = gszStackNodeType[giStackNodeIndex];
				if (iNodeType ==0) // 没有结点而且也没有前一次分析结点的类型则把数据作为不可知，直接返回
					return 0;  
				
				switch(iNodeType)
				{
				case EDC_NODE:
					iRet = UnPackEdcParam(psTemp,iFirstIndex);
					break;
				case ACQ_NODE:
					iRet = UnPackAcqParam(psTemp,iFirstIndex,FALSE);
					break;
				case ISSURE_NODE:
					iRet = UnPackIssureParam(psTemp,iFirstIndex,FALSE);
					break;
				case CARD_NODE:
					iRet = UnPackCardParam(psTemp,iFirstIndex,FALSE);
					break;
				case EMVAID_NODE:
					iRet = UnpackEmvAidParam(psTemp,iCurDatelen,FALSE);
					break;
				case EMVCAPK_NODE:
					iRet = UnpackEmvCapkParam(psTemp,iCurDatelen,FALSE);
				break;
				default:
					break;
				} 

				if (iRet == ERR_NOT_DATE || iRet == ERR_FILE_FORMAT)
				{
					return iRet;
				}

				if (iRet < 0 )
				{
					DispUnpackErrMsg(iRet);
				}
			}

			iNodeType = GetNodeType(pszFirNodeName,iFNodeNamelen);
			giStackNodeIndex++;
			gszStackNodeType[giStackNodeIndex] = iNodeType;  // 进栈

			iNextIndex = FindFirstNode(psTemp+iFirstIndex,iUnpackDatelen-iFirstIndex,&pszNextNodeName,(uint*)&iNextNodeNamelen);
			if (iNextIndex == ERR_NOT_DATE || iNextIndex == ERR_FILE_FORMAT)
			{
				return iNextIndex;
			}
			if (iNextIndex == ERR_NOT_FIND)  // 如果没有后继段则数据全部属于该段，处理完成后退出
			{
				iCurDatelen = iUnpackDatelen;
				iPackDatelen += iCurDatelen;
			}
			else                     // 后继能找到段则，分析处理的是当前段后后继段之间的数据并对处理的数据加总
			{
				if (*pszFirNodeName != '\\' && *pszNextNodeName == '\\')  
				{// 如果第一个段是起始段标志，第二个段是段的结束标志，则两个段名肯定相同，不同则返回文件格式错误
					if (memcmp(pszNextNodeName+1,pszFirNodeName,iFNodeNamelen) !=0 )
					{
						return ERR_FILE_FORMAT;
					}
				}
				iCurDatelen = (pszNextNodeName-1) - (psTemp+iFirstIndex);  // 段名前关键字"<"，减去上一段名所在行的终点
				iPackDatelen += pszNextNodeName - psTemp -1;               // 考虑到iFirstIndex的非零长度，即第一个段的起始点到起始指针的说明，解释数据
			}

			if (FindOneChar(psTemp+iFirstIndex,iCurDatelen,'=')!=ERR_NOT_FIND)
			{ 
				iNodeType = gszStackNodeType[giStackNodeIndex];
				if (iNodeType ==0) 
					return 0;  
				
				switch(iNodeType)
				{
				case EDC_NODE:
					iRet = UnPackEdcParam(psTemp+iFirstIndex,iCurDatelen);
					break;
				case ACQ_NODE:
					iRet = UnPackAcqParam(psTemp+iFirstIndex,iCurDatelen,TRUE);
					break;
				case ISSURE_NODE:
					iRet = UnPackIssureParam(psTemp+iFirstIndex,iCurDatelen,TRUE);
					break;
				case CARD_NODE:
					iRet = UnPackCardParam(psTemp+iFirstIndex,iCurDatelen,TRUE);
					break;
				case EMVAID_NODE:
					iRet = UnpackEmvAidParam(psTemp+iFirstIndex,iCurDatelen,TRUE);
					break;
				case EMVCAPK_NODE:
					iRet = UnpackEmvCapkParam(psTemp+iFirstIndex,iCurDatelen,TRUE);
				break;
				default:
					break;
				}

				if (iRet == ERR_NOT_DATE || iRet == ERR_FILE_FORMAT)
				{
					return iRet;
				}
				if (iRet < 0 )
				{
					DispUnpackErrMsg(iRet);
				}
			}

			continue;
		}
	}

	return 0;
}


int  UnPackEdcParam(char *psEdcParam, uint uiLen)
{
	int  iIndex,iValueNameLen,iValueLen;
	uint uiPacklen;
	char  *pTemp,*pValueName,*pValue;

	uiPacklen = 0;
	while(uiPacklen<uiLen)
	{
		pTemp = psEdcParam+uiPacklen;
		iIndex = FilterHeadChars(pTemp,uiLen-uiPacklen);
		if ((iIndex == ERR_NOT_DATE) || (iIndex >= uiLen-uiPacklen))  //所有数据虑掉就不需要下面的分析 
		{
			return 0;
		}

		pTemp += iIndex;
		uiPacklen += iIndex;

		iIndex = GetVarNameAndVaule(pTemp,uiLen-uiPacklen,&pValueName,(uint*)&iValueNameLen,&pValue,(uint*)&iValueLen);
		if (iIndex <0 )
			return iIndex;

		uiPacklen += iIndex;

		if (iValueNameLen<=0 || iValueLen <=0)
		{
			continue;
		}
		SetVarValue(stEDCConfg,pValueName,iValueNameLen,pValue,iValueLen);
	}

	return 0;
}


int  CheckEdcParam(void)   // 检查EDC参数的完整性
{
	int   iRet;
	char  sBuffer[16];

	if (memcmp(sZmk,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16) == 0)
	{
		lcdClrLine(2,7);
		lcdDisplay(0,3,DISP_ASCII|DISP_MEDIACY,"Pls download ZKey");
		PubBeepErr();
		PubWaitKey(5);
		return 0;
	}

	iRet = PedWriteMasterKey(TDEA_NONE,ZMK_KEY_ID,ZMK_KEY_ID,16,(uint8_t *)sZmk);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存ZMK密钥错误", "SAVE ZMK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return 0;
	}

	memset(sBuffer,0,sizeof(sBuffer));
	if (memcmp(sTmk,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16) == 0)
	{
		lcdClrLine(2,7);
		lcdDisplay(0,3,DISP_ASCII|DISP_MEDIACY,"Pls download");
		lcdDisplay(0,3,DISP_ASCII|DISP_MEDIACY,"TMK key");
		PubBeepErr();
		PubWaitKey(5);
		return 0;
	}

	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sTmk,(uint8_t *)sBuffer);
	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sTmk+8,(uint8_t *)sBuffer+8);	
	PubXor(sBuffer+8,sTmk,8);
	iRet = PedWriteMasterKey(TDEA_NONE,MASTER_KEY_ID,MASTER_KEY_ID,16,(uint8_t *)sBuffer);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存PKEY密钥错误", "SAVE TMK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return 0;
	}

	memset(sBuffer,0,sizeof(sBuffer));
	if (memcmp(sTak,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",16) == 0)
	{
		lcdClrLine(2,7);
		lcdDisplay(0,3,DISP_ASCII|DISP_MEDIACY,"Pls download");
		lcdDisplay(0,3,DISP_ASCII|DISP_MEDIACY,"TAK key");
		PubBeepErr();
		PubWaitKey(5);
		return 0;
	}

	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sTak,(uint8_t *)sBuffer);
	PedTDEA(TDEA_DECRYPT,TYPE_MASTERKEY,ZMK_KEY_ID,8,(uint8_t *)sTak+8,(uint8_t *)sBuffer+8);	
	PubXor(sBuffer+8,sTak,8);
	iRet = PedWriteMasterKey(TDEA_NONE,TAK_KEY_ID,TAK_KEY_ID,16,(uint8_t *)sBuffer);
	if (iRet!=PED_RET_OK)
	{
		lcdClrLine(2,7);
		Display2Strings("保存PKEY密钥错误", "SAVE TAK ERR");
		lcdDisplay(0,7,DISP_ASCII,"ERR CODE: %d",iRet);
		PubBeepErr();
		PubWaitKey(5);
		return 0;
	}

	return 0;
}


int  UnPackAcqParam(char *psAcqParam, uint uilen, char ChFlag)
{
	int  iRet, iIndex,iValueNameLen,iValueLen,iIssureKey;
	unsigned int uiPacklen;
	char  *pTemp,*pValueName,*pValue;
	
	if (ChFlag == 1)
	{
		if (gstSysParam.uiAcqNum+1 >= MAX_ACQ)
		{
			return 0; 
		}

		memset(&gstCurAcq,0,sizeof(gstCurAcq));
		for(iIssureKey=0; iIssureKey<MAX_ISSUER; iIssureKey++)
		{
			gstCurAcq.sIssuerKey[iIssureKey] = INV_ISSUER_KEY;	 
		}
		gstSysParam.uiAcqNum++;
		gstCurAcq.ucKey = gstSysParam.uiAcqNum;
		gstCurAcq.ucIndex = gstSysParam.uiAcqNum-1;
	}
	else
	{
		memcpy(&gstCurAcq,&gstSysParam.stAcqList[gstSysParam.uiAcqNum-1],sizeof(gstCurAcq));
	}

	iRet = 0;
	uiPacklen = 0;
	while(uiPacklen<uilen)
	{
		pTemp = psAcqParam+uiPacklen;
		iIndex = FilterHeadChars(pTemp,uilen-uiPacklen);
		if ((iIndex == ERR_NOT_DATE) || (iIndex >= uilen-uiPacklen))  //所有数据虑掉就不需要下面的分析 
		{
			iRet = 0;
			break;
		}

		pTemp += iIndex;
		uiPacklen += iIndex;
		
		iIndex = GetVarNameAndVaule(pTemp,uilen-uiPacklen,&pValueName,(uint*)&iValueNameLen,&pValue,(uint*)&iValueLen);
		if (iIndex <0 )
		{
			iRet = iIndex;
			break;
		}
		
		uiPacklen += iIndex;

		if (iValueNameLen<=0 || iValueLen <=0)
		{
			continue;
		}
		SetVarValue(stACQConfg,pValueName,iValueNameLen,pValue,iValueLen);
	}
	
	memcpy(&gstSysParam.stAcqList[gstSysParam.uiAcqNum-1],&gstCurAcq,sizeof(gstCurAcq));
	return iRet;
}

int  CheckAcqParam(void)   // 检查ACQ参数的完整性
{
	int    iIssureKey;

	for(iIssureKey=0; iIssureKey<MAX_ISSUER; iIssureKey++)
	{
		if (gstCurAcq.sIssuerKey[iIssureKey] != INV_ISSUER_KEY)
		{
			break;
		}		 
	}
	if (iIssureKey >= MAX_ISSUER)
		return ERR_ACQ_ISSURE;  

	if (strlen(gstCurAcq.szName) ==0)
		return ERR_ACQ_NAME;

	if (strlen(gstCurAcq.szTermID)!=8)
		return ERR_ACQ_TID;

	if (strlen(gstCurAcq.szMerchantID) != 15)
		return ERR_ACQ_MID;

	switch (gstCurAcq.stTxnCommCfg.ucCommType)
	{
	case 'G':
	case 'g':
		gstCurAcq.stTxnCommCfg.ucCommType = CT_GPRS;
		break;
	case 'C':
	case 'c':
		gstCurAcq.stTxnCommCfg.ucCommType = CT_CDMA;
		break;
	case 't':
	case 'T':
		gstCurAcq.stTxnCommCfg.ucCommType = CT_TCPIP;
		break;
	case 'r':
	case 'R':
		gstCurAcq.stTxnCommCfg.ucCommType = CT_RS232;
		break;
	case 'w':
	case 'W':
		gstCurAcq.stTxnCommCfg.ucCommType = CT_WIFI;
		break;
	case 'm':
	case 'M':
		gstCurAcq.stTxnCommCfg.ucCommType = CT_MODEM;
		break;
	default:
		gstCurAcq.stTxnCommCfg.ucCommType = gstPosCapability.uiCommType;
		gstCurAcq.stTxnCommCfg.ucCommType = gstCurAcq.stTxnCommCfg.ucCommType & ~CT_RS232;
		break;
	}
	
	if (!(gstCurAcq.stTxnCommCfg.ucCommType & gstPosCapability.uiCommType))
	{
		gstCurAcq.stTxnCommCfg.ucCommType = gstPosCapability.uiCommType;
		gstCurAcq.stTxnCommCfg.ucCommType = gstCurAcq.stTxnCommCfg.ucCommType & ~CT_RS232;
	}

	gstCurAcq.stTxnCommCfg.stPSTNPara.ucSendMode = MODEM_COMM_SYNC;
    gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.connect_mode = MODEM_COMM_SYNC;
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.connect_speed = 1200;
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_mode = MODEM_DAIL_DTMF;
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_pause = 1;
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.dial_timeo = 30;
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.extension_chk = 1;
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.region = ModemRegion(Malaysia);
	gstCurAcq.stTxnCommCfg.stPSTNPara.stDialPara.idle_timeo = 60;
	
	gstCurAcq.stTxnCommCfg.stWIFIIpPara  = gstCurAcq.stTxnCommCfg.stTcpIpPara;
	memcpy(&gstSysParam.stAcqList[gstSysParam.uiAcqNum-1],&gstCurAcq,sizeof(gstCurAcq));
	return 0;
}


int  UnPackIssureParam(char *psIssureParam, uint uilen, char ChFlag)
{
	int  iRet, iIndex,iValueNameLen,iValueLen;
	unsigned int uiPacklen;
	char  *pTemp,*pValueName,*pValue;
	
	if (ChFlag == 1)
	{
		if (gstSysParam.uiIssuerNum + 1 >= MAX_ISSUER)
		{
			return 0;
		}

		memset(&gstCurIssuer,0,sizeof(gstCurIssuer));
		gstSysParam.uiIssuerNum++;
		gstCurIssuer.ucKey = gstSysParam.uiIssuerNum;
		for (iIndex = 0; iIndex < MAX_ISSUER; iIndex++)
		{
			if (gstSysParam.stAcqList[gstSysParam.uiAcqNum-1].sIssuerKey[iIndex]==INV_ISSUER_KEY)
			{
				gstSysParam.stAcqList[gstSysParam.uiAcqNum-1].sIssuerKey[iIndex] = gstCurIssuer.ucKey;
				break;
			}
		}
	}
	else
	{
		memcpy(&gstCurIssuer,&gstSysParam.stIssuerList[gstSysParam.uiIssuerNum-1],sizeof(gstCurIssuer));
	}
	
	iRet = 0;
	uiPacklen = 0;
	while(uiPacklen<uilen)
	{
		pTemp = psIssureParam+uiPacklen;
		iIndex = FilterHeadChars(pTemp,uilen-uiPacklen);
		if ((iIndex == ERR_NOT_DATE) || (iIndex >= uilen-uiPacklen))  //所有数据虑掉就不需要下面的分析 
		{
			iRet = 0 ;
			break;
		}

		pTemp += iIndex;
		uiPacklen += iIndex;
		
		iIndex = GetVarNameAndVaule(pTemp,uilen-uiPacklen,&pValueName,(uint*)&iValueNameLen,&pValue,(uint*)&iValueLen);
		if (iIndex <0 )
		{
			iRet = iIndex;
			break;
		}
		
		uiPacklen += iIndex;

		if (iValueNameLen<=0 || iValueLen <=0)
		{
			continue;
		}
		SetVarValue(stIssureConfg,pValueName,iValueNameLen,pValue,iValueLen);
	}
	
	memcpy(&gstSysParam.stIssuerList[gstSysParam.uiIssuerNum-1],&gstCurIssuer,sizeof(gstCurIssuer));
	return 0;
}


int  CheckIssureParam(void)   // 检查ACQ参数的完整性
{
	uint   iCardIndex;

	for (iCardIndex=0;iCardIndex<MAX_CARD;iCardIndex++)
	{
		if (gstCurIssuer.ucKey == gstSysParam.stCardTable[iCardIndex].ucIssuerKey)
		{
			break;
		}
	}
	if (iCardIndex>=MAX_CARD)
		return ERR_ISSURE_CARD;

	if (strlen(gstCurIssuer.szName) <= 0)
	{
		return ERR_ISSURE_NAME;
	}

	return 0;
}


int  UnPackCardParam(char *psCardParam, uint uilen, char ChFlag)
{
	int  iRet, iIndex,iValueNameLen,iValueLen;
	unsigned int uiPacklen;
	char  *pTemp,*pValueName,*pValue;
		
	if (ChFlag == 1)
	{
		if (gstSysParam.uiCardNum + 1 >= MAX_CARD)
		{
			return 0;
		}

		memset(&gstCurCard,0,sizeof(gstCurCard));
		gstSysParam.uiCardNum++;
		gstCurCard.ucAcqKey = gstSysParam.stAcqList[gstSysParam.uiAcqNum -1].ucKey ;
		gstCurCard.ucIssuerKey = gstSysParam.stIssuerList[gstSysParam.uiIssuerNum -1].ucKey ;
	}
	else
	{
		memcpy(&gstCurCard,&gstSysParam.stCardTable[gstSysParam.uiCardNum-1],sizeof(gstCurCard));
	}

	iRet = 0;
	uiPacklen = 0;
	while(uiPacklen<uilen)
	{
		pTemp = psCardParam+uiPacklen;
		iIndex = FilterHeadChars(pTemp,uilen-uiPacklen);
		if ((iIndex == ERR_NOT_DATE) || (iIndex >= uilen-uiPacklen))  //所有数据虑掉就不需要下面的分析 
		{
			iRet = 0;
			break;
		}
		
		pTemp += iIndex;
		uiPacklen += iIndex;
		
		iIndex = GetVarNameAndVaule(pTemp,uilen-uiPacklen,&pValueName,(uint*)&iValueNameLen,&pValue,(uint*)&iValueLen);
		if (iIndex <0 )
		{
			iRet =  iIndex;
			break;
		}
		
		uiPacklen += iIndex;
		
		if (iValueNameLen<=0 || iValueLen <=0)
		{
			continue;
		}
		SetVarValue(stCardConfg,pValueName,iValueNameLen,pValue,iValueLen);
	}
	
	memcpy(&gstSysParam.stCardTable[gstSysParam.uiCardNum-1],&gstCurCard,sizeof(gstCurCard));
	return 0;
}


int  CheckCardParam(void)   // 检查CARD参数的完整性 
{
	if (memcmp(gstCurCard.sPanRangeLow,"\x00\x00\x00\x00\x00",5) == 0  &&
		memcmp(gstCurCard.sPanRangeHigh,"\x00\x00\x00\x00\x00",5) == 0 )
	{
		return ERR_CARD_RANGE;
	}

	if (memcmp(gstCurCard.sPanRangeLow,gstCurCard.sPanRangeHigh,5) >=0 )
	{
		return ERR_CARD_RANGE;
	}

	return 0;
}


// 解包EMV AID 参数
int  UnpackEmvAidParam(char *psEmvAidParam, uint uilen, char ChFlag)
{
	int  iRet, iIndex,iValueNameLen,iValueLen;
	unsigned int uiPacklen;
	char  *pTemp,*pValueName,*pValue;
	
	if (ChFlag == 1)
	{		
		memset((char*)&stParamAid,0,sizeof(stParamAid));
	}
	
	iRet = 0;
	uiPacklen = 0;
	while(uiPacklen<uilen)
	{
		pTemp = psEmvAidParam+uiPacklen;
		iIndex = FilterHeadChars(pTemp,uilen-uiPacklen);
		if ((iIndex == ERR_NOT_DATE) || (iIndex >= uilen-uiPacklen))  //所有数据虑掉就不需要下面的分析 
		{
			iRet = 0;
			break;
		}
		
		pTemp += iIndex;
		uiPacklen += iIndex;
		
		iIndex = GetVarNameAndVaule(pTemp,uilen-uiPacklen,&pValueName,(uint*)&iValueNameLen,&pValue,(uint*)&iValueLen);
		if (iIndex <0 )
		{
			iRet =  iIndex;
			break;
		}
		
		uiPacklen += iIndex;
		
		if (iValueNameLen<=0 || iValueLen <=0)
		{
			continue;
		}
		SetVarValue(stEmvAidConfg,pValueName,iValueNameLen,pValue,iValueLen);
	}
	
	return 0;
}

int  CheckEmvAidParam(void)
{
	char  szbuffer[32];

	if (!(stParamAid.aid_len>0 && stParamAid.aid_len<=16))
	{
		return ERR_EMV_AIDLEN;
	}

	memset(szbuffer,0,sizeof(szbuffer));
	if (memcmp(stParamAid.aid,szbuffer,16)==0)
	{
		return ERR_EMV_AID;
	}

	if (emv_add_one_aid_info(&stParamAid)!=EMV_RET_SUCCESS)
	{
		return ERR_EMV_SAVEAID;
	}

	SaveAIDLogFile(&stParamAid);

	return 0;
}

// 解包EMV AID 参数
int  UnpackEmvCapkParam(char *psEmvCapkParam, uint uilen, char ChFlag)
{
	int  iRet, iIndex,iValueNameLen,iValueLen;
	unsigned int uiPacklen;
	char  *pTemp,*pValueName,*pValue;
	
	if (ChFlag == 1)
	{		
		memset((char*)&stParamCapk,0,sizeof(stParamCapk));
	}
	
	iRet = 0;
	uiPacklen = 0;
	while(uiPacklen<uilen)
	{
		pTemp = psEmvCapkParam+uiPacklen;
		iIndex = FilterHeadChars(pTemp,uilen-uiPacklen);
		if ((iIndex == ERR_NOT_DATE) || (iIndex >= uilen-uiPacklen))  //所有数据虑掉就不需要下面的分析 
		{
			iRet = 0;
			break;
		}
		
		pTemp += iIndex;
		uiPacklen += iIndex;
		
		iIndex = GetVarNameAndVaule(pTemp,uilen-uiPacklen,&pValueName,(uint*)&iValueNameLen,&pValue,(uint*)&iValueLen);
		if (iIndex <0 )
		{
			iRet =  iIndex;
			break;
		}
		
		uiPacklen += iIndex;
		
		if (iValueNameLen<=0 || iValueLen <=0)
		{
			continue;
		}
		SetVarValue(stEmvCapkConfg,pValueName,iValueNameLen,pValue,iValueLen);
	}
	
	return 0;
}

int  CheckEmvCapkParam(void)
{
	char  szbuffer[32];
	int   iRet;
	static  int  sgiCnt = 0;

	if (stParamCapk.Index==0)
	{
		return ERR_EMV_INDEX;
	}
	
	memset(szbuffer,0,sizeof(szbuffer));
	if (memcmp(stParamCapk.RID,szbuffer,5)==0)
	{
		return ERR_EMV_RID;
	}

	if (stParamCapk.len_of_modulus==0 || stParamCapk.len_of_modulus > 248)
	{
		return ERR_EMV_MODLEN;
	}

	if (stParamCapk.len_of_exponent==0)
	{
		return ERR_EMV_EXPLEN;
	}

	if (memcmp(stParamCapk.expiration_date,szbuffer,3)==0)
	{
		return ERR_EMV_EXPDATA;
	}

	sgiCnt++;
	iRet = emv_add_one_ca_public_key(&stParamCapk);
	if (iRet!=EMV_RET_SUCCESS)
	{
		lcdClrLine(2,7);
		lcdDisplay(0,2,DISP_CFONT,"CAPK ERR : %d",iRet);
		lcdDisplay(0,4,DISP_CFONT,"INDEX:%d [%d]",stParamCapk.Index,sgiCnt);
		PubBcd2Asc0((char *)stParamCapk.RID,5,szbuffer);
		lcdDisplay(0,6,DISP_CFONT,szbuffer);
		PubWaitKey(10);
		return ERR_EMV_SAVECAPK;
	}

	SaveCAPKLogFile(&stParamCapk);

	return 0;
}


int  SetVarValue(ST_PARAM_FILE *pstNodeCfg,char*psVarName,uint uiNameLen,char*psValue,uint uiValueLen)
{
	uint  uiCnt,uilen,iReValLen,index,iBit;
	unsigned long ulTempVal;
	ST_PARAM_FILE  *pstTemp;
	char  *pTemp,szName[100+1];
	unsigned char  ucTemp;

	if (uiNameLen<=0 || uiValueLen<=0)
		return 0;

	memset(szName,0,sizeof(szName));
	uiCnt = 0;
	pstTemp = &pstNodeCfg[uiCnt];
	while(pstTemp->pszName != NULL)
	{
		uilen = strlen(pstTemp->pszName);
		memcpy(szName,psVarName,uiNameLen);
		PubStrUpper(szName);   //   参数大小写不敏感
		if (memcmp(pstTemp->pszName,szName,uilen)==0 && uiNameLen==uilen)
		{
			iReValLen = pstTemp->iValueLen;
			switch (pstTemp->iAttr)
			{
			case ATTR_SBIT:
				//  先去掉空格和其他数据，其他数据暂时不加。
				index = uiValueLen;
				for(uiCnt=0; uiCnt<index;uiCnt++)
				{
					if(*psValue==' ')
					{
						psValue++;
						uiValueLen --;
					}
					else
						break;
				}
				//  去掉后面的非0-1数据
				pTemp = psValue+uiValueLen-1;
				for (uiCnt= uiValueLen; uiCnt>=0;uiCnt--)
				{
					if ( (*pTemp <'0') ||  (*pTemp > '1') )
					{
						uiValueLen--;
					}
					else
					{
						break;
					}
					pTemp--;
				}

				if (uiValueLen<=0)
				{
					return 0;
				}
				pTemp = pstTemp->pValue;
				memset(pTemp,0, (pstTemp->iValueLen)/8);
				if (uiValueLen > iReValLen)
				{
					uiValueLen = iReValLen;
				}
				for(uiCnt=0; uiCnt<uiValueLen;uiCnt++)
				{
					ucTemp = 0x80;
					if(*psValue=='1')
					{
						iBit = uiCnt%8;
						index = uiCnt/8;
						ucTemp = ucTemp >> iBit;
						pTemp[index] |= ucTemp;
					}
					psValue++;
				}
				break;
			case ATTR_BIT:
				//  先去掉空格和其他数据，其他数据暂时不加。
				index = uiValueLen;
				for(uiCnt=0; uiCnt<index;uiCnt++)
				{
					if(*psValue==' ')
						psValue++;
					else
						break;
				}
				if(uiCnt >= uiValueLen)
				{
					return 0;
				}
				if (*psValue == '1')
				{
					iReValLen = pstTemp->iValueLen;
					ucTemp = iReValLen % 0x0100;
					uiCnt = iReValLen / 0x0100;
					pTemp = pstTemp->pValue;
					pTemp[uiCnt] |= ucTemp;
				}
				else
				{
					if (*psValue == '0')
					{
						iReValLen = pstTemp->iValueLen;
						ucTemp = iReValLen % 0x0100;
						uiCnt = iReValLen / 0x0100;
						pTemp = pstTemp->pValue;
						pTemp[uiCnt] &= ~ucTemp;
					}
				}
				break;
			case ATTR_NLONG:
				//  先去掉空格和其他数据，其他数据暂时不加。
				index = uiValueLen;
				for(uiCnt=0; uiCnt<index;uiCnt++)
				{
					if(*psValue==' ')
					{
						psValue++;
						uiValueLen --;
					}
					else
						break;
				}
				//  去掉后面的非数字数据
				pTemp = psValue+uiValueLen-1;
				for (uiCnt= uiValueLen; uiCnt>=0;uiCnt--)
				{
					if ( (*pTemp <'0') ||  (*pTemp > '9') )
					{
						uiValueLen--;
					}
					else
					{
						break;
					}
					pTemp--;
				}
				if(uiValueLen<=0)
				{
					return 0;
				}				
				if (uiValueLen<iReValLen)
				{
					iReValLen = uiValueLen;
				}

				PubAsc2Long(psValue,iReValLen,&ulTempVal);
				*((uint*)(pstTemp->pValue)) = (uint)ulTempVal;
				break;

			case ATTR_CLONG:
				//  先去掉空格和其他数据，其他数据暂时不加。
				index = uiValueLen;
				for(uiCnt=0; uiCnt<index;uiCnt++)
				{
					if(*psValue==' ')
					{
						psValue++;
						uiValueLen --;
					}
					else
						break;
				}
				//  去掉后面的非数字数据
				pTemp = psValue+uiValueLen-1;
				for (uiCnt= uiValueLen; uiCnt>=0;uiCnt--)
				{
					if ( (*pTemp <'0') ||  (*pTemp > '9') )
					{
						uiValueLen--;
					}
					else
					{
						break;
					}
					pTemp--;
				}
				if(uiValueLen<=0)
				{
					return 0;
				}				
				if (uiValueLen<iReValLen)
				{
					iReValLen = uiValueLen;
				}
				
				PubAsc2Long(psValue,iReValLen,&ulTempVal);
				*((unsigned char *)(pstTemp->pValue)) = (unsigned char)ulTempVal;
				break;

			case ATTR_BCD:
				//  先去掉空格和其他数据，其他数据暂时不加。
				index = uiValueLen;
				for(uiCnt=0; uiCnt<index;uiCnt++)
				{
					if(*psValue==' ')
					{
						psValue++;
						uiValueLen--;
					}
					else
						break;
				}
				//  去掉后面的非0-F的数据
				pTemp = psValue+uiValueLen-1;
				for (uiCnt= uiValueLen; uiCnt>=0;uiCnt--)
				{
					if (!( ( *pTemp >='0' &&  *pTemp <= '9' ) ||   
						   ( *pTemp >='A' &&  *pTemp <= 'F' ) || 
						    ( *pTemp >='a' &&  *pTemp <= 'f' )  ))
					{
						uiValueLen--;
					}
					else
					{
						break;
					}
					pTemp--;
				}
				if(uiValueLen<=0)
				{
					return 0;
				}
				if (uiValueLen < iReValLen)
				{
					iReValLen = uiValueLen;
				}
				memset(pstTemp->pValue,0,(pstTemp->iValueLen)/2);
				PubAsc2Bcd(psValue,iReValLen,pstTemp->pValue);
				break;
			case ATTR_NSTR:
				//  先去掉空格和其他数据，其他数据暂时不加。
				index = uiValueLen;
				for(uiCnt=0; uiCnt<index;uiCnt++)
				{
					if(*psValue==' ')
					{
						psValue++;
						uiValueLen --;
					}
					else
						break;
				}
				//  去掉后面的非数字数据
				pTemp = psValue+uiValueLen-1;
				for (uiCnt= uiValueLen; uiCnt>=0;uiCnt--)
				{
					if ( (*pTemp <'0') ||  (*pTemp > '9') )
					{
						uiValueLen--;
					}
					else
					{
						break;
					}
					pTemp--;
				}
				if(uiValueLen<=0)
				{
					return 0;
				}				
				if (uiValueLen<iReValLen)
				{
					iReValLen = uiValueLen;
				}
				memset(pstTemp->pValue,0,pstTemp->iValueLen);
				memcpy(pstTemp->pValue,psValue,iReValLen);
				break;
			case ATTR_CSTR:
			default :
				//  先去掉空格和其他数据，其他数据暂时不加。
				index = uiValueLen;
				for(uiCnt=0; uiCnt<index;uiCnt++)
				{
					if(*psValue==' ')
					{
						psValue++;
						uiValueLen --;
					}
					else
						break;
				}
				if (uiValueLen<=0)
				{
					return 0;
				}
				if (uiValueLen<iReValLen)
				{
					iReValLen = uiValueLen;
				}
				memset(pstTemp->pValue,0,pstTemp->iValueLen);
				memcpy(pstTemp->pValue,psValue,iReValLen);
				break;
			}

			return 0;
		}

		uiCnt++;
		pstTemp = &pstNodeCfg[uiCnt];
	}

	return 0;
}

int  ExistParamFile(void)
{
	int    iRet;

	iRet = GetParamfileName();
	if (iRet != 0)
	{
		return FALSE;
	}

	if (fileExist((char *)gszParamFileName)>=0)
	{
		return TRUE;
	}

	return FALSE;
}


void RemoveParamFile(void)
{
	int iRet;

	if (fileExist((char *)gszParamFileName)>=0)
	{
		iRet = fileRemove(gszParamFileName);
		if (iRet != 0)
		{
			PubStrLower(gszParamFileName);
			fileRemove(gszParamFileName);	
		}
	}

	return;
}


int  GetParamFile(void)
{
	int   fd , iFileSizes;
	char  szBuff[FILE_BUFFER+1], *pTemp;
	int  iReadlen,iSpecIndex,iRet,iTrunCatelen,iTotalAsysLen;
	
	memset(gszStackNodeType,0,sizeof(gszStackNodeType));
	giStackNodeIndex = 0;
	memset(sZmk,0,sizeof(sZmk));
	memset(sTmk,0,sizeof(sTmk));
	memset(sTak,0,sizeof(sTak));	

	fd = fileOpen(gszParamFileName,O_RDWR);
	if (fd<0)
	{
		PubTRACE1("open:%s", gszParamFileName);
		PubTRACE1("errno:%d", errno);
		return ERR_OPENFILE;
	}
	iFileSizes = fileSize(gszParamFileName);
	if (iFileSizes <= 2)
	{
		return ERR_FILE_FORMAT;
	}

	iTotalAsysLen = 0;
	iReadlen = 0;
	while (iFileSizes>0)
	{
		memset(szBuff,0,sizeof(szBuff));
		iReadlen = fileRead(fd,szBuff,FILE_BUFFER);
		if (iReadlen < 0)
		{
			fileClose(fd);
			return ERR_READFILE;
		}
		if (iReadlen==0)
		{
			break;
		}
		
		if (iReadlen > 2)
		{
			iTrunCatelen = 0;
			pTemp = szBuff;
			while(iTrunCatelen<iReadlen)
			{	
				iSpecIndex = FindReverOneChar(pTemp,iReadlen - iTrunCatelen,0x0A);
				if (iSpecIndex==ERR_NOT_DATE || iSpecIndex == ERR_NOT_FIND)
				{
					fileClose(fd);
					return ERR_FILE_FORMAT;
				}
				
				memset(pTemp+iSpecIndex+iTrunCatelen,0,iReadlen-iSpecIndex-iTrunCatelen);
				iRet = fileSeek(fd,iTotalAsysLen+iSpecIndex+iTrunCatelen,SEEK_SET);
				iReadlen = iSpecIndex+iTrunCatelen;
				break;				
			}
			iFileSizes -= iReadlen;
		}
		else
		{
			iFileSizes -= iReadlen;
			continue;
		}

		iTotalAsysLen += iReadlen;
		UnPackParamFile(szBuff,iReadlen);
	}
	
	fileClose(fd);
	return  0;
}


void DispUnpackErrMsg(int iErrCode)
{
	char  szerrMsg[16+1];

	GetUnpackErrMsg(iErrCode,szerrMsg);
	lcdCls();
	lcdDisplay(0,0,DISP_CFONT|DISP_INVLINE|DISP_MEDIACY," PARAMETER ERR  ");
	switch (iErrCode & 0xFF00)
	{
	case ERR_ACQ_BASE:
		if (gstSysParam.uiAcqNum == 0 || strlen(gstSysParam.stAcqList[gstSysParam.uiAcqNum-1].szName)<=0)
			lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"ERR ACQ : %d",gstSysParam.uiAcqNum);
		else
			lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"  %s ERR",gstSysParam.stAcqList[gstSysParam.uiAcqNum-1].szName);
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,szerrMsg);
		break;

	case ERR_ISSURE_BASE:
		if (gstSysParam.uiIssuerNum == 0 || strlen(gstSysParam.stIssuerList[gstSysParam.uiIssuerNum-1].szName)<=0)
			lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"ERR ISSURE : %d",gstSysParam.uiIssuerNum);
		else
			lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"  %s ERR",gstSysParam.stIssuerList[gstSysParam.uiIssuerNum-1].szName);
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,szerrMsg);
		break;

	case ERR_CARD_BASE:
		lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY," %d  CARD ERR",gstSysParam.uiCardNum);
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,szerrMsg);
		break;
	case ERR_EMV_BASE:
		lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"EMV ERR");
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,szerrMsg);
		break;
	default:
		lcdDisplay(0,4,DISP_CFONT|DISP_MEDIACY,"UNPACK ERR %d" , iErrCode);
		break;
	}
	PubWaitKey(600);
	PubWaitKey(5);
}

typedef struct _tagUNPACKERR_INFO
{
	int		iErrCode;
	char	szErrMsg[16+1];
}UNPACKERR_INFO;

UNPACKERR_INFO sg_stUnpackErrMsg[] = 
{
	{ERR_ACQ_ISSURE,          "NO ISSURE IN ACQ"},
	{ERR_ACQ_NAME,            "ACQ NAME ERR"},
	{ERR_ACQ_TID,             "NO TID IN ACQ"},
	{ERR_ACQ_MID,             "NO MID IN ACQ"},
	{ERR_ACQ_COMM,            "COMMINICATE ERR"},
	{ERR_ISSURE_NAME,         "ISSURE NAME ERR"},
	{ERR_ISSURE_CARD,         "NO CARDTABLE "},
	{ERR_CARD_RANGE,          "CARD RANGE ERR"},
	{0, ""},
};

void GetUnpackErrMsg(int iErrCode,char *psErrMsg)
{
	int		iCnt;
	
	for(iCnt=0; sg_stUnpackErrMsg[iCnt].iErrCode!=0; iCnt++)
	{
		if(sg_stUnpackErrMsg[iCnt].iErrCode == iErrCode &&
			psErrMsg != NULL)
		{
			sprintf(psErrMsg, "%.16s", sg_stUnpackErrMsg[iCnt].szErrMsg);
			break;
		}
	}
}


int  GetParamfileName(void)
{
	int   iRet, iFileNums , iCnt;
	FILE_INFO  stFileInfoList[356];
	char  *pTemp,*pTemp1;

	memset(stFileInfoList,0,sizeof(stFileInfoList));
	iFileNums = fileGetInfo(stFileInfoList);
	

	if (iFileNums <= 0)
	{
		return iFileNums;
	}

	for (iCnt=0; iCnt<iFileNums; iCnt++)
	{
/*		if (stFileInfoList[iCnt].type != FILE_TYPE_DATA)
		{
			continue;
		}*/

		pTemp = strstr((char*)stFileInfoList[iCnt].name,".TXT");
		pTemp1 = strstr((char*)stFileInfoList[iCnt].name,".txt");
		if (pTemp == NULL && pTemp1 == NULL)
		{
			continue;
		}

		iRet = fileOpen((char*)stFileInfoList[iCnt].name,O_RDWR);
		if (iRet >= 0)
		{
			fileClose(iRet);
			memset(gszParamFileName,0,sizeof(gszParamFileName));
			memcpy(gszParamFileName,stFileInfoList[iCnt].name,sizeof(stFileInfoList[iCnt].name));
			return 0;
		}
		fileClose(iRet);
	}

	return ERR_NOT_FILE;
}


