#include "message_queue.h"
#include <sys/msg.h>
#include <stdio.h>
#include "errno.h"
#include <string.h>

uint32_t CreateMessageQueue(uint32_t key)
{
    return msgget((key_t)key, 0666 | IPC_CREAT);
}

uint32_t GetMessageQueue(uint32_t key)
{
    return msgget((key_t)key, 0666);
}

uint32_t SendMessage(int32_t msqid, const void *msgp, size_t msgsz, int32_t msgflg)
{
    uint32_t ret = msgsnd(msqid, msgp, msgsz, IPC_NOWAIT);
    printf("SendMessage %s\n",strerror(errno));
    if (EINVAL == errno)
    {
        printf("msgsnd errno is EINVAL\n");
    }
    return ret;
}

uint32_t RecvMessage(int32_t msqid, void *msgp, size_t msgsz, int32_t msgtyp, int32_t msgflg)
{
    uint32_t ret = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
    printf("RecvMessage %s\n",strerror(errno));
    return ret;
}

