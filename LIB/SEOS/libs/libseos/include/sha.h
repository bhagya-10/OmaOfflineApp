/****************************************************************************
NAME
	sha.h - hashº¯Êý

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	pyming		2010.09.14		- add precompile process
****************************************************************************/

#ifndef POS_SHA_H
#define POS_SHA_H

#include <stdint.h>

#define SHA_VERSION 1

//in PC or C51 it is 1234
#define SHA_BYTE_ORDER 1234
//in MC68Kvz328 it is 4321
//#define SHA_BYTE_ORDER 4321

/* NIST Secure Hash Algorithm */
/* heavily modified by Uwe Hollerbach <uh@alumni.caltech edu> */
/* from Peter C. Gutmann's implementation as found in */
/* Applied Cryptography by Bruce Schneier */

/* This code is in the public domain */

#define SHA_BLOCKSIZE		64
#define SHA_DIGESTSIZE		20


typedef struct {
	uint32_t	digest[5];				/* message digest */
	uint32_t	count_lo, count_hi;		/* 64-bit bit count */
	uint8_t 	data[SHA_BLOCKSIZE];	/* SHA data buffer */
	int32_t		local;					/* unprocessed amount in data */
} SHA_INFO;

void sha_init(SHA_INFO *sha_info);
void sha_update(SHA_INFO *sha_info, const void *buffer, int count);
void sha_final(void *digest, SHA_INFO *sha_info);

void Hash(const void *DataIn, uint32_t DataInLen, void *DataOut);

#endif

