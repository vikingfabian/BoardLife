#define MULTIPLEX_LEDCOUNT 7

#include "SevenSegmentDisplay.h"
#include "ShiftRegister.h"

#ifndef MULTIPLEXH

#define MULTIPLEXH

class Multiplex
{
public:
  byte totalCommonCount;
  static byte DisplayLedToRegister[];
  static const byte DisplayDotToRegister;
  static byte DisplayCommonToRegister[];

  byte currentCommon = 0;
 
  static Multiplex Single;

  ShiftRegister *shiftReg = nullptr;
  
  SevenSegmentDisplay *displayRed =nullptr;
  SevenSegmentDisplay *displayGreen =nullptr;

  void setupDisplays();
  void setupDisplayTestMode();
  void multiplexNext();
  void multiplexNext_Test();
};



void setupTimerInterrupt();
#endif