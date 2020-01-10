
#ifndef  _PARAM_H
#define  _PARAM_H

#include "global.h"

#define    FILE_BUFFER      20480

#define   EDC_NODE_NAME         "EDC"
#define   ACQ_NODE_NAME         "ACQ"
#define   ISSURE_NODE_NAME      "ISSURE"
#define   CARD_NODE_NAME        "CARD"

#define   MAX_NODE_LEN       10
#define   EDC_NODE_LEN       3
#define   ACQ_NODE_LEN       3
#define   ISSURE_NODE_LEN    6
#define   CARD_NODE_LEN      4


#define   ATTR_BCD     0x01    // 压缩的BCD码
#define   ATTR_CSTR    0x02    // 字符串
#define   ATTR_NLONG   0x04    // 数字字符串，各个数据必须是 0—9,转换成长整型数据 注意接受该数据的地址是int,long类型
#define   ATTR_BIT     0x08    // BIT位数据,需要后面的长度数据作为BIT转换
#define   ATTR_SBIT    0x10    // BIT位数据串，eg. "1111111111000111",
#define   ATTR_NSTR    0x20    // 数字串，不需要压缩直接存放，和ATTR_CSTR唯一区别在于去掉数值前面的非数字数据
#define   ATTR_CLONG   0x40    // 数字字符串，各个数据必须是 0—9, 转换成无符号的字节类型。接受该数据的地址是char 类型

typedef struct _tagST_PARAM_FILE
{
	void  *pszName;        //参数名称
	void  *pValue;         //对应参数值的保存地址
	int    iAttr;          //参数值的类型,属性
	int    iValueLen;      //参数最大长度,如果是BIT属性，该段可以设置该BIT的位置，例如0x0304:第三字节的BIT4
}ST_PARAM_FILE;


#define   EDC_NODE       0x0001
#define   ACQ_NODE       0x0002
#define   ISSURE_NODE    0x0003
#define   CARD_NODE      0x0004
#define   EMVAID_NODE    0x0005
#define   EMVCAPK_NODE   0x0006

typedef struct _tagST_NODE_INFO
{
	void  *pszNodeName;        //结点名称
	int    iNodeType;          //结点的类型,属性
}ST_NODE_INFO;


#define  ERR_NOT_DATE         -1   // 没有可分析,处理的数据
#define  ERR_FILE_FORMAT      -2   // 参数文件格式错误
#define  ERR_NOT_FIND         -3   // 找不到指定数据      
#define  ERR_NODE_TYPE        -4   // 节点类型错误

#define  ERR_ACQ_BASE         -0x0100
#define  ERR_ISSURE_BASE      -0x0200
#define  ERR_CARD_BASE        -0x0400
#define  ERR_EMV_BASE         -0x0800

#define  ERR_ACQ_ISSURE       -0x0101   // acq中没有ISSURE
#define  ERR_ACQ_NAME         -0x0102   // ACQ 没有NAME
#define  ERR_ACQ_TID          -0x0103
#define  ERR_ACQ_MID          -0x0104
#define  ERR_ACQ_COMM         -0x0105

#define  ERR_ISSURE_NAME      -0x0201
#define  ERR_ISSURE_CARD      -0x0202

#define  ERR_CARD_RANGE       -0x0401

#define  ERR_EMV_AIDLEN       -0x0801
#define  ERR_EMV_AID          -0x0802
#define  ERR_EMV_RID          -0x0803
#define  ERR_EMV_INDEX        -0x0804
#define  ERR_EMV_MODLEN       -0x0805
#define  ERR_EMV_EXPLEN       -0x0806
#define  ERR_EMV_EXPDATA      -0x0807
#define  ERR_EMV_SAVEAID      -0x0891
#define  ERR_EMV_SAVECAPK     -0x0892


//   虑掉数据左边的换行符号和空格
int  FilterHeadChars(char *psString,uint uiLen);

//   从字符串的起始位置开始找特定的字符，并将特定字符的偏移位返回
int  FindOneChar(char *psString, uint uiStrLen,char CharSet);
//  从字符串的最后一位开始找特定的字符
int  FindReverOneChar(char *psString, uint uiStrLen,char CharSet);

//  从数据缓冲区中分析出一行数据,将数据行结束的偏移地址返回
int  GetStringLine(char *psSource,uint uiLen);

//  在限定的缓冲区内找到第一个段标志，包括起始段标志或者段的结束标志。
int  FindFirstNode(char *psParam, uint uiParamLen,char **psNodeName,uint *uiNameLen);

// 从数据行分析得到变量名和变量值。如果能分析成功，则将该数据行的结束偏移值返回
int  GetVarNameAndVaule(char * psStrLine,uint uiStrLen,char **psVarName,uint *uiNameLen,char **psVarValue,uint *uiVallen);

int  UnPackParamFile(char *psParam, uint uiParamLen);

//  解包EDC参数
int  UnPackEdcParam(char *psEdcParam, uint uiLen);
int  CheckEdcParam(void);   // 检查EDC参数的完整性

//  解包ACQ参数，ChFlag用于表示是重新保存一个新的ACQ参数还是在上次没有解释完成的ACQ中继续解释
int  UnPackAcqParam(char *psAcqParam, uint uilen, char ChFlag);
int  CheckAcqParam(void);   // 检查ACQ参数的完整性

//  解包ISSURE参数
int  UnPackIssureParam(char *psIssureParam, uint uilen, char ChFlag);
int  CheckIssureParam(void);   // 检查ACQ参数的完整性

//  解包CARD参数
int  UnPackCardParam(char *psCardParam, uint uilen, char ChFlag);
int  CheckCardParam(void);   // 检查CARD参数的完整性  

// 解包EMV AID 参数
int  UnpackEmvAidParam(char *psEmvAidParam, uint uilen, char ChFlag);
int  CheckEmvAidParam(void);

// 解包EMV AID 参数
int  UnpackEmvCapkParam(char *psEmvCapkParam, uint uilen, char ChFlag);
int  CheckEmvCapkParam(void);

int  SetVarValue(ST_PARAM_FILE *pstNodeCfg,char*psVarName,uint uiNameLen,char*psValue,uint uiValueLen);

//   分析参数文件并保存到EDC的对应参数结构中
int  GetParamFile(void);

int  ExistParamFile(void);

void RemoveParamFile(void);

void DispUnpackErrMsg(int iErrCode);
void GetUnpackErrMsg(int iErrCode,char *psErrMsg);

int  GetParamfileName(void);
 
#endif


