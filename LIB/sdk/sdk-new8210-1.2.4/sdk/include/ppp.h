/******************************************************************************
 * ppp.h
 * 
 * Copyright (c) 2011 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2011-01-15   1.0.0    Li.Hua       written
 ******************************************************************************/


#ifndef __PPP_H__
#define __PPP_H__

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define PPP_ALG_PAP					0x01
#define PPP_ALG_CHAP				0x02
#define PPP_ALG_MSCHAPV1			0x04
#define PPP_ALG_MSCHAPV2			0x08

#define PPP_LINK_BUILDING			1032



int ppp_open(const char *devname, const char *chat,
			 const char *user, const char *passwd,
			 uint32_t auth, int timeout);

int ppp_close(const char *devname);
int ppp_check(const char *devname);
int ppp_set_dns(int mode);


#ifdef __cplusplus
 }
#endif /* __cplusplus */

#endif /* __PPP_H__ */


