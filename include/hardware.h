/*! \file */

#ifndef HARDWARE_H
#define HARDWARE_H

#include <cstdint>

/// \brief Manages external hardware connections.
/// \details This module handles all hardware things
/// that aren't comms and logging. This includes
/// sensors, motors, solenoids, etc.
namespace hardware
{

/// \brief Initializes external sensor connections.
/// \return true if all good, false otherwise.
bool init();

/// \brief Checks if hardware is ok.
/// \return true on ok, false on big bad.
bool ok();

/// \brief closes and flushes logs, closes sensor connections.
/// \param code The reason for exiting.
void exit(int code);

/// \brief Resets all hardware things back to initialization.
/// \return true if reinitialization is successful.
bool reset();

/// \brief Updates sensor data and handles physical device control
void loop();

/// \brief Unlocks a particular code for the motor system.
/// \details This function allows the pump-combustor system
/// to be robustly unlocked; it must be called with 0, 1, and 2,
/// in that order, to be unlocked. Receipt of these codes
/// out of order will re-lock all motor locks.
/// \param code The code to unlock.
void unlock(uint8_t code);

/// \brief Locks the motor.
/// \details Locks access to the motor. Must call unlock()
/// to reallow access.
void lock();

/// \brief Checks if the motor is locked.
/// \return true if motor is locked, false otherwise.
bool isLocked();

/// \brief Provides the lock state, a uint8_t.
/// \details The lock state is a number between 0 and max_lock,
/// a compile-time constant. If the state is less than max_lock,
/// the lock is locked.
/// \return The lock state.
uint8_t lockState();

/// \brief Performs a continuity test and returns the result.
/// \return true of continuity is good, false otherwise.
bool continuity();

/// \brief Closes all valves on the vehicle.
void closeValves();

/// \brief Opens all valves on the vehicle.
void openValves();

/// \brief Launches the rocket.
void launch();

/// \brief Get the state of the feed line.
bool feedLineConnected();

/// \brief Disconnects the oxidizer feed line.
void disconnectFeedLine();

/// \brief Start fill operations.
void beginFill(); 

/// \brief Stop the fill operation.
void stopFill();

/// \brief Get whether a fill is ongoing.
bool isFillOngoing();

void setLed(uint8_t bitmask);

} // namespace hardware

#endif // HARDWARE_H

