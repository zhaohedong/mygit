#ifndef MQ_COMMON_H
#define MQ_COMMON_H

#define MSG_QUEUE_ID 1234
#define MSG_SIZE 512

typedef struct {
    long msgType;
    char data[MSG_SIZE];
} Message;

#endif // MQ_COMMON_H