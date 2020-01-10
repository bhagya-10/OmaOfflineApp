/******************************************************************************
 * printer.h
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

#ifndef LINUX_PRINTER_H
#define LINUX_PRINTER_H

#include <stdint.h>
#include <linux/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 *  打印机参数结构
 *  ro_xxxx : read only，该参数只能读，不能修改
 *  rw_xxxx : read write，该参数可读写
 */
typedef struct printer_param {
	uint32_t		ro_width;			/* 打印宽度 */
	uint32_t 		ro_bits_per_pixel;	/* guess what */
	uint32_t		rw_gray;			/* 打印灰度 */
	int32_t			rw_hi_temp;			/* 打印头温度 */
	int32_t			rw_lo_temp;			/* 最大允许打印温度 */
} printer_param_t;


typedef struct printer_param_ex{
	unsigned char ro_para_ver;      /* 参数版本, 当前 01  */
	unsigned char rw_dir_mode;      /* 方向模式, 默认双向打印 */
	unsigned char rw_Line_space;    /* 行间隙, 默认值: 0  */
	unsigned char rw_left_adj;      /* 左调整, 默认值: 2  */
	unsigned char rw_right_adj;     /* 右调整, 默认值: 1  */
	unsigned char rw_print_pins;    /* 打印针数, 默认值: 9 */
	unsigned char FontFormat;       /* 字体格式，0为正常，1为压缩字体8*8*/
	unsigned short rw_print_speed;    /* 打印速度*/	
  unsigned short rw_left_adj_Us;      /* 左调整, 默认值: 500   30-550 */		
	unsigned short rw_right_adj_Us;      /* 右调整, 默认值:100   30-550*/	
	unsigned char reserve[9];
}printer_param_ex_t;

/* 针式打印机寿命数据 */
typedef struct st_printer_lifetime_data {
	int char_steps;
	int feed_steps;
	int pin_shots[9];
} printer_lifetime_data_t;


/*
 *  打印机状态，所有参数都只能读取
 */
/* 数据位 */
#define PRINTER_STATUS_BUSY				0				/* 设备忙 */
#define PRINTER_STATUS_HIGHT_TEMP		1				/* 错误，温度过高 */
#define PRINTER_STATUS_PAPER_LACK		2				/* 错误，缺纸 */
#define PRINTER_STATUS_NO_BATTERY		3				/* 无电池 */
#define PRINTER_STATUS_FEED				8				/* 正在走纸 */
#define PRINTER_STATUS_PRINT			9				/* 正在印数据 */
#define PRINTER_STATUS_FORCE_FEED		30				/* 强制走纸 */
#define PRINTER_STATUS_POWER_ON			31				/* 电源打开 */

#define PRINTER_STATUS_ERROR_MASK       0xFE

typedef struct printer_status {
	unsigned long	status;
	int32_t			voltage;			/* 打印电压 */
	int32_t			temperature;		/* 当前打印头温度 */
} printer_status_t;

/*
 * S : 表示通过指针设置
 * T : 表示直接使用参数值通知
 * G : 表示'获取（Get）'，通过指针来应答
 * Q : 表示'查询（Query）',通过返回值来应答
 * X : 表示'交换（eXchange）',原子交换G和S
 * H : 表示'切换（Shift）',原子交换T和Q
 */
#define PRINTER_IOC_MAGIC		'P'
#define PRINTER_IOC_MAX			10

#define PRINTER_IOCGSTATUS		_IOR(PRINTER_IOC_MAGIC,	1,	printer_status_t *)
#define PRINTER_IOCTFEEDSTART	_IOW(PRINTER_IOC_MAGIC,	2,	uint32_t)
#define PRINTER_IOCTFEEDSTOP	_IO (PRINTER_IOC_MAGIC,	3)
#define PRINTER_IOCTRESET		_IO (PRINTER_IOC_MAGIC,	4)
#define PRINTER_IOCGPARAM		_IOR(PRINTER_IOC_MAGIC,	5,	printer_param_t *)
#define PRINTER_IOCSPARAM		_IOW(PRINTER_IOC_MAGIC,	6,	printer_param_t *)
#define PRINTER_IOCGPARAM_EX	_IOR(PRINTER_IOC_MAGIC,	7,	printer_param_ex_t *)    
#define PRINTER_IOCSPARAM_EX	_IOW(PRINTER_IOC_MAGIC,	8,	printer_param_ex_t *)    
#define PRINTER_IOCGOPER_DATA	_IOR(PRINTER_IOC_MAGIC,	9,	printer_lifetime_data_t *)


int printer_open(const char *filename, int oflags);

int printer_close(int fd);

int printer_read(int fd, void *buf, size_t nbytes);

int printer_write(int fd, const void *buf, size_t nbytes);

int printer_get_status(int fd, printer_status_t *status);

int printer_feed_start(int fd, uint32_t step);

int printer_feed_stop(int fd);

int printer_reset(int fd);

int printer_get_param(int fd, printer_param_t *param);

int printer_set_param(int fd, const printer_param_t *param);

int printer_get_param_ex(int fd, printer_param_ex_t *param);

int printer_set_param_ex(int fd, const printer_param_ex_t *param);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* end for LINUX_PRINTER_H */

