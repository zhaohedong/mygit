#include <stdint.h>

static uint16_t a = 7;
static void* g_p = &a;

int main()  
{   
    uint16_t aa = *(uint16_t*)g_p;
    int a = 77;
    int b = 88;
    return 0;  
}  