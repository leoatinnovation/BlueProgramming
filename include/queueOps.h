#ifndef __QUEUEOPS_H__
#define __QUEUEOPS_H__

#include "bt_data.h"

#define QUEUE_SIZE 10


typedef struct message_queue {
    bt_data_t *buffer[QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t full;
} message_queue_t;


void init_queue(message_queue_t *queue);
void enqueue(message_queue_t *queue, bt_data_t *item);
bt_data_t* dequeue(message_queue_t *queue);



#endif