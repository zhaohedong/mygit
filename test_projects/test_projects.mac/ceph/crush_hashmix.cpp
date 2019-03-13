#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
using namespace std;

#define crush_hashmix(a, b, c) do {			\
		a = a-b;  a = a-c;  a = a^(c>>13);	\
		b = b-c;  b = b-a;  b = b^(a<<8);	\
		c = c-a;  c = c-b;  c = c^(b>>13);	\
		a = a-b;  a = a-c;  a = a^(c>>12);	\
		b = b-c;  b = b-a;  b = b^(a<<16);	\
		c = c-a;  c = c-b;  c = c^(b>>5);	\
		a = a-b;  a = a-c;  a = a^(c>>3);	\
		b = b-c;  b = b-a;  b = b^(a<<10);	\
		c = c-a;  c = c-b;  c = c^(b>>15);	\
	} while (0)

int main(int argc, char *argv[])
{
    int i;
    for (i = 0; i < argc; i++)
        printf("Argument %d is %s.\n", i, argv[i]);
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = atoi(argv[3]);
    crush_hashmix(a,b,c);
    
    printf("a is %d \n", a);
    printf("b is %d \n", b);
    printf("c is %d \n", c);
    return 0;
}

