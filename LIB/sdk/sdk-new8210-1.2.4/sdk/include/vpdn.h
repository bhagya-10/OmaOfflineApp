
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
    VPDN_NO_ERROR,              /* vpdn���ųɹ������������ѽ��� */
    VPDN_IS_CONNECTING,         /* vpdn���ڽ��в��ţ�һ��Ҫ������ȡVPDN״̬ʱ�Ż᷵�ش�״̬ */
    VPDN_IS_DISCONNECTED,       /* vpdn�����ѶϿ������������ɽ���״̬ת�Ͽ�״̬ʱͨ���ֵ */
    VPDN_ERROR,                 /* vpdn�������� */
    VPDN_CERT_FILE_UNAVAILABLE, /* vpdn֤�鲻���ڻ򲻿ɶ� */
    VPDN_NO_DEFAULT_ROUTE,      /* ϵͳĬ��·�ɲ����� */
    
} RESULT_ERROR_ENUM;

typedef void (*result_notify_func_t)(result_arg_t *arg);

/**
 * vpdn_open - vpdn���ʼ��
 *
 * @debug_mode: �Ƿ���vpdn�⴦�ڵ���״̬
 *
 * �ᴴ��pthread�߳���vpdn��������������
 * vpdn������ÿ��ֻ����һ��Ӧ�ó����������ӣ������Ҫ���Ӧ�ó������VPDN���ţ�
 * �ɵ���vpdn_close�Ͽ���vpdn������������
 *
 * Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
 */
int vpdn_open(int debug_mode);

/**
 * vpdn_close - �ر�vpdn��
 *
 * ��Ͽ���vpdn�����������ӣ�������ɾ���̡߳�
 * ע�⣬���ñ��ӿڲ��������Ҷ�vpdn�������ӡ�����Ҷ�vpdn�������ӣ������vpdn_disconnect
 *
 * Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
 */
int vpdn_close(void);

/**
 * vpdn_call_pptp - ʹ��pptpЭ�����vpdn����
 *
 * @arg: pptpЭ��Ĳ��Ų���������μ�pptp_arg_t
 * @func: pptp���Ž����Ļص����������Դ�result_arg_t֪�����ŵĽ��
 *
 * ���ųɹ����������vpdn interface����״̬�仯����vpdn���������жϣ���ô�ûص����ٴα�����
 * 
 * Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
 */
int vpdn_call_pptp(pptp_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_call_l2tp - ��ʹ��IPSec���ܣ���ʹ��l2tpЭ�����vpdn����
 *
 * @arg: l2tpЭ��Ĳ��Ų���������μ�l2tp_arg_t
 * @func: l2tp���Ž����Ļص����������Դ�result_arg_t֪�����ŵĽ��
 *
 * ���ųɹ����������vpdn interface����״̬�仯����vpdn���������жϣ���ô�ûص����ٴα�����
 */
int vpdn_call_l2tp(l2tp_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_call_l2tp_psk - ʹ��Ԥ������Կ��IPSec���ܷ�ʽ����l2tpЭ�����vpdn����
 *
 * @arg: l2tpЭ��Ĳ��Ų���������μ�l2tp_psk_arg_t
 * @func: l2tp���Ž����Ļص����������Դ�result_arg_t֪�����ŵĽ��
 *
 * PSK: Pre-Shared Key, ��ipsec��һ�ּ��ܷ�ʽ��l2tpЭ�鱾��δ���ܣ�����Ҫipsec�������������ݵİ�ȫ��
 * ���ųɹ����������vpdn interface����״̬�仯����vpdn���������жϣ���ô�ûص����ٴα�����
 * 
 * Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
 */
int vpdn_call_l2tp_psk(l2tp_psk_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_call_l2tp_cert - ʹ��֤���IPSec���ܷ�ʽ����l2tpЭ�����vpdn����
 *
 * @arg: l2tpЭ��Ĳ��Ų���������μ�l2tp_cert_arg_t
 * @func: l2tp���Ž����Ļص����������Դ�result_arg_t֪�����ŵĽ��
 *
 * ֤�������ipsec����һ�ּ��ܷ�ʽ,�ٷ����ƽ���X.509,��ʱҲ�ɳ���RSA����ΪX.509Ҳ�ǻ���RSA
 * ��������Ҫ�ṩCA��֤�飬�û�֤��ͷ�����֤�飬�����l2tp_cert_arg_t
 * ���ųɹ����������vpdn interface����״̬�仯����vpdn���������жϣ���ô�ûص����ٴα�����
 * 
 * Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
 */
int vpdn_call_l2tp_cert(l2tp_cert_arg_t *arg, result_notify_func_t func);

/**
 * vpdn_disconnect - �Ҷ�vpdn��������
 *
 * @func: �Ҷ�vpdn�������Ӻ�Ļص����������Դ�result_arg_t֪�����ӿڵĲ������
 *
 * ������pptp, l2tp/ipsec psk, l2tp/ipsec certЭ�齨����vpdn���� 
 * 
 * Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
 */
int vpdn_disconnect(result_notify_func_t func);

/**
 * vpdn_generate_def_route - �Ƿ�����vpdn��Ĭ��·��
 *
 * @is_gen_def_route: TRUE or FALSE
 *
 * Ĭ�ϲ�����ָ��vpn�����Ĭ��·��, ��vpdn�����ɹ����ͨ����Ի�ȡ��ͨ��vpn����Ľӿ����ƺͽӿ���������ppp1��
 * ��VPN�������DNS��������ַ�����/var/run/ppp/resolv.conf.vpdn
 * 
 * ������ָ���vpn�����Ĭ��·��ʱ�����е�Ĭ��·�ɺ� DNS �ᱻ�滻����vpdn�������ӹҶϺ�Ĭ��·�ɺ�DNS�ᱻ
 * �ָ�Ϊ����ǰ������
 *
 * ��������Ĭ��·��ֻ�Ե��β���������Ч��vpdn���ŶϿ���ָ�ΪĬ��ֵ
 * 
 * Return: �ɹ�����0��ʧ�ܷ��ظ�ֵ
 */
int vpdn_generate_def_route(int is_gen_def_route);


#endif /* _VPDN_H_ */
