#include <iostream>
#include <Transmission.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "usage: bin2txt \"log/LOG-01.bin\"" << std::endl;
        return 1;
    }

    auto packets = Transmission::fromFile(argv[1]);

    for (auto p : packets)
    {
        std::cout << Transmission::packet2str(p) << std::endl;
    }

    return 0;
}
