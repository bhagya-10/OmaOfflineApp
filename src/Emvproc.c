
#include "global.h"

/********************** Internal macros declaration ************************/
// macros for analyze EMV TLV string
#define TAGMASK_CLASS			0xC0	// tag mask of tag class
#define TAGMASK_CONSTRUCTED		0x20	// tag mask of constructed/primitive data
#define TAGMASK_FIRSTBYTE		0x1F	// tag mask of first byte
#define TAGMASK_NEXTBYTE		0x80	// tag mask of next byte

#define LENMASK_NEXTBYTE		0x80	// length mask
#define LENMASK_LENBYTES		0x7F	// mask of bytes of lenght

#define TAG_NULL_1				0x00	// null tag
#define TAG_NULL_2				0xFF	// null tag

#define DE55_LEN_FIXED		0x01	// for amex
#define DE55_LEN_VAR1		0x02
#define DE55_LEN_VAR2		0x03

#define DE55_MUST_SET		0x10	// 必须存在
#define DE55_OPT_SET		0x20	// 可选择存在
#define DE55_COND_SET		0x30	// 根据条件存在
/********************** Internal structure declaration *********************/
// callback function for GetTLVItem() to save TLV value
typedef void (*SaveTLVData)(uint uiTag, UINT8 *psData, INT32 iDataLen);
/********************** Internal functions declaration *********************/
static int    SetAmexDE55(DE55Tag *pstList, UINT8 *psOutData, int *piOutLen);
static INT32  SetStdDE55(UINT8 bForUpLoad, DE55Tag *pstList, UINT8 *psOutData, INT32 *piOutLen);
static INT32  SetStdDE56(DE55Tag *pstList, UINT8 *psOutData, INT32 *piOutLen);
static INT32  GetTLVItem(UINT8 **ppsTLVString, INT32 iMaxLen, SaveTLVData pfSaveData, UINT8 bExpandAll);
static INT32  IsConstructedTag(uint uiTag);
static void SaveRspICCData(uint uiTag, UINT8 *psData, INT32 iDataLen);
static void BuildTLVString(UINT32 uiEmvTag, UINT8 *psData, INT32 iLength, UINT8 **ppsOutData);
static void SaveEmvData(void);
static void AdjustIssuerScript(void);
static void SaveScriptData(uint uiTag, UINT8 *psData, INT32 iDataLen);
static void PackTLVData(uint uiTag, UINT8 *psData, uint uiDataLen, UINT8 *psOutData, INT32 *piOutLen);
static void PackTLVHead(uint uiTag, uint uiDataLen, UINT8 *psOutData, INT32 *piOutLen);
static INT32  CalcTLVTotalLen(uint uiTag, uint uiDataLen);
static void PackScriptData(void);

/********************** Internal variables declaration *********************/
static UINT8 sAuthData[16];			// 发卡方认证数据
static UINT8 sIssuerScript[300];	// 发卡方脚本
// { // for test only
// 	0x71, 0x12+0x0F, 0x9F, 0x18, 0x00, 0x86, 0x0D, 0x84, 0x1E, 0x00, 0x00, 0x08,
// 	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
// 	0x86, 0x0D, 0x84, 0x1E, 0x00, 0x00, 0x08,
// 	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
// 	0x72, 0x12+4, 0x9F, 0x18, 0x04, 0,1,2,3, 0x86, 0x0D, 0x84, 0x1E, 0x00, 0x00, 0x08,
// 	0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
// };
//static INT32 iScriptLen=40+15+4;

struct emv_core_init              gstEmvCoreParam;
struct terminal_mck_configure     gstPosEmvConfig;
struct emv_callback_func          gstEMVCallBackFunc;  // 定义的回调函数指针
EMV_STATUS		     gstEmvStatus;
int                  gIsFirstSelectApp ;  // 是否是第一次选择应用
int                  sgiSaveTVRFlag;      // 是否保存过TAC

static INT32			sgAuthDataLen, sgScriptLen;
static ScriptInfo	sgScriptInfo;
static UINT8		sgScriptBak[300];
static INT32			sgCurScript, bHaveScript, sgScriptBakLen;

// AMEX format bit 55
static DE55Tag sgAmexTagList[] =
{
	{0x9F26, DE55_LEN_FIXED, 8},
	{0x9F10, DE55_LEN_VAR1,  32},
	{0x9F37, DE55_LEN_FIXED, 4},
	{0x9F36, DE55_LEN_FIXED, 2},
	{0x95,   DE55_LEN_FIXED, 5},
	{0x9A,   DE55_LEN_FIXED, 3},
	{0x9C,   DE55_LEN_FIXED, 1},
	{0x9F02, DE55_LEN_FIXED, 6},
	{0x5F2A, DE55_LEN_FIXED, 2},
	{0x9F1A, DE55_LEN_FIXED, 2},
	{0x82,   DE55_LEN_FIXED, 2},
	{0x9F03, DE55_LEN_FIXED, 6},
	{0x5F34, DE55_LEN_FIXED, 1},
	{0x9F27, DE55_LEN_FIXED, 1},
	{0x9F06, DE55_LEN_VAR1,  16},
	{0x9F09, DE55_LEN_FIXED, 2},
	{0x9F34, DE55_LEN_FIXED, 3},
	{0x9F0E, DE55_LEN_FIXED, 5},
	{0x9F0F, DE55_LEN_FIXED, 5},
	{0x9F0D, DE55_LEN_FIXED, 5},
	{0},
};

// 消费/(预)授权,55域EMV标签, TLV format
static DE55Tag sgStdEmvTagList[] =
{
	{0x5F2A, DE55_MUST_SET, 0},
	{0x82,   DE55_MUST_SET, 0},
	{0x84,   DE55_MUST_SET, 0},
	{0x95,   DE55_MUST_SET, 0},
	{0x9A,   DE55_MUST_SET, 0},
	{0x9C,   DE55_MUST_SET, 0},
	{0x9F02, DE55_MUST_SET, 0},
	{0x9F03, DE55_MUST_SET, 0},
	{0x9F09, DE55_MUST_SET, 0},
	{0x9F10, DE55_MUST_SET, 0},
	{0x9F1A, DE55_MUST_SET, 0},
	{0x9F1E, DE55_MUST_SET, 0},
	{0x9F26, DE55_MUST_SET, 0},
	{0x9F27, DE55_MUST_SET, 0},
	{0x9F33, DE55_MUST_SET, 0},
	{0x9F34, DE55_MUST_SET, 0},
	{0x9F35, DE55_MUST_SET, 0},
	{0x9F36, DE55_MUST_SET, 0},
	{0x9F37, DE55_MUST_SET, 0},
	{0x9F41, DE55_MUST_SET, 0},
	{0},
};

// 消费/(预)授权,56域EMV标签, TLV format
static DE55Tag sgStdEmvField56TagList[] =
{
	{0x5A,   DE55_MUST_SET, 0},
	{0x95,   DE55_MUST_SET, 0},
	{0x9B,   DE55_MUST_SET, 0},
	{0x9F10, DE55_MUST_SET, 0},
	{0x9F26, DE55_MUST_SET, 0},
	{0x9F27, DE55_MUST_SET, 0},
	{0},
};

#if 0
static DE55Tag stErrorTxnTagList[] =
{
	{0x9F26, DE55_MUST_SET},
	{0x9F27, DE55_MUST_SET},
	{0x9F10, DE55_MUST_SET},
	{0x9F37, DE55_MUST_SET},
	{0x9F36, DE55_MUST_SET},
	{0x95,   DE55_MUST_SET},
	{0x9A,   DE55_MUST_SET}
	{0x9C,   DE55_MUST_SET},
	{0x9F02, DE55_MUST_SET},
	{0x5F2A, DE55_MUST_SET},
	{0x82,   DE55_MUST_SET},
	{0x9F1A, DE55_MUST_SET},
	{0x9F34, DE55_OPT_SET},
	{0x9F03, DE55_OPT_SET},
	{0x9F0E, DE55_OPT_SET},
	{0x9F0F, DE55_OPT_SET},
	{0x9F0D, DE55_OPT_SET},
	{0},
};
#endif

/********************** external reference declaration *********************/
/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/
//显示应用列表，等待持卡人选择
//如果只有一个应用，则EMV内核不会调用该函数
int cEMVSelectApp(void)
{
	INT32			iRet, iCnt;  
	UINT32          iAppNums;
	MenuItem	    stAppMenu[MAX_APP_NUM+1];
	struct candapp  stAppList[MAX_APP_NUM]; 
	uint8_t           szMasterTcc[10];

	iRet = iCnt = 0;
	emv_get_data_element("\x9F\x53",2,(uint8_t*)&iRet,(char*)szMasterTcc);
	if (szMasterTcc[0] != 0x52)
	{
		szMasterTcc[0] = 0x52;
		emv_set_data_element((uint8_t *)"\x9F\x53",2,1,szMasterTcc);
	}
	
	iAppNums = 0;
 	memset((char*)&stAppList[0],0,sizeof(stAppList));
    emv_get_mck_configure(&gstPosEmvConfig);  // 获取MCK配置
    emv_get_all_candapps((uint8_t*)&iAppNums,stAppList); // 获取所有的候选应用信息

	if (iAppNums==0)
	{
		errno = EMV_ERRNO_NO_APP;
		return -1;
	}

	if (iAppNums == 1 && strlen((char *)stAppList[0].tCandAppName)==0)
	{
		emv_get_data_element("\x50",1, (uint8_t*)&iCnt, (char*)gstTransInfo.stTranLog.szAppLabel);	// application label
		if( iRet==EMV_RET_SUCCESS && strlen((char *)gstTransInfo.stTranLog.szAppLabel))
		{
			strcpy((char *)stAppList[0].tCandAppName,(char *)gstTransInfo.stTranLog.szAppLabel);
		}
		else
		{
			iRet = emv_get_data_element("\x4F",1, (uint8_t*)&iCnt, (char*)gstTransInfo.stTranLog.sAID);	// AID
			if (iRet == EMV_RET_SUCCESS)
			{
				PubBcd2Asc0((char *)gstTransInfo.stTranLog.sAID,iCnt,(char *)stAppList[0].tCandAppName);
			}
		}

		if (strlen((char *)stAppList[0].tCandAppName)==0)
		{
			return 0;
		}
	}
	
	if( gIsFirstSelectApp)
	{
		lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		{
			PubDispString("NOT ACCEPT", 3|DISP_LINE_CENTER);
			PubDispString("TRY AGAIN",  5|DISP_LINE_CENTER);
		}
		else
		{
			lcdDisplay(4, 3, DISP_CFONT, "交易拒绝,请重试");
			lcdDisplay(4, 5, DISP_ASCII, "NOT ACCEPT,TRY AGAIN");
		}
		PubWaitKey(3);
	}

	sgiSaveTVRFlag = 1;
	gIsFirstSelectApp = 1;   // 已经选择应用
	PubASSERT( iAppNums<=MAX_APP_NUM );
	memset(stAppMenu, 0, sizeof(stAppMenu));
	for(iCnt=0; iCnt<iAppNums && iCnt<MAX_APP_NUM; iCnt++)
	{
		stAppMenu[iCnt].bVisible = TRUE;
		stAppMenu[iCnt].pfMenuFunc = NULL;
		sprintf((char *)stAppMenu[iCnt].szMenuName, "%.16s", stAppList[iCnt].tCandAppName);
	}

	stAppMenu[iCnt].bVisible = FALSE;
	stAppMenu[iCnt].pfMenuFunc = NULL;
	stAppMenu[iCnt].szMenuName[0] = 0;
	
	sysDelayMs(50);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		iRet = PubGetMenu((UINT8 *)"Select App", stAppMenu, MENU_CFONT, USER_OPER_TIMEOUT);
	else
		iRet = PubGetMenu((UINT8 *)"应用选择",   stAppMenu, MENU_CFONT, USER_OPER_TIMEOUT);
	if( iRet<0 )
	{
		errno = EMV_ERRNO_CANCEL;
		return -1;
	}
	errno = 0;
	return iRet;
}


