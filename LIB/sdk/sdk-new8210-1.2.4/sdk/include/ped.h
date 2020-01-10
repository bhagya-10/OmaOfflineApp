/******************************************************************************
 * pedapi.h
 * 
 * Copyright (c) 2012 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2012-08-28   1.0.0    Li.Hua       written
 * 2015-08-12   1.1.0    Li.Hua       Add SM4 Key and related
 ******************************************************************************/

#ifndef __PED_USER_API_H__
#define __PED_USER_API_H__

#include <stdint.h>

#ifdef __cplusplus 
extern "C" { 
#endif 

/* 
 * PED ״̬����: ����������������
 */
enum {
	PED_STATUS_LOCK			= 0,
	PED_STATUS_NORMAL,
	PED_STATUS_EXPIRED,
};

#define TYPE_PIN_INTERVAL		0x01		// PIN Input interval
#define TYPE_ACCOUNT_INTERVAL	0x03		// Account data encrypt interval

/* 
 * �ӽ����㷨���壬��0~7λ��ʾ
 */
#define TDEA_DECRYPT            0x00000000	// TDES����
#define TDEA_ENCRYPT            0x00000001	// TDES����
#define TDEA_NONE               0x00000002	// �����ӽ��ܲ���(��ped��apiʹ��)
#define SM4_DECRYPT				0x00000010	// SM4����
#define SM4_ENCRYPT				0x00000011	// SM4����
#define SM4_NONE				0x00000012	// �����ӽ��ܲ���(��ped��apiʹ��)

/* 
 * �ӽ���ģʽ���壬��8~15λ��ʾ
 */
#define TDEA_MODE_ECB			0x00000000	// TDEA MODE ECB
#define TDEA_MODE_CBC			0x00000100	// TDEA MODE CBC
#define TDEA_MODE_CFB			0x00000200	// TDEA MODE CFB
#define TDEA_MODE_OFB			0x00000300	// TDEA MODE OFB
#define SM4_MODE_ECB			0x00001000	// SM4 MODE ECB
#define SM4_MODE_CBC			0x00001100	// SM4 MODE CBC
#define SM4_MODE_CFB			0x00001200	// SM4 MODE CFB
#define SM4_MODE_OFB			0x00001300	// SM4 MODE OFB

/*
 * У���㷨����24~31λ��ʾ
 */
#define KEY_VERIFY_NONE			0x00000000	// ��
#define KEY_VERIFY_KVC			0x01000000	// KVC use DES
#define KEY_VERIFY_CMAC			0x02000000	// CMAC
#define KEY_VERIFY_SM4_KVC		0x11000000	// KVC use SM4
#define KEY_VERIFY_MASK			0xff000000	// ����

/*
 * ��Կ����
 */
#define KEY_TYPE_SIEK			0x01	// The key to encrypt the internal sensitive infomation(internal use)
#define KEY_TYPE_MASTK			0x02	// MASTER KEY
#define KEY_TYPE_PINK			0x03	// PIN KEY
#define KEY_TYPE_MACK			0x04	// MAC KEY
#define KEY_TYPE_FIXPINK		0x05	// Fixed PIN KEY
#define KEY_TYPE_FIXMACK		0x06	// Fixed MAC KEY
#define KEY_TYPE_DUKPTK			0x07	// DUKPT KEY
#define KEY_TYPE_EMRKEY			0x08	// The key for securty magstripe reader
#define KEY_TYPE_KMMK			0x09	// The key for KMM
#define KEY_TYPE_EAK			0x0A	// Account Data KEY
#define KEY_TYPE_FIXEAK		    0x0B	// Fixed Account Data KEY
#if defined(CONFIG_CUSTOMER_EXADIGM)
#define KEY_TYPE_KEK			0x0C
#define KEY_TYPE_DEK			0x0D
#endif
#if defined(CONFIG_PCI_3_X)
#define KEY_TYPE_CERT			0x0E	// CA Certificate
#endif
#define KEY_TYPE_SM4_MASTK		0x12	// MASTER KEY
#define KEY_TYPE_SM4_PINK		0x13	// PIN KEY of SM4
#define KEY_TYPE_SM4_MACK		0x14	// MAC KEY of SM4
#define KEY_TYPE_SM4_FIXPINK	0x15	// Fixed PIN KEY
#define KEY_TYPE_SM4_FIXMACK	0x16	// Fixed MAC KEY

#define KEY_TYPE_EXPIRED_KEY	0xFE	// The expired key
#define KEY_TYPE_INVALID		0xFF	// Invalid key

/*
 * PIN BLOCK �㷨����
 */
#define PIN_BLOCK_FORMAT_0		0x00	//  PIN BLOCK FORMAT 0
#define PIN_BLOCK_FORMAT_1		0x01	//  PIN BLOCK FORMAT 1
#define PIN_BLOCK_FORMAT_2		0x02	//  PIN BLOCK FORMAT 2
#define PIN_BLOCK_FORMAT_3		0x03	//  PIN BLOCK FORMAT 3
#define PIN_BLOCK_FORMAT_4		0x04	//  PIN BLOCK FORMAT 4
#define PIN_BLOCK_FORMAT_EPS	0x0A	//  PIN BLOCK FORMAT EPS

/*
 * MAC BLOCK �㷨����
 */
#define MAC_MODE_1				0x00	//  MAC method 1, TDES-TDES...TDES
#define MAC_MODE_2				0x01	//  MAC method 2, XOR...XOR...TDES
#define MAC_MODE_EMV			0x02	//  MAC for EMV EMV, DES-DES...TDES
#define MAC_MODE_CUP			0x03	//  MAC for CUP, XOR-XOR...TDES(left)-XOR-TDES...
#define MAC_MODE_DUKPT			0x04	//  MAC for DUKPT,Expand, XOR-XOR...TDES(left)-XOR-TDES...

/*
 * ����ֵ����
 */
#define PED_RET_OK              0x00	//  PED OK
#define PED_RET_BASE_NO			2000
#define PED_RET_LOCKED          (PED_RET_BASE_NO +  1)	//  PED Locked
#define PED_RET_ERROR           (PED_RET_BASE_NO +  2)	//  The others error
#define PED_RET_COMMERR         (PED_RET_BASE_NO +  3)	//  Communicate with PED failed
#define PED_RET_NEEDAUTH        (PED_RET_BASE_NO +  4)	//  Need auth before use sensitive service or expired
#define PED_RET_AUTHERR         (PED_RET_BASE_NO +  5)	//  PED auth failed
#define PED_RET_WEAK_KEY		(PED_RET_BASE_NO +  6)	//  weak length key
#define PED_RET_COLLISION_KEY	(PED_RET_BASE_NO +  7)	//  collision key
#define PED_RET_KEYINDEXERR     (PED_RET_BASE_NO +  8)	//  The index of key incorrect
#define PED_RET_NOKEY           (PED_RET_BASE_NO +  9)	//  No designated key in PED
#define PED_RET_KEYFULL         (PED_RET_BASE_NO + 10)	//  Key space is full
#define PED_RET_OTHERAPPKEY     (PED_RET_BASE_NO + 11)	//  The designated key is not belong to this APP
#define PED_RET_KEYLENERR       (PED_RET_BASE_NO + 12)	//  The key length error
#define PED_RET_NOPIN           (PED_RET_BASE_NO + 13)	//  Card holder press ENTER directly when enter PIN(no PIN)
#define PED_RET_CANCEL          (PED_RET_BASE_NO + 14)	//  Card holder press CANCEL to quit enter PIN
#define PED_RET_TIMEOUT         (PED_RET_BASE_NO + 15)	//  Timeout
#define PED_RET_NEEDWAIT        (PED_RET_BASE_NO + 16)	//  Two frequent between 2 sensitive API
#define PED_RET_KEYOVERFLOW     (PED_RET_BASE_NO + 17)	//  DUKPT KEY overflow
#define PED_RET_NOCARD          (PED_RET_BASE_NO + 18)	//  No ICC
#define PED_RET_ICCNOTPWRUP     (PED_RET_BASE_NO + 19)	//  ICC no power up
#define PED_RET_PARITYERR       (PED_RET_BASE_NO + 20)	//  The parity incorrect
#define PED_RET_UNSUPPORTED		(PED_RET_BASE_NO + 255)	//  can not support


typedef struct
{
    char		Model[16];		// �����ͺ�
    char		HwVer[16];		// Ӳ���汾��Ϣ
    char		SwVer[16];		// ����汾��Ϣ
    uint32_t	Status;			// PED״̬
    uint32_t	MaxBps;			// PED���֧�ֵ�ͨѶ������
    uint32_t	MaxAppNum;		// ����֧�ֵ�Ӧ����
    uint32_t	MasterKeyNum;	// ���MK�洢��
    uint32_t	PINKeyNum;		// ���PIN Key�洢��
    uint32_t	MACKeyNum;		// ���MAC Key�洢��
    uint32_t	FixPINKeyNum;	// ���Fix PIN Key�洢��
    uint32_t	FixMACKeyNum;	// ���Fix MAC Key�洢��
    uint32_t	DukptKeyNum;	// ���Dukpt Key�洢��
	uint32_t	Ctime;			// ��Կ����ʱ��
	uint32_t	MagKeyTime;		// �ſ���Կ����ʱ��
	uint32_t	MagKeyStatus;	// �ſ���Կ�Ƿ���Ч
	int16_t		MaxTemp;		// ��������
	int16_t		MinTemp;		// ��������
	int16_t		CurTemp;		// ��ǰCPU���¶�
    uint8_t		Res[154];		// Ԥ��
}PedConfig_t;   //  �������ⲿ��PED��Ϣ256 bytes

typedef struct {
     uint32_t	modlen;           // The module length of PIN encryption public key,
     uint8_t	mod[256];         // The module data of PIN encryption public key,
     uint32_t	explen;           // The exponet length of PIN encryption public key,
     uint8_t	exp[4];           // The exponet data of PIN encryption public key,
     uint8_t	iccrandom[8];     // The random data from ICC
} RsaPinKey_t;

typedef struct {
	uint8_t 	cla;
	uint8_t 	ins;
	uint8_t 	p1;
	uint8_t		p2;
	uint8_t 	leflg;
	uint8_t 	le;
	RsaPinKey_t rsakey;
} IccOfflinePinApdu_t;


/* ��ȡ����� */
uint32_t ped_get_random(void);
int ped_get_random_nr(uint32_t bytes, void *data);

/* ��ȡPED��������Ϣ */
int ped_get_config(PedConfig_t *config);

/* ��root��Ȩ�����ܵ��� */
int ped_get_status(uint32_t *status);

/* ���� PIN �����λ�úʹ�С */
int ped_set_pin_input_region(int x, int y, int width, int height);

/* ��ϵͳ������ʱ����ͨ������÷��ؾܾ� */
int ped_format(void);

/* ֻ�����ǰ�û���������Կ���ݣ���Ӱ�������û� */
int ped_clear_user_keys(void);

/* ɾ��ָ����һ����Կ */
int ped_delete_key(uint32_t KeyType, uint32_t KeyIndex);

/* ��ȡ��ʹ�����з���ļ��ʱ�� */
int ped_get_sensitive_timer(uint32_t SensitiveType);

/* ����PIN���볬ʱʱ�� 30~120 �� */
int ped_set_pin_input_timeout(uint32_t timeout);

/* ��ȡdukpt��KSN������һ�ο����������һ��PIN��MAC */
int ped_get_dukpt_ksn(uint32_t KeyIndex, void *Ksn);

/* ʹ��dukpt��Կ�������� */
int ped_dukpt_encrypt_request(int index, uint32_t DataLen, const void *DataIn, void *DataOut);

/* ʹ��dukpt��Կ����Ӧ������ */
int ped_dukpt_decrypt_response(int index, uint32_t DataLen, const void *DataIn, void *DataOut);

/* ʹ��dukpt��Կ����Ӧ�����ݵ�mac */
int ped_dukpt_calc_mac_response(int index, uint32_t Mode, uint32_t DataLen, const void *DataIn, void *MacBlock);

/* ʹ��dukpt��Կ��֤Ӧ�����ݵ�MAC�Ƿ���ȷ����Կ��ped_get_macʹ�õ���ͬ */
int ped_dukpt_verify_mac_response(int index, uint32_t Mode, uint32_t DataLen, 
                                  const void *DataIn, const void *MacBlock, const void *MacValMask);

/* ����Master Key���ܵ�PIN Key */
int ped_write_pin_key(uint32_t MasterKeyID, uint32_t PinKeyID,
					  uint32_t PinKeyLen,   const void *PinKey);
int ped_write_pin_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);
int ped_write_sm4_pin_key(uint32_t MasterKeyID, uint32_t DestKeyID,
					  uint32_t KeyLen, const void *KeyData);
