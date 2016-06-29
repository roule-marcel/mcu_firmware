#ifndef __SHELL_H__
#define __SHELL_H__

#define _SHELL_FUNC_LIST_MAX_SIZE 32

void shell_init();
int shell_add(char c, int (* pfunc)(int argc, char ** argv), char * description);
int shell_exec(char c, char * buf);
int sh_help(int argc, char ** argv);

#endif //__SHELL_H__
