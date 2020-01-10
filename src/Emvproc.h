
/****************************************************************************
NAME
	Emvproc.h - ʵ��EMV���״���

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	pyming     2008.04.01      - created
****************************************************************************/

#ifndef _EMVPROC_H
#define _EMVPROC_H

#include "New8210.h"

#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif 

// EMV���һ�ν��׵�״̬
typedef struct _tagEMV_STATUS
{
	int	    	iARQCOldLen;
	UINT8		sARQCOld[32];
	UINT8		sTSIOld[2];
	UINT8		sTVROld[5];		// TVR/TSI before send tho host

	UINT8		sTSINew[2];
	UINT8		sTVRNew[5];

	UINT8		sIACDeinal[5];
	UINT8		sIACOnline[5];
	UINT8		sIACDefault[5];

	UINT8		sTACDeinal[5];
	UINT8		sTACOnline[5];
	UINT8		sTACDefault[5];
}EMV_STATUS;

// EMV������־�ļ���¼
typedef struct _tagEMV_ERR_LOG
{
	UINT8	bValid;
	UINT8	ucAidLen;
	UINT8	szAID[16+1];
	UINT8	szPAN[19+1];
	UINT8	ucPANSeqNo;
	UINT8	szAmount[12+1];
	UINT8	szTipAmt[12+1];
	UINT8	szRspCode[2+1];
	UINT8	szDateTime[14+1];
	UINT8	szRRN[12+1];
	UINT8	szAuthCode[6+1];
	UINT8	sTVR[5];
	UINT8	sTSI[2];
	UINT32	uiReqICCDataLen;
	UINT8	sReqICCData[260];
	UINT32	uiReqField56Len;
	UINT8	sReqField56[110];
	UINT32	uiRspICCDataLen;
	UINT8	sRspICCData[150];
	ulong	ulSTAN;
}EMV_ERR_LOG;

typedef struct _tagDE55Tag
{
	UINT32	uiEmvTag;
	UINT8	ucOption;
	UINT8	ucLen;		// for process AMEX bit 55, no used for ADVT/TIP
}DE55Tag;

typedef struct _tagScriptInfo
{
	UINT32	uiTag;
	INT32		iIDLen;
	UINT8	sScriptID[4];
	INT32		iCmdLen[20];
	UINT8	sScriptCmd[20][300];
}ScriptInfo;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// EMV�ص�����
int  cEMVSelectApp(void);              // ѡ��Ӧ��
void cEMVSelectLanguage(void);         // ѡ������
int  cEMVGetOnlinePIN(void);           // ��������PIN
//int  cEMVInputEncryptPIN(const R_RSA_PUBLIC_KEY *pstRsaPubKey, UINT8 *psIccRandom);      // �ѻ�����PINУ��
//int  cEMVInputPlainPIN(void);           //   �ѻ�����PINУ��
int  cEMVCheckCertificate(void);        //  �ֿ���֤�����
int  cEMVAdviceProc(INT32 Flg);              // ���鴦��
int  cEMVReferProc(void);               // ��������
int  cEMVOnlineProc( UINT8 *RspCode, UINT8 *AuthCode, INT32 *AuthCodeLen,
					  UINT8  *IAuthData, INT32 *IAuthDataLen, UINT8 *script, INT32 *ScriptLen, int *online_result); // ��������


int cEMVGetCardholderPin(void *CardPin);
int cEMVShowPinVerifyResult(UINT8 TryCounter);
int cEMVGetTransactionAmount(UINT32 *pTransAmount, UINT32 *pOtherAmount);
int cEMVCheckExceptPAN(uint8_t PANLen, const void *PAN, uint8_t PANSn);
int cEMVGetTransLogAmount(uint8_t PanLen, const void *PAN, uint8_t PanSn);
INT32 emvCheckCRL(uint8_t index,const void *rid,const void *cert_sn);
	
void   ClearExpireCAPK(void);
int    FinishEmvTran(void);
void   ViewTVR_TSI(void);
int    SetDE55(UINT8 bForUpLoad, UINT8 *psOutData, int *piOutLen);
int    SetDE56(UINT8 *psOutData, int *piOutLen);
int    SetStdEmptyDE56(UINT8 *psOutData, int *piOutLen);
int    InitEMVCore(void);

void   SaveTVRTSI(UINT8 bBeforeOnline);
INT32  emvProcTrans(void);

extern struct emv_core_init             gstEmvCoreParam;     // EMV�ں˲���
extern struct terminal_mck_configure    gstPosEmvConfig;     // �ն����ã���������
extern struct emv_callback_func         gstEMVCallBackFunc;  // ����Ļص�����ָ��
extern EMV_STATUS	    	gstEmvStatus;
extern int                  gIsFirstSelectApp;  // �Ƿ��ǵ�һ��ѡ��Ӧ��
extern int                  sgiSaveTVRFlag;      // �Ƿ񱣴��TAC

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
