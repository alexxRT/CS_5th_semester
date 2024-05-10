
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct Qelem_ {
    struct Qelem_* next;
    void*          payload;
}Qelem_t;

typedef struct Queue_ {
    Qelem_t* head;
    Qelem_t* tail;
    pthread_mutex_t mutex;
}Queue_t;

typedef enum queue_stat {
    QUEUE_EMPTY = 1,
    QUEUE_BUSY  = 2,
    QUEUE_ERROR = 3
} QUEUE;

void  init_queue(Queue_t* queue);
void  erase_queue(Queue_t* queue);
void  push(Queue_t* queue, void* elem);
void* pop(Queue_t* queue);
QUEUE empty(Queue_t* queue);

#endif