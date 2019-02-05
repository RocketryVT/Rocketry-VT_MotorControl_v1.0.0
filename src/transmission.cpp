#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <transmission.h>

// builds an ascii message packet
// the given string is taken as the entire packet data;
// if it doesn't begin with a '#' character, one will be appended
// empty strings will be treated as a lone '#' character
std::vector<unsigned char> transmission::buildPacket(std::string msg)
{
    if (msg.length() > 255)
        msg = msg.substr(0, 255);

    uint8_t len = msg.length();
    std::vector<unsigned char> packet { 0xAA, 0x14, len, '#' };
    for (auto e : msg)
        packet.push_back(e);
    appendChecksum(packet); 
    return packet;
}

std::vector<unsigned char> transmission::buildPacket(
    uint8_t id, std::vector<unsigned char> data)
{
    if (data.size() > 255) data.resize(255);

    uint8_t len = data.size();
    std::vector<unsigned char> packet { 0xAA, 0x14, len, id };
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
            #ifdef DEBUG
            std::cout << "Checksum error: got (" << std::hex
                << (int) c0 << ", " << (int) c1 << "), expected ("
                << (int) c0_true << ", " << (int) c1_true << ")"
                << std::endl;
            #endif
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

// converts a packet to a nice human readable form
std::string transmission::packet2str(const std::vector<unsigned char> &data)
{
    std::stringstream ss;
    ss << std::hex;
    bool ascii = data.size() > 3 && data[3] == '#';
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (ascii && i > 3 && i < data.size() - 2)
            ss << data[i];
        else
            ss << std::setfill('0') << std::setw(2) << (int) data[i];
        if (i < data.size() - 1 &&
            !(ascii && i > 3 && i < data.size() - 3))
            ss << " ";
        if (i == 3 || (data.size() > 2 && i == 3U + data[2]))
            ss << "| ";
    }
    return ss.str();
}
