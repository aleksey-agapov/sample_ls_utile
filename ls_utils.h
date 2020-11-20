/*
 * ls_utils.h
 *
 *  Created on: Nov 20, 2020
 *      Author: Agapov Aleksey
 */

#ifndef LS_UTILS_H_
#define LS_UTILS_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define OBJ_NOT_FOUND 2


#define KEY_SHORT 0
#define KEY_LIST 1
#define KEY_HELP 2
#define KEY_ERROR 4

enum CMD_STATUS {
	SHORT = KEY_SHORT,
	LIST  = KEY_LIST,
	HELP  = KEY_HELP,
	ERROR = KEY_ERROR
};

extern void errmsg( const char* format, ... );
extern void show_help(char * app_name);
extern char * new(size_t mem_size);
extern void list_dir(const char * path, int is_list);


#endif /* LS_UTILS_H_ */
