/****************************************************************************
NAME
	tdes.h - hash函数

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
*		  潘平彬		  2007-09-12		     创建			01-01-01
****************************************************************************/

#ifndef POS_TDES_H
#define POS_TDES_H

#include <stdint.h>

#ifndef  ERROR
#define  ERROR					0x01
#endif

//#ifndef TDEA_NONE
//#define TDEA_NONE                  0x02            // none
//#endif

//#ifndef TDEA_ENCRYPT
//#define TDEA_ENCRYPT            0x01            // TDEA encryption
//#endif

//#ifndef TDEA_DECRYPT
//#define TDEA_DECRYPT            0x00            // TDEA decryption
//#endif

void Des(const void *source, uint8_t *dest, const void *key, uint8_t flag);


/**************************************************************************
* 函数名称:
*		   int  TDEA(uchar *DataIn, uchar *DataOut, uchar *DesKey, uint KeyLen, uint Mode)
* 功能描述:
*		  实现Tripe DES 运算
* 输入参数:
*		  DataIn          待进行TDES运算的数据(8字节)
*              DesKey         DES密钥
*              KeyLen         密钥长度(8，16，24三种可能值)
*              Mode            加密或者解密运算
* 输出参数:
*		  DataOut        TDES运算后得到的数据(8字节)
* 返回值:
*		  0                  运算成功
*             0x01              输入参数错误(密钥长度错误或者操作模式错误)
* 历史记录:
*		   修改人		修改日期	修改内容		版本号
*		  潘平彬		  2007-09-12		     创建			01-01-01
****************************************************************************/
int TDEA(const void *DataIn, void *DataOut, const void *DesKey, uint32_t KeyLen, uint32_t Mode);

#endif
