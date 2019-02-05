#include <iostream>
#include <transmission.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "usage: bin2txt \"log/LOG-01.bin\"" << std::endl;
        return 1;
    }

    auto packets = transmission::fromFile(argv[1]);

    for (auto p : packets)
    {
        std::cout << transmission::packet2str(p) << std::endl;
    }

    return 0;
}
