#include "myRay.h"

#include "common.h"

void DrawAxes(ThreadData *data, double scale, Vector2 offset) {
    const double amplitude = 3.0;  // TODO: нужно будет получать его из функции
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
         i += data->graph.step / 10) {
        int x = (int)(i * scale) + (int)offset.x;
        DrawText(TextFormat("%.2f", (double)i / data->graph.step), x, axisY + 5,
                 10, BLACK);
    }

    // Метки на оси Y
    for (int j = -amplitude; j <= amplitude; j += 1) {
        int y = (int)(axisY -
                      j * (scale * (data->window.height / (2 * amplitude))));
        DrawText(TextFormat("%d", j), axisX_start + 5, y, 10, BLACK);
        DrawLine(axisX_start - 5, y, axisX_start + 5, y, BLACK);
    }
}

void DrawGraph(ThreadData *data, double scale, Vector2 offset) {
    const double amplitude = 3.0;  // TODO: нужно будет получать его из функции
    size_t prev_x = 0;
    double prev_y = 0;

    // График
    for (size_t i = 0;
         i < data->window.width / scale + (int)fabs(offset.x) / scale; i++) {
        int x = (int)(i * scale) + (int)offset.x;
        int y = (int)(offset.y + (data->window.height / 2) -
                      (data->graph.array[i] / amplitude) *
                          (scale * (data->window.height / 2)));

        if (i > 0) {
            DrawLine(prev_x, prev_y, x, y, RED);
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

    InitAudioDevice();  // Initialize audio device

    Music music = LoadMusicStream("resources/music.wav");
    music.looping = false;
    float pitch = 1.0f;

    PlayMusicStream(music);

    float timePlayed = 0.0f;
    bool pause = false;

    SetTargetFPS(60);
    size_t itter = 0;
    double scale = 1.0;           // Масштаб
    Vector2 offset = {0, 0};      // Смещение
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        if (itter >= data->graph.size) {
            itter = 0;
        }

        // Update
        UpdateMusicStream(music);

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
            StopMusicStream(music);
            PlayMusicStream(music);
            pause = false;
        }

        if (IsKeyPressed(KEY_P)) {
            pause = !pause;

            if (pause)
                PauseMusicStream(music);
            else
                ResumeMusicStream(music);
        }

        if (IsKeyDown(KEY_DOWN))
            pitch -= 0.01f;
        else if (IsKeyDown(KEY_UP))
            pitch += 0.01f;

        SetMusicPitch(music, pitch);

        // Get timePlayed
        timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music) *
                     (screenWidth - 40);
        BeginDrawing();
        //---------------------------------------------------------------------
        ClearBackground(RAYWHITE);

        DrawRectangle(20, screenHeight - 20 - 12, screenWidth - 40, 12,
                      LIGHTGRAY);
        DrawRectangle(20, screenHeight - 20 - 12, (int)timePlayed, 12, MAROON);
        DrawRectangleLines(20, screenHeight - 20 - 12, screenWidth - 40, 12,
                           GRAY);

        // Отрисовка оси и графика
        DrawAxes(data, scale, offset);
        DrawGraph(data, scale, offset);
        // DrawGraph(data->array, data->size);

        DrawText(TextFormat("FPS: %d", GetFPS()), 40, 150, 20, MAROON);

        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= 0 && mousePos.x < data->window.width) {
            size_t index = (size_t)((mousePos.x - offset.x) / scale);
            if (index < data->graph.size) {
                DrawText(TextFormat("X: %.2f", data->graph.array[index]), 10,
                         10, 20, BLACK);
                DrawText(TextFormat("Y: %.2f", index / data->graph.step), 150,
                         10, 20, BLACK);
                DrawText(TextFormat("Scale: %.2f", offset.x), 10, 25, 20,
                         BLACK);
            }
        }
        //---------------------------------------------------------------------
        EndDrawing();
        itter++;
    }

    // De-Initi
    UnloadMusicStream(music);

    CloseAudioDevice();

    CloseWindow();
    return NULL;
}