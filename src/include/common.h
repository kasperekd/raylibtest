#ifndef COMMON_H
#define COMMON_H

#include <malloc.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    // size_t *array;
    double *array;
    size_t size;
} ThreadData;

#endif  // !COMMON_H
