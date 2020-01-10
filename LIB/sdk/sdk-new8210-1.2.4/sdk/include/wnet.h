#ifndef __WLAN_GPRS_H__
#define __WLAN_GPRS_H__

#include <stdint.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define WNET_OK						(0)

#define WNET_DLOPEN_ERR				(-4000)	// open libxxx.so err
#define WNET_DLSYM_ERR				(-4001)	// get function point from libxxx.so err
#define WNET_INIT_ERR				(-4002)	// wnet init err
#define WNET_NOINIT_ERR				(-4003)	// wnet do not init
#define WNET_PARAM_ERR				(-4004)	// param err
#define WNET_GET_OP_ERR				(-4005)	// get_operation struct err
#define WNET_NO_FUN_ERR				(-4006)	// not the function which beed call 
#define WNET_POWER_DOWN_ERR			(-4007)	// went power down err
#define WNET_BEEN_USE_ERR			(-4008)	// wnet have been userd now
#define WNET_SUSPEND_ERR			(-4009)	// wnet have been suspend now

//for sim card
#define WNET_SIM_NOINSERT_ERR		(-4010)	// sim card had not insert
#define WNET_SIM_PIN_ERR			(-4011)	// sim card pin err
#define WNET_SIM_PUK_ERR			(-4012)	// sim card puk err
#define WNET_SIM_BUSY				(-4013)	// sim card busy

#define	WNET_NO_MODULE_ERR			(-5000)	// not wnet module
#define	WNET_RECV_TIMEOUT_ERR		(-5001)	// receive from wnet timeout
#define	WNET_USER_CANCEL_ERR		(-5002)	// user cancel
#define	WNET_RECV_DATA_ERR			(-5003)	// not used
#define	WNET_MOD_RESP_ERR			(-5004)	// receive data from wnet err
#define	WNET_SEND_DATA_ERR			(-5005)	// send data to wnet err
#define	WNET_COMM_OPEN_ERR			(-5006)	// series port open err
#define	WNET_SMS_DIV_ERR			(-5007)	// not used

//for gprs network
#define WET_DETTACHED_ERR			(-5008)	// Dettached
#define WNET_ATTACHING_ERR			(-5009)	// the module is looking for the base station.
#define WNET_ABNORMAL_ERR			(-5010)	// abnormal error
#define WNET_CSQWEAK_ERR			(-5011)	// the signal is too weak, please check the antenna

//for phone
#define WET_EMERGENCY_ERR			(-5012)	// SIM/UIM is in emergency status
#define WET_RING_ERR				(-5013)	// detect ringing
#define WET_BUSY_ERR				(-5014)	// detect call in progress
#define WET_DIALING_ERR 			(-5015)	// dialing
#define WET_PPP_INVALID_ERR 		(-5016)	// ppp connect disable

//for async function invoke
#define WNET_API_RUNNING			(-6001)	// have a api running ,do not exec complete
#define WNET_API_GETTING_DATA		(-6002)	// last api do not get the retval and output data , so can not exec cur api(use for request api function)
#define WNET_API_INDEX_ERR			(-6003)	// the api index is not cur api index or pid err(use for check	api function)
#define WNET_API_MEMORY_ERR			(-6004)	// memcory err (eg :malloc failed)


//define other
#define	GPRS_MODULE_TYPE			(0x0a)
#define	CDMA_MODULE_TYPE			(0x0b)
#define	WIFI_MODULE_TYPE			(0x0c)
#define	WCDMA_MODULE_TYPE			(0x0d)
#define	CDMA2000_MODULE_TYPE		(0x0e)
#define	TDSCDMA_MODULE_TYPE			(0x0f)

#define WNET_UPDATE_FW_ON			(0x01)
#define WNET_UPDATE_FW_OFF			(0x00)

// for SMS
enum {
	WNET_SMS_UNREAD	= 0,
	WNET_SMS_READ	= 1,
	WNET_SMS_UNSENT	= 2,
	WNET_SMS_SENT	= 3,
	WNET_SMS_ALL	= 4,
};

//from 8110
#define MAX_SMS_NUMBER				20
#define PDU_SMS_CONTENT_LENGTH		300

