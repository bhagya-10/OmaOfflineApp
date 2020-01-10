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
 * PED 状态定义: 锁定，正常，过期
 */
enum {
	PED_STATUS_LOCK			= 0,
	PED_STATUS_NORMAL,
	PED_STATUS_EXPIRED,
};

#define TYPE_PIN_INTERVAL		0x01		// PIN Input interval
#define TYPE_ACCOUNT_INTERVAL	0x03		// Account data encrypt interval

/* 
 * 加解密算法定义，用0~7位表示
 */
#define TDEA_DECRYPT            0x00000000	// TDES解密
#define TDEA_ENCRYPT            0x00000001	// TDES加密
#define TDEA_NONE               0x00000002	// 不做加解密操作(给ped的api使用)
#define SM4_DECRYPT				0x00000010	// SM4解密
#define SM4_ENCRYPT				0x00000011	// SM4加密
#define SM4_NONE				0x00000012	// 不做加解密操作(给ped的api使用)

/* 
 * 加解密模式定义，用8~15位表示
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
 * 校验算法，用24~31位表示
 */
#define KEY_VERIFY_NONE			0x00000000	// 无
#define KEY_VERIFY_KVC			0x01000000	// KVC use DES
#define KEY_VERIFY_CMAC			0x02000000	// CMAC
#define KEY_VERIFY_SM4_KVC		0x11000000	// KVC use SM4
#define KEY_VERIFY_MASK			0xff000000	// 掩码

/*
 * 密钥类型
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
 * PIN BLOCK 算法定义
 */
#define PIN_BLOCK_FORMAT_0		0x00	//  PIN BLOCK FORMAT 0
#define PIN_BLOCK_FORMAT_1		0x01	//  PIN BLOCK FORMAT 1
#define PIN_BLOCK_FORMAT_2		0x02	//  PIN BLOCK FORMAT 2
#define PIN_BLOCK_FORMAT_3		0x03	//  PIN BLOCK FORMAT 3
#define PIN_BLOCK_FORMAT_4		0x04	//  PIN BLOCK FORMAT 4
#define PIN_BLOCK_FORMAT_EPS	0x0A	//  PIN BLOCK FORMAT EPS

/*
 * MAC BLOCK 算法定义
 */
#define MAC_MODE_1				0x00	//  MAC method 1, TDES-TDES...TDES
#define MAC_MODE_2				0x01	//  MAC method 2, XOR...XOR...TDES
#define MAC_MODE_EMV			0x02	//  MAC for EMV EMV, DES-DES...TDES
#define MAC_MODE_CUP			0x03	//  MAC for CUP, XOR-XOR...TDES(left)-XOR-TDES...
#define MAC_MODE_DUKPT			0x04	//  MAC for DUKPT,Expand, XOR-XOR...TDES(left)-XOR-TDES...

/*
 * 返回值定义
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
    char		Model[16];		// 机器型号
    char		HwVer[16];		// 硬件版本信息
    char		SwVer[16];		// 软件版本信息
    uint32_t	Status;			// PED状态
    uint32_t	MaxBps;			// PED最高支持的通讯波特率
    uint32_t	MaxAppNum;		// 最大可支持的应用数
    uint32_t	MasterKeyNum;	// 最大MK存储数
    uint32_t	PINKeyNum;		// 最大PIN Key存储数
    uint32_t	MACKeyNum;		// 最大MAC Key存储数
    uint32_t	FixPINKeyNum;	// 最大Fix PIN Key存储数
    uint32_t	FixMACKeyNum;	// 最大Fix MAC Key存储数
    uint32_t	DukptKeyNum;	// 最大Dukpt Key存储数
	uint32_t	Ctime;			// 密钥生成时间
	uint32_t	MagKeyTime;		// 磁卡密钥生成时间
	uint32_t	MagKeyStatus;	// 磁卡密钥是否有效
	int16_t		MaxTemp;		// 高温限制
	int16_t		MinTemp;		// 低温限制
	int16_t		CurTemp;		// 当前CPU的温度
    uint8_t		Res[154];		// 预留
}PedConfig_t;   //  反馈给外部的PED信息256 bytes

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


/* 获取随机数 */
uint32_t ped_get_random(void);
int ped_get_random_nr(uint32_t bytes, void *data);

