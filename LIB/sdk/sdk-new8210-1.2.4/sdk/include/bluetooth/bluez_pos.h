
#ifndef _BLUEZ_DBUS_H_
#define _BLUEZ_DBUS_H_

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

#define BASE_SET_BT_PRE         "NewPosTech_"
#define BASE_SET_PAIR_PINCODE   "1234"

#define BLUEZ_MAN_PID_FILE      "/var/run/bluez-man.pid" 
#define BLUETOOTH_INI           "/etc/bluetooth/bluetooth.ini"
#define BTD_CTRL_SERVER_FILE    "/var/tmp/btd_ctrl_server"
#define BT_DAEMON_PID_FILE      "/var/run/bt_daemon.pid"
#define PDRIVERD_PID_FILE       "/var/run/pdriverd.pid"

#define PASSKEY_IPC_FILE        "/var/tmp/passkey_agent_ipc_file"
#define PASSKEY_AGENT_PID_FILE  "/var/run/passkey-agent.pid"

#define MAX_HCI_DEVICES_NUM     8
#define MAX_HCI_DEVICE_NAME     32
#define MAX_HCI_BONDED_NUM      128
#define MAX_BTADDR_LEN          32
#define MAX_DISC_NAME_LEN       64
#define MAX_DBUS_OBJ_PATH       256
#define MAX_INTERFACE_NAME      64
#define MAX_ADAPTER_MODE_LEN    32

typedef enum {
    QUIT_PINCODE_WINDOW,
    ENABLE_PAIRING,
    DISABLE_PAIRING,

    MAX_PASSKEY_AGENT_MSG
} PASSKEY_AGENT_MSG_T;

typedef enum {
    BLUEZ_MANAGE_REGISTER = 1,
    BASE_PAIR_CANCEL,       
    BASE_PAIR_CANCEL_OK,    /* D -> M */
    BASE_PAIR_CANCEL_FAIL,  /* D -> M */
    BASE_PAIR_CREATE,
    BASE_CONNECTING,        /* D -> M */
    BASE_CHECKING,          /* D -> M */
    BASE_PAIR_CREATE_OK,    /* D -> M */
    BASE_PAIR_CREATE_FAIL,  /* D -> M */

    CTRL_DISCONNECTED,
    PHONE_PAIR_CANCEL,
    PHONE_PAIR_CREATE,
    PHONE_PAIR_CREATE_OK,
    PHONE_GET_CONNECTE_INFO,
    PHONE_IS_CONNECTED,
    PHONE_IS_DISCONNECTED,

    MAX_BTD_CTRL_IPC_MSG
} BTD_CTRL_IPC_MSG_T;

typedef struct {
    int msg_type;
    char msg_data[0];
} ipc_msg_t;

typedef struct {
    ipc_msg_t head;
    int val;
} int_msg_t;

typedef struct {
    ipc_msg_t head;
    char btaddr[MAX_BTADDR_LEN];
} btaddr_msg_t;


typedef void (*bluez_pair_result_callback_t)(void *arg, int status, const char *btaddr);
typedef void (*bluez_network_connect_callback_t)(void *arg, int status, const char *btaddr, 
    const char *intf_name);

typedef enum {
    NO_ERROR = 0,
    UNSPEC_ERROR, /* may be un-special error */
    TRY_AGAIN,
    CREATE_PTHREAD_FAILED,
    DBUS_INIT_FAILED,
    IPC_INIT_FAILED,
    NO_MEMORY,
    PAIR_TIMEOUT,
    PAIR_FAILED,
    UNSUPPORT_EVENT_TYPE,
    ALREADY_EXSIT,
    IN_PROGRESS,
    NOT_FOUND,
    START_AGENT_FAILED,
    SET_MODE_FAILED,
    INVALID_ARG,
    CREATE_CONNECTION_FAILED,
    CORRUPT_CONNECTION_EAGAIN,
    
} BLUEZ_RETURN_T;


int bluez_dbus_init(int debug_mode);

int bluez_get_default_adapter(char hci[MAX_HCI_DEVICE_NAME]);
int bluez_list_adapter(char ret_device[MAX_HCI_DEVICES_NUM][MAX_HCI_DEVICE_NAME]);

int bluez_adapter_set_name(const char *adapter, const char *name);
int bluez_adapter_set_visible(const char *adapter, int is_visible);
int bluez_adapter_set_address(const char *adapter, const char *btaddr0); 

int bluez_adapter_get_name(const char *adapter, char *name_buf, int buf_len);
int bluez_adapter_get_address(const char *adapter, char *addr_buf, int buf_len);
int bluez_adapter_get_mode(const char *adapter, char mode_buf[MAX_ADAPTER_MODE_LEN]);
int bluez_adapter_get_remote_name(const char *adapter, const char *btaddr, char name[64]);

