#ifndef XBEE_IO_H
#define XBEE_IO_H

namespace XBeeIO {

/************ Global Variables ************/
extern const unsigned char BUFF_INPUT;
extern unsigned char input_buff[];
extern unsigned long buff_length;
extern const unsigned char OUTPUT_BUFF_LENGTH;
extern unsigned char output_buff[];


/* Data */
extern unsigned char PACKET_SIZE;

/**
 * Recieves input from the XBee and saves it to "input_buff"
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the input_buffer was updated
 */
bool update_input_buffer();

/**
 * Transmits data over the XBee antenna
 * 
 * INPUT
 * int type -> data packet to transmit
 */
void transmit_data(unsigned int type);

/** 
 * Parses the "input_buff" character array
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the buffer was parsed succesfully
 */
bool parse_input_buffer();

/** 
 * Clears the "input_buff" character array
 * 
 * INPUT
 * void
 * 
 * RETURN
 * bool -> true if the array was cleared
 */
bool clear_input_buffer(unsigned long ii = 0);


/**
 * Displays hex values of the buffer to the XBee port
 */
void dispbuff();

/* Transmits the full data string in ASCII */
void transmit_data_string();

}

#endif // XBEE_IO_H
