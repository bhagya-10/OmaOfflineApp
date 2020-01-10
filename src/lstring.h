#ifndef _LSTRING_H_11_04_2002
#define _LSTRING_H_11_04_2002

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
//int lstrchar(const char *str,int strpos,DEVICECONTEXT *dc);
//char *lstrcpy( char *strDestination, const char *strSource ,DEVICECONTEXT *dc);
//char *lstrcat(char * s1, const char * s2,DEVICECONTEXT *dc);
//size_t lstrlen(const char * s,DEVICECONTEXT *dc);
//char *lstrshift(const char *s, int strpos,DEVICECONTEXT *dc);
//void lstrsetchar(char *s, int strpos, int c,DEVICECONTEXT *dc);
//int lstrcmp( const char *string1, const char *string2 ,DEVICECONTEXT *dc);

//char *strncpy(char * s1, const char * s2, size_t n);
void TrimLeft( char * strValue );
void TrimRight( char * strValue );
void Trim( char * strValue );

int stricmp( const char *str1, const char* str2);
int strnicmp(const char * dest, const char * source, size_t n);
char *strdup( const char *strSource );

void lstrupr(char* str);

#ifdef __cplusplus
}
#endif

#endif

