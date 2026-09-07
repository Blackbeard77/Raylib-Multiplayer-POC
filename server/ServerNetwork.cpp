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
                playerIds[event.peer] = playerId;
                game.AddPlayer(playerId);

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
                if (event.packet->dataLength >= sizeof(PacketType))
                {
                    const auto packetType =
                        *reinterpret_cast<const PacketType*>(
                            event.packet->data
                        );

                    if (packetType == PacketType::PlayerInput &&
                        event.packet->dataLength >= sizeof(PlayerInputPacket))
                    {
                        const auto* input =
                            reinterpret_cast<const PlayerInputPacket*>(
                                event.packet->data
                            );

                        const auto playerIt =
                            playerIds.find(event.peer);

                        if (playerIt != playerIds.end())
                        {
                            // Pass player's input into the authorative game simulation.
                            game.ApplyInput(playerIt->second, *input);
                            std::cout
                                << "[SERVER] Player "
                                << playerIt->second
                                << " Input:"
                                << " W=" << input->up
                                << " S=" << input->down
                                << " A=" << input->left
                                << " D=" << input->right
                                << '\n';
                        }
                    }
                }

                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                const auto playerIt =
                    playerIds.find(event.peer);

                if (playerIt != playerIds.end())
                {
                    std::cout
                        << "[SERVER] Player "
                        << playerIt->second
                        << " disconnected.\n";

                    // Remove the player's authoritative state before forgetting the connection.
                    game.RemovePlayer(playerIt->second);
                    playerIds.erase(playerIt);
                }

                break;
            }

            default:
                break;
        }
    }
}