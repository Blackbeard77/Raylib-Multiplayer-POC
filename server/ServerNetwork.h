#pragma once

#include <cstdint>

#include <enet/enet.h>

#include "NetworkProtocol.h"

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
};