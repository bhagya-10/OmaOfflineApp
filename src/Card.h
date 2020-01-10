
/****************************************************************************
NAME
    Card.h - 定义所有与磁卡，芯片卡相关的处理函数

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.14      - created
****************************************************************************/

#ifndef _CARD_H
#define _CARD_H

#define ICC_USER		0
// 刷卡/插卡事件
#define NO_SWIPE_INSERT		0x00	// 没有刷卡/插卡
#define CARD_SWIPED			0x01	// 刷卡
#define CARD_INSERTED		0x02	// 插卡
#define CARD_KEYIN			0x04	// 手输卡号
#define SKIP_DETECT_ICC		0x20	// 不检查ICC插卡事件 // 防止用户拔卡过快
#define FALLBACK_SWIPE		0x40	// 刷卡(FALLBACK)
#define SKIP_CHECK_ICC		0x80	// 不检查service code

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

INT32  SwipeCardProc(unsigned char bCheckICC);
INT32  InsertCardProc(void);
INT32  GetCardInfo(UINT32 uiMode);
INT32  ManualInputPan(uint8_t ucInitChar);
INT32  ReadMagCardInfo(void);
INT32  IsChipCard(uint8_t *pszTrack2);
INT32  ValidCard(void);
INT32  ValidPanNo(uint8_t *pszPanNo);
INT32  ValidCardExpiry(void);
void   GetCardHolderName(char *pszHolderName);
INT32  ConfirmPanInfo(void);
void   MaskPan(uint8_t *pszInPan, uint8_t *pszOutPan);
INT32  ExtractPAN(uint8_t *pszPAN, uint8_t *pszOutPan);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// _UITL_H

// end of file
