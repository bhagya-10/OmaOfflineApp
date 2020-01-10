#ifndef     _DEFINE_H_
#define     _DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif
//#include <stdint.h>
#define DebugModuleName "ums_tms"


#define     ON                      0x01
#define     OFF                                         0x00

#define     YES                     0x01
#define     NO                      0x00

#define     OK                                          0x00

#define  ERROR                                  0x01


#define     TMS_PROMPT_EN       0       //  prompt in English
#define     TMS_PROMPT_CHN      1       //  prompt in Chinese
#define     TMS_PROTO_NEWTMS    0       //  tms protocol: newtms

#define     TMS_COMM_GPRS       1       //  uses socket comm(gprs, cdma, lan...)
#define     TMS_COMM_CDMA       2       //  uses socket comm(gprs, cdma, lan...)
#define     TMS_COMM_PPP        3       //  uses PPP comm
#define     TMS_COMM_MDMASY     4       //  uses MODEM ASYN
#define     TMS_COMM_UART       5       //  uses RS232 comm
#define     TMS_COMM_LAN                6                       //uses LAN


#define     NEWTMS_MAX_TASKNUM              127         //  ����������Ŀ

//#define       FIRMWARE_DOWN_LINUX     0x11    //  7110�ں�����
//#define       FILE_DOWN_LINUX         0x12    //  7110�ļ�����
//#define       DELETE_FILE_LINUX       0x13    //  7110�ļ�ɾ��
//#define     DYMATIC_FILE_LINUX		 0x14		//dongtaiku
#define     PARAM_FILE_LINUX                 0x01      //Ӧ�ò����ļ�
#define     DYMATIC_FILE_LINUX               0x04      //api����ģ��
#define     DATA_FILE_DOWN           0x06   //  �����ļ�����
#define     FIRMWARE_DOWN_LINUX      0x08    //  7110�ں�����
#define     FILE_DOWN_LINUX          0x09      //��ִ�г���

#define     DESKTOP_FILE_LINUX               0x0A     //desktop
#define     PORT_RET_BUSY           0x05        //  û����������Դ����
#define     PORT_RET_PORTERR        0x02        //  �߼����ںŷǷ�
#define     PORT_RET_PARAERR        0xfe        //  ��Ч��ͨѶ����
#define     PORT_RET_OK             0x00        //  �ɹ�

/* UART */
#define     PORT_COM1                           0x00        //  ���ؿ�
#define     PORT_MODEM                          0x01        //  modemģ��
#define     PORT_WNET                           0x02        //  ����ģ�����̫��
#define     PORT_PINPAD             0x03        //  PINPAD��
#define     PORT_GPS                0x04        //  GPS
#define         PORT_EXT                                0x05            //	��չ��

//  �ļ�����
#define     FILE_TYPE_BOOTLOADER    0x80    //  BOOT LOADER���
#define     FILE_TYPE_BOOT          0x81    //  BOOT���
#define     FILE_TYPE_MONITOR       0x82    //  MONITOR���
#define     FILE_TYPE_CONFIG        0x90    //  ϵͳ�����ļ�
#define     FILE_TYPE_FONT          0xA0    //  �ֿ��ļ�
#define     FILE_TYPE_TMS           0xA1    //  tms��ʱ�ļ�
#define     FILE_TYPE_PED           0xB0    //  PED�ļ�

#define     FILE_TYPE_APP           0x01    //  0x01    //  Ӧ�ó���
#define     FILE_TYPE_DSRM          0x02    //  0x07    //  Ӧ�ó�����ʾע���ļ�
#define     FILE_TYPE_DATA          0x03    //  0x04    //  Ӧ�ó��򴴽��������ļ�
#define     FILE_TYPE_PARA          0x04    //  0x06    //  �����ļ�
#define     FILE_TYPE_ASPUBKEY      0x11    //  �յ���ǩ����Կ�ļ�
#define     FILE_TYPE_AAPUBKEY      0x12    //  �յ���KISSA��Ȩ��Կ�ļ�


#define NET_OK              0       //  �޴�������
#define NET_ERR_RSP         1024    //  "ERROR" is return from the module
#define NET_ERR_NOSIM       1025    //  SIM/UIM card is not inserted or not detected
#define NET_ERR_PIN         1026    //  SIM PIN is required
#define NET_ERR_PUK         1027    //  SIM PUK is required
#define NET_ERR_PWD         1028    //  SIM PIN is incorrect
#define NET_ERR_SIMDESTROY  1029    //  SIM card destroy
#define NET_ERR_CSQWEAK     1030    //  the signal is too weak, please check the antenna
#define NET_ERR_LINKCLOSED  1031    //  the module is offline
#define NET_ERR_LINKOPENING 1032    //  the module is connecting to the network
#define NET_ERR_DETTACHED   1033    //  Dettached
#define NET_ERR_ATTACHING   1034    //  the module is looking for the base station.
#define NET_ERR_EMERGENCY   1035    //  SIM/UIM is in emergency status
#define NET_ERR_RING        1036    //  detect ringing
#define NET_ERR_BUSY        1037    //  detect call in progress
#define NET_ERR_DIALING     1038    //  dialing
#define NET_ERR_UNKNOWN     1039    //  unknown module type
#define NET_ERR_ABNORMAL    1040    //  abnormal error
#define NET_ERR_NOMODULE    1041    //  no module in the unit

