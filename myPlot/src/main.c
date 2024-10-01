#include "common.h"
#include "myCalc.h"
#include "myRay.h"

// TODO: Более тонкая настройка разрешения графика

// TODO: Вынести в atexit
void freeThreadData(ThreadData *threadData) {
    if (threadData != NULL) {
        if (threadData->graph != NULL) {
            for (size_t i = 0; i < threadData->num_of_graph; i++) {
                if (threadData->graph[i].array != NULL) {
                    free(threadData->graph[i].array);
                    threadData->graph[i].array = NULL;
                }
            }
            free(threadData->graph);
            threadData->graph = NULL;
        }
        free(threadData);
    }
}

int main(void) {
    pthread_t visual;
    pthread_t calc;

    WinData window;
    window.height = 600;
    window.width = 800;

    ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
    assert(data != NULL);
    data->window = window;
    data->num_of_graph = 2;  // количество хранимых графов
    assert(data->num_of_graph > 1);

    data->graph = (GraphData *)malloc(sizeof(GraphData) * data->num_of_graph);
    assert(data->graph != NULL);

    // Инициализация графиков
    for (size_t i = 0; i < data->num_of_graph; i++) {
        data->graph[i].step = 1000.0;
        data->graph[i].size = (size_t)(data->graph[i].step * 1);
        data->graph[i].array = NULL;
    }

    // Первый график
    data->graph[0].wave.amplitude = 6;
    data->graph[0].wave.f = 9.0;
    data->graph[0].wave.phi = (double)(PI / 5);

    data->graph[1].wave.amplitude = 6;
    data->graph[1].wave.f = 18.0;
    data->graph[1].wave.phi = (double)(PI);

    // Выделяем память для массивов array
    data->graph[0].array =
        (double *)malloc(sizeof(double) * data->graph[0].size);
    assert(data->graph[0].array != NULL);

    data->graph[1].array =
        (double *)malloc(sizeof(double) * data->graph[1].size);
    assert(data->graph[1].array != NULL);

    printf("\n%ld\n", data->graph[0].size);

    pthread_create(&visual, NULL, win_main, data);  // Отрисовка
    pthread_create(&calc, NULL, calc_main, data);   // Вычисления

    sleep(1);
    // Задание
    printf("\n2)Maximum frequency in signal: %.3f\n", data->graph[0].wave.f);
    printf("3)Minimum sampling rate: %.3f\n", data->graph[0].wave.f * 2);

    pthread_join(visual, NULL);  // Отрисовка
    pthread_join(calc, NULL);    // Вычисления

    // cleanup
    freeThreadData(data);
    return 0;
}