#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <thread>
#include <algorithm>
#include <cctype>
#include <map>

#include <transmission.h>
#include <control.h>
#include <logging.h>

const std::map<uint8_t, std::string> channel_list
{
    // NULL
    {0,   "/null"}, // do not change!
    // NETWORKING
    {1,   "/control/motor/ping"},
    {2,   "/control/motor/small-talk"},
    {3,   "/control/support/ping"},
    {4,   "/control/support/small-talk"},
    // HARDWARE LOCKS
    {5,   "/control/motor/lock"},
    {6,   "/control/motor/unlock"},
    {7,   "/control/support/lock"},
    {8,   "/control/support/unlock"},
    // HARDWARE OPERATIONS
    {9,   "/control/begin-fill"},
    {10,  "/control/stop-fill"},
    {11,  "/control/detach-fill-line"},
    {12,  "/control/open-valves"},
    {13,  "/control/close-valves"},
    {14,  "/control/launch"},
    // CONSOLE OUTPUT
    {15,  "/motor/console"},
    {16,  "/support/console"},
    // PROGRAM REFLECTION
    {17,  "/control/motor/echo-channels"},
    {18,  "/control/motor/echo-commands"},
    {19,  "/control/support/echo-channels"},
    {20,  "/control/support/echo-commands"},
    // LOGGING
    {21,  "/control/motor/begin-log"},
    {22,  "/control/motor/echo-active-logs"},
    {23,  "/control/motor/echo-logs"},
    {24,  "/control/motor/stop-logs"},
    {25,  "/control/support/begin-log"},
    {26,  "/control/support/echo-active-logs"},
    {27,  "/control/support/echo-logs"},
    {28,  "/control/support/stop-logs"},
    // PROGRAM TERMINATION
    {29,  "/control/abort-all"},
    {30,  "/control/motor/abort"},
    {31,  "/control/motor/reboot"},
    {32,  "/control/motor/reset"},
    {33,  "/control/motor/shutdown"},
    {34,  "/control/support/abort"},
    {35,  "/control/support/reboot"},
    {36,  "/control/support/reset"},
    {37,  "/control/support/shutdown"},
    // UNIT TESTS
    {38,  "/control/motor/echo-tests"},
    {39,  "/control/motor/perform-tests"},
    {40,  "/control/support/echo-tests"},
    {41,  "/control/support/perform-tests"},
    // INTERNAL TRANSMISSION CHANNELS
    {42,  "/motor/support/line-disconnected"},
    {43,  "/motor/support/tank-full"},
    {44,  "/support/motor/fuel-begin"},
    // TEMPORARY
    {45,  "/control/motor/system"},
    {46,  "/control/support/system"}
};

const std::map<uint8_t, std::string>& transmission::channels()
{
    return channel_list;
}

uint8_t transmission::getId(const std::string &channel_name)
{
    for (auto pair : channel_list)
    {
        if (pair.second == channel_name) return pair.first;
    }

    std::stringstream ss;
    ss <<  "WARNING: Cannot find ID for channel "
       << channel_name << std::endl;
    logging::announce(ss.str(), false, true);
    return 0;
}

const std::string& transmission::getChannel(uint8_t id)
{
    if (channel_list.find(id) == channel_list.end())
    {
        return channel_list.at(0);
    }
    return channel_list.at(id);
}

// builds an ascii message packet
// the given string is taken as the entire packet data
std::vector<unsigned char> transmission::buildPacket(std::string msg)
{
    if (msg.length() > 255)
        msg = msg.substr(0, 255);

    uint8_t len = msg.length();
    std::vector<unsigned char> packet { 0xAA, 0x14, len, getId("/motor/console") };
    for (auto e : msg)
        packet.push_back(e);
    appendChecksum(packet); 
    return packet;
}

std::vector<unsigned char> transmission::buildPacket(
    const std::string& channel, std::vector<unsigned char> data)
{
    if (data.size() > 255) data.resize(255);

    uint8_t len = data.size();
    std::vector<unsigned char> packet { 0xAA, 0x14, len, getId(channel) };
    for (auto e : data)
        packet.push_back(e);
    appendChecksum(packet);
    return packet;
}

