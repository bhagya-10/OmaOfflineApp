/******************************************************************************
 * modem_iface.h
 *
 * Copyright (c) 2008 by Li.Hua <lihua_338@163.com>.
 *
 * DESCRIPTION: - modem用户层接口文件
 *
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2009-09-08   1.0.0.0  Li.Hua       written
 ******************************************************************************/

#ifndef MODEM_INTERFACE_H
#define MODEM_INTERFACE_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
enum {
	MODEM_DAIL_DTMF				= 0,
	MODEM_DAIL_PULSE,
};

enum
{
	MODEM_ERRNO_OK				= 0,
	MODEM_ERRNO_CONNECT			= 0,
	MODEM_ERRNO_ERROR			= -3072,
	MODEM_ERRNO_BUSY			= -3073,
	MODEM_ERRNO_NO_DIALTONE		= -3074,
	MODEM_ERRNO_NO_CARRIER		= -3075,
	MODEM_ERRNO_NO_LINE			= -3076,
	MODEM_ERRNO_NO_ANSWER		= -3077,
	MODEM_ERRNO_OFF_HOOK		= -3078,
	MODEM_ERRNO_LINE_IN_USE		= -3079,
	MODEM_ERRNO_UN_OBTAINABLE	= -3080,
	MODEM_ERRNO_LINE_REVERSAL	= -3081,
	MODEM_ERRNO_EXTENSION_IN_USE= -3082,
};


enum {
	MODEM_OPERATE_PROCESS		= 0,	/* 操作正在处理 */
	MODEM_OPERATE_DONE,					/* 操作已经完成 */
};

enum {
	MODEM_COMM_ASYNC			= 0,	/* 异步连接 */
	MODEM_COMM_SYNC,					/* 同步连接 */
};

enum {
	MODEM_CONNECT_1200BPS		= 1200,
	MODEM_CONNECT_2400BPS		= 2400,
	MODEM_CONNECT_4800BPS		= 4800,
	MODEM_CONNECT_7200BPS		= 7200,
	MODEM_CONNECT_9600BPS		= 9600,
	MODEM_CONNECT_12000BPS		= 12000,
	MODEM_CONNECT_14400BPS		= 14400,
	MODEM_CONNECT_16800BPS		= 16800,
	MODEM_CONNECT_19200BPS		= 19200,
	MODEM_CONNECT_21600BPS		= 21600,
	MODEM_CONNECT_24000BPS		= 24000,
	MODEM_CONNECT_26400BPS		= 26400,
	MODEM_CONNECT_28800BPS		= 28800,
	MODEM_CONNECT_31200BPS		= 31200,
	MODEM_CONNECT_33600BPS		= 33600,
	MODEM_CONNECT_48000BPS		= 48000,
	MODEM_CONNECT_56000BPS		= 56000,
};

typedef enum
{
	MODEM_STATE_NOT_INIT			=0,	/* 未初始化 */
	MODEM_STATE_NO_SET_MODE,			/* 未设置模式，需要继续设置 */
	MODEM_STATE_SYNC_MODE, 				/* 初始化为同步模式 */
	MODEM_STATE_ASYN_MODE, 				/* 初始化为异步模式 */
	MODEM_STATE_DISCONNECT, 			/* 断开连接 */
	MODEM_STATE_WAITTING,				/* 被拨状态，等待连接 */
	MODEM_STATE_DAILING,				/* 正在拨号 */
	MODEM_STATE_CONNECT_SDLC,			/* 等待SDLC连接 */
	MODEM_STATE_CONNECT,				/* 连接成功 */
}MODEM_STATUS;


typedef struct  {
    unsigned int t1;
    unsigned int t2;
    unsigned int n2;
    unsigned int n2count;
    unsigned int window;
    unsigned int state;
    unsigned int mode;
} ModemSdlcParms_t;

