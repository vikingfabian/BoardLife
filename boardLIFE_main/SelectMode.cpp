#include "SelectMode.h"


  SelectMode::SelectMode(byte modeButtonIndex){
    this->modeButtonIndex = modeButtonIndex;
    switch(this->modeButtonIndex){
          case 0:
              this->modetype = ModeType::Life;
              break;
          case 1:
              this->modetype = ModeType::Timer;
              break;
          case 2:
              this->modetype = ModeType::Dice;
              break;
        }        
  }

  void SelectMode::loop(){

  }

  void SelectMode::selectModeEvent(bool value, ModeManager *modes){
     //Button already has bounce protection
        
      if (value == Ref::KeyDown) 
      {        
        if (modes->activeMode->type() == this->modetype)
        {
          Serial.println("toggle Mode under type");
          modes->activeMode->nextSubMode();
          // ++this->modeIndex;
          // if (this->modeIndex >= this->modeIndexCount)
          // { this->modeIndex = 0; }
        }
          
        Serial.println("setMode");
        modes->setMode(this->modetype, this->modeButtonIndex);
        
      }
  }

  