// 需要时候实现
void cEMVSelectLanguage(void)
{
	sysDelayMs(50);
	return ;
}

//等待用户输入卡密码
//下边是处理代码，应用应该根据实际情况修改
int  cEMVGetOnlinePIN(void)
{
	int		iRet;

	iRet = GetPIN(0x80);
    switch(iRet)
    {
	case PED_RET_ERROR:
	case PED_RET_APPTSSAKERR:
	case PED_RET_LOCKED:
	case PED_RET_NEEDWAIT:
	case PED_RET_PEDTSSAKERR:
		errno = EMV_ERRNO_DATA;
		iRet = -1;
		break;
	case PED_RET_NOPIN:
		errno = EMV_ERRNO_BYPASS_PIN;
		iRet = -1;
		break;
	case PED_RET_CANCEL:
	case PED_RET_TIMEOUT:
		errno = EMV_RET_CANCEL;
		iRet = -1;
		break;
	case PED_RET_OK:
		iRet = EMV_RET_SUCCESS;
		break;
	default:
		errno = EMV_ERRNO_DATA;
		iRet = -1;
    }
    return iRet;
}

/*
int cEMVInputEncryptPIN(const R_RSA_PUBLIC_KEY *pstRsaPubKey, UINT8 *psIccRandom)
{
    APDU_SEND stSendApdu;
    APDU_RESP stRecvApdu;
    int  iCnt, iRet = EMV_RET_SUCCESS;
    RSA_PINKEY stRsaPinKey;
    char sRandom[8];
	unsigned char szbuffer[2];
	int  iLength;
	int  iLastTimes, iRemainTimes;

    emvGetMCKConfig(&gstPosEmvConfig);

	iRet = emv_get_data_element("\x9f\x17",2,&iLength,szbuffer);  // 获取密码校验次数
	if (iRet == EMV_RET_SUCCESS)
	{
		if (szbuffer[0]==0x01)
		{
			lcdClrLine(2, 7);
			Display2Strings("只剩下 1 次机会","LAST PIN TRY");
			PubWaitKey(10);			
		}
	}

	iRet = EMV_RET_SUCCESS;
    while (1)
    {
        memset(&stRsaPinKey, 0x00, sizeof(RSA_PINKEY));
        stRsaPinKey.modlen = pstRsaPubKey->bits/8;
        memcpy(stRsaPinKey.mod, pstRsaPubKey->modulus, stRsaPinKey.modlen);
        stRsaPinKey.explen = 4;
        memcpy(stRsaPinKey.exp, pstRsaPubKey->exponent, 4);
        stRsaPinKey.iccrandomlen = 8;
        memcpy(stRsaPinKey.iccrandom, psIccRandom, 8);
        stRsaPinKey.termrandomlen = stRsaPinKey.modlen - 17;
        if((stRsaPinKey.termrandomlen%8) == 0)
        {
            for(iCnt = 0; iCnt < stRsaPinKey.termrandomlen/8; iCnt++)
            {
                PedGetRandom(stRsaPinKey.termrandom + 8*iCnt);
            }
        }
        else
        {
            for(iCnt = 0; iCnt < stRsaPinKey.termrandomlen/8; iCnt++)
            {
                PedGetRandom(stRsaPinKey.termrandom + 8*iCnt);
            }

            PedGetRandom(sRandom);
            memcpy(stRsaPinKey.termrandom + 8*iCnt, sRandom, stRsaPinKey.termrandomlen%8);
        }

		// 循环等待直到允许输入密钥
		iLastTimes = PedGetSensitiveTimer(TYPE_PIN_INTERVAL);
		iLastTimes /= 1000;
		if (iLastTimes)
		{
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Wait a Sec.(%dS)",iLastTimes);
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Wait for Input");
			}
			else
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请 稍 候 (%dS)",iLastTimes);
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"请等待输入");
			}
		}
		while(PedGetSensitiveTimer(TYPE_PIN_INTERVAL))
		{
			iRemainTimes = PedGetSensitiveTimer(TYPE_PIN_INTERVAL);
			iRemainTimes /= 1000;
			if (iRemainTimes < iLastTimes)
			{
				lcdClrLine(2,7);
				if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				{
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Wait a Sec.(%dS)",iLastTimes);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Wait for Input");
				}
				else
				{
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请 稍 候 (%dS)",iLastTimes);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"请等待输入");
				}
				iLastTimes = iRemainTimes;
			}
		}

		lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("Input PWD:",    3|DISP_LINE_LEFT);
		else
			PubDispString("请输入密码:",    3|DISP_LINE_LEFT);
        lcdGoto(28,5);

		stSendApdu.CLA = 0x00;
        stSendApdu.INS = 0x20;
        stSendApdu.P1 = 0x00;
        stSendApdu.P2 = 0x88;

        if(gstPosEmvConfig.cSupBypassPIN)
            iRet = PedGetOffLineCipherPin(ICC_USERCARD,"0,4,5,6,7,8,9,10,11,12",&stRsaPinKey, &stSendApdu,&stRecvApdu);
        else
            iRet = PedGetOffLineCipherPin(ICC_USERCARD,"4,5,6,7,8,9,10,11,12",&stRsaPinKey, &stSendApdu,&stRecvApdu);
        switch(iRet)
        {
            case PED_RET_NOCARD:
            case PED_RET_ICCNOTPWRUP:
                iRet = EMV_RET_ICCERR;
                break;
            case PED_RET_ERROR:
            case PED_RET_APPTSSAKERR:
            case PED_RET_LOCKED:
            case PED_RET_NEEDWAIT:
            case PED_RET_PEDTSSAKERR:
                iRet = EMV_RET_DATAERR;
                break;
            case PED_RET_NOPIN:
                iRet = EMV_RET_BYPASSPIN;
                break;
            case PED_RET_CANCEL:
            case PED_RET_TIMEOUT:
                iRet = EMV_RET_CANCEL;
                break;
            case PED_RET_OK:
                iRet = EMV_RET_SUCCESS;
                break;
            default:
                iRet = EMV_RET_DATAERR;
                break;
        }

		if (iRet != EMV_RET_SUCCESS)
		{
			return iRet;
		}
        if((stRecvApdu.SWA == 0x90) && (stRecvApdu.SWB == 0x00))
        {
            lcdClrLine(2, 7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				PubDispString("Correct pwd",    3|DISP_LINE_LEFT);
			else
				PubDispString(" 密码正确 ",    3|DISP_LINE_LEFT);
            return EMV_RET_SUCCESS;
        }
        else
		{
			if((stRecvApdu.SWA == 0x69) && ((stRecvApdu.SWB == 0x83) || (stRecvApdu.SWB == 0x84)))
				return  EMV_RET_PINBLOCKED;
			
			if((stRecvApdu.SWA == 0x63) && (stRecvApdu.SWB == 0xC0))
				return EMV_RET_PINBLOCKED;
			
			if((stRecvApdu.SWA == 0x63) && ((stRecvApdu.SWB & 0xCF) == 0xC0))
			{
				lcdClrLine(2,7);
				if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				{
					lcdDisplay(0,1,DISP_CFONT|DISP_MEDIACY,"Password error");
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Remain times: %d",stRecvApdu.SWB&0x0F);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Continue reInput?");
				}
				else
				{
					lcdDisplay(0,1,DISP_CFONT|DISP_MEDIACY," 密码错误 ");
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"剩下 %d 次机会",stRecvApdu.SWB&0x0F);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"重新输入?");
				}
				if( !PubYesNo(USER_OPER_TIMEOUT) )
				{
					return ERR_USERCANCEL;
				}
			}
			else
			{
				return EMV_RET_RESPERR;
			}
		}
    }
}*/
/*
//   脱机明文PIN校验
int cEMVInputPlainPIN(void)
{
    APDU_SEND stSendApdu;
    APDU_RESP stRecvApdu;
    int  iRet = EMV_RET_SUCCESS;
	unsigned char szbuffer[2];
	int  iLength;
	int  iLastTimes, iRemainTimes;

    emvGetMCKConfig(&gstPosEmvConfig);

	iRet = emv_get_data_element("\x9f\x17",2,&iLength,szbuffer);  // 获取密码校验次数
	if (iRet == EMV_RET_SUCCESS)
	{
		if (szbuffer[0]==0x01)
		{
			lcdClrLine(2, 7);
			Display2Strings("只剩下 1 次机会","LAST PIN TRY");
			PubWaitKey(10);
		}
	}

	iRet = EMV_RET_SUCCESS;
    while (1)
    {
        stSendApdu.CLA = 0x00;
        stSendApdu.INS = 0x20;
        stSendApdu.P1 = 0x00;
        stSendApdu.P2 = 0x80;

		// 循环等待直到允许输入密钥
		iLastTimes = PedGetSensitiveTimer(TYPE_PIN_INTERVAL);
		iLastTimes /= 1000;
		if (iLastTimes)
		{
			lcdClrLine(2,7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Wait a Sec.(%dS)",iLastTimes);
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Wait for Input");
			}
			else
			{
				lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请 稍 候 (%dS)",iLastTimes);
				lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"请等待输入");
			}
		}
		while(PedGetSensitiveTimer(TYPE_PIN_INTERVAL))
		{
			iRemainTimes = PedGetSensitiveTimer(TYPE_PIN_INTERVAL);
			iRemainTimes /= 1000;
			if (iRemainTimes < iLastTimes)
			{
				lcdClrLine(2,7);
				if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				{
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Wait a Sec.(%dS)",iLastTimes);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Wait for Input");
				}
				else
				{
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"请 稍 候 (%dS)",iLastTimes);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"请等待输入");
				}
				iLastTimes = iRemainTimes;
			}
		}

		lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("Input PWD:",    3|DISP_LINE_LEFT);
		else
			PubDispString("请输入密码:",    3|DISP_LINE_LEFT);
        lcdGoto(28,5);

        if(gstPosEmvConfig.cSupBypassPIN)
            iRet = PedGetOffLinePlainPin(ICC_USERCARD,"0,4,5,6,7,8,9,10,11,12",&stSendApdu,&stRecvApdu);
        else
            iRet = PedGetOffLinePlainPin(ICC_USERCARD,"4,5,6,7,8,9,10,11,12",&stSendApdu,&stRecvApdu);

        switch(iRet)
        {
            case PED_RET_NOCARD:
            case PED_RET_ICCNOTPWRUP:
                iRet = EMV_RET_ICCERR;
                break;
            case PED_RET_ERROR:
            case PED_RET_APPTSSAKERR:
            case PED_RET_LOCKED:
            case PED_RET_NEEDWAIT:
            case PED_RET_PEDTSSAKERR:
                iRet = EMV_RET_DATAERR;
                break;
            case PED_RET_NOPIN:
                iRet = EMV_RET_BYPASSPIN;
                break;
            case PED_RET_CANCEL:
            case PED_RET_TIMEOUT:
                iRet = EMV_RET_CANCEL;
                break;
            case PED_RET_OK:
                iRet = EMV_RET_SUCCESS;
                break;
            default:
                iRet = EMV_RET_DATAERR;
                break;
        }

		if (iRet != EMV_RET_SUCCESS)
		{
			return iRet;
		}

        if((stRecvApdu.SWA == 0x90) && (stRecvApdu.SWB == 0x00))
        {
            lcdClrLine(2, 7);
			if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				PubDispString("Correct pwd",    3|DISP_LINE_LEFT);
			else
				PubDispString(" 密码正确 ",    3|DISP_LINE_LEFT);
            return EMV_RET_SUCCESS;
        }
        else
		{
			if((stRecvApdu.SWA == 0x69) && ((stRecvApdu.SWB == 0x83) || (stRecvApdu.SWB == 0x84)))
				return  EMV_RET_PINBLOCKED;

			if((stRecvApdu.SWA == 0x63) && (stRecvApdu.SWB == 0xC0))
				return EMV_RET_PINBLOCKED;

			if((stRecvApdu.SWA == 0x63) && ((stRecvApdu.SWB & 0xCF) == 0xC0))
			{
				lcdClrLine(2,7);
				if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
				{
					lcdDisplay(0,1,DISP_CFONT|DISP_MEDIACY,"Password error");
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Remain times: %d",stRecvApdu.SWB&0x0F);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Continue reInput?");
				}
				else
				{
					lcdDisplay(0,1,DISP_CFONT|DISP_MEDIACY," 密码错误 ");
					lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"剩下 %d 次机会",stRecvApdu.SWB&0x0F);
					lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"重新输入?");
				}
				if( !PubYesNo(USER_OPER_TIMEOUT) )
				{
					return ERR_USERCANCEL;
				}
			}
			else
			{
				return EMV_RET_RESPERR;
			}
		}
    }
} */


