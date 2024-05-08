#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "memory.h"

typedef struct interval_ {
    float left_p;
    float right_p;
    float h;
}interval_t;


typedef struct work_ {
    float left_p;
    float right_p;
}work_t;


typedef float (*integrate_function)(float x);

typedef struct thread_data_ {
    work_t* work;
    interval_t* intervals;
    integrate_function func;
    float result;
    double eval_time;
}thread_data_t;


// can be specified on build with -D option
#ifndef PRECISION
    #define PRECISION 0.00001
#endif

// specified num of intervals with different h step value
#ifndef INTERVALS_NUM
    #define INTERVALS_NUM 10
#endif

#ifndef THREADS_NUM
    #define THREADS_NUM 8
#endif


float x_squared(float x) {
    return sin(1 / x);
}


float method_trapezei(float left_p, float right_p, float h, integrate_function func) {
    size_t n = roundf((right_p - left_p) / h);

    float integral = (func(left_p) + func(right_p)) / 2;

    float sum = 0;
    for (size_t i = 0; i < n; i ++) {
        sum += func(left_p + i*h);
    }

    integral += sum;
    integral *= h;

    return integral;
}


float get_h(float left_p, float right_p, integrate_function func) {
    //first approximation, take simply one tenth of the interval
    float h = (right_p - left_p) / 10;
    float error;

    do {
        error = fabs(method_trapezei(left_p, right_p, h / 2, func) \
         - method_trapezei(left_p, right_p, h, func));

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

void* evaluate_integral(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;

    work_t* work            = data->work;
    interval_t* intervals   = data->intervals;
    integrate_function func = data->func;

    size_t i = 0;
    while (intervals[i].right_p < work->left_p && i < INTERVALS_NUM)
        i ++;
    if (i >= INTERVALS_NUM) {
        printf("Bad val, evaluation out of range!!!\n");
        data->result = -1;
        pthread_exit(NULL);
    }

    float a = work->left_p;
    float b = work->right_p;

    float b_curr = fmin(intervals[i].right_p, b);
    float integral = 0;

    clock_t start = clock();
    while (a < b) {
        printf("evaluating..\n");
        integral += method_trapezei(a, b_curr, intervals[i].h, func);

        a = b_curr;
        i ++;
        if (i < INTERVALS_NUM)
            b_curr = fmin(intervals[i].right_p, b);
    }
    clock_t end = clock();

    data->result = integral;
    data->eval_time = (double)(end - start) / CLOCKS_PER_SEC;
    pthread_exit(NULL);
}



int main() {
    pthread_t threads[THREADS_NUM];

    interval_t*    intervals   = create_intervals(0.01, 0.02, x_squared);
    work_t*        work        = create_work(intervals);
    thread_data_t* thread_work = CALLOC(THREADS_NUM, thread_data_t);

    for (size_t thread = 0; thread < THREADS_NUM; thread ++) {
        thread_work[thread].func      = x_squared;
        thread_work[thread].intervals = intervals;
        thread_work[thread].work      = &work[thread];

        printf("I am thread [%lu], my work [%f, %f]\n", thread, work[thread].left_p, work[thread].right_p);
    }

    for (size_t thread = 0; thread < THREADS_NUM; thread ++) {
        pthread_create(&threads[thread], NULL, evaluate_integral, (void*)&thread_work[thread]);
    }

    for (size_t thread = 0; thread < THREADS_NUM; thread ++) {
        pthread_join(threads[thread], NULL);
    }

    //sum among thread args results
    float integral = 0;
    for (size_t i = 0; i < THREADS_NUM; i ++) {
        integral += thread_work[i].result;
        printf("I am thread [%lu]. My eval time = %lf\n", i, thread_work[i].eval_time);
    }

    printf ("Integral value = %lf\n", integral);

    delete_intervals(intervals);
    delete_work(work);
    FREE(thread_work);

    return 0;
}