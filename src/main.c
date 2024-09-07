#include "common.h"
#include "myCalc.h"
#include "myRay.h"

int main(void) {
    pthread_t visual, calc;

    ThreadData data;
    data.size = 800;
    // TODO Обработать исключения
    data.array = (size_t *)malloc(sizeof(size_t) * data.size);

    pthread_create(&visual, NULL, win_main, &data);  // Отрисовка
    pthread_create(&calc, NULL, calc_main, &data);   // Вычисления

    pthread_join(visual, NULL);  // Отрисовка
    pthread_join(calc, NULL);    // Вычисления

    free(data.array);
    return 0;
}