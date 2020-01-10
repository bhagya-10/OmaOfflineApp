/*
 * @file    emv_contactless.h
 *          
 * @author  harrison lee <lhxsz@21cn.com>.
 *          
 * @date    2014-10-18
 *          
 * @brief   
 *          
 * @note    
 *          
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2014-10-18   1.0.0    Harrison     Create
 */

#ifndef EMV_CONTACTLESS_H_
#define EMV_CONTACTLESS_H_
#include "emvapi.h"


#ifdef __cplusplus
extern "C" {
#endif



#define EMV_CL_OK_APPROVED			0   
#define EMV_CL_DECLINE   			1   
#define EMV_CL_NEED_ONLINE 			2   
#define EMV_CL_SELECT_NEXT    		3    
#define EMV_CL_OTHER_INTERFACE 		4  
#define EMV_CL_OTHER_CARD       	5   
#define EMV_CL_TRY_AGAIN        	6   
#define EMV_CL_NEED_ANOTHER_TAP 	7   
#define EMV_CL_TERMINATED    		8   
#define EMV_CL_ERROR        	    9 
#define EMV_CL_NO_SUPPORTED         10 
#define EMV_CL_CANCELED             11


#define EMV_CL_PURCHASE               0x00
#define EMV_CL_PURCHASE_WITH_CASHBACK 0x09
#define EMV_CL_CASH_WITHDRAWVAL       0x01
#define EMV_CL_CASH_DISBURSEMENT      0x17
#define EMV_CL_REFUND                 0x20



typedef unsigned char EMV_CL_TRANS_TYPE;

typedef enum{
	EMV_CL_APP_UNKNOW,
		EMV_CL_APP_MAGSTRIPE,
		EMV_CL_APP_EMV
}EMV_CL_APP_TYPE;

typedef enum{	
	    EMV_CL_KERNEL_UNKNOW,
		EMV_CL_KERNEL_1,
		EMV_CL_KERNEL_2,
		EMV_CL_KERNEL_3,
		EMV_CL_KERNEL_4,
		EMV_CL_KERNEL_5,
		EMV_CL_KERNEL_6,
		EMV_CL_KERNEL_7		
}EMV_CL_KERNEL_ID;




#pragma pack(1)
typedef struct {
	EMV_CL_TRANS_TYPE   trans_type;
	EMV_CL_KERNEL_ID  	kernel_id;
	int                 supported;
	int              	aid_len;
	unsigned char    	aid[16];	
	unsigned int     	param_len;
	unsigned char    	param[1];
}emv_cl_kernel_param;
#pragma pack()


typedef enum{
	EMV_CL_APDU_PROCESSING,
		EMV_CL_APDU_ABORTED,
		EMV_CL_APDU_DONE
}EMV_CL_APDU_STATUS;



typedef int (*fn_emv_cl_kernel_param_get_count)(void);
typedef int (*fn_emv_cl_kernel_param_get)(int index, 
										  void * pBuff,
										  unsigned int * BuffSize);

typedef int (*fn_emv_cl_apdu_send)(int fd, unsigned int sendlen, const void * senddata);
typedef int (*fn_emv_cl_apdu_resp_get)(int fd, int AllowStop, unsigned int *recvlen, void *recvdata, EMV_CL_APDU_STATUS * status);


void emv_cl_reader_set_callback(fn_emv_cl_apdu_send 			apdu_send,		
									    fn_emv_cl_apdu_resp_get     apdu_resp_get);



void emv_cl_kernel_param_set_callback(fn_emv_cl_kernel_param_get_count param_get_count,
												fn_emv_cl_kernel_param_get       param_get);

int emv_cl_kernel_param_init(void);
int emv_cl_kernel_param_add(const emv_cl_kernel_param * config);
int emv_cl_kernel_param_remove(
								EMV_CL_TRANS_TYPE     trans_type,
								EMV_CL_KERNEL_ID	  KernelID,										
	                            const unsigned char * aid, 
	                            unsigned int          aid_len);
int emv_cl_kernel_param_get_count(void);
int emv_cl_kernel_param_get(int index,
	                        emv_cl_kernel_param ** config);


int emv_cl_trans_param_set(EMV_CL_KERNEL_ID KernelID,
						   const unsigned char * param,
						   unsigned int          param_len);

int emv_cl_trans_param_get(EMV_CL_KERNEL_ID KernelID,
								   unsigned char * param,
								   unsigned int  * param_len);


int emv_cl_pre_processing(void);
int emv_cl_entry_point( int ifd, int select_next, 
						 EMV_CL_KERNEL_ID * 	pKernelID,
	                     unsigned char * aid, unsigned int * aid_len, 
	                     unsigned char * FCI, unsigned int * FCI_Len
	                  );



int emv_cl_transaction(int ifd,
	                   EMV_CL_KERNEL_ID      KernelID, 
	                   unsigned char * aid,  unsigned int aid_len, 
                       unsigned char * FCI,  unsigned int FCI_Len);
int emv_cl_transaction_complete(EMV_CL_KERNEL_ID KernelID);                      

int emv_cl_torn_record_delete_all(EMV_CL_KERNEL_ID KernelID);
#ifdef __cplusplus
}
#endif
#endif
