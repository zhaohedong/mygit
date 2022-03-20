#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
typedef enum {
  LOG_LEVEL_FATAL = 0,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG
} LOG_LEVEL;
static pthread_mutex_t mutex_;
static LOG_LEVEL logLevel_ = LOG_LEVEL_INFO;

#ifndef LOG_TAG
#define LOG_TAG "bluetooth"
#endif

#define LOG_DEBUG(...) LOG_Output(LOG_LEVEL_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOG_INFO(...) LOG_Output(LOG_LEVEL_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_WARN(...) LOG_Output(LOG_LEVEL_WARNING, LOG_TAG, __VA_ARGS__)
#define LOG_ERROR(...) LOG_Output(LOG_LEVEL_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_FATAL(...) LOG_Output(LOG_LEVEL_FATAL, LOG_TAG, __VA_ARGS__)
//#define MY_LOG_INFO(...) LOG_INFO("[%s %s]%s:%d:%s():__VA_ARGS__, __TIME__,  __DATE__, __FILE__, __LINE__, __FUNCTION__)

static char printfBuf[1024];
int LOG_Output(LOG_LEVEL level, const char* tag, const char* fmt, ...) {
  if (level > logLevel_) {
    return 0;
  }

  //pthread_mutex_lock(&mutex_);

  va_list arglist;
  va_start(arglist, fmt);
  vsprintf(printfBuf, fmt, arglist);
  va_end(arglist);

  printf("[%s]: %s\n", tag, printfBuf);
  //pthread_mutex_unlock(&mutex_);

  return 0;
}

#define BT_NO_ERROR 0
#define HFP_AG_ERROR_CHECK(ret)                                                     \
    do {                                                                            \
      if (ret != BT_NO_ERROR) {                                                     \
          LOG_INFO("%s:%d:%s():ret[%d]",  __FILE__, __LINE__, __FUNCTION__, ret);   \
          return ret;                                                               \
      }                                                                             \
    } while (0)