int ped_write_sm4_pin_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

/* ����Master Key���ܵ�MAC Key */
int ped_write_mac_key(uint32_t MasterKeyID, uint32_t MacKeyID,
					  uint32_t MacKeyLen,   const void *MacKey);
int ped_write_mac_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID, 
						 uint32_t KeyLen, const void *KeyData);
int ped_write_sm4_mac_key(uint32_t MasterKeyID, uint32_t DestKeyID, 
					  uint32_t KeyLen,      const void *KeyData);
int ped_write_sm4_mac_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID, 
						 uint32_t KeyLen, const void *KeyData);


/* ����Master Key���ܵ�Account Data Key */
int ped_write_account_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

int ped_write_account_key(uint32_t MasterKeyID, uint32_t DestKeyID,
								uint32_t KeyLen,   const void *KeyData);

#if !defined(CONFIG_PCI_3_X)	
/* ��Կע�� */
/**
 * @fn          ped_write_master_key
 * @brief       ע��DES�㷨������Կ
 * @param in    Mode: ����ģʽΪ TDEA_DECRYPT | (KEY_VERIFY_NONE, KEY_VERIFY_KVC, KEY_VERIFY_CMAC)
 *					  ����ģʽΪ TDEA_NONE
 *				MasterKeyID: ����Ǽ���ģʽ���ò���ָ���ڽ��ܵ�����Կ����
 *							 ���������ģʽ���ò���û������
 *				DestKeyID: Ҫע����Կ����Կ����
 *				KeyLen:    ��Կ���ȣ�DES��Կ���ȱ�����8,16,24�ֽ�
 *				KeyData:   ��Կ�����Ļ�����
 * @param out   ��
 * @return      0    - �ɹ�
 *				���� - ʧ�ܣ��鿴errno
 * @note        
 */
