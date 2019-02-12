/*! \file */

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <vector>

/// \brief Describes behavior idioms apart from specific modules.
namespace behavior
{

/// \brief Called upon the reciept of a data packet.
/// \param id The packet ID.
/// \param data The data contained in the packet.
void dataReceipt(uint8_t id, const std::vector<uint8_t> &data);

} // namespace behavior

#endif
