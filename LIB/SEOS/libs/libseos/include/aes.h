/******************************************************************************
 * aes.h
 * 
 * Copyright (c) 2011 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2011-08-18   1.0.0    Li.Hua       written
 ******************************************************************************/

#ifndef POS_AES_H
#define POS_AES_H

#include <stdint.h>

#define GETU32(pt) (((uint32_t)(pt)[0] << 24) ^ ((uint32_t)(pt)[1] << 16) ^ ((uint32_t)(pt)[2] <<  8) ^ ((uint32_t)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (uint8_t)((st) >> 24); (ct)[1] = (uint8_t)((st) >> 16); (ct)[2] = (uint8_t)((st) >>  8); (ct)[3] = (uint8_t)(st); }


#define AES_ENCRYPT	1
#define AES_DECRYPT	0

#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16

typedef struct {
		unsigned int rd_key[4 *(AES_MAXNR + 1)];
		int rounds;
} aes_key_t;

#ifndef AES_KEY
#define AES_KEY aes_key_t
#endif

int aes_set_encrypt_key(const unsigned char *userKey, 
						const int bits, AES_KEY *key);
int aes_set_decrypt_key(const unsigned char *userKey, 
						const int bits, AES_KEY *key);
void aes_encrypt(const unsigned char *in, unsigned char *out,
				const AES_KEY *key);
void aes_decrypt(const unsigned char *in, unsigned char *out,
				const AES_KEY *key);

void aes_ecb_encrypt(const unsigned char *in, unsigned char *out, 
					const AES_KEY *key, const int enc);

void aes_cbc_encrypt(const unsigned char *in, unsigned char *out, 
					const unsigned long length, const AES_KEY *key,	
					unsigned char *ivec, const int enc);

void aes_cfb128_encrypt(const unsigned char *in, unsigned char *out, 
					const unsigned long length, const AES_KEY *key,	
					unsigned char *ivec, int *num, const int enc);

void aes_cfb1_encrypt(const unsigned char *in, unsigned char *out,	
					const unsigned long length, const AES_KEY *key,	
					unsigned char *ivec, int *num, const int enc);

void aes_cfb8_encrypt(const unsigned char *in, unsigned char *out,	
					const unsigned long length, const AES_KEY *key,	
					unsigned char *ivec, int *num, const int enc);

void aes_cfbr_encrypt_block(const unsigned char *in, unsigned char *out, 
							const int nbits, const AES_KEY *key, 
							unsigned char *ivec, const int enc);

void aes_ofb128_encrypt(const unsigned char *in, unsigned char *out, 
						const unsigned long length, const AES_KEY *key,	
						unsigned char *ivec, int *num);

void aes_ctr128_encrypt(const unsigned char *in, unsigned char *out,
						const unsigned long length, const AES_KEY *key,
						unsigned char ivec[AES_BLOCK_SIZE],
						unsigned char ecount_buf[AES_BLOCK_SIZE],
						unsigned int *num);

/* For IGE, see also http://www.links.org/files/openssl-ige.pdf */
/* NB: the IV is _two_ blocks long */
void aes_ige_encrypt(const unsigned char *in, unsigned char *out,
					const unsigned long length, const AES_KEY *key,
					unsigned char *ivec, const int enc);

/* NB: the IV is _four_ blocks long */
void aes_bi_ige_encrypt(const unsigned char *in, unsigned char *out,
						const unsigned long length, 
						const AES_KEY *key, const AES_KEY *key2, 
						const unsigned char *ivec, const int enc);


#endif


