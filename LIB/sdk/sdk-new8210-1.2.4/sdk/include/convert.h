/******************************************************************************
 * convert.h
 * 
 * Copyright (c) 2010 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2010-07-12   1.0.0    Li.Hua       written
 ******************************************************************************/

#ifndef CHARSET_CONVERT_H
#define CHARSET_CONVERT_H

#include <sys/types.h>
#include <iconv.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int charset_convert(
			const char *from,   const char *to,
			const void *inbuf,  size_t inbuf_size,
			void       *outbuf, size_t outbuf_size
	);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* end for CHARSET_CONVERT_H */

