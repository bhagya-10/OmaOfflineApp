/******************************************************************************
 * ini.h
 * 
 * Copyright (c) 2010 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2010-07-13   1.0.0    Li.Hua       written
 ******************************************************************************/

#ifndef INI_FILE_OPERATE_H
#define INI_FILE_OPERATE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void	INIFILE;

INIFILE *ini_open(const char *path, int oflag, ...);
int ini_close(INIFILE *_file);

int ini_load(INIFILE *_file);
int ini_save(INIFILE *_file);
int ini_save_as(INIFILE *_file, const char *filename);

int ini_obtain_section_numbers(INIFILE *_file);
int ini_obtain_key_numbers(INIFILE *_file, const char *section);
int ini_browse_section(INIFILE *_file, char *section[], unsigned int nr);
int ini_browse_key(INIFILE *_file, const char *section, char *key[], unsigned int nr);

int ini_set(INIFILE *_file, const char *section, const char *key, const char *value);
int ini_del(INIFILE *_file, const char *section, const char *key);
const char *ini_get(INIFILE *_file, const char *section, const char *key);
int ini_get_int(INIFILE *_file, const char *section, const char *key);
int ini_set_int(INIFILE *_file, const char *section, const char *key, int value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* end for INI_FILE_OPERATE_H */

