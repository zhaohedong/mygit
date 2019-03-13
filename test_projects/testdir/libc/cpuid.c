#include <stdio.h>
int main ()
{

     int op = eax, ebx, ecx, edx;
     FILE *f;
       __asm__("cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (op));
     f = fopen("conftest_cpuid", "w"); 
     if (!f) return 1;
     fprintf(f, "%x:%x:%x:%x\n", eax, ebx, ecx, edx);
     fclose(f);
     return 0;
}