int cEMVGetCardholderPin(void *CardPin)
{
 /*   int  iRet ;
	unsigned char szbuffer[20];
	int  iLength;*/

	lcdClrLine(2, 7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
		PubDispString("Input PWD:",    3|DISP_LINE_LEFT);
	else
		PubDispString("请输入密码:",    3|DISP_LINE_LEFT);

	lcdFlip();
	return 0;

/*	memset(szbuffer,0,sizeof(szbuffer));
    if(gstPosEmvConfig.cSupBypassPIN)
        iRet = PubGetString(PASS_IN,0,12,szbuffer,USER_OPER_TIMEOUT);
    else
        iRet = PubGetString(PASS_IN,4,12,szbuffer,USER_OPER_TIMEOUT);
	if (iRet != 0)
	{
		return EMV_RET_CANCEL;
	}

	iLength = strlen((char*)szbuffer);
	if(gstPosEmvConfig.cSupBypassPIN && iLength ==0)
	{
		CardPin[0] = 0;
		return EMV_RET_BYPASSPIN;
	}

	sysDelayMs(50);
	strcpy((char*)CardPin,(char*)szbuffer);
	return iLength;*/
}


int cEMVShowPinVerifyResult(UINT8 TryCounter)
{
	int  iRet;
	
	if(TryCounter==0)
	{
        lcdClrLine(2, 7);
		if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
			PubDispString("Correct pwd",    3|DISP_LINE_CENTER);
		else
			PubDispString(" 密码正确 ",    3|DISP_LINE_CENTER);
		PubWaitKey(1);
        return EMV_RET_SUCCESS;
	}
	
	lcdClrLine(2,7);
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		lcdDisplay(0,1,DISP_CFONT|DISP_MEDIACY,"Password error");
		lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"Remain times: %d",TryCounter);
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"Continue reInput?");
	}
	else
	{
		lcdDisplay(0,1,DISP_CFONT|DISP_MEDIACY," 密码错误 ");
		lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"剩下 %d 次机会",TryCounter);
		lcdDisplay(0,5,DISP_CFONT|DISP_MEDIACY,"重新输入?");
	}

	lcdFlip();
	while(1)
	{
		iRet = kbGetKey();		
		if(iRet == KEY_ENTER)
		{                        
			return EMV_RET_SUCCESS;
		}
		if(iRet == KEY_CANCEL)
		{
			errno = EMV_RET_CANCEL;
			return -1;
		}
	}
	
	errno = EMV_RET_CANCEL;
	return -1;
}


int cEMVGetTransactionAmount(UINT32 *pTransAmount, UINT32 *pOtherAmount)
{
	int 	iRet,iLen;
	char    szCurrName[4+1],szBuff[12+1];
	char    sBcdAmount[12+1],szTotalAmt[12+1];
	
	if( gstTransInfo.stTranLog.szAmount[0]!=0 )
	{
		return EMV_RET_SUCCESS;
	}

	iRet = iLen = 0;
	sysDelayMs(50);
	while( 1 )
	{
		iRet = InputAmount(AMOUNT);
		if( iRet!=0 )
		{
			errno = EMV_RET_CANCEL;
			return -1;
		}
		
		iRet = GetTipAmount();
		if( iRet!=0 )
		{
			errno = EMV_RET_CANCEL;
			return -1;
		}
		
		PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, (char *)gstTransInfo.stTranLog.szTipAmount, 12, szTotalAmt);
		if (pTransAmount != NULL)
		{
			*pTransAmount = PubAsc2Long(szTotalAmt,12,NULL);
		}

/*		if (pOtherAmount != NULL)
		{
			*pOtherAmount = PubAsc2Long(gstTransInfo.stTranLog.szTipAmount,12,NULL);
		}*/
		
		// 输入反现金额
		if (gstPosEmvConfig.terminal_capabilities[0] & 0x10 )
		{
			iRet = emv_get_data_element("\x9C",1,(uint8_t *)&iLen,(char*)szBuff);
			if (iRet)
			{
				errno = EMV_ERRNO_DATA;
				return -1;
			}
			
			if ( (szBuff[0] & EMV_CASHBACK) == EMV_CASHBACK)
			{
				Display2StringHigh("请输入金额", "ENTER AMOUNT ?");			
				sprintf((char *)szCurrName, "%.3s", gstSysParam.stEdcInfo.szCurrencyName);	
				memset(szBuff,0,sizeof(szBuff));
				
				iRet = PubGetAmount((uint8_t *)szCurrName, 2, 0, 10, (uint8_t *)szBuff, USER_OPER_TIMEOUT, 0);
				if( iRet != 0 )
				{
					errno = EMV_RET_CANCEL;
					return -1;
				}
				PubAddHeadChars(szBuff, 12, '0');
			}
			
			PubAscAdd((char *)gstTransInfo.stTranLog.szAmount, szBuff, 12, szTotalAmt);
			PubAsc2Bcd(szTotalAmt,12,sBcdAmount);
			if (pTransAmount != NULL)
			{
				*pTransAmount = PubAsc2Long(szTotalAmt,12,NULL);
			}
		}
		
		break;
	}
	
	return EMV_RET_SUCCESS;
}

INT32 emvCheckCRL(uint8_t index,const void *rid,const void *cert_sn)
{
	errno = EMV_ERRNO_NODATA;
	return -1;
}

int cEMVCheckExceptPAN(uint8_t PANLen, const void *PAN, uint8_t PANSn)
{
	errno = EMV_ERRNO_NODATA;
	return -1;
}

int cEMVGetTransLogAmount(uint8_t PanLen, const void *PAN, uint8_t PanSn)
{
	return 0;
}

