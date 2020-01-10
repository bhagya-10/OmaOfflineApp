/******************************************************************************
 * rsa.h
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

#ifndef POS_RSA_H
#define POS_RSA_H

#include <stdint.h>

// 返回值定义
#define RE_DATA 0x0401
#define RE_LEN  0x0406

/* RSA key lengths.
 */
#define MIN_RSA_MODULUS_BITS	508
/* #define MAX_RSA_MODULUS_BITS 1024 ** linq modify ->>2048 ***/
#define MAX_RSA_MODULUS_BITS	2048
#define MAX_RSA_MODULUS_LEN		((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS		((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN		((MAX_RSA_PRIME_BITS + 7) / 8)

/* Length of digit in bits */
#define NN_DIGIT_BITS			32
#define NN_HALF_DIGIT_BITS		16
/* Length of digit in bytes */
#define NN_DIGIT_LEN			(NN_DIGIT_BITS / 8)
/* Maximum length in digits */
#define MAX_NN_DIGITS			((MAX_RSA_MODULUS_LEN + NN_DIGIT_LEN - 1) / NN_DIGIT_LEN + 1)
/* Maximum digits */
#define MAX_NN_DIGIT			0xffffffff
#define MAX_NN_HALF_DIGIT		0xffff

/* Macros.
 */
#define LOW_HALF(x)				((x) & MAX_NN_HALF_DIGIT)
#define HIGH_HALF(x)			(((x) >> NN_HALF_DIGIT_BITS) & MAX_NN_HALF_DIGIT)
#define TO_HIGH_HALF(x)			(((NN_DIGIT)(x)) << NN_HALF_DIGIT_BITS)
#define DIGIT_MSB(x)			(uint32_t)(((x) >> (NN_DIGIT_BITS - 1)) & 1)
#define DIGIT_2MSB(x)			(uint32_t)(((x) >> (NN_DIGIT_BITS - 2)) & 3)

#define NN_ASSIGN_DIGIT(a, b, digits) {NN_AssignZero (a, digits); a[0] = b;}
#define NN_EQUAL(a, b, digits)	(! NN_Cmp (a, b, digits))
#define NN_EVEN(a, digits)		(((digits) == 0) || ! (a[0] & 1))

typedef struct     // 公钥结构
{
	uint32_t bits;                   /* length in bits of modulus */
	uint8_t  modulus[256];           /* modulus */
	uint8_t  exponent[4];            /* public exponent */
	uint16_t usCRC;                  /*CRC 校验码*/
} R_RSA_PUBLIC_KEY;

typedef struct    // 私钥结构
{
	uint32_t bits;                   /* length in bits of modulus */
	uint8_t  modulus[256];           /* modulus */
	uint8_t  publicExponent[4];      /* public exponent */
	uint8_t  exponent[256];          /* private exponent */
	uint8_t  prime[2][128];          /* prime factors */
	uint8_t  primeExponent[2][128];  /* exponents for CRT */
	uint8_t  coefficient[128];       /* CRT coefficient */
	uint16_t usCRC;                  /*CRC 校验码*/
} R_RSA_PRIVATE_KEY;


int32_t RSAPublicDecrypt(  void *output,				/* output block */
						 uint32_t *outputLen,			/* length of output block */
						 const void *input,			/* input block */
						 uint32_t inputLen,			/* length of input block */
						 R_RSA_PUBLIC_KEY *publicKey);/* RSA public key */

int32_t RSAPrivateEncrypt ( void *output,					/* output block */
						   uint32_t *outputLen,			/* length of output block */
						   const void *input,				/* input block */
						   uint32_t inputLen,				/* length of input block */
							R_RSA_PRIVATE_KEY *privateKey);	/* RSA private key */


#endif /* POS_RSA_H */

