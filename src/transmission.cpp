#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>

#include <transmission.h>

const std::vector<std::string> channel_list
{
    "null", // must be the first element - trash channel
    "ground/ping",
    "rocket/ping",
    "relay/ping",
    "rocket/console",
    "relay/rocket/console",
    "relay/console",
    "rocket/status",
    "relay/rocket/status",
    "rocket/imudata",
    "relay/rocket/imudata",
    "rocket/voltage",
    "relay/rocket/voltage",
    "rocket/motor-info",
    "relay/rocket/motor-info",
    "ground/shutdown",
    "ground/reset",
    "ground/abort",
    "ground/perform-test",
    "ground/relay-mode",
    "relay/ground/relay-mode",
    "ground/echo-lexicon",
    "ground/echo-loglist",
    "ground/echo-recipes",
    "ground/log",
    "ground/unlog",
    "ground/echo-tests",
    "ground/echo-channels",
    "ground/set-lock",
    "ground/small-talk",
    "ground/set-status",
    "ground/echo-status",
    "ground/fill-nitrous",
    "ground/disconnect-feed",
    "ground/bleed-nitrous",
};

const std::vector<std::string>& transmission::channels()
{
    return channel_list;
}

uint8_t transmission::getId(const std::string &channel_name)
{
    auto loc = std::find(channel_list.begin(), channel_list.end(), channel_name);
    if (loc == channel_list.end())
    {
        std::cerr << "Cannot find ID for channel "
            << channel_name << std::endl;
        return 0;
    }
    return std::distance(channel_list.begin(), loc);
}

const std::string& transmission::getChannel(uint8_t id)
{
    if (id >= channel_list.size()) return channel_list[0];
    return channel_list[id];
}

// builds an ascii message packet
// the given string is taken as the entire packet data
std::vector<unsigned char> transmission::buildPacket(std::string msg)
{
    if (msg.length() > 255)
        msg = msg.substr(0, 255);

    uint8_t len = msg.length();
    std::vector<unsigned char> packet { 0xAA, 0x14, len, getId("rocket/console") };
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
    bool parsing = true;
    std::vector<std::vector<unsigned char>> packets;

    while (bytestream.size() && parsing)
    {
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
    uint8_t channel = packet[3];
    std::string channelstr = channel_list[channel];
    ss << std::setw(25) << std::left << std::setfill(' ') << channelstr;
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

