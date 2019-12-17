
- qemu backtrace
```
#include <execinfo.h>

static void dump_stack(void)
{
    int j, nptrs;
#define SIZE 100
    void *buffer[100];
    char **strings;

    nptrs = backtrace(buffer, SIZE);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);
    printf("\n");

    free(strings);
}

addr2line -e qemu-system-x86_64 +0x123456
```
