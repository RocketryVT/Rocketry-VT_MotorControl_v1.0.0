/*! \file */

#ifndef COMMS_H
#define COMMS_H

#include <fstream>
#include <deque>
#include <vector>

/// \brief Handles all incoming and outgoing communications
/// for the motor controller.
namespace comms
{

/// \brief Initializes the comms hardware and buffers.
/// \return true on goodie goodie, false otherwise.
bool init();

/// \brief Checks if anything is wrong.
/// \return true on ok, false otherwise.
bool ok();

/// \brief Resets the comms module.
void reset();

/// \brief Exits the comms module.
/// \param code The reason for exiting.
void exit(int code);

/// \brief Call this every few milliseconds to update the
/// input and output buffers and receive packets.
void loop();

/// \brief Queues a std::vector onto the output buffer.
/// \param data A vector of bytes to be transmitted.
void transmit(const std::vector<unsigned char> &data);

/// \brief Queues a std::string onto the output buffer.
/// \param str A string to be transmitted.
void transmit(const std::string &str);

/// \brief Flushes the output buffer to the hardware device.
/// \param blocking A switch to enable blocking transmission.
/// \details Flushes the output buffer to the radio via the
/// TTY USB port. The function will attempt to write
/// all bytes in the buffer before returning.
void flush();

} // namespace comms

#endif // COMMS_H
