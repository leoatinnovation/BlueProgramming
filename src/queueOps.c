#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queueOps.h"

// queue structure
message_queue_t queue;

void init_queue(message_queue_t *queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->empty, NULL);
    pthread_cond_init(&queue->full, NULL);
}

void enqueue(message_queue_t *queue, bt_data_t *item) {
    pthread_mutex_lock(&queue->mutex);
    printf("enqueue ============ \n");
    while (queue->count == QUEUE_SIZE) {
        pthread_cond_wait(&queue->full, &queue->mutex);
    }
    queue->buffer[queue->rear] = item;
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->count++;
    pthread_cond_signal(&queue->empty);
    pthread_mutex_unlock(&queue->mutex);
}

bt_data_t* dequeue(message_queue_t *queue) {
    pthread_mutex_lock(&queue->mutex);
    printf(" dequeue============ \n");
    while (queue->count == 0) {
        pthread_cond_wait(&queue->empty, &queue->mutex);
    }
    bt_data_t *item = queue->buffer[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->count--;
    pthread_cond_signal(&queue->full);
    pthread_mutex_unlock(&queue->mutex);
    return item;
}