/************************************************************************
    通知处理(如果不支持，直接返回就可以了)
	脱机通知的处理：
	通过函数EMVGetTLVData()获得通知数据包需要的数据，存贮到交易日志中，
	然后在交易结算时，再联机发送到主机。
	需要注意的是：通知数据包的任何数据(比如金额等)不可以用于交易对帐。

	联机通知的处理：
	(1)拨号连接主机。
	(2)通过函数EMVGetTLVData()获得通知数据包需要的数据，再发送到主机。
	需要注意的是：联机通知方式在我们的POS机中应该不会使用。
************************************************************************/
int cEMVAdviceProc(INT32 Flg)
{
	return 0;
}

/************************************************************************
  语音参考处理 ,如果不支持，如果是卡片发起的参考，
  则可根据发卡行要求直接返回REFER_DENIAL或REFER_ONLINE,
  一般情况下不应该直接返回REFER_APPROVE(除非发卡行要求这么做)

  如果不支持，如果是发卡行(主机)发起的参考，
  则可根据发卡行要求直接返回REFER_DENIAL
  一般情况下不应该直接返回REFER_APPROVE(除非发卡行要求这么做)
  下边是参考的处理代码，供参考
************************************************************************/
int cEMVReferProc(void)
{
	uint8_t len;
	uint8_t pan[100], buff[200];
	int rc;
	
	memset(pan, 0x00, sizeof(pan));
	memset(buff, 0x00, sizeof(buff));
	len = 0;
	emv_get_data_element("\x5A", 1, &len, buff);
	PubBcd2Asc((char*)buff,(int)len,(char*)pan);
	
	lcdClrLine(2, 7);	
	if(gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN) // if no support chinese
	{
		lcdDisplay(0,2,DISP_CFONT|DISP_MEDIACY,"请电告银行");
		lcdDisplay(0,4,DISP_ASCII,"APP PAN:");
		lcdDisplay(0,5,DISP_ASCII|DISP_MEDIACY,"%s", pan);
		lcdDisplay(0,6,DISP_ASCII|DISP_MEDIACY,"ACCEPT TRANS?");
	}
	else // if support chinese
	{
		lcdDisplay(0,2,DISP_CFONT|DISP_MEDIACY,"CALL YOUR BANK");
		lcdDisplay(0,4,DISP_ASCII,"APP PAN:");
		lcdDisplay(0,5,DISP_ASCII|DISP_MEDIACY,"%s", pan);
		lcdDisplay(0,6,DISP_CFONT|DISP_MEDIACY,"批准交易?");
	}
	
	lcdFlip();
	while(1)
	{
		rc = kbGetKey();
		if(rc == KEY_ENTER)
		{
			return 0;
		}
		
		if(rc == KEY_CANCEL)
		{
			errno = EMV_ERRNO_DECLINE;
			return -1;  // Decline the transaction
		}
	}

	return 0;
}

//  持卡人证件检查
int cEMVCheckCertificate(void)
{
    int   iCertLen, iKey,uiCertType;
    char  szCert[41];

	iCertLen = 0;
	memset(szCert,0,sizeof(szCert));
    if (emv_get_data_element("\x9F\x62", 2, (uint8_t *)&iCertLen, (char*)&uiCertType) != EMV_RET_SUCCESS)
	{
		errno = EMV_ERRNO_DATA;
		return -1;
	}
	
    if (emv_get_data_element("\x9F\x61", 2, (uint8_t *)&iCertLen, (char*)szCert) != EMV_RET_SUCCESS)
	{
		errno = EMV_ERRNO_DATA;
		return -1;
	}
	
    if (iCertLen > 40)
	{
		errno = EMV_ERRNO_DATA;
		return -1;
	}
	
	if (gstSysParam.stEdcInfo.ucLanguage == EDC_LANGUGE_EN)
	{
		PubDisplayTitle(TRUE,"Check certificate");
		switch (uiCertType) 
		{
		case 0x00:	lcdDisplay(0, 2, DISP_CFONT, "identity card:");		break;
		case 0x01:	lcdDisplay(0, 2, DISP_CFONT, "officer ID");			break;
		case 0x02:	lcdDisplay(0, 2, DISP_CFONT, "Passport:");   		break;
		case 0x03:	lcdDisplay(0, 2, DISP_CFONT, "Immigrant ID:");	    break;
		case 0x04:	lcdDisplay(0, 2, DISP_CFONT, "Temp identity:");		break;
		case 0x05:  lcdDisplay(0, 2, DISP_CFONT, "Other:");      	    break;
		}
	}
	else
	{
		PubDisplayTitle(TRUE,"   请核对证件   ");
		switch (uiCertType) 
		{
		case 0x00:	lcdDisplay(0, 2, DISP_CFONT, "身份证:");			break;
		case 0x01:	lcdDisplay(0, 2, DISP_CFONT, "军官证:");			break;
		case 0x02:	lcdDisplay(0, 2, DISP_CFONT, "护照:");   			break;
		case 0x03:	lcdDisplay(0, 2, DISP_CFONT, "入境证:");	        break;
		case 0x04:	lcdDisplay(0, 2, DISP_CFONT, "临时身份证:");		break;
		case 0x05:  lcdDisplay(0, 2, DISP_CFONT, "其它证件:");      	break;
		}
	}
    lcdDisplay(0, 4, DISP_CFONT,szCert);
	iKey = PubYesNo(USER_OPER_TIMEOUT);	
    if (iKey)
        return EMV_RET_SUCCESS;
	
	errno = EMV_ERRNO_DATA;
	return -1;
}


/************************************************************************
    联机处理 , 处理步骤：
	(1)拨号连接主机,如果所有交易都要联机，那么可以在插入IC卡时预拨号,
	   如果拨号失败返回ONLINE_FAILED
	(2)通过函数EMVGetTLVData()获得交易数据包需要的数据，并打包。
	(3)保存冲正数据及标志,然后发送交易数据包到主机(冲正处理完全由应用完成)
	(4)接收主机的回应数据包,根据主机的回应，做如下返回：
	   A.如果主机返回批准，则根据返回数据填写RspCode、AuthCode、AuthCodeLen等
		 参数的值，并返回ONLINE_APPROVE
	   B.如果主机拒绝交易,则根据返回数据填写RspCode,如果其他参数也有数据值，
		 同样需要填写，返回ONLINE_DENIAL
	   C.如果主机请求语音参考,则根据返回数据填写RspCode,如果其他参数也有数据值，
		 同样需要填写，返回ONLINE_REFER。需要说明的是：很多情况可能没有参考处理，
		 在这种情况下，应用程序就不需要返回ONLINE_REFER了

	等交易处理成功后，应用程序才可以清除冲正标志。
************************************************************************/
INT32  cEMVOnlineProc( UINT8 *RspCode, UINT8 *AuthCode, INT32 *AuthCodeLen,
					   UINT8  *IAuthData, INT32 *IAuthDataLen, UINT8 *script, INT32 *ScriptLen, int *online_result)
{
	int	    	iRet, iLength, iRetryPIN;
	ulong	    ulICCDataLen;
	char	    *psICCData, *psTemp; 

	// initialize output parameters
	*RspCode      = 0;
	*AuthCodeLen  = 0;
	*IAuthDataLen  = 0;
	*ScriptLen    = 0;
	gstTransInfo.bIsFirstGAC = FALSE;
	SaveTVRTSI(TRUE);

	// prepare online DE55 data
	iRet = SetDE55(FALSE, (uint8_t *)&gstSendPack.sICCData[2], &iLength);
	if( iRet!=0 )
	{
		errno = EMV_ERRNO_ONLINE_ERROR;
		return -1;
	}
	PubASSERT( iLength<LEN_ICC_DATA );
	PubLong2Char((ulong)iLength, 2, gstSendPack.sICCData);
	memcpy(gstTransInfo.stTranLog.sIccData, &gstSendPack.sICCData[2], iLength);	// save for batch upload
	gstTransInfo.stTranLog.uiIccDataLen = (UINT32)iLength;

	// 冲正交易处理 & 离线交易上送
	// 判断上次交易是否需要进行冲正等
	iRet = TranReversal();
	if( iRet!=0 )
	{
		errno = EMV_ERRNO_ONLINE_ERROR;
		return -1;
	}

	iRetryPIN = 0;
	while( 1 )
	{
		iRet = SendRecvPacket();
		if( iRet!=0 )
		{
			errno = EMV_ERRNO_ONLINE_ERROR;
			return -1;
		}

		if( memcmp(gstRecvPack.szRspCode, "55", 2)!=0 || ++iRetryPIN>3 || !ChkIfNeedPIN() )
		{
			break;
		}

		// 重新输入PIN
		iRet = GetPIN(TRUE);
		if( iRet!=0 )
		{
			errno = EMV_ERRNO_ONLINE_ERROR;
			return -1;
		}
		sprintf((char *)gstSendPack.szSTAN, "%06lu", gstSysCtrlParam.ulSTAN);
		memcpy(&gstSendPack.sPINData[0], "\x00\x08", 2);
		memcpy(&gstSendPack.sPINData[2], gstTransInfo.sPinBlock, 8);
	}

	// set response code
	memcpy(RspCode,  gstRecvPack.szRspCode,  LEN_RSP_CODE);
	gstTransInfo.bIsOnlineTxn = TRUE;

	// get response issuer data
	sgAuthDataLen = sgScriptLen = 0;

	PubChar2Long(gstRecvPack.sICCData, 2, &ulICCDataLen);
	psICCData = &gstRecvPack.sICCData[2];
	for(psTemp=psICCData; psTemp<psICCData+ulICCDataLen; )
	{
		iRet = GetTLVItem((UINT8**)&psTemp, psICCData+ulICCDataLen-psTemp, SaveRspICCData, TRUE);
		if( iRet<0 )
		{	// if analyze response ICC data failed, return fail
			*online_result = 1;
			errno = EMV_ERRNO_DECLINE;
			return -1;
		}
	}
	memcpy(IAuthData, sAuthData, sgAuthDataLen);
	*IAuthDataLen = sgAuthDataLen;
	AdjustIssuerScript();
	memcpy(script, sIssuerScript, sgScriptLen);
	*ScriptLen = sgScriptLen;

	if( memcmp(gstRecvPack.szRspCode, "00", LEN_RSP_CODE)!=0 )
	{
		*online_result = 3;
		errno = EMV_ERRNO_DECLINE;
		return -1;
	}
	// set authorize code only if txn approved
	memcpy(AuthCode, gstRecvPack.szAuthCode, LEN_AUTH_CODE);
	*AuthCodeLen = strlen((char *)gstRecvPack.szAuthCode);

	*online_result = 0;
	return 0;
}


