#include "myRay.h"

#include "common.h"

// TODO: Найти график с самой большой амплитудой и/или маленьким шагом.
// Пока основным считается график 0
void DrawAxes(ThreadData *data, double scale, Vector2 offset,
              double amplitude) {
    int axisX_start = (int)offset.x;
    int axisX_end = data->window.width;
    int axisY = (int)(offset.y + (data->window.height / 2));
    // Ось X
    DrawLine(axisX_start, axisY, axisX_end, axisY, BLACK);
    // Ось Y
    DrawLine(axisX_start, 0, axisX_start, data->window.height, BLACK);

    // Метки на оси X
    for (size_t i = 0;
         i < data->window.width / scale + (int)fabs(offset.x) / scale;
         i += data->graph[0].step / 10) {
        int x = (int)(i * scale) + (int)offset.x;
        DrawText(TextFormat("%.2f", (double)i / data->graph[0].step), x,
                 axisY + 5, 10, BLACK);
    }

    // Метки на оси Y
    for (int j = -amplitude; j <= amplitude; j += 1) {
        int y = (int)(axisY -
                      j * (scale * (data->window.height / (2 * amplitude))));
        DrawText(TextFormat("%d", j), axisX_start + 5, y, 10, BLACK);
        DrawLine(axisX_start - 5, y, axisX_start + 5, y, BLACK);
    }
}

// TODO: Сейчас рисуются всек графики из структуры. Нужно разделить на рисование
// каждого отдельного.
void DrawGraph(ThreadData *data, int num, Color _color, double scale,
               Vector2 offset) {
    size_t prev_x = 0;
    double prev_y = 0;

    // График
    for (size_t i = 0;
         (i < data->window.width / scale + (int)fabs(offset.x) / scale) &&
         (i < data->graph[num].size);
         i++) {
        int x = (int)(i * scale) + (int)offset.x;

        int y = (int)(offset.y + (data->window.height / 2) -
                      (data->graph[num].array[i] /
                       data->graph[num].wave.amplitude) *
                          (scale * (data->window.height / 2)));

        if (i > 0) {
            DrawLine(prev_x, prev_y, x, y, _color);
        }

        prev_x = x;
        prev_y = y;
    }
}

void *win_main(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    const int screenWidth = data->window.width;
    const int screenHeight = data->window.height;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: MSAA 4X

    InitWindow(screenWidth, screenHeight, "raylib");

    bool space = false;
    int curr_index = 0;
    SetTargetFPS(60);
    double scale = 1.0;           // Масштаб
    Vector2 offset = {0, 0};      // Смещение
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mouseDelta = GetMouseDelta();
            offset.x += mouseDelta.x;
            offset.y += mouseDelta.y;
        }

        double temp_scale = GetMouseWheelMove();

        if (fabs(temp_scale) > 0.01) {
            double distance_from_one = fabs(scale - 1.0);
            double divisor = 20.0;  // Базовый делитель
            divisor += distance_from_one / 10.0;

            scale += temp_scale / divisor;
            scale = fmax(scale, 0.01);
            scale = floor(scale * 100) / 100;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (space) {
                space = false;
            } else {
                space = true;
            }
        }
        BeginDrawing();
        //---------------------------------------------------------------------
        ClearBackground(RAYWHITE);

        if (space) {
            curr_index = 1;
            DrawAxes(data, scale, offset, data->graph[1].wave.amplitude);
            DrawGraph(data, 1, BLUE, scale, offset);
        } else {
            curr_index = 0;
            DrawAxes(data, scale, offset, data->graph[0].wave.amplitude);
            DrawGraph(data, 0, RED, scale, offset);
        }

        DrawText(TextFormat("FPS: %d", GetFPS()), 40, 150, 20, MAROON);

        // TODO: Вынести в отдельную функцию.
        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= 0 && mousePos.x < data->window.width) {
            size_t index = (size_t)((mousePos.x - offset.x) / scale);
            if (index < data->graph[curr_index].size) {
                DrawText(
                    TextFormat("X: %.3f", index / data->graph[curr_index].step),
                    10, 10, 20, BLACK);
                DrawText(
                    TextFormat("Y: %.3f", data->graph[curr_index].array[index]),
                    150, 10, 20, BLACK);
                DrawText(TextFormat("Scale: %.2f", scale), 10, 30, 20, BLACK);
            }
        }
        //---------------------------------------------------------------------
        EndDrawing();
    }

    // De-Init

    CloseWindow();
    return NULL;
}