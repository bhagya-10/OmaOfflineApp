#ifndef MIFARE_PLUS_H_
#define MIFARE_PLUS_H_

#include <stdint.h>

#ifndef	UINT8
#define UINT8		uint8_t		
#endif
#ifndef	UINT32
#define UINT32		uint32_t
#endif
#ifndef	UINT16
#define UINT16		uint16_t
#endif

#ifndef	INT8
#define INT8		int8_t
#endif
#ifndef	INT32
#define INT32		int32_t
#endif
#ifndef	INT16
#define INT16		int16_t
#endif



// ���巵��ֵ
#define MIF_RET_SUCCESS         0       // ������ȷ
#define MIF_RET_TRANSERR        -1      // ͨ��ʧ��
#define MIF_RET_TIMEOUT         -2      // ��ʱ����
#define MIF_RET_PROTERR         -3      // ���ݴ�
#define MIF_RET_MULTICARD       -4      // �࿨����
#define MIF_RET_NOCARD          -5      // �޿�
#define MIF_RET_ERRPARA         -6      // ��������
#define MIF_RET_NOACTIVED       -7      // ��Ƭδ����
#define MIF_RET_NOAUTH          -8      // ����δ��֤
#define MIF_RET_OVERFLOW        -9      // �ڲ����������
#define MIF_RET_AUTHERR         -10     // ��֤ʧ��
#define MIF_RET_VALUEERR        -11     // ֵ����
#define MIF_RET_NOSUPPORTTCL    -12     // ��Tag��֧��T=CLЭ��
#define MIF_RET_NOSUPPORTCMD    -13     // ������Tag��֧��


//FOR Mifare Plus Card Err Code
#define MIF_RET_PLNOAUTH		MIF_RET_NOAUTH		//δ��֤

#define MIF_RET_PLAUTHERR       MIF_RET_AUTHERR     // ��֤����
#define MIF_RET_PLCMDFLOWERR    -16     // ̫���д���������������
#define MIF_RET_PLCRC1ERR       -17     // �����CRC�ڵ�һ��
#define MIF_RET_PLCRC2ERR       -18     // �����CRC�ڵڶ���
#define MIF_RET_PLCONDITIONERR  -19     // �������������
#define MIF_RET_PLMACERR        -20     // PCD���͵�MAC����
#define MIF_RET_PLBNOERR        -21     // PCD���͵Ŀ�Ŵ���
#define MIF_RET_PLNOBNOERR      -22     // PICC�޴˿��
#define MIF_RET_PLLENERR        -23     // PCD�������ݳ��ȴ���
#define MIF_RET_PLLOPERATEERR   -24     // PCD��PICCֵ��ز�������

#define MIF_RET_PLPICCMACERR    -25     // PICC���ص�MAC����
#define MIF_RET_PLNOFIRAUTHERR  -26     // δ����first auth


// AES��֤��������
	//for mifPLAuthInAct,mifPLAuthInProF,mifPLAuthInProN
	//#define MIF_PLUSM1_BLOCK_KEYA				0x00
	//#define MIF_PLUSM1_BLOCK_KEYB				0x01

	#define MIF_PLUSM1_BLOCK_KEYA				'A'
	#define MIF_PLUSM1_BLOCK_KEYB				'B'

	//for mifPLAuthInAct only
	#define MIF_PLUS_AUTHL1_KEY					0x02

	//for mifPLAuthInProF,mifPLAuthInProN only
	#define MIF_PLUS_MASTER_KEY                	0x03
	#define MIF_PLUS_ORIGIN_KEY                	0x04
	#define MIF_PLUS_CONFIG_KEY                	0x05
	#define MIF_PLUS_SWITCHL2_KEY              	0x06
	#define MIF_PLUS_SWITCHL3_KEY              	0x07



//����plus��ר��������
	#define FIRSTAUTH              			0x70
	#define FOLLOWAUTH              		0x76
	#define AUTHPART2              			0x72

	//FOR L0
	#define WRITEPERSO              		0xA8
	#define COMMITPERSO              		0xAA

	//FOR L2
	#define MULTIWRITE              		0xA8
	#define MULTIREAD              			0x38

	//for L3 and ISO14443-4
	#define ENWRITEMAC              		0xA1
	#define ENWRITE              			0xA0
	#define PLWRITEMAC              		0xA3
	#define PLWRITE              			0xA2

	#define ENREADMAC              			0x31//dataencrypt,cmd mac,response mac
	#define ENREAD              			0x34//dataencrypt,no cmd mac,no response mac
	#define PLREADMAC              			0x33//dataplain,cmd mac,response mac
	#define PLREAD              			0x36//dataplain,no cmd mac,no response mac

	#define ENINCREMENTMAC              	0xB7//dataencrypt,cmd mac,response mac
	#define ENINCREMENT              		0xB6//dataencrypt, cmd mac,no response mac

	#define ENDECREMENTMAC              	0xB9//dataencrypt,cmd mac,response mac
	#define ENDECREMENT              		0xB8//dataencrypt, cmd mac,no response mac

	#define RESTOREMAC              		0xC3//cmd mac,response mac
	#define RESTORE              			0xC2// cmd mac,no response mac

	#define TRANSFERMAC              		0xB5//cmd mac,response mac
	#define TRANSFER              			0xB4// cmd mac,no response mac

	#define ENINCREMAC              		0xB1//dataencrypt,cmd mac,response mac
	#define ENINCRE              			0xB0//dataencrypt, cmd mac,no response mac

	#define ENDECREMAC              		0xB3//dataencrypt,cmd mac,response mac
	#define ENDECRE              			0xB2//dataencrypt, cmd mac,no response mac

