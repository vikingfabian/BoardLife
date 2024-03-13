#include "ModeDice.h"

#define DICESTATE_REST 0
#define DICESTATE_HOLD 1
#define DICESTATE_ROLL 2
#define DICESTATE_RESULT 3

//DICE
unsigned long ModeDice::stateTime = 0;

static const byte ModeDice::SUBMODE_ONE = 0;
static const byte ModeDice::SUBMODE_TWO = 1;
static const byte ModeDice::SUBMODE_BUZZERS = 2; 
static const byte ModeDice::SUBMODE_DEBUG = 3;

ModeType ModeDice::type() { return ModeType::Dice; }
byte ModeDice::subModeIndexCount() { return 4; } 

void ModeDice::loop(){
  Mode::loop();

  if (this->introState == 0)
  {
    unsigned long time = millis() - stateTime;  
    if (subModeIndex == SUBMODE_BUZZERS)
    {   
      if (buzzPlayer == 0)
      {     
        if (time > 1000)
        {
          stateTime = millis();
          buzzTotalSec++;
          bRefreshDisplay = true;
        }
      }
      else
      {
        if (time > 500)
        {
          stateTime = millis();
          buzzTotalSec++;
          bRefreshDisplay = true;
        }
      }
    }
    else if (subModeIndex != SUBMODE_DEBUG)
    {
      if (state_restHoldRollResult == DICESTATE_HOLD)
      {
          roll();
      }
      else if (state_restHoldRollResult == DICESTATE_ROLL)
      {      
        roll();
        if (time > 300)
        {
            Serial.println("sprak!");
            state_restHoldRollResult = DICESTATE_RESULT;
            rollUpdateTime = 0;
            bRefreshDisplay = true;
        }     
      }
    }     
  }
}

void ModeDice::display_Debug(bool display1)
{
  // Serial.println("state_pausePlayAlertDone");
  Serial.println("display_Debug");
  
  SevenSegmentDisplay *display = display1? Multiplex::Single.displayRed : Multiplex::Single.displayGreen;
  
  byte character = Ref::CHAR_NUM + debugDisplayNumber;
  bool dot = false;
  if (debugDisplayNumber == 0)
  {
    character = Ref::CHAR_MINUS;
  }
  else if (debugDisplayNumber == 9)
  {
    character = Ref::CHAR_NUM + 8;
    dot = true;

    Serial.println("dot = true");
  }

  for (int charIndex = 0; charIndex < debugDisplayCharCount; ++charIndex)
  {   
    int pos = (display->CommonCount - 1) -( charIndex + debugDisplayPosition);
    pos %=  display->CommonCount;

    display->DisplayString[pos] = character;
    display->Dots[pos] = dot;    
  }
}

void ModeDice::refeshDisplay() 
{  
  if (subModeIndex == SUBMODE_BUZZERS)
  {
    if (buzzPlayer == 0)
    {
      Multiplex::Single.displayRed->DisplayString[0] = Ref::CHAR_b;
      Multiplex::Single.displayRed->DisplayString[1] = Ref::CHAR_U;
      Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_Z;
      Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_Z;

      Multiplex::Single.displayGreen->clearChars();          
      int thirds = buzzTotalSec % 3;
      Multiplex::Single.displayGreen->DisplayString[2 - thirds] = Ref::CHAR_MINUS;
    }
    else if (buzzPlayer < 100)
    {
      /* Flash player number */
      Multiplex::Single.displayGreen->clearChars();
      Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_MINUS;
      Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_MINUS;
      Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_MINUS;

      Multiplex::Single.displayRed->clearChars();
      if(buzzTotalSec & 1)
      {
        Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_P;
        Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_NUM + buzzPlayer;
        if (buzzTotalSec >= 6)
        {
          buzzPlayer = 100; //Idle state, wait for reset
          Multiplex::Single.displayGreen->clearChars();
        }
      }
    }   
     
  }
  else if (subModeIndex == SUBMODE_DEBUG)
  {
    Multiplex::Single.displayRed->setAllDots(false);
    Multiplex::Single.displayGreen->setAllDots(false);
    Multiplex::Single.displayRed->clearChars();
    Multiplex::Single.displayGreen->clearChars();

    switch (debugMode_red_green_both)
    {
      case 0:
        display_Debug(true);
        break;
      case 1:
        display_Debug(false);
        break;    
      default:
        display_Debug(true);
        display_Debug(false);
        break;
    }    
  }  
  else
  {
    if (state_restHoldRollResult > DICESTATE_REST)
    {
        Multiplex::Single.displayRed->setNumber(result);
    } 

    Multiplex::Single.displayGreen->setNumber(diceMax);
    Multiplex::Single.displayGreen->DisplayString[Multiplex::Single.displayGreen->numberStringInvLength - 1] = Ref::CHAR_d;
    
    if (subModeIndex == SUBMODE_TWO && diceMax <= 9)
    {
      Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_NUM + 2;
    }
  }
}

