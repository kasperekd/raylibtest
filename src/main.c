#include "common.h"
#include "myCalc.h"
#include "myRay.h"

int main(void) {
    pthread_t visual;
    pthread_t calc;

    WinData window;
    window.height = 600;
    window.width = 800;

    ThreadData data;
    data.window = window;
    // data.graph.size = data.window.width;
    data.graph.step = 1000.0;
    data.graph.size = window.width * data.graph.step;
    // data.graph.step = 0.01;
    data.graph.wave.amplitude = 6.0;
    data.graph.wave.f = 9.0;
    data.graph.wave.phi = (double)(PI / 5);
    // TODO Обработать исключения
    // data.array = (size_t *)malloc(sizeof(size_t) * data.size);
    data.graph.array = (double *)malloc(sizeof(double) * data.graph.size);

    pthread_create(&visual, NULL, win_main, &data);  // Отрисовка
    pthread_create(&calc, NULL, calc_main, &data);   // Вычисления

    pthread_join(visual, NULL);  // Отрисовка
    pthread_join(calc, NULL);    // Вычисления

    free(data.graph.array);
    return 0;
}