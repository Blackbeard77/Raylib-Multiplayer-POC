#include "ClientNetwork.h"

#include <iostream>

ClientNetwork::ClientNetwork()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "[CLIENT] Failed to initialize ENet.\n";
        return;
    }

    client = enet_host_create(
        nullptr,
        1,
        2,
        0,
        0
    );

    if (client == nullptr)
    {
        std::cerr << "[CLIENT] Failed to create ENet client.\n";
    }
}

ClientNetwork::~ClientNetwork()
{
    if (serverPeer != nullptr)
    {
        enet_peer_disconnect(serverPeer, 0);
    }

    if (client != nullptr)
    {
        enet_host_destroy(client);
    }

    enet_deinitialize();
}

bool ClientNetwork::Connect(const char* host, std::uint16_t port)
{
    if (client == nullptr)
        return false;

    ENetAddress address{};

    if (enet_address_set_host(&address, host) != 0)
        return false;

    address.port = port;

    serverPeer = enet_host_connect(
        client,
        &address,
        2,
        0
    );

    return serverPeer != nullptr;
}

void ClientNetwork::Update()
{
    if (client == nullptr)
        return;

    ENetEvent event{};

    while (enet_host_service(client, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                connected = true;
                std::cout << "[CLIENT] Connected to server.\n";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
            {
                if (event.packet->dataLength >= sizeof(WelcomePacket))
                {
                    const auto* welcome =
                        reinterpret_cast<const WelcomePacket*>(event.packet->data);

                    if (welcome->type == PacketType::Welcome)
                    {
                        playerId = welcome->playerId;

                        std::cout
                            << "[CLIENT] Assigned Player ID: "
                            << playerId
                            << '\n';
                    }
                }

                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
                connected = false;
                serverPeer = nullptr;
                std::cout << "[CLIENT] Disconnected from server.\n";
                break;

            default:
                break;
        }
    }
}

void ClientNetwork::SendInput(const PlayerInputPacket& input)
{
    if (!connected || serverPeer == nullptr)
        return;

    ENetPacket* packet = enet_packet_create(
        &input,
        sizeof(input),
        0
    );

    enet_peer_send(serverPeer, 0, packet);
}

bool ClientNetwork::IsConnected() const
{
    return connected;
}

std::uint32_t ClientNetwork::GetPlayerId() const
{
    return playerId;
}