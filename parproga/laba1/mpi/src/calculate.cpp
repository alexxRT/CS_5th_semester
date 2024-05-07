#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "memory.h"
#include "io.h"
#include "equation.h"
#include "calculate.h"


//grad = phi*div(u) + u dot grad(phi)
float gradient_triangle(mpi_task_t* task, float grid_step, function_t u[]) {
    //init all points where to evaluate grad
    float time      = task->time;
    float x = task->point[0] * grid_step;
    float y = task->point[1] * grid_step;

    float phi_xy = task->phi[0];
    float phi_x  = task->phi[1];
    float phi_y  = task->phi[2];

    //div(u)
    float ux_x = (u[0](time, x + grid_step, y) - u[0](time, x, y)) / grid_step;
    float uy_y = (u[1](time, x, y + grid_step) - u[1](time, x, y)) / grid_step;

    float div_u = ux_x + uy_y;

    //gradient(phi) dot u
    float grad_phi_x = 0;
    float grad_phi_y = 0;

    grad_phi_x = (phi_xy - phi_x) / grid_step;
    grad_phi_y = (phi_xy - phi_y) / grid_step;

    float phi_product = u[0](time, x, y)*grad_phi_x + u[1](time, x, y)*grad_phi_y;

    //return gradient
    return div_u*phi_xy + phi_product;
}

float gradient_centered(mpi_task_t* task, float grid_step, function_t u[]) {
    float time = task->time;
    float x    = task->point[0] * grid_step;
    float y    = task->point[1] * grid_step;

    float phi_xy = task->phi[0]; 
    float phi_y  = task->phi[1];
    float phi_yy = task->phi[2];

    float ux_x = (u[0](time, x, y) - 0.5*(u[0](time, x - grid_step, y - grid_step) + u[0](time, x - grid_step, y + grid_step))) / grid_step;
    float uy_y = (u[1](time, x - grid_step, y + grid_step) - u[1](time, x - grid_step, y - grid_step)) / (2 * grid_step);

    float div_u = ux_x + uy_y;

    float grad_phi_x = 0;
    float grad_phi_y = 0;

    grad_phi_x = (phi_xy - 0.5*(phi_y + phi_yy)) / grid_step;
    grad_phi_y = (phi_yy - phi_y) / (2 * grid_step);

    float phi_product = u[0](time, x, y)*grad_phi_x + u[1](time, x, y)*grad_phi_y;

    return div_u*phi_xy + phi_product;
}

void update_result(equation_t* equation) {
    //update border function
    //then make it possible to set up phi_0 on all borders and inital values
    float time_step = equation->time_step;
    equation->time += time_step;

    //update border y = 0
    for (size_t x = 0; x < equation->phi->size; x ++) {
        float point_x = equation->phi->grid_step * x;
        equation->phi->grid_val[x][0] = equation->phi_x0(equation->time, point_x);
    }

    //update border x = 0
    for (size_t y = 0; y < equation->phi->size; y ++) {
        float point_y = equation->phi->grid_step * y;
        equation->phi->grid_val[0][y] = equation->phi_y0(equation->time, point_y);
    }

    //update inside grid
    for (size_t x = 0; x < equation->phi->size; x ++) {
        for (size_t y = 0; y < equation->phi->size; y ++) {
            equation->phi->grid_val[x][y] -= equation->phi->gradient[x][y]*time_step;
        }
    }

    return;
}