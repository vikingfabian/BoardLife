#include <Arduino.h>
#include <Arduino_BuiltIn.h>
#include <stdint.h>
#include "RotaryEncoder.h"

#ifndef INPUTH

#define INPUTH

class Input{
public:
  static Input Single;
  
  RotaryEncoder *rotInput;

  unsigned long lastDebounceTime = 0;

  uint8_t ActionPin = A0;
  uint8_t SetResetPin = A1;
  uint8_t ArrowUpPin = 13;
  uint8_t ArrowDownPin = 12;
  uint8_t ModeLifePin = 11;
  uint8_t ModeTimePin = 10;
  uint8_t ModeDicePin = 9;

  bool actionVal = false;
  bool setResetVal = false;
  bool arrowUpVal = false;
  bool arrowDown = false;
  bool modeLifeVal = false;
  bool modeTimeVal = false;
  bool modeDiceVal = false;

  bool actionEvent = false;
  bool setResetEvent = false;
  bool arrowUpEvent = false;
  bool arrowDownEvent = false;
  bool modeLifeEvent = false;
  bool modeTimeEvent = false;
  bool modeDiceEvent = false;

  void init(RotaryEncoder *rotInput);
  void loop();
  void printTestInput();
};

#endif