#define UMAX_APP_COUNT		30 //Ӧ���������
#define UMAX_MODULE_COUNT	100 //�����ļ��������
//Ӧ����Ϣ�ṹ��
typedef struct
{
	char AppName[20+1]; //Ӧ����
	char AppDispName[20+1]; //Ӧ����ʾ��,�������01000006��ǩ,���ͱ�ǩ����
	char AppVar[15+1];        // Ӧ�ð汾�� tmsϵͳֻ֧��15λ�汾�� add wgz
	char TerminalID[8+1]; //�ն˺�
	char ShopId[15+1]; //�̻���
	uint8_t bisMainApp; //�Ƿ�����Ӧ�ã�0-��1-��
}UAPPINFO;
//Ӧ����Ϣ�ܽṹ��
typedef struct
{
	uint8_t bCount; //ʵ��Ӧ����
	UAPPINFO AppInfo[UMAX_APP_COUNT]; //�μ�struct UAPPINFO
}UAPPINFOLIST;

//ģ������ö��
typedef enum
{
	UMODULE_FILE = 0, //�ļ���2-5���ļ�����ʹ��0��������������
	UMODULE_PARAFILE = 1, //�����ļ�
	UMODULE_TRADE = 2, //ҵ��ģ��
	UMODULE_PUBLIC = 3, //����ģ��
	UMODULE_API = 4, //APIģ��
	UMODULE_PRIVATE = 5, //����˽�п�
	UMODULE_DATA = 6, //�������ļ�(Ԥ��)
	UMODULE_DRIVER = 7, //Ӳ������(Ԥ��)
	UMODULE_OS = 8, //�ײ�OS(Ԥ��)
	UMODULE_APP = 9        //Ӧ�ã���ִ���ļ� add andy.wang 
}UModuleType;

//ģ����Ϣ�ṹ��
typedef struct
{
	char ModuleName[50]; //ģ����
	int ModuleType; //�μ�enum UModuleType
	char AppName[20+1];      //Ӧ�������������ļ�����ĳ��Ӧ�� add andy.wang
}UMODULEINFO;


//ģ����Ϣ�ܽṹ��
typedef struct
{
	uint8_t bCount; //ʵ�������ļ���
	UMODULEINFO ModuleInfo[UMAX_MODULE_COUNT];//�μ�struct UMODULEINFO
}UMODULEINFOLIST;


//TMS����ͨѶ����ö��
typedef enum
{
	TMS_BYRS232 = 1, //����
	TMS_BYMODEM = 2, //MODEM�첽
	TMS_BYETHENET = 3, //������̫��
	TMS_BYWIFI = 4, //WIFI
	TMS_BYGPRS = 5, //GPRS��WCDMA��wcdma��gprs������ͬ��
	TMS_BYCDMA = 6, //CDMA
	TMS_BYMODEMPPP = 7, //MODEMPPP���ź���TCPIP
	TMS_BYUSB = 8 //USB
}TMSComType;

//TMS����ͨѶ�ṹ
typedef struct
{
	unsigned char ucCOMMType; /*ͨ�����ͣ��μ� enum UTMSCOMTYPE*/
	unsigned char ucTPDUFlag; /*TPDU���ڱ�ʶ: 0-�ޣ�1-��*/
	unsigned char aucTPDU[5]; /*TPDU���루���ʶ��TPDU��*/
	unsigned int uiSockID; /*tcpipͨѶSocket ID UTMSCOMTYPE��3,4,5,6,7*/
	unsigned long hCommHandle; /*ͨѶ��� UTMSCOMTYP�� 1,2,8*/
	/*���²���Ϊ����POS�����õ��Ĳ������������Ҳ��ÿɶ��������õ�������Ҫ��Ӳ����뼰ʱ��Ӧ*/
	unsigned char ucPackNum; /*���ؽ׶η������������������ذ���������С1�����8*/
	unsigned int uiFrameLen; /*���ؽ׶δ�������֡�ĳ��ȣ���λ���ֽڣ�*/
	unsigned char ucSyncFlag; /*ǿ���ն�������ִ��һ��һ���ı��Ľ���ģʽ��0-�ر� 1-��*/
	unsigned char ucIfCheck; /*���ؽ׶�����У���ʶ��0-��У�� 1-У��*/
	unsigned char ucBreakType; /*���ؽ׶��Ƿ���������ȡ��/��������ж�����0-������ 1-����*/
	unsigned int uiReTime; /*ͨѶ���Ľ��ճ�ʱ�����½��մ���(���������ط����Ĵ�����*/
	unsigned int uiTimeoutMs; /*����ͨѶ���Ľ��ճ�ʱʱ�䣨��λ�����룩*/
	unsigned int uiDelayTime; /*�ն�ÿ���������ݰ�֮ǰ����ʱ����λ�����룩*/
	unsigned int uiSlotTime; /*�������������������ʱ�䣨��λ�����룩*/
	unsigned char auReserver[100]; /*Ԥ����*/
	unsigned int uiRFULen; /*��չ�ռ䳤��*/
	void *pvRFU; /*��չ�ռ�ָ��*/
}TMSPARAMETERS;


//TMS��API����ֵö��
typedef enum
{
	TMS_FAIL = -1, //ʧ��
	TMS_OK = 0 //�ɹ�
}TMSStatus;

#ifdef __cplusplus
}
#endif


#endif

