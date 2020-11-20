/*
 * ls_utils.c
 *
 *  Created on: Nov 20, 2020
 *      Author: Agapov Aleksey
 */

#include <libgen.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>
#include <pwd.h>
#include <grp.h>
#include "ls_utils.h"

#define DEF_BUFF(buff_name, buf_size) \
	char buff_name[buf_size]; \
	memset(buff_name,0,buf_size);


void list_file_info(const char * path_item, const char* item_name);


void errmsg( const char* format, ... )
{
  va_list arglist;
  printf( "Error: " );
  va_start( arglist, format );
  vprintf( format, arglist );
  va_end( arglist );
}

void show_help(char * app_name){
	puts("!!!========================================================!!!"); /*  */
	printf("exec:%s [options] <path>\n",  basename(app_name));
	puts("!!!=====options:===========================================!!!");
	puts("!!!==========-l============================================!!!"); /*  */
	puts("!!!==========-h============================================!!!"); /*  */
	puts("!!!========================================================!!!"); /*  */
}

char * new(size_t mem_size) {
	char * mem_buf = malloc(mem_size);
	if (mem_buf) {
		memset(mem_buf,0,mem_size);
	} else {
		fprintf(stderr, "Error allocate memory.\n");
		abort();
	}
	return mem_buf;
}


void list_dir(const char *path, int is_list) {
	DIR *d;
	struct dirent *dir;
	d = opendir(path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_name[0] != '.') {
				if (is_list) {
					char *item_full_path = new(
							strlen(path) + strlen(dir->d_name) + 2);
					sprintf(item_full_path, "%s/%s", path, dir->d_name);
					list_file_info(item_full_path, dir->d_name);
					free(item_full_path);
				} else {
					printf("%s\t", dir->d_name);
				}
			}
		}
		closedir(d);
	} else {
		char *item_full_path = new(strlen(path)+1);
		strcpy(item_full_path,path);
		if (is_list) {
			list_file_info(path, basename(item_full_path));
		} else {
			printf("%s\n", basename(item_full_path));
		}
		free (item_full_path);
	}
}

const char * mode_to_string(char *mode_buffer, mode_t obj_mode)
 {
	int count = 0;

	switch (obj_mode & S_IFMT) {

	case S_IFBLK:
		mode_buffer[count] = 'b';
		break;
	case S_IFCHR:
		mode_buffer[count] = 'c';
		break;
	case S_IFDIR:
		mode_buffer[count] = 'd';
		break;
	case S_IFIFO:
		mode_buffer[count] = 'p';
		break;
	case S_IFLNK:
		mode_buffer[count] = 'l';
		break;
	case S_IFREG:
		mode_buffer[count] = '-';
		break;
	case S_IFSOCK:
		mode_buffer[count] = 's';
		break;
	default:
		mode_buffer[count] = '?';
		break;
	}
	count ++;


	mode_buffer[count] = (obj_mode & S_IRUSR) ? 'r' : '-'; count ++; // 0
	mode_buffer[count] = (obj_mode & S_IWUSR) ? 'w' : '-'; count ++; // 1
	mode_buffer[count] = (obj_mode & S_IXUSR) ? 'x' : '-'; count ++; // 2
	mode_buffer[count] = (obj_mode & S_IRGRP) ? 'r' : '-'; count ++; // 3
	mode_buffer[count] = (obj_mode & S_IWGRP) ? 'w' : '-'; count ++; // 4
	mode_buffer[count] = (obj_mode & S_IXGRP) ? 'x' : '-'; count ++; // 5
	mode_buffer[count] = (obj_mode & S_IROTH) ? 'r' : '-'; count ++; // 6
	mode_buffer[count] = (obj_mode & S_IWOTH) ? 'w' : '-'; count ++; // 7
	mode_buffer[count] = (obj_mode & S_IXOTH) ? 'x' : '-'; count ++; // 8


	if (obj_mode & S_ISUID) mode_buffer[3] = (mode_buffer[3] == 'x')?'s':'S';
	if (obj_mode & S_ISGID) mode_buffer[6] = (mode_buffer[6] == 'x')?'s':'S';
	if (obj_mode & S_ISVTX) mode_buffer[9] = (mode_buffer[9] == 'x')?'t':'T';
	return mode_buffer;
 }


char* get_auth_to_string(char *auth_buff, struct stat item_status) {

	struct group *grp;
	struct passwd *pwd;

	pwd = getpwuid(item_status.st_uid);

	if (pwd != NULL) {
		sprintf(auth_buff, " %s", pwd->pw_name);
	} else {
		sprintf(auth_buff, " %ld", (long) item_status.st_uid);
	}

	grp = getgrgid(item_status.st_gid);
	if (grp != NULL) {
		sprintf(auth_buff+strlen(auth_buff), " %s", grp->gr_name);
	} else {
		sprintf(auth_buff+strlen(auth_buff), " %ld", (long) item_status.st_gid);
	}

	return auth_buff;
}





void list_file_info(const char *path_item, const char* item_name) {
	struct stat item_status;
	if (stat(path_item, &item_status) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}

	DEF_BUFF(mode_buff, 16)
	DEF_BUFF(auth_buff, 128)
	DEF_BUFF(time_buff, 32)


	strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", localtime(&item_status.st_ctime));

	printf("%s %ld%32s%17lld\t%s %s\n",
			mode_to_string(mode_buff, item_status.st_mode),
			(long) item_status.st_nlink,
			get_auth_to_string(auth_buff, item_status),
			(long long) item_status.st_size,
			time_buff,
			item_name);
}












