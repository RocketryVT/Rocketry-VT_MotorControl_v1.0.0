#include <iostream>
#include <iomanip>
#include <fstream>
#include <deque>
#include <chrono>

#include <transmission.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "usage: readin <filename>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile)
    {
        std::cout << "Failed to open "
            << std::string(argv[1]) << std::endl;
        return 2;
    }

    std::map<uint8_t, std::function<
        std::string(std::vector<uint8_t>)>> translations{
    { 130, [] (std::vector<uint8_t> data)
    {
        uint64_t millis;
        uint8_t status;
        float o2p, o2t, cp, ct, nh, thrust;
        data >> millis >> status >> o2p
            >> o2t >> cp >> ct >> nh >> thrust;
        std::stringstream ss;
        ss << millis << " " << (int) status << " " << o2p << " " << o2t
            << " " << cp << " " << ct << " " << nh << " " << thrust;
        return ss.str();
    }},

    { 131, [] (std::vector<uint8_t> data)
    {
        uint64_t millis;
        data >> millis;
        std::stringstream ss;
        ss << millis;
        return ss.str();
    }}};

    auto start = std::chrono::steady_clock::now();
    std::deque<uint8_t> buffer;
    while (true)
    {
        size_t rptr = infile.tellg();
        infile.seekg(0, std::ios::end);
        size_t end = infile.tellg();
        
        if (rptr > end)
            infile.seekg(0, std::ios::beg);
        else infile.seekg(rptr);

	    while (rptr < end) 
        {
            char char_in = infile.get();
		    buffer.push_back(char_in);
            ++rptr;
	    }

        auto packets = transmission::parse(buffer);
        for (auto p : packets)
        {
            auto now = std::chrono::steady_clock::now();
            uint64_t millis = std::chrono::duration_cast
                <std::chrono::milliseconds>(now - start).count();
            std::cout << std::setw(11) << std::left
                << std::fixed << std::setprecision(2)
                << millis/1000.0 << std::dec
                << transmission::packet2str(p, translations)
                << std::endl;
        }
    }

    return 0;
}
