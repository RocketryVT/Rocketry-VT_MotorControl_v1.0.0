#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <iostream>
#include <vector>
#include <string>
#include <deque>

namespace transmission
{

enum packet_t : unsigned char
{
    echo_version        = 0x00,
    log_begin           = 0x01,
    log_end             = 0x02,
    echo_mode           = 0x10,
    solid_motor_test    = 0x40,
    cold_flow_test      = 0x51,
    cold_flow_test_exr  = 0x52,
    run_unit_tests      = 0xB0
};

// Compiles data to a char array so that it can be pushed to the 
// serial port directly. Also this makes it easier to compute a
// checksum on the data
//
// INPUTS
// unsigned int type -> data transmission packet type (see documentation)
//
// OUTPUT
// std::vector<unsigned char> -> requested data packet
//
std::vector<unsigned char> buildPacket(unsigned int type);

// builds an ascii message packet out of a string
std::vector<unsigned char> buildPacket(std::string msg);

// builds a packet given an arbitrary id and bytestring
std::vector<unsigned char> buildPacket(
    uint8_t id, std::vector<uint8_t> data);

// parses a vector of bytes and sifts out all the good packets
// which it returns in a vector of packets
//
// this function modifies the std::deque which is passed to it,
// popping off bytes which are processed and leaving alone bytes
// that are not
std::vector<std::vector<unsigned char>>
    parse(std::deque<unsigned char>& bytestream);

// Computes the exclusive or parity check of the bytes in a message
//
// INPUTS
// std::vector &packet -> data packet to calculate checksum for
// unsigned char &c0 -> First byte of checksum
// unsigned char &c1 -> Second byte of checksum
void xorchecksum(const std::vector<unsigned char> &packet,
    unsigned char &c0, unsigned char &c1);

// appends a checksum onto a vector
// 
//    *slaps roof of vector*
//      this bad boy can fit so many checksums in it
//
void appendChecksum(std::vector<unsigned char> &packet);

// called upon the successful receipt of a data packet
// the argument data should be stripped of the header,
// length, and checksum bytes
// returns true if successful, false if error encountered
bool dataReceipt(uint8_t id, const std::vector<unsigned char> &data);

// reads all the packets in a binary file and puts all valid
// ones into a vector of packets; returns an empty list
// if the given filename is invalid
std::vector<std::vector<unsigned char>>
    fromFile(const std::string& filename);

// converts a packet to a nice human readable form
std::string packet2str(const std::vector<unsigned char> &data);

} // namespace Transmission

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
//  You can also pull data from vectors and stick it into
//  fundamental types using the >> operator:
//
//      std::vector<unsigned char> packet = getPacket();
//      double d1, d2, d3, d4;
//      int n1, n2, n3;
//      packet >> d1 >> d2 >> d3 >> d4 >> n1 >> n2 >> n3;
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

// note: this function's behavior is not guaranteed
// to be any good if the vector doesn't contain enough
// bytes to fill whatever you're trying to fill
template <typename T, typename U =
    std::enable_if_t<std::is_fundamental<T>::value, T>>
std::vector<unsigned char>& operator >>
    (std::vector<unsigned char> &vec, T& data)
{
    if (sizeof(T) > vec.size()) return vec;

    std::vector<unsigned char> extract;
    for (int i = sizeof(T) - 1; i >= 0; --i)
    {
        extract.push_back(vec[i]);
    }
    vec.erase(vec.begin(), vec.begin() + extract.size());
    unsigned char *ptr = reinterpret_cast<unsigned char*>(&data);
    std::copy(extract.begin(), extract.end(), ptr);
    return vec;
}

template <typename T, typename U =
    std::enable_if_t<std::is_fundamental<T>::value, T>>
std::vector<unsigned char>& operator >>
    (std::vector<unsigned char> &vec,
    const std::vector<unsigned char> &data)
{
    while (vec.size() >= sizeof(T))
    {
        T elem;
        vec >> elem;
        data.push_back(elem);
    }
    return vec;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TRANSMISSION_H
