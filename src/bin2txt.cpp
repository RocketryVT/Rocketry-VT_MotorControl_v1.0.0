#include <iostream>
#include <iomanip>
#include <transmission.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "usage: bin2txt \"log/LOG-01.bin\"" << std::endl;
        return 1;
    }
    int logs = -1;
    if (argc == 3)
    {
        logs = std::stoi(argv[2]);
    }

    auto packets = transmission::fromFile(argv[1]);

    int count = 0;
    for (auto p : packets)
    {
        if (logs < 0 || count >= (int) packets.size() - logs)
            std::cout << std::hex << std::setfill('0')
                << std::setw(4) << count << "    "
                << transmission::packet2str(p) << std::endl;
        ++count;
    }

    return 0;
}
