#include "ModeManager.h"

//MANAGER
ModeManager::ModeManager() {
    this->modeLifeIns = new ModeLife();
    this->modeTimerIns = new ModeTimer();
    this->modeDiceIns = new ModeDice();

    Serial.println("mode manager");
    setMode(ModeType::Life, 255);
}

void ModeManager::prepareStorage(){
    this->modeLifeIns->prepareStorage();
    this->modeTimerIns->prepareStorage();
    this->modeDiceIns->prepareStorage();
}

void ModeManager::setMode(ModeType type, byte buttonIndex) {    
    
    switch (type)
    {
        case ModeType::Life:
            this->activeMode = modeLifeIns;
            Multiplex::Single.displayRed->DisplayString[0] = Ref::CHAR_L;
            Multiplex::Single.displayRed->DisplayString[1] = Ref::CHAR_NUM + 1;
            Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_F;
            Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_E;

            switch (this->activeMode->subModeIndex)
            {
                case 0:
                    Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_NUM + 1;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_n;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_d;
                    break;
                case 1:
                    Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_d;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_NUM + 1;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_r;
                    break;
                case 2:
                    Multiplex::Single.displayGreen->DisplayString[0] = 0;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_NUM + 2;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_P;
                    break;
                case 3:
                    Multiplex::Single.displayGreen->DisplayString[0] = 0;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_NUM + 4;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_P;
                    break;
            }
            //Multiplex::Single.displayGreen->setNumber(this->activeMode->modeIndex + 1);
            break;
        case ModeType::Timer:
            this->activeMode = modeTimerIns;
            Multiplex::Single.displayRed->DisplayString[0] = Ref::CHAR_t;
            Multiplex::Single.displayRed->DisplayString[1] = Ref::CHAR_NUM + 1;
            Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_n;
            Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_r;

// static const byte ModeTimer::SUBMODE_DOWN = 0;
// static const byte ModeTimer::SUBMODE_UP = 1;
// static const byte ModeTimer::SUBMODE_REPEATEDOWN = 2; //DOWN räknare som alltid fortsätter
// static const byte ModeTimer::SUBMODE_CHESS = 3; //pilar används till pause, wheel bytar spelare

            //Multiplex::Single.displayGreen->setNumber(this->activeMode->subModeIndex + 1);
            switch (this->activeMode->subModeIndex)
            {
                case 0:
                    Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_d;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_NUM;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_n;
                    break;
                case 1:
                    Multiplex::Single.displayGreen->DisplayString[0] = 0;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_U;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_P;
                    break;
                case 2:
                    Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_r;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_E;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_P;
                    break;
                case 3:
                    Multiplex::Single.displayGreen->DisplayString[0] = Ref::CHAR_C;
                    Multiplex::Single.displayGreen->DisplayString[1] = Ref::CHAR_H;
                    Multiplex::Single.displayGreen->DisplayString[2] = Ref::CHAR_NUM + 5;
                    break;
            }

            break;
        case ModeType::Dice:
            this->activeMode = modeDiceIns;
            
            if ( this->activeMode->subModeIndex == 3)
            {
                //DEBUG
                Multiplex::Single.displayRed->DisplayString[0] = Ref::CHAR_d;
                Multiplex::Single.displayRed->DisplayString[1] = Ref::CHAR_b;
                Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_U;
                Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_NUM + 9;
            }
            else if ( this->activeMode->subModeIndex == 2)
            {
                //BUZZ
                Multiplex::Single.displayRed->DisplayString[0] = Ref::CHAR_b;
                Multiplex::Single.displayRed->DisplayString[1] = Ref::CHAR_U;
                Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_Z;
                Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_Z;
            }
            else
            {
                Multiplex::Single.displayRed->DisplayString[0] = Ref::CHAR_d;
                Multiplex::Single.displayRed->DisplayString[1] = Ref::CHAR_NUM + 1;
                Multiplex::Single.displayRed->DisplayString[2] = Ref::CHAR_C;
                Multiplex::Single.displayRed->DisplayString[3] = Ref::CHAR_E;
            }

            Multiplex::Single.displayGreen->setNumber(this->activeMode->subModeIndex + 1);
            break;
    }

    //Set dots after modeIndex    
    Multiplex::Single.displayRed->setDotsFromLeft(this->activeMode->subModeIndex + 1);
    Multiplex::Single.displayGreen->setAllDots(false);
        
    Mode::buttonIndex = buttonIndex;
    Mode::introState = millis() + 1200;
    
    EEPROM.write(Ref::MEM_ADRESS_MODE_TYPE, type);
   // EEPROM.write(Ref::MEM_ADRESS_MODE_INDEX, this->activeMode->modeIndex);//
    EEPROM.write(Ref::MEM_ADRESS_MODE_BUTTON, buttonIndex);
}

