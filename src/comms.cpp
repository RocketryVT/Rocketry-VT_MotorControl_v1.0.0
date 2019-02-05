#include <iostream>
#include <chrono>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <bitset>

#include <comms.h>
#include <config.h>
#include <control.h>
#include <transmission.h>
#include <hardware.h>
#include <Assert.h>

std::deque<unsigned char> input_buff, output_buff;
std::ifstream txstream;
const std::string txfilepath = "in.bin";
bool fail_flag = false;

bool comms::init()
{
    txstream.open(txfilepath);
    if (!txstream)
    {
        #ifdef DEBUG
        std::cout << "Failed to open comms device: \""
            << txfilepath << "\"" << std::endl;
        #endif
        fail_flag = true;
        return false;
    }
    return true;
}

bool comms::ok()
{
    return !fail_flag;
}

void comms::loop()
{
    size_t rptr = txstream.tellg();
    txstream.seekg(0, std::ios::end);
    size_t end = txstream.tellg();
    if (rptr > end) rptr = 0;
    txstream.seekg(rptr);

	while (rptr < end) 
    {
        char char_in = txstream.get();
		input_buff.push_back(char_in);
        ++rptr;
	}

    auto packets = transmission::parse(input_buff);
    for (auto p : packets)
    {
        std::vector<unsigned char> data;
        for (size_t i = 4; i < p.size() - 2; ++i)
            data.push_back(p[i]);
        transmission::dataReceipt(p[3], data);
    }
}

// clears input and output buffers
void comms::reset()
{
    input_buff.clear();
    output_buff.clear();
}

// queues a vector of chars onto the output buffer
void comms::transmit(std::vector<unsigned char> data)
{
    for (auto e : data) output_buff.push_back(e);
}

// queues a string onto the output buffer
void comms::transmit(const std::string& str)
{
    for (auto e : str) output_buff.push_back(e);
}

void comms::flush()
{
}