#define PDU_TYPE_TP_UDHI			0x40	// user data header indicate
#define PDU_TYPE_TP_SRI				0x20	// send status report
#define PDU_TYPE_TP_MMS				0x04	// more message send
#define PDU_TYPE_TP_MTI				0x03

#define PDU_TYPE_TP_SEND			0x01
#define PDU_TYPE_TP_RECV			0x00


#define	SEND_ONLY					(0x01)
#define	RECV_ONLY					(0x02)
#define	SEND_RECV					(0x03)
#define	RECV_BUFF_SIZE				(1024*2)

typedef struct {
	char		Type;				// 模块类型
	char		Name[64];			// 模块名称
	char		Manufacturer[64];	// 模块厂商
	char		Ver[64];			// 模块版本信息
	char		Pppdev[64];			// ppp设备名称
	char		Atdev[64];			// AT指令设备名称
	char		Reserv[256-64-64];	// 保留字段，扩展用
} wnet_moduleinfo_t;


typedef struct {
	int			arfcn;				// 小区频点号
	int			rxlev;				// 小区接收功率，经常变化
	int			bsic;				// 小区基站号
	int			cell_id;			// 小区号
	int			lac;				// 小区位置区号
	int			mnc;
	int			mcc;
	int			aiRev[5];
} tBaseStationGPRS;


typedef struct {
	int			BandClass;
	int			Channel;			// the channel number of the ME's current primary cell
	int			SID;				// the system ID. 
	int			NID;				// the network ID.
	int			BaseStationPRev;
	int			PilotPNoffset;		// the pilot offset of the ME's current primary cell.
	int			BaseStationID;		// the base station ID.
	int			SlotCycleIndex;		// the slot cycle index
	int			RawEcIo;			// the ME's pilot strength in dBm
	int			RxPower;			// the strength (dBm) of the signals received by the ME
	int			TxPower;			// the strength (dBm) of the signals transmitted by the ME.   
	int			TxAdj;				// the ME's transmitted power adjustment.
} tBaseStationCDMA;

typedef struct {
	int			cell_id;
	int			lac;	
	int			mnc;
	int			mcc;
	int			uarfcn;	
	int			psc;	
	int			rscp;	
	int			ecno;	
} tBaseStationWCDMA;

typedef struct {
	int			iFlag;	// 0:NONE, 1:GPRS, 2:CDMA, 3:WCDMA
	union {
		tBaseStationGPRS	oGprs;
		tBaseStationCDMA	oCdma;
        tBaseStationWCDMA   oWcdma;
	} Wnet;
} tBaseStation;


typedef struct {
	uint8_t		byLength;	   // number length
	uint8_t		byType; 	   // type,0x91:international, 0x81:Inland
	uint8_t		abyNumber[30]; // number content
} tTelInfo;


typedef struct {
	char		strSca[64];		// server central tel number
	char		strDstAddr[64];	// Destination address
	uint8_t		byPid;			// Protocol ID, the default value is 0x00
	uint8_t		byDcs;			// Decord Standard
	uint8_t		byUniqueID;		// Decord Standard
	uint8_t		byTpVp;			// SMS Validity Period
	uint8_t		abyPduType[MAX_SMS_NUMBER+1];			// PDU type
	int			aiContentLength[MAX_SMS_NUMBER+1];	// the length of sms content
	int			aiCompressLength[MAX_SMS_NUMBER+1];	// the length of sms content
	char		strContentData[MAX_SMS_NUMBER*PDU_SMS_CONTENT_LENGTH];	// the content of sms
	uint8_t		abyRev[24];
} tPduSmsSend;


typedef struct {
	int			index;			// the index in the sms memory
	char		strScts[16];	// the time of sms server, like "12032718250032" --- 2012-03-27 18:25:00 32
	char		strSca[64];		// server central tel number
	char		strOa[64];		// Orient address
	uint8_t		byStatus;		// 0:"REC UNREAD", 1:"REC READ", 2:"STO UNSENT", 3:"STO SENT", 4:"ALL"
	uint8_t		byPduType;		// PDU type
	uint8_t		byPid;			// Protocol ID
	uint8_t		byDcs;			// Decord Standard
	int			iContentLength;	// the length of sms content
	char		strContentData[336];	// the content of sms
} tPduSmsRecv;


