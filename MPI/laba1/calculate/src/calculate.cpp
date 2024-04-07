#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "memory.h"
#include "io.h"


void solve_on_grid(equation_t* equation, net_types_t net) {
    switch (net) {
        case TRIANGLE:
            return evaluate_triangle(equation);
            break;
        case CROSS:
            return evaluate_cross(equation);
            break;
        case CROSS_HALFED:
            return evaluate_cross_halfed(equation);
            break;
        default:
            std::cout << "Unknown net type! " << net << " Please check your input!"
            return;
    }
};
