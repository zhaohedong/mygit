#include <stdint.h>
uint32_t CreateThread(void*(*rountine)(void*), void* args);