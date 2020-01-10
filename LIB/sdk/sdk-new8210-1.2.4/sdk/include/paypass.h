#ifndef PAYPASS_H_
#define PAYPASS_H_
#include "emvapi.h"
#include "emv_cl.h"


#ifdef __cplusplus
extern "C" {
#endif
#pragma pack(1)
typedef struct{
	int 				m_amount_present;
	unsigned int	 	m_amount;
    int                 m_amount_other_present;
	unsigned int 		m_amount_other;
	int                 m_transaction_type_present;
	EMV_CL_TRANS_TYPE   m_transaction_type;
	int                 m_transaction_catagory_code_present;
	unsigned char       m_transaction_catagory_code;
	int                 m_transaction_currency_code_present;
	unsigned char       m_transaction_currency_code[2];
	unsigned char       m_transaction_currency_exponent;
	int                 m_balance_read_before_gac_present;
	int                 m_balance_read_before_gac_len;
	unsigned char       m_balance_read_before_gac[6];
	int                 m_balance_read_after_gac_present;
	int                 m_balance_read_after_gac_len;
	unsigned char       m_balance_read_after_gac[6];
	unsigned char       m_merchant_custom_data[20];
}paypass_trans_param;
#pragma pack()

int paypass_trans_param_set(const paypass_trans_param * param);
int paypass_trans_param_get(paypass_trans_param * param);
EMV_CL_TRANS_TYPE paypass_trans_param_get_trans_type(void);

int paypass_transaction_pre_transaction(void);

int paypass_entry_point(int ifd, int select_next,
	                         unsigned char * aid, unsigned int * aid_len, 
	                         unsigned char * FCI, unsigned int * FCI_Len);



int paypass_transaction(	int                  ifd, 
	                           unsigned char       * aid, 
	                           unsigned int          aid_len, 
							   const unsigned char * FCI, 
							   unsigned int          FCI_Len
							   );


int paypass_tranaction_complete(void);



EMV_CL_APP_TYPE paypass_app_type_get(void);

int paypass_clean(unsigned char * CleanData, unsigned int * size, int * IsLast);
int paypass_torn_record_delete_all(void);

int paypass_msg_signal_data_get(unsigned char * buff, unsigned int * size);
int paypass_final_trans_data_get(unsigned char * buff, unsigned int * size);


void paypass_ms_error_wait_start(void);
void paypass_ms_error_wait(void);


#ifdef __cplusplus
}
#endif


#endif
