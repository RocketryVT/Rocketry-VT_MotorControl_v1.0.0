#include <iostream>
#include <chrono>
#include <cstring>
#include <cctype>
#include <iomanip>

#include "XBee_IO.h"
#include "config.h"
#include "control.h"
#include "Transmission.h"
#include "Hardware.h"
#include "Assert.h"

std::deque<unsigned char> input_buff, output_buff;
std::fstream XBee;
const std::string xbee_filepath = "infile.txt";
bool fail_flag = false;

bool XBeeIO::init()
{
    XBee.open(xbee_filepath, std::ios::in | std::ios::out);
    if (!XBee)
    {
        return false;
        fail_flag = true;
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

	/* For debugging only */
	if (type == 0xFF)
    {
		transmit_data_string();
		return;
	}
	
	// Build packet and transmit it
	auto packet = buildPacket(type);
    for (auto e : packet)
        output_buff.push_back(e);
	// XBee.write((char*) output_buff, len);
	return;
}

void XBeeIO::flush()
{
    auto size = output_buff.size();
    while (output_buff.size())
    {
        auto e = output_buff.front();
        std::cout << std::hex << std::setw(2)
            << std::setfill('0') << (int) e << " ";
        output_buff.pop_front();
    }
    if (size) std::cout << std::dec << std::endl;
}

bool handle(const std::vector<unsigned char> &data);

bool XBeeIO::parse()
{
    std::vector<std::vector<unsigned char>> packets;

    bool parsing = true;

    while (input_buff.size() && parsing)
    {
        // go to next 0xAA 0x14
        while (input_buff.size() > 0 && input_buff.front() != 0xAA)
        {
            input_buff.pop_front();
        }

        if (input_buff.size() < 2)
        {
            continue;
        }

        // if the next byte isn't 0x14, pop the front and repeat
        if (input_buff[1] != 0x14)
        {
            input_buff.pop_front();
            continue;
        }

        if (input_buff.size() < 3) // length byte hasn't arrived
        {
            parsing = false;
            continue;
        }

        auto data_length = input_buff[2];
        if (input_buff.size() < 3 + data_length + 2) // incomplete data
        {
            parsing = false;
            continue;
        }

        // at this point we know there exists an entire discrete packet
        // if there's a checksum error that doesn't matter so don't
        // go popping stuff off the deque just yet
        std::vector<unsigned char> packet;

        // exclude checksum for now
        for (unsigned char i = 0; i < 3 + data_length; ++i)
            packet.push_back(input_buff[i]);

        unsigned char c0 = input_buff[3 + data_length],
                      c1 = input_buff[4 + data_length];

        unsigned char c0_true, c1_true;
        Transmission::xorchecksum(packet, c0_true, c1_true);

        if (c0 != c0_true || c1 != c1_true) // shit, checksum error
        {
            std::cout << "Checksum error: got (" << std::hex
                << (int) c0 << ", " << (int) c1 << "), expected ("
                << (int) c0_true << ", " << (int) c1_true << ")"
                << std::endl;
            input_buff.pop_front();
            continue;
        }

        // hooray, no checksum error! add those digits to the packet
        // and start popping things off the deque
        packet.push_back(c0);
        packet.push_back(c1);
        packets.push_back(packet);

        for (unsigned char i = 0; i < packet.size(); ++i)
            input_buff.pop_front();
    }

    unsigned long long time = std::chrono::duration_cast<std::chrono::
        milliseconds>(cfg::TIME - cfg::START_TIME).count();

    for (auto p : packets)
    {
        std::vector<unsigned char> data;

        std::cout << std::dec << "New packet (" << time << "): [";
        for (unsigned char i = 0; i < 3; ++i)
        {
            std::cout << " " << std::hex << std::setfill('0')
                << std::setw(2) << (int) p[i];
        }
        std::cout << " ] [";
        for (unsigned char i = 3; i < p[2] + 3; ++i)
        {
            std::cout << " " << std::hex << std::setfill('0')
                << std::setw(2) << (int) p[i];
            data.push_back(p[i]);
        }
        std::cout << " ] [";
        for (unsigned char i = p[2] + 3; i < p.size(); ++i)
        {
            std::cout << " " << std::hex << std::setfill('0')
                << std::setw(2) << (int) p[i];
        }
        std::cout << " ]" << std::endl;

        handle(data);
    }
}

/** 
 * Parses the "input_buff" character array
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the buffer was parsed succesfully
 */
bool handle(const std::vector<unsigned char> &data)
{
    using namespace cfg;
    using namespace State_Data;
    using namespace XBeeIO;

    unsigned char type = data[0];
    bool ascii = false;

    switch (type)
    {
        case 0x00: transmit_data(0x00); // echo firmware
                   break;
        
        case 0x01: Hardware::turn_LED_on(); // turn LED on
                   break;

        case 0x02: Hardware::turn_LED_off(); // turn LED off
                   break;

        // why would we need to clear the input buffer?
        case 0x03: input_buff.clear(); // clear the input buffer
                   break;

        case 0x04: if (data.size() < 2) break;
                   MODE = data[1]; // set mode
                   transmit_data(0x10);
                   break;

        case 0x05: transmit_data(0xB0); // run unit tests
                   break;

        case 0x10: Hardware::openStepperMotor(); // open motor
                   break;

        case 0x11: Hardware::closeStepperMotor(); // close motor
                   break;

                   // set parameters
        case 0x20: // cfg::data_period = data[1] + (data[2] << 8);
                   // max_time = data[3];
                   break;

        case 0x21: // XBee.print("DATA_PERIOD: "); // print params
                   // XBee.print(data_period_ms);
                   // XBee.print(" ms\n");
                   // XBee.print("MAX_TIME: ");
                   // XBee.print(max_time);
                   // XBee.print(" s\n");
                   break;

                   // 0x23 - '#'
        case 0x23: ascii = true; // parse ascii message
                   break;

        case 0x36: if (data.size() < 2) break; // edit params
                   DATA_OUT_TYPE = data[1];
                   if (DATA_OUT_TYPE == 0x10)
                       transmit_data_string();
                   break;

        case 0x44: dispbuff(); // display buffers
                   break;

                   // simulation packet
        case 0x50: if (data.size() < 9) break;
                   TIME = std::chrono::steady_clock::now();
                   DATA_OUT_TYPE    = data[0];
                   MODE             = data[1];
                   STATUS           = data[2];
                   DATA_P1          = data[3];
                   DATA_P2          = data[4];
                   DATA_T1          = data[5];
                   DATA_T2          = data[6];
                   DATA_THR         = data[7];
                   NEW_DATA         = data[8];
                   break;

        case 0xFF: control::reset(); // soft reset
                   break;

        default:   std::cout << "It's free real estate: 0x"
                       << std::hex << (int) data[0] << std::endl;
                   return false;
    }
   
    if (!ascii) return true;

    std::string msg(data.begin(), data.end());
    std::cout << "Recieved new ASCII message: \""
        << msg << "\"" << std::endl;
 
    if (msg == "#LED ON")
    {
        Hardware::turn_LED_on();
    }
    else if (msg == "#LED OFF")
    {
        Hardware::turn_LED_off();
    }
    else if (msg == "#VERSION")
    {
        std::cout << "Current firmware version is "
            << cfg::version << std::endl;
    }
    else if (msg == "#SAY HI")
    {
        std::cout << "Hello, world!" << std::endl;
    }
    else if (msg == "#BEST SUBTEAM?")
    {
        std::cout << "Software is the best subteam!" << std::endl;
    }
    else if (msg == "#WHAT TEAM?")
    {
        std::cout << "WILDCATS" << std::endl;
    }
    else
    {
        std::cout << "Unknown ASCII message: \""
            << msg << "\"" << std::endl;
        return false;
    }
	
    return true;
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
}

/* Transmits the full data string in ASCII */
void XBeeIO::transmit_data_string()
{
    using namespace State_Data;
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>
                (cfg::TIME - cfg::START_TIME).count();
    std::cout << time << "," << STATUS << "," << DATA_P1
         << "," << DATA_P2 << "," << DATA_T1 << "," << DATA_T2
         << "," << DATA_T3 << "," << DATA_THR << ","
         << NEW_DATA << "," << MODE << "," << std::endl; // "ENDL" << __LF__;
}
