

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

#include "Adafruit_MAX31855.h"
#include "config.h"
#include "XBee_IO.h"

const unsigned char LOW = 0;
const unsigned char HIGH = 1;

Adafruit_MAX31855::Adafruit_MAX31855(unsigned char _sclk, unsigned char _cs, unsigned char _miso) {
  sclk = _sclk;
  cs = _cs;
  miso = _miso;

  initialized = false;
}



void Adafruit_MAX31855::begin(void) {
  // define pin modes
  // pinMode(cs, OUTPUT);
  // digitalWrite(cs, HIGH);

  if (sclk == -1) {
    // hardware SPI
    //start and configure hardware SPI
    // SPI.begin();
  } else {
    // pinMode(sclk, OUTPUT);
    // pinMode(miso, INPUT);
  }
  initialized = true;
}



float Adafruit_MAX31855::readCelsius(void) {
  

  int32_t v;

  // XBeeIO::update_input_buffer();
  
  // Close and open serial ports b/c sometimes things just mess up
  
  v = spiread32();
 
  //Serial.print("0x"); Serial.println(v, HEX);

  /*
  float internal = (v >> 4) & 0x7FF;
  internal *= 0.0625;
  if ((v >> 4) & 0x800)
    internal *= -1;
  Serial.print("\tInternal Temp: "); Serial.println(internal);
  */

   if (v & 0x7) {
    // uh oh, a serious problem!
    return 0;
  }
  if (v & 0x80000000) {
    // Negative value, drop the lower 18 bits and explicitly extend sign bits.
    v = 0xFFFFC000 | ((v >> 18) & 0x00003FFFF);
  }
  else {
    // Positive value, just drop the lower 18 bits.
    v >>= 18;
  }
  //Serial.println(v, HEX);

  float centigrade = v;

  // LSB = 0.25 degrees C
  centigrade *= 0.25;
  return centigrade;
}

unsigned char Adafruit_MAX31855::readError() {
  return spiread32() & 0x7;
}

float Adafruit_MAX31855::readFarenheit(void) {
  float f = readCelsius();
  f *= 9.0;
  f /= 5.0;
  f += 32;
  return f;
}

unsigned long Adafruit_MAX31855::spiread32(void) {
  int i;
  unsigned long d = 0;

  // backcompatibility!
  if (! initialized) {
    begin();
  }

  //  digitalWrite(cs, LOW);
  //delay(1);

  if(sclk == -1) {
    // hardware SPI
  
    /*  
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

    d = SPI.transfer(0);
    d <<= 8;
    d |= SPI.transfer(0);
    d <<= 8;
    d |= SPI.transfer(0);
    d <<= 8;
    d |= SPI.transfer(0);

    SPI.endTransaction();
    */

  } else {
    // software SPI

    /*
    digitalWrite(sclk, LOW);
    //delay(1);

    for (i=31; i>=0; i--) {
      digitalWrite(sclk, LOW);
      //delay(1);
      d <<= 1;
      if (digitalRead(miso)) {
  d |= 1;
      }

      digitalWrite(sclk, HIGH);
      //delay(1);
    }
    */
  }

  // digitalWrite(cs, HIGH);
    
  // Serial.println(d, HEX);
  return d;
}
