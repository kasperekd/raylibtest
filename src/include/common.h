#ifndef COMMON_H
#define COMMON_H

#include <malloc.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    size_t width;
    size_t height;
} WinData;

typedef struct {
    WinData window;
    //------------//
    size_t size;
    double *array;
} ThreadData;

#endif  // !COMMON_H
