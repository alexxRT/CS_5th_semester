#include "io.h"
#include "calculate.h"

#include <math.h>


const size_t iteration_num = 1000;

float get_time_step(float time) {
    return time / iteration_num;
}


float phi_0(float time) {
    float omega = 0.1;
    return sin(omega*time);
}

float u_x (float time, float x, float y) {
    return 1;
}

float u_y(float time, float x, float y) {
    return 1;
}


int main() {
    equation_t* equation = create_equation(10, 0.001, u_x, u_y, phi_0);

    fprintf(stdin, "Please choose grid evaluation type:\n");
    net_types_t type;
    fscanf(stdin, "%d", &type);

    fprintf(stdin, "Please enter in seconds modeling time:\n");
    float time = 0.0;
    fscanf(stdin, "%f", &time);

    float t = 0.0;
    float tau = get_time_step(time);

    while (t < time) {
        solve_on_grid(equation, type);

        dump_result("dump.txt", equation);
        t += tau;
    }

    destroy_equation(equation);

    return 0;
}