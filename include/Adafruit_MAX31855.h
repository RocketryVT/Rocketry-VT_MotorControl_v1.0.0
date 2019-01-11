/*************************************************** 
  This is a library for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef ADAFRUIT_MAX31855_H
#define ADAFRUIT_MAX31855_H

class Adafruit_MAX31855 {
 public:
  Adafruit_MAX31855(unsigned char _sclk, unsigned char _cs, unsigned char _miso);
  

  void begin(void);
  
  float readCelsius(void);
  float readFarenheit(void);
  unsigned char readError();

 private:
  bool initialized;

  unsigned char sclk, miso, cs;
  unsigned long spiread32(void);
};

#endif
