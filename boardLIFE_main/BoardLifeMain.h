#include <Arduino_BuiltIn.h>
#include <Arduino.h>
#include <stdint.h>
#include <EEPROM.h>
#include "Ref.h"
#include "Input.h"
#include "Mode.h"
#include "SelectMode.h"
#include "RotaryEncoder.h"

#ifndef BoardLifeMainH

#define BoardLifeMainH
//Respresents the four buttons on the top

//enum ModeType : byte;

class BoardLifeMain{
  public:

  static BoardLifeMain Single;
  ModeManager *modes;

  SelectMode *modeInput1;
  SelectMode *modeInput2;
  SelectMode *modeInput3;

  //RotaryEncoder *rotInput;
  
  BoardLifeMain();

  void init();

  void storageSetup(bool factoryReset);

  void loop();

};



#endif