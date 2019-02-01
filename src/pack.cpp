#include <iostream>
#include <string>
#include <vector>

#include <Transmission.h>

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        std::string message = std::string(argv[1]);
        uint8_t len = (uint8_t) message.length();

        std::vector<uint8_t> packet { 0xAA, 0x14, len, '#' };
        for (auto e : message)
            packet.push_back(e);
        Transmission::appendChecksum(packet);

        for (auto e : packet)
            std::cout << e;
    }
    else
    {
        std::vector<uint8_t> packet = { 0xAA, 0x14, 0x00, 0x06, 0xAA, 0x12 };
        Transmission::appendChecksum(packet);
        for (auto e : packet)
            std::cout << e;
    }
    return 0;
}
