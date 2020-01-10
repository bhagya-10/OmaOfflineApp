
/****************************************************************************
NAME
    ISO8583.h - ����POS����Ա���ع���Ļ��������⺯��

DESCRIPTION
    ʵ�ֱ�׼��ISO8583���ĵĴ��/����⺯��,֧��64Bit/128Bit���ISO8583���ĵ�
    ���/���.

REFERENCE
    1. ISO8583-1998.doc

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008-04-01      - created
****************************************************************************/

/*****************************************************************************
COMMENT:
   FIELD_ATTR element:
      eElementAttr:
	 Attr_n  :  numeric data('0'~'9','A'~'F')
                    each data element represents 1 nibble.
                    (2 data elements = 1 byte)
	 Attr_z  :  numeric data(0x30~0x3F)
                    each data element represents 1 nibble.
                    (2 data elements = 1 byte)
	 Attr_b  :  binary data, each data element represents 1 bit.
                    (8 data elements = 1 byte)
	 Attr_a  :  alpha-number and special characters.
                    each data element represents 1 byte.
	 Attr_UnUsed  :  the element is no used.
         Attr_Over    :  definiens over.
      eLengthAttr:
	 Attr_fix  - fixed length.
	 Attr_var1 - variable length, one byte at the beginning of the data
                 element identify the number of positions following to the
                 end of that data element.
                 (0x00 ~ 0x99)
	 Attr_var2 - variable length, two bytes at the beginning of the data
                 element identify the number of positions following to the
                end of that data element.
                (0x00 0x00 ~ 0x09 0x99)
      uiLength:
	    length of the data element.
NOTE:
   All fixed length Attr_n data elements are right justified with leading
   zeroes.
   All fixed length Attr_a data elements are left justified with trailing
   spaces.
   All fixed length Attr_b data elements are left justified with trailing
   zeroes.
   All variable length Attr_n data elements are left justified with trailing
   zero.
   There are two bytes for showing length at the beginning of all Attr_b data
   elements while the struct ST_8583 are inputed or outputed.
*****************************************************************************/

#ifndef _ISO8583_H
#define _ISO8583_H

/*���⴦����: �ɱ䳤��Attr_n������Ԫ���ҿ��룬����
#define VarAttrN_RightJustify*/


#define	LEN_MSG_CODE			4
#define	LEN_BITMAP				8
#define	LEN_PAN					19
#define	LEN_PROC_CODE			6
#define	LEN_TRAN_AMT			12
#define	LEN_FRN_AMT				12
#define	LEN_DCC_RATE			8
#define	LEN_STAN				6
#define	LEN_LOCAL_TIME			6
#define	LEN_LOCAL_DATE			4
#define	LEN_EXP_DATE			4
#define	LEN_SETTLE_DATE			4
#define	LEN_ENTRY_MODE			4
#define	LEN_PAN_SEQ_NO			3
#define	LEN_NII					3
#define	LEN_COND_CODE			2
#define	LEN_TRACK2				37
#define	LEN_TRACK3				104
#define	LEN_RRN					12
#define	LEN_AUTH_CODE			6
#define	LEN_RSP_CODE			2
#define	LEN_TERM_ID				8
#define	LEN_MERCHANT_ID			15
#define	LEN_ADDL_RSP			2
#define	LEN_TRACK1				76
#define	LEN_FIELD48				3
#define	LEN_FRN_CURCY_CODE		3
#define	LEN_CURCY_CODE			3
#define	LEN_PIN_DATA			8
#define	LEN_EXT_AMOUNT			12
#define	LEN_ICC_DATA			260
#define	LEN_ICC_DATA2			110
#define LEN_FIELD57             300
#define	LEN_FIELD60				600
#define	LEN_TMSFIELD60			600
#define	LEN_FIELD61				30
#define	LEN_INVOICE_NO			6
#define LEN_FIELD62             30
#define	LEN_FIELD63				100
#define LEN_MAC					8

