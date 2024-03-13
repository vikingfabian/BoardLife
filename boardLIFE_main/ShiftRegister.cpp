#include "ShiftRegister.h"


//al out pin 9, most logical to let the series go right to left
 
  ShiftRegister::ShiftRegister(byte latchPin, byte clockPin, byte dataPin, byte serieLength){
    this->latchPin = latchPin;
    this->clockPin = clockPin;
    this->dataPin = dataPin;
    this->serieLength = serieLength;
 
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);  
    pinMode(clockPin, OUTPUT);
  }
 
  void ShiftRegister::SendData()
  {
    unsigned int nextByte = dataOut;
    digitalWrite(latchPin, LOW);
    //MSBFIRST LSBFIRST  
    for (int i = 0; i < serieLength; ++i){
      shiftOut(dataPin, clockPin, MSBFIRST, nextByte);
      nextByte = nextByte >> 8;
    }
    digitalWrite(latchPin, HIGH);
  }
 
  void ShiftRegister::setBit(int position, bool value){
    if (value)
      bitSet(dataOut, position);
    else
      bitClear(dataOut, position);
  }
 
// void ShiftRegister::setup()
// {
//   Serial.begin(9600);
 
//   Serial.println("INIT");
 
 
//  shiftReg = new ShiftRegister(A5, A4, A3, 2);
 
// }
 
// void loop()
// {
//   if (shiftReg->dataOut == 0){
//     shiftReg->dataOut = 255;
//   }
//   else{
//     shiftReg->dataOut = 0;
//   }
//   shiftReg->SendData(); // turn on the  next dataOut
//   delay(500);
//   for (int i = 0; i < 16; i++)   // loop that will turn the dataOut one by one
//   {
//     shiftReg->setBit(i, true); // assigning the respective dataOut the HIGH values
//     shiftReg->SendData();// turn off the previous dataOut
//     delay(500);
//   }
 
// }
 
 
