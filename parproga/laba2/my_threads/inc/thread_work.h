#ifndef THREAD_WORK_H
#define THREAD_WORK_H

#include "run_config.h"

typedef struct interval_ {
    float left_p;
    float right_p;
    float h;
}interval_t;

typedef struct work_ {
    float left_p;
    float right_p;
}work_t;

interval_t* create_intervals(float left_p, float right_p, integrate_function func);
void print_intervals (interval_t* intervals);
void delete_intervals(interval_t* intervals);

work_t* create_work(interval_t* intervals);
void    delete_work(work_t* work);

#endif


