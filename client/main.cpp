#include "raylib.h"

int main()
{
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    // Create the game window and OpenGL context.
    InitWindow(screenWidth, screenHeight, "Raylib Multiplayer POC");

    // Keep the client update/render loop capped at 60 FPS for now.
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            "MULTIPLAYER POC",
            40,
            40,
            30,
            RAYWHITE
        );

        DrawCircle(
            screenWidth / 2,
            screenHeight / 2,
            30,
            RED
        );

        EndDrawing();
    }

    // Release the window and graphics resources before exiting.
    CloseWindow();

    return 0;
}