// 删除过期CAPK
void ClearExpireCAPK(void)
{ 
	int			iCnt,iRet,iCapkNum;
	char        szDateTime[16+1];
	CAPKLIST    stCapkList;

	sysGetTime(szDateTime);

	iCapkNum = emv_get_ca_public_key_num();
	iCnt = 0;
	while(iCnt < iCapkNum)
	{
		iRet =  emv_get_one_ca_public_key_by_no(iCnt,&stCapkList);
		iCnt++;
		if (iRet != EMV_RET_SUCCESS)
		{
			continue;
		}

		if( memcmp(szDateTime,stCapkList.expiration_date,3) >0 )
		{
			emv_delete_one_ca_public_key(stCapkList.RID,stCapkList.Index);
		}
	}
}


INT32 FinishEmvTran(void)
{
	INT32		iRet, iLength;
	ulong       ulIccStatusCode;
//	char        szBuffer[100];
	char            szMasterTcc[10];

	// 根据需要设置是否强制联机
	emv_get_mck_configure(&gstPosEmvConfig);
//	gstPosEmvConfig.cSupForceOnline = (gstTransInfo.stTranLog.ucTranType!=SALE ? 1 : 0);
	// 要求联机交易，不允许脱机交易
	gstPosEmvConfig.support_forced_online_capability = 1;
	SHA1((uint8_t*)&gstPosEmvConfig, sizeof(struct terminal_mck_configure)-20, gstPosEmvConfig.checksum);
	emv_set_mck_configure(&gstPosEmvConfig);

	// clear last EMV status
	memset(&gstEmvStatus, 0, sizeof(EMV_STATUS));
	SaveEmvStatus();

	iRet = 0;
	emv_get_data_element("\x9F\x53",2,(uint8_t *)&iRet,(char*)szMasterTcc);
	if (szMasterTcc[0] != 0x52)
	{
		szMasterTcc[0] = 0x52;
		emv_set_data_element((uint8_t *)"\x9F\x53",2,1,(uint8_t *)szMasterTcc);
	} 

	// 处理EMV交易
	iRet = emvProcTrans();
	SaveTVRTSI(FALSE);
 	if( iRet==EMV_RET_CANCEL || iRet == EMV_ERRNO_ONLINE_TIMEOUT)
 	{
 		return ERR_USERCANCEL;
 	}
	if( gstTransInfo.bIsOnlineTxn && memcmp(gstTransInfo.stTranLog.szRspCode, "00", 2)==0 )
	{
		// update for reversal(maybe have script result)
		SetDE55(FALSE, (uint8_t *)&gstSendPack.sICCData[2], &iLength);
		PubLong2Char((ulong)iLength, 2, gstSendPack.sICCData);
		gstTransInfo.stTranLog.uiIccDataLen = (UINT32)iLength;
		SaveRevInfo(TRUE);	// update reversal information
	}
	if( iRet!=EMV_RET_SUCCESS )
	{
		SaveEmvErrLog();
		ulIccStatusCode = emv_get_last_iccard_sw();
 		if( gstTransInfo.bIsFirstGAC && ulIccStatusCode==0x6985 &&
 			gstTransInfo.stTranLog.szPan[0]=='5' )
 		{	// for TIP fallback when 1st GAC return 6985
			gstTransInfo.iFallbackErrCode = EMV_RET_ICCERR;
 			return ERR_NEED_FALLBACK;
 		}

		if( !ChkIfAmex() && gstTransInfo.bIsOnlineTxn &&
			(memcmp(gstTransInfo.stTranLog.szRspCode, "01", 2)==0 ||
			 memcmp(gstTransInfo.stTranLog.szRspCode, "02", 2)==0) )
		{
			SetDE55(TRUE, gstTransInfo.stTranLog.sIccData, &iLength);
			gstTransInfo.stTranLog.uiIccDataLen = (UINT32)iLength;
		}

		if( gstTransInfo.stTranLog.szRspCode[0]!=0 &&
			memcmp(gstTransInfo.stTranLog.szRspCode, "00", 2)!=0 )
		{	// show reject code from host
			return AfterTranProc();
		}

		DisaptEmvTransFail(ERR_TRAN_FAIL);
		return ERR_TRAN_FAIL;
	}

	// 交易成功, 保存EMV交易数据
	SaveEmvData();
	if( !gstTransInfo.bIsOnlineTxn )
	{
		return FinishOffLine();
	}

	return AfterTranProc();
}

INT32 SetDE55(UINT8 bForUpLoad, UINT8 *psOutData, INT32 *piOutLen )
{
	if( ChkIfAmex() )
	{
		return SetAmexDE55(sgAmexTagList, psOutData, piOutLen);
	}
	else
	{
		return SetStdDE55(bForUpLoad, sgStdEmvTagList, psOutData, piOutLen);
	}
}

// set AMEX bit 55, structure of TLV items
int SetAmexDE55(DE55Tag *pstList, UINT8 *psOutData, int *piOutLen)
{
	int		iRet, iCnt, iLength, iTaglen;
	UINT8	*psTemp, sBuff[128], sTagbuf[4];
	
	*piOutLen = 0;
	memcpy(psOutData, "\xC1\xC7\xD5\xE2\x00\x01", 6);	// AMEX header
	psTemp = psOutData+6;
	iRet = iCnt = iLength = iTaglen = 0;
	
	for(iCnt=0; pstList[iCnt].uiEmvTag!=0; iCnt++)
	{
		iLength = 0;
		memset(sBuff, 0, sizeof(sBuff));
		memset(sTagbuf,0,sizeof(sTagbuf));
		PubLong2Char(pstList[iCnt].uiEmvTag,2,(char *)sTagbuf);
		iTaglen= 2;
		if (sTagbuf[0]==0)
		{
			sTagbuf[0] = sTagbuf[1];
			iTaglen = 1;
		}
		iRet = emv_get_data_element(sTagbuf, iTaglen, (uint8_t*)&iLength,(char*)sBuff);
		if( iRet!=EMV_RET_SUCCESS )
		{
			return ERR_TRAN_FAIL;
		}
		
		if( pstList[iCnt].ucOption==DE55_LEN_VAR1 )
		{
			*psTemp++ = (UINT8)iLength;
		}
		else if( pstList[iCnt].ucOption==DE55_LEN_VAR2 )
		{
			*psTemp++ = (UINT8)(iLength>>8);
			*psTemp++ = (UINT8)iLength;
		}
		memcpy(psTemp, sBuff, iLength);
		psTemp += iLength;
	}
	*piOutLen = (psTemp-psOutData);
	
	return 0;
}

// set ADVT/TIP bit 55
INT32 SetStdDE55(UINT8 bForUpLoad, DE55Tag *pstList, UINT8 *psOutData, INT32 *piOutLen)
{
	INT32		iRet, iCnt, iLength,iTaglen;
	UINT8	*psTemp, sBuff[200],sTagbuf[4];

	*piOutLen = 0;
	psTemp    = psOutData;
	iRet = iCnt = iLength = iTaglen = 0;

	if ( gstTransInfo.stTranLog.ucEntryMode & MODE_FALLBACK_SWIPE )
	{
		if (!ChkIfAmex())
		{
			memcpy(psTemp, "\xDF\xEC\x01\x01", 4);
			psTemp += 4;
			memcpy(psTemp, "\xDF\xED\x01", 3);
			psTemp += 3;		
//			if( gstTransInfo.iFallbackErrCode==EMV_RET_NOAPP ||
//				gstTransInfo.iFallbackErrCode==EMV_RET_ICCERR )
			if (gstTransInfo.iFallbackErrCode != 0)
			{
				*psTemp++ = 0x02;
			}
			else
			{
				*psTemp++ = 0x01;
			} 
		}
		else
		{
			memcpy(psTemp, "\xDF\xEC\x01\x01", 4);
			psTemp += 4;
//			memcpy(psTemp, "\xDF\xED\x01\x01", 4);
			memcpy(psTemp, "\xDF\xED\x01\x02", 4);
			psTemp += 4;
		}
	}
	else if( gstTransInfo.stTranLog.ucEntryMode & MODE_CHIP_INPUT )
	{
		for(iCnt=0; pstList[iCnt].uiEmvTag!=0; iCnt++)
		{
			memset(sBuff, 0, sizeof(sBuff));
			memset(sTagbuf,0,sizeof(sTagbuf));
			PubLong2Char(pstList[iCnt].uiEmvTag,2,(char *)sTagbuf);
			iTaglen= 2;
			if (sTagbuf[0]==0)
			{
				sTagbuf[0] = sTagbuf[1];
				sTagbuf[1] = 0;
				iTaglen = 1;
			}

			iRet = emv_get_data_element(sTagbuf, iTaglen, (uint8_t*)&iLength,(char*)sBuff);
			if( iRet==EMV_RET_SUCCESS )
			{
				BuildTLVString(pstList[iCnt].uiEmvTag, sBuff, iLength, &psTemp);
			}
			else if( pstList[iCnt].ucOption==DE55_MUST_SET )
			{
				BuildTLVString(pstList[iCnt].uiEmvTag, NULL, 0, &psTemp);
//				return ERR_TRAN_FAIL;	// 如果必须存在的TAG不存在,交易失败
			}
		}

		// for different bank
		iRet = emv_get_data_element("\x5F\x30", 2 , (uint8_t*)&iLength, (char*)sBuff);	// server code
		if( iRet==EMV_RET_SUCCESS )
		{
			BuildTLVString(0x5F30, sBuff, 2, &psTemp);
		}

		iRet = emv_get_data_element("\x5F\x34", 2 , (uint8_t*)&iLength, (char*)sBuff);	// PAN Seq #
		if( iRet==EMV_RET_SUCCESS )
		{
			BuildTLVString(0x5F34, sBuff, 1, &psTemp);
		}

		iRet = emv_get_data_element("\x9F\x28", 2 , (uint8_t*)&iLength, (char*)sBuff);	// PAN Seq #
		if( iRet==EMV_RET_SUCCESS )
		{
			BuildTLVString(0x9F28, sBuff, iLength, &psTemp);
		}

		iRet = emv_get_data_element("\x9F\x29", 2 , (uint8_t*)&iLength, (char*)sBuff);	// PAN Seq #
		if( iRet==EMV_RET_SUCCESS )
		{
			BuildTLVString(0x9F29, sBuff, iLength, &psTemp);
		}

		memset(sBuff, 0, sizeof(sBuff));
		if( bForUpLoad )
		{
			iRet = emv_get_data_element("\x5A", 1, (uint8_t*)&iLength, (char*)sBuff);
			if( iRet==EMV_RET_SUCCESS )
			{
				BuildTLVString(0x5A, sBuff, iLength, &psTemp);
			}
			else
			{
				BuildTLVString(0x5A, NULL, 0, &psTemp);
			}
		}
		else
		{
/*			iRet = emv_get_data_element("\x57",1 , &iLength ,sBuff);
			if( iRet==EMV_RET_SUCCESS )
			{
				BuildTLVString(0x57, sBuff, iLength, &psTemp);
			}
			else
			{
				BuildTLVString(0x57, NULL, 0, &psTemp);
			} */
		}

/*		memset(sBuff, 0, sizeof(sBuff));
		emv_get_data_element("\x9B",1,&iLength ,sBuff);
		BuildTLVString(0x9B, sBuff, iLength, &psTemp);*/

		if( gstTransInfo.stTranLog.szPan[0]=='5' )
		{	// for master card TCC = "R" -- retail
			BuildTLVString(0x9F53, (UINT8 *)"R", 1, &psTemp);
		}

		//  BOC master does not need 9F08
		if( gstTransInfo.stTranLog.szPan[0]!='5' )
		{
			memset(sBuff, 0, sizeof(sBuff));
			iRet = emv_get_data_element("\x9F\x08",2, (uint8_t*)&iLength ,(char*)sBuff);
			if( iRet== EMV_RET_SUCCESS )
			{
				BuildTLVString(0x9F08, sBuff, iLength, &psTemp);
			}
			else
			{
				BuildTLVString(0x9F08, NULL, 0, &psTemp);
			}
		}

/*		memset(sBuff, 0, sizeof(sBuff));
		iRet = emvGetScriptResult(sBuff, &iLength);
		if( iRet==EMV_RET_SUCCESS )
		{
			BuildTLVString(0xDF5B, sBuff, iLength, &psTemp);
		}*/

//		memcpy(psTemp, "\xDF\xEF\x02\x15\x08", 5);
//		psTemp += 5;
	}
	else
	{
		return 0;
	}

//	memcpy(psTemp, "\xDF\xEE\x01\x05", 4);
//	psTemp += 4;

	*piOutLen = (psTemp-psOutData);

	return 0;
}

