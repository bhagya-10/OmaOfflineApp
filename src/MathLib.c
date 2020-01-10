
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdint.h>
#include "MathLib.h"
#include <ctype.h>

//��Դ����Ŀ�괮�������Ŀ�괮(�������������)
void PubXor(char *psSource1, char *psSource2, int iLength)
{
    int   iCnt;

    for(iCnt = 0; iCnt < iLength; iCnt++)
    {
        psSource1[iCnt] ^= psSource2[iCnt];
    }
}

//��������Դ���ֽ��˫�����ȿɶ���16���ƴ�,0x12AB-->"12AB"
void PubBcd2Asc(char *psIHex, int iHexLen, char *psOAsc)
{
    static const char szMapTable[17] = {"0123456789ABCDEF"};
    int   iCnt,index;
    unsigned char  ChTemp;

    for(iCnt = 0; iCnt < iHexLen; iCnt++)
    {
		ChTemp = (unsigned char)psIHex[iCnt];
		index = (ChTemp / 16) & 0x0F;
        psOAsc[2*iCnt]   = szMapTable[index];
		ChTemp = (unsigned char) psIHex[iCnt];
		index = ChTemp & 0x0F;
        psOAsc[2*iCnt + 1] = szMapTable[index];
    }
}

// ͬvOneTwo()����,����Ŀ�괮����һ '\0'
void PubBcd2Asc0(char *psIHex, int iHexLen, char *pszOAsc)
{
    PubBcd2Asc(psIHex, iHexLen, pszOAsc);
    pszOAsc[2*iHexLen] = 0;
}

//���ɶ���16���ƴ��ϲ�����һ�볤�ȵĶ����ƴ�,"12AB"-->0x12AB
void PubAsc2Bcd(char *psIAsc, int iAscLen, char *psOBcd)
{
    char   Chtmp,ChBcd;
    int    iCnt;

    for(iCnt = 0; iCnt < iAscLen; iCnt += 2)
    {
        Chtmp = psIAsc[iCnt];
		if( Chtmp > '9' )
        {
			Chtmp = (char)toupper((int)Chtmp) - 'A' + 0x0A;
        }
        else
        {
            Chtmp &= 0x0F;
        }
		ChBcd = (Chtmp << 4); // ��ȡBCD�ĸ�λ

        Chtmp = psIAsc[iCnt+1];
        if( Chtmp > '9' )
        {
            Chtmp = (char)toupper((int)Chtmp) - 'A' + 0x0A;
        }
		else
        {
            Chtmp &= 0x0F;
        }
		ChBcd |= Chtmp; // ��ȡBCD��λ

        psOBcd[iCnt/2] = ChBcd;
    }
}

//�����ִ�ת��ɳ�����,�൱��atol(),���Ǹú�����Ҫ�����ַ�����
unsigned long PubAsc2Long(char *psString, int iStrLen, ulong *pulOut)
{
    char    szBuff[15+1];
	unsigned long    ulTmp;

    sprintf(szBuff, "%.*s", (iStrLen <= 15 ? iStrLen : 15 ), psString);
	ulTmp =  (unsigned long)atol(szBuff);

    if (pulOut != NULL)
	{
		*pulOut = ulTmp;
	}
    return ulTmp;
}

//������ת���ɸ�λ��ǰ���ַ�����ʾ(���������ֽڸ�ʽ���)
void PubLong2Char(unsigned long ulSource, int iAsclen, char *psOAsc)
{
    int    iCnt;

    for(iCnt = 0; iCnt < iAsclen; iCnt++)
    {
        psOAsc[iCnt] = (char)(ulSource >> (8 * (iAsclen - iCnt - 1)));
    }
}

