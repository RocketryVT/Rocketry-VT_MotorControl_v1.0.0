#include <iostream>
#include <chrono>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <bitset>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
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
const std::string filepath = "/dev/ttyACM0";
bool fail_flag = false;

bool init()
{
    struct termios config;
    arduino = open(filepath.c_str(), O_RDWR | O_NONBLOCK |
                                     O_NOCTTY | O_SYNC);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (arduino < 0)
    {
        logging::announce("Failed to open rx comms device: \"" +
            filepath + "\"", true, true);
        fail_flag = true;
        return false;
    }

    if (!isatty(arduino))
    {
        logging::announce("Provided comms device (\"" +
            filepath + "\") is not a TTY device", true, true);
        control::exit(exit_code::not_a_tty);
    }

    if (tcgetattr(arduino, &config) < 0)
    {
        logging::announce("Failed to get termios settings.", true, true);
        control::exit(exit_code::termios_get_fail);
    }

    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR |
                        PARMRK | INPCK | ISTRIP | IXON);
    config.c_oflag = 0;
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= CS8;
    config.c_cc[VMIN] = 0;
    config.c_cc[VTIME] = 0;

    if (cfsetispeed(&config, B921600) < 0 ||
        cfsetospeed(&config, B921600) <0)
    {
        logging::announce("Failed to set baudrate.", true, true);
        control::exit(exit_code::termios_baud_fail);
    }

    if (tcsetattr(arduino, TCSAFLUSH, &config) < 0)
    {
        logging::announce("Failed to set termios settings.", true, true);
        control::exit(exit_code::termios_set_fail);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    logging::announce("Opened comms device: " + filepath, true, true);
    logging::announce("== Hello! ==", true, true);
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

    flush();
}

// clears input and output buffers
void reset()
{
    flush();
    input_buff.clear();
    output_buff.clear();
    close(arduino);
    init();
}

// exits the comms module
void exit(int)
{
    logging::announce("== Goodbye. ==", true, true);
    flush();
    input_buff.clear();
    output_buff.clear();
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
    int32_t iters = 10000;
    bool send_callsign = output_buff.size() > 0;
    while (output_buff.size() > 0 && --iters > 0)
    {
        unsigned char byte = output_buff.front();
        int ret = write(arduino, &byte, 1);
        fsync(arduino);
        if (ret < 1)
        {
            logging::announce("Warning: radio overloaded.", true, true);
            std::cout << std::string(std::strerror(errno)) << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else output_buff.pop_front();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    if (send_callsign)
    {
        const char *callsign = "KM4BTL";
        write(arduino, callsign, 6);
    }
}

} // namespace comms

