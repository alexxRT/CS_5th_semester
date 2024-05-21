#include "io.h"
#include "mpi.h"
#include "equation.h"
#include "calculate.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Lack of arguments! Need 3, provided " << argc - 1 << "\n";
        return -1;
    }

    size_t grid_size = atol(argv[1]);
    float grid_step  = atof(argv[2]);
    const char* dump_file = argv[3];
    float time_step  = grid_step / 10;

    FILE* clear = fopen(dump_file, "w");
    if (clear) 
        fclose(clear);

    equation_t* equation = create_equation(grid_size, grid_step, time_step, u_x, u_y, phi_x0, phi_y0);
    pairs_t*    pairs    = create_pairs(grid_size);

    function_t u[] = {u_x, u_y};

    clock_t start, end;

    start = clock();

    for (size_t iter = 0; iter < 1000; iter ++) {
        dump_result(dump_file, "a", equation);
        for (size_t layer = 1; layer < pairs->pairs_num; layer ++) {
            for (size_t i = 1; i < pairs[layer].size; i ++) {
                size_t x = pairs[layer].array[2 * i];
                size_t y = pairs[layer].array[2 * i + 1];

                mpi_task_t task = {};
                init_task_triangle(&task, pairs[layer].array + 2*i, equation);
                equation->phi->gradient[x][y] = gradient_triangle(&task, grid_step, u);
            }
        }
        update_result(equation);
    }

    end = clock();
    printf("Calculation Time: %lf sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    delete_equation(equation);
    delete_pairs(pairs);

    return 0;
}