
/****************************************************************************
NAME
    password.h - 定义密码输入、检查模块

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.09.12      - created
****************************************************************************/

#ifndef _PASSWORD_H
#define _PASSWORD_H

#define  ERR_RS232_OK              0   // 成功
#define  ERR_RS232_FAILURE        -1   // 串口操作失败
#define  ERR_RS232_RX_DATA        -2   // 接收数据错误
#define  ERR_RS232_RX_TIMEOUT     -3   // 接收数据超时
#define  ERR_RS232_RX_LENGTH      -4   // 接收数据长度超限
#define  ERR_RS232_RX_CHECKSUM    -5   // 校验数据出错

#define CMD_DOWNLOAD_KEY          0x0306

// 密码类型
enum {BANK_PWD,TERM_PWD,BANK_PWD2, VOID_PWD, REFUND_PWD, ADJUST_PWD, SETTLE_PWD, MAX_PWD};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int  CheckBankPWD(void);
int  CheckTermPWD(void);
int  CheckBankPWD2(void);
int  CheckTransVoidPWD(void);
int  CheckTransRefundPWD(void);
int  CheckTransAdjustPWD(void);
int  CheckTransSettlePWD(void);

void ModifyBankPWD(void);
void ModifyTermPWD(void);
void ModifyTransVoidPWD(void);
void ModifyTransRefundPWD(void);
void ModifyTransAdjustPWD(void);
void ModifyTransSettlePWD(void);

void  DownMasterKey(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _PASSWORD_H

// end of file
