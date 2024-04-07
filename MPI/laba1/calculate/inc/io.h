#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>

// curent task requires solution in two dimentional space as W(x, y)
const int DIM_SIZE = 2;

const int GRID_START = 1;

typedef struct result_ {
    size_t  size;
    float   grid_step;
    float** grid_val;

}result_t;


typedef float (*function_t)(size_t, float, float);
typedef float (*function_init_t)(size_t);

typedef struct equation_ {
    //velocity vector
    function_t u[DIM_SIZE];
    // current phi value, zero on a field when initilized
    result_t* phi;
    //initial function for x = 0, y = 0
    function_init_t phi_0;
}equation_t;


equation_t* create_equation(size_t grid_size, float grid_step, function_t u_x, function_t u_y, function_init_t phi_0);
void        destroy_equation(equation_t* equation);

void dump_result(const char* file_path, equation_t* equation);

#endif //IO_H