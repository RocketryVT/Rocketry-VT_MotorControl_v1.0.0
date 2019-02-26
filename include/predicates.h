/*! \file */

#ifndef PREDICATES_H
#define PREDICATES_H

/// \brief Manages all preflight tests.
/// \details This namespace contains all preflight checks,
/// small and large, which may need to be performed in order
/// to perform a task which depends on them.
/// For example, in order to fill the nitrous tank, you must
/// first be sure the nitrous pump is properly connected,
/// and et cetera. Most if not all functions here
/// will take no arguments and return a boolean.
namespace predicates
{

/// \brief Checks whether the propellant fill line is
/// properly connected.
/// \return true if the fill line is connected.
bool fillLineConnected();

/// \brief Checks whether the nitrous tank is full, within a
/// certain threshold.
/// \return true if the tank is full.
bool nitrousTankFull();

/// \brief Checks whether the nitrous tank is stable,
/// whatever that means.
/// \return true if the tank is stable.
bool nitrousTankStable();

/// \brief Checks whether electronic systems have good
/// continuity.
/// \return true if continuity is good.
bool continuityOk();

/// \brief Checks whether the connection with the ground
/// station is established and stable.
/// \return true if telemetry is all good.
bool telemetryOk();

/// \brief Checks whether an overpressure event is
/// occurring or has occurred.
/// \return true if an overpressure event has occurred.
bool overpressureOccured();

/// \brief Checks whether the motor is locked.
/// \return true if the motor is locked.
bool motorLocked();

} // namespace predicates

#endif // PREDICATES_H
