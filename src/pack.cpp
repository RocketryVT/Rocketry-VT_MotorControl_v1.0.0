#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <transmission.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        while (true)
        {
            std::cout << " (pack) " << std::flush;
            std::string input;
            std::getline(std::cin, input);
            system(("./bin/pack " + input + " >> in.bin").c_str());
        }
    }

    std::string channel(argv[1]);
    std::vector<uint8_t> data;
    for (int i = 2; i < argc; ++i)
    {
        data.push_back(std::stoi(argv[i]));
    }
    auto packet = transmission::buildPacket(channel, data);
    for (auto e : packet)
        std::cout << e;
    return 0;
}
