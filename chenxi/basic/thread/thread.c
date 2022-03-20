#include "pthread.h"

uint32_t CreateThread(void*(*rountine)(void*), void* args)
{
    pthread_t threadId;
    return  pthread_create(&threadId, 0,  rountine, args);
}