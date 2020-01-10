
/****************************************************************************
NAME
	Mathlib.h - 算术运算公共函数

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	pyming		2008.03.14		- add precompile process
****************************************************************************/

#ifndef _MATHLIB_H
#define _MATHLIB_H

#ifndef TRUE
	#define TRUE	1
	#define FALSE	0
#endif

#ifndef ulong
#define ulong	unsigned long
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************
 功    能:      计算结构成员的偏移
 输入参数:
    type        结构体类型
    member      结构体成员名称
 输出参数:
    none
 返 回 值:      该结构体成员的偏移量
****************************************************************************/
#define OFFSET(type, member)    ( (unsigned long)(&((type *)0)->member) )


/****************************************************************************
 功    能:      计算两个整数数字/表达式最大(小)值
 输入参数:
    a,b         待比较的整数/表达式
 输出参数:
    none
 返 回 值:      最大(小)值
****************************************************************************/

#ifndef  MAX
#define MAX(a, b)       ( (a)>=(b) ? (a) : (b) )
#endif

#ifndef  MAX
#define MIN(a, b)       ( (a)<=(b) ? (a) : (b) )
#endif


/****************************************************************************
 功    能:      计算整数数字/表达式绝对值
 输入参数:
    a           整数数字/表达式
 输出参数:
    none
 返 回 值:      绝对值
****************************************************************************/
#ifndef  ABS
#define  ABS(a)      ( (a)>=0 ? (a) : (-1)*(a) )
#endif

/****************************************************************************
 功    能:      将源串与目标串异或后存入目标串(两个串长度相等)
 输入参数:
    psVect1     目标串
    psVect2     源串
    nLength     串长度
 输出参数:
    psVect1     异或后的目标串
 返 回 值:      无
****************************************************************************/
void PubXor(char *psSource1, char *psSource2, int iLength);


/****************************************************************************
 功    能:      将二进制源串分解成双倍长度可读的16进制串,0x12AB-->"12AB"
 输入参数:
    psIn        源串
    iHexLen     源串长度
 输出参数:
    psOut       目标串
 返 回 值:      无
****************************************************************************/
void PubBcd2Asc(char *psIHex, int iHexLen, char *psOAsc);


/****************************************************************************
 功    能:      同vOneTwo()函数,并在目标串后添一 '\0'
 输入参数:
    psIn        源串
    nLength     源串长度
 输出参数:
    psOut       目标串
 返 回 值:      无
****************************************************************************/
void PubBcd2Asc0(char *psIHex, int iLength, char *pszOAsc);


/****************************************************************************
 功    能:      将可读的16进制串合并成其一半长度的二进制串,"12AB"-->0x12AB
 输入参数:
    psIn        源串
    nLength     源串长度
 输出参数:
    psOut       目标串
 返 回 值:      无
****************************************************************************/
void PubAsc2Bcd(char *psIAsc, int iAscLen, char *psOBcd);


/****************************************************************************
 功    能:      将数字串转变成长整数,相当于atol(),但是该函数不要求零字符结束
 输入参数:
    sString     数字串,必须为合法的数字,不需要'\0'结尾
    nLength     数字串长度
 输出参数:
    pulOut:     转变后的长整数
 返 回 值:      转变后的长整数
****************************************************************************/
ulong PubAsc2Long(char *psString, int iStrLen, unsigned long *pulOut);


/****************************************************************************
 功    能:      整型数转换成高位在前的字符串表示(按照网络字节格式存放)
 输入参数:
    ulSource    待转换的整数
    uiTCnt      目标缓冲区的大小(字节数)
 输出参数:
    psTarget    转换后的串
 返 回 值:      无
****************************************************************************/
void PubLong2Char(unsigned long ulSource, int iAsclen, char *psOAsc);


/****************************************************************************
 功    能:      高位在前的字符串表示转换为整型数
 输入参数:
    psSource    待转换的串
    uiSCnt      源串的长度
 输出参数:
    *pulTarget  转换后整数
 返 回 值:      转换后整数
****************************************************************************/
unsigned long PubChar2Long(char *psString, int iStrLen, unsigned long *pulOut);


/****************************************************************************
功    能:       将整数转换为BCD格式的串存放,1234 --> 0x12 0x34
输入参数:
    ulSource    待转换的整数
    uiTCnt      目标缓冲区的大小(字节数)
输出参数:
    psTarget    目标缓冲区
返 回 值:       无
****************************************************************************/
void PubLong2Bcd(unsigned long ulSource, int iBcdLen, char *psOBcd);


