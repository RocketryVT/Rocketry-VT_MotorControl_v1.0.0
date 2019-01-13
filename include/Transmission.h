#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <vector>

namespace Transmission
{
	/**
	* Compiles data to a char array so that it can be pushed to the 
	* serial port directly. Also this makes it easier to compute a
	* checksum on the data
	*
	* INPUTS
	* unsigned int type -> data transmission packet type (see documentation)
    * 
    * OUTPUT
    * std::vector<unsigned char> -> requested data packet
	*/
	std::vector<unsigned char> buildPacket(unsigned int type);

	/**
	 * Computes the exclusive or parity check of the bytes in a message
	 *
	 * INPUTS
     * std::vector &packet -> data packet to calculate checksum for
	 * unsigned char &c0 -> First byte of checksum
	 * unsigned char &c1 -> Second byte of checksum
	 */
	void xorchecksum(const std::vector<unsigned char> &packet,
        unsigned char &c0, unsigned char &c1);

	/**
	* Converts a series of unsigned char values to a 
	* 32 bit floating point value to four bytes. The most
	* significant byte is transmitted first.
	*
	* INPUTS
	* char* a -> Most significant byte
	* char* b -> 2nd most significant byte
	* char* c -> 3rd most significant byte
	* char* d -> Least significant byte
	*
	* OUTPUTS
	* float -> Floating point value
	*/
	float charsToFloat(unsigned char a, unsigned char b,
        unsigned char c, unsigned char d);

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
	void floatToChars(float x, unsigned char &a, unsigned char &b,
        unsigned char &c, unsigned char &d);

} // namespace Transmission

#endif // TRANSMISSION_H