//�����������
	#define OPINCRETANS              		0x00
	#define OPDECRETANS              		0x01

	#define OPINCREMENT              		0x02
	#define OPDECREMENT              		0x03

	#define OPTRANSFER              		0x04
	#define OPRESTORE              			0x05




//����session key base ���һ�ֽ�����
	#define CRYPTO1SKB						0x33
	#define ENCKEYSKB						0x11

	//�����Ƶ���ʽ��ͬ��������
	#define MACKEYSKB						0x22

//�����Ƿ�Ϊfirst auth����follow auth
	#define FIRSTAUTHTYPE					0x00
	#define FOLLOWAUTHTYPE					0x01

//����L3��ȫ�������ݴ�������
	#define DATAENCRYPT_CMDMAC_RESMAC		0x00//���ݼ��ܣ�cmd mac,response mac
	#define DATAENCRYPT_CMDMAC				0x02//���ݼ��ܣ�cmd mac,no response  mac
	#define DATAENCRYPT						0x03//���ݼ��ܣ�no cmd mac,no response  mac

	#define DATAPLAIN						0x04//�������ģ�no cmd mac,no response  mac
	#define DATAPLAIN_CMDMAC				0x05//�������ģ�cmd mac,no response  mac
	#define DATAPLAIN_CMDMAC_RESMAC			0x06//�������ģ�cmd mac, response  mac

typedef struct 
{
	UINT8 TransID[4];//TI ����ISO-4���һ����֤�����PICC��TIֵ�����ں���VI��һ���֣���һ�����������б��ֲ���
	UINT8 EMCKey[16];//��֤�������Ƶ������������ݼ��ܵ�session key,����L2�����������Ƶ�CRYPTO1Key��session key
	UINT8 MACKey[16];//��֤�������Ƶ��������ڼ���MAC��session key
	UINT8 PICCcap2[6];//��first��֤�еõ���PICCcap2ֵ
	UINT16 Wctr;//����������������д��������������VIһ����
	UINT16 Rctr;//���������������ж���������������VIһ����
	UINT16 FirstAuthFlag;//�Ƿ���й�һ�γɹ���first auth in iso14443-4 mode
	UINT16 PLUSERRNO;//���Ը���ʹ��
}MIF_PLUS_STRUCT;


typedef struct
{
	UINT32 BlockNo;//���
	UINT8 AESKeyType;//��ΪA/B����Կ��master key,configth key,L2 switch key,L3 switch key,SL1 Additional AES auth key
	UINT8 AESKey[16];
	UINT8 M1KeyType;
	UINT8 M1Key[6];
	UINT8 UID[4];//Ŀǰ֧��4�ֽ�UID��
}MIF_PLUSAUTH_STRUCT;




INT32 mifPLAuthInAct(MIF_PLUSAUTH_STRUCT *AuthPara);//auth in active mode,flow auth
INT32 mifPLAuthInPro(const UINT32 AuthType,MIF_PLUSAUTH_STRUCT *AuthPara);//auth in protocol mode,firse auth by aes


//for L0 fun
INT32 mifPLWritePerso(const UINT16 BNo,UINT8 *Data);//write Card config key,master key,(L2 swtich key,L3 swtich key for L1) or other block key or block values
INT32 mifPLCommitPerso(void);//switch up card to L1 or L3

//for L1 fun
//the same with M1 card interface

//for L2 fun
//operate card is the same with M1 card interface
//addtional fun as follow
//���������ӿڽ��������ݿ����
INT32 mifPLMultiWriteBlock(const UINT32 StartBlockNo,const INT32 BlockCount,const UINT8 *BlockData);
INT32 mifPLMultiReadBlock(const UINT32 StartBlockNo, const INT32 BlockCount,UINT8 *BlockData);



//for ISO14443-4 fun
//operate card is differ with M1 card interface
INT32 mifPLWriteBlock(const UINT32 CommType,const UINT16 BNo,const INT32 BlockCount, const UINT8 *BlockData);
INT32 mifPLReadBlock(const UINT32 CommType,const UINT16 BNo,const INT32 BlockCount,UINT8 *BlockData);

//����ֵ�������������ݿ����
INT32 mifPLSetValue(const UINT32 CommType,const UINT32 BNo,const INT32 BlockCount, const UINT32 Values);
INT32 mifPLIncreTrans(const UINT32 CommType,const UINT32 SrcBNo,const UINT32 DesBNo, const UINT32 Values);
INT32 mifPLDecreTrans(const UINT32 CommType,const UINT32 SrcBNo,const UINT32 DesBNo, const UINT32 Values);
INT32 mifPLIncrement(const UINT32 CommType,const UINT32 BNo ,const UINT32 Values);
INT32 mifPLDecrement(const UINT32 CommType,const UINT32 BNo, const UINT32 Values);
INT32 mifPLRestore(const UINT32 CommType,const UINT32 BNo);
INT32 mifPLTransfer(const UINT32 CommType,const UINT32 BNo);

UINT16 GetPlusErrNo(void);


#endif

