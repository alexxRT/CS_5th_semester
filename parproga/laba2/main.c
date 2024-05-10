#include "thread_work.h"
#include "thread_calculate.h"
#include "run_config.h"
#include "memory.h"
#include <pthread.h>
#include <stdio.h>

// integration function
float f(float x) {
    return sin(1 / x);
}

int main() {
    pthread_t threads[THREADS_NUM];

    interval_t*    intervals   = create_intervals(0.001, 0.002, f);
    work_t*        work        = create_work(intervals);
    thread_data_t* thread_work = CALLOC(THREADS_NUM, thread_data_t);

    assign_work(thread_work, intervals, work);

    for (size_t thread = 0; thread < THREADS_NUM; thread ++)
        pthread_create(&threads[thread], NULL, evaluate_integral, (void*)&thread_work[thread]);

    for (size_t thread = 0; thread < THREADS_NUM; thread ++)
        pthread_join(threads[thread], NULL);

    //sum among thread args results
    print_result(thread_work);

    delete_intervals(intervals);
    delete_work(work);
    FREE(thread_work);

    return 0;
}