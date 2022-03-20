#include <stdint.h>
#include <stddef.h>
uint32_t CreateMessageQueue(uint32_t key);
uint32_t GetMessageQueue(uint32_t key);
uint32_t SendMessage(int32_t msqid, const void *msgp, size_t msgsz, int32_t msgflg);
uint32_t RecvMessage(int32_t msqid, void *msgp, size_t msgsz, int32_t msgtyp, int32_t msgflg);
