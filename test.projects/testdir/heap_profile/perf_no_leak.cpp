#include <stdio.h>
#include <unistd.h>
int main()
{
for (int i = 1; i < 10; i ++)
{
 char* p = new char[1 << 26];
 delete p;
 fprintf(stderr, "loop %d\n", i);
 sleep(1);
}
 return 0;
}
