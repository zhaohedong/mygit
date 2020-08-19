#include "stdio.h"
#define uint16_t unsigned short
int func1(uint16_t a, uint16_t b)
{
	uint16_t c,d = 0;
	c = a;
	d = b;	
	return c+d;
}
int main()
{
	func1(1,2);
	return 0;
}
