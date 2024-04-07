#include <iostream>
#include <stdio.h>

#include "io.h"
#include "memory.h"

//this lib provides engine to evaluate equation on a net
// all functions of type evaluate_net_type()
#include "calculate.h"

void dump_result(const char* file_path, equation_t* equation) {
    FILE* file = fopen(file_path, "w");
    if (!file) {
        std::cout << "Can not dump results!";
        return;
    }

    fprintf(file, "%lu %f %d\n", result->size, result->grid_step, GRID_START);

    for (size_t x_i = 0; x_i < result->size; x_i ++) {
        for (size_t y_j = 0; y_j < result->size; y_j ++) {
            fprintf(file, "%f ", result->grid_val[x_i][y_j]);
        }
        fprintf(file, "\n");
    }

    return;
}

result_t* create_result(size_t size, float grid_step) {
    // initing required memory
    result_t* result = CALLOC(1, result_t);
    result->grid_val = CALLOC(size, float*);
    for (int i = 0; i < size; i ++)
        result->grid_val[i] = CALLOC(size, float);

    result->size = size;
    result->grid_step = grid_step;

    return result;
}

void delete_result(result_t* result) {
    size_t size = result->size;

    for (size_t i = 0; i < size; i ++)
        FREE(result->grid_val[i]);

    FREE(result->grid_val);
    FREE(result);
}

// here continuity equation is inited
equation_t* create_equation(size_t grid_size, float grid_step, \
                            function_t u_x, function_t u_y, function_init_t phi_0) {
    equation_t* equation = CALLOC(1, equation_t);
    if (!equation) {
        std::cout << "Failed to create equation!\n" << "calloc returned NULL\n";
        return nullptr;
    }

    equation->u[0]  = u_x;
    equation->u[1]  = u_y;
    equation->phi_0 = phi_0;

    // two dimentional space, where equation is being solved
    equation->phi = create_result(grid_size, grid_step);

    return equation
};

void destroy_equation(equation_t* equation) {
    delete_result(equation->phi)
    FREE(equation);

    return;
}