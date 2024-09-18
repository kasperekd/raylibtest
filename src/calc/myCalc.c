#include "myCalc.h"

#include "common.h"

#define PI 3.14159265358979323846

// TODO: Нужно принудительно завершать поток при поступлении кода возврата от
// окна
void *calc_main(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    // TODO: Убрать хардкод индексов ареев
    for (size_t i = 0; i < data->graph[0].size; i++) {
        double t = (double)i / data->graph[0].step;
        data->graph[0].array[i] =
            (double)(data->graph[0].wave.amplitude *
                     sin(2 * PI * data->graph[0].wave.f * t +
                         data->graph[0].wave.phi));
    }

    for (size_t i = 0; i < data->graph[1].size; i++) {
        double t = (double)i / data->graph[1].step;
        data->graph[1].array[i] =
            (double)(data->graph[1].wave.amplitude *
                     sin(2 * PI * data->graph[1].wave.f * t +
                         data->graph[1].wave.phi));
    }

    return NULL;
}