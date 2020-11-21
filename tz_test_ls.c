/*
 ============================================================================
 Name        : tz_test_ls.c
 Author      : Agapov Aleksey
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "ls_utils.h"

int
main (int argc, char **argv)
{
	int ret_status = EXIT_SUCCESS;
	int index;
	int cmd_options;
	size_t path_size = 0;
	char *search_path = NULL;

	enum CMD_STATUS list_status = SHORT;

	while ((cmd_options = getopt(argc, argv, "hl::")) != -1) {
		switch (cmd_options) {
		case 'l':				/* LIST OPTION */
			list_status = list_status | LIST;
			break;
		case 'h':				/* HELP OPTION */
			list_status = list_status | HELP;
			break;
		default:
			list_status = list_status | ERROR;
			errmsg("Unknown option `-%c'.\n", cmd_options);
			break;
		}

		if (list_status & ERROR) {			/* ABORT EXECUTION*/
			break;
		}

	}
	if (list_status & ERROR) {				/* SET ERROR CODE */
		ret_status = EXIT_FAILURE;
	} else if (list_status & HELP) {		/* SHOW HELP */
		show_help(argv[0]);
	} else {								/* SHOW LIST FILE */
		for (index = optind; index < argc; index++) {
			path_size += strlen(argv[index]) + 1;
		}

		if (path_size) {					/* PATH TAKE FROM INPUT */
			int copy_index = 0;
			search_path = new(path_size + 1);

			for (index = optind; index < argc; index++) {
				if (copy_index) {
					search_path[copy_index] = ' ';
				}
				strcpy(search_path + copy_index, argv[index]);
				copy_index += strlen(argv[index]);
			}
		} else {							/* PATH TAKE FROM ENVEROMENT */
			char *pwd_path = getenv("PWD");
			if (pwd_path) {
				path_size = strlen(pwd_path);
				search_path = new(path_size + 1);
				memcpy(search_path, pwd_path, path_size);
			}
		}

		if (search_path) {					/* LIST PATH SET */
			printf("Path:%s\n", search_path);
			if (access(search_path, F_OK)==0) {	/* PATH FOUND */
				list_dir(search_path,list_status & LIST);
			} else {							/* PATH NOT FOUND */
				ret_status = OBJ_NOT_FOUND;
				errmsg("Path %s - not found.", search_path);
			}

			free(search_path);
		} else {
			ret_status = EXIT_FAILURE;
			errmsg("Unknown path.");
		}
	}

	return ret_status;
}

