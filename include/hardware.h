/*! \file */

#ifndef HARDWARE_H
#define HARDWARE_H

/// \brief Manages external hardware connections.
/// \details This module handles all hardware things
/// that aren't comms and logging. This includes
/// sensors, motors, solenoids, etc.
namespace hardware {

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
void reset();

/// \brief Updates sensor data and handles physical device control
void loop();

}

#endif // HARDWARE_H

