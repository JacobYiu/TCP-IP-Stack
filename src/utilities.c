#include "imports.h"
#include "utilities.h"

int command_run(char *cmd, ...)
{
    va_list args;                   
    char buffer[CMDBUFFERLENGTH];
    va_start(args, cmd);            //Initialize arguments

    vsnprintf(buffer, CMDBUFFERLENGTH, cmd, args);  //Write to buffer

    printf("The command is: %s\n", buffer);         //Print buffer

    return system(buffer);                          //Execute commands on a terminal
}

void print_error(char *errorMessage, ...)
{
    va_list args;                           
    char buffer[200];
    va_start (args, errorMessage);
    
    vsnprintf(buffer,sizeof(buffer), errorMessage, args);
    va_end(args);

    perror(buffer);                                 //Prints the error message to stderr
}

void print_hexdump(char *str, int len)
{
    printf("Printing hexdump:\n");
    for (int i = 0; i < len; i ++) {
        if (i % 8 == 0) printf("\n");
        printf("%02x ", (unsigned char)str[i]);
    }

    printf("\n");
}