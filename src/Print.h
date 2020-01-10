
/****************************************************************************
NAME
    print.h - 定义打印模块

DESCRIPTION

REFERENCE

MODIFICATION SHEET:
    MODIFIED   (YYYY.MM.DD)
    pyming     2008.09.12      - created
****************************************************************************/

#ifndef _PRINT_H
#define _PRINT_H

#include "posapi.h"
#include "New8210.h"

#define  PRN_NORMAL		    0
#define  PRN_REPRINT		1

#define  PRN_DATA_ERR	    3
#define  PRN_ERR			4
#define  PRN_NO_DOT		   0xFC
#define  PRN_DATA_BIG	   0xFE
#define  PRN_CANCEL		   0xA1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _tagPRNFONT_INFO
{
	INT8  szFontName[40];
	INT8  szFontCharacter[40];
	INT32 iStyle;
	INT32 iSize; 
	INT32 iPriority;
}PRNFONT_INFO;

void   InitSysAllPrnFont(void);
void   GetCurrentPrnFont(PRNFONT_INFO *PstCurPrnFont);
INT32  SetCurrentPrnFont(PRNFONT_INFO stCurPrnFont);

INT32  PrnCurAcqTransList(void);
void   PrnAllList(void);
void   ConvTransName(uint8_t *pszOrgName, uint8_t *pszOutName);
INT32  PrintReceipt(uint8_t ucPrnFlag);
INT32  PrnTotalIssuer(uint8_t ucIssuerKey);
INT32  PrintSettle(uint8_t ucPrnFlag);
void   PrintParam(void);
INT32  StartPrinter(void);
void   PrnEngTime(void);
void   PrnHead(void);
void   PrintfStrigMidOfLine(char *pszString);

extern PRNFONT_INFO  gSongGB2312CHn;
extern PRNFONT_INFO  gSongGB18030CHn;
extern PRNFONT_INFO  gCourierNewAsciiEn;
extern PRNFONT_INFO  gArialAsciiEn32;
extern PRNFONT_INFO  gArialAsciiEn24;
extern PRNFONT_INFO  gArialAsciiEn20;
extern PRNFONT_INFO  gSystemAsciiEn16;

#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif	// _PRINT_H

// end of file
