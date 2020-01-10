
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
 * bluez_rfcomm_connect_uuid - 通过uuid连接rfcomm
 *
 * @btaddr: 要连接的对端的蓝牙设备地址，可通过bluez_list_paired_device_var获取
 * @uuid: uuid字符串，例如: "00001101-0000-1000-8000-00805F9B34FB"
 * @dev_name_buf: 输出参数，连接成功后返回蓝牙串口设备文件，例如: "/dev/rfcomm0"
 * @buflen: dev_name_buf的长度
 *
 * 连接成功后，系统会创建一个蓝牙串口设备文件，应用程序可以通过该串口设备文件与对端通信。
 * 注意: 在open该串口后要设置串口波特率，例如:
 * tty_property_config(fd, 115200, 8, 'n', 1, 'n');
 * 
 * 对端断开蓝牙连接时，串口的read操作会返回错误。反过来如果串口read操作返回错误也表示蓝牙连接已断开
 *
 * 成功返回0，失败返回非0.
 */ 
int bluez_rfcomm_connect_uuid(const char *btaddr, const char *uuid, char *dev_name_buf, int buflen);

/**
 * bluez_rfcomm_connect_channel - 通过指定的通道连接rfcomm
 *
 * 操作方法与bluez_rfcomm_connect_uuid一样
 *
 */
int bluez_rfcomm_connect_channel(const char *btaddr, int channel, char *dev_name_buf, int buflen);

/**
 * bluez_rfcomm_disconnect - 断开rfcomm
 *
 * @dev_name: bluez_rfcomm_connect_uuid或bluez_rfcomm_connect_channel返回的串口设备文件
 *
 * close串口设备文件后才去断开rfcomm，断开rfcomm后串口设备文件会被删除
 */
int bluez_rfcomm_disconnect(const char *dev_name);

/**
 * bluez_rfcomm_disconnect_all - 断开所有的本设备当前的所有蓝牙连接
 *
 * 防止应用程序异常退出时未能断开之前的蓝牙连接，新启动的程序可以先调用本接口确保所有的蓝牙连接都已断开。
 */
int bluez_rfcomm_disconnect_all(void);

int bluez_setup(void);

int bluez_power_down(void);

#endif

