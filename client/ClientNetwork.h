#pragma once

#include <cstdint>

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#include <enet/enet.h>

#include "NetworkProtocol.h"

class ClientNetwork
{
public:
    ClientNetwork();
    ~ClientNetwork();

    bool Connect(const char* host, std::uint16_t port);
    void Update();
    void SendInput(const PlayerInputPacket& input);

    bool IsConnected() const;
    std::uint32_t GetPlayerId() const;

private:
    ENetHost* client = nullptr;
    ENetPeer* serverPeer = nullptr;

    bool connected = false;
    std::uint32_t playerId = 0;
};