#include "myCalc.h"

#include "common.h"

// TODO: Нужно принудительно завершать поток при поступлении кода возврата от
// окна
#define PI 3.14159265358979323846
void *calc_main(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (size_t i = 0; i < data->size; i++) {
        double t = i;
        data->array[i] = (double)(100 * sin(2 * PI * 0.01 * t + (PI)));
        sleep(1);
    }
    return NULL;
}