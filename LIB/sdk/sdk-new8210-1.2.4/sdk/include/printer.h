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
 *  ��ӡ�������ṹ
 *  ro_xxxx : read only���ò���ֻ�ܶ��������޸�
 *  rw_xxxx : read write���ò����ɶ�д
 */
typedef struct printer_param {
	uint32_t		ro_width;			/* ��ӡ��� */
	uint32_t 		ro_bits_per_pixel;	/* guess what */
	uint32_t		rw_gray;			/* ��ӡ�Ҷ� */
	int32_t			rw_hi_temp;			/* ��ӡͷ�¶� */
	int32_t			rw_lo_temp;			/* ��������ӡ�¶� */
} printer_param_t;


typedef struct printer_param_ex{
	unsigned char ro_para_ver;      /* �����汾, ��ǰ 01  */
	unsigned char rw_dir_mode;      /* ����ģʽ, Ĭ��˫���ӡ */
	unsigned char rw_Line_space;    /* �м�϶, Ĭ��ֵ: 0  */
	unsigned char rw_left_adj;      /* �����, Ĭ��ֵ: 2  */
	unsigned char rw_right_adj;     /* �ҵ���, Ĭ��ֵ: 1  */
	unsigned char rw_print_pins;    /* ��ӡ����, Ĭ��ֵ: 9 */
	unsigned char FontFormat;       /* �����ʽ��0Ϊ������1Ϊѹ������8*8*/
	unsigned short rw_print_speed;    /* ��ӡ�ٶ�*/	
  unsigned short rw_left_adj_Us;      /* �����, Ĭ��ֵ: 500   30-550 */		
	unsigned short rw_right_adj_Us;      /* �ҵ���, Ĭ��ֵ:100   30-550*/	
	unsigned char reserve[9];
}printer_param_ex_t;

/* ��ʽ��ӡ���������� */
typedef struct st_printer_lifetime_data {
	int char_steps;
	int feed_steps;
	int pin_shots[9];
} printer_lifetime_data_t;


/*
 *  ��ӡ��״̬�����в�����ֻ�ܶ�ȡ
 */
/* ����λ */
#define PRINTER_STATUS_BUSY				0				/* �豸æ */
#define PRINTER_STATUS_HIGHT_TEMP		1				/* �����¶ȹ��� */
#define PRINTER_STATUS_PAPER_LACK		2				/* ����ȱֽ */
#define PRINTER_STATUS_NO_BATTERY		3				/* �޵�� */
#define PRINTER_STATUS_FEED				8				/* ������ֽ */
#define PRINTER_STATUS_PRINT			9				/* ����ӡ���� */
#define PRINTER_STATUS_FORCE_FEED		30				/* ǿ����ֽ */
#define PRINTER_STATUS_POWER_ON			31				/* ��Դ�� */

#define PRINTER_STATUS_ERROR_MASK       0xFE

typedef struct printer_status {
	unsigned long	status;
	int32_t			voltage;			/* ��ӡ��ѹ */
	int32_t			temperature;		/* ��ǰ��ӡͷ�¶� */
} printer_status_t;

/*
 * S : ��ʾͨ��ָ������
 * T : ��ʾֱ��ʹ�ò���ֵ֪ͨ
 * G : ��ʾ'��ȡ��Get��'��ͨ��ָ����Ӧ��
 * Q : ��ʾ'��ѯ��Query��',ͨ������ֵ��Ӧ��
 * X : ��ʾ'������eXchange��',ԭ�ӽ���G��S
 * H : ��ʾ'�л���Shift��',ԭ�ӽ���T��Q
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