//Set 56 field
INT32 SetDE56(UINT8 *psOutData, INT32 *piOutLen)
{
	*piOutLen = 0;
	if( ChkIfAmex() )
	{
		return 0;
	}

	return SetStdDE56(sgStdEmvField56TagList, psOutData, piOutLen);
}

INT32 SetStdEmptyDE56(UINT8 *psOutData, INT32 *piOutLen)
{
	if( ChkIfAmex() )
	{
		*piOutLen = 0;
		return 0;
	}

	memcpy(psOutData, "\xDF\xF0\x07\x00\x20\x20\x20\x20\x20\x20", 10);
	*piOutLen = 10;
	return 0;
}

INT32 SetStdDE56(DE55Tag *pstList, UINT8 *psOutData, INT32 *piOutLen)
{
	INT32		iRet, iCnt, iLength,iTaglen;
	UINT8	*psTemp, sBuff[110],sTagbuf[4];

	// build header of bit 56
	*piOutLen = 0;
	psTemp    = psOutData;
	iRet = iCnt = iLength = iTaglen = 0;

	memcpy(psTemp, "\xDF\xF0\x07\x01", 4);
	psTemp += 4;
	PubLong2Bcd(gstTransInfo.stTranLog.ulInvoiceNo, 3, (char *)psTemp);
	psTemp += 3;
	PubLong2Bcd(gstTransInfo.stTranLog.ulSTAN, 3, (char *)psTemp);
	psTemp += 3;

	// build common EMV core tags for all HK banks
	for(iCnt=0; pstList[iCnt].uiEmvTag!=0; iCnt++)
	{
		memset(sBuff, 0, sizeof(sBuff));
		memset(sTagbuf,0,sizeof(sTagbuf));
		PubLong2Char(pstList[iCnt].uiEmvTag,2,(char *)sTagbuf);
		iTaglen= 2;
		if (sTagbuf[0]==0)
		{
			sTagbuf[0] = sTagbuf[1];
			iTaglen = 1;
		}
		iRet = emv_get_data_element(sTagbuf, iTaglen, (uint8_t*)&iLength,sBuff);
		if( iRet==EMV_RET_SUCCESS )
		{
			BuildTLVString(pstList[iCnt].uiEmvTag, sBuff, iLength, &psTemp);
		}
		else
		{
			BuildTLVString(pstList[iCnt].uiEmvTag, NULL, 0, &psTemp);
		}
	}

	// process special EMC core tags for different banks
	memset(sBuff, 0, sizeof(sBuff));
	iRet = emv_get_script_result(sBuff, &iLength);
	if( iRet!=EMV_RET_SUCCESS )
	{
		*piOutLen = (psTemp-psOutData);
		return 0;
	}

	// continue issuer script result process
	memcpy(psTemp, "\xDF\x91", 2);
	psTemp   += 2;
	*psTemp++ = (UINT8)iLength;
	memcpy(psTemp, sBuff, iLength);
	psTemp += iLength;

	*piOutLen = (psTemp-psOutData);

	return 0;
}

// bExpandAll:       TRUE: expand constructed item, FALSE: not
INT32 GetTLVItem(UINT8 **ppsTLVString, INT32 iMaxLen, SaveTLVData pfSaveData, UINT8 bExpandAll)
{
	INT32			iRet;
	UINT8		*psTag, *psSubTag;
	uint		uiTag, uiLenBytes;
	ulong		lTemp;

	// skip null tags
	for(psTag=*ppsTLVString; psTag<*ppsTLVString+iMaxLen; psTag++)
	{
		if( (*psTag!=TAG_NULL_1) && (*psTag!=TAG_NULL_2) )
		{
			break;
		}
	}
	if( psTag>=*ppsTLVString+iMaxLen )
	{
		*ppsTLVString = psTag;
		return 0;	// no tag available
	}

	// process tag bytes
	uiTag = *psTag++;
	if( (uiTag & TAGMASK_FIRSTBYTE)==TAGMASK_FIRSTBYTE )
	{	// have another byte
		uiTag = (uiTag<<8) + *psTag++;
	}
	if( psTag>=*ppsTLVString+iMaxLen )
	{
		return -1;
	}

	// process length bytes
	if( (*psTag & LENMASK_NEXTBYTE)==LENMASK_NEXTBYTE )
	{
		uiLenBytes = *psTag & LENMASK_LENBYTES;
		PubChar2Long((char *)psTag+1, uiLenBytes, &lTemp);
	}
	else
	{
		uiLenBytes = 0;
		lTemp      = *psTag & LENMASK_LENBYTES;
	}
	psTag += uiLenBytes+1;
	if( psTag+lTemp>*ppsTLVString+iMaxLen )
	{
		return -2;
	}
	*ppsTLVString = psTag+lTemp;	// advance pointer of TLV string

	// save data
	(*pfSaveData)(uiTag, psTag, (INT32)lTemp);
	if( !IsConstructedTag(uiTag) || !bExpandAll )
	{
		return 0;
	}

	// constructed data
	for(psSubTag=psTag; psSubTag<psTag+lTemp; )
	{
		iRet = GetTLVItem(&psSubTag, psTag+lTemp-psSubTag, pfSaveData, TRUE);
		if( iRet<0 )
		{
			return iRet;
		}
	}

	return 0;
}

INT32 IsConstructedTag(uint uiTag)
{
	INT32		i;

	for(i=0; (uiTag&0xFF00) && i<2; i++)
	{
		uiTag >>= 8;
	}

	return ((uiTag & TAGMASK_CONSTRUCTED)==TAGMASK_CONSTRUCTED);
}

void SaveRspICCData(uint uiTag, UINT8 *psData, INT32 iDataLen)
{
	switch( uiTag )
	{
	case 0x91:
		memcpy(sAuthData, psData, min(iDataLen, 16));
		sgAuthDataLen = min(iDataLen, 16);
		break;

	case 0x71:
	case 0x72:
		sIssuerScript[sgScriptLen++] = (UINT8)uiTag;
		if( iDataLen>127 )
		{
			sIssuerScript[sgScriptLen++] = 0x81;
		}
		sIssuerScript[sgScriptLen++] = (UINT8)iDataLen;
		memcpy(&sIssuerScript[sgScriptLen], psData, iDataLen);
		sgScriptLen += iDataLen;
		break;

	case 0x9F36:
//		memcpy(sATC, psData, MIN(iDataLen, 2));	// ignore
		break;

	default:
		break;
	}
}

// 只处理基本数据元素Tag,不包括结构/模板类的Tag
void BuildTLVString(UINT32 uiEmvTag, UINT8 *psData, INT32 iLength, UINT8 **ppsOutData)
{
	UINT8	*psTemp;

	if( iLength<0 )
	{
		return;
	}

	// 设置TAG
	psTemp = *ppsOutData;
	if( uiEmvTag & 0xFF00 )
	{
		*psTemp++ = (UINT8)(uiEmvTag >> 8);
	}
	*psTemp++ = (UINT8)uiEmvTag;

	// 设置Length
	if( iLength<=127 )	// 目前数据长度均小余127字节,但仍按标准进行处理
	{
		*psTemp++ = (UINT8)iLength;
	}
	else
	{	// EMV规定最多255字节的数据
		*psTemp++ = 0x81;
		*psTemp++ = (UINT8)iLength;
	}

	// 设置Value
	if( iLength>0 )
	{
		memcpy(psTemp, psData, iLength);
		psTemp += iLength;
	}

	*ppsOutData = psTemp;
}

