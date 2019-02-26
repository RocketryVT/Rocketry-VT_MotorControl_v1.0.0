/*! \file */

#ifndef EMATCH_H
#define EMATCH_H

namespace drivers
{

namespace ematch
{

/// \brief Enables a particular pin.
/// \param pin The pin number to enable.
void enable(uint8_t pin);

/// \brief Disables a particular pin.
/// \param pin The pin number to disable.
void disable(uint8_t pin);

/// \brief Disables all pins on the e-match.
void disableAll();

} // namespace ematch

} // namespace drivers

#endif // EMATCH_H