/****************************************************************************
功    能:       转换BCD串为整数
输入参数:
    psSource    待转换的BCD数据格式的串
    uiSCnt      源串的长度
输出参数:
    *pulTarget  转换后整数
返 回 值:       转换后整数
****************************************************************************/
unsigned long PubBcd2Long(char *psBcd, int iBcdLen, unsigned long *pulOut);


/****************************************************************************
功    能:       将字符串转换为大写
输入参数:
   pszString    待转换的字符串,以'\0'结束
输出参数:
   pszString    转换后的字符串,以'\0'结束
返 回 值:       无
****************************************************************************/
void PubStrUpper(char *pszString);


/****************************************************************************
功    能:       将字符串转换为小写
输入参数:
   pszString    待转换的字符串,以'\0'结束
输出参数:
   pszString    转换后的字符串,以'\0'结束
返 回 值:       无
****************************************************************************/
void PubStrLower(char *pszString);


/****************************************************************************
 功    能:      删除字符串中的空白字符(包括,空格/回车/换行/TAB字符)
 输入参数:
    pszString   待处理的字符串,以'\0'结束
 输出参数:
    pszString   处理后的字符串,以'\0'结束
 返 回 值:   none
****************************************************************************/
void PubTrimStr(char *pszString);


/****************************************************************************
 功    能:      删除字符串中的特定字符
 输入参数:
    pszString   待处理的字符串,以'\0'结束
    ucSpcChar   特定字符
 输出参数:
    pszString   处理后的字符串,以'\0'结束
 返 回 值:      无
****************************************************************************/
void PubTrimSpcStr(char *pszString, char ucSpcChar);


/****************************************************************************
 功    能:        删除字符串右边连续的特定字符
 输入参数:
    pszString     待处理的字符串,以'\0'结束
    ucRemoveChar  待删除字符
 输出参数:
    pszString     处理后的字符串,以'\0'结束
 返 回 值:        无
****************************************************************************/
void PubTrimTailChars(char *pszString, char ucRemoveChar);


/****************************************************************************
 功    能:        删除字符串左边连续的特定字符
 输入参数:
    pszString     待处理的字符串,以'\0'结束
    ucRemoveChar  待删除字符
 输出参数:
    pszString     处理后的字符串,以'\0'结束
 返 回 值:        无
****************************************************************************/
void PubTrimHeadChars(char *pszString, char ucRemoveChar);


/****************************************************************************
功    能:       不区分大小写的字符串比较函数
输入参数:
    pszStr1     待比较的字符串1
    pszStr2     待比较的字符串2
输出参数:       无
返 回 值:
    0               ok
    <0              pszStr1 < pszStr2
    >0              pszStr1 > pszStr2
****************************************************************************/
int PubStrNoCaseCmp(char *pszStr1, char *pszStr2);


/****************************************************************************
功    能:       填充前导字符,以便达到uiTargetLen长度
输入参数:
    pszString   待填充的前导字符
	uiTargetLen 填充后达到的长度
	ucAddChar   填充的字符
输出参数:
    pszString   反转后的字符串,以'\0'结束
返 回 值:       无
****************************************************************************/
void PubAddHeadChars(char *pszString, int uiTargetLen, char ChAddChar);


/****************************************************************************
功    能:       反转字符串
输入参数:
    pszString   待反转的字符串,以'\0'结束
输出参数:
    pszString   反转后的字符串,以'\0'结束
返 回 值:       无
****************************************************************************/
void PubStrReverse(char *pszString);


/****************************************************************************
功    能:       取字节的高四位
输入参数:
    ucInChar    待取高四位的字节
输出参数:
返 回 值:       高四位的字节
****************************************************************************/
char PubHigh4Bit(char ChInChar);


/****************************************************************************
功    能:       取字节的低四位
输入参数:
    ucInChar    待取低四位的字节
输出参数:
返 回 值:       低四位的字节
****************************************************************************/
char PubLow4Bit(char ChInChar);


/****************************************************************************
功    能:       两个ASC字符串相乘
输入参数:
    psStr1      被乘数ASC字符串
    ucStr1Len   psStr1的长度
    psStr2      乘数ASC字符串
    ucStr2Len   psStr2的长度
输出参数:
    psStr1      相乘的结果,长度为ucStr1Len+ucStr2Len
返 回 值:       无
****************************************************************************/
void PubAscMul(char *pszFaciend, char *pszMultiplier, char *pszProduct);


