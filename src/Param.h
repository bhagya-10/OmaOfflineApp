
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


#define   ATTR_BCD     0x01    // ѹ����BCD��
#define   ATTR_CSTR    0x02    // �ַ���
#define   ATTR_NLONG   0x04    // �����ַ������������ݱ����� 0��9,ת���ɳ��������� ע����ܸ����ݵĵ�ַ��int,long����
#define   ATTR_BIT     0x08    // BITλ����,��Ҫ����ĳ���������ΪBITת��
#define   ATTR_SBIT    0x10    // BITλ���ݴ���eg. "1111111111000111",
#define   ATTR_NSTR    0x20    // ���ִ�������Ҫѹ��ֱ�Ӵ�ţ���ATTR_CSTRΨһ��������ȥ����ֵǰ��ķ���������
#define   ATTR_CLONG   0x40    // �����ַ������������ݱ����� 0��9, ת�����޷��ŵ��ֽ����͡����ܸ����ݵĵ�ַ��char ����

typedef struct _tagST_PARAM_FILE
{
	void  *pszName;        //��������
	void  *pValue;         //��Ӧ����ֵ�ı����ַ
	int    iAttr;          //����ֵ������,����
	int    iValueLen;      //������󳤶�,�����BIT���ԣ��öο������ø�BIT��λ�ã�����0x0304:�����ֽڵ�BIT4
}ST_PARAM_FILE;


#define   EDC_NODE       0x0001
#define   ACQ_NODE       0x0002
#define   ISSURE_NODE    0x0003
#define   CARD_NODE      0x0004
#define   EMVAID_NODE    0x0005
#define   EMVCAPK_NODE   0x0006

typedef struct _tagST_NODE_INFO
{
	void  *pszNodeName;        //�������
	int    iNodeType;          //��������,����
}ST_NODE_INFO;


#define  ERR_NOT_DATE         -1   // û�пɷ���,���������
#define  ERR_FILE_FORMAT      -2   // �����ļ���ʽ����
#define  ERR_NOT_FIND         -3   // �Ҳ���ָ������      
#define  ERR_NODE_TYPE        -4   // �ڵ����ʹ���

#define  ERR_ACQ_BASE         -0x0100
#define  ERR_ISSURE_BASE      -0x0200
#define  ERR_CARD_BASE        -0x0400
#define  ERR_EMV_BASE         -0x0800

#define  ERR_ACQ_ISSURE       -0x0101   // acq��û��ISSURE
#define  ERR_ACQ_NAME         -0x0102   // ACQ û��NAME
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


//   �ǵ�������ߵĻ��з��źͿո�
int  FilterHeadChars(char *psString,uint uiLen);

//   ���ַ�������ʼλ�ÿ�ʼ���ض����ַ��������ض��ַ���ƫ��λ����
int  FindOneChar(char *psString, uint uiStrLen,char CharSet);
//  ���ַ��������һλ��ʼ���ض����ַ�
int  FindReverOneChar(char *psString, uint uiStrLen,char CharSet);

//  �����ݻ������з�����һ������,�������н�����ƫ�Ƶ�ַ����
int  GetStringLine(char *psSource,uint uiLen);

//  ���޶��Ļ��������ҵ���һ���α�־��������ʼ�α�־���߶εĽ�����־��
int  FindFirstNode(char *psParam, uint uiParamLen,char **psNodeName,uint *uiNameLen);

// �������з����õ��������ͱ���ֵ������ܷ����ɹ����򽫸������еĽ���ƫ��ֵ����
int  GetVarNameAndVaule(char * psStrLine,uint uiStrLen,char **psVarName,uint *uiNameLen,char **psVarValue,uint *uiVallen);

int  UnPackParamFile(char *psParam, uint uiParamLen);

//  ���EDC����
int  UnPackEdcParam(char *psEdcParam, uint uiLen);
int  CheckEdcParam(void);   // ���EDC������������

//  ���ACQ������ChFlag���ڱ�ʾ�����±���һ���µ�ACQ�����������ϴ�û�н�����ɵ�ACQ�м�������
int  UnPackAcqParam(char *psAcqParam, uint uilen, char ChFlag);
int  CheckAcqParam(void);   // ���ACQ������������

//  ���ISSURE����
int  UnPackIssureParam(char *psIssureParam, uint uilen, char ChFlag);
int  CheckIssureParam(void);   // ���ACQ������������

//  ���CARD����
int  UnPackCardParam(char *psCardParam, uint uilen, char ChFlag);
int  CheckCardParam(void);   // ���CARD������������  

// ���EMV AID ����
int  UnpackEmvAidParam(char *psEmvAidParam, uint uilen, char ChFlag);
int  CheckEmvAidParam(void);

// ���EMV AID ����
int  UnpackEmvCapkParam(char *psEmvCapkParam, uint uilen, char ChFlag);
int  CheckEmvCapkParam(void);

int  SetVarValue(ST_PARAM_FILE *pstNodeCfg,char*psVarName,uint uiNameLen,char*psValue,uint uiValueLen);

//   ���������ļ������浽EDC�Ķ�Ӧ�����ṹ��
int  GetParamFile(void);

int  ExistParamFile(void);

void RemoveParamFile(void);

void DispUnpackErrMsg(int iErrCode);
void GetUnpackErrMsg(int iErrCode,char *psErrMsg);

int  GetParamfileName(void);
 
#endif


