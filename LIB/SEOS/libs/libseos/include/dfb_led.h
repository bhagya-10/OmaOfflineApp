/*
 * dfb_led.h
 *
 *  Created on: 2013-9-5
 *      Author: justin
 */

#ifndef DFB_LED_H_
#define DFB_LED_H_


#define LED_BLUE	  0
#define LED_YELLOW  1
#define LED_GREEN	  2
#define LED_RED		  3

#define LED_OFF 	  0
#define LED_ON		  1
#define LED_BLINK 	2

int led_init(void);
int led_exit(void);
int led_set_mode(int led_no, int status);
int led_get_mode(int led_no);
int led_set_nfc_fd(int fd);

#endif /* DFB_LED_H_ */