int bluez_adapter_is_visible(const char *adapter);
int bluez_adapter_is_baseset(const char *adapter0, const char *btaddr);


int bluez_adapter_cancel_wait_pairing(const char *adapter);
int bluez_adapter_wait_pairing(const char *adapter, const char *pincode, 
    bluez_pair_result_callback_t func, void *func_arg, unsigned int timeout_in_sec);


int bluez_adapter_pair_devices(const char *adapter, const char *btaddr,
    bluez_pair_result_callback_t func, void *func_arg);
int bluez_adapter_cancel_pair_devices(const char *adapter, const char *btaddr);
int bluez_adapter_remove_paired(const char *adapter, const char *btaddr);
int bluez_adapter_is_device_paired(const char *adapter, const char *btaddr);
/**
 * bluez_list_paired_device - 获取已配对的设备列表
 *
 * @adapter: 蓝牙名称，如hci0, hci1
 * @ret_bond_bta: 输出参数，存储绑定列表
 *
 * Return: 成功返回已配对的设备个数，如果无已配对设备，则返回0，出错返回负值
 *
 */
int bluez_adapter_list_paired(const char *adapter, 
    char ret_bond_bta[MAX_HCI_BONDED_NUM][MAX_BTADDR_LEN]);


int bluez_adapter_discover_devices(const char *adapter);
int bluez_adapter_cancel_discovery(const char *adapter);


int bluez_network_connect(const char *btaddr, const char *pan_type,
    bluez_network_connect_callback_t func, void *func_arg);
int bluez_network_cancel_connecting(const char *btaddr);
int bluez_network_disconnect(const char *btaddr);
int bluez_network_is_connected(const char *btaddr);
int bluez_adapter_is_remote_networking(const char *adapter, const char *btaddr);


/* bluez independent library, it does not need bluez_dbus_init before using them  */

int  bluez_adapter_set_address(const char *adapter, const char *btaddr0);
void bluez_generate_random_address(char btaddr[32]);

int  bluez_adapter_set_down(const char *adapter);
int  bluez_adapter_set_up(const char *adapter);

int bluez_serial_connect_uuid(const char *btaddr, const char *uuid, char serial_dev[256]);
int bluez_serial_connect_channel(const char *btaddr, int channel, char serial_dev[256]);
int bluez_serial_disconnect(const char *serial_dev);


/* bluez 库会对外发送如下事件，库使用者可以注册接收这些事件。相应的事件回调函数见下方 */
typedef enum {
    /* adapter event */
    PAIR_DEVICE_SUCCESS = 0,
    PAIR_DEVICE_REMOVED,
    REMOTE_DEVICE_FOUND,
    DISCOVERY_COMPLETED,
    REMOTE_DEVICE_DISCONNECTED,
    MODE_CHANGED,
    MAX_ADAPTER_EVENT, /* adapter event end */

    /* manager event begin */
    DEFAULT_ADAPTER_CHANGED,
    MAX_MANAGER_EVENT, /* manager event end */

    /* connection event begin */
    NETWORK_CONNECTED,
    NETWORK_DISCONNECTED,
    MAX_NETWORK_CONN_EVENT, /* connection event END */

    MAX_BLUEZ_EVENT
} BLUEZ_EVENT_T;

typedef void (*bluez_eve_hook_PAIR_DEVICE_SUCCESS)(void *arg, const char *btaddr);
typedef void (*bluez_eve_hook_PAIR_DEVICE_REMOVED)(void *arg, const char *btaddr);
typedef void (*bluez_eve_hook_DISCOVERY_COMPLETED)(void *arg);
typedef void (*bluez_eve_hook_REMOTE_DEVICE_FOUND)(void *arg, const char *btaddr, 
    unsigned int dev_class, int rssi);
typedef void (*bluez_eve_hook_REMOTE_DEVICE_DISCONNECTED)(void *arg, const char *btaddr);
typedef void (*bluez_eve_hook_MODE_CHANGED)(void *arg, const char *new_mode);
typedef void (*bluez_eve_hook_DEFAULT_ADAPTER_CHANGED)(void *arg, const char *new_def_adapter);

typedef void (*bluez_eve_hook_NETWORK_CONNECTED)(void *arg);
typedef void (*bluez_eve_hook_NETWORK_DISCONNECTED)(void *arg);

int bluez_register_adapter_event_hook(int event_type, void *event_handler, void *arg);
int bluez_unregister_adapter_event_hook(int event_type, void *event_handler);

#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* _BLUEZ_MAN_H_ */
