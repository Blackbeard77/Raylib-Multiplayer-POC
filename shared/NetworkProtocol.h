#pragma once

#include <cstdint>

// Every network message starts with one of these values so the receiver
// knows how to interpret the bytes that follow.
enum class PacketType : std::uint8_t
{
    Welcome
};

// Sent by the server immediately after a client connects.
struct WelcomePacket
{
    PacketType type = PacketType::Welcome;
    std::uint32_t playerId = 0;
};
