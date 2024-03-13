#include "BoardLifeMain.h"


  BoardLifeMain::BoardLifeMain(){

    this->modeInput1 = new SelectMode(0);
    this->modeInput2 = new SelectMode(1);
    this->modeInput3 = new SelectMode(2);
  }

  void BoardLifeMain::storageSetup(bool factoryReset){
    //Setup save/load
    const byte CheckVal = 122;

    if (!factoryReset && 
      EEPROM.read(Ref::MEM_ADRESS_CHECK1) == CheckVal && 
      EEPROM.read(Ref::MEM_ADRESS_CHECK2) == CheckVal){
      //Previous save is available
      ModeType modetype = EEPROM.read(Ref::MEM_ADRESS_MODE_TYPE);
      byte modeindex = EEPROM.read(Ref::MEM_ADRESS_MODE_INDEX);  //Används inte
      byte buttonindex = EEPROM.read(Ref::MEM_ADRESS_MODE_BUTTON);
      modes->setMode(modetype, buttonindex);
    }
    else{
      //New memory setup
      EEPROM.write(Ref::MEM_ADRESS_CHECK1, CheckVal);
      EEPROM.write(Ref::MEM_ADRESS_CHECK2, CheckVal); 

      modes->prepareStorage();
    }
  }
  
  void BoardLifeMain::init(){

    modes = new ModeManager();
    storageSetup(false);  
  }

  void BoardLifeMain::loop() {

    Input::Single.loop();
    modes->activeMode->loop();

    if (Mode::bRefreshDisplay)
    {
      Serial.println("refresh");
      modes->activeMode->refeshDisplay();
      Mode::bRefreshDisplay = false;
    }
         
    if (Input::Single.modeLifeEvent){
      this->modeInput1->selectModeEvent(Input::Single.modeLifeVal, this->modes);
    }
    if (Input::Single.modeTimeEvent){
      Serial.println("modeTimeEvent");
      this->modeInput2->selectModeEvent(Input::Single.modeTimeVal, this->modes);
    }
    if (Input::Single.modeDiceEvent){
      this->modeInput3->selectModeEvent(Input::Single.modeDiceVal, this->modes);
    }
   
  }  

  BoardLifeMain BoardLifeMain::Single = BoardLifeMain();



