/******************************************************************************
 * posapi.h
 * 
 * Copyright (c) 2010 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2010-07-15   1.0.0    Li.Hua       written
 ******************************************************************************/

#ifndef POS_API_H
#define POS_API_H
 
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

#include <directfb.h>

#include "saferw.h"
 
#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */
  

#ifndef     min
#define min(x, y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x < _y ? _x : _y; })

#endif

#ifndef     max
#define max(x, y) ({ \
	typeof(x) _x = (x);	\
	typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x > _y ? _x : _y; })

#endif


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef ALIGN
#define ALIGN(x,a) ({ \
	typeof(x) _x = (x);	\
	typeof(a) _a = (a);	\
	((_x+_a-1)&~(_a-1)); })

#endif


struct kbdset {
	int 		beep_enable;
	uint16_t	beep_freq;
	uint16_t	beep_time;
	int			backlight;
	int			feed_enable;
};

typedef struct {
	char	module_name[32];
	int		module_exist;

	union {
		struct MachineName {
			char	name[32];
		} terminal;
		
		struct PrinterConfig {
			char	type[32];
		} printer;

		struct ModemConfig {
			uint32_t	max_speed_asyn;
			uint32_t	max_speed_sync;
		} modem;

		struct UsbHostConfig {
			char	version[32];
		} usbhost;
	} module_info;
} MachineConfig_t;

/* LCD 类型定义 */
enum{
	LCD_TYPE_RESERVE_0 =0, 		/*保留*/
	LCD_TYPE_RESERVE_1, 		/*保留*/	
	LCD_TYPE_RESERVE_2, 		/*保留*/
	LCD_TYPE_320_480_C_V,		/*320*480竖屏*/
	LCD_TYPE_256_150_M,			/*256*150黑白屏*/
	LCD_TYPE_128_64_M,			/*128*64黑白屏*/
	LCD_TYPE_240_320_C_V,		/*240*320 TFT竖屏*/
	LCD_TYPE_320_240_C_H,		/*320*240 TFT横屏*/	
};

#define HARDWARE_MACHINE			"machine"			// Name of Machine
#define HARDWARE_BATTERY			"battery"			// Battery
#define HARDWARE_LCD				"lcd"				// LCD
#define HARDWARE_TOUCHSCREEN		"touchscreen"		// Touch Screen
#define HARDWARE_KEYBOARD			"keyboard"			// Keyboard
#define HARDWARE_MSR				"msr"				// Magnetic Stripe Reader
#define HARDWARE_PRINTER			"printer"			// printer
#define HARDWARE_NFC				"nfc"				// contactless
#define HARDWARE_TF					"tf"				// TransFlash
#define HARDWARE_IC					"ic"				// IC
#define HARDWARE_SAM				"sam"				// SAM
#define HARDWARE_2SIM				"2sim"				// Dual SIM
#define HARDWARE_UART				"uart"				// UART
#define HARDWARE_BARSCANNER			"barscanner"		// Bar Scanner
#define HARDWARE_WIRELESS			"wireless"			// GPRS or CDMA
#define HARDWARE_MODEM				"modem"				// Modem
#define HARDWARE_LAN				"lan"				// LAN
#define HARDWARE_WIFI				"wifi"				// WIFI
#define HARDWARE_BLUETOOTH			"bluetooth"			// Bluetooth
#define HARDWARE_USBH				"usbh"				// USB Host
#define HARDWARE_USBD				"usbd"				// USB Device
#define HARDWARE_USBO				"usbo"				// USB OTG
#define HARDWARE_GPS				"gps"				// GPS
#define HARDWARE_FINGERPRINT		"fingerprint"		// Fingerpinter
#define HARDWARE_NFC_LED			"nfc_led"			// contactless led

int sys_get_hardware_config(const char *module, char type[128], int *numbers);

enum {
	VERSION_OF_MAINBOARD = 0,
	VERSION_OF_PORTBOARD,
	VERSION_OF_FLAGOTP,
};
int sys_get_hardware_version(int type, char version[], int maxsize);

int sys_is_matchine_otp(void);
int sys_reboot(void);

int sys_poweroff(void);

int sys_kbd_set(const struct kbdset *pset);

int sys_kbd_get(struct kbdset *pset);

int sys_set_time(const struct tm *tm);

int sys_get_status_bar_height(void);

int sys_delay_to_suspend(void);

int sys_suspend_now(void);

int sys_sober_delay_suspend(int timeout);

int sys_pm_enable_keypower(int enable);

int sys_pm_enable_auto_suspend(int enable);

int sys_pm_check(void);

int sys_pm_suspend_allow_auto_resume(void);

int sys_pm_suspend_block(void);

char *sys_get_sn(char *sn, int nbytes);

void beep(unsigned int frequency, unsigned int keepms);

int sys_backlight_set_time(int keep);
int sys_backlight_get_time(int *keep);

int sys_backlight_set_brightness(uint8_t brightness);
int sys_backlight_get_brightness(uint8_t *brightness);

int print_surface(int print_fd, IDirectFBSurface *fb_surface, uint32_t print_height);

int sys_get_machine_config(MachineConfig_t config[], uint32_t nr);

int sys_software_update(const char *filename);

int getruntime(struct timeval *__tv, __timezone_ptr_t __tz);

int open_vpdn_manage(void);

int bluez_setup(void);
int bluez_power_down(void);

int wifi_power_control(int is_open);
int wifi_power_control_force(int is_open);
int wifi_set_high_speed(int is_high_speed);
int ext_nfc_driver_load(void);
int ext_nfc_driver_unload(void);

int sound_play(int fd, const char *filename);
int sound_set_volume(int fd, uint32_t volume);
int sound_set_channel(int fd, uint32_t channel);
int sound_open(const char *filepath, int oflags);
int sound_close(int fd);


typedef struct { 
	char  iface[16];	/* 网络接口名 */
	int   dhcp;			/* 如果启用dhcp则下面参数无效 */
	char  IP[16];		/*本地IP地址*/ 
	char  GW[16];		/*本地网关*/ 
	char  Mask[16];		/*本地子网掩码*/ 
	char  Dns1[16];		/*DNS*/ 
	char  Dns2[16];		/*DNS*/ 
} IPCONFIG_t;
int network_setting(const IPCONFIG_t *ipconfig);


typedef struct 
{
	uint32_t status;//0-仅电池供电;1-正充电;2-充满电;3-仅外电
	uint32_t cur_values;//当前电压值
	uint32_t max_values;//最高电压值
	uint32_t min_values;//最低电压值
	uint32_t percent;//当前电量百分比
}battery_info_t;

//return : 0 - successful , other - failed
int sys_battery_info(battery_info_t *battery_info);


#ifdef __cplusplus
  }
#endif /* __cplusplus */
 
#endif /* end for POS_API_H */
 

