/******************************************************************************
 * magstripe.h
 * 
 * Copyright (c) 2010 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2010-07-14   1.0.0.0  Li.Hua       written
 ******************************************************************************/

#ifndef LINUX_MAGSTRIPE_H
#define LINUX_MAGSTRIPE_H

#include <stdint.h>
#include <linux/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define MAGCARD_TRACK_NUM					3

#define MAGCARD_DECODE_OK					0
#define MAGCARD_DECODE_ERR_RAW				1	/* ԭʼ���ݴ��� */
#define MAGCARD_DECODE_ERR_LRC				2	/* LRCУ����� */
#define MAGCARD_DECODE_ERR_ODD				4	/* ��żУ����� */

/*
 * S : ��ʾͨ��ָ������
 * T : ��ʾֱ��ʹ�ò���ֵ֪ͨ
 * G : ��ʾ'��ȡ��Get��'��ͨ��ָ����Ӧ��
 * Q : ��ʾ'��ѯ��Query��',ͨ������ֵ��Ӧ��
 * X : ��ʾ'������eXchange��',ԭ�ӽ���G��S
 * H : ��ʾ'�л���Shift��',ԭ�ӽ���T��Q
 */
#define MAGCARD_IOC_MAGIC		'M'
#define MAGCARD_IOC_MAX			10

#define MAGCARD_IOCGTRACKSET	_IOR (MAGCARD_IOC_MAGIC, 1,	user_track_info_t *)
#define MAGCARD_IOCSTRACKSET	_IOW (MAGCARD_IOC_MAGIC, 2,	user_track_info_t *)
#define MAGCARD_IOC_IOCTL_CMD   _IOWR(MAGCARD_IOC_MAGIC, 3, void *)


typedef struct track_info {
	int					bit_pre_code;						/* ����λ���� */
	uint8_t				start_flag;							/* ��ʼ��־ */
	uint8_t				end_flag;							/* ������־ */
	uint8_t				base;								/* ��ֵ */
} track_info_t;

typedef struct magcard_info {
	int					trackno;							/* �ŵ��� */
	int					status;								/* �ŵ�״̬�������� */
	int					size;								/* ���ݴ�С */
	uint8_t				data[244];							/* �ŵ����� */
} magcard_info_t;

typedef struct user_track_info {
	track_info_t		trackinfo[MAGCARD_TRACK_NUM];
} user_track_info_t;

typedef struct user_magcard_info {
	magcard_info_t		magcardtrack[MAGCARD_TRACK_NUM];
} user_magcard_info_t ;


int magstripe_open(const char *filename, int oflags);

int magstripe_close(int fd);

int magstripe_read(int fd, user_magcard_info_t *cardinfo);

int magstripe_read_timeout(int fd, user_magcard_info_t * cardinfo, unsigned long ms);

int magstripe_write(int fd, const user_magcard_info_t *cardinfo);

int magstripe_write_timeout(int fd, const user_magcard_info_t * cardinfo, unsigned long ms);

int magstripe_get_trackset(int fd, user_track_info_t *trackinfo);

int magstripe_set_trackset(int fd, const user_track_info_t *trackinfo);


int magstripe_read_serialnumber(int fd, unsigned char bSerialNumber[10]);
int magstripe_reset(int fd);
int magstripe_setkey(int fd);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* end for LINUX_MAGSTRIPE_H */

