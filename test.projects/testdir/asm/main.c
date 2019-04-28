extern void print(void);
extern void myprint(void);
extern void print2(void);
extern void print3(int,int,int);
extern void print4(char*,int,int,int);
//#include "stdio.h"

char a[256*1024*1024] = {0};
int localprint(char* s, int la, int lb, int lc)
{
 //printf("input is %s \n",s);
 return 0;
}

int main(int argc, char* argv[])
{
// int a = 4;
// int b = 8;
// int c = 12;
// int d = 16;
// int e = 20;
// myprint();

// print3(1,2,3);
// localprint(argv[1],1,2,3);
 while(1) print4(argv[1],1,2,3);
 return 0;
}

