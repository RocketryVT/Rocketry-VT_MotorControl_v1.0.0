#include <iostream>
#include <chrono>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <bitset>

#include "XBee_IO.h"
#include "config.h"
#include "control.h"
#include "Transmission.h"
#include "Hardware.h"
#include "Assert.h"

std::deque<unsigned char> input_buff, output_buff;
std::ifstream XBee;
const std::string xbee_filepath = "xbee.bin";
bool fail_flag = false;

bool XBeeIO::init()
{
    XBee.open(xbee_filepath);
    if (!XBee)
    {
        #ifdef DEBUG
        std::cout << "Failed to open xbee file: \""
            << xbee_filepath << "\"" << std::endl;
        #endif
        fail_flag = true;
        return false;
    }
    return true;
}

bool XBeeIO::ok()
{
    return !fail_flag;
}

/**
 * Recieves input from the XBee and saves it to "input_buff"
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the input_buffer was updated
 */
bool XBeeIO::update_input_buffer()
{
    size_t rptr = XBee.tellg();
    XBee.seekg(0, std::ios::end);
    size_t end = XBee.tellg();
    if (rptr > end) rptr = 0;
    XBee.seekg(rptr);

	char char_in;
	while (rptr < end) 
    {
        char_in = XBee.get();
		input_buff.push_back(char_in);
        ++rptr;
	}
	return true;
}

// queues a single char onto the output buffer
void XBeeIO::transmit(unsigned char data)
{
    output_buff.push_back(data);
}

// queues a vector of chars onto the output buffer
void XBeeIO::transmit(std::vector<unsigned char> data)
{
    for (auto e : data) output_buff.push_back(e);
}

// queues a string onto the output buffer
void XBeeIO::transmit(const std::string& str)
{
    for (auto e : str) output_buff.push_back(e);
}

/**
 * Transmits data over the XBee antenna
 * 
 * INPUT
 * int type -> data packet to transmit
 */
void XBeeIO::transmit_data(unsigned int type)
{
	using namespace Transmission;

	// Build packet and transmit it
	auto packet = buildPacket(type);
    for (auto e : packet)
        output_buff.push_back(e);
	return;
}

void XBeeIO::flush()
{
    auto size = output_buff.size();
    while (output_buff.size())
    {
        auto e = output_buff.front();
        output_buff.pop_front();
        Hardware::write(e);
    }
}

void XBeeIO::parse()
{
    auto packets = Transmission::parse(input_buff);
    for (auto p : packets)
    {
        std::vector<unsigned char> data;

        std::cout << Transmission::packet2str(p) << std::endl;

        for (size_t i = 4; i < p.size() - 2; ++i)
        {
            data.push_back(p[i]);
        }
        Transmission::dataReceipt(p[3], data);
    }
}

// clears input and output buffers
void XBeeIO::reset()
{
    input_buff.clear();
    output_buff.clear();
}

/**
 * Displays hex values of the buffer to the XBee port
 */
void XBeeIO::dispbuff()
{
    #ifdef DEBUG
    std::cout << "INPUT (" << input_buff.size() << "):";
    for (auto e : input_buff)
        std::cout << " " << std::hex << (int) e;
    std::cout << " | ";
    for (auto e : input_buff) {
        if (std::isprint(e))
            std::cout << e;
        else
            std::cout << ".";
    }
    std::cout << " |";
    std::cout << std::endl;
    std::cout << "OUTPUT (" << output_buff.size() << "):";
    for (auto e : output_buff)
        std::cout << " " << std::hex << (int) e;
    std::cout << " | ";
    for (auto e : output_buff) {
        if (std::isprint(e))
            std::cout << e;
        else
            std::cout << ".";
    }
    std::cout << " |";
    std::cout << std::endl;
    #endif
}

