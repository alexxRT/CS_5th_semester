#ifndef RUN_CONFIG
#define RUN_CONFIG

#include <math.h>

// can be specified on build with -D option
#ifndef PRECISION
    #define PRECISION 0.00001
#endif

// specified num of intervals with different h step value
#ifndef INTERVALS_NUM
    #define INTERVALS_NUM 10
#endif

#ifndef THREADS_NUM
    #define THREADS_NUM 8
#endif

typedef float (*integrate_function)(float x);
extern  float f(float x);

#endif