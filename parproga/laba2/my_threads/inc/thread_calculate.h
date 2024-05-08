#ifndef THREAD_CALCULATE_H
#define THREAD_CALCULATE_H

#include "thread_work.h"
#include "run_config.h"



typedef struct thread_data_ {
    work_t* work;
    interval_t* intervals;
    integrate_function func;
    float result;
    double eval_time;
}thread_data_t;

void* evaluate_integral(void* arg);
void assign_work(thread_data_t* thread_work, interval_t* intervals, work_t* work);
void print_result(thread_data_t* result);

#endif