#include "equation.h"
#include <math.h>

//staying waves on both borders
float phi_x0(float time, float x) {
    float omega = 1;
    return sin(x) * cos(omega * time);
}

float phi_y0(float time, float y) {
    float omega = 1;
    return -sin(y) * cos(omega * time);
}

float u_x (float time, float x, float y) {
    return 1;
}

float u_y(float time, float x, float y) {
    return 1;
}
