#include <iostream>
#include <chrono>
#include <cstring>
#include <cctype>

#include "XBee_IO.h"
#include "Default_Config.h"
#include "control.h"
#include "Transmission.h"
#include "Hardware.h"
#include "Assert.h"

/************ Global Variables ************/
// const unsigned long XBeeIO::BUFF_INPUT = 1024;
// unsigned char XBeeIO::input_buff[XBeeIO::BUFF_INPUT];
// unsigned long XBeeIO::buff_length = 0;
// const unsigned long XBeeIO::OUTPUT_BUFF_LENGTH = 255;
// unsigned char XBeeIO::output_buff[XBeeIO::OUTPUT_BUFF_LENGTH];
std::deque<unsigned char> XBeeIO::input_buff, XBeeIO::output_buff;

/* Data */
unsigned char XBeeIO::PACKET_SIZE = 30;

std::fstream XBeeIO::XBee("infile.txt", std::ios::in | std::ios::out);

/**
 * Recieves input from the XBee and saves it to "input_buff"
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the input_buffer was updated
 */
bool XBeeIO::update_input_buffer()
{
    size_t rptr = XBee.tellg();
    XBee.seekg(0, std::ios::end);
    size_t end = XBee.tellg();
    if (rptr > end) rptr = 0;
    XBee.seekg(rptr);

	char char_in;
	while (rptr < end) 
    {
        char_in = XBee.get();
		input_buff.push_back(char_in);
        ++rptr;
	}
	return true;
}

/**
 * Transmits data over the XBee antenna
 * 
 * INPUT
 * int type -> data packet to transmit
 */
void XBeeIO::transmit_data(unsigned int type) {

	using namespace Transmission;

	/* For debugging only */
	if (type == 0xFF) {
		transmit_data_string();
		return;
	}
	
	// Build packet and transmit it
	unsigned int len = 0;
	buildPacket(output_buff, len, type);
	// XBee.write((char*) output_buff, len);
	return;
}

/** 
 * Parses the "input_buff" character array
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the buffer was parsed succesfully
 */
