/******************************************************************************
 * led.h
 * 
 * Copyright (c) 2010 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2010-07-14   1.0.0    Li.Hua       written
 ******************************************************************************/

#ifndef LINUX_LED_H
#define LINUX_LED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define LED_ARROW_DOWN			"arrow_down"
#define LED_ARROW_UP			"arrow_up"

#define LED_BATTERY_SHELL		"battery_shell"
#define LED_BATTERY_1			"battery_1"
#define LED_BATTERY_2			"battery_2"
#define LED_BATTERY_3			"battery_3"
#define LED_BATTERY_4			"battery_4"

#define LED_BACKLIGHT			"backlight"
#define LED_CARD				"card"
#define LED_LCD_CONTRAST		"contrast"
#define LED_LOCK				"lock"
#define LED_POWER_SUPPLY		"power_supply"

#define LED_MODEM_OFFHOOK		"modem_offhook"
#define LED_MODEM_ONHOOK		"modem_onhook"
#define LED_MODEM_BASE			"modem_base"
#define LED_MODEM_POWER			"modem_power"
#define LED_MODEM_RESET			"modem_reset"


#define LED_SIGNAL_STATION		"signal_station"
#define LED_SIGNAL_1			"signal_1"
#define LED_SIGNAL_2			"signal_2"
#define LED_SIGNAL_3			"signal_3"
#define LED_SIGNAL_4			"signal_4"

#define LED_ONLINE				"online"
#define LED_TX					"tx"
#define LED_RX					"rx"

#define LED_WIRELESS_RESET		"wirelessreset"
#define LED_WIRELESS_UPDATE		"wirelessupdate"
#define LED_WIRELESS_POWER		"wirelesspower"
#define LED_WIRELESS_UART_EN	"uart1en"
#define LED_WIRELESS_UART_CS	"uart1cs"
#define LED_PINPAD_UART_CS		"uart2cs"
#define LED_WIRELESS_SOFT_RESET	"wirelesssoftreset"



#define LED_BRIGHTNESS_MIN		0
#define LED_BRIGHTNESS_MAX		255



int led_set_brightness(const char *led_name, uint32_t brightness);

int led_get_brightness(const char *led_name, uint32_t *brightness);


#define MIN_LED_CONTRAST_BRIGHTNESS         10


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* end for LINUX_LED_H */

