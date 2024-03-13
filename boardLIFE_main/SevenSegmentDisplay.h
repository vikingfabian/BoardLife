#include <Arduino.h>
#include <Arduino_BuiltIn.h>
#include <stdint.h>
#include "Ref.h"
 
#define MULTIPLEX_LEDCOUNT 7

#ifndef SEVENSEGDISPLAYH

#define SEVENSEGDISPLAYH
class SevenSegmentDisplay{
  public:
// 7-seg layout
 
//    A
//  F   B
//    G
//  E   C
//    D   DP
    //static const int DotLed; 

    static bool chararcterLayouts_charIx_ledOn[28][MULTIPLEX_LEDCOUNT];
 
    //static byte LedIdToPin[MULTIPLEX_LEDCOUNT];
 
    byte CommonCount;
    byte numberStringInvLength;
    // static byte commonIxToPin[];
 
    //byte currentCommon = 0;
 
    byte *DisplayString;//[CommonCount];
    bool *Dots;
 
    SevenSegmentDisplay(byte commonCount);
 
    void setNumber(int number, byte emptyChar = 0);
 
    void setAllDots(bool value);

    void setDotsFromLeft(int count);
    
    void multiplexNext();

    void clearChars();
};
 #endif
 

 

