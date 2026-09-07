#include <iostream>
#include <enet/enet.h>
#include "NetworkProtocol.h"

int main()
{
    // ENet has to be initialized once before any networking objects are created.
    if (enet_initialize() != 0)
    {
        std::cerr << "[SERVER] Failed to initialize ENet.\n";
        return 1;
    }

    ENetAddress address{};

    // ENET_HOST_ANY means "listen on all local network interfaces".
    address.host = ENET_HOST_ANY;
    address.port = 7777;

    // Create a server that can accept up to 32 clients.
    // The final two zeros mean we are not limiting upload/download bandwidth.
    ENetHost* server = enet_host_create(
        &address,
        32,
        2,
        0,
        0
    );

    if (server == nullptr)
    {
        std::cerr << "[SERVER] Failed to create ENet server.\n";
        enet_deinitialize();
        return 1;
    }

    std::cout << "[SERVER] Listening on port 7777.\n";

    ENetEvent event{};

    std::uint32_t nextPlayerId = 1;

    // Keep processing network events while the server is running.
    while (true)
    {
        while (enet_host_service(server, &event, 1000) > 0)
        {
            if (event.type == ENET_EVENT_TYPE_CONNECT)
            {
                const std::uint32_t playerId = nextPlayerId++;

                std::cout
                    << "[SERVER] Client connected. Assigned Player ID: "
                    << playerId
                    << '\n';

                WelcomePacket welcome{};
                welcome.playerId = playerId;

                ENetPacket* packet = enet_packet_create(
                    &welcome,
                    sizeof(welcome),
                    ENET_PACKET_FLAG_RELIABLE
                );

                enet_peer_send(event.peer, 0, packet);
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();

    return 0;
}