void SaveEmvData(void)
{
	INT32		iLength;

	iLength = 0;
	emv_get_data_element("\x9F\x26",2, (uint8_t*)&iLength,gstTransInfo.stTranLog.sAppCrypto);
	emv_get_data_element("\x8A",1,   (uint8_t*)&iLength,  gstTransInfo.stTranLog.szRspCode);
	emv_get_data_element("\x95", 1, (uint8_t*)&iLength, gstTransInfo.stTranLog.sTVR);
	emv_get_data_element("\x9B",  1, (uint8_t*)&iLength , gstTransInfo.stTranLog.sTSI);
	emv_get_data_element("\x9F\x36", 2,  (uint8_t*)&iLength, gstTransInfo.stTranLog.sATC);

	// save for upload
	SetDE55(TRUE, gstTransInfo.stTranLog.sIccData, &iLength);
	gstTransInfo.stTranLog.uiIccDataLen = (UINT32)iLength;

	if( !gstTransInfo.bIsOnlineTxn )
	{	// ICC脱机, offline approved
		SaveTVRTSI(TRUE);
		GetNewTraceNo();
//		sprintf((char *)gstTransInfo.stTranLog.szRspCode, "00");
//		sprintf((char *)gstTransInfo.stTranLog.szCondCode, "06");
		sprintf((char *)gstTransInfo.stTranLog.szAuthCode, "%06lu", gstSysCtrlParam.ulSTAN);
	}
}

// core cannot process correctly if length of 9F18 is zero
// eg, 71 12 9F 18 00 86 0D 84 1E 00 00 08 11 22 33 44 55 66 77 88
void AdjustIssuerScript(void)
{
	INT32		iRet;
	UINT8	*psTemp;

	memset(sgScriptBak, 0, sizeof(sgScriptBak));
	memset((char*)&sgScriptInfo, 0, sizeof(sgScriptInfo));
	sgCurScript = sgScriptBakLen = 0;
	bHaveScript  = FALSE;
	for(psTemp=sIssuerScript; psTemp<sIssuerScript+sgScriptLen; )
	{
		iRet = GetTLVItem(&psTemp, sIssuerScript+sgScriptLen-psTemp, SaveScriptData, TRUE);
		if( iRet<0 )
		{
			return;
		}
	}
	if( bHaveScript && sgCurScript>0 )
	{
		PackScriptData();
	}

	memcpy(sIssuerScript, sgScriptBak, sgScriptBakLen);
	sgScriptLen = sgScriptBakLen;
}

// callback function for process issuer script
void  SaveScriptData(uint uiTag, UINT8 *psData, INT32 iDataLen)
{
	switch( uiTag )
	{
	case 0x71:
	case 0x72:
		if( bHaveScript && sgCurScript>0 )
		{
			PackScriptData();
		}
		sgScriptInfo.uiTag = uiTag;
		bHaveScript = TRUE;
		break;

	case 0x9F18:
		sgScriptInfo.iIDLen = min(4, iDataLen);
		memcpy(sgScriptInfo.sScriptID, psData, MIN(4, iDataLen));
		break;

	case 0x86:
		sgScriptInfo.iCmdLen[sgCurScript] = iDataLen;
		memcpy(sgScriptInfo.sScriptCmd[sgCurScript], psData, iDataLen);
		sgCurScript++;
		break;

	default:
		break;
	}
}

void PackTLVData(uint uiTag, UINT8 *psData, uint uiDataLen, UINT8 *psOutData, INT32 *piOutLen)
{
	INT32		iHeadLen;

	PackTLVHead(uiTag, uiDataLen, psOutData, &iHeadLen);
	memcpy(psOutData+iHeadLen, psData, uiDataLen);
	*piOutLen = (uiDataLen+iHeadLen);
}

void PackTLVHead(uint uiTag, uint uiDataLen, UINT8 *psOutData, INT32 *piOutLen)
{
	UINT8	*psTemp;

	// pack tag bytes
	psTemp = psOutData;
	if( uiTag & 0xFF00 )
	{
		*psTemp++ = uiTag>>8;
	}
	*psTemp++ = uiTag;

	// pack length bytes
	if( uiDataLen<=127 )
	{
		*psTemp++ = (UINT8)uiDataLen;
	}
	else
	{
		*psTemp++ = LENMASK_NEXTBYTE|0x01;	// one byte length
		*psTemp++ = (UINT8)uiDataLen;
	}

	*piOutLen = (psTemp-psOutData);
}

INT32 CalcTLVTotalLen(uint uiTag, uint uiDataLen)
{
	INT32		iLen;

	// get length of TLV tag bytes
	iLen = 1;
	if( uiTag & 0xFF00 )
	{
		iLen++;
	}

	// get length of TLV length bytes
	iLen++;
	if( uiDataLen>127 )
	{
		iLen++;
	}

	return (iLen+uiDataLen);
}

// re-generate issuer script(remove issuer script ID, if the length is zero)
void PackScriptData(void)
{
	INT32		iCnt, iTotalLen, iTempLen;

	iTotalLen = 0;
	if( sgScriptInfo.iIDLen>0 )
	{
		iTotalLen += CalcTLVTotalLen(0x9F18, 4);
	}
	for(iCnt=0; iCnt<sgCurScript; iCnt++)
	{
		iTotalLen += CalcTLVTotalLen(0x86, sgScriptInfo.iCmdLen[iCnt]);
	}
	PackTLVHead(sgScriptInfo.uiTag, iTotalLen, &sgScriptBak[sgScriptBakLen], &iTempLen);
	sgScriptBakLen += iTempLen;

	if( sgScriptInfo.iIDLen>0 )
	{
		PackTLVData(0x9F18, sgScriptInfo.sScriptID, 4, &sgScriptBak[sgScriptBakLen], &iTempLen);
		sgScriptBakLen += iTempLen;
	}
	for(iCnt=0; iCnt<sgCurScript; iCnt++)
	{
		PackTLVData(0x86, sgScriptInfo.sScriptCmd[iCnt], sgScriptInfo.iCmdLen[iCnt], &sgScriptBak[sgScriptBakLen], &iTempLen);
		sgScriptBakLen += iTempLen;
	}

	memset((char*)&sgScriptInfo, 0, sizeof(sgScriptInfo));
	sgCurScript = 0;
}

// save EMV status for FUNC 9
void SaveTVRTSI(UINT8 bBeforeOnline)
{
	INT32				iRet, iLength, iCnt;
	UINT8			    sTermAID[16];
	AIDLIST		        stEmvApp;

	iRet = iLength = iCnt = 0;
	if( bBeforeOnline )
	{
		emv_get_data_element("\x95", 1, (uint8_t*)&iLength, gstEmvStatus.sTVROld);
		emv_get_data_element("\x9B", 1,   (uint8_t*)&iLength ,gstEmvStatus.sTSIOld);
		gstEmvStatus.iARQCOldLen = 0;
		emv_get_data_element("\x9F\x10",2,(uint8_t*)&gstEmvStatus.iARQCOldLen, gstEmvStatus.sARQCOld);

		// search TAC from terminal parameter
		memset(sTermAID, 0, sizeof(sTermAID));
		emv_get_data_element("\x9F\x06",2, (uint8_t*)&iLength,sTermAID);
		for(iCnt=0; iCnt<MAX_APP_NUM; iCnt++)
		{
			memset(&stEmvApp, 0, sizeof(AIDLIST));
			iRet = emv_get_one_aid_info(iCnt, &stEmvApp);
			if( iRet < 0 )
			{
				continue;
			}
			
			if( memcmp(sTermAID, stEmvApp.aid, stEmvApp.aid_len)==0 )
			{
				memcpy(gstEmvStatus.sTACDeinal,  stEmvApp.terminal_action_code_denial,  5);
				memcpy(gstEmvStatus.sTACOnline,  stEmvApp.terminal_action_code_online,  5);
				memcpy(gstEmvStatus.sTACDefault, stEmvApp.terminal_action_code_default, 5);
				break;
			}
		} 

		sgiSaveTVRFlag = 0;
	}
	else
	{
		emv_get_data_element("\x95", 1 ,  (uint8_t*)&iLength,  gstEmvStatus.sTVRNew);
		emv_get_data_element("\x9B", 1,  (uint8_t*)&iLength,  gstEmvStatus.sTSINew);

		emv_get_data_element("\x9F\x0E",2 , (uint8_t*)&iLength,gstEmvStatus.sIACDeinal);
		emv_get_data_element("\x9F\x0D",2, (uint8_t*)&iLength, gstEmvStatus.sIACOnline);
		emv_get_data_element("\x9F\x0F",2, (uint8_t*)&iLength, gstEmvStatus.sIACDefault);

		if (sgiSaveTVRFlag)
		{
			memset(sTermAID, 0, sizeof(sTermAID));
			emv_get_data_element("\x9F\x06",2, (uint8_t*)&iLength,sTermAID);
			for(iCnt=0; iCnt<MAX_APP_NUM; iCnt++)
			{
				memset((char*)&stEmvApp, 0, sizeof(AIDLIST));
				iRet = emv_get_one_aid_info(iCnt, &stEmvApp);
				if( iRet < 0 )
				{
					continue;
				}
				if( memcmp(sTermAID, stEmvApp.aid, stEmvApp.aid_len)==0 )
				{
					memcpy(gstEmvStatus.sTACDeinal,  stEmvApp.terminal_action_code_denial,  5);
					memcpy(gstEmvStatus.sTACOnline,  stEmvApp.terminal_action_code_online,  5);
					memcpy(gstEmvStatus.sTACDefault, stEmvApp.terminal_action_code_default, 5);
					break;
				}
			}
		}
	}
	SaveEmvStatus();
}

