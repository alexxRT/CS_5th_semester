#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "queue.h"

void init_queue(Queue_t* queue) {
    queue->head = NULL;
    queue->tail = NULL;

    pthread_mutex_init(&queue->mutex, NULL);

    return;
}

void erase_queue(Queue_t* queue) {
    pthread_mutex_destroy(&queue->mutex);

    Qelem_t* elem = queue->tail; 
    Qelem_t* next = elem;
    while (next) {
        next = elem -> next; 
        free(elem);
    }
    return;
}

void push(Queue_t* queue, void* elem) {
    Qelem_t* new_elem = CALLOC(1, Qelem_t);
    new_elem->payload = elem;
    new_elem->next    = NULL;

    pthread_mutex_lock(&queue->mutex);

    if (queue->head == NULL) {
        queue->head = new_elem;
        queue->tail = new_elem;
    }
    else {
        new_elem->next = queue->tail;
        queue->tail = new_elem;
    }

    pthread_mutex_unlock(&queue->mutex);
}

void* pop(Queue_t* queue) {
    // printf("Tail address %p\n", queue->tail);
    // printf("Head address %p\n", queue->head);

    pthread_mutex_lock(&queue->mutex);

    if (queue->head == NULL) {
        //fprintf(stderr, "Nothing to pop!! Error, returning NULL...\n");

        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }
    else if (queue->head == queue->tail) {
        void* elem = queue->tail->payload;

        free(queue->head);
        queue->head = NULL;
        queue->tail = NULL;

        pthread_mutex_unlock(&queue->mutex);
        return elem;
    }
    else {
        void*    elem = queue->tail->payload;
        Qelem_t* tail = queue->tail;

        queue->tail = queue->tail->next;
        free(tail);

        pthread_mutex_unlock(&queue->mutex);
        return elem;
    }

    pthread_mutex_unlock(&queue->mutex);
    return NULL;
}

QUEUE empty(Queue_t* queue) {
    pthread_mutex_lock(&queue->mutex);

    if (queue->tail == NULL) {
        pthread_mutex_unlock(&queue->mutex);
        return QUEUE_EMPTY;
    }
    else {
        pthread_mutex_unlock(&(queue->mutex));
        return QUEUE_BUSY;
    }

    pthread_mutex_unlock(&queue->mutex);
    return QUEUE_ERROR;
}