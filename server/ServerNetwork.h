#pragma once

#include <cstdint>
#include <unordered_map>
#include <enet/enet.h>
#include "NetworkProtocol.h"
#include "ServerGame.h"

class ServerNetwork
{
public:
    ServerNetwork();
    ~ServerNetwork();

    bool Start(std::uint16_t port);
    void Update();

private:
    ENetHost* server = nullptr;
    std::uint32_t nextPlayerId = 1;
    ServerGame game;

    // Maps each connected ENet peer to the player ID assigned by the server.
    std::unordered_map<ENetPeer*, std::uint32_t> playerIds;
};