#include "ModeLife.h"

static const byte ModeLife::SUBMODE_INDIRECT = 0;
static const byte ModeLife::SUBMODE_DIRECT = 1;
static const byte ModeLife::SUBMODE_2PLAYERS = 2;
static const byte ModeLife::SUBMODE_4PLAYERS = 3;


//LIFE
ModeType ModeLife::type() { return ModeType::Life; }
byte ModeLife::subModeIndexCount() { return 4; } 

void ModeLife::refeshDisplay() {
  if (viewResetDisplay() == false)
  {
    if (mayUpdateDotRow1())
    { 
      Multiplex::Single.displayRed->setAllDots(false);
    }

    if (mayUpdateDotRow2())
    { 
      Multiplex::Single.displayGreen->setAllDots(false);
    }  

    if (editState)
    {
      Multiplex::Single.displayRed->setNumber(valueStart);
      display_edit();
      display_InputLevel(true);
      Multiplex::Single.displayGreen->clearChars();
    }
    else
    {

      if (subModeIndex == SUBMODE_4PLAYERS)
      {
        Multiplex::Single.displayRed->setNumber(valueCurrent[currentPlayer]);
      }
      else
      {
        Multiplex::Single.displayRed->setNumber(valueCurrent[0]);
        display_InputLevel(false);
      }
           
      if (subModeIndex == SUBMODE_2PLAYERS)
      {
        Multiplex::Single.displayGreen->setNumber(valueCurrent[1]);  
      }
      else 
      {
        
        if (subModeIndex == SUBMODE_4PLAYERS && valueChange == 0)
        {
          Multiplex::Single.displayGreen->DisplayString[0] = 0;
          Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_P;
          Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_NUM + 1 + currentPlayer;
        }
        else 
        {
          Multiplex::Single.displayGreen->setNumber(valueChange);
        }
      }
      
    }
  }
}

void ModeLife::initMode() {
  Mode::initMode();

  valueStart = EEPROM_readInt(Ref::MEM_ADRESS_LIFE_VALUESTART);
  
  input_ResetEvent();
  MaxLevel = 4;

}

void ModeLife::prepareStorage() {
  EEPROM_writeInt(Ref::MEM_ADRESS_LIFE_VALUESTART, 20);
}

void ModeLife::save(){
  EEPROM_writeInt(Ref::MEM_ADRESS_LIFE_VALUESTART, valueStart);
}

void ModeLife::loop(){
  Mode::loop();

  if ((subModeIndex == SUBMODE_DIRECT || subModeIndex == SUBMODE_4PLAYERS) && 
      valueChange != 0 &&
      millis() > valueChangeTime + 2000)
  {
    valueChange = 0;
    bRefreshDisplay = true;
  }   
}

void  ModeLife::input_wheel(int change){
  int level = TenLevel[inputLevel];
  change *= level;
  
  if (editState){
    valueStart += change;
  }
  else{
    valueChangeTime = millis();

    if (subModeIndex != SUBMODE_2PLAYERS)
    { valueChange += change; }
        
    if (subModeIndex != SUBMODE_INDIRECT)
    {
      valueCurrent[currentPlayer] += change;
    }    
  }

  bRefreshDisplay = true;
}

void  ModeLife::input_action(bool value) {
  if (value == Ref::KeyDown)
  {
    if (editState){ //Leave edit state
      ToggleEditState();
    }
    else{ //Commit change
      if (subModeIndex == SUBMODE_INDIRECT)
      {
        valueCurrent[0] += valueChange;
      }
      else if (subModeIndex >= SUBMODE_2PLAYERS)
      {
        int max = 2;
        if (subModeIndex >= SUBMODE_4PLAYERS)
        {
          max = 4;
        }

        if (valueChange != 0)
        {
          valueChange = 0;
        }
        else
        {
          currentPlayer++;
          if (currentPlayer >= max)
          {
            currentPlayer = 0;
          }

          if (subModeIndex == SUBMODE_2PLAYERS)
          {
            flashDotRow(currentPlayer == 0);
          }
        }
      }
      valueChange = 0;
    }

    bRefreshDisplay = true;
  }
}

void ModeLife::input_setReset(bool value) {
  if (value == Ref::KeyDown)
  {
      if (!editState)
      {
        resetKeyDownTime = millis();
      }
  }
  else if (ignoreSetKeyUp)
  {
    ignoreSetKeyUp = false;
  }
  else
  {
    if (!editState && valueChange != 0)
    {
      valueChange = 0; //backa värde input, ska jag flasha punkter
    }
    else
    {
      ToggleEditState();
    }    
  }  
}

void ModeLife::input_ResetEvent()
{
  for (int i = 0; i < 4; ++i)
  {
    valueCurrent[i] = valueStart;
  }
  valueChange = 0;
}

void ModeLife::ToggleEditState(){
  if (editState){
    //Save the new start value
    valueChange = 0;
    save();
  }

  editState = !editState;
  flashDotRow(editState);
  refeshLevel();
  inputLevel = 1;

  
  bRefreshDisplay = true;
}

void ModeLife::refeshLevel(){
  MaxLevel = editState? 4 : 3;
  
  Mode::refeshLevel();
}