/* 获取PED的配置信息 */
int ped_get_config(PedConfig_t *config);

/* 仅root特权程序能调用 */
int ped_get_status(uint32_t *status);

/* 设置 PIN 输入的位置和大小 */
int ped_set_pin_input_region(int x, int y, int width, int height);

/* 在系统被攻击时，普通程序调用返回拒绝 */
int ped_format(void);

/* 只清除当前用户下所有密钥数据，不影响其它用户 */
int ped_clear_user_keys(void);

/* 删除指定的一个密钥 */
int ped_delete_key(uint32_t KeyType, uint32_t KeyIndex);

/* 获取可使用敏感服务的间隔时间 */
int ped_get_sensitive_timer(uint32_t SensitiveType);

/* 设置PIN输入超时时间 30~120 秒 */
int ped_set_pin_input_timeout(uint32_t timeout);

/* 获取dukpt的KSN，调用一次可以允许计算一次PIN和MAC */
int ped_get_dukpt_ksn(uint32_t KeyIndex, void *Ksn);

/* 使用dukpt密钥加密数据 */
int ped_dukpt_encrypt_request(int index, uint32_t DataLen, const void *DataIn, void *DataOut);

/* 使用dukpt密钥解密应答数据 */
int ped_dukpt_decrypt_response(int index, uint32_t DataLen, const void *DataIn, void *DataOut);

/* 使用dukpt密钥计算应答数据的mac */
int ped_dukpt_calc_mac_response(int index, uint32_t Mode, uint32_t DataLen, const void *DataIn, void *MacBlock);

/* 使用dukpt密钥验证应答数据的MAC是否正确，密钥和ped_get_mac使用的相同 */
int ped_dukpt_verify_mac_response(int index, uint32_t Mode, uint32_t DataLen, 
                                  const void *DataIn, const void *MacBlock, const void *MacValMask);

/* 保存Master Key加密的PIN Key */
int ped_write_pin_key(uint32_t MasterKeyID, uint32_t PinKeyID,
					  uint32_t PinKeyLen,   const void *PinKey);
int ped_write_pin_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);
int ped_write_sm4_pin_key(uint32_t MasterKeyID, uint32_t DestKeyID,
					  uint32_t KeyLen, const void *KeyData);
int ped_write_sm4_pin_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

/* 保存Master Key加密的MAC Key */
int ped_write_mac_key(uint32_t MasterKeyID, uint32_t MacKeyID,
					  uint32_t MacKeyLen,   const void *MacKey);
int ped_write_mac_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID, 
						 uint32_t KeyLen, const void *KeyData);
int ped_write_sm4_mac_key(uint32_t MasterKeyID, uint32_t DestKeyID, 
					  uint32_t KeyLen,      const void *KeyData);
int ped_write_sm4_mac_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID, 
						 uint32_t KeyLen, const void *KeyData);


/* 保存Master Key加密的Account Data Key */
int ped_write_account_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

int ped_write_account_key(uint32_t MasterKeyID, uint32_t DestKeyID,
								uint32_t KeyLen,   const void *KeyData);

#if !defined(CONFIG_PCI_3_X)	
/* 密钥注入 */
/**
 * @fn          ped_write_master_key
 * @brief       注入DES算法的主密钥
 * @param in    Mode: 加密模式为 TDEA_DECRYPT | (KEY_VERIFY_NONE, KEY_VERIFY_KVC, KEY_VERIFY_CMAC)
 *					  明文模式为 TDEA_NONE
 *				MasterKeyID: 如果是加密模式，该参数指用于解密的主密钥索引
 *							 如果是明文模式，该参数没有意义
 *				DestKeyID: 要注入密钥的密钥索引
 *				KeyLen:    密钥长度，DES密钥长度必须是8,16,24字节
 *				KeyData:   密钥的密文或明文
 * @param out   无
 * @return      0    - 成功
 *				其它 - 失败，查看errno
 * @note        
 */
