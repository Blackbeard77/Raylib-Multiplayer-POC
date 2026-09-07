#pragma once

#include <cstdint>

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#include <enet/enet.h>
#include "NetworkProtocol.h"
#include <unordered_map>

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
    std::unordered_map<ENetPeer*, std::uint32_t> playerIds;

    bool connected = false;
    std::uint32_t playerId = 0;
};