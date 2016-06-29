#include "shell.h"

#include <serial/cprintf/cprintf.h>

#define ARGC_MAX 8

char help[] = "help";

typedef struct{
	char c;
	int (* func)(int argc, char ** argv);
	char * description;
} shell_func_t;

int shell_func_list_size = 0;
shell_func_t shell_func_list[_SHELL_FUNC_LIST_MAX_SIZE];

void shell_init() {
	shell_add('h', sh_help, help);
}

int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description) {
	if (shell_func_list_size < _SHELL_FUNC_LIST_MAX_SIZE) {
		shell_func_list[shell_func_list_size].c = c;
		shell_func_list[shell_func_list_size].func = pfunc;
		shell_func_list[shell_func_list_size].description = description;
		shell_func_list_size++;
		return 0;
	}
	
	return -1;
}

int shell_exec(char c, char * buf)
{
	int i;

	int argc;
   	char * argv[ARGC_MAX];
	char *p;

	for(i = 0 ; i < shell_func_list_size ; i++) {
		if (shell_func_list[i].c == c) {
			argc = 1;
			argv[0] = buf;

			for(p = buf ; *p != '\0' && argc < ARGC_MAX ; p++){
				if(*p == ' ') {
					*p = '\0';
					argv[argc++] = p+1;
				} 
			}

			return shell_func_list[i].func(argc, argv);
		}
	}
	cprintf("%c: no such command\r\n");
	return -1;
}

int sh_help(int argc, char ** argv) {
	int i;
	for(i = 0 ; i < shell_func_list_size ; i++) {
		cprintf("%c %s\r\n", shell_func_list[i].c, shell_func_list[i].description);
	}

	return 0;
}
