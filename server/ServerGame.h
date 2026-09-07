#pragma once

#include <cstdint>
#include <unordered_map>

#include "NetworkProtocol.h"

struct PlayerState
{
    float x = 640.0f;
    float y = 360.0f;
};

class ServerGame
{
public:
    void AddPlayer(std::uint32_t playerId);
    void RemovePlayer(std::uint32_t playerId);

    void ApplyInput(
        std::uint32_t playerId,
        const PlayerInputPacket& input
    );

private:
    std::unordered_map<std::uint32_t, PlayerState> players;
};