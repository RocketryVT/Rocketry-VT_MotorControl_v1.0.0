#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <iostream>
#include <vector>
#include <string>

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

    // builds an ascii message packet out of a string
    std::vector<unsigned char> buildPacket(std::string msg);

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

    // appends a checksum onto a vector
    // 
    //    *slaps roof of vector*
    //      this bad boy can fit so many checksums in it
    //
    void appendChecksum(std::vector<unsigned char> &packet);

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

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //
    //  MAGIC TEMPLATE FUNCTIONS BELOW
    //
    //  These do all the translating from fundamental types
    //  (chars, ints, longs, etc) into raw bytestrings.
    //  They are defined as overloads on the << operator.
    //  The syntax for using these functions is as follows:
    //
    //      std::vector<unsigned char> bytes;
    //      double data1 = 1.23, data2 = 4.56, data3 = -9.87;
    //      bytes << data1 << data2 << data3;
    //
    //  At this point bytes should have a length of 24
    //  (3*sizeof(double)), and will contain the bytes of the
    //  3 doubles.
    //
    //  In the same way, vectors of fundamental types can be
    //  converted just as easily:
    //
    //      std::vector<unsigned char> bytes;
    //      std::vector<int32_t> data { 67, -826, 23441, -519 };
    //      bytes << data;
    //
    //  bytes will now have a length of 16 (4*sizeof(uint32_t))
    //  and should contain the bytes of the four integers.
    //
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    template <typename T, typename U =
        std::enable_if_t<std::is_fundamental<T>::value, T>>
    std::vector<unsigned char>& operator <<
        (std::vector<unsigned char> &vec, T data)
    {
        unsigned char *c = reinterpret_cast<unsigned char*>(&data);
        for (int i = sizeof(T) - 1; i >= 0; --i)
        {
            vec.push_back(c[i]);
        }
        return vec;
    }

    template <typename T, typename U =
        std::enable_if_t<std::is_fundamental<T>::value, T>>
    std::vector<unsigned char>& operator <<
        (std::vector<unsigned char> &vec,
        const std::vector<unsigned char> &data)
    {
        for (auto e : data)
            vec << e;
        return vec;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

} // namespace Transmission

#endif // TRANSMISSION_H
