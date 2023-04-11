#ifndef UTILITIES_H
#define UTILITIES_H

#define CMDBUFFERLENGTH 100

int command_run(char *cmd, ...);            //used to run a command from terminal
void print_error(char *errorMessage, ...);  //Used to print an error
void print_hexdump(char *str, int length);  //Used for debugging 

#define CLEAR(x) memset(&x, 0, sizeof(x))

#endif