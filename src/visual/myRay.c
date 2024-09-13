#include "myRay.h"

#include "common.h"

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
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        if (itter >= data->size) {
            itter = 0;
        }

        // Update
        UpdateMusicStream(music);

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
        size_t prev_x = 0;
        double prev_y = 0;
        for (size_t i = 0; i < itter; i++) {
            // DrawPixel(data->array[i], i, RED);
            // DrawLine((size_t)prev_x, prev_y, (size_t)data->array[i], i, RED);
            DrawLine(prev_x, 150 + prev_y, i, 150 + (size_t)data->array[i],
                     RED);
            prev_x = i;
            prev_y = data->array[i];
        }

        DrawText(TextFormat("FPS: %d", GetFPS()), 40, 150, 20, MAROON);
        // DrawText(TextFormat("data: %lu", data->array[799]), 40, 200, 20,
        //          MAROON);
        DrawText(TextFormat("data: %f", data->array[itter]), 40, 200, 20,
                 MAROON);

        EndDrawing();
        itter++;
    }

    // De-Initi
    UnloadMusicStream(music);

    CloseAudioDevice();

    CloseWindow();
    return NULL;
}