#include <stdio.h>
#include <unistd.h>
#include "message_queue.h"
#include "mq_common.h"
#include "mq_client.h"
#include "thread.h"

int main (int argc, char *argv[])
{
    Message msg = {0};
    uint32_t msgId = CreateMessageQueue(MSG_QUEUE_ID);
    printf("main msgId[%d] \n", msgId);

    CreateThread(MqClient, NULL);
    while (1)
    {
        uint32_t len = RecvMessage(msgId, &msg, MSG_SIZE, 22, 0);
        printf("RecvMessage msgId[%d], len[%d], RecvMessage is %s\n", msgId, len, msg.data);
        sleep(3);
    }
    return 0;
}