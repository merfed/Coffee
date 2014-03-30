#pragma once

/*

You should call spawnm_main, it will call back to your
sub_help and sub_system

*/

extern int sub_system(char * command);
extern void sub_help();

int spawnm_main(int argc,char *argv[]);

// helpers for you to convert sub_system to argc/argv :
int system_argcargv(char * commandStr,char * exeName);
extern int sub_system_argcargv(int argc,char * argv[]);