//��λ��ǰ���ַ�����ʾת��Ϊ������
unsigned long PubChar2Long(char *psString, int iStrLen, unsigned long *pulOut)
{
    int     iCnt;
	unsigned long   ulTmp,ulValue;

    ulTmp = 0L;
    for(iCnt = 0; iCnt < iStrLen; iCnt++)
    {
		ulValue = (unsigned long)psString[iCnt] & 0xFF;
        ulTmp |= ( ulValue << 8 * (iStrLen - iCnt - 1));
    }

	if (pulOut != NULL)
	{
		*pulOut = ulTmp;
	}
    return ulTmp;
}

//������ת��ΪBCD��ʽ�Ĵ����,1234 --> 0x12 0x34
void PubLong2Bcd(unsigned long ulSource, int iBcdLen, char *psOBcd)
{
    char    szFmt[30], szBuf[30];

    sprintf(szFmt, "%%0%dlu", iBcdLen*2);
    sprintf(szBuf, szFmt, ulSource);
    if( psOBcd != NULL )
    {
        PubAsc2Bcd(szBuf, iBcdLen * 2, psOBcd);
    }
}

//ת��BCD��Ϊ����
unsigned long PubBcd2Long(char *psBcd, int iBcdLen, unsigned long *pulOut)
{
    char   szBuf[30];
	unsigned long   ulTmp;

	memset(szBuf,0,sizeof(szBuf));
    PubBcd2Asc0(psBcd, iBcdLen, szBuf);
	ulTmp = (unsigned long)atol((char *)szBuf);
    if( pulOut != NULL )
    {
        *pulOut = ulTmp;
    }

    return ulTmp;
}

//���ַ���ת��Ϊ��д
void PubStrUpper(char *pszString)
{
    while( *pszString )
    {
        *pszString = toupper((char)*pszString);
        pszString++;
    }
}

//���ַ���ת��ΪСд
void PubStrLower(char *pszString)
{
    while( *pszString )
    {
        *pszString = (char)tolower((int)*pszString);
        pszString++;
    }
}

//ɾ���ַ����еĿհ��ַ�(����,�ո�/�س�/����/TAB�ַ�)
void PubTrimStr(char *pszString)
{
#define ISSPACE(ch) ( ((ch) == ' ')  || ((ch) == '\t') || \
                      ((ch) == '\n') || ((ch) == '\r') )

    char *p, *q;

    if( !pszString || !*pszString )
    {
        return;
    }

    p = &pszString[strlen((char*)pszString) - 1];
    while(( p > pszString) && ISSPACE(*p) )
    {
        *p-- = 0;
    }
    if( (p == pszString) && ISSPACE(*p) )  *p = 0;

    for(p = pszString; *p && ISSPACE(*p); p++);
    if( p != pszString )
    {
        q = pszString;
        while( *p )   *q++ = *p++;
        *q = 0;
    }
#undef ISSPACE
}


//ɾ���ַ������˵��ض��ַ�
void PubTrimSpcStr(char *pszString, char ChSpcChar)
{
	char *p, *q;

	if( !pszString || !*pszString )
	{
		return;
	}

	p = &pszString[strlen((char*)pszString) - 1];
	while( (p > pszString) && ((*p) == ChSpcChar))
	{
		*p-- = 0;
	}
	if( p == pszString && ((*p) == ChSpcChar) )  *p = 0;

	for(p = pszString; *p && ((*p) == ChSpcChar); p++);
	if( p != pszString )
	{
		q = pszString;
		while( *p )   *q++ = *p++;
		*q = 0;
	}
}


//ɾ���ַ����ұ��������ض��ַ�
void PubTrimTailChars(char *pszString, char ChRemoveChar)
{
	int		i, iLen;

	if( !pszString || !*pszString )
    {
        return;
    }

	iLen = strlen((char *)pszString);
	for(i=iLen-1; i>=0; i--)
	{
		if( pszString[i]!=ChRemoveChar )
		{
			break;
		}
		pszString[i] = 0;
	}
}

