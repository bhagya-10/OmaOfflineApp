/******************************************************************************
 * bar_scanner.h
 *
 *
 * DESCRIPTION: - 用户层 扫描头 操作函数声明
 *
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * alex.chen	2013-05-29 ,  Written
 ******************************************************************************/

#ifndef __SCANNER_USER_H__
#define __SCANNER_USER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


int bar_open(const char *pathname, int oflags);
int bar_close(int fd);
ssize_t bar_scan(int fd, uint32_t onoff);
ssize_t bar_read(int fd, void *buf, size_t nbytes);
ssize_t bar_read_timeout(int fd, void *buf, size_t nbytes, uint32_t timeoutms);
ssize_t bar_get_ver(int fd, int8_t type[32], int8_t ver[128]);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


