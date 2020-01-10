/******************************************************************************
 * md5.h
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

#ifndef POS_MD5_H
#define POS_MD5_H

#include <stdint.h>

#define MD5_DIGEST_SIZE		16
#define MD5_HMAC_BLOCK_SIZE	64
#define MD5_BLOCK_WORDS		16
#define MD5_HASH_WORDS		4

typedef struct {
	uint32_t hash[MD5_HASH_WORDS];
	uint32_t block[MD5_BLOCK_WORDS];
	uint64_t byte_count;
}md5_ctx;


void md5_init(md5_ctx *tfm);
void md5_update(md5_ctx *tfm, const void *data, unsigned int len);
void md5_final(md5_ctx *tfm, void *out);
void md5(const void* DataIn, uint32_t DataInLen, void* DataOut);


#endif

