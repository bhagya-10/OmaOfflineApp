#ifndef VISA_H_
#define VISA_H_
#include "emvapi.h"

#define VISA_OK						       0
#define VISA_TRANSACTION_DECLIEN           1
#define VISA_TRANSACTION_TERMINATED        2
#define VISA_APP_BLOCKED                   3
#define VISA_CANCELED                 	   4
#define VISA_CONTACT_TRANSACTION_NEEDED    5
#define VISA_NEED_VSDC_TRANSACTION         6
#define VISA_NEED_RETRY                    7

#define VISA_ICC_ERROR			           8
#define VISA_INVALID_DATA                  9
#define VISA_DATA_MISSING                  10
#define VISA_DATA_DUPLICATE                11
#define VISA_MEMORY_OVERFLOW			   12
#define VISA_MEMORY_NO_ENOUGH			   13
#define VISA_PROGRAMING_ERROR              14
#define VISA_COMMUNICATION_ERROR           15 
#define VISA_CONDITIONS_NOT_SATISFIED      16
#define VISA_CARD_EXPIRED                  17
#define VISA_CARD_IN_BLACK_LIST            18
#define VISA_CANNOT_CONNECT_DECLINE        19
#define VISA_NO_APP                        20
#define VISA_ISSUER_UPDATE_FAILED          21
#define VISA_NEED_RE_SHOW_CARD             22
#define VISA_APDU_TIMEOUT                  23

#define VISA_ENABLED                       1
#define VISA_DISABLED                      0

#define VISA_ZERO_CHECK_DISABLE            VISA_DISABLED
#define VISA_ZERO_CHECK_OPTION1            1
#define VISA_ZERO_CHECK_OPTION2            2

#pragma pack(1)
struct visa_parameters
{
	uint8_t 	m_TransactionProperty[4];	
	uint8_t     m_StatusCheck;
	uint8_t     m_ZeroCheck;
	uint8_t     m_TransactionLimitCheck;
	uint8_t     m_FloorLimitCheck;
	uint8_t     m_CVMRequireCheck;
	uint32_t 	m_Transaction_Limit;
	uint32_t  	m_Floor_Limit;
	uint32_t 	m_CVM_Require_Limit;	
	uint8_t     m_CVN17_Supported;
	uint8_t     m_Track1_Supported;
	uint8_t     m_Track2_Supported;
};
#pragma pack()

typedef enum
{
	VISA_UNKNOW_APP,
 	VISA_MSD_CVN17,
		VISA_MSD_LEGACY,
		VISA_QVSDC//,
//		VISA_VSDC
}VISA_APP_TYPE;

typedef VISA_APP_TYPE * LP_VISA_APP_TYPE;

#define VISA_TRANS_PURCHASE 0x00
#define VISA_TRANS_CASH     0x01
#define VISA_TRANS_REFUND   0x20


struct visa_terminal_aid
{
	uint8_t aid_len;                
	uint8_t aid[16];	         
	uint8_t support_partial_aid_select;             
	uint8_t application_priority;            
	uint32_t target_percentage;           
	uint32_t maximum_target_percentage;        
	uint32_t terminal_floor_limit;          
	uint32_t threshold_value;          
	uint8_t terminal_action_code_denial[5];        
	uint8_t terminal_action_code_online[5];        
	uint8_t terminal_action_code_default[5];       
	uint8_t acquirer_identifier[6];        
	uint8_t len_of_default_ddol;           
	uint8_t default_ddol[254];           
	uint8_t len_of_default_tdol;           
	uint8_t default_tdol[254];           
	uint8_t application_version[2];          
	uint8_t len_of_terminal_risk_management_data;      
	uint8_t terminal_risk_management_data[8]; 
	
	struct visa_parameters m_cl_normal_param;   //Default Parameters
	struct visa_parameters m_cl_cash_param;     //Cash transaction Prarmeters
	struct visa_parameters m_cl_cashback_param; //Cashback parameters
};


struct visa_drl_item{
	uint8_t program_id_len;
	uint8_t program_id[16];
	struct visa_parameters m_cl_normal_param; //Default parameters
	struct visa_parameters m_cl_cash_param;   //Cash transaction parameters
	struct visa_parameters m_cl_cashback_param; //Cashback transaction parameters
};

#ifdef __cplusplus
extern "C" {
#endif


void visa_drl_enable(void);
void visa_drl_disable(void);
int  visa_drl_status(void);
void visa_drl_clear(void);
int  visa_drl_add(const struct visa_drl_item * drl);
int  visa_drl_delete(uint8_t program_id_len, const uint8_t * program_id);
int  visa_drl_count(void);
int  visa_drl_get(uint32_t index, struct visa_drl_item * drl);



void visa_terminal_aid_clear(void);
int  visa_terminal_aid_add(const struct visa_terminal_aid *info);
int  visa_terminal_aid_get(int index, struct visa_terminal_aid *info);
int  visa_terminal_aid_delete(uint8_t aid_len, const void *aid);
int  visa_terminal_aid_count(void);
int  visa_terminal_aid_check(uint8_t aid_len, const void *aid);

typedef int  (*fn_callback_visa_terminal_aid_get)(int index, void * pAidInfo, unsigned int *puiSize);
typedef int  (*fn_callback_visa_terminal_aid_count)(void);
void visa_aid_manage_set_callback(fn_callback_visa_terminal_aid_count callback_visa_terminal_aid_count,
											fn_callback_visa_terminal_aid_get callback_visa_terminal_aid_get);



typedef int (*fn_visa_detect_broken)(unsigned int DetectHandle); //If broken detected, return none zero
void visa_set_broken_detect_func(fn_visa_detect_broken  pFn_DetectBroken, unsigned int DetectHandle);
int visa_get_broken_source(void);

int visa_get_parameter(struct visa_parameters * parameters);

int visa_pre_transaction(uint32_t uiTarnsCounter, uint8_t TransType, uint32_t Amount, uint32_t AmountOther, uint8_t withcashback);
int visa_transaction(int fd);
int visa_complete(void);
int visa_issuer_update_need(void);
int visa_issuer_update(int fd);
int visa_signature_needed(int * pSignature);
int visa_get_app_type(LP_VISA_APP_TYPE pAppType);
int visa_need_save_failed_log(void);
int visa_msd_get_track1(char * pTrack1, unsigned int uiSize);
int visa_msd_get_track2(char * pTrack2, unsigned int uiSize);
int visa_get_terminal_entry_capability(char * pszEntryCapability, unsigned int uiSize);

#ifdef __cplusplus
}
#endif


#endif
