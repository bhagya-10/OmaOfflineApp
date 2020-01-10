#ifndef	PED_SIGNATURE_H
#define PED_SIGNATURE_H
#include "global.h"


#define  MAX_PACKET_LEN 2048*5+10



int ShakeHands(void);
int SetElecSignTimeOut(uint16_t usTimeOutMs);
int P_GetElecSignature(ST_TS_ELEC_SIGNATURE *pstElecSignInfo);
uint8_t TransElecSignature(void);
#endif
