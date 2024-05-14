#ifndef QUEUE_STANDARD_H
#define QUEUE_STANDARD_H
#include <cmath>
#include <stdio.h>
#include <cfloat>

// #define DEBUG_MODE
#ifdef DEBUG_MODE
#define debug_print(...) fprintf(stderr, __VA_ARGS__)
#endif
#ifndef DEBUG_MODE
#define debug_print(...)
#endif

// #define PREEMPTION_UNINTERRUPTIBLE

typedef long double real;
#define infinity LDBL_MAX
typedef unsigned int jclass;
#define ln logl

#define DONE_TOLERANCE 0.00000001

#endif
