#include "myCalc.h"

#include "common.h"

// TODO: Нужно принудительно завершать поток при поступлении кода возврата от
// окна
void *calc_main(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (size_t i = 0; data->array[data->size - 1] < 0xffffffffffffffff; i++) {
        for (size_t j = 0; j < data->size; j++) {
            // sleep(1);
            data->array[j] = j * i;
        }
        sleep(5);
    }

    return NULL;
}