bool XBeeIO::parse_input_buffer()
{
	using namespace Default_Config;
	using namespace State_Data;

	// Empty buffer
	if (input_buff.size() < 4) return false;
	
	// Parse binary command
	// Search for header
	unsigned long i = 0; int is_binary = 0;
	while (i < input_buff.size() - 1) // TODO: Make this work with trash data before the header
    {
		if (input_buff[i] == 0xAA && input_buff[i+1] == 0x14)
        { 
			is_binary = true;
			break;
		}
		i++;
	}

	// Decode binary packet
	bool hashdr;
	unsigned long clri = 0;
	if (is_binary)
    {
		while (i < input_buff.size())
        {
			hashdr = false;

			// Search for header
			while (i < input_buff.size())
            {
				if (input_buff[i] == 0xAA && input_buff[i+1] == 0x14)
                {
					i = i + 3;
					hashdr = true;
					break;
				}
				i = i + 1;
			}
		
			// If incomplete data packet, end loop
			if (input_buff.size() < 4 && input_buff.size() < (i + input_buff[i-1] - 1)) {
				return false;
			}
	
			// Binary Lexicon
			if (hashdr)
            {
                std::cout << "RCV MSG TYPE " << (int) input_buff[i] << std::endl;
				if (input_buff[i] == 0x00) // Echo Firmware Version
                {
					transmit_data(0x00);
				}
				if (input_buff[i] == 0x01) // Turn LED ON
                {
					Hardware::turn_LED_on();
				}
				else if (input_buff[i] == 0x02) // Turn LED OFF
                {
					Hardware::turn_LED_off();
				}
				else if (input_buff[i] == 0x03) // Clear input_buffer
                {
					clear_input_buffer();
				}
				else if (input_buff[i] == 0x04) // Set MODE
                {
					MODE = input_buff[i+1];
					transmit_data(0x10); // Transmit the current mode
				}
				else if (input_buff[i] == 0x05) // Unit tests
                {
					transmit_data(0xB0); // Runs the tests
				}
				else if (input_buff[i] == 0x10)
                {
					// Open Stepper Motor
					Hardware::openStepperMotor();
				}
				else if (input_buff[i] == 0x11)
                {
					// Close Stepper Motor
					Hardware::closeStepperMotor();
				}
//				else if (input_buff[i] == 0x20) { // Set parameters
//					data_period_ms = input_buff[i+1] + (input_buff[i+2] << 8);
//					max_time = input_buff[i+3]; 
//	
//					if (data_period_ms == 0) data_period_ms = 10;
//				}
//				else if (input_buff[i] == 0x21) { // Display parameters{
//					XBee.print("DATA_PERIOD: ");
//					XBee.print(data_period_ms);
//					XBee.print(" ms\n");
//					XBee.print("MAX_TIME: ");
//					XBee.print(max_time);
//					XBee.print(" s\n");
//				}
//				else if (input_buff[i] == 0x31) { // Set parameters
//					data_period_ms = input_buff[i+1] + (input_buff[i+2] << 8);
//					if (data_period_ms == 0) data_period_ms = 10;
//				}
				else if (input_buff[i] == 0x36) // Set Output Packet Type
                {
					DATA_OUT_TYPE = input_buff[i+1];
					if (input_buff[i+1] == 0x10)
                    {
						transmit_data_string();
					}
				}
				else if (input_buff[i] == 0x44) // print buffer for debugging
                {
					dispbuff();
				}
				else if (input_buff[i] == 0x50) // Simple Simulation Packet
                {
					// See documentation for data packet structure
					TIME = std::chrono::steady_clock::now();
					DATA_OUT_TYPE  = input_buff[i+0];
					MODE           = input_buff[i+1]; // 5
					STATUS         = input_buff[i+2];
					DATA_P1		   = input_buff[i+3];
					DATA_P2        = input_buff[i+4];
					DATA_T1        = input_buff[i+5];
					DATA_T2        = input_buff[i+6];
					DATA_THR       = input_buff[i+7];
					NEW_DATA       = input_buff[i+8];
				}
				else // unrecognized command
                {
					i--;
					continue;
				}
		
				i += input_buff[i-1];
				clri = i;
			}
		}
		
		if (clri)
        {
			clear_input_buffer(clri);
			return true;
		}
	}

	// Check for soft reset
	i = 3;
	while (i < input_buff.size()) // TODO: Make this work with trash data before the header
    {
		if (input_buff[i] == 0xFF && input_buff[i-1] == 0xFF &&
            input_buff[i-2] == 0xFF && input_buff[i-3] == 0xFF)
        { 
			control::reset();
			return true;
		}
		i++;
	}
	
    /*
	// Text Lexicon
	if (!std::strncmp((char*) input_buff, "LED ON", 6))
    {
		Hardware::turn_LED_on();
	}
	else if (!std::strncmp((char*) input_buff, "LED OFF", 7))
    {
		Hardware::turn_LED_off();
	}
	else
	{
		return false;
	}
    */

    return true;
}

/** 
 * Clears the "input_buff" character array
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the array was cleared
 */
bool XBeeIO::clear_input_buffer(unsigned long ii)
{
    input_buff.clear();
	return true;
}

/**
 * Displays hex values of the buffer to the XBee port
 */
void XBeeIO::dispbuff()
{
    std::cout << "BUFFER (" << input_buff.size() << "):";
    for (auto e : input_buff)
        std::cout << " " << std::hex << (int) e;
    std::cout << " | ";
    for (auto e : input_buff) {
        if (std::isprint(e))
            std::cout << e;
        else
            std::cout << ".";
    }
    std::cout << " |";
    std::cout << std::endl;
}

/* Transmits the full data string in ASCII */
void XBeeIO::transmit_data_string()
{
    using namespace State_Data;
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now().time_since_epoch()).count();
    XBee << time << "," << STATUS << "," << DATA_P1
         << "," << DATA_P2 << "," << DATA_T1 << "," << DATA_T2
         << "," << DATA_T3 << "," << DATA_THR << ","
         << NEW_DATA << "," << MODE << "," << "ENDL" << __LF__;
}
