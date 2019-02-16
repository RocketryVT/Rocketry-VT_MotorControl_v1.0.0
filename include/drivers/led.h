/*! \file */

#ifndef LED_H
#define LED_H

#include <cstdint>

namespace drivers
{

/// \brief Handles all LED functionality at the hardware level.
namespace led
{

/// \brief Sets the LEDs to a particular state.
/// \details Sets the 8 LEDs to a particular
/// state encoded by an 8 bit integer.
/// \param val The value to set the LEDs to.
void set(uint8_t val);

/// \brief Sets a particular LED's state.
/// \details Given an index, sets the state of
/// the LED at that index. Index must be between
/// 0 and 7, inclusive.
void set(uint8_t index, bool state);

/// \brief Gets the current LED state.
uint8_t get();

} // namespace led

} // namespace drivers

#endif // LED_H
