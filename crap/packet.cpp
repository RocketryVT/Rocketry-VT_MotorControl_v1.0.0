#include <iostream>
#include <cstdint>
#include <array>
#include <vector>

#include <Transmission.h>

namespace transmission
{

const uint8_t SIZEOF_SYNC_BYTES = 3;
const std::array<uint8_t, SIZEOF_SYNC_BYTES> sync_bytes { 'V', 'T', 'R' };

template <std::string name, uint8_t id, uint8_t dataLength> class packet
{
    const static uint8_t length = SIZEOF_SYNC_BYTES + 3 + dataLength;
    class dataType = std::array<uint8_t, dataLength>;

    std::array<uint8_t, dataLength> data;

    public:

    packet(const dataType &arr) : data(arr) { };

    std::vector<uint8_t> toVector()
    {
        std::vector<uint8_t> bytes;
        for (auto e : sync_bytes)
            bytes.push_back(e);
        bytes.push_back(id);
        for (auto e : data)
            bytes.push_back(e);
        Transmission::appendChecksum(bytes);
        return bytes;
    }

    void write(std::ostream& os)
    {
        for (auto e : toVector())
            os << e;
    }

    static void read(std::ostream& os)
    {
    }
};



} // namespace transmission

int main()
{
    transmission::packet<'#', 5> pack({ 'H', 'e', 'l', 'l', 'o' });
    pack.write(std::cout);

    transmission::packet<'#', 3> pack2({ 'A', 'B', 'C' });
    pack2.write(std::cout);

    return 0;
}