// ɾ��ǰ���ַ�
void PubTrimHeadChars(char *pszString, char ChRemoveChar)
{
	char	*p;

	if( !pszString || !*pszString )
    {
        return;
    }

	for(p=pszString; *p && *p==ChRemoveChar; p++);
	if( p!=pszString )
	{
		while( (*pszString++ = *p++) );
	}
}

// ���ǰ���ַ�,�Ա�ﵽuiTargetLen����
void PubAddHeadChars( char *pszString, int iTargetLen, char ChAddChar )
{
	int	iLen;

	iLen = strlen((char *)pszString);
	if( iLen>=iTargetLen )
	{
		return;
	}

	memmove(pszString+iTargetLen-iLen, pszString, iLen+1);
	memset(pszString, ChAddChar, iTargetLen-iLen);
}

// �����ִ�Сд���ַ����ȽϺ���
//Compare two strings without regard to capital or lowercase
int PubStrNoCaseCmp(char *pszStr1, char *pszStr2)
{
    if( !pszStr1 || !pszStr2 )
    {
        return -1;
    }

    while( *pszStr1 && *pszStr2 )
    {
        if( toupper((char)*pszStr1) != toupper((char)*pszStr2) )
        {
            return (toupper((char)*pszStr1) - toupper((char)*pszStr2));
        }
        pszStr1++;
        pszStr2++;
    }
    if( !*pszStr1 && !*pszStr2 )
    {
        return 0;
    }
    if( !*pszStr1 )
    {
        return -1;
    }

    return 1;
}

//��ת�ַ���
void PubStrReverse(char *pszString)
{
    int     i, j, iLength;
    char    ucTmp;

    iLength = strlen((char*)pszString);
    for(i = 0,j = iLength - 1; i < iLength / 2; i++,j--)
    {
        ucTmp        = pszString[i];
        pszString[i] = pszString[j];
        pszString[j] = ucTmp;
    }
}


//ȡ�ֽڵĸ���λ
char PubHigh4Bit(char ChInChar)
{
	return ((ChInChar/16) & 0x0F);
}


//ȡ�ֽڵĵ���λ
//get the low 4 bit of the byte
char PubLow4Bit(char ChInChar)
{
	return (ChInChar & 0x0F);
}


//����ASC�ַ������,�������pszProduct��
void PubAscMul(char *pszFaciend, char *pszMultiplier, char *pszProduct)
{
	char	*p, ucTemp, ucCarryBit, szBuff[100+1];
	int	    iFaciLen, iMulLen, iProdPos, iCnt;

	iFaciLen = strlen((char *)pszFaciend);
	iMulLen  = strlen((char *)pszMultiplier);

	sprintf((char *)szBuff, "%0*ld", iFaciLen+iMulLen, 0L);

	for(iProdPos=0; iFaciLen>0; iFaciLen--,iProdPos++)
	{
		ucCarryBit = 0;
		p = &szBuff[iProdPos];

		for(iCnt=iMulLen; iCnt>0; iCnt--)
		{
			ucTemp = (pszFaciend[iFaciLen-1] & 0x0F) * (pszMultiplier[iCnt-1] & 0x0F) +
					 ucCarryBit + (*p & 0x0F);
			*p++   = (ucTemp % 10) + '0';
			ucCarryBit = ucTemp / 10;
		}
		if( ucCarryBit!=0 )
		{
			*p++ = ucCarryBit + '0';
		}
	}
	PubTrimTailChars(szBuff, '0');
	PubStrReverse(szBuff);

	if( szBuff[0]==0 )
	{
		sprintf((char *)szBuff, "0");
	}
	if( pszProduct!=NULL )
	{
		sprintf((char *)pszProduct, "%s", szBuff);
	}
}


