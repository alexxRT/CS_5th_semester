#include <iostream>
#include <stdio.h>

#include "io.h"
#include "memory.h"

//this lib provides engine to evaluate equation on a net
// all functions of type evaluate_net_type()
//#include "calculate.h"

void dump_result(const char* file_path, const char* mode, equation_t* equation) {
    FILE* file = fopen(file_path, mode);
    if (!file) {
        std::cout << "Can not dump results!";
        return;
    }

    result_t* result = equation->phi;
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

    result->gradient = CALLOC(size, float*);
    for (int i = 0; i < size; i ++)
        result->gradient[i] = CALLOC(size, float);

    result->size = size;
    result->grid_step = grid_step;

    return result;
}

void delete_result(result_t* result) {
    size_t size = result->size;

    for (size_t i = 0; i < size; i ++)
        FREE(result->grid_val[i]);

    for (size_t i = 0; i < size; i ++)
        FREE(result->gradient[i]);
    
    FREE(result->gradient);
    FREE(result->grid_val);
    FREE(result);
}

// here continuity equation is inited
equation_t* create_equation(size_t grid_size, float grid_step, float time_step, \
                            function_t u_x, function_t u_y, function_init_t phi_x0, function_init_t phi_y0) {
    equation_t* equation = CALLOC(1, equation_t);
    if (!equation) {
        std::cout << "Failed to create equation!\n" << "calloc returned NULL\n";
        return nullptr;
    }

    equation->u[0]   = u_x;
    equation->u[1]   = u_y;
    equation->phi_x0 = phi_x0;
    equation->phi_y0 = phi_y0;


    // two dimentional space, where equation is being solved
    equation->time = 0;
    equation->time_step = time_step;
    equation->phi = create_result(grid_size, grid_step);

    //init border y = 0
    for (size_t x = 0; x < equation->phi->size; x ++) {
        float point_x = equation->phi->grid_step * x;
        equation->phi->grid_val[x][0] = equation->phi_x0(0, point_x);
    }

    //init border x = 0
    for (size_t y = 0; y < equation->phi->size; y ++) {
        float point_y = equation->phi->grid_step * y;
        equation->phi->grid_val[0][y] = equation->phi_y0(0, point_y);
    }
    
    return equation;
};

void delete_equation(equation_t* equation) {
    delete_result(equation->phi);
    FREE(equation);

    return;
}

pairs_t* create_pairs(size_t grid_size) {
    pairs_t* pairs = CALLOC(grid_size, pairs_t);
    pairs->pairs_num = grid_size;

    for (size_t i = 0; i < grid_size; i ++) {
        size_t x = i;
        size_t y = 0;

        pairs[i].array = CALLOC(2*grid_size, size_t);
        pairs[i].size = 0;

        size_t position = pairs[i].size;

        while (position < grid_size) {
            pairs[i].array[2*position] = x;
            pairs[i].array[2*position + 1] = y;

            y ++;
            position ++;
        }

        pairs[i].size = position;
    }

    return pairs;
}

void print_pairs(pairs_t* pairs) {
    printf("%lu\n", pairs->pairs_num);
    for (size_t i = 0; i < pairs->pairs_num; i ++) {
        for (size_t k = 0; k < pairs[i].size; k ++) {
            printf("[%lu, %lu] ", pairs[i].array[2 * k], pairs[i].array[2 * k + 1]);
        }
        printf("\n");
    }

    return;
}

void delete_pairs(pairs_t* pairs) {
    for (size_t i = 0; i < pairs->pairs_num; i ++) {
        FREE(pairs[i].array);
    }
    FREE(pairs);

    return;
}