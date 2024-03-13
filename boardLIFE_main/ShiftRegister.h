#include <Arduino.h>
#include <Arduino_BuiltIn.h>
#include <stdint.h>

#ifndef SHIFTREGH

#define SHIFTREGH

class ShiftRegister{
public:
  //74HC595 16 pin Shift Register
  //Kom ihåg, Reset pin 10 -> HIGH, Enable pin 13 -> LOW
  byte latchPin;// shift reg pin 12
  byte clockPin;// shift reg pin 11
  byte dataPin;// shift reg pin 14  
 
 
  unsigned int dataOut = 0;       // Variable which will save the value of dataOuts
  byte serieLength; //Put shift registers in series, serial out pin 9, most logical to let the series go right to left
 
  ShiftRegister(byte latchPin, byte clockPin, byte dataPin, byte serieLength);
 
  void SendData();
 
  void setBit(int position, bool value);
};
 
#endif

 
 

