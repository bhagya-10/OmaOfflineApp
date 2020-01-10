#ifndef __PDUPARSE_H__
#define	__PDUPARSE_H__
int Wnet_PduSmsParse(const char *pRecvBuff, int iRecvLen, tSmsRecv RecvSms[], 
													int iSmsLength, int iMode);
int Wnet_PduSmsDivision(const tSmsSend *pSmsSend, tPduSmsSend *pPduSmsSend);

#endif