void ModeDice::roll() {
  if (millis() - rollUpdateTime > 60)
  {
    rollUpdateTime = millis();
    result = random(1, diceMax + 1);
    if (subModeIndex == SUBMODE_TWO)
    {
      result += random(1, diceMax + 1);
    }
    bRefreshDisplay = true;
  }
}

void ModeDice::buzzKeydown(byte player)
{
  if (buzzPlayer == 0)
  {
    if (buzzTotalSec > 0) //input lock
    {
      buzzPlayer = player;
      stateTime = millis();
      buzzTotalSec = 0;
      bRefreshDisplay = true;
    }
  }
  else if (buzzPlayer == 100)
  {//Reset
    wheelTotalChange = 0;
    buzzPlayer = 0;
    stateTime = millis() - 500; //set input delay
    buzzTotalSec = 0;
    bRefreshDisplay = true;
  }  
}

void ModeDice::initMode() {
  Mode::initMode();
  diceMax = EEPROM_readInt(Ref::MEM_ADRESS_DICE_MAX);
}

void ModeDice::prepareStorage() {
  EEPROM_writeInt(Ref::MEM_ADRESS_DICE_MAX, 6);
}

void ModeDice::save(){
  EEPROM_writeInt(Ref::MEM_ADRESS_DICE_MAX, diceMax);
}

void  ModeDice::changeMax(int change){
  if (state_restHoldRollResult == DICESTATE_RESULT){
    state_restHoldRollResult = DICESTATE_REST;
  }

  if (state_restHoldRollResult == DICESTATE_REST){
    diceMax += change; 
    diceMax = constrain(diceMax, 2, 100);
  }

  bRefreshDisplay = true;
}

void  ModeDice::input_wheel(int change){
  if (subModeIndex == SUBMODE_DEBUG)
  {   
    debugDisplayNumber = constrain(debugDisplayNumber + change, 0, 9);
    bRefreshDisplay = true;    
  }
  else if (subModeIndex == SUBMODE_BUZZERS)
  {
    wheelTotalChange += change;
    if (wheelTotalChange >= 3)
    {
      buzzKeydown(5);
    }
    else if (wheelTotalChange <= -3)
    {
      buzzKeydown(6);
    }
    
  }  
  else
  { 
    changeMax(change);
  }
}

void  ModeDice::input_levelarrows(int arrowdir, bool value) {
  if (subModeIndex == SUBMODE_BUZZERS)
  {
    if (value == Ref::KeyDown){
      buzzKeydown(arrowdir < 0? 3 : 4);
    }
  }  
  else if (subModeIndex == SUBMODE_DEBUG)
  {
    if (value == Ref::KeyDown)
    {
      debugDisplayCharCount += arrowdir; //action
     if ( debugDisplayCharCount < 1)
     { debugDisplayCharCount = 4; }
     else if ( debugDisplayCharCount > 4)
     { debugDisplayCharCount = 1; }
     
      bRefreshDisplay = true;
    } 
  } 
  else
  {
    if (value == Ref::KeyDown)
    {
      changeMax(arrowdir * 10);
    }
  }
}

void  ModeDice::input_action(bool value) 
{
  if (subModeIndex == SUBMODE_BUZZERS)
  {
    if (value == Ref::KeyDown){
      buzzKeydown(1);
    }
  }  
  else if (subModeIndex == SUBMODE_DEBUG)
  {
    if (value == Ref::KeyDown)
    {
      debugMode_red_green_both++; //action
      if (debugMode_red_green_both >= 3)
      { debugMode_red_green_both = 0; }
      bRefreshDisplay = true;
    } 
  }
  else
  {
    if (value == Ref::KeyDown)
    {
      randomSeed(millis());
      state_restHoldRollResult = DICESTATE_HOLD;
    }
    else
    {
      state_restHoldRollResult = DICESTATE_ROLL;
      stateTime = millis();
    }
  }
}

void ModeDice::input_setReset(bool value) {

  if (subModeIndex == SUBMODE_BUZZERS)
  {
    if (value == Ref::KeyDown){
      buzzKeydown(2);
    } 
  }  
  else if (subModeIndex == SUBMODE_DEBUG)
  {
    if (value == Ref::KeyDown){
      debugDisplayPosition++;
      if (debugDisplayPosition >= 4)
      {
        debugDisplayPosition = 0;
      }
      bRefreshDisplay = true;
    }
  } 
  else
  {
    if (value == Ref::KeyDown){
      if (state_restHoldRollResult == DICESTATE_RESULT)
      {
          state_restHoldRollResult = DICESTATE_REST;
      }
      bRefreshDisplay = true;
    }
  }
}

void ModeDice::input_ResetEvent()
{ 
}
