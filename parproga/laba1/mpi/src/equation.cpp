#include "equation.h"
#include <math.h>

#define precision 0.00001

//staying waves on both borders
float phi_x0(float time, float x) {
    float omega = 1;
    return sin(x) * cos(omega * time);
}

float phi_y0(float time, float y) {
    float omega = 1;
    return sin(y) * cos(omega * time);
}

float u_x (float time, float x, float y) {
    return 1;
    float center_x = 0.5;
    float center_y = 0.5;

    if (fabs(x - center_x) < precision && fabs(y - center_y) < precision)
        return 0;

    return (x - center_x) / sqrtf(powf(x - center_x, 2) + powf(y - center_y, 2));
}

float u_y(float time, float x, float y) {
    return 1;
    float center_x = 0.5;
    float center_y = 0.5;

    if (fabs(x - center_x) < precision && fabs(y - center_y) < precision)
        return 0;

    return (y - center_y) / sqrtf(powf(x - center_x, 2) + powf(y - center_y, 2));
}
