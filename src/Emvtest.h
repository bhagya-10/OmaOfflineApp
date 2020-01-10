
#ifndef _EMVTEST_H
#define _EMVTEST_H

#include "New8210.h"

#define MAX_APP_NUM       32         //EMV��Ӧ���б����ɴ洢��Ӧ����
#define MAX_KEY_NUM       64         //EMV����֤���Ĺ�Կ�����ɴ洢�Ĺ�Կ��

#define PART_MATCH        0x01       //Ӧ��ѡ��ƥ���־(����ƥ��)
#define FULL_MATCH        0x00       //Ӧ��ѡ��ƥ���־(��ȫƥ��)


void LoadTestKeys(void);
void LoadTestApps(void);


typedef struct _tag_CAPK_INFO
{
	unsigned char KeyID;
	struct issuer_ca_public_key *KeyValue;
}ST_CAPK_INFO;


extern ST_CAPK_INFO AllEmvCapk[];


#endif
