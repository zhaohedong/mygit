#include <stdio.h>
#include "mq_common.h"
#include <string.h>
#include "message_queue.h"
#include <unistd.h>
#include <errno.h>
void* MqClient(void* args)
{
    Message msg = {0};
    msg.msgType = 22;
    uint32_t msgId = CreateMessageQueue(MSG_QUEUE_ID);
    printf("MqClient msgId[%d] \n", msgId);
    char *pMsg = "SendMessag Here";
    snprintf((char*)&msg.data[0], strlen(pMsg), "%s", "SendMessag Here");
    while (1)
    {
        // uint32_t len = SendMessage(msgId, (const void *)&msg, MSG_SIZE, 0);
        uint32_t len = SendMessage(msgId, (const void *)&msg, strlen(pMsg) + 1, 0);
        printf("SendMessag msgId[%d] len[%d] content is %s\n", msgId, len, msg.data);
        sleep(3);
    }
    return 0;
}