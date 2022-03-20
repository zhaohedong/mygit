#include "log.h"

int fun1() {
  LOG_INFO("%s", __FUNCTION__);
  HFP_AG_ERROR_CHECK(100);
  LOG_INFO("%s", __FUNCTION__);
}

int fun2() {
  LOG_INFO("%s", __FUNCTION__);
  return 0;
}

int main()  
{   
    int a = 77;
    int b = 88;
    //bool c = true;
    char* str= "what happend";
    fun1();
    fun2();
    LOG_INFO("%s", __FUNCTION__);
    //LOG_INFO("faf a = %d, b = %d", a, b);
    //LOG_INFO("[%s %s]%s:%d:%s():%s", __TIME__,  __DATE__, __FILE__, __LINE__, __FUNCTION__, str);
    //LOG_INFO("File:%s", __LINE__);
    //printf("file: %s\tline: %d\r\n", __FILE__, __LINE__);
    return 0;  
}  