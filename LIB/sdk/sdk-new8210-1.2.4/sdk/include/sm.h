/**
 * @file    sm3.h
 *          
 * @author  Li.Hua <lihua_338@163.com>.
 *          
 * @date    2015-09-08
 *          
 * @brief   
 *          
 * @note    
 *          
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 *
 */

#ifndef HARDWARE_SM3_H
#define HARDWARE_SM3_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int		len;
	void	*key;
} SM2_PriKey_t ;

typedef struct {
	int		len;
	void	*x;
	void	*y;
} SM2_PubKey_t ;

/*
 * SM2 接口函数
 * 非对密钥称算法
 */
 
/**
 * @fn			ped_sm2_genkey
 * @brief		generate SM2 key pair.
 * @param in	PriKeyExist: 0-generate private key by random
 *							 1-private was specifed, and used to generate public key
 *				PriKey:		if PriKeyExist=1, it pointer to private key
 * @param out	PriKey:		if PriKeyExist=0, output private key
 *				PubKey:		output public key
 * @return		0 - successfull
 *				else - failure
 * @note		
 */
int ped_sm2_genkey(SM2_PriKey_t *PriKey, SM2_PubKey_t *PubKey, int PriKeyExist);

/**
 * @fn			ped_sm2_pubkey_encrypt
 * @brief		encrypt data using sm2 public key
 * @param in	PubKey: 	public key
 *				InLen:		length of DataIn
 *				DataIn: 	data to be encrypted
 * @param out	OutLen: 	length of ciphertext, normally it equal InLen + 96
 *				DataOut:	ciphertext
 * @return		0 - successfull
 *				else - failure
 * @note		
 */
int ped_sm2_pubkey_encrypt(const SM2_PubKey_t *PubKey,
						   uint32_t InLen,   const void *DataIn,
						   uint32_t *OutLen, void *DataOut);

/**
 * @fn			ped_sm2_prikey_decrypt
 * @brief		decrypt data using sm2 private key
 * @param in	PriKey:		private key
 *				InLen:		length of DataIn
 *				DataIn:		data to be decrypted
 * @param out	OutLen:		length of plaintext, normally it equal InLen - 96
 *				DataOut:	plaintext
 * @return		0 - successfull
 *				else - failure
 * @note		
 */
int ped_sm2_prikey_decrypt(const SM2_PriKey_t *PriKey,
						   uint32_t InLen,   const void *DataIn,
						   uint32_t *OutLen, void *DataOut);

/**
 * @fn			ped_sm2_get_hash_value
 * @brief		calculate SM3 value
 * @param in	PubKey:		public key
 *				Msg:		user message
 *				MsgLen:		length of Msg
 *				ID:			user ID
 *				IDLen:		length of ID
 * @param out	sm3value:	sm3 value
 * @return		0 - successfull
 *				else - failure
 * @note		
 */
int ped_sm2_get_hash_value(const SM2_PubKey_t *PubKey,
						   const void *Msg, uint32_t MsgLen,
						   const void *ID,  uint32_t IDLen,
						   void *sm3value);

/**
 * @fn			ped_sm2_sign
 * @brief		sign message using private key
 * @param in	PriKey:		private key
 *				Hash:		calcuate by call ped_sm2_get_hash_value
 *				HashLen:	length of Hash
 * @param out	pResult:	signature data
 *				pResultLen:	length of pResult
 * @return		0 - successfull
 *				else - failure
 * @note		
 */
int ped_sm2_sign(const SM2_PriKey_t *PriKey,
				 const void *Hash,	uint32_t HashLen,                  
				 void *pResult,		uint32_t *pResultLen);

/**
 * @fn			ped_sm2_verify
 * @brief		verify signature using sm2 public key
 * @param in	PubKey:	public key
 *				PubKeyLen:	length of public key, 64 bytes
 *				Hash:		calcuate by call ped_sm2_get_hash_value
 *				HashLen:	length of Hash
 *				Sign:		signature data, it was generate by ped_sm2_sign
 *				SignLen:	length of Sign
 * @param out	
 * @return		0 - verify ok
 *				else - verify failed or other error
 * @note		
 */
int ped_sm2_verify(const void *PubKey, int PubKeyLen,
				   const void *Hash,   int HashLen,
				   const void *Sign,   int SignLen);


/*
 * SM3 接口函数
 * 哈希算法
 */
typedef struct {
	uint8_t	context[256];
} SM3_CONTEXT_t;

int ped_sm3_init(SM3_CONTEXT_t *ctx);
int ped_sm3_update(SM3_CONTEXT_t *ctx, const void *data, int len);
int ped_sm3_final(SM3_CONTEXT_t *ctx, uint8_t output[32]);


/*
 * SM4 接口函数
 * 对称密钥算法
 */
int ped_sm4(uint32_t Mode, uint32_t KeyType, uint32_t KeyIndex,
			uint8_t IV[16], uint32_t DataLen, const void *InBuf, void *OutBuf);

int ped_sm4_calc(uint32_t Mode, uint8_t IV[16], uint32_t KeyLen, const void *Key,
				 uint32_t InLen,  const void *input, void *output);



#ifdef __cplusplus
}
#endif

#endif

