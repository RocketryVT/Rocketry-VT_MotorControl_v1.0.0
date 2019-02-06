#include <iostream>
#include <map>
#include <functional>

#include <behavior.h>
#include <logging.h>
#include <transmission.h>
#include <hardware.h>
#include <control.h>
#include <config.h>
#include <comms.h>

namespace behavior
{

std::map<uint8_t, std::function<void(std::vector<uint8_t>)>>
on_receive
{

{0x06, [] (std::vector<uint8_t>)
{
    state::last_ping = std::chrono::steady_clock::now();
}},

{0x07, [] (std::vector<uint8_t> data)
{
    if (data.size() == 4)
    {
        float f;
        data >> f;
        state::o2p = f;
    }
}},

{0x23, [] (std::vector<uint8_t> data)
{
    std::string msg(data.begin(), data.end());
    #ifdef DEBUG
    std::cout << "Recieved new ASCII message: \""
        << msg << "\"" << std::endl;
    #endif

    if (msg == "MARCO")
    {
        state::last_ping = std::chrono::steady_clock::now();
        std::cout << "Polo!" << std::endl;
    }
    else if (msg == "VERSION")
    {
        std::cout << "Current firmware version is "
            << cfg::version << std::endl;
    }
    else if (msg == "SAY HI")
    {
        std::cout << "Hello, world!" << std::endl;
    }
    else if (msg == "BEST SUBTEAM?")
    {
        std::cout << "Software is the best subteam!" << std::endl;
    }
    else if (msg == "WHAT TEAM?")
    {
        std::cout << "WILDCATS" << std::endl;
    }
    else if (msg == "RESET")
    {
        control::reset();
    }
    else if (msg == "SHUTDOWN")
    {
        control::exit(1); // soft shutdown
    }
    else
    {
        #ifdef DEBUG
        std::cout << "Unknown ASCII message: \""
            << msg << "\"" << std::endl;
        #endif
    }
}},

{0xFE, [] (std::vector<uint8_t> data)
{
    if (data.size() < 1) return;
    control::exit(data[0]);
}},

{0xFF, [] (std::vector<uint8_t>)
{
    control::reset();
}}

}; // on_receive

// called upon the successful receipt of a data packet
// the argument data should be stripped of the header,
// length, and checksum bytes
// returns true if successful, false if error encountered
bool dataReceipt(uint8_t id, const std::vector<uint8_t> &data)
{
    auto action = on_receive.find(id);
    if (action != on_receive.end())
        action->second(data);
    else
    {
        #ifdef DEBUG
        std::cout << "It's free real estate: 0x"
            << std::hex << (int) id << std::endl;
        #endif
    }
    return true;
}

} // namespace behavior

/*
// Compiles data to a char array so that it can be pushed to the
// serial port directly. Also this makes it easier to compute a
// checksum on the data
std::vector<unsigned char> transmission::buildPacket(unsigned int type)
{
    // packet and header
    std::vector<unsigned char> packet { 0xAA, 0x14 };

	// Chars used to store float bytes
    uint32_t msec = std::chrono::duration_cast<std::chrono::milliseconds>
        (state::time - cfg::start_time).count();
	
	switch (type)
    {
                   // echo version
        case 0x00: return buildPacket(cfg::version);

        case 0x01: // SD Card Begin of Save File

            packet << (uint8_t) 6 << (uint8_t) 0x01;
            break;

        case 0x02: // SD Card End of Save File

            packet << (uint8_t) 6 << (uint8_t) 0x02;
            break;

        case 0x10:

            packet << (uint8_t) 7 << (uint8_t) 0x10 << (uint8_t) state::mode;
            break;

        case 0x40: // Solid Motor Static Fire Tests 2018-11-11

            packet << (uint8_t) 22 << (uint8_t) 0x40
                << (uint32_t) msec << (uint8_t) state::mode
                << (uint16_t) state::status << (float) state::t1
                << (float) state::thrust << (uint8_t) state::new_data;
            break;

        case 0x51: // Cold flow test data

            packet << (uint8_t) 34 << (uint8_t) 0x51
                << (uint32_t) msec << (uint8_t) state::mode << (uint16_t) state::status
                << (float) state::p1 << (float) state::p2
                << (float) state::t1 << (float) state::t2
                << (float) state::thrust << (uint8_t) state::new_data;
            break;

        case 0x52: // Cold flow test data

            packet << (uint8_t) 38 << (uint8_t) 0x52
                << (uint32_t) msec << (uint8_t) state::mode << (uint16_t) state::status
                << (float) state::p1 << (float) state::p2 << (float) state::t1
                << (float) state::t2 << (float) state::t3 << (float) state::thrust
                << (uint8_t) state::new_data;
            break;

        case 0xB0: // Do Unit Tests

            Test::run_tests(true);
            return buildPacket(Test::results_string);

        default: // Do nothing

            packet.clear();
            return packet;
	}
    appendChecksum(packet);
    return packet;
}
*/