typedef struct {
	int			index;			// the index in the sms memory
	uint8_t		abyTime[8];		// the time of sms server, format:YYMMDDHHMMSSZZ, ZZ is zone
								// like "\x12\x03\x27\x18\x25\x00\x32\x00" --- 2012-03-27 18:25:00 32
	tTelInfo	oSca;			// server central tel number
	tTelInfo	oOa;			// Orient address
	// Such as "\x05\x10\x08\x6F"---->"10086"
	// Such as "\x0d\x86\x18\x92\x84\x84\x80\x8f"---->"8618928484808"
	uint8_t		byStatus;		// 0:"REC UNREAD", 1:"REC READ", 2:"STO UNSENT", 3:"STO SENT", 4:"ALL"
	uint8_t		byCurItem;		// From 0 to TotalNum-1
	uint8_t		byTotalNum;		// TotalNum >= 1
	uint8_t		byDecodeStandard;// 0x00:ASCII; 0x08:UCS2(UNICODE 16); 0x04:Hex data
	int			iSmsLength;		// the length of sms content
	uint8_t		abyUniqueID[2];	// SMS unique ID
	char		stSmsData[170];	// the content of sms
} tSmsRecv; // 256 bytes

typedef struct {
	tTelInfo	oSca;			// server central tel number
	tTelInfo	oDstAddr;
	int			iSmsLength;		// the length of sms content
	uint8_t		abySmsData[MAX_SMS_NUMBER*153+20]; // the content of sms
									// ASCII Maxin length 3060 bytes
									// UCS2  Maxin length 2680 bytes
									// Hex	 Maxin length 2680 bytes

	uint8_t		byTpVp;			// SMS Validity Period
/*	TP-VP			value Validity period value
	0x00 to 0x8f	(TP-VP + 0x01) * 0x05 minutes	 (i.e. 5 minutes intervals up to 12 hours)
	0x90 to 0xa7	0x0c hours + ((TP-VP - 0x8f) * 0x1e minutes)
	0xa8 to 0xc4	(TP-VP - 0xa6) * 0x01 day
	0xc5 to 0xff	(TP-VP - 0xc0) * 0x01 week
*/

	uint8_t		byDecodeStandard; // 0x00:ASCII, 0x08:UCS2(UNICODE 16), 0x04:Hex data
	uint8_t		byStausReport;	// 1:need report send status, 0:not need report send status
	uint8_t		abyRev[21];
} tSmsSend;

/* USB_DT_DEVICE: Device descriptor */
typedef struct usb_device_descriptor {
	uint16_t	idVendor;
	uint16_t	idProduct;
	uint16_t	bcdDevice;
    char		dev_path[256];
} usb_descriptor_t;


typedef struct {
	int		(*wnet_Reset)(int);
	int		(*wnet_Init)(int);//transimt fd
	int		(*wnet_PowerDown)(int);//power down
	
	int		(*wnet_Signal)(int, int *);
	int		(*wnet_SetAttached)(int, int);//增加GPRS附网命令
	int		(*wnet_CheckSim)(int);
	int		(*wnet_InputSimPin)(int, const char *);//增加输入pin功能
	int		(*wnet_GetIMEI)(int, char *);
	int		(*wnet_GetInfo)(int, wnet_moduleinfo_t *);//获取模块信息
	
/* SMS api */
	int		(*wnet_Sms_GetList)(int, int, tSmsRecv *, int, int *);
	int		(*wnet_Sms_Send)(int, const tSmsSend *);
	int		(*wnet_Sms_Read)(int, int, tSmsRecv *);
	int		(*wnet_Sms_Delete)(int, int);

/*phone*/
	int		(*wnet_PhoneStatus)(int, char *, unsigned int);
	int		(*wnet_PhoneAnswer)(int);
	int		(*wnet_PhoneHangUp)(int);
	int		(*wnet_PhoneCall)(int, const char *);

/*other */
	int		(*wnet_GetBaseStationInfo)(int, tBaseStation *);
	int		(*wnet_GetNeighbourBaseStationInfo)(int, tBaseStation *, int, int *);
	int		(*wnet_ReadSimCardID)(int, char *, int);
	int		(*wnet_update_fw)(int, int);
	int		(*wnet_check_ppp_available)(int);//检查PPP连接有效性

/*power saveing*/
	int		(*wnet_resume)(int, void *, int);
	int		(*wnet_suspend)(int, void *, int);

	int		(*wnet_ussd_cmd)(int, int , const void *, int, int *, void *, int *);

	int		(*wnet_ReadSimCardIMSI)(int, char *, int);
	int		(*wnet_usb_detect)(int fd, const usb_descriptor_t desc[], int array_size);
	
//reserve api interface
	int		(*reserve[254])(int);
} wnet_operations_t;


