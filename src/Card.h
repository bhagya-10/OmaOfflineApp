
/****************************************************************************
NAME
    Card.h - ����������ſ���оƬ����صĴ�����

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.04.14      - created
****************************************************************************/

#ifndef _CARD_H
#define _CARD_H

#define ICC_USER		0
// ˢ��/�忨�¼�
#define NO_SWIPE_INSERT		0x00	// û��ˢ��/�忨
#define CARD_SWIPED			0x01	// ˢ��
#define CARD_INSERTED		0x02	// �忨
#define CARD_KEYIN			0x04	// ���俨��
#define SKIP_DETECT_ICC		0x20	// �����ICC�忨�¼� // ��ֹ�û��ο�����
#define FALLBACK_SWIPE		0x40	// ˢ��(FALLBACK)
#define SKIP_CHECK_ICC		0x80	// �����service code

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
