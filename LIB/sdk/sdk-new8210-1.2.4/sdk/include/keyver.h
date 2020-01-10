/**
 * @file    keyver.h
 *          
 * @author  Li.Hua <lihua_338@163.com>.
 *          
 * @date    2014-11-27
 *          
 * @brief   
 *          
 * @note    
 *          
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 *
 */

#ifndef __KEYVER_H__
#define __KEYVER_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum {
	SIGN_KEY_BOOT		= 0,
	SIGN_KEY_KERNEL,
	SIGN_KEY_FIRMWARE,
	SIGN_KEY_APP,
	SIGN_KEY_USER0,
	SIGN_KEY_USER1,
	SIGN_KEY_USER2,
	SIGN_KEY_USER3,
	SIGN_KEY_USER4,
	SIGN_KEY_USER5,
	SIGN_KEY_USER6,
	SIGN_KEY_USER7,
	SIGN_KEY_USER8,
	SIGN_KEY_USER9,
	SIGN_KEY_USER10,
	SIGN_KEY_USER11,
	SIGN_KEY_USER12,
	SIGN_KEY_USER13,
	SIGN_KEY_USER14,	
	SIGN_KEY_USER15,
};

typedef struct {
	char	version[64];	// key information
	int		status;			// 0: effective, 1: expired
	int		type;			// key type, refer enum above
} KeyVerInfo_t;

/**
 * @fn          get_key_version
 * @brief       get key information about key and its signer
 * @param in    type - refer enum above
 * @param out   key  - key information, key can set to NULL
 *              signer - who sign the key, signer can be set to NULL
 * @return      
 * @note        
 */
int get_key_version(int type, KeyVerInfo_t *key, KeyVerInfo_t *signer);

#ifdef __cplusplus
}	/* extern "C" */
#endif /* __cplusplus */

#endif /* __KEYVER_H__ */

