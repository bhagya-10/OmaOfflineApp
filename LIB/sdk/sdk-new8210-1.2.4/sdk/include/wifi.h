#ifndef _WIFI_H_
#define _WIFI_H_

/* WIFI�����ʽ */
typedef enum { 
    WIFI_KEY_TYPE_NOKEY,  /*��ʹ������*/
    WIFI_KEY_TYPE_HEX,    /*ʹ��ʮ�����Ƶ������ʽ*/
    WIFI_KEY_TYPE_ASCII   /*ʹ��ASCII�������ʽ*/
} WIFI_KEY_TYPE;

/* WIFI��ȫ����ģʽ */
typedef enum { 
    WIFI_NET_SEC_NONE,       /*��ʹ�ð�ȫ����ģʽ*/
    WIFI_NET_SEC_WEP_OPEN,   /*ʹ�ÿ�����Կ��WEPģʽ*/
    WIFI_NET_SEC_WEP_SHARED, /*ʹ�ù�����Կ��WEPģʽ*/
    WIFI_NET_SEC_WPA,        /*ʹ��WPAģʽ*/
    WIFI_NET_SEC_WPA2,        /*ʹ��WPA2ģʽ*/
    WIFI_NET_SEC_WPA_PSK,     /*ʹ��WPA-PSKģʽ*/
    WIFI_NET_SEC_WPA2_PSK     /*ʹ��WPA2-PSKģʽ*/
} WIFI_SEC_MODE;

typedef struct {
    char  SSID[124];   /*WIFI�û���*/
    int   SecMode;     /*��ȫ����ģʽ���μ�enum WIFI_SEC_MODE*/
} ssid_info_t;

typedef struct {
    char  SSID[124];   /*WIFI�û���*/
    int   SecMode;     /*��ȫ����ģʽ���μ�enum WIFI_SEC_MODE*/
    int   signal;
    char  pad[124];
} ssid_info_more_t;

/* WIFIͨѶ�����ṹ�� */
typedef struct {
    char  Pad[3];           /* Ԥ�� */
    char  DHCP;             /*�Ƿ���DHCP,0-��1-��*/
    int   KeyType;          /*�����ʽ���μ�enum WIFI_KEY_TYPE*/
    int   SecMode;          /*��ȫ����ģʽ���μ�enum WIFI_SEC_MODE*/
    char  SSID[128];        /*WIFI�û���*/
    char  Pwd[64];         /*WIFI����*/
    char  Pwd2[64];         /* WEP ����2 */
    char  Pwd3[64];         /* WEP ����3 */
    char  Pwd4[64];         /* WEP ����4 */
    char  IP[16];           /*����IP��ַ*/
    char  Gateway[16];	    /*��������*/
    char  Mask[16];         /*������������*/
    char  Dns1[16];         /*DNS*/
    char  Dns2[16];         /*DNS*/
} ssid_connect_t;


typedef enum { 
	WR_DEVICE_UNAVAILABLE = -8,  /* �豸������ */
	WR_PARA_ERR      = -7,       /* ������*/
	WR_CONNECT_ERR   = -6,       /* �޷�����*/
    WR_CONNECTING    = -5,       /* ͨѶ������*/
	WR_LOGOFF        = -4,       /* ͨѶ��·�Ͽ�*/
	WR_CANCEL        = -3,       /* ����ȡ��*/
	WR_TIMEOUT       = -2,       /* ��ʱ*/
    WR_FAIL          = -1,       /* ʧ��*/
    WR_SUCCESS	    =  0,        /* �ɹ�*/
    WR_LINK_OK       =  1,       /* ͨѶ��·OK */
} WififResultStatus;

int wifi_power_control(int to_open);
int wifi_power_control_force(int to_open);
int wifi_set_high_speed(int is_high_speed);
int wifi_scan_ssid(ssid_info_t ppszSSIDList[], int *pnNumList);
int wifi_scan_ssid_more(ssid_info_more_t ppszSSIDList[], int *pnNumList);
int wifi_connect_ssid(const ssid_connect_t *st_wificonfig);
int wifi_disconnect(void);
int wifi_get_status(void);
int wifi_get_signal(void);
int wifi_set_icon_on(int is_icon_on);

/**
 * wifi_reassoc - wifi���¹��� 
 *
 * ͨ��wifi���Ӳ���tcp��̨ʱ�����Ե��ñ��ӿ����¹�����
 * ���¹���������»�ȡip
 *
 * �ɹ�����0��ʧ�ܷ��ط�0
 */
int wifi_reassoc(void);

/**
 * wifi_config_ui - ����wifi�����ý���
 *
 * @fontsize: ���ý���������С������Ĭ��Ϊ΢���źڣ������Сһ������Ϊ21
 * @language: 1: ���ģ�0: Ӣ��
 *
 * ���óɹ�����0�����ɹ����ط�0�����簴ȡ���Ƴ����ý���
 */
int wifi_config_ui(int fontsize, int language);

/**
 * wifi_get_current_ssid - ��ȡ��ǰ�����ӵ�ssid
 *
 * ����ܻ�ȡ����ǰ�����ӵ�ssid��˵��wifi�Ѿ�������
 * ͨ��wifi_config_ui����wifi�󣬿��Ե��ñ��ӿ�ȷ��wifi�Ƿ����ӳɹ�
 */
int wifi_get_current_ssid(char *connected_ssid, int len);


#endif /* _WIFI_H_ */

