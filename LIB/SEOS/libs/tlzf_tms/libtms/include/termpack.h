/******************************************************************************
 * termpack.h
 * 
 * Copyright (c) 2013 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2013-08-04   1.0.0    Li.Hua       written
 ******************************************************************************/

#include <stdint.h>

#ifndef __TERMINAL_PACK_H__
#define __TERMINAL_PACK_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct tagTermPack	TermPack_t;

struct tagTermPack {
	uint32_t	space[16];
};

int TermPack_Init(TermPack_t *tp);
int TermPack_Reset(TermPack_t *tp);
int TermPack_Process(TermPack_t *tp, const void *data, uint32_t nr);
int TermPack_Read(TermPack_t *tp, void *data, uint32_t nr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TERMINAL_PACK_H__ */