int ped_write_master_key(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);
/**
 * @fn          ped_write_sm4_master_key
 * @brief       注入SM4算法的主密钥
 * @param in    Mode: 加密模式为 SM4_DECRYPT | (KEY_VERIFY_NONE, KEY_VERIFY_SM4_KVC)
 *					  明文模式为 SM4_NONE
 *				MasterKeyID: 如果是加密模式，该参数指用于解密的主密钥索引
 *							 如果是明文模式，该参数没有意义
 *				DestKeyID: 要注入密钥的密钥索引
 *				KeyLen:    密钥长度，SM4密钥长度必须是16字节
 *				KeyData:   密钥的密文或明文
 * @param out   无
 * @return      0    - 成功
 *				其它 - 失败，查看errno
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


/* 加密用户PIN */
int ped_get_pin(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
				const char *CardNo, void *PinBlock);

int ped_get_pin_fix(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
					const char *CardNo, void *PinBlock);

/**
 * @fn          ped_get_pin_sm4
 * @brief       计算PIN Block, 使用SM4算法
 * @param in    KeyIndex: 密钥索引
 *				Mode: PIN_BLOCK_FORMAT_0, 其它模式不支持
 *				ExpectPinLen: 允许输入pin的长度列表，[数字<,数字>]
 *				CardNo: 处理过的卡号，16字节，前面4字节无效，只取后面12字节
 * @param out   PinBlock: 16字节PIN Block
 * @return      0    - 成功
 *				其它 - 失败，查看errno
 * @note        SM4算法计算出来的PIN Block是16字节，该函数隐式使用SM4的PIN密钥进行加密
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

/* 计算数据MAC */
int ped_get_mac(uint32_t KeyIndex, uint32_t Mode, 
				uint32_t DataLen, const void *DataIn, void *MacOut);

int ped_get_mac_fix(uint32_t KeyIndex, uint32_t Mode,
					uint32_t DataLen, const void *DataIn, void *MacOut);

/**
 * @fn          ped_get_mac_sm4
 * @brief       计算MAC, 使用SM4算法
 * @param in    KeyIndex: 密钥索引
 *				Mode: MAC_MODE_1, MAC_MODE_EMV, MAC_MODE_CUP, 其它不支持
 *				DataLen: 数据长度，必须是16的倍数，如果不足16字节，自行补足16字节
 *				DataIn: 要计算的数据
 * @param out   MacOut: 计算出来的MAC, 16字节
 * @return      0    - 成功
 *				其它 - 失败，查看errno
 * @note        SM4算法计算出来的MAC Block是16字节，该函数隐式使用SM4的MAC密钥进行加密。
 *				要计算的数据需要由调用者填充至16字节，有2中填充标准，一种是全部补充0x00；
 *				另外一种是补充0x80 0x00 0x00 ...
 */
int ped_get_mac_sm4(uint32_t KeyIndex, uint32_t Mode,
					uint32_t DataLen, const void *DataIn, void *MacOut);

int ped_get_mac_fix_sm4(uint32_t KeyIndex, uint32_t Mode,
						uint32_t DataLen, const void *DataIn, void *MacOut);

int ped_get_mac_dukpt(uint32_t KeyIndex, uint32_t Mode,
					  uint32_t DataLen, const void *DataIn, void *MacBlock);


/*Account Data 加解密*/
int ped_account_encrypt(uint32_t Mode, uint32_t KeyIndex,
			  			uint32_t DataLen, const void *InBuf, void *OutBuf);

int ped_account_encrypt_fix(uint32_t Mode, uint32_t KeyIndex,
			  			uint32_t DataLen, const void *InBuf, void *OutBuf);

/* 签名机专用函数，普通用户调用会被拒绝 */
int ped_kmm_dencrypt(uint32_t Mode, uint32_t DataLen, const void *DataIn, void *DataOut);


#if !defined(CONFIG_PCI_3_X)	
int ped_tdea(uint32_t Mode, uint32_t KeyType, uint32_t KeyIndex,
			 uint8_t IV[8], uint32_t DataLen, const void *InBuf, void *OutBuf);
#endif

/* 该接口提供给密钥注入程序使用，调用者必须是root权限，认证需要接口 */
int ped_inject_key_by_root(uint32_t CUID, uint32_t KeyType, uint32_t KeyIndex,
						   uint32_t KeyLen, const void *PlaintextKey);

/* 用密文密钥加密8字节数据 */
int ped_encrypt_by_cipherkey(uint32_t Mode,	uint32_t MasterIndex,
					const void *CipherKeyData, const void *DataIn, void *DataOut);

/* 检查指定密钥是否存在 */
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

