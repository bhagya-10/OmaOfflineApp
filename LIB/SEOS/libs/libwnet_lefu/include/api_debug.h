/******************************************************************************
 * api_debug.h
 * 
 * Copyright (c) 2013 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2014-06-18   1.0.0    Tim.zhang      written
 ******************************************************************************/

#ifndef __API_DEBUG_H__
#define __API_DEBUG_H__

#include <stdio.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#if defined(CONFIG_DEBUG_LEVEL)

#define debuglevel		CONFIG_DEBUG_LEVEL
enum {
	DBG_EMERG	=0,	/* system is unusable				*/
	DBG_ALERT	=1,	/* action must be taken immediately	*/
	DBG_CRIT	=2,	/* critical conditions				*/
	DBG_ERR		=3,	/* error conditions					*/
	DBG_WARNING	=4,	/* warning conditions				*/
	DBG_NOTICE	=5,	/* normal but significant condition	*/
	DBG_INFO	=6,	/* informational					*/
	DBG_DEBUG	=7,	/* debug-level messages				*/
	DBG_API		=8,	/* show api enter and exit message  */
};

#define api_debug(level, fmt, arg...)		\
			do {							\
				if (debuglevel >= level) {	\
					int __errno__ = errno;	\
					printf("%s[%s][%d]:"fmt,__FILE__,__FUNCTION__,__LINE__, ##arg); 	\
					fflush(stdout);			\
					errno = __errno__;		\
				} 							\
			} while (0)
			
#define api_enter(fmt, arg...)			\
		api_debug(DBG_API, "[+ %s +]"  fmt "\n", DebugModuleName, ##arg)
			
#define api_exit(fmt, arg...)			\
		api_debug(DBG_API, "[- %s -]"  fmt "\n", DebugModuleName, ##arg)


#else /* !defined(CONFIG_DEBUG_LEVEL) */

#define api_debug(level, format, arg...)	do { } while (0)
#define api_enter(fmt, arg...)			do { } while (0)
#define api_exit(fmt, arg...)			do { } while (0)

#endif


#if defined(CONFIG_DEBUG_LEVEL)
#define debuging(fmt, arg...)	\
		do { \
			int saveerr=errno; \
			struct timeval tv; \
			gettimeofday(&tv, NULL); \
			printf("[%ld.%03ld]: %s, %d, " fmt "\n", tv.tv_sec, tv.tv_usec/1000,__FUNCTION__, __LINE__,  ##arg); \
			fflush(stdout); \
			errno = saveerr; \
		} while (0)
#else /* !defined(CONFIG_DEBUG_LEVEL) */
	
#define debuging(fmt, arg...)	do { } while (0)
	
#endif



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __UMS_DEBUG_H__ */

