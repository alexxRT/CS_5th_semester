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
    float** gradient;
}result_t;

typedef struct pairs_array_ {
    size_t*     array;
    size_t   size;
    size_t   pairs_num;
}pairs_t;

typedef float (*function_t)(float, float, float);
typedef float (*function_init_t)(float, float);

typedef struct equation_ {
    //velocity vector
    function_t u[DIM_SIZE];
    // current phi value, zero on a field when initilized
    result_t* phi;

    //initial function for t = 0 , y = 0
    function_init_t phi_x0;
    //initial function for t = 0 , x = 0
    function_init_t phi_y0;

    float time_step;
    float time;
}equation_t;

//self written data structure to parallel more convinient
pairs_t* create_pairs(size_t grid_size);
void print_pairs(pairs_t* pairs);
void delete_pairs(pairs_t* pairs);

equation_t* create_equation(size_t grid_size, float grid_step, float time_step, function_t u_x, function_t u_y,\
                             function_init_t phi_x0, function_init_t phi_y0);
void        delete_equation(equation_t* equation);

void dump_result(const char* file_path, const char* mode, equation_t* equation);

#endif //IO_H