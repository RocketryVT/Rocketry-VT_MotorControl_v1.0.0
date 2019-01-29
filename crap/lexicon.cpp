#include <iostream>
#include <cstdint>
#include <map>
#include <utility>
#include <functional>
#include <vector>

int main()
{
    using id_len = std::pair<uint8_t, uint8_t>;
    using packet_action = std::function<bool(uint8_t id, const std::vector<uint8_t> &vec)>;
    std::map<id_len, packet_action> lexicon;

    lexicon[id_len(4, 5)] = [] (uint8_t id, const std::vector<uint8_t> &vec)
    {
        std::cout << "4 and 5!" << std::endl;
        return true;
    };
    lexicon[id_len(1, 2)] = [] (uint8_t id, const std::vector<uint8_t> &vec)
    {
        std::cout << "Numba one!" << std::endl;
        return true;
    };

    for (auto e : lexicon)
    {
        id_len pair = e.first;
        packet_action action = e.second;

        uint8_t id = pair.first;
        uint8_t len = pair.second;
        bool success = action(id, std::vector<uint8_t>(len));
    }

    return 0;
}
