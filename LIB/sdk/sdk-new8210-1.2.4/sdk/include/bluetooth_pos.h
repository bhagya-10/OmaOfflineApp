
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#define BLUETOOTH_API_SERVER        "/var/tmp/bluetooth_api_server_socket"

#undef  MAX_PAIRED_DEVICES          /* this definition is removed */
#define MAX_DEV_NAME_LEN            64
#define MAX_BADDR_LEN               32

enum BLUEZ_API_ID {
    GET_ADAPTER_INFO,
    LIST_PAIRED_DEVICE,
    SET_VISIBLE,
    
    RFCOMM_CONNECT_CHANNEL,
    RFCOMM_CONNECT_UUID,
    RFCOMM_DISCONNECT,
    RFCOMM_DISCONNECT_ALL,

    MAX_BLUEZ_API_ID
};

struct remote_dev_info {
    char name[MAX_DEV_NAME_LEN];
    char bdaddr[MAX_BADDR_LEN];
};

struct local_dev_info {
    char name[MAX_DEV_NAME_LEN];
    char bdaddr[MAX_BADDR_LEN];
};

struct rfcomm_connect_info {
    char btaddr[MAX_BADDR_LEN];
    char uuid[64];
    int channel;
};

int bluez_list_paired_device_var(struct remote_dev_info *devinfo, int count);
int bluez_set_visible(int on_off);
int bluez_get_info(struct local_dev_info *devinfo);

/**
 * bluez_rfcomm_connect_uuid - ͨ��uuid����rfcomm
 *
 * @btaddr: Ҫ���ӵĶԶ˵������豸��ַ����ͨ��bluez_list_paired_device_var��ȡ
 * @uuid: uuid�ַ���������: "00001101-0000-1000-8000-00805F9B34FB"
 * @dev_name_buf: ������������ӳɹ��󷵻����������豸�ļ�������: "/dev/rfcomm0"
 * @buflen: dev_name_buf�ĳ���
 *
 * ���ӳɹ���ϵͳ�ᴴ��һ�����������豸�ļ���Ӧ�ó������ͨ���ô����豸�ļ���Զ�ͨ�š�
 * ע��: ��open�ô��ں�Ҫ���ô��ڲ����ʣ�����:
 * tty_property_config(fd, 115200, 8, 'n', 1, 'n');
 * 
 * �Զ˶Ͽ���������ʱ�����ڵ�read�����᷵�ش��󡣷������������read�������ش���Ҳ��ʾ���������ѶϿ�
 *
 * �ɹ�����0��ʧ�ܷ��ط�0.
 */ 
int bluez_rfcomm_connect_uuid(const char *btaddr, const char *uuid, char *dev_name_buf, int buflen);

/**
 * bluez_rfcomm_connect_channel - ͨ��ָ����ͨ������rfcomm
 *
 * ����������bluez_rfcomm_connect_uuidһ��
 *
 */
int bluez_rfcomm_connect_channel(const char *btaddr, int channel, char *dev_name_buf, int buflen);

/**
 * bluez_rfcomm_disconnect - �Ͽ�rfcomm
 *
 * @dev_name: bluez_rfcomm_connect_uuid��bluez_rfcomm_connect_channel���صĴ����豸�ļ�
 *
 * close�����豸�ļ����ȥ�Ͽ�rfcomm���Ͽ�rfcomm�󴮿��豸�ļ��ᱻɾ��
 */
int bluez_rfcomm_disconnect(const char *dev_name);

/**
 * bluez_rfcomm_disconnect_all - �Ͽ����еı��豸��ǰ��������������
 *
 * ��ֹӦ�ó����쳣�˳�ʱδ�ܶϿ�֮ǰ���������ӣ��������ĳ�������ȵ��ñ��ӿ�ȷ�����е��������Ӷ��ѶϿ���
 */
int bluez_rfcomm_disconnect_all(void);

int bluez_setup(void);

int bluez_power_down(void);

#endif

