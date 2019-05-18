#include <iostream>
#include <chrono>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <bitset>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

#include <comms.h>
#include <logging.h>
#include <config.h>
#include <control.h>
#include <transmission.h>
#include <behavior.h>
#include <hardware.h>
#include <Assert.h>

namespace comms
{

std::deque<unsigned char> input_buff, output_buff;
int arduino;
std::ofstream out;
const std::string filepath = "/dev/serial/by-id/usb-Arduino_"
        "Srl_Arduino_Mega_85535303636351714190-if00";
bool fail_flag = false;
bool init()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    arduino = open(filepath.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY | O_SYNC);
    if (arduino < 0)
    {
        logging::announce("Failed to open rx comms device: \"" +
            filepath + "\"", true, true);
        fail_flag = true;
        return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    logging::announce("Opened comms device: " + filepath, true, true);
    return true;
}

bool ok()
{
    return !fail_flag;
}

void loop()
{
    char char_in[1];
    while (read(arduino, char_in, 1) > 0)
    {
	    input_buff.push_back(char_in[0]);
    }

    auto packets = transmission::parse(input_buff);

    for (auto p : packets)
    {
        std::vector<unsigned char> data;
        for (size_t i = 4; i < p.size() - 2; ++i)
            data.push_back(p[i]);
        behavior::dataReceipt(p[3], data);
    }
}

// clears input and output buffers
void reset()
{
    input_buff.clear();
    output_buff.clear();

    close(arduino);
    init();
}

// queues a vector of chars onto the output buffer
void transmit(const std::vector<unsigned char> &data)
{
    for (auto e : data) output_buff.push_back(e);
}

// queues a string onto the output buffer
void transmit(const std::string& str)
{
    transmit(transmission::buildPacket(str));
}

void flush()
{
    std::vector<unsigned char> outgoing;

    while (output_buff.size() > 0)
    {
        unsigned char byte = output_buff.front();
        int ret = write(arduino, &byte, 1);
        if (ret < 0)
        {
            std::cout << "An error occurred while transmitting." << std::endl;
            std::cout << std::string(std::strerror(errno)) << std::endl;
        }
        else output_buff.pop_front();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

} // namespace comms

