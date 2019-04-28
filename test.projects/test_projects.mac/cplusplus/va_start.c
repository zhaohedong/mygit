#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "string.h"

int demo(char*, ...);
/**
* @brief The entry of this program
*
* @param argc counts of argument
* @param argv argument variables stored in
*
* @return EXIT_SUCCESS
*/
int main (int argc, char **argv)
{
demo("Demo","This","is","a","demo!","");
return 0;
}	//end of function main



int demo (char* msg, ...)
{
va_list argp;
int argno = 0;
char* para;

va_start(argp,msg);
while(1)
{
para = va_arg(argp, char*);
if(strcmp(para, "")==0)
break;
printf("parameter #%d is : %s\n", argno, para);
argno++;
}
va_end(argp);
return 0;
}	//end of function demo
