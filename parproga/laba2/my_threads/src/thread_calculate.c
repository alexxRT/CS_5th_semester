
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "thread_calculate.h"
#include "method.h"


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

    double a = work->left_p;
    double b = work->right_p;

    double b_curr = fmin(intervals[i].right_p, b);
    double integral = 0;

    clock_t start = clock();
    while (a < b) {
        integral += method(a, b_curr, intervals[i].h, func);

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

void assign_work(thread_data_t* thread_work, interval_t* intervals, work_t* work) {
    for (size_t thread = 0; thread < THREADS_NUM; thread ++) {
        thread_work[thread].func      = f;
        thread_work[thread].intervals = intervals;
        thread_work[thread].work      = &work[thread];

        // printf("I am thread [%lu], my work [%f, %f]\n", thread, work[thread].left_p, work[thread].right_p);
    }
}

void print_result(thread_data_t* result) {
    double integral = 0;
    for (size_t i = 0; i < THREADS_NUM; i ++) {
        integral += result[i].result;
        printf("I am thread [%lu]. My eval time = %lf\n", i, result[i].eval_time);
    }

    printf ("Integral value = %.15f\n", integral);
}