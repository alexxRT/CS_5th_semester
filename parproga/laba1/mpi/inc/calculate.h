#ifndef CALCULATE_H
#define CALCULATE_H

#include "io.h"

// we support two types of counting net
// TRIANGLE:               CROSS_HALFED:
//     **                       **       
//  ** **                    ** ** **    
//                                       
typedef enum net_types_ {
    TRIANGLE     = 1,
    CROSS        = 2,
    CROSS_HALFED = 3
} net_types_t;

typedef struct task_ {
    float time;
    float phi[3];
    size_t point[2];
}mpi_task_t;

typedef struct task_result_ {
    float gradient;
    size_t x;
    size_t y;
}task_result_t;


float gradient_centered(mpi_task_t* task, float grid_step, function_t u[]);
float gradient_triangle    (mpi_task_t* task, float grid_step, function_t u[]);

void update_result(equation_t* equation);



#endif // CALCULATE_H