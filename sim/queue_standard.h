#ifndef QUEUE_STANDARD_H
#define QUEUE_STANDARD_H
#include <cmath>
#include <stdio.h>
#include <cfloat>

#define DEBUG_MODE
#ifdef DEBUG_MODE
#define debug_print printf
#endif
#ifndef DEBUG_MODE
#define debug_print(x);
#endif

typedef long double real;
#define infinity LDBL_MAX;
typedef unsigned int jclass;
#define ln logl

#endif
