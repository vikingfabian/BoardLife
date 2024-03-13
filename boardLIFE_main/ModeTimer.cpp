#include "ModeTimer.h"


static const byte ModeTimer::SUBMODE_DOWN = 0;
static const byte ModeTimer::SUBMODE_UP = 1;
static const byte ModeTimer::SUBMODE_REPEATEDOWN = 2; //DOWN räknare som alltid fortsätter
static const byte ModeTimer::SUBMODE_CHESS = 3; //pilar används till pause, wheel bytar spelare
//static const byte ModeTimer::SUBMODE_ADVANCEDCHESS = 4; //Ställ in antal spelare, bonus (extra tid somn fås), delay (tid innan nedräkning), confirm (pausar automatiskt vid spelar byte)

//TIMER
ModeType ModeTimer::type() { return ModeType::Timer; }
byte ModeTimer::subModeIndexCount() { return 4; } 

void ModeTimer::refeshDisplay() {  

  if (mayUpdateDotRow1())
  {
    Multiplex::Single.displayRed->setAllDots(false);
  }
  Multiplex::Single.displayGreen->setAllDots(false);

if (viewTimeDisplay)
{
  Multiplex::Single.displayRed->setNumber(valueMinutCurrent * 100 + valueSecondCurrent, Ref::CHAR_NUM);
}
else 
{
  Multiplex::Single.displayRed->clearChars();
}
 
 Serial.print("refeshDisplay");
 Serial.println(state_pausePlayAlertDone);
Serial.println(editState);

  if (editState)
  {    
    display_InputLevel(true);
    display_edit();
  }
  else
  {
    if (viewTimeDisplay)
    {
      Multiplex::Single.displayRed->Dots[1] = true;
    }
    
    switch (state_pausePlayAlertDone)
    {
      case 0:
        {
          Multiplex::Single.displayGreen->DisplayString[0] = 0;
          Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_P;
          Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_P;
        }
        break;

      case 1:
        {
          Multiplex::Single.displayGreen->clearChars();          
          int totalSec = valueMinutCurrent * 60 + valueSecondCurrent;
          int thirds = totalSec % 3;
          Multiplex::Single.displayGreen->DisplayString[2 - thirds] = Ref::CHAR_MINUS;
        }
        break;

      default:
        {
          Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_F;
          Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_NUM + 1;
          Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_n;
        }
        break;
    }
  }
}

void ModeTimer::initMode() {
  Mode::initMode();
  valueMinStart = EEPROM_readInt(Ref::MEM_ADRESS_TIMER_VALUEMINSTART);
  valueSecStart = EEPROM_readInt(Ref::MEM_ADRESS_TIMER_VALUESECSTART);
  if (valueMinStart + valueSecStart <= 0){
    valueSecStart = 15;
  }

  Serial.println("ModeTimer::initMode");
  Serial.println(valueMinStart);
  Serial.println(valueSecStart);

  valueMinutCurrent = valueMinStart;
  valueSecondCurrent = valueSecStart;
  editState = false;
  
  MaxLevel = 4;
  state_pausePlayAlertDone = 0;
  prevMillis = millis();
}

void ModeTimer::prepareStorage() {
  EEPROM_writeInt(Ref::MEM_ADRESS_TIMER_VALUEMINSTART, 00);
  EEPROM_writeInt(Ref::MEM_ADRESS_TIMER_VALUESECSTART, 15);
}

void ModeTimer::save(){
  EEPROM_writeInt(Ref::MEM_ADRESS_TIMER_VALUEMINSTART, valueMinStart);
  EEPROM_writeInt(Ref::MEM_ADRESS_TIMER_VALUESECSTART, valueSecStart);  
}

