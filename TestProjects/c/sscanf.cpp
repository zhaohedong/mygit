/* sscanf example */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex>
#include <string>

int bta_hf_client_parse_cind_list(char* buffer)
{
  int offset = 0;
  char name[16];
  uint32_t min, max;
  uint32_t index = 0;
  int res;

  while (1) {
    res = sscanf(buffer, "(\"%16[^\"]\",(%u%*[-,]%u))%n", name, &min, &max, &offset);
    printf("name[%s], min[%d], max[%d], offset[%d], res[%d]\n", name, min, max, offset, res);
    if (res != 3) {
      printf("name[%s], min[%d], max[%d], offset[%d], res[%d]\n", name, min, max, offset, res);
      return -1;
    }

    buffer += offset;
    index++;

    if (*buffer != ',') {
      break;
    }

    buffer++;
  }
  return 0;
}

int main()
{
    char* str  = "(\"service\",(0-1)),(\"call\",(0,1)),(\"callsetup\",(0-3)),(\"callheld\",(0-2)),(\"signal\",(0-5)),(\"roam\",(0,1)),(\"battchg\",(0-5)))";
#if 0
   int day, year;
   char weekday[20], month[20], dtm[100];

   strcpy( dtm, "1:2,3 4");
   int a,b,c,d;
   sscanf( dtm, "%d:%d,%d %d", &a, &b, &c, &d );

   int e = sizeof("abc"     "def");

   printf("%d %d %d %d %d\n", a, b, c, d, e);
#endif
    //bta_hf_client_parse_cind_list(str);

    int offset = 0;
    uint8_t mode = 0;
    char opstr[64] = {0};
    char* str2 = "(1,2,3,4)";
    //sscanf(buffer, "\"%32[^\"]\"\r\n%n", numstr, &offset)
    //sscanf(str2, "%hhi,0,\"%16[^\"]\"%n", &mode, opstr, &offset);
    //sscanf(str2, "%hhi,0,\'%16[^\']\'%n", &mode, opstr, &offset);
    sscanf(str2, "(%20[^)]", opstr);

    std::string tmpStr = opstr;
    std::regex regex("\\,");
    std::vector<std::string> out(std::sregex_token_iterator(tmpStr.begin(), tmpStr.end(), regex, -1),
                                 std::sregex_token_iterator());
    for (auto s : out) {
      printf("s is %s\n", s.c_str());
    }
    return(0);
}