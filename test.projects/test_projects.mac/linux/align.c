#include "stdio.h"
struct align {
char *ptr1;
char c;
char *ptr2;
};

#define A(x)  T_##x
#define B(x)  #x
//#define C(x)  #@x

A(aa)
B(aa)
//C(aa)
int main()
{
	struct align a;
	printf("&a.ptr1 is 0x%x\n",&a.ptr1);
	printf("&a.c    is 0x%x\n",&a.c);
	printf("&a.ptr2 is 0x%x\n",&a.ptr2);
	
	return 0;
}
