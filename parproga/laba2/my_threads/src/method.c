#include "method.h"
#include <math.h>

float method(float left_p, float right_p, float h, integrate_function func) {
    size_t n = roundf((right_p - left_p) / h);

    float integral = (func(left_p) + func(right_p)) / 2;

    float sum = 0;
    for (size_t i = 0; i < n; i ++) {
        sum += func(left_p + i*h);
    }

    integral += sum;
    integral *= h;

    return integral;
}

// integration function declaration
float f(float x) {
    return sin(1 / x);
}