typedef struct {
	char	*echo;
} wnet_echo_t;

int wlan_module_commport_open(const char *devname, int speed);
int wlan_module_commport_close(int fd);
int wlan_module_read_timeout(int fd, void *buf, uint32_t nbytes,
								uint32_t first_timeout, uint32_t last_timeout);
int wlan_module_exec_atcmd(int fd, const char *send, uint32_t *recv_len, char *recv, int mode,
							uint32_t first_recv_to, uint32_t block_recv_to);
int wlan_module_exec_atcmd_echo(int fd, const char *send, uint32_t *recv_len, char *recv, int mode,
								uint32_t first_recv_to, uint32_t block_recv_to, const wnet_echo_t *echo);
int wlan_module_load_patch(int fd, const char *modulename, const char *patchfile);


int wnet_init(const char *comm_dev);
int wnet_reset(void);
int wnet_power_down(void);
int wnet_power_on(void);
int wnet_signal(int *signal_num);
int wnet_set_attached(int flag);
int wnet_checksim(void);
int wnet_inputsimpin(const char *pin);
int wnet_getimei(char *IMEI);
int wnet_getmodinfo(wnet_moduleinfo_t *info);
int wnet_sms_getlist(int listtype, tSmsRecv sms[], int maxno, int *SmsNum);
int wnet_sms_send(const tSmsSend *sms);
int wnet_sms_read(int index, tSmsRecv *smsdata);
int wnet_sms_delete(int index);
int wnet_phone_status(char *PhoneNumber, uint32_t uiBuffSize);
int wnet_phone_answer(void);
int wnet_phone_hangup(void);
int wnet_phone_call(const char *PhoneNum);
int wnet_get_basestation_info(tBaseStation *psBaseStationInfo);
int wnet_get_neighbourbasestation_info(tBaseStation *psBaseStationInfo, int iNum, int *piOutNum);
int wnet_read_simcardID(char *psSimCardID, int iLen);
int wnet_read_simcardIMSI(char *IMSI, int iLen);
int wnet_update_fw(int mode);

int wnet_resume(void *arg, int len);
int wnet_suspend(void *arg, int len);

int wnet_ussd_cmd(int n, const void *sendstr, int dcs, int *m, void *recvstr, int *recvdcs);
int wnet_check_ppp_available(void);

int wnet_get_current_sim(void);
int wnet_select_sim(int simno);

int wnet_exec_at_command(const char *at, char *recvbuf, uint32_t first_recv_to, uint32_t block_recv_to);

int getruntime(struct timeval *__tv, __timezone_ptr_t __tz);
int sys_get_usb_desc(struct usb_device_descriptor *desc, int array_size);

extern void (*PRINTF)( const char * frm,...);

#define GPRS_DBG(value) do{PRINTF("%s<%d> value:%d\n",__FUNCTION__,__LINE__,value);}while(0)

#include <sys/types.h>
extern pid_t gettid(void);

#define wnet_api_enter(format, arg...)	\
	PRINTF("[%d]==>{%s}%s(" format ")\n", gettid(), __FILE__, __FUNCTION__, ##arg)
	
#define wnet_api_exit(format, arg...)	\
	do { \
		int saveerrno = errno; \
		PRINTF("[%d]<=={%s}%s()" format "\n", gettid(), __FILE__, __FUNCTION__, ##arg); \
		errno = saveerrno; \
	} while (0)

#define wnet_api_debug(format, arg...)	\
	{ \
			int saveerrno = errno; \
			PRINTF("{%s}<%s>: " format "\n", __FILE__, __FUNCTION__, ##arg); \
			errno = saveerrno; \
	} while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

