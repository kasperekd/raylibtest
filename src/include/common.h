#ifndef COMMON_H
#define COMMON_H

#include <malloc.h>
#include <pthread.h>
#include <unistd.h>

typedef struct WinData {
    size_t width;
    size_t height;
} WinData;

typedef struct GraphData {
    size_t size;
    double step;  // на еденицу X
    double *array;
} GraphData;

typedef struct ThreadData {
    WinData window;
    //------------//
    GraphData graph;  // При необходимости можно завести в массив
} ThreadData;

#endif  // !COMMON_H
