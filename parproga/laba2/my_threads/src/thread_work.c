#include <math.h>
#include <stdlib.h>
#include "thread_work.h"
#include "method.h"
#include "memory.h"


float get_h(float left_p, float right_p, integrate_function func) {
    //first approximation, take simply one tenth of the interval
    float h = (right_p - left_p) / 10;
    float error;

    do {
        error = fabs(method(left_p, right_p, h / 2, func) \
         - method(left_p, right_p, h, func));

         h /= 2;
    }
    while (error > (PRECISION / INTERVALS_NUM));

    return h;
}


interval_t* create_intervals(float left_p, float right_p, integrate_function func) {
    interval_t* intervals = CALLOC(INTERVALS_NUM, interval_t);
    float width = (right_p - left_p) / INTERVALS_NUM;

    for (size_t i = 0; i < INTERVALS_NUM; i ++) {
        intervals[i].left_p  = left_p + i * width;
        intervals[i].right_p = left_p + (i + 1) * width;
        intervals[i].h       = get_h(intervals[i].left_p, intervals[i].right_p, func);
    }

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

float get_work(float left_p, size_t work_size, interval_t* intervals) {
    size_t i = 0;
    float right_p = left_p;

    while (intervals[i].right_p < left_p && i < INTERVALS_NUM)
        i ++;
    if (i >= INTERVALS_NUM) {
        printf("Coudnt find interval, begin is out of integral range!!!\n");
        return -1;
    }

    float h_step = intervals[i].h;

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

    float left_p = intervals[0].left_p;
    float right_p = left_p;

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