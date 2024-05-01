#include "mpi.h"
#include "memory.h"
#include "io.h"
#include "equation.h"
#include "calculate.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_task_triangle(mpi_task_t* task, size_t* pair, equation_t* equation) {
    task->time = equation->time;
    task->point[0] = pair[0];
    task->point[1] = pair[1];
    task->phi[0]   = equation->phi->grid_val[pair[0]][pair[1]];
    task->phi[1]   = equation->phi->grid_val[pair[0] - 1][pair[1]];
    task->phi[2]   = equation->phi->grid_val[pair[0]][pair[1] - 1];
}

void init_task_centered(mpi_task_t* task, size_t* pair, equation_t* equation) {
    task->time = equation->time;
    task->point[0] = pair[0];
    task->point[1] = pair[1];
    task->phi[0]   = equation->phi->grid_val[pair[0]][pair[1]];
    task->phi[1]   = equation->phi->grid_val[pair[0] - 1][pair[1] - 1];
    task->phi[2]   = equation->phi->grid_val[pair[0] - 1][pair[1] + 1];
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

// on binary call specify max_x or max_y value and x/y_step
int main (int argc, char** argv) {
    if (argc < 4) {
        printf("Calculations have never been started! Lack of arguments!\n");
        printf("Need 3, provided %d\n", argc - 1);

        return -1;
    }

    //mpi initialization
    MPI_Init(&argc, &argv);

    // equation initialization
    size_t grid_size  = atol(argv[1]);
    float  grid_step  = atof(argv[2]);
    const char* dump_file = argv[3];
    float  time_step  = grid_step / 10; 

    function_t u[] = {u_x, u_y};

    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    equation_t* equation = create_equation(grid_size, grid_step, time_step, u_x, u_y, phi_x0, phi_y0);
    pairs_t*    pairs    = create_pairs(grid_size);

    //initing data types to send over proccesses:
    MPI_Datatype mpi_task   = create_mpi_task_type();

    clock_t start, end;

    // processes' borders
    size_t id_left  = 0;
    size_t id_right = 0;

    id_left  = rank * grid_size / size;
    id_right = (rank + 1) * grid_size / size;

    if (rank == size - 1)
        id_right = grid_size;

    int* recvcount = (int*)CALLOC(size, int);
    int* displs    = (int*)CALLOC(size, int);

    recvcount[rank] = id_right - id_left;
    displs[rank]    = id_left;

    MPI_Allgather(recvcount + rank, 1, MPI_INT, recvcount, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(displs + rank, 1, MPI_INT, displs, 1, MPI_INT, MPI_COMM_WORLD);

    start = clock();

    for (int iter = 0; iter < 1000; iter ++) {
        if (rank == 0) {
            dump_result(dump_file, "a", equation);
            for (size_t layer = 1; layer < pairs->pairs_num; layer ++) {
                for (size_t i = id_left + 1; i < id_right; i ++) {
                    size_t x = pairs[layer].array[2 * i];
                    size_t y = pairs[layer].array[2 * i + 1];

                    mpi_task_t task = {};
                    if (i == id_right - 1) {
                        init_task_triangle(&task, pairs[layer].array + 2 * i, equation);
                        equation->phi->gradient[x][y] = gradient_triangle(&task, grid_step, u);
                    }
                    else {
                        init_task_centered(&task, pairs[layer].array + 2 * i, equation);
                        equation->phi->gradient[x][y] = gradient_centered(&task, grid_step, u);
                    }
                }
            }
        }
        else {
            for (size_t layer = 1; layer < pairs->pairs_num; layer ++) {
                for (size_t i = id_left; i < id_right; i ++) {
                    size_t x = pairs[layer].array[2 * i];
                    size_t y = pairs[layer].array[2 * i + 1];

                    mpi_task_t task = {};
                    if (i == id_right - 1) {
                        init_task_triangle(&task, pairs[layer].array + 2 * i, equation);
                        equation->phi->gradient[x][y] = gradient_triangle(&task, grid_step, u);
                    }
                    else {
                        init_task_centered(&task, pairs[layer].array + 2 * i, equation);
                        equation->phi->gradient[x][y] = gradient_centered(&task, grid_step, u);
                    }
                }
            }
        }

        update_result(equation);

        //share results
        for (size_t layer = 1; layer < grid_size; layer ++) {
            float* recv_buf = equation->phi->grid_val[layer];
            float* send_buf = recv_buf + id_left;

            size_t num = id_right - id_left;

            MPI_Allgatherv(send_buf, num, MPI_FLOAT, recv_buf, recvcount, displs, MPI_FLOAT, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        end = clock();
        printf("Calculation Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
        // delete_equation(equation);
    }

    delete_equation(equation);
    delete_pairs(pairs);
    FREE(recvcount);
    FREE(displs);

    MPI_Finalize();

    return 0;
}