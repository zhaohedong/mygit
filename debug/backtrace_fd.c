#include<stdio.h>
  #include<execinfo.h>
  #include<stdlib.h>

  FILE *fp;
  void func5() {
        void *array[12];
        int size, i, fd;
        char **str;
        fp = fopen("./backtrace_info.txt", "a");
        fd = fileno(fp);
        fseek(fp, 0, SEEK_END);
        size = backtrace(array, 12);
        printf("Backtrace:\n");
        for (i = 0; i < size; i++) {
                printf("0x%08x\n", (int)array[i]); // printing symbols on output screen
        }

        /* converts address to string and writes the o/p to a file */
        backtrace_symbols_fd(array, size, fd);
        printf("No of level in backtrace:%d\n", size);
  }

  void func4() {
        func5();
  }

  void func3() {
        func4();
  }

  void func2() {
        func3();
  }

  void func1() {
        func2();
  }


  int main() {
        func1();
        return 0;
  }
