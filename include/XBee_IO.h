#ifndef XBEE_IO_H
#define XBEE_IO_H

#include <fstream>
#include <deque>
#include <vector>

namespace XBeeIO
{

// initializes xbee interface, buffers, etc
// returns true on success, false otherwise
bool init();

// checks if anything is wrong
// returns true on ok, false otherwise
bool ok();

// Recieves input from the XBee and saves it to "input_buff"
// bool -> true if the input_buffer was updated
bool update_input_buffer();

// queues a single char onto the output buffer
void transmit(unsigned char data);

// queues a vector of chars onto the output buffer
void transmit(std::vector<unsigned char> data);

// queues a string onto the output buffer
void transmit(const std::string& str);

// transmits data over the XBee antenna
void transmit_data(unsigned int type);

// flushes data in the output buffer to the antenna
void flush();

// Parses the "input_buff" character array
void parse();

// clears the input and output buffers
void reset();

// displays hex values of the buffer to the XBee port
void dispbuff();

/* transmits the full data string in ASCII */
void transmit_data_string();

} // namespace XBeeIO

#endif // XBEE_IO_H