// show last EMV status
void ViewTVR_TSI(void)
{
	INT32		iTemp;
	char    	szBuff[20];

	LoadEmvStatus();

	lcdCls();
	lcdDisplay(0, 0, DISP_ASCII, "Before TSI=%02X %02X", gstEmvStatus.sTSIOld[0], gstEmvStatus.sTSIOld[1]);
	lcdDisplay(0, 1, DISP_ASCII, "TVR=%02X %02X %02X %02X %02X",
			gstEmvStatus.sTVROld[0], gstEmvStatus.sTVROld[1], gstEmvStatus.sTVROld[2],
			gstEmvStatus.sTVROld[3], gstEmvStatus.sTVROld[4]);
	lcdDisplay(0, 2, DISP_ASCII, "IssuAppData=");
	lcdGoto(0, 3);
//	ScrFontSet(DISP_ASCII);
	for(iTemp=0; iTemp<gstEmvStatus.iARQCOldLen; iTemp++)
	{
		printf("%02X", gstEmvStatus.sARQCOld[iTemp]);
	}
	PubWaitKey(USER_OPER_TIMEOUT);

	lcdCls();
	lcdDisplay(0, 0, DISP_ASCII, "After TSI=%02X %02X", gstEmvStatus.sTSINew[0], gstEmvStatus.sTSINew[1]);
	lcdDisplay(0, 1, DISP_ASCII, "TVR=%02X %02X %02X %02X %02X",
			gstEmvStatus.sTVRNew[0], gstEmvStatus.sTVRNew[1], gstEmvStatus.sTVRNew[2],
			gstEmvStatus.sTVRNew[3], gstEmvStatus.sTVRNew[4]);

	PubBcd2Asc0((char *)gstEmvStatus.sTACDeinal, 5, szBuff);
	lcdDisplay(0, 2, DISP_ASCII, "TACDenial =%10.10s", szBuff);

	PubBcd2Asc0((char *)gstEmvStatus.sTACOnline, 5, szBuff);
	lcdDisplay(0, 3, DISP_ASCII, "TACOnline =%10.10s", szBuff);

	PubBcd2Asc0((char *)gstEmvStatus.sTACDefault, 5, szBuff);
	lcdDisplay(0, 4, DISP_ASCII, "TACDefault=%10.10s", szBuff);

	PubBcd2Asc0((char *)gstEmvStatus.sIACDeinal, 5, szBuff);
	lcdDisplay(0, 5, DISP_ASCII, "IACDenial =%10.10s", szBuff);

	PubBcd2Asc0((char *)gstEmvStatus.sIACOnline, 5, szBuff);
	lcdDisplay(0, 6, DISP_ASCII, "IACOnline =%10.10s", szBuff);

	PubBcd2Asc0((char *)gstEmvStatus.sIACDefault, 5, szBuff);
	lcdDisplay(0, 7, DISP_ASCII, "IACDefault=%10.10s", szBuff);

	PubWaitKey(USER_OPER_TIMEOUT);
}

int  InitEMVCore()
{
	int iRet;

	emv_set_kernel_type(KERNEL_TYPE_PBOC);
	emv_init_data_element();  // 初始化EMV内核数据元

	memset((char*)&gstPosEmvConfig,0,sizeof(gstPosEmvConfig));
	emv_get_mck_configure(&gstPosEmvConfig);
	gstPosEmvConfig.terminal_type = 0x22;          // 终端类型
	memcpy(gstPosEmvConfig.terminal_capabilities,"\xE0\xB0\xC8", 3);  // 终端性能
	memcpy(gstPosEmvConfig.additional_terminal_capabilities,  "\xE0\x00\xF0\xA0\x01", 5);  // 终端附加性能
	gstPosEmvConfig.support_pse_selection_method = 1;         // PSE选择方式
//	gstPosEmvConfig.cSupPSESelect = 0;         // PSE选择方式
	gstPosEmvConfig.support_cardholder_confirmation = 1;        // 支持持卡人确认, 由持卡人选择应用
	gstPosEmvConfig.support_certificate_revocation = 1;      // IssCert 检查    
    gstPosEmvConfig.support_default_ddol = 1;           // 支持默认DDOL	
    gstPosEmvConfig.support_bypass_pin_entry = 1;         // Bypass PIN, 可跳过密码
    gstPosEmvConfig.support_get_pin_try_counter = 1;         // 支持获取PIN验证次数	
    gstPosEmvConfig.support_floor_limit_checking = 1 ;  // 支持最低限额检测
    gstPosEmvConfig.support_random_transaction_selection = 1 ;    // 支持随机交易选择
    gstPosEmvConfig.support_velocity_checking = 1 ;    // 支持频度检测
    gstPosEmvConfig.support_transaction_log = 1;          // 交易日志 - KERN 在最低限额检查中使用
    gstPosEmvConfig.support_exception_file = 1;       // 异常文件检查	
    gstPosEmvConfig.support_terminal_action_codes = 1;      // 支持终端行为代码
    gstPosEmvConfig.support_forced_online_capability = 0 ;      // 交易强制联机能力
    gstPosEmvConfig.support_advices = 1;            // 终端是否支持通知
    gstPosEmvConfig.support_issuer_initiated_voice_referrals = 1;       // 支持发卡行发起的参考
    gstPosEmvConfig.support_batch_data_capture = 1;      // 支持批数据采集
    gstPosEmvConfig.support_online_data_capture = 1;     // 支持联机数据采集
    gstPosEmvConfig.support_default_tdol = 1;           // 支持默认TDOL	
	gstPosEmvConfig.support_multi_language = 1;         // 支持多语言
	gstPosEmvConfig.support_forced_acceptance_capability = 0;       // 支持强制接受交易
	gstPosEmvConfig.support_card_initiated_voice_referrals  = 1;     // 支持卡发起的参考
	gstPosEmvConfig.support_trm_based_on_aip = 0;          // 支持基于AIP的风险管理--强制进行终端风险管理
    gstPosEmvConfig.pos_entry_mode  = 0x92 ;    // POS Entry Code   	        
	SHA1((uint8_t*)&gstPosEmvConfig, sizeof(gstPosEmvConfig)-20 ,gstPosEmvConfig.checksum);// 与各配置对应的hash值

	iRet = emv_set_mck_configure(&gstPosEmvConfig);
	if (iRet!=EMV_RET_SUCCESS)
	{
		return iRet;
	}

	memset((char*)&gstEmvCoreParam,0,sizeof(gstEmvCoreParam));
	strcpy((char *)gstEmvCoreParam.terminal_id,"12345678");
	strcpy((char *)gstEmvCoreParam.merchant_id,"012345678901234");
	strcpy((char *)gstEmvCoreParam.merchant_cate_code,"12");
	gstEmvCoreParam.merchant_name_loc_len = strlen("EMV TEST MERCH");
	strcpy((char *)gstEmvCoreParam.merchant_name_loc,"EMV TEST MERCH");
	gstEmvCoreParam.transaction_type =  EMV_GOODS;
 	strcpy((char *)gstEmvCoreParam.terminal_country_code,"\x04\x58");
 	strcpy((char *)gstEmvCoreParam.transacion_currency_code,"\x04\x58");
 	strcpy((char *)gstEmvCoreParam.refer_currency_code,"\x04\x58");	
	gstEmvCoreParam.transacion_currency_exponent = 0x02;
	gstEmvCoreParam.refer_currency_exponent = 0x02;
	gstEmvCoreParam.refer_currency_coefficient = 0;

	iRet = emv_set_core_init_parameter(&gstEmvCoreParam);
	if (iRet!=EMV_RET_SUCCESS)
	{
		return iRet;
	}

	gstEMVCallBackFunc.emv_candidate_apps_selection = cEMVSelectApp;
	gstEMVCallBackFunc.emv_multi_language_selection = cEMVSelectLanguage;
	gstEMVCallBackFunc.emv_get_amount = cEMVGetTransactionAmount;
	gstEMVCallBackFunc.emv_get_pin = cEMVGetCardholderPin;
	gstEMVCallBackFunc.emv_return_pin_verify_result = cEMVShowPinVerifyResult;
	gstEMVCallBackFunc.emv_check_online_pin = cEMVGetOnlinePIN;
	gstEMVCallBackFunc.emv_check_certificate = cEMVCheckCertificate;
	gstEMVCallBackFunc.emv_online_transaction_process = cEMVOnlineProc;
	gstEMVCallBackFunc.emv_issuer_referral_process = cEMVReferProc;
	gstEMVCallBackFunc.emv_advice_process = cEMVAdviceProc;
	gstEMVCallBackFunc.emv_check_revocation_certificate = emvCheckCRL;
	gstEMVCallBackFunc.emv_check_exception_file = cEMVCheckExceptPAN;
	gstEMVCallBackFunc.emv_get_transaction_log_amount = cEMVGetTransLogAmount;

	iRet = emv_init_callback_func(&gstEMVCallBackFunc);
	if (iRet!=EMV_RET_SUCCESS)
	{
		return iRet;
	}	

	emv_delete_all_terminal_aids();		
	emv_delete_all_ca_public_keys();
     
	return EMV_RET_SUCCESS;
}


INT32 emvProcTrans(void)
{
	int iRet = EMV_RET_SUCCESS;
	int need_online = 0;
	
	emv_process_restriction();
		
	iRet = emv_cardholder_verify(icc_fd);
	iRet = iRet?errno:0;	
	if (iRet != EMV_RET_SUCCESS)
	{		
		return iRet;
	}
	
	if (EMV_RET_SUCCESS == iRet)
	{
		iRet = emv_terminal_risk_manage(icc_fd);
		iRet = iRet?errno:0;
		if (iRet != EMV_RET_SUCCESS)
		{		
			return iRet;
		}
	}
	
	if (EMV_RET_SUCCESS == iRet)
	{
		iRet = emv_terminal_action_analysis(icc_fd, &need_online);
		iRet = iRet?errno:0;
		if (iRet != EMV_RET_SUCCESS)
		{	
			return iRet;
		}
	}
	
	if ((EMV_RET_SUCCESS == iRet)&& (need_online))
	{
		iRet = emv_online_transaction(icc_fd);
		iRet = iRet?errno:0;
		if (iRet != EMV_RET_SUCCESS)
		{		
			return iRet;
		}
	}	

	return iRet;
}

// end of file

