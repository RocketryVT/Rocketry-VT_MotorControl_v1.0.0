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
#include <behavior.h>
#include <hardware.h>
#include <Assert.h>

namespace comms
{

/// Input and output buffers containing bytes waiting
/// to be sent to their fun homes.
std::deque<unsigned char> input_buff, output_buff;
/// The filestream from which bytes come and to
/// which bytes go.
std::ifstream txstream;
/// The name of the comms hardware device.
const std::string txfilepath = "in.bin";
/// The failure state of this namespace.
bool fail_flag = false;

bool init()
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

bool ok()
{
    return !fail_flag;
}

void loop()
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
void transmit(std::vector<unsigned char> data)
{
    for (auto e : data) output_buff.push_back(e);
}

// queues a string onto the output buffer
void transmit(const std::string& str)
{
    for (auto e : str) output_buff.push_back(e);
}

void flush()
{
}


