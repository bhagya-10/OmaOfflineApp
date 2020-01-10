
#ifndef _VPDN_H_
#define _VPDN_H_

typedef struct {
    struct in_addr server;      /* ip address */
    char domain[32];            /* maybe it is needed for windows server */
    char username[64];          /* username for ppp */
    char password[32];          /* password for ppp */
    int encrypt;                /* TRUE for encrypt(mppe-128) else no encrypt */
} pptp_arg_t;

typedef struct {
    struct in_addr server;      /* ip address */
    char domain[32];            /* maybe it is needed for windows server */
    char username[64];          /* username for ppp */
    char password[32];          /* password for ppp */
} l2tp_arg_t;

typedef struct {
    struct in_addr server;      /* ip address */
    char domain[32];            /* maybe it is needed for windows server */
    char username[64];          /* username for ppp */
    char password[32];          /* password for ppp */
    char ipsec_psk_secret[32];  /* pre-shared key */
} l2tp_psk_arg_t;

typedef struct {
    struct in_addr server;      /* ip address */
    char domain[32];            /* maybe it is needed for windows server */
    char username[64];          /* username for ppp */
    char password[32];          /* password for ppp */
    char cert_path[128];        /* CA files directory, which must have 5 files: 
                                     * 1) cacert.pem
                                     * 2) vpnclient_cert.pem, vpnclient_key.pem, 
                                     * 3) vpngateway_cert.pem, vpngateway_key.pem */
    char cert_client_pw[32];    /* the password when you generate vpnclient_cert.pem */
    char cert_server_pw[32];    /* the password when you generate vpngateway_cert.pem */
} l2tp_cert_arg_t;

typedef struct {
    int error;                  /* NON-zero indicate an error occur, see RESULT_ERROR_ENUM for detail */
    char intf_name[64];         /* vpdn interface */
    int ifindex;                /* vpdn interface id */
    int intf_is_up;             /* vpdn interface state */
} result_arg_t;

typedef enum {
    VPDN_NO_ERROR,              /* vpdn拨号成功，拨号连接已建立 */
    VPDN_IS_CONNECTING,         /* vpdn正在进行拨号，一般要主动获取VPDN状态时才会返回此状态 */
    VPDN_IS_DISCONNECTED,       /* vpdn连接已断开，拨号连接由建立状态转断开状态时通告此值 */
    VPDN_ERROR,                 /* vpdn其他错误 */
    VPDN_CERT_FILE_UNAVAILABLE, /* vpdn证书不存在或不可读 */
    VPDN_NO_DEFAULT_ROUTE,      /* 系统默认路由不存在 */
    
} RESULT_ERROR_ENUM;

typedef void (*result_notify_func_t)(result_arg_t *arg);

/**
 * vpdn_open - vpdn库初始化
 *
 * @debug_mode: 是否让vpdn库处于调试状态
 *
 * 会创建pthread线程与vpdn管理器进行连接
 * vpdn管理器每次只允许一个应用程序与其连接，如果需要多个应用程序进行VPDN拨号，
 * 可调用vpdn_close断开与vpdn管理器的连接
 *
 * Return: 成功返回0，失败返回负值
 */
int vpdn_open(int debug_mode);

/**
 * vpdn_close - 关闭vpdn库
 *
 * 会断开与vpdn管理器的连接，但不会删除线程。
 * 注意，调用本接口不会主动挂断vpdn拨号连接。如需挂断vpdn拨号连接，请调用vpdn_disconnect
 *
 * Return: 成功返回0，失败返回负值
 */
int vpdn_close(void);

/**
 * vpdn_call_pptp - 使用pptp协议进行vpdn拨号
 *
 * @arg: pptp协议的拨号参数，具体参见pptp_arg_t
 * @func: pptp拨号结束的回调函数，可以从result_arg_t知道拨号的结果
 *
 * 拨号成功结束后，如果vpdn interface出现状态变化或者vpdn拨号连接中断，那么该回调会再次被调用
 * 
 * Return: 成功返回0，失败返回负值
 */
int vpdn_call_pptp(pptp_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_call_l2tp - 不使用IPSec加密，仅使用l2tp协议进行vpdn拨号
 *
 * @arg: l2tp协议的拨号参数，具体参见l2tp_arg_t
 * @func: l2tp拨号结束的回调函数，可以从result_arg_t知道拨号的结果
 *
 * 拨号成功结束后，如果vpdn interface出现状态变化或者vpdn拨号连接中断，那么该回调会再次被调用
 */
int vpdn_call_l2tp(l2tp_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_call_l2tp_psk - 使用预共享密钥的IPSec加密方式进行l2tp协议进行vpdn拨号
 *
 * @arg: l2tp协议的拨号参数，具体参见l2tp_psk_arg_t
 * @func: l2tp拨号结束的回调函数，可以从result_arg_t知道拨号的结果
 *
 * PSK: Pre-Shared Key, 是ipsec的一种加密方式。l2tp协议本身并未加密，它需要ipsec加密来保障数据的安全性
 * 拨号成功结束后，如果vpdn interface出现状态变化或者vpdn拨号连接中断，那么该回调会再次被调用
 * 
 * Return: 成功返回0，失败返回负值
 */
int vpdn_call_l2tp_psk(l2tp_psk_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_call_l2tp_cert - 使用证书的IPSec加密方式进行l2tp协议进行vpdn拨号
 *
 * @arg: l2tp协议的拨号参数，具体参见l2tp_cert_arg_t
 * @func: l2tp拨号结束的回调函数，可以从result_arg_t知道拨号的结果
 *
 * 证书加密是ipsec的另一种加密方式,官方名称叫做X.509,有时也可称做RSA，因为X.509也是基于RSA
 * 调用者需要提供CA根证书，用户证书和服务器证书，具体见l2tp_cert_arg_t
 * 拨号成功结束后，如果vpdn interface出现状态变化或者vpdn拨号连接中断，那么该回调会再次被调用
 * 
 * Return: 成功返回0，失败返回负值
 */
int vpdn_call_l2tp_cert(l2tp_cert_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_disconnect - 挂断vpdn拨号连接
 *
 * @func: 挂断vpdn拨号连接后的回调函数，可以从result_arg_t知道本接口的操作结果
 *
 * 适用于pptp, l2tp/ipsec psk, l2tp/ipsec cert协议建立的vpdn连接 
 * 
 * Return: 成功返回0，失败返回负值
 */
int vpdn_disconnect(result_notify_func_t func);

/**
 * vpdn_generate_def_route - 是否生成vpdn的默认路由
 *
 * @is_gen_def_route: TRUE or FALSE
 *
 * 默认不生成指向vpn网络的默认路由, 从vpdn创建成功后的通告可以获取到通向vpn网络的接口名称和接口索引，如ppp1；
 * 而VPN网络里的DNS服务器地址存放在/var/run/ppp/resolv.conf.vpdn
 * 
 * 当生成指向该vpn网络的默认路由时，旧有的默认路由和 DNS 会被替换。当vpdn拨号连接挂断后，默认路由和DNS会被
 * 恢复为拨号前的配置
 *
 * 配置生成默认路由只对当次拨号连接有效，vpdn拨号断开后恢复为默认值
 * 
 * Return: 成功返回0，失败返回负值
 */
int vpdn_generate_def_route(int is_gen_def_route);


#endif /* _VPDN_H_ */
