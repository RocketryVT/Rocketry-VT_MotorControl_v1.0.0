#include <iostream>
#include <chrono>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <bitset>

#include <comms.h>
#include <logging.h>
#include <config.h>
#include <control.h>
#include <transmission.h>
#include <behavior.h>
#include <hardware.h>
#include <Assert.h>

namespace comms
{

std::deque<unsigned char> input_buff, output_buff;
std::ifstream in;
std::ofstream out;
const std::string rxfilepath = "in.bin";
const std::string txfilepath = "out.bin";
bool fail_flag = false;

bool init()
{
    in.open(rxfilepath);
    if (!in)
    {
        logging::announce("Failed to open rx comms device: \"" +
            rxfilepath + "\"", true, true);
        fail_flag = true;
        return false;
    }
    in.seekg(0, std::ios::end);

    logging::announce("Opened rx comms device: " + rxfilepath, true, true);

    out.open(txfilepath);
    if (!out)
    {
        logging::announce("Failed to open tx comms device: \"" +
            txfilepath + "\"", true, true);
        fail_flag = true;
        return false;
    }

    logging::announce("Opened tx comms device: " + txfilepath, true, true);

    return true;
}

bool ok()
{
    return !fail_flag;
}

void loop()
{
    size_t rptr = in.tellg();
    in.seekg(0, std::ios::end);
    size_t end = in.tellg();
    in.seekg(rptr);

	while (rptr < end) 
    {
        char char_in = in.get();
		input_buff.push_back(char_in);
        ++rptr;
	}

    auto packets = transmission::parse(input_buff);
    for (auto p : packets)
    {
        std::vector<unsigned char> data;
        for (size_t i = 4; i < p.size() - 2; ++i)
            data.push_back(p[i]);
        behavior::dataReceipt(p[3], data);
    }
}

// clears input and output buffers
void reset()
{
    input_buff.clear();
    output_buff.clear();
}

// queues a vector of chars onto the output buffer
void transmit(const std::vector<unsigned char> &data)
{
    for (auto e : data) output_buff.push_back(e);
}

// queues a string onto the output buffer
void transmit(const std::string& str)
{
    transmit(transmission::buildPacket(str));
}

void flush()
{
    while (output_buff.size() > 0)
    {
        out << output_buff.front();
        output_buff.pop_front();
    }
    out.flush();
}

} // namespace comms

