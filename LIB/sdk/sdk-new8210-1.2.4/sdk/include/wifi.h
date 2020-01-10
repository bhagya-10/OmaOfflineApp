#ifndef _WIFI_H_
#define _WIFI_H_

/* WIFI密码格式 */
typedef enum { 
    WIFI_KEY_TYPE_NOKEY,  /*不使用密码*/
    WIFI_KEY_TYPE_HEX,    /*使用十六进制的密码格式*/
    WIFI_KEY_TYPE_ASCII   /*使用ASCII的密码格式*/
} WIFI_KEY_TYPE;

/* WIFI安全加密模式 */
typedef enum { 
    WIFI_NET_SEC_NONE,       /*不使用安全加密模式*/
    WIFI_NET_SEC_WEP_OPEN,   /*使用开放密钥的WEP模式*/
    WIFI_NET_SEC_WEP_SHARED, /*使用共享密钥的WEP模式*/
    WIFI_NET_SEC_WPA,        /*使用WPA模式*/
    WIFI_NET_SEC_WPA2,        /*使用WPA2模式*/
    WIFI_NET_SEC_WPA_PSK,     /*使用WPA-PSK模式*/
    WIFI_NET_SEC_WPA2_PSK     /*使用WPA2-PSK模式*/
} WIFI_SEC_MODE;

typedef struct {
    char  SSID[124];   /*WIFI用户名*/
    int   SecMode;     /*安全加密模式，参见enum WIFI_SEC_MODE*/
} ssid_info_t;

typedef struct {
    char  SSID[124];   /*WIFI用户名*/
    int   SecMode;     /*安全加密模式，参见enum WIFI_SEC_MODE*/
    int   signal;
    char  pad[124];
} ssid_info_more_t;

/* WIFI通讯参数结构体 */
typedef struct {
    char  Pad[3];           /* 预留 */
    char  DHCP;             /*是否开启DHCP,0-否，1-是*/
    int   KeyType;          /*密码格式，参见enum WIFI_KEY_TYPE*/
    int   SecMode;          /*安全加密模式，参见enum WIFI_SEC_MODE*/
    char  SSID[128];        /*WIFI用户名*/
    char  Pwd[64];         /*WIFI密码*/
    char  Pwd2[64];         /* WEP 密码2 */
    char  Pwd3[64];         /* WEP 密码3 */
    char  Pwd4[64];         /* WEP 密码4 */
    char  IP[16];           /*本地IP地址*/
    char  Gateway[16];	    /*本地网关*/
    char  Mask[16];         /*本地子网掩码*/
    char  Dns1[16];         /*DNS*/
    char  Dns2[16];         /*DNS*/
} ssid_connect_t;


typedef enum { 
	WR_DEVICE_UNAVAILABLE = -8,  /* 设备不可用 */
	WR_PARA_ERR      = -7,       /* 参数错*/
	WR_CONNECT_ERR   = -6,       /* 无法连接*/
    WR_CONNECTING    = -5,       /* 通讯连接中*/
	WR_LOGOFF        = -4,       /* 通讯链路断开*/
	WR_CANCEL        = -3,       /* 按键取消*/
	WR_TIMEOUT       = -2,       /* 超时*/
    WR_FAIL          = -1,       /* 失败*/
    WR_SUCCESS	    =  0,        /* 成功*/
    WR_LINK_OK       =  1,       /* 通讯链路OK */
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
 * wifi_reassoc - wifi重新关联 
 *
 * 通过wifi连接不上tcp后台时，可以调用本接口重新关联。
 * 重新关联后会重新获取ip
 *
 * 成功返回0，失败返回非0
 */
int wifi_reassoc(void);

/**
 * wifi_config_ui - 弹出wifi的配置界面
 *
 * @fontsize: 配置界面的字体大小，字体默认为微软雅黑，字体大小一般设置为21
 * @language: 1: 中文，0: 英文
 *
 * 配置成功返回0，不成功返回非0，例如按取消推出配置界面
 */
int wifi_config_ui(int fontsize, int language);

/**
 * wifi_get_current_ssid - 获取当前已连接的ssid
 *
 * 如果能获取到当前已连接的ssid，说明wifi已经连接上
 * 通过wifi_config_ui配置wifi后，可以调用本接口确认wifi是否连接成功
 */
int wifi_get_current_ssid(char *connected_ssid, int len);


#endif /* _WIFI_H_ */

