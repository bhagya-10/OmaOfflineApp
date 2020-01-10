/******************************************************************************
 * modem.h
 *
 * Copyright (c) 2009 by Li.Hua <lihua_338@163.com>.
 *
 * DESCRIPTION: - 用户层 modem 操作函数声明
 *
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2009-06-02   1.0.0.0  Li.Hua       written
 ******************************************************************************/

#ifndef __MODEM_USER_H__
#define __MODEM_USER_H__

#include <stdint.h>
#include "modem_iface.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int modem_power_control(int onoff);

int modem_open(const char *pathname, int oflags);
int modem_close(int fd);

ssize_t modem_write(int fd, const void *buf, size_t nbytes);
ssize_t modem_write_timeout(int fd, const void *buf, size_t nbytes, uint32_t timeoutms);

ssize_t modem_read(int fd, void *buf, size_t nbytes);
ssize_t modem_read_timeout(int fd, void *buf, size_t nbytes, uint32_t timeoutms);

int modem_wait_until_sent(int fd);

int modem_dialing(int fd, const char *telephone);
int modem_hangup(int fd);

int modem_get_status(int fd, int *status);
int modem_get_last_errno(int fd, int *err);

int modem_set_dial_parms(int fd, const ModemDialParms_t *dp);
int modem_get_dial_parms(int fd, ModemDialParms_t *dp);

int modem_get_sdlc_parms(int fd, ModemSdlcParms_t *sp);
int modem_set_sdlc_parms(int fd, const  ModemSdlcParms_t *sp);

int modem_enter_auto_answer(int fd);
int modem_exit_auto_answer(int fd);

int modem_check_extension(int fd);
int modem_exec_at_command(int fd, ModemAtCmd_t *at);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


