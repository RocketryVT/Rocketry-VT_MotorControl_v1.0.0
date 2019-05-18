/*! \file */

#ifndef CONTROL_MAIN_H
#define CONTROL_MAIN_H

enum exit_code : int
{
    execution_complete = 0,
    soft_shutdown = 1,
    sigint = 2,
    connection_timeout = 5,
    failed_channel_map = 6,
    termios_get_fail = 10,
    termios_set_fail = 11,
    termios_baud_fail = 12,
    not_a_tty = 13
};

/// \brief Contains administrative functions which form
/// the backbone of the controller flow of control.
namespace control
{

/// \brief Initializes all motor controller subsystems.
/// \return true on success, false otherwise.
bool init();

/// \brief Runs all system daemons, such as sensor reading,
/// logging, hardware management, communications, etc.
void loop();

/// \brief Resets the controller to its startup state.
void reset();

/// \brief Checks if the controller is cool and groovy.
/// \return true if cool and groovy, false on error.
bool ok();

/// \brief Shutdown the controller.
/// \param code The reason for shutdown.
void exit(int code);

} // namespace control

#endif // CONTROL_MAIN_H
