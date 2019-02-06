/*! \file */

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <vector>

/// \brief Describes behavior idioms apart from specific modules.
namespace behavior
{

bool dataReceipt(uint8_t id, const std::vector<uint8_t> &data);

} // namespace behavior

#endif
