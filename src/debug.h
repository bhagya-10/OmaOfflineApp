#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
* @brief Init debug function, on com port 115200,8n1
*
* @return 0 success,-1 fail
*/
int Init_debug(void);

/*!
* @brief Uninit debug function
*
* @return 0 success,-1 fail
*/
int Uninit_debug(void);

/*!
* @brief Print formatted string on debug
*
* @return 0 success
*/
int Debug_Log(char *buf,...);

#ifdef __cplusplus
}
#endif

#endif
