#include "ServerNetwork.h"

#include <iostream>

ServerNetwork::ServerNetwork()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "[SERVER] Failed to initialize ENet.\n";
    }
}

ServerNetwork::~ServerNetwork()
{
    if (server != nullptr)
    {
        enet_host_destroy(server);
    }

    enet_deinitialize();
}

bool ServerNetwork::Start(std::uint16_t port)
{
    ENetAddress address{};

    address.host = ENET_HOST_ANY;
    address.port = port;

    server = enet_host_create(
        &address,
        32,
        2,
        0,
        0
    );

    if (server == nullptr)
    {
        std::cerr << "[SERVER] Failed to create ENet server.\n";
        return false;
    }

    std::cout
        << "[SERVER] Listening on port "
        << port
        << ".\n";

    return true;
}

void ServerNetwork::Update()
{
    if (server == nullptr)
        return;

    ENetEvent event{};

    while (enet_host_service(server, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
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

                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                // We will handle gameplay packets here next.

                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                std::cout << "[SERVER] Client disconnected.\n";
                break;
            }

            default:
                break;
        }
    }
}