/****************************************************************************
功    能:       长度相等的两个ASC字符串相加，前补0
输入参数:
    psAddend1   被加数ASC字符串
    psAddend2   加数ASC字符串
    uiLen       字符串的长度
输出参数:
    pszSum      相加的结果
返 回 值:       无
****************************************************************************/
void PubAscAdd(char *psAddend1, char *psAddend2, int iLen, char *pszSum);
#define SafeAscAdd(a,b,c)	PubAscAdd(a,b,c,a)


/****************************************************************************
功    能:       长度相等的两个ASC字符串相减，前补0
输入参数:
    psMinuend    被减数ASC字符串
    psSubtrahend 减数ASC字符串
    uiLen        字符串的长度
输出参数:
    pszResult   结果
返 回 值:       无
****************************************************************************/
void PubAscSub(char *psMinuend, char *psSubtrahend, int iLen, char *pszResult);
#define SafeAscSub(a,b,c)	PubAscSub(a,b,c,a)


/****************************************************************************
功    能:       ASC字符串减1,结果放在原串里
输入参数:
    psAscStr    需减1的ASC字符串
    nStrLen     psAscStr的长度
输出参数:
    psAscStr    减1后的ASC字符串
返 回 值:       0  成功
                1  失败
****************************************************************************/
char PubAscDec(char *psAscStr, int iStrLen);


/****************************************************************************
功    能:       ASC字符串加1,结果放在原串里
输入参数:
    psAscStr    需加1的ASC字符串
    nStrLen     psAscStr的长度
输出参数:
    psAscStr    加1后的ASC字符串
返 回 值:       0  成功
                1  失败
****************************************************************************/
char PubAscInc(char *psAscStr, int iStrLen);


/****************************************************************************
功    能:       长度相等的两个BCD字符串相加，前补0
输入参数:
    psAddend1   被加数BCD字符串
    psAddend2   加数BCD字符串
    uiLen       字符串的长度
输出参数:
    psResult     相加的结果
返 回 值:       无
****************************************************************************/
void PubBcdAdd(char *psAddend1, char *psAddend2, int iLen, char *psResult);
#define SafeBcdAdd(a,b,c)		PubBcdAdd(a, b, c, a)


/****************************************************************************
功    能:       长度相等的两个BCD字符串相减，前补0
输入参数:
    psMinuend      被减数BCD字符串
    psSubtrahend   减数BCD字符串
    uiLen          字符串的长度
输出参数:
    psResult      相减的结果
返 回 值:       无
****************************************************************************/
void PubBcdSub(char *psMinuend, char *psSubtrahend, int iLen, char *psResult);
#define SafeBcdSub(a,b,c)		PubBcdSub(a, b, c, a)


/****************************************************************************
功    能:       BCD字符串减1,结果放在原串里
输入参数:
    psBcdStr    需减1的BCD字符串
    nStrLen     psBcdStr的长度
输出参数:
    psBcdStr    减1后的BCD字符串
返 回 值:         0  成功
                1  失败
****************************************************************************/
char PubBcdDec(char *psBcdStr, int iStrLen);


/****************************************************************************
功    能:       BCD字符串加1,结果放在原串里
输入参数:
    psBcdStr    需加1的BCD字符串
    nStrLen     psBcdStr的长度
输出参数:
    psBcdStr    加1后的BCD字符串
返 回 值:       0  成功
                1  失败
****************************************************************************/
char PubBcdInc(unsigned char *psBcdStr, int iStrLen);


// 计算LRC
char PubCalcLRC(char *psData, unsigned long ulLength, char ChInit);


//  判断字符串中数字是否都是特定的字符
char PubIsSpecStr(char *psStr, int iLen, char Char);

//将非全部数字的字符的ASCII转换成BCD，只取后面的字符
void PubNoAsc2Bcd(char *psIAsc, int iAscLen, char *psOBcd);

#define INI_VECTORLo 0x00
#define INI_VECTORHi 0x00
#define POLYNOMIALHi 0x80
#define POLYNOMIALLo 0x05
// 计算CRC
unsigned short   PubGetCRC(char *psData, unsigned long ulLength);

// 计算CRC
unsigned short PubCalcCRC(unsigned char *psData, unsigned long ulLength, unsigned short usInit);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _UTIL_H */