#define ModemRegion(Region)	Modem##Region
typedef enum {
	ModemRegion(Algeria)		=1,
	ModemRegion(Argentina),
	ModemRegion(Armenia),
	ModemRegion(Australia),
	ModemRegion(Austria),
	ModemRegion(Bahamas),
	ModemRegion(Bahrain),
	ModemRegion(Belarus),
	ModemRegion(Belgium),
	ModemRegion(Bermuda),
	ModemRegion(Brazil),
	ModemRegion(Brunei),
	ModemRegion(Bulgaria),
	ModemRegion(Canada),
	ModemRegion(Caribbean),
	ModemRegion(Chile),
	ModemRegion(China),
	ModemRegion(Colombia),
	ModemRegion(CostaRica),
	ModemRegion(Croatia),
	ModemRegion(Cyprus),
	ModemRegion(Czech),
	ModemRegion(Denmark),
	ModemRegion(Dominican),
	ModemRegion(Dubai),
	ModemRegion(Egypt),
	ModemRegion(ElSalvador),
	ModemRegion(Ecuador),
	ModemRegion(Estonia),
	ModemRegion(Finland),
	ModemRegion(France),
	ModemRegion(Georgia),
	ModemRegion(Germany),
	ModemRegion(Ghana),
	ModemRegion(Greece),
	ModemRegion(Guadeloupe),
	ModemRegion(Guam),
	ModemRegion(HongKong),
	ModemRegion(Hungary),
	ModemRegion(Iceland),
	ModemRegion(India),
	ModemRegion(Indonesia),
	ModemRegion(Ireland),
	ModemRegion(Israel),
	ModemRegion(Italy),
	ModemRegion(Japan),
	ModemRegion(Jordan),
	ModemRegion(Kazakhstan),
	ModemRegion(Korea),
	ModemRegion(Kuwait),
	ModemRegion(Kyrgyzstan),
	ModemRegion(Latvia),
	ModemRegion(Lebanon),
	ModemRegion(Lesotho),
	ModemRegion(Liechtenstein),
	ModemRegion(Lithuania),
	ModemRegion(Luxembourg),
	ModemRegion(Macao),
	ModemRegion(Malaysia),
	ModemRegion(Malta),
	ModemRegion(Martinique),
	ModemRegion(Mexico),
	ModemRegion(Moldova),
	ModemRegion(Morocco),
	ModemRegion(Netherlands),
	ModemRegion(NewZealand),
	ModemRegion(Nigeria),
	ModemRegion(Norway),
	ModemRegion(Oman),
	ModemRegion(Pakistan),
	ModemRegion(Paraguay),
	ModemRegion(Peru),
	ModemRegion(Philippines),
	ModemRegion(Poland),
	ModemRegion(Polynesia),
	ModemRegion(Portugal),
	ModemRegion(PuertoRico),
	ModemRegion(Qatar),
	ModemRegion(Reunion),
	ModemRegion(Romania),
	ModemRegion(Russia),
	ModemRegion(SaudiArabia),
	ModemRegion(Singapore),
	ModemRegion(Slovakia),
	ModemRegion(Slovenia),
	ModemRegion(SouthAfrica),
	ModemRegion(Spain),
	ModemRegion(SriLanka),
	ModemRegion(Sweden),
	ModemRegion(Switzerland),
	ModemRegion(Syria),
	ModemRegion(Taiwan),
	ModemRegion(Thailand),
	ModemRegion(Tunisia),
	ModemRegion(Turkey),
	ModemRegion(UAE),
	ModemRegion(USA),
	ModemRegion(Ukraine),
	ModemRegion(UnitedKingdom),
	ModemRegion(Uruguay),
	ModemRegion(Uzbekistan),
	ModemRegion(Venezuela),
	ModemRegion(Yemen),
	ModemRegion(Zambia),

	ModemRegion(Senegal),
}RegionCode_t;

/******************************************************************************/
typedef struct {
	int				connect_mode;	/* 连接模式: 同步、异步 */
	int				connect_speed;	/* 连接速率 */
	int				dial_mode;		/* 拨号模式: DTMF Pluse (秒) */
	int				dial_pause;		/* ','暂停时间 (秒) */
	int				dial_timeo;		/* 拨号等待的超时时间 (秒) */
	int				idle_timeo;		/* 无数据通信的超时时间,0为永远不超时 */
	int				extension_chk;	/* 是否检查并接电话 */
	RegionCode_t	region;			/* 国家区域码 */
	uint8_t			bell_ccitt;		/* 贝尔拨号，CCITT拨号 */
	uint8_t			without_fc;		/* 是否使不用快速链接 */
	uint8_t			t_answertone;	/* 应答音稳定时间 */
	uint8_t			t_dialtone;		/* 拨号音检查时间 */
	uint8_t			dtmf_level;     /* DTMF level(6-14),默认为10*/ 
	uint8_t			noblinddial;	/* 是否不支持盲拨，0-支持，1-不支持 */
	uint8_t			reserved[10];	/* 保留扩展 */
} ModemDialParms_t;

