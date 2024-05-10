#include <math.h>
#include <stdlib.h>
#include "thread_work.h"
#include "method.h"
#include "memory.h"
#include "queue.h"

typedef struct init_interval_ {
    Queue_t* queue;
    integrate_function func;
}init_interval_t;

#define MIN_H 1e-15

float get_h(double left_p, double right_p, integrate_function func) {
    //first approximation, take simply a half of the interval
    double h = (right_p - left_p) / 2;
    double error;

    do {
        error = fabs(method(left_p, right_p, h / 2, func) \
         - method(left_p, right_p, h, func));

        if (h / 2 < MIN_H)
             break;

        h /= 2;
    }
    while (error > (PRECISION / INTERVALS_NUM));

    if (error >= (PRECISION / INTERVALS_NUM)) {
        printf ("Please decrise precision, num of intervals or integral range!!!\n");
        printf ("Unable to find h, undefined output is comming ...\n");
    }

    return h;
}

void* init_interval(void* args) {
    init_interval_t* init = (init_interval_t*)args;
    Queue_t* queue        = init->queue;

    while (empty(queue) == QUEUE_BUSY) {
        interval_t* interv = (interval_t*)pop(queue);
        if (interv) {
            interv->h = get_h(interv->left_p, interv->right_p, init->func);
        }
    }

    pthread_exit(NULL);
    return NULL;
}


interval_t* create_intervals(double left_p, double right_p, integrate_function func) {
    interval_t* intervals = CALLOC(INTERVALS_NUM, interval_t);
    double width = (right_p - left_p) / INTERVALS_NUM;

    pthread_t threads[THREADS_NUM];
    Queue_t queue = {};
    init_queue(&queue);

    for (size_t i = 0; i < INTERVALS_NUM; i ++) {
        intervals[i].left_p  = left_p + i * width;
        intervals[i].right_p = left_p + (i + 1) * width;
        push(&queue, intervals + i);
    }

    init_interval_t init = {};
    init.queue = &queue;
    init.func  = func;
    for (size_t thread = 0; thread < THREADS_NUM; thread++)
        pthread_create(&threads[thread], NULL, init_interval, &init);

    for (size_t thread = 0; thread < THREADS_NUM; thread ++) 
        pthread_join(threads[thread], NULL);

    erase_queue(&queue);

    return intervals;
}

void print_intervals(interval_t* intervals) {
    for (size_t i = 0; i < INTERVALS_NUM; i ++) {
        printf("Interval [%lu] left border: %lf\n", i, intervals[i].left_p);
        printf("Interval [%lu] h step value: %lf\n", i, intervals[i].h);
    }

    return;
}

void delete_intervals(interval_t* intervals) {
    FREE(intervals);

    return;
}

size_t get_work_size(interval_t* intervals) {
    size_t work_size = 0;

    for (size_t i = 0; i < INTERVALS_NUM; i ++) {
        work_size += ((intervals[i].right_p - intervals[i].left_p) / intervals[i].h);
    }

    return lround(work_size / THREADS_NUM);
}

double get_work(double left_p, size_t work_size, interval_t* intervals) {
    size_t i = 0;
    double right_p = left_p;

    while (intervals[i].right_p < left_p && i < INTERVALS_NUM)
        i ++;
    if (i >= INTERVALS_NUM) {
        printf("Coudnt find interval, begin is out of integral range!!!\n");
        return -1;
    }

    double h_step = intervals[i].h;

    while (work_size > 0) {
        right_p += h_step;

        if (right_p > intervals[i].right_p && i < INTERVALS_NUM - 1)
            i ++;
            h_step = intervals[i].h;

        work_size -= 1;
    }

    return right_p;
}

work_t* create_work(interval_t* intervals) {
    work_t* work = CALLOC(THREADS_NUM, work_t);

    size_t work_size = get_work_size(intervals);

    double left_p = intervals[0].left_p;
    double right_p = left_p;

    for (size_t thread = 0; thread < THREADS_NUM - 1; thread ++) {
        work[thread].left_p  = left_p;
        work[thread].right_p = get_work(left_p, work_size, intervals);

        left_p = work[thread].right_p;
    }

    work[THREADS_NUM - 1].left_p  = left_p;
    work[THREADS_NUM - 1].right_p = intervals[INTERVALS_NUM - 1].right_p;

    return work;
}

void delete_work(work_t* work) {
    FREE(work);
}