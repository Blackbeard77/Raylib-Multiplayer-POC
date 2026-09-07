#include "ServerNetwork.h"

int main()
{
    ServerNetwork network;

    if (!network.Start(7777))
        return 1;

    while (true)
    {
        network.Update();
    }

    return 0;
}