// ˵����
// 1. ��Binary���͵��򣬽ṹ��Ա������ǰ��2���ֽ�Ϊ���ݳ���
//    ��ʽΪ����ЧΪ��ǰ�������ֽ�˳��
// 2. �Է�Binary������ֱ��ʹ��C���ַ�������/��ʽ���������и�ֵ����
//    �����2���ֽ���Ϊ�˴洢'\0'�ַ�(Ϊ���㴦��������һ���ַ�)
typedef struct _tagSTISO8583
{
	char	szMsgCode[LEN_MSG_CODE+2];				// message code
	char	sBitMap[2*LEN_BITMAP];					// ��Ҫ��2
	char	szPan[LEN_PAN+2];						// PAN
	char	szProcCode[LEN_PROC_CODE+2];			// proc code
	char	szTranAmt[LEN_TRAN_AMT+2];				// Txn Amount
	char	szFrnAmt[LEN_FRN_AMT+2];				// Foreign amt
	char	szDccRate[LEN_DCC_RATE+2];				// DCC Rate
	char	szSTAN[LEN_STAN+2];						// STAN
	char	szLocalTime[LEN_LOCAL_TIME+2];			// time, hhmmss
	char	szLocalDate[LEN_LOCAL_DATE+2];			// date, YYMM
	char	szExpDate[LEN_EXP_DATE+2];				// Expiry, YYMM
	char	szEntryMode[LEN_ENTRY_MODE+2];			// entry mode
	char	szPanSeqNo[LEN_PAN_SEQ_NO+2];			// PAN seq #
	char	szNii[LEN_NII+2];						// NII
	char	szCondCode[LEN_COND_CODE+2];			// Cond. code
	char	szTrack2[LEN_TRACK2+2];					// track 2
	char	szTrack3[LEN_TRACK3+2];					// track 3
	char	szRRN[LEN_RRN+2];						// RRN
	char	szAuthCode[LEN_AUTH_CODE+2];			// auth code
	char	szRspCode[LEN_RSP_CODE+2];				// rsp code
	char	szTermID[LEN_TERM_ID+2];				// terminal id
	char	szMerchantID[LEN_MERCHANT_ID+2];		// merchant id
	char	szAddlRsp[LEN_ADDL_RSP+2];				// add'l rsp
	char	szTrack1[LEN_TRACK1+2];					// track 1
	char	sField48[LEN_FIELD48+2];				// for instalment or cvv2 for visa/master card
	char	szFrnCurcyCode[LEN_FRN_CURCY_CODE+2];	// for DCC
	char	szCurcyCode[LEN_CURCY_CODE+2];			// for DCC
	char	sPINData[LEN_PIN_DATA+2];				// PIN data
	char	szExtAmount[LEN_EXT_AMOUNT+2];			// extra amount
	char	sICCData[LEN_ICC_DATA+2];				// ICC data
	char	sICCData2[LEN_ICC_DATA2+2];				// ICC data, FOR HK
	char    sField57[LEN_FIELD57+2];                // encrypt data
	char	szField60[LEN_FIELD60+2];
	char	szField61[LEN_FIELD61+2];
	char	sField62[LEN_FIELD62+2];
	char	sField63[LEN_FIELD63+2];
	char	sMac[LEN_MAC+2];
}ST_ISO8583;

// TMSר�ñ���
typedef struct _tagSTTMS8583
{
	char	szMsgCode[LEN_MSG_CODE+2];				// message code
	char	sBitMap[2*LEN_BITMAP];					// ��Ҫ��2
	char	szProcCode[LEN_PROC_CODE+2];			// proc code
	char	szSTAN[LEN_STAN+2];						// STAN
	char	szLocalTime[LEN_LOCAL_TIME+2];			// time, hhmmss
	char	szLocalDate[LEN_LOCAL_DATE+2];			// date, YYMM
	char	szNii[LEN_NII+2];						// NII
	char	szRspCode[LEN_RSP_CODE+2];				// rsp code
	char	szTermID[LEN_TERM_ID+2];				// terminal id
	char	szMerchantID[LEN_MERCHANT_ID+2];		// merchant id
	char	sField60[LEN_TMSFIELD60+2];
	char	szField61[LEN_FIELD61+2];
}STTMS8583;


#define  ERR_UNPACK_LEN     -1000         /* ��������ݳ�����ʵ�ʳ��Ȳ��� */

#define Attr_a			0
#define Attr_b			1
#define Attr_n			2
#define Attr_z			3
#define Attr_UnUsed		4
#define Attr_Over		5

#define Attr_var1		0
#define Attr_var2		1
#define Attr_fix		2

#ifndef uint
typedef  unsigned int   uint;
#endif

typedef struct _tagFIELD_ATTR
{
   char	eElementAttr;
   char	eLengthAttr;
   uint		uiLength;
}FIELD_ATTR;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

extern FIELD_ATTR glEdcDataDef[];		// 8583��Ϣ����
extern FIELD_ATTR glTMSDataDef[];		// ��������ʱ��Ϣ����

/****************************************************************************
 Function:          change 8583 struct to 8583 packet
 Param in:
    pDataAttr       struct array to define attribute of bitmap and
                    data elements
    pSt8583         pointer to point to struct 8583
 Param out:
    pusOut          8583 packet
    puiOutLen       bytes of 8583 packet
 Return Code:
    0               Ok
    <0              message header error
    <-1000          length of msg field overflow
    <-2000          attributes define error
****************************************************************************/
INT32 PubPack8583(FIELD_ATTR *pDataAttr, void *pSt8583, char *pusOut, uint *puiOutLen);

/****************************************************************************
 Function:          change 8583 packet to 8583 struct
 Param in:
   pDataAttr        struct array to define attribute of bitmap and
   pusIn            8583 packet
   uiInLen          bytes of 8583 packet
 Param out:
   pSt8583          pointer to point to struct 8583
 Return Code:
    0               Ok
    <0              message header error
    -1000           length of unpacked not same as actural data
    <-1000          length of msg field overflow
    <-2000          attributes define error
****************************************************************************/
INT32 PubUnPack8583(FIELD_ATTR *pDataAttr, char *pusIn, uint uiInLen, void *pSt8583);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* _PACK8583_H */

// end of file