int ped_write_master_key(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);
/**
 * @fn          ped_write_sm4_master_key
 * @brief       ע��SM4�㷨������Կ
 * @param in    Mode: ����ģʽΪ SM4_DECRYPT | (KEY_VERIFY_NONE, KEY_VERIFY_SM4_KVC)
 *					  ����ģʽΪ SM4_NONE
 *				MasterKeyID: ����Ǽ���ģʽ���ò���ָ���ڽ��ܵ�����Կ����
 *							 ���������ģʽ���ò���û������
 *				DestKeyID: Ҫע����Կ����Կ����
 *				KeyLen:    ��Կ���ȣ�SM4��Կ���ȱ�����16�ֽ�
 *				KeyData:   ��Կ�����Ļ�����
 * @param out   ��
 * @return      0    - �ɹ�
 *				���� - ʧ�ܣ��鿴errno
 * @note        
 */
int ped_write_sm4_master_key(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

int ped_inject_pin_key(uint32_t PinKeyID, uint32_t PinKeyLen, const void *PinKey);

int ped_inject_mac_key(uint32_t MacKeyID, uint32_t MacKeyLen, const void *MacKey);

int ped_inject_sm4_pin_key(uint32_t PinKeyID, uint32_t PinKeyLen, const void *PinKey);

int ped_inject_sm4_mac_key(uint32_t PinKeyID, uint32_t PinKeyLen, const void *PinKey);

int ped_inject_account_key(uint32_t DataKeyID, uint32_t DataKeyLen, const void *DataKey);

int ped_create_dukpt_key(uint32_t KeyIndex, const void *InitKey, 
						 uint32_t KeyLen,   const void *Ksn);
#endif


/* �����û�PIN */
int ped_get_pin(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
				const char *CardNo, void *PinBlock);

int ped_get_pin_fix(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
					const char *CardNo, void *PinBlock);

/**
 * @fn          ped_get_pin_sm4
 * @brief       ����PIN Block, ʹ��SM4�㷨
 * @param in    KeyIndex: ��Կ����
 *				Mode: PIN_BLOCK_FORMAT_0, ����ģʽ��֧��
 *				ExpectPinLen: ��������pin�ĳ����б�[����<,����>]
 *				CardNo: ������Ŀ��ţ�16�ֽڣ�ǰ��4�ֽ���Ч��ֻȡ����12�ֽ�
 * @param out   PinBlock: 16�ֽ�PIN Block
 * @return      0    - �ɹ�
 *				���� - ʧ�ܣ��鿴errno
 * @note        SM4�㷨���������PIN Block��16�ֽڣ��ú�����ʽʹ��SM4��PIN��Կ���м���
 */
int ped_get_pin_sm4(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
				const char *CardNo, void *PinBlock);

int ped_get_pin_fix_sm4(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
					const char *CardNo, void *PinBlock);

int ped_get_pin_dukpt(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen,  
					  const void *CardNo, void *PinBlock);

int ped_icc_offline_plain_pin(int iccfd, const char *ExpectPinLen,
							  const IccOfflinePinApdu_t *icc,
							  uint32_t *rsplen, void *rspdata);

int ped_icc_offline_cipher_pin(int iccfd, const char *ExpectPinLen,
							   const IccOfflinePinApdu_t *icc,
							   uint32_t *rsplen, void *rspdata);

/* ��������MAC */
int ped_get_mac(uint32_t KeyIndex, uint32_t Mode, 
				uint32_t DataLen, const void *DataIn, void *MacOut);

int ped_get_mac_fix(uint32_t KeyIndex, uint32_t Mode,
					uint32_t DataLen, const void *DataIn, void *MacOut);

/**
 * @fn          ped_get_mac_sm4
 * @brief       ����MAC, ʹ��SM4�㷨
 * @param in    KeyIndex: ��Կ����
 *				Mode: MAC_MODE_1, MAC_MODE_EMV, MAC_MODE_CUP, ������֧��
 *				DataLen: ���ݳ��ȣ�������16�ı������������16�ֽڣ����в���16�ֽ�
 *				DataIn: Ҫ���������
 * @param out   MacOut: ���������MAC, 16�ֽ�
 * @return      0    - �ɹ�
 *				���� - ʧ�ܣ��鿴errno
 * @note        SM4�㷨���������MAC Block��16�ֽڣ��ú�����ʽʹ��SM4��MAC��Կ���м��ܡ�
 *				Ҫ�����������Ҫ�ɵ����������16�ֽڣ���2������׼��һ����ȫ������0x00��
 *				����һ���ǲ���0x80 0x00 0x00 ...
 */
int ped_get_mac_sm4(uint32_t KeyIndex, uint32_t Mode,
					uint32_t DataLen, const void *DataIn, void *MacOut);

int ped_get_mac_fix_sm4(uint32_t KeyIndex, uint32_t Mode,
						uint32_t DataLen, const void *DataIn, void *MacOut);

int ped_get_mac_dukpt(uint32_t KeyIndex, uint32_t Mode,
					  uint32_t DataLen, const void *DataIn, void *MacBlock);


/*Account Data �ӽ���*/
int ped_account_encrypt(uint32_t Mode, uint32_t KeyIndex,
			  			uint32_t DataLen, const void *InBuf, void *OutBuf);

int ped_account_encrypt_fix(uint32_t Mode, uint32_t KeyIndex,
			  			uint32_t DataLen, const void *InBuf, void *OutBuf);

/* ǩ����ר�ú�������ͨ�û����ûᱻ�ܾ� */
int ped_kmm_dencrypt(uint32_t Mode, uint32_t DataLen, const void *DataIn, void *DataOut);


#if !defined(CONFIG_PCI_3_X)	
int ped_tdea(uint32_t Mode, uint32_t KeyType, uint32_t KeyIndex,
			 uint8_t IV[8], uint32_t DataLen, const void *InBuf, void *OutBuf);
#endif

/* �ýӿ��ṩ����Կע�����ʹ�ã������߱�����rootȨ�ޣ���֤��Ҫ�ӿ� */
int ped_inject_key_by_root(uint32_t CUID, uint32_t KeyType, uint32_t KeyIndex,
						   uint32_t KeyLen, const void *PlaintextKey);

/* ��������Կ����8�ֽ����� */
int ped_encrypt_by_cipherkey(uint32_t Mode,	uint32_t MasterIndex,
					const void *CipherKeyData, const void *DataIn, void *DataOut);

/* ���ָ����Կ�Ƿ���� */
int ped_check_key(uint32_t KeyIndex, uint32_t KeyType, uint32_t Mode);

int ped_rsa_generate(int index, int keybits, int exponent, int *publen, void *pubkey);

int ped_rsa_inject_masterkey(uint32_t index,      uint32_t MkIndex,
							 uint32_t cipherlen,  const void *cipher,
							 const uint8_t n1[8], uint8_t n2[8]);

#if defined(CONFIG_CUSTOMER_EXADIGM)
int NP_MultiAddKEKbySlot(int key_index, unsigned char* kek_key, int  key_size);
int NP_MultiAddDEKbySlot(int key_index, unsigned char* dek_key, int key_size);
int NP_MultiReadKEKbySlot(int key_index, unsigned char* kek_key, int *key_size);
int NP_MultiReadDEKbySlot(int key_index, unsigned char* dek_key, int *key_size);
int NP_MultiDeleteKEKbySlot(int key_index);
int NP_MultiDeleteDEKbySlot(int key_index);
#endif


int ped_write_cert(uint32_t Index, uint32_t Uid, uint32_t KeyLen, const void *KeyData);
int ped_read_cert(uint32_t Index, void *KeyData);

#ifdef __cplusplus 
}
#endif 

#endif /* __PED_USER_API_H__ */

