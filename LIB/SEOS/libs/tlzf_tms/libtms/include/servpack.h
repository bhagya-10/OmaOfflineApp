/******************************************************************************
 * servpack.h
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

#ifndef __SERVER_PACK_H__
#define __SERVER_PACK_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct tagServPack	ServPack_t;

struct tagServPack {
	uint32_t	space[16];
};

int ServPack_Init(ServPack_t *sp);
int ServPack_Reset(ServPack_t *sp);
int ServPack_Process(ServPack_t *sp, const void *data, uint32_t nr);
int ServPack_Read(ServPack_t *sp, void *data, uint32_t nr);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SERVER_PACK_H__ */

