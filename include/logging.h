/*! \file */

#ifndef LOGGING_H
#define LOGGING_H

#include <vector>

/// \brief Handles internal system status logging.
namespace logging
{

/// \brief Opens the log file and begins logging.
/// \details Must be called before any other
/// functions in this namespace will do anything.
/// \return true if a-ok, false otherwise.
bool init();

/// \brief Checks if the logging module is all good.
/// \return true if ok, false otherwise.
bool ok();

/// \brief Exits the logging module; flushes and closes the file.
/// \param code The reason for exiting.
void exit(int code);

/// \brief Resets logging to its startup state.
void reset();

/// \brief Writes an array of data to the log.
/// \details The provided data will be wrapped as a packet
/// per the same transmission protocol as defined by
/// the transmission namespace.
/// \param data The data to log.
void write(const std::vector<uint8_t> &data);

/// \brief Flushes the log file.
void flush();

} // namespace logging

#endif // LOGGING_H
