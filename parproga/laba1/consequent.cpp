#include "io.h"
#include "mpi.h"
#include "equation.h"
#include "calculate.h"
#include <iostream>


void init_task(mpi_task_t* task, size_t* pair, equation_t* equation) {
    task->time = equation->time;
    task->point[0] = pair[0];
    task->point[1] = pair[1];
    task->phi[0]   = equation->phi->grid_val[pair[0]][pair[1]];
    task->phi[1]   = equation->phi->grid_val[pair[0] - 1][pair[1]];
    task->phi[2]   = equation->phi->grid_val[pair[0]][pair[1] - 1];
}

MPI_Datatype create_mpi_task_type() {
    const int    nitems = 3;
    int          blocklengths[3] = {1, 3, 2};
    MPI_Datatype types[3] = {MPI_FLOAT, MPI_FLOAT, MPI_UNSIGNED_LONG};
    MPI_Datatype mpi_task;
    MPI_Aint     offsets[3];

    offsets[0] = offsetof(mpi_task_t, time);
    offsets[1] = offsetof(mpi_task_t, phi);
    offsets[2] = offsetof(mpi_task_t, point);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_task);
    MPI_Type_commit(&mpi_task);

    return mpi_task;
}

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
                init_task(&task, pairs[layer].array + 2*i, equation);
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