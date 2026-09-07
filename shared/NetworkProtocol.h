#pragma once

#include <cstdint>

// Every network message starts with one of these values so the receiver
// knows how to interpret the bytes that follow.
enum class PacketType : std::uint8_t
{
    Welcome,
    PlayerInput
};

// Sent by the server immediately after a client connects.
struct WelcomePacket
{
    PacketType type = PacketType::Welcome;
    std::uint32_t playerId = 0;
};

// Describes what the player wants to do.
// The server will decide how that input affects the actual game state.
struct PlayerInputPacket
{
    PacketType type = PacketType::PlayerInput;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
};