//������ȵ�����ASC�ַ������
void PubAscAdd(char *psAddend1, char *psAddend2, int iLen, char *pszSum)
{
	char	*pszResult, ChCarryBit, ChTemp, szBuff[100];

	ChCarryBit = 0;
	pszResult  = szBuff;
	while( iLen>0 )
	{
		ChTemp = (psAddend1[iLen-1] & 0x0F) + (psAddend2[iLen-1] & 0x0F) + ChCarryBit;
		*pszResult++ = (ChTemp % 10) + '0';
		ChCarryBit   = (ChTemp>9) ? 1 : 0;
		iLen--;
	}
	if( ChCarryBit!=0 )
	{
		*pszResult++ = '1';
	}
	*pszResult = 0;

	PubStrReverse(szBuff);
	if( pszSum!=NULL )
	{
		sprintf((char *)pszSum, "%s", szBuff);
	}
}


//������ȵ�����ASC�ַ������(psMinuend>psSubtrahend)
void PubAscSub(char *psMinuend, char *psSubtrahend, int iLen, char *pszResult)
{
	char	*pszOut, ChCarryBit, ChTemp, szBuff[100];

	pszOut     = szBuff;
	ChCarryBit = 0;
	while( iLen>0 )
	{
		ChTemp = (psMinuend[iLen-1] & 0x0F) - (psSubtrahend[iLen-1] & 0x0F) - ChCarryBit + 10;
		*pszOut++  = (ChTemp % 10) + '0';
		ChCarryBit = (psMinuend[iLen-1]<psSubtrahend[iLen-1]+ChCarryBit) ? 1 : 0;
		iLen--;
	}
	*pszOut = 0;

	PubStrReverse(szBuff);
	if( pszResult!=NULL )
	{
		sprintf((char *)pszResult, "%s", szBuff);
	}
}

//ASC�ַ�����1,�������ԭ����,�з���ֵ
char PubAscDec(char *psAscStr, int iStrLen)
{
	psAscStr += iStrLen - 1;
	while(--(*psAscStr) < '0')
	{
		*psAscStr-- = '9';
		if(--iStrLen == 0)
			return 1;
	}

	return 0;
}


//ASC�ַ�����1,�������ԭ����,�з���ֵ
char PubAscInc(char *psAscStr, int iStrLen)
{
	psAscStr += iStrLen - 1;
	while(++(*psAscStr) > '9')
	{
		*psAscStr-- = '0';
		if(--iStrLen == 0) 
			return 1;
	}

	return 0;
}


//������ȵ�����BCD����ӣ��������psBcdStr1��
void PubBcdAdd(char *psAddend1, char *psAddend2, int iLen, char *psResult)
{
	char	sAdd1[100+1], sAdd2[100];

	PubBcd2Asc(psAddend1, iLen, sAdd1);
	PubBcd2Asc(psAddend2, iLen, sAdd2);
	PubAscAdd(sAdd1, sAdd2, iLen*2, sAdd1);
	PubAsc2Bcd(sAdd1, iLen*2, psResult);
}


//������ȵ�����BCD��������������psAddend1��
void PubBcdSub(char *psMinuend, char *psSubtrahend, int iLen, char *psResult)
{
	char	sMinuend[100+1], sSubtrahend[100];
	
	PubBcd2Asc(psMinuend, iLen, sMinuend);
	PubBcd2Asc(psSubtrahend, iLen, sSubtrahend);
	PubAscSub(sMinuend, sSubtrahend, iLen*2, sMinuend);
	PubAsc2Bcd(sMinuend, iLen*2, psResult);
}


//BCD�ַ�����1,�������ԭ����,�з���ֵ
char PubBcdDec(char *psBcdStr, int iStrLen)
{
	psBcdStr += iStrLen - 1;
	while((char)--(*psBcdStr) < 0)
	{
		*psBcdStr = (char)0x99;
		psBcdStr--;
		if(--iStrLen == 0)
			return 1;
	}

	if(((*psBcdStr) & 0x0f) > 9)
		*psBcdStr -= 0x06;

	return 0;
}


