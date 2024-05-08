#ifndef METHOD_H
#define METHOD_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "run_config.h"

float method(float left_p, float right_p, float h, integrate_function func);

#endif