#ifndef CALCULATE_H
#define CALCULATE_H

#include "io.h"

// we support three types of counting net
// TRIANGLE:       CROSS:          CROSS_HALFED:
//     **            **                **       
//  ** **         ** ** **          ** ** **    
//                   **                         
typedef enum net_types_ {
    TRIANGLE     = 1,
    CROSS        = 2,
    CROSS_HALFED = 3
} net_types_t;

void solve_on_grid(equation_t* equation, net_types_t net);

void evaluate_triangle(equation_t* equation) {return;};
void evaluate_cross(equation_t* eqaution) {return;};
void evaluate_cross_halfed(equation_t* equation) {return;};



#endif // CALCULATE_H