//BCD�ַ�����1,�������ԭ����,�з���ֵ
char PubBcdInc(unsigned char *psBcdStr, int iStrLen)
{
	psBcdStr += iStrLen - 1;
	while(++(*psBcdStr) > 0x99)
	{
		*psBcdStr = 0;
		psBcdStr--;
		if((--iStrLen) == 0)
			return 1;
	}

	if(((*psBcdStr) & 0x0f) > 9)
		*psBcdStr += 0x06;

	return 0;
}

// ����LRC
char PubCalcLRC(char *psData, unsigned long ulLength, char ChInit)
{
	while( ulLength>0 )
	{
		ChInit ^= *psData++;
		ulLength--;
	}
	
	return ChInit;
}

//  �ж��ַ����������Ƿ����ض����ַ�
char PubIsSpecStr(char *psStr, int iLen, char Char)
{
	int		iCnt;
	
	for(iCnt=0; iCnt<iLen; iCnt++)
	{
		if( psStr[iCnt]!=Char )
		{
			return FALSE;
		}
	}
	
	return TRUE;
}


unsigned short   PubGetCRC(char *psData, unsigned long ulLength)
{
	int  crc_return;                                                                    
	unsigned char CRC16Lo=INI_VECTORLo;                                                            
	unsigned char CRC16Hi=INI_VECTORHi;                                                           
	unsigned char SaveHi,SaveLo;                                                                  
	unsigned int i, j;                                                                            
	
	for (i=0; i<ulLength; i++)
	{                                                              
		CRC16Lo = CRC16Lo ^ *psData++;
		
		for (j=0; j<8; j++)
		{                                                                          
			SaveHi = CRC16Hi;                                                                           
			SaveLo = CRC16Lo;                                                                           
			CRC16Hi = CRC16Hi >>1;                                                                      
			CRC16Lo = CRC16Lo >>1;       
			
			if ((SaveHi & 0x1) == 0x1)  
			{
				CRC16Lo = CRC16Lo | 0x80;       
			}
			
			if ((SaveLo & 0x1) == 0x1)
			{                                                                 
				CRC16Hi = CRC16Hi ^ POLYNOMIALHi;                                                         
				CRC16Lo = CRC16Lo ^ POLYNOMIALLo;                                                         
			}                                                                                           
		}                                                                                             
	}    
	
	crc_return=CRC16Hi*256 + CRC16Lo;                                                             
	return (crc_return);
}


//����ȫ�����ֵ��ַ���ASCIIת����BCD��ֻȡ������ַ�
void PubNoAsc2Bcd(char *psIAsc, int iAscLen, char *psOBcd)
{
    unsigned char   Chtmp,ChBcd;
    int    iCnt;
	
    for(iCnt = 0; iCnt < iAscLen; iCnt += 2)
    {
        Chtmp = psIAsc[iCnt];
        Chtmp &= 0x0F;
		ChBcd = (Chtmp << 4); // ��ȡBCD�ĸ�λ
		
        Chtmp = psIAsc[iCnt+1];
        Chtmp &= 0x0F;
		ChBcd |= Chtmp; // ��ȡBCD��λ
		
        psOBcd[iCnt/2] = ChBcd;
    }
}


// ����CRC
unsigned short PubCalcCRC(unsigned char *psData, unsigned long ulLength, unsigned short usInit)
{
    unsigned char  ucTemp;
	static unsigned short g_awEpphalfCrc16CCITT[16]={ /* CRC ���ֽ���ʽ�� */
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
		0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
	};
	
    while (ulLength-- != 0)
    {
        ucTemp = ((unsigned char)(usInit>>8))>>4;
        usInit <<= 4;
        usInit ^= g_awEpphalfCrc16CCITT[ucTemp^(*psData/16)];
        ucTemp = ((unsigned char)(usInit>>8))>>4;
        usInit <<= 4;
        usInit ^= g_awEpphalfCrc16CCITT[ucTemp^(*psData&0x0f)];
        psData++;
    }
	
	return usInit;
}

// end of file

