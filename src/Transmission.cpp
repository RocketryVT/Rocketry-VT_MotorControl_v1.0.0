#include <iostream>

#include "Test_all.h"
#include "Transmission.h"
#include "config.h"
#include "Assert.h"
#include "XBee_IO.h"

/**
* Compiles data to a char array so that it can be pushed to the
* serial port directly. Also this makes it easier to compute a
* checksum on the data
*
* INPUTS
* unsigned int type -> data transmission packet type (see documentation)
* 
* OUTPUTS
* a std::vector containing the data packet
*/
std::vector<unsigned char> Transmission::buildPacket(unsigned int type)
{
	using namespace State_Data;

    // packet and header
    std::vector<unsigned char> packet { 0xAA, 0x14 };

	// Chars used to store float bytes
    uint32_t msec = std::chrono::duration_cast<std::chrono::milliseconds>
        (cfg::DATA_TIME - cfg::START_TIME).count();
	
	switch (type)
    {
                   // echo version
        case 0x00: return buildPacket(cfg::version);

        case 0x01: // SD Card Begin of Save File

            packet << (uint8_t) 6 << (uint8_t) 0x01;
            break;

        case 0x02: // SD Card End of Save File

            packet << (uint8_t) 6 << (uint8_t) 0x02;
            break;

        case 0x10:

            packet << (uint8_t) 7 << (uint8_t) 0x10 << (uint8_t) MODE;
            break;

        case 0x40: // Solid Motor Static Fire Tests 2018-11-11

            packet << (uint8_t) 22 << (uint8_t) 0x40
                << (uint32_t) msec << (uint8_t) MODE
                << (uint16_t) STATUS << (float) DATA_T1
                << (float) DATA_THR << (uint8_t) NEW_DATA;
            break;

        case 0x51: // Cold flow test data

            packet << (uint8_t) 34 << (uint8_t) 0x51
                << msec << (uint8_t) MODE << (uint16_t) STATUS
                << (float) DATA_P1 << (float) DATA_P2
                << (float) DATA_T1 << (float) DATA_T2
                << (float) DATA_THR << (uint8_t) NEW_DATA;
            break;

        case 0x52: // Cold flow test data

            packet << (uint8_t) 38 << (uint8_t) 0x52
                << msec << (uint8_t) MODE << (uint16_t) STATUS
                << (float) DATA_P1 << (float) DATA_P2 << (float) DATA_T1
                << (float) DATA_T2 << (float) DATA_T3 << (float) DATA_THR
                << (uint8_t) NEW_DATA;
            break;

        case 0xB0: // Do Unit Tests

            // run_tests(true);
            packet.clear();
            for (unsigned int i = 0; i < Test::resstriter; i++)
            {
                packet.push_back(Test::resstr[i]);
            }		
            break;

        default: // Do nothing

            packet.clear();
            return packet;
	}
    appendChecksum(packet);
    return packet;
}

// builds an ascii message packet
// the given string is taken as the entire packet data;
// if it doesn't begin with a '#' character, one will be appended
// empty strings will be treated as a lone '#' character
std::vector<unsigned char> Transmission::buildPacket(std::string msg)
{
    std::vector<unsigned char> packet { 0xAA, 0x14 };
    
    if (msg.length() == 0) msg = "#";
    if (msg[0] != '#')
    {
        msg = "#" + msg;
    }
    packet.push_back(msg.length());

    for (auto e : msg)
        packet.push_back(e);
    
    unsigned char c0, c1;
    xorchecksum(packet, c0, c1);
    packet.push_back(c0);
    packet.push_back(c1);
    return packet;
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
void Transmission::xorchecksum(const std::vector<unsigned char> &packet,
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
void Transmission::appendChecksum(std::vector<unsigned char> &packet)
{
    unsigned char c0, c1;
    xorchecksum(packet, c0, c1);
    packet << c0 << c1;
}

/**
 * Converts a series of unsigned char values to a
 * 32 bit floating point value to four bytes. The most
 * significant byte is transmitted first.
 *
 * INPUTS
 * char a -> Most significant byte
 * char b -> 2nd most significant byte
 * char c -> 3rd most significant byte
 * char d -> Least significant byte
 *
 * OUTPUTS
 * float -> Floating point value
 */
float Transmission::charsToFloat(unsigned char a, unsigned char b,
    unsigned char c, unsigned char d)
{
	unsigned long val = (a << 24) | (b << 16) | (c << 8) | d;
	return *(float*) &val;
}

/**
 * Converts a 32 bit floating point value to four bytes by
 * assigning values to the non-const char pointers. The most
 * significant byte is transmitted first.
 *
 * INPUTS
 * float x -> floating point value
 * char* a -> Most significant byte
 * char* b -> 2nd most significant byte
 * char* c -> 3rd most significant byte
 * char* d -> Least significant byte
 *
 * OUTPUTS
 * N/A
 */
void Transmission::floatToChars(float x, unsigned char &a, 
    unsigned char &b, unsigned char &c, unsigned char &d)
{
	unsigned long i = *(unsigned long*) &x;
	a = (i >> 24) & 0xFF;
	b = (i >> 16) & 0xFF;
	c = (i >> 8) & 0xFF;
	d = i & 0xFF;
}