void ModeTimer::loop()
{
  Mode::loop();

  if (this->introState == 0)
  {
    unsigned long time = millis();

// Serial.println("state_pausePlayAlertDone");
// Serial.println(state_pausePlayAlertDone);

  if (state_pausePlayAlertDone == 1)
  {
    unsigned long timeDiff = time - prevMillis;

    valueMsCurrent += timeDiff;
    if (valueMsCurrent > 1000)
    {
      valueMsCurrent -= 1000;

      if (subModeIndex == SUBMODE_UP)
      {   //COUNTING UP
          ++valueSecondCurrent;
          if (valueSecondCurrent > 59)
          {            
            ++valueMinutCurrent;
            valueSecondCurrent = 0;            
          }
      }
      else //COUNTING DOWN
      {
          --valueSecondCurrent;
          if (valueSecondCurrent < 0){
            if (valueMinutCurrent > 0){
              --valueMinutCurrent;
              valueSecondCurrent = 60;
            }
          }

          if (valueSecondCurrent + valueMinutCurrent <= 0){
            state_pausePlayAlertDone = 2;
            Serial.println("beep beep");
            
            flashCount = 0;
          }
        }
      }

      bRefreshDisplay = true;
    }    
    else if (state_pausePlayAlertDone == 2)
    {
      if (millis() - flashTme > 500)
      {
        flashTme = millis();
        viewTimeDisplay = !viewTimeDisplay;
        if (++flashCount >= 6)
        {
          state_pausePlayAlertDone++;
        }
        bRefreshDisplay = true;
      }
    }
    else
    {
      viewTimeDisplay = true;
    }
    
    prevMillis = time;
  }
}

void  ModeTimer::input_wheel(int change){
 
  if (editState){
    if (inputLevel <= 2)
    {
      if (inputLevel == 1){
        valueSecondCurrent += change;
      }
      else{
        valueSecondCurrent += change * 10;
      }

      if (valueSecondCurrent < 0)
      {
        valueSecondCurrent += 60;
        if (valueMinutCurrent > 0){
          --valueMinutCurrent;
        }
      }
      else if (valueSecondCurrent > 60)
      {
        valueSecondCurrent -= 60;
        if (valueMinutCurrent < Ref::TimerMinMax){
          ++valueMinutCurrent;
        }
      }
    }
    else {
      if (inputLevel == 3){
        valueMinutCurrent += change;
      }
      else{
        valueMinutCurrent += change * 10;
      }

      if (valueMinutCurrent < 0)
      {
        valueMinutCurrent = Ref::TimerMinMax;
      }
      else if (valueMinutCurrent > Ref::TimerMinMax)
      {
        valueMinutCurrent = 0;
      }
    }
    
    bRefreshDisplay = true;
  }   
}

void  ModeTimer::input_action(bool value) {
  if (value == Ref::KeyDown)
  {
    if (editState){ //Leave edit state
      ToggleEditState();
    }
    else{ //Pause/play
      switch (state_pausePlayAlertDone)
      {
        case 0:
          state_pausePlayAlertDone = 1;
          break;
        case 1:
          state_pausePlayAlertDone = 0;
          break;
        default:
          //Restart a new timer
          state_pausePlayAlertDone = 0;
          valueSecondCurrent = valueSecStart;
          valueMinutCurrent = valueMinStart;
          break;          
      }

      bRefreshDisplay = true;
    }
  }
}

void ModeTimer::input_setReset(bool value) {
  if (value == Ref::KeyUp)
  {  
    if (subModeIndex == SUBMODE_UP)
    {
      valueMsCurrent = 0;
      valueSecondCurrent = 0;
      valueMinutCurrent = 0;
      bRefreshDisplay = true;
    }
    else
    {          
      ToggleEditState();      
    }
  }
}

void ModeTimer::input_ResetEvent()
{ 
}

void ModeTimer::ToggleEditState(){
  if (editState){
    //Save the new start value
    valueSecStart = valueSecondCurrent;
    valueMinStart = valueMinutCurrent;
    
    save();
    
    // Multiplex::Single.displayRed->setAllDots(false); 
    // Multiplex::Single.displayRed->Dots[1] = true;
  }
  else{
    state_pausePlayAlertDone = 0;
    viewTimeDisplay = true;
    valueSecondCurrent = valueSecStart;
    valueMinutCurrent = valueMinStart;
    flashDotRow(true);
  }

  editState = !editState;
  
  refeshLevel();
  bRefreshDisplay = true;
}

void ModeTimer::refeshLevel(){
  MaxLevel = editState? 4 : 3;
  Mode::refeshLevel();
}
