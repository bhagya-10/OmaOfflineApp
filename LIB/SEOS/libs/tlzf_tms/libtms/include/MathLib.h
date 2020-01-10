
/****************************************************************************
NAME
	Mathlib.h - 算术运算公共函数

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	pyming		2008.03.14		- add precompile process
****************************************************************************/

#ifndef _MATHLIB_H
#define _MATHLIB_H

#ifndef TRUE
	#define TRUE	1
	#define FALSE	0
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

#ifndef ulong
#define ulong unsigned long
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void TMSPubU16ToBcd(char *Bcd, unsigned short const Src, unsigned int Len);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _UTIL_H */
