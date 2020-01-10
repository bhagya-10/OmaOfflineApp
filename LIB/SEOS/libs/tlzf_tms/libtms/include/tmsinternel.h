#ifndef     _TMSINTERNEL_H_
#define     _TMSINTERNEL_H_

#include "tmslib.h"
#include "newtmslib.h"
#include "seos.h"


#define  USER_OPER_TIMEOUT		    60		  // 用户超时设置
//  定义通讯的基本元素
typedef struct
{
    int     sockfd;
    int     recvTimeOut;
    int     backup_recvtime;
    int     (*get_recvtime)(int, int *);
    int     (*set_recvtime)(int, int);
    int     (*send)(int, const void *, int, int);
    int     (*recv)(int, void *, int, int);
}tms_comm_opt_t;

int32_t tmsSetDispLang(int32_t mode);
int32_t s_tmsGetDispLang(void);










//void s_tmsDisp0(int32_t Col, int32_t Line, uint32_t DispMode, uint8_t *HanziMsg, uint8_t *EngMsg);
//void s_tmsDisp1(int32_t Col, int32_t Line, uint32_t DispMode, uint8_t *HanziMsg, uint8_t *EngMsg, int32_t Para1);
//void s_tmsDisp2(int32_t Col, int32_t Line, uint32_t DispMode, uint8_t *HanziMsg, uint8_t *EngMsg, int32_t Para1, int32_t Para2);
int32_t s_tmsGetKey(int32_t TimeoutMs);
UINT32 s_tmsStrToLong(UINT8 *buff2);
void s_tmsLongToStr(UINT32 LongDat, UINT8 *buff2);
UINT16 s_tmsStrToUINT16(UINT8 *buff2);
void s_tmsUINT16ToStr(UINT16 Num, UINT8 *buff2);

void tmsShowRet(int32_t Col, int32_t Line, int32_t Mode, int32_t Retcode);


#endif

