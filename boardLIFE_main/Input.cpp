#include "Input.h"

Input Input::Single = Input();

void Input::init(RotaryEncoder *rotInput){
    this->rotInput = rotInput;
    pinMode(ActionPin, INPUT);
    pinMode(SetResetPin, INPUT);
    pinMode(ArrowUpPin, INPUT);
    pinMode(ArrowDownPin, INPUT);
    pinMode(ModeLifePin, INPUT);
    pinMode(ModeTimePin, INPUT);
    pinMode(ModeDicePin, INPUT);
}

void Input::loop() {

    bool pastBounceDelay = (millis() - lastDebounceTime) > 20;

    bool readActionPin = digitalRead(ActionPin);
    actionEvent = (readActionPin != actionVal) && pastBounceDelay;
    actionVal = readActionPin;

    bool readSetResetPin = digitalRead(SetResetPin);
    setResetEvent = (readSetResetPin != setResetVal) && pastBounceDelay;
    setResetVal = readSetResetPin;

    bool readArrowUpPin = digitalRead(ArrowUpPin);
    arrowUpEvent = (readArrowUpPin != arrowUpVal) && pastBounceDelay;
    arrowUpVal = readArrowUpPin;

    bool readArrowDownPin = digitalRead(ArrowDownPin);
    arrowDownEvent = (readArrowDownPin != arrowDown) && pastBounceDelay;
    arrowDown = readArrowDownPin;

    bool readModeLifePin = digitalRead(ModeLifePin);
    modeLifeEvent = (readModeLifePin != modeLifeVal) && pastBounceDelay;
    modeLifeVal = readModeLifePin;

    bool readModeTimePin = digitalRead(ModeTimePin);
    modeTimeEvent = (readModeTimePin != modeTimeVal) && pastBounceDelay;
    modeTimeVal = readModeTimePin;

    bool readModeDicePin = digitalRead(ModeDicePin);
    modeDiceEvent = (readModeDicePin != modeDiceVal) && pastBounceDelay;
    modeDiceVal = readModeDicePin;

    if (actionEvent ||
        setResetEvent ||
        arrowUpEvent ||
        arrowDownEvent ||
        modeLifeEvent ||
        modeTimeEvent ||
        modeDiceEvent)
    {
        lastDebounceTime = millis(); // reset the debounce timer
    }
}

void Input::printTestInput(){
    Serial.print(actionVal); 
    Serial.print(", ");
    Serial.print(setResetVal); 
    Serial.print(", ");
    Serial.print(arrowUpVal); 
    Serial.print(", ");
    Serial.print(arrowDown); 
    Serial.print(", ");
    Serial.print(modeLifeVal); 
    Serial.print(", ");
    Serial.print(modeTimeVal); 
    Serial.print(", ");
    Serial.println(modeDiceVal); 
}