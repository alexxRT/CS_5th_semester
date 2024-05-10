#include "method.h"
#include <math.h>

double method(double left_p, double right_p, double h, integrate_function func) {
    size_t n = roundf((right_p - left_p) / h);

    double integral = (func(left_p) + func(right_p)) / 2;

    double sum = 0;
    for (size_t i = 0; i < n; i ++) {
        sum += func(left_p + i*h);
    }

    integral += sum;
    integral *= h;

    return integral;
}