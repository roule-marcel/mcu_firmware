#include "shell.h"

#include <serial/cprintf/cprintf.h>

typedef struct{
	char c;
	int (* func)(char * buf);
} shell_func_t;

int shell_func_list_size = 0;
shell_func_t shell_func_list[_SHELL_FUNC_LIST_MAX_SIZE];

void shell_init() {

}

int shell_add(char c, int (* pfunc)(char *)) {
	if (shell_func_list_size < _SHELL_FUNC_LIST_MAX_SIZE) {
		shell_func_list[shell_func_list_size].c = c;
		shell_func_list[shell_func_list_size].func = pfunc;
		shell_func_list_size++;
		return 0;
	}
	
	return -1;
}

int shell_exec(char c, char * buf)
{
	int i;
	for(i = 0 ; i < shell_func_list_size ; i++) {
		if (shell_func_list[i].c == c) {
			return shell_func_list[i].func(buf);
		}
	}
	cprintf("%c: no such command\r\n");
	return -1;
}
