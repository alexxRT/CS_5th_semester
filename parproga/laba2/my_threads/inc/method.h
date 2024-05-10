#ifndef METHOD_H
#define METHOD_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "run_config.h"

double method(double left_p, double right_p, double h, integrate_function func);

#endif