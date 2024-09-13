#include "myRay.h"

#include "common.h"

void DrawGraph(ThreadData *data, double scale, Vector2 offset) {
    size_t prev_x = 0;
    double prev_y = 0;

    for (size_t i = 0; i < data->size; i++) {
        int x = (int)(i * scale) + (int)offset.x;
        int y = (int)(150 + data->array[i] * scale) + (int)offset.y;

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

    // SetConfigFlags(FLAG_MSAA_4X_HINT);  // NOTE: MSAA 4X

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
        if (itter >= data->size) {
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

        ClearBackground(RAYWHITE);

        DrawRectangle(20, screenHeight - 20 - 12, screenWidth - 40, 12,
                      LIGHTGRAY);
        DrawRectangle(20, screenHeight - 20 - 12, (int)timePlayed, 12, MAROON);
        DrawRectangleLines(20, screenHeight - 20 - 12, screenWidth - 40, 12,
                           GRAY);

        // Draw help instructions
        // DrawRectangle(20, 20, 425, 145, WHITE);
        // DrawRectangleLines(20, 20, 425, 145, GRAY);
        // DrawText("PRESS SPACE TO RESTART MUSIC", 40, 40, 20, BLACK);
        // DrawText("PRESS P TO PAUSE/RESUME", 40, 70, 20, BLACK);
        // DrawText("PRESS UP/DOWN TO CHANGE SPEED", 40, 100, 20, BLACK);
        // DrawText(TextFormat("SPEED: %f", pitch), 40, 130, 20, MAROON);

        DrawGraph(data, scale, offset);

        // size_t prev_x = 0;
        // double prev_y = 0;
        // for (size_t i = 0; i < itter; i++) {
        //     // DrawPixel(data->array[i], i, RED);
        //     // DrawLine((size_t)prev_x, prev_y, (size_t)data->array[i], i,
        //     RED); DrawLine(prev_x, 150 + prev_y, i, 150 +
        //     (size_t)data->array[i],
        //              RED);
        //     prev_x = i;
        //     prev_y = data->array[i];
        // }

        DrawText(TextFormat("FPS: %d", GetFPS()), 40, 150, 20, MAROON);
        // DrawText(TextFormat("data: %lu", data->array[799]), 40, 200, 20,
        //          MAROON);
        // DrawText(TextFormat("data: %f", data->array[itter]), 40, 200, 20,
        //          MAROON);

        Vector2 mousePos = GetMousePosition();
        if (mousePos.x >= 0 && mousePos.x < data->window.width) {
            size_t index = (size_t)((mousePos.x - offset.x) / scale);
            if (index < data->size) {
                DrawText(TextFormat("Value: %.2f", data->array[index]), 10, 10,
                         20, BLACK);
                DrawText(TextFormat("Scale: %.2f", scale), 10, 25, 20, BLACK);
            }
        }

        EndDrawing();
        itter++;
    }

    // De-Initi
    UnloadMusicStream(music);

    CloseAudioDevice();

    CloseWindow();
    return NULL;
}