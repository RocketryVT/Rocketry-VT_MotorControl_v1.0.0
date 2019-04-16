/*! \file */

#ifndef LOGGING_H
#define LOGGING_H

#include <vector>
#include <map>
#include <functional>
#include <chrono>

/// \brief Handles internal system status logging.
namespace logging
{

/// \brief A log recipe struct, for convenience.
/// \details Contains a description of a given log,
/// and a function which returns a valid packet
/// for transmission. Will be called at a certain
/// frequency by logging::loop().
/// \var recipe::description A human-readable description
/// of this log recipe.
/// \var recipe::function A function which produces
/// a valid form of this log type.
struct recipe
{
    std::string description;
    std::function<std::vector<uint8_t>(void)> function;
};

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

/// \brief Handles all repeated logging.
void loop();

/// \brief Resets logging to its startup state.
/// \return true if reinitialization is successful.
bool reset();

/// \brief Writes an array of data to the log.
/// \details The provided data will be wrapped as a packet
/// per the same transmission protocol as defined by
/// the transmission namespace.
/// \param data The data to log.
void write(const std::vector<uint8_t> &data);

/// \brief Writes a string message to the log.
/// \details The string will be wrapped as an ASCII packet
/// per the transmittion protocol defined in transmission.
/// \param str The string to log.
void write(const std::string &str);

/// \brief Flushes the log file.
void flush();

/// \brief Gets a list of all active logs.
/// \details Returns a std::map of all active logs and their
/// associated frequences. A log is indicated as a uint8_t,
/// paired with a std::chrono::milliseconds to indicate
/// the minimum amount of time between logs.
/// \return A std::map containing all active logs.
const std::map<uint8_t, std::chrono::milliseconds>& loglist();

/// \brief Adds a log to the active log list.
/// \details A log is indicated by its uint8_t id (a
/// recipe must be provided via logging::addRecipe()). A period
/// must be provided as a chrono::duration; this is the MINIMUM
/// period between logs; no guarantees are made as to the maximum
/// time between logs.
/// \param id The log ID.
/// \param period The minimum period between logs of this type.
void addLog(uint8_t id, const std::chrono::milliseconds &period);

/// \brief Removes a log from the loglist.
/// \param id The log to remove, indicated by its ID.
void removeLog(uint8_t id);

/// \brief Removes all logs from the loglist.
void clearLogs();

/// \brief Adds a log recipe to the recipe book.
/// \details New recipes may be added to the recipe book with this
/// function. A recipe is a function which takes no arguments and
/// builds a particular log from those variables available in
/// namespace state. A "log" in this context is a fully fleshed out
/// packet, with a header, checksum, and data.
/// \param desc The channel name of this log.
/// \param func A std::function which constructs a log.
void addRecipe(uint8_t id, const std::string &channel,
    const std::function<std::vector<uint8_t>(void)> &func);

/// \brief Returns the list of recipes currently loaded.
/// \return A std::map of IDs to recipes.
const std::map<uint8_t, recipe>& recipes();

/// \brief Sends a given string message to the radio, console, or both.
/// \details This function should be preferred over std::cout, as it allows
/// the logging namespace to decide how to handle internal logging, console
/// output, and outgoing radio communications.
/// \param str The std::string message to send.
/// \param radio Whether to send this message via radio.
/// \param console Whether to send this message to the console.
void announce(const std::string &str, bool radio, bool console);

/// \brief Sends a given packet to the radio, console, or both.
/// \details This function should be preferred over std::cout, as it allows
/// the logging namespace to decide how to handle internal logging, console
/// output, and outgoing radio communications.
/// \param packet The packet to send.
/// \param radio Whether to send this message via radio.
/// \param console Whether to send this message to the console.
void announce(const std::vector<uint8_t> &packet, bool radio, bool console);

} // namespace logging

#endif // LOGGING_H
