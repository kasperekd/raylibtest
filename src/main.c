#include "common.h"
#include "myCalc.h"
#include "myRay.h"

int main(void) {
    pthread_t visual, calc;

    WinData window;
    window.height = 600;
    window.width = 800;

    ThreadData data;
    data.window = window;
    data.size = data.window.width;

    // TODO Обработать исключения
    // data.array = (size_t *)malloc(sizeof(size_t) * data.size);
    data.array = (double *)malloc(sizeof(double) * data.size);

    pthread_create(&visual, NULL, win_main, &data);  // Отрисовка
    pthread_create(&calc, NULL, calc_main, &data);   // Вычисления

    pthread_join(visual, NULL);  // Отрисовка
    pthread_join(calc, NULL);    // Вычисления

    free(data.array);
    return 0;
}