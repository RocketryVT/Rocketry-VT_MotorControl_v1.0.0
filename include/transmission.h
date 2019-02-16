/*! \file */

#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <iostream>
#include <vector>
#include <string>
#include <deque>

/// \brief A library which handles transmission protocol.
/// \details This namespace is more of a library than a
/// module of the controller, and can be used outside of
/// this project, ideally. It should take away the guess
/// work of constructing and decoding packets.
namespace transmission
{

/// \brief Builds an ascii message packet out of a string.
/// \details Takes a std::string and constructs an ASCII
/// packet out of it.
/// \param msg The message to wrap in a packet.
/// \return A vector of bytes containing the packet.
std::vector<unsigned char> buildPacket(std::string msg);

/// \brief Builds a packet given an arbitrary id and bytestring.
/// \param id The id of the packet.
/// \param data The data the packet will contain.
/// \return A vector of bytes containing the packet.
std::vector<unsigned char> buildPacket(
    uint8_t id, std::vector<uint8_t> data);

/// \brief Parses a deque of bytes.
/// \details Takes a deque of bytes and parses it for all the
/// valid packets it can find; these packets are returned.
/// WARNING: the passed deque is stripped of the bytes which
/// are successfully parsed.
/// \param bytestream The deque of bytes.
/// \return A vector of packets.
std::vector<std::vector<unsigned char>>
    parse(std::deque<unsigned char>& bytestream);

/// \brief Computes the exclusive or parity check of a packet.
/// \param packet A data packet to calculate checksum for.
/// \param c0 First byte of checksum.
/// \param c1 Second byte of checksum.
void xorchecksum(const std::vector<unsigned char> &packet,
    unsigned char &c0, unsigned char &c1);

/// \brief Appends a checksum onto a vector.
/// \details
///    *slaps roof of vector*
///      this bad boy can fit so many checksums in it
/// \param packet The packet to add a checksum to.
void appendChecksum(std::vector<unsigned char> &packet);

/// \brief Reads all the packets in a binary file
/// \param filename The name of the binary file
/// \return A vector of packets contained in the file
std::vector<std::vector<unsigned char>>
    fromFile(const std::string& filename);

/// \brief Converts a packet to a human-readable string
/// \param data The packet to convert.
/// \return A string representation of the packet.
std::string packet2str(const std::vector<unsigned char> &data);

} // namespace transmission

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

/// \brief Converts a variable to bytes and stores them
/// \param vec The vector to store the bytes in
/// \param data A variable containing data to be converted
/// \return The original vector with added bytes
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

/// \brief Converts a variable to bytes and stores them
/// \param vec The vector to store the bytes in
/// \param data A vector of data to be converted
/// \return The original vector with added bytes
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

/// \brief Extracts bytes from a vector
/// \param vec The vector of bytes
/// \param data The variable to fill with bytes
/// \return The original vector, with a few less bytes
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

/// \brief Extracts bytes from a vector
/// \param vec The vector of bytes
/// \param data A vector of variables to populate
/// \return The original vector, with some bytes missing
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
