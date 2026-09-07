#include "ServerGame.h"

void ServerGame::AddPlayer(std::uint32_t playerId)
{
    players[playerId] = PlayerState{};
}

void ServerGame::RemovePlayer(std::uint32_t playerId)
{
    players.erase(playerId);
}

void ServerGame::ApplyInput(
    std::uint32_t playerId,
    const PlayerInputPacket& input
)
{
    const auto playerIt = players.find(playerId);

    if (playerIt == players.end())
        return;

    PlayerState& player = playerIt->second;

    constexpr float movementAmount = 3.0f;

    if (input.up)
        player.y -= movementAmount;

    if (input.down)
        player.y += movementAmount;

    if (input.left)
        player.x -= movementAmount;

    if (input.right)
        player.x += movementAmount;
}