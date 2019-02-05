#ifndef XBEE_IO_H
#define XBEE_IO_H

#include <fstream>
#include <deque>
#include <vector>

namespace comms
{

// initializes xbee interface, buffers, etc
// returns true on success, false otherwise
bool init();

// checks if anything is wrong
// returns true on ok, false otherwise
bool ok();

// clears the input and output buffers
void reset();

// process the things
void loop();

// queues a vector of chars onto the output buffer
void transmit(std::vector<unsigned char> data);

// queues a string onto the output buffer
void transmit(const std::string& str);

// flushes data in the output buffer to the antenna
void flush();

} // namespace XBeeIO

#endif // XBEE_IO_H
