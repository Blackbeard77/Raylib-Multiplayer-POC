// Prevent Windows headers pulled in by ENet from defining APIs that
// collide with raylib names such as Rectangle, CloseWindow and DrawText.
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#include <enet/enet.h>
#include "raylib.h"
#include "NetworkProtocol.h"

int main() {
    constexpr int screenWidth = 1280;
    constexpr int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Raylib Multiplayer POC");
    SetTargetFPS(60);

    // ENet must be initialized before creating any network host.
    if (enet_initialize() != 0) {
        CloseWindow();
        return 1;
    }

    // Client host:
    // - nullptr because we are not listening on a specific local address
    // - 1 outgoing connection
    // - 2 communication channels
    ENetHost *client = enet_host_create(
        nullptr,
        1,
        2,
        0,
        0
    );

    if (client == nullptr) {
        enet_deinitialize();
        CloseWindow();
        return 1;
    }

    ENetAddress serverAddress{};

    // Connect to this machine.
    enet_address_set_host(&serverAddress, "127.0.0.1");
    serverAddress.port = 7777;

    ENetPeer *serverPeer = enet_host_connect(
        client,
        &serverAddress,
        2,
        0
    );

    std::uint32_t playerId = 0;

    bool connected = false;

    while (!WindowShouldClose()) {
        ENetEvent event{};

        // Poll network events without blocking the game loop.
        while (enet_host_service(client, &event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                connected = true;
            }
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                // Make sure the packet is large enough before interpreting its bytes.
                if (event.packet->dataLength >= sizeof(WelcomePacket)) {
                    const auto *welcome =
                            reinterpret_cast<const WelcomePacket *>(event.packet->data);

                    if (welcome->type == PacketType::Welcome) {
                        playerId = welcome->playerId;
                    }
                }

                // ENet owns received packets until we explicitly release them.
                enet_packet_destroy(event.packet);
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

        DrawText(
            TextFormat("PLAYER ID: %u", playerId),
            40,
            90,
            20,
            RAYWHITE
        );

        EndDrawing();
    }

    if (serverPeer != nullptr) {
        enet_peer_disconnect(serverPeer, 0);
    }

    enet_host_destroy(client);
    enet_deinitialize();

    CloseWindow();

    return 0;
}
