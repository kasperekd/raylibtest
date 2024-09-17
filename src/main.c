#include "common.h"
#include "myCalc.h"
#include "myRay.h"

// TODO: Более тонкая настройка разрешения графика

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
    data.graph.size = data.graph.step * 1;

    printf("\n%ld\n", data.graph.size);

    data.graph.wave.amplitude = 3.4;
    data.graph.wave.f = 9.0;
    data.graph.wave.phi = (double)(PI / 5);

    // TODO Обработать исключения
    data.graph.array = (double *)malloc(sizeof(double) * data.graph.size);

    pthread_create(&visual, NULL, win_main, &data);  // Отрисовка
    pthread_create(&calc, NULL, calc_main, &data);   // Вычисления

    sleep(1);
    // Задание
    printf("\n2)Maximum frequency in signal: %.3f\n", data.graph.wave.f);
    printf("3)Minimum sampling rate: %.3f\n", data.graph.wave.f * 2);
    //

    pthread_join(visual, NULL);  // Отрисовка
    pthread_join(calc, NULL);    // Вычисления

    free(data.graph.array);
    return 0;
}