typedef struct {
	int16_t	timeo;			/* 命令响应超时时间(秒)，0表示不超时, < 0表示不等待应答 */
	uint8_t	reserved[6];
	char	cmd[256];		/* at命令 */
	char	maybe[760];		/* 可能响应关键字列表，如果遇到连续2个0x00则结束 */
	char	rsp[1024];		/* 实际应答 */
} ModemAtCmd_t;

/*
 * S : 表示通过指针设置
 * T : 表示直接使用参数值通知
 * G : 表示'获取（Get）'，通过指针来应答
 * Q : 表示'查询（Query）',通过返回值来应答
 * X : 表示'交换（eXchange）',原子交换G和S
 * H : 表示'切换（Shift）',原子交换T和Q
 */
#define MODEM_IOC_MAGIC		'M'

#define MODEM_IOTRESET		_IO (MODEM_IOC_MAGIC, 0)		/* reset the modem */
#define MODEM_IOTQSYNCMODE	_IOW(MODEM_IOC_MAGIC, 1, int)	/* set synchronization communication mode */
#define MODEM_IOTQANSYMODE	_IOW(MODEM_IOC_MAGIC, 2, int)	/* set asynchronism communication mode */
#define MODEM_IOSQDIAL		_IOW(MODEM_IOC_MAGIC, 3, const char*) /* dailing number */
#define MODEM_IOTQHANGUP	_IO (MODEM_IOC_MAGIC, 4)		/* huangup */
#define MODEM_IOGQSTATE		_IOR(MODEM_IOC_MAGIC, 5, int)	/* get modem current status */
#define MODEM_IOGQCOMMODE	_IOR(MODEM_IOC_MAGIC, 6, int)	/* get modem communication mode */
#define MODEM_IOGQOPDONE	_IOR(MODEM_IOC_MAGIC, 7, int)	/* get modem operate done */
#define MODEM_IOGQLASTERROR	_IOR(MODEM_IOC_MAGIC, 8, int)	/* get modem last error */
#define MODEM_IOGQSDLCPARMS	_IOR(MODEM_IOC_MAGIC, 9, ModemSdlcParms_t*) /* get sdlc parms */
#define MODEM_IOSQSDLCPARMS	_IOW(MODEM_IOC_MAGIC, 10, const ModemSdlcParms_t*) /* set sdlc parms */
#define MODEM_IOTAUTOANSWER _IO (MODEM_IOC_MAGIC, 11) 		/* enter auto answer mode */
#define MODEM_IOTEXITANSWER	_IO (MODEM_IOC_MAGIC, 12)		/* exit auto answer mode */
#define MODEM_IOTDIALMODE	_IOW(MODEM_IOC_MAGIC, 13, int)	/* dial mode: DTMF PLUSE */
#define MODEM_IOTDIALTIMEO	_IOW(MODEM_IOC_MAGIC, 14, int)	/* wait time for carrier silence or dial tone */
#define MODEM_IOTDIALPAUSE	_IOW(MODEM_IOC_MAGIC, 15, int)	/* pause time for dial delay */
#define MODEM_IOTIDLETIMEO	_IOW(MODEM_IOC_MAGIC, 16, int)	/* set idle timeout for no data exchange */
#define MODEM_IOTEXTENCHK	_IOW(MODEM_IOC_MAGIC, 17, int)	/* whether check extension */
#define MODEM_IOGEXTENSION	_IOR(MODEM_IOC_MAGIC, 18, int *)	/* get extension status */
#define MODEM_IOHEXECATCMD	_IOWR(MODEM_IOC_MAGIC, 19, ModemAtCmd_t*)	/* Execute AT commandd */
#define MODEM_IOSDIALPARMS	_IOW(MODEM_IOC_MAGIC, 20, const ModemDialParms_t*)	/* set parameter of dial */
#define MODEM_IOGDIALPARMS	_IOR(MODEM_IOC_MAGIC, 21, ModemDialParms_t*)		/* get parameter of dial */
#define MODEM_IOGCHIPINFO	_IOR(MODEM_IOC_MAGIC, 22, char*)	/* get modem chip info name*/


#define MODEM_IOC_MAX		32

/******************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


