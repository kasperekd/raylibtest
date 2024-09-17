#include "myCalc.h"

#include "common.h"

#define PI 3.14159265358979323846

// TODO: Нужно принудительно завершать поток при поступлении кода возврата от
// окна
void *calc_main(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (size_t i = 0; i < data->graph.size; i++) {
        // double t = (double)i / 10000;
        double t = (double)i / data->graph.step;
        data->graph.array[i] = (double)(data->graph.wave.amplitude *
                                        sin(2 * PI * data->graph.wave.f * t +
                                            data->graph.wave.phi));
    }
    return NULL;
}