std::vector<std::vector<unsigned char>>
    transmission::parse(std::deque<unsigned char>& bytestream)
{
    auto now = std::chrono::steady_clock::now(), start = now;
    auto timeout = std::chrono::milliseconds(100);

    bool parsing = true;
    std::vector<std::vector<unsigned char>> packets;

    while (bytestream.size() && parsing && now - start < timeout)
    {
        now = std::chrono::steady_clock::now();

        // go to next 0xAA 0x14
        while (bytestream.size() > 0 && bytestream.front() != 0xAA)
        {
            bytestream.pop_front();
        }

        if (bytestream.size() < 2) continue; // wait for next sync byte

        // if the next byte isn't 0x14, pop the front and repeat
        if (bytestream[1] != 0x14)
        {
            bytestream.pop_front();
            continue;
        }

        if (bytestream.size() < 4) // header hasn't arrived
        {
            parsing = false;
            continue;
        }

        uint8_t data_length = bytestream[2];

        if (bytestream.size() < 4U + data_length + 2U) // incomplete data
        {
            parsing = false;
            continue;
        }

        // at this point we know there exists an entire discrete packet
        // if there's a checksum error that doesn't matter so don't
        // go popping stuff off the deque just yet
        std::vector<unsigned char> packet;

        // exclude checksum for now
        for (size_t i = 0; i < 4U + data_length; ++i)
        {
            packet.push_back(bytestream[i]);
        }
        unsigned char c0 = bytestream[4 + data_length],
                      c1 = bytestream[5 + data_length];

        unsigned char c0_true, c1_true;
        transmission::xorchecksum(packet, c0_true, c1_true);

        if (c0 != c0_true || c1 != c1_true) // shit, checksum error
        {
            std::cout << "Checksum error: got (" << std::hex
                << (int) c0 << ", " << (int) c1 << "), expected ("
                << (int) c0_true << ", " << (int) c1_true << ")"
                << std::endl;
            bytestream.pop_front();
            continue;
        }

        // hooray, no checksum error! add those digits to the packet
        // and start popping things off the deque
        packet.push_back(c0);
        packet.push_back(c1);
        packets.push_back(packet);

        for (size_t i = 0; i < packet.size(); ++i)
        {
            bytestream.pop_front();
        }
    }

    return packets;
}

/**
* Computes the exclusive or parity check of the bytes in a message
*
* INPUTS
* char* str -> pointer to char array
* unsigned int len -> lenght of str
* unsigned char* c1 -> First byte of checksum
* unsigned char* c2 -> Second byte of checksum
*/
void transmission::xorchecksum(const std::vector<unsigned char> &packet,
    unsigned char &c0, unsigned char &c1)
{
	c0 = 0;
	c1 = 0;
	unsigned int i = 0;
	while (i < packet.size() -1 )
    {
		c0 ^= packet[i];
		c1 ^= packet[i + 1];
		i += 2;
	}
	if (i < packet.size())
    {
		c0 ^= packet[i];
	}
	return;
}

// appends a checksum onto a packet
void transmission::appendChecksum(std::vector<unsigned char> &packet)
{
    unsigned char c0, c1;
    xorchecksum(packet, c0, c1);
    packet << c0 << c1;
}

// reads a binary file and returns a list of packets
std::vector<std::vector<unsigned char>>
    transmission::fromFile(const std::string &filename)
{
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) return std::vector<std::vector<unsigned char>>();
    
    infile.seekg(0, std::ios::end);
    size_t end = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::deque<unsigned char> bytestream;
	  for (size_t i = 0; i < end; ++i) 
		    bytestream.push_back(infile.get());

    return transmission::parse(bytestream);
}

// converts a packet to a nice human readable form.
// assumes the packet is a valid packet
std::string transmission::packet2str(const std::vector<unsigned char> &packet)
{
    std::stringstream ss;
    ss << std::hex;
    uint8_t id = packet[3];
    std::string channel = getChannel(id);
    ss << std::setw(25) << std::left << std::setfill(' ') << channel;
    ss << std::right;
    for (size_t i = 0; i < packet.size(); ++i)
    {
        ss << std::setfill('0') << std::setw(2) << (int) packet[i];
        if (i < packet.size() - 1) ss << " ";
    }
    return ss.str();
}

std::string transmission::packet2str(
    const std::vector<unsigned char> &packet,
    std::map<uint8_t, std::function<
        std::string(std::vector<uint8_t>)>> &translations)
{
    if (packet.size() < 6) return "";

    uint8_t id = packet[3];
    std::vector<uint8_t> data(packet.begin() + 4, packet.end() - 2);
    auto tr = translations.find(id);
    if (tr != translations.end())
    {
        std::stringstream ss;
        ss << std::setw(25) << std::left << std::setfill(' ')
            << transmission::getChannel(id) << tr->second(data);
        return ss.str();
    }
    return packet2str(packet);
}

