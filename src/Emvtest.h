
#ifndef _EMVTEST_H
#define _EMVTEST_H

#include "New8210.h"

#define MAX_APP_NUM       32         //EMV库应用列表最多可存储的应用数
#define MAX_KEY_NUM       64         //EMV库认证中心公钥表最多可存储的公钥数

#define PART_MATCH        0x01       //应用选择匹配标志(部分匹配)
#define FULL_MATCH        0x00       //应用选择匹配标志(完全匹配)


void LoadTestKeys(void);
void LoadTestApps(void);


typedef struct _tag_CAPK_INFO
{
	unsigned char KeyID;
	struct issuer_ca_public_key *KeyValue;
}ST_CAPK_INFO;


extern ST_CAPK_INFO AllEmvCapk[];


#endif
