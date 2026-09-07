#include "raylib.h"

#include "ClientNetwork.h"

int main()
{
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Raylib Multiplayer POC");
    SetTargetFPS(60);

    ClientNetwork network;
    network.Connect("127.0.0.1", 7777);

    while (!WindowShouldClose())
    {
        network.Update();

        PlayerInputPacket input{};

        input.up = IsKeyDown(KEY_W);
        input.down = IsKeyDown(KEY_S);
        input.left = IsKeyDown(KEY_A);
        input.right = IsKeyDown(KEY_D);

        network.SendInput(input);

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            network.IsConnected()
                ? "CONNECTED TO SERVER"
                : "CONNECTING...",
            40,
            40,
            30,
            network.IsConnected() ? GREEN : YELLOW
        );

        DrawText(
            TextFormat("PLAYER ID: %u", network.GetPlayerId()),
            40,
            90,
            20,
            RAYWHITE
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}