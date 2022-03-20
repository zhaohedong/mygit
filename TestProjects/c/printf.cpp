/* sscanf example */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex>
#include <string>
typedef uint8_t unsigned char;

int main()
{
  uint8_t i = 12;
  printf("i is %d by percent d\n", i);
  printf("i is %hhu by percent hhu\n", i);

  uint8_t j = 0x12;
  uint8_t k = 0x34; 

  auto t = j ^ k;
  printf("t size is %d \n", t);

}