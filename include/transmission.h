/*! \file */

#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <map>
#include <functional>

/// \brief A library which handles transmission protocol.
/// \details This namespace is more of a library than a
/// module of the controller, and can be used outside of
/// this project, ideally. It should take away the guess
/// work of constructing and decoding packets.
namespace transmission
{

/// \brief Get the list of available channels.
/// \return A list of channels, a vector of strings.
const std::vector<std::string>& channels();

/// \brief Get the ID of a particular channel, if that channel exists.
/// \param The name of the channel to ID.
/// \return The ID of the channel, a uint8_t.
uint8_t getId(const std::string &channel_name);

/// \brief Get the channel name with the associated ID. Returns the null channel
/// if no such ID exists.
/// \param The ID of the queried channel.
/// \return The name of the channel, a std::string.
const std::string& getChannel(uint8_t id);

/// \brief Builds an ascii message packet out of a string.
/// \details Takes a std::string and constructs an ASCII
/// packet out of it.
/// \param channel The channel this packet will be broadcast to.
/// \param msg The message to wrap in a packet.
/// \return A vector of bytes containing the packet.
std::vector<unsigned char> buildPacket(std::string msg);

/// \brief Builds a packet given an arbitrary id and bytestring.
/// \param channel The name of the channel to be broadcast to.
/// \param data The data the packet will contain.
/// \return A vector of bytes containing the packet.
std::vector<unsigned char> buildPacket(
    const std::string &channel, std::vector<uint8_t> data);

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
/// \param packet The packet to convert.
/// \return A string representation of the packet.
std::string packet2str(const std::vector<unsigned char> &packet);

/// \brief Converts a packet to a human-readable string,
/// provided a list of binary-to-text translations.
/// \details This function is similar to the packet2str which
/// only takes a std::vector, but augments this function with
/// a list of translation functions, which are functions that
/// map packets of a certain ID to an ASCII string.
/// \param packet The packet to convert.
/// \param translations A map of IDs to translation functions.
/// \return A string representation of the packet.
std::string packet2str(const std::vector<unsigned char> &packet,
    std::map<uint8_t, std::function<
        std::string(std::vector<uint8_t>)>> &translations);

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
    const std::vector<T> &data)
{
    for (auto e : data)
        vec << e;
    return vec;
}

/// \brief Converts a variable to bytes and stores them
/// \param vec The vector to store the bytes in
/// \param data An array of data to be converted
/// \return The original vector with added bytes
template <typename T, size_t N, typename U =
    std::enable_if_t<std::is_fundamental<T>::value, T>>
std::vector<unsigned char>& operator <<
    (std::vector<unsigned char> &vec,
    const std::array<T, N> &data)
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
    const std::vector<T> &data)
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
