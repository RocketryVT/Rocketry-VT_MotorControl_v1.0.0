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
	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned char d;
	unsigned char c0 = 0;
	unsigned char c1 = 0;
	unsigned int i = 0;
    unsigned long long msec;
	
	switch (type) {
	case 0x00: // Initialization message
		
		packet.push_back(cfg::version.length());
		packet.push_back(0x00); // Transmit Type
		// Fill str with firmware version string
        for (auto e : cfg::version)
            packet.push_back(e);
		return packet;
	case 0x01: // SD Card Begin of Save File
		packet.push_back(6);
		packet.push_back(0x01);
		xorchecksum(packet, c0, c1);
		packet.push_back(c0); // Checksum
		packet.push_back(c1); // Checksum
		return packet;
	case 0x02: // SD Card End of Save File
		packet.push_back(6);
		packet.push_back(0x02);
		xorchecksum(packet, c0, c1);
		packet.push_back(c0); // Checksum
		packet.push_back(c1); // Checksum
		return packet;
	case 0x10:
		packet.push_back(7); // Length
		packet.push_back(0x10); // Type
		packet.push_back(MODE); // Mode
		xorchecksum(packet, c0, c1);
		packet.push_back(c0); // Checksum
		packet.push_back(c1); // Checksum
		return packet;
	case 0x40: // Solid Motor Static Fire Tests 2018-11-11
		cfg::TIME = cfg::DATA_TIME;
		packet.push_back(22); // length
		packet.push_back(0x40); // type

        // time
        msec = std::chrono::duration_cast<std::chrono::milliseconds>
            (cfg::TIME - cfg::START_TIME).count();
		packet.push_back((unsigned char) (msec >> 24) & 0xFF);
		packet.push_back((unsigned char) (msec >> 16) & 0xFF);
		packet.push_back((unsigned char) (msec >> 8) & 0xFF);
		packet.push_back((unsigned char) (msec >> 0) & 0xFF);

		packet.push_back(MODE); // mode

        // status
		packet.push_back((unsigned char) (STATUS >> 8));
		packet.push_back((unsigned char) (STATUS >> 8));

		// Temperature Oxidizer
		floatToChars(DATA_T1, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Thrust
		floatToChars(DATA_THR, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// New Data
		packet.push_back(NEW_DATA);

		// Checksum
		xorchecksum(packet, c0, c1);
		packet.push_back(c0);
		packet.push_back(c1);

		return packet;
	case 0x51: // Cold flow test data
		cfg::TIME = cfg::DATA_TIME;

        packet.push_back(34); // length
		packet.push_back(0x51); // type

		// Time
        msec = std::chrono::duration_cast<std::chrono::milliseconds>
            (cfg::TIME - cfg::START_TIME).count();
		packet.push_back((unsigned char) (msec >> 24) & 0xFF);
		packet.push_back((unsigned char) (msec >> 16) & 0xFF);
		packet.push_back((unsigned char) (msec >> 8) & 0xFF);
		packet.push_back((unsigned char) (msec >> 0) & 0xFF);

		// Mode
	    packet.push_back(MODE);

		// Status
		packet.push_back((unsigned char) (STATUS >> 8));
		packet.push_back((unsigned char) (STATUS >> 8));

		// Pressure Oxidizer
		floatToChars(DATA_P1, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Pressure Combustion
		floatToChars(DATA_P2, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Temperature Oxidizer
		floatToChars(DATA_T1, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Temperature Combustion
		floatToChars(DATA_T2, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Thrust
		floatToChars(DATA_THR, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// New Data
		packet.push_back(NEW_DATA);

		// Checksum
		xorchecksum(packet, c0, c1);
		packet.push_back(c0);
		packet.push_back(c1);

		return packet;
	case 0x52: // Cold flow test data
		cfg::TIME = cfg::DATA_TIME;

		packet.push_back(38); // length
		packet.push_back(0x52); // type

		// Time
        msec = std::chrono::duration_cast<std::chrono::milliseconds>
            (cfg::TIME - cfg::START_TIME).count();
		packet.push_back((unsigned char) (msec >> 24) & 0xFF);
		packet.push_back((unsigned char) (msec >> 16) & 0xFF);
		packet.push_back((unsigned char) (msec >> 8) & 0xFF);
		packet.push_back((unsigned char) (msec >> 0) & 0xFF);

		// Mode
		packet.push_back(MODE);

		// Status
		packet.push_back((unsigned char) (STATUS >> 8));
		packet.push_back((unsigned char) (STATUS >> 8));

		// Pressure Oxidizer
		floatToChars(DATA_P1, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Pressure Combustion
		floatToChars(DATA_P2, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Temperature Oxidizer
		floatToChars(DATA_T1, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Temperature Combustion
		floatToChars(DATA_T2, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Temperature Post Combustion
		floatToChars(DATA_T3, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// Thrust
		floatToChars(DATA_THR, a, b, c, d);
		packet.push_back(a);
		packet.push_back(b);
		packet.push_back(c);
		packet.push_back(d);

		// New Data
		packet.push_back(NEW_DATA);

		// Checksum
		xorchecksum(packet, c0, c1);
		packet.push_back(c0);
		packet.push_back(c1);

		return packet;
	case 0xB0: // Do Unit Tests
		
		// Run Tests
		// run_tests(true);
        packet.clear();
		
		// Compile string
		for (unsigned int i = 0; i < Test::resstriter; i++)
        {
			packet.push_back(Test::resstr[i]);
		}
		
		return packet;
	default: // Do nothing
        packet.clear();
		return packet;
	}
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
