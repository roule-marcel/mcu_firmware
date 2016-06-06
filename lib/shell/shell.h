#ifndef __SHELL_H__
#define __SHELL_H__

#define _SHELL_FUNC_LIST_MAX_SIZE 64

void shell_init();
int shell_add(char c, int (* pfunc)(char *));
int shell_exec(char c, char * buf);

#endif //__SHELL_H__
