#ifndef HARDWARE_H
#define HARDWARE_H

#include <fstream>

#include "config.h"

namespace hardware {

// initializes logging and sensors; returns true if ok
bool init();

// checks if hardware is ok
// returns true on ok
bool ok();

// closes and flushes logs, closes sensor connections
void exit(int code);

// resets all hardware things
void reset();

// Updates data variables by calling functions that control data
// acquisition from connected devices
void loop();

}

#endif // HARDWARE_H

