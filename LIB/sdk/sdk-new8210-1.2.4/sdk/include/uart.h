/******************************************************************************
 * uart.h
 * 
 * Copyright (c) 2010 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2010-07-05   1.0.0.0  Li.Hua       written
 ******************************************************************************/

#ifndef LINUX_UART_H
#define LINUX_UART_H

#include <termios.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

unsigned int tty_baud_to_value(speed_t speed);
speed_t tty_value_to_baud(unsigned int value);
int set_tty_speed(int fd, int speed);
int tty_property_config(int fd, int baudrate, int databits, int parity, int stopbits, int flow);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* end for LINUX_UART_H */

