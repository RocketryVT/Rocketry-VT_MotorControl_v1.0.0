/*! \file */

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <vector>

/// \brief Describes behavior idioms apart from specific modules.
namespace behavior
{

/// \brief Called upon the successful receipt of a data packet
/// \details The argument data should be stripped of the header, length,
/// and checksum bytes
/// \param id The packet ID.
/// \param data The data contained in the packet.
void dataReceipt(uint8_t id, const std::vector<uint8_t> &data);

} // namespace behavior

#endif
