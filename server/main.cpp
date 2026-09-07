#include <iostream>
#include <enet/enet.h>

int main()
{
    // ENet has to be initialized once before any networking objects are created.
    if (enet_initialize() != 0)
    {
        std::cerr << "[SERVER] Failed to initialize ENet.\n";
        return 1;
    }

    ENetAddress address{};

    // ENET_HOST_ANY means "listen on all local network interfaces".
    address.host = ENET_HOST_ANY;
    address.port = 7777;

    // Create a server that can accept up to 32 clients.
    // The final two zeros mean we are not limiting upload/download bandwidth.
    ENetHost* server = enet_host_create(
        &address,
        32,
        2,
        0,
        0
    );

    if (server == nullptr)
    {
        std::cerr << "[SERVER] Failed to create ENet server.\n";
        enet_deinitialize();
        return 1;
    }

    std::cout << "[SERVER] Listening on port 7777.\n";

    // Keep the server alive for now.
    std::cin.get();

    enet_host_destroy(server);
    enet_deinitialize();

    return 0;
}