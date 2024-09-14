#include "myCalc.h"

#include "common.h"

// TODO: Нужно принудительно завершать поток при поступлении кода возврата от
// окна
#define PI 3.14159265358979323846
void *calc_main(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (size_t i = 0; i < data->graph.size; i++) {
        // double t = data->graph.step * i;
        double t = (double)i / data->graph.step;
        data->graph.array[i] = (double)(3 * cos(2 * PI * 12 * t + (PI / 8)));
    }
    return NULL;
}