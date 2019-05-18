#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include <transmission.h>

int main()
{
    srand(std::chrono::steady_clock::now().time_since_epoch().count());
    while (true)
    {
        uint8_t len = rand() % 256;
        uint8_t id = rand() % 256;
        std::vector<uint8_t> data;
        for (size_t i = 0; i < len; ++i)
            data.push_back(rand() % 256);

        auto packet = transmission::buildPacket(transmission::getChannel(id), data);

        // mess up a random byte in 1/5 packets
        if (rand() % 100 < 20)
        {
            size_t index = rand() % packet.size();
            packet[index] = rand() % 256;
        }

        for (auto e : packet)
            std::cout << e;
    }
    return 0;
}
