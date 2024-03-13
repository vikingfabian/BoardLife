#include <Arduino_BuiltIn.h>
#include <Arduino.h>
#include <stdint.h>
#include "ModeManager.h"
  
#ifndef MODEINPUTH

#define MODEINPUTH


class SelectMode{
  public:
  byte modeButtonIndex;
  ModeType modetype;
  //byte modeIndex;
  //byte modeIndexCount;
  // bool prevValue; 
  // long keyChangeTime;
  // static const long ResetTime = 2000;

  SelectMode(byte modeButtonIndex);

  void loop();

  void selectModeEvent(bool value, ModeManager *modes);
};

#endif