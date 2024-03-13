#include "Mode.h"

bool Mode::editState = true;

Mode::Mode(){
  subModeIndex = 0;
}

int Mode::TenLevel[] = { 0, 1, 10, 100, 1000 };
long Mode::introState = 0;
byte Mode::buttonIndex = 0;
long Mode::flashDots = 0;
static const unsigned long Mode::ResetHoldTime = 800;
byte Mode::flashDotsDisplay = 255;
byte Mode::MaxLevel = 255;
bool Mode::bRefreshDisplay = false;

void Mode::loop(){
  
  if (this->introState > 0){
    // Serial.print("red");
    // Serial.print(Multiplex::Single.displayRed->Dots[0]);
    // Serial.print(Multiplex::Single.displayRed->Dots[1]);
    // Serial.print(Multiplex::Single.displayRed->Dots[2]);
    // Serial.println(Multiplex::Single.displayRed->Dots[3]);

    if (millis() > this->introState){
      exitIntro();
    }
  }
  else{
    
    if (Input::Single.setResetEvent)
    {
        input_setReset(Input::Single.setResetVal);
    }

    if (Input::Single.actionEvent)
    {
        input_action(Input::Single.actionVal);
    }
    
    if (Input::Single.arrowUpEvent)
    {
        input_levelarrows(1, Input::Single.arrowUpVal);
    }
    if (Input::Single.arrowDownEvent)
    {
        input_levelarrows(-1, Input::Single.arrowDown);
    }

    int rotationInput = Input::Single.rotInput->readSnaps();

    if (rotationInput != 0)
    {
      input_wheel(rotationInput);
    }

    if (flashDots != 0 && millis() > flashDots){
      flashDots = 0;
      bRefreshDisplay = true;
    }

    if (resetKeyDownTime > 0)
    {
      if (Input::Single.setResetVal)
      {
        unsigned long holdTime = millis() - resetKeyDownTime;
        if (holdTime >= ResetHoldTime)
        {
          resetKeyDownTime = 0;
          ignoreSetKeyUp = true;
          input_ResetEvent();
        }
      }
      else
      {
        resetKeyDownTime = 0;
      }
      bRefreshDisplay = true;
    }
  }
}

void Mode::refeshLevel(){
  Serial.print("refeshLevel ");
  Serial.println(inputLevel);
  if (inputLevel < 1)
  { 
    inputLevel = Mode::MaxLevel;
  }
  else if (inputLevel > Mode::MaxLevel)
  { 
    inputLevel =  1;
  }
  Serial.print("to ");
  Serial.println(inputLevel);
}

void Mode::flashDotRow(bool Display1){
  if (Display1){
      flashDotsDisplay = 1;
      Multiplex::Single.displayRed->setAllDots(true);
    }
    else{
      flashDotsDisplay = 2;
      Multiplex::Single.displayGreen->setAllDots(true);
    }
  flashDots = millis() + 200;
}

bool Mode::mayUpdateDotRow1()
{
  return flashDots == 0 || flashDotsDisplay != 1;
}
bool Mode::mayUpdateDotRow2()
{
  return flashDots == 0 || flashDotsDisplay != 2;
}

void Mode::exitIntro(){
  Serial.println("exitIntro");
  this->introState = 0;
  editState = false;
  if (!firstTimeInit)
  {
    initMode();
    firstTimeInit = true;
  }
  refeshLevel();
  bRefreshDisplay = true;
}

void Mode::onExit(){

}

void Mode::EEPROM_writeInt(byte address, int value){
  EEPROM.write(address, value >> 8);
  EEPROM.write(address+1, value & 0xFF);
}

int Mode::EEPROM_readInt(byte address){
  byte val1 = EEPROM.read(address);
  byte val2 = EEPROM.read(address+1);
  
  return (val1 << 8) + val2;
}

void Mode::initMode(){
  flashDots = 0;
}

void Mode::nextSubMode()
{
  Serial.print("nextSubMode ");
  Serial.println(this->subModeIndex);
  Serial.print("count ");
  Serial.println(this->subModeIndexCount());
  ++this->subModeIndex;
  if (this->subModeIndex >= this->subModeIndexCount())
  { this->subModeIndex = 0; }
}

void Mode::prepareStorage(){}
void Mode::save(){}
void  Mode::input_levelarrows(int arrowdir, bool value) {
  if (value == Ref::KeyDown){
    inputLevel += arrowdir;

    refeshLevel();
    bRefreshDisplay = true;

    //refeshDisplay();
  }
}
void Mode::display_edit()
{
  Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_E;
  Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_d;
  Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_NUM + 1;
}

void Mode::display_InputLevel(bool display1)
{
  SevenSegmentDisplay *display = display1? Multiplex::Single.displayRed : Multiplex::Single.displayGreen;
  display->Dots[display->CommonCount - inputLevel] = true;
}
//void Mode::onExit(){}


bool  Mode::isResetKeyUp()
{
  bool reset = (millis() - resetKeyDownTime) > ResetHoldTime;
  resetKeyDownTime = 0;
  return reset;
}

bool Mode::viewResetDisplay()
{
  bool view = resetKeyDownTime != 0 && ((millis() - resetKeyDownTime) > 200);
  if (view)
  {
    Multiplex::Single.displayRed->setAllDots(false);
    Multiplex::Single.displayGreen->setAllDots(false);

    unsigned long holdTime = millis() - resetKeyDownTime;
    long viewLoading = map(holdTime, 0, ResetHoldTime, 0, 4);
    // if (viewLoading > 3) 
    // { viewLoading = 3; }

    for (long i = 0; i < viewLoading; i++)
    {
      Multiplex::Single.displayRed->Dots[i] = true;
    }

    Multiplex::Single.displayRed->DisplayString[0] = 0;
    Multiplex::Single.displayRed->DisplayString[1] = 0;
    Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_r;
    Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_E;

    Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_NUM + 5;
    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_E;
    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_t;

    //Multiplex::Single.displayRed->Dots[Multiplex::Single.displayRed->CommonCount - inputLevel] = true;
  }

  return view;
}
