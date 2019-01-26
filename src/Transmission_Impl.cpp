#include <iostream>
#include <iomanip>
#include <sstream>

#include "Test_all.h"
#include "Transmission.h"
#include "Hardware.h"
#include "control.h"
#include "config.h"
#include "Assert.h"
#include "XBee_IO.h"

/**
* Compiles data to a char array so that it can be pushed to the
* serial port directly. Also this makes it easier to compute a
* checksum on the data
*
* INPUTS
* unsigned int type -> data transmission packet type (see documentation)
* 
* OUTPUTS
* a std::vector containing the data packet
*/
std::vector<unsigned char> Transmission::buildPacket(unsigned int type)
{
	using namespace State_Data;

    // packet and header
    std::vector<unsigned char> packet { 0xAA, 0x14 };

	// Chars used to store float bytes
    uint32_t msec = std::chrono::duration_cast<std::chrono::milliseconds>
        (cfg::DATA_TIME - cfg::START_TIME).count();
	
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

            packet << (uint8_t) 7 << (uint8_t) 0x10 << (uint8_t) MODE;
            break;

        case 0x40: // Solid Motor Static Fire Tests 2018-11-11

            packet << (uint8_t) 22 << (uint8_t) 0x40
                << (uint32_t) msec << (uint8_t) MODE
                << (uint16_t) STATUS << (float) DATA_T1
                << (float) DATA_THR << (uint8_t) NEW_DATA;
            break;

        case 0x51: // Cold flow test data

            packet << (uint8_t) 34 << (uint8_t) 0x51
                << (uint32_t) msec << (uint8_t) MODE << (uint16_t) STATUS
                << (float) DATA_P1 << (float) DATA_P2
                << (float) DATA_T1 << (float) DATA_T2
                << (float) DATA_THR << (uint8_t) NEW_DATA;
            break;

        case 0x52: // Cold flow test data

            packet << (uint8_t) 38 << (uint8_t) 0x52
                << (uint32_t) msec << (uint8_t) MODE << (uint16_t) STATUS
                << (float) DATA_P1 << (float) DATA_P2 << (float) DATA_T1
                << (float) DATA_T2 << (float) DATA_T3 << (float) DATA_THR
                << (uint8_t) NEW_DATA;
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

// called upon the successful receipt of a data packet
// the argument data should be stripped of the header,
// length, and checksum bytes
// returns true if successful, false if error encountered
bool Transmission::dataReceipt(const std::vector<unsigned char> &data)
{
    using namespace cfg;
    using namespace State_Data;
    using namespace XBeeIO;

    if (data.size() == 0)
    {
        #ifdef DEBUG
        std::cout << "Recieved empty packet!" << std::endl;
        #endif
        return false;
    }

    #ifdef DEBUG
    std::cout << "Recieved packet with data of length "
        << data.size() << std::endl;
    #endif

    unsigned char type = data[0];
    bool ascii = false;

    switch (type)
    {
        case 0x00: transmit_data(0x00); // echo firmware
                   break;
        
        case 0x01: Hardware::setLED(true); // turn LED on
                   break;

        case 0x02: Hardware::setLED(false); // turn LED off
                   break;

        // why would we need to clear the input buffer?
        case 0x03: // input_buff.clear(); // clear the input buffer
                   break;

        case 0x04: if (data.size() < 2) break;
                   MODE = data[1]; // set mode
                   transmit_data(0x10);
                   break;

        case 0x05: transmit_data(0xB0); // run unit tests
                   break;

        case 0x10: Hardware::openStepperMotor(); // open motor
                   break;

        case 0x11: Hardware::closeStepperMotor(); // close motor
                   break;

                   // set parameters
        case 0x20: // cfg::data_period = data[1] + (data[2] << 8);
                   // max_time = data[3];
                   break;

        case 0x21: // XBee.print("DATA_PERIOD: "); // print params
                   // XBee.print(data_period_ms);
                   // XBee.print(" ms\n");
                   // XBee.print("MAX_TIME: ");
                   // XBee.print(max_time);
                   // XBee.print(" s\n");
                   break;

                   // 0x23 - '#'
        case 0x23: ascii = true; // parse ascii message
                   break;

        case 0x36: if (data.size() < 2) break; // edit params
                   DATA_OUT_TYPE = data[1];
                   if (DATA_OUT_TYPE == 0x10)
                       transmit_data_string();
                   break;

        case 0x44: dispbuff(); // display buffers
                   break;

                   // simulation packet
        case 0x50: if (data.size() < 9) break;
                   TIME = std::chrono::steady_clock::now();
                   DATA_OUT_TYPE    = data[0];
                   MODE             = data[1];
                   STATUS           = data[2];
                   DATA_P1          = data[3];
                   DATA_P2          = data[4];
                   DATA_T1          = data[5];
                   DATA_T2          = data[6];
                   DATA_THR         = data[7];
                   NEW_DATA         = data[8];
                   break;

        case 0xFF: control::reset(); // soft reset
                   break;

        default:   
                   #ifdef DEBUG
                   std::cout << "It's free real estate: 0x"
                       << std::hex << (int) data[0] << std::endl;
                   #endif
                   return false;
                   
    }
   
    if (!ascii) return true;

    std::string msg(data.begin(), data.end());
    #ifdef DEBUG
    std::cout << "Recieved new ASCII message: \""
        << msg << "\"" << std::endl;
    #endif
 
    if (msg == "#LED ON")
    {
        Hardware::setLED(true);
    }
    else if (msg == "#LED OFF")
    {
        Hardware::setLED(false);
    }
    else if (msg == "#VERSION")
    {
        #ifdef DEBUG
        std::cout << "Current firmware version is "
            << cfg::version << std::endl;
        #endif
    }
    else if (msg == "#SAY HI")
    {
        #ifdef DEBUG
        std::cout << "Hello, world!" << std::endl;
        #endif
    }
    else if (msg == "#BEST SUBTEAM?")
    {
        #ifdef DEBUG
        std::cout << "Software is the best subteam!" << std::endl;
        #endif
    }
    else if (msg == "#WHAT TEAM?")
    {
        #ifdef DEBUG
        std::cout << "WILDCATS" << std::endl;
        #endif
    }
    else
    {
        #ifdef DEBUG
        std::cout << "Unknown ASCII message: \""
            << msg << "\"" << std::endl;
        #endif
        return false;
    }
	
    return true;
}

