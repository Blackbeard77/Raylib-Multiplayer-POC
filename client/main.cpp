// Prevent Windows headers pulled in by ENet from defining APIs that
// collide with raylib names such as Rectangle, CloseWindow and DrawText.
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#include <enet/enet.h>
#include "raylib.h"
int main()
{
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Raylib Multiplayer POC");
    SetTargetFPS(60);

    // ENet must be initialized before creating any network host.
    if (enet_initialize() != 0)
    {
        CloseWindow();
        return 1;
    }

    // Client host:
    // - nullptr because we are not listening on a specific local address
    // - 1 outgoing connection
    // - 2 communication channels
    ENetHost* client = enet_host_create(
        nullptr,
        1,
        2,
        0,
        0
    );

    if (client == nullptr)
    {
        enet_deinitialize();
        CloseWindow();
        return 1;
    }

    ENetAddress serverAddress{};

    // Connect to this machine.
    enet_address_set_host(&serverAddress, "127.0.0.1");
    serverAddress.port = 7777;

    ENetPeer* serverPeer = enet_host_connect(
        client,
        &serverAddress,
        2,
        0
    );

    bool connected = false;

    while (!WindowShouldClose())
    {
        ENetEvent event{};

        // Poll network events without blocking the game loop.
        while (enet_host_service(client, &event, 0) > 0)
        {
            if (event.type == ENET_EVENT_TYPE_CONNECT)
            {
                connected = true;
            }
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            connected ? "CONNECTED TO SERVER" : "CONNECTING...",
            40,
            40,
            30,
            connected ? GREEN : YELLOW
        );

        EndDrawing();
    }

    if (serverPeer != nullptr)
    {
        enet_peer_disconnect(serverPeer, 0);
    }

    enet_host_destroy(client);
    enet_deinitialize();

    CloseWindow();

    return 0;
}