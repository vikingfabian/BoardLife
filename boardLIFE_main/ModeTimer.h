#include <Arduino.h>
#include <Arduino_BuiltIn.h>
#include <stdint.h>
#include "Mode.h"
#include "Multiplex.h"

#ifndef MODETIMERH

#define MODETIMERH

struct ModeTimer : Mode
{
    static const byte SUBMODE_DOWN;
    static const byte SUBMODE_UP;
    static const byte SUBMODE_REPEATEDOWN;
    static const byte SUBMODE_CHESS; //pilar används till pause, wheel bytar spelare

    byte state_pausePlayAlertDone;
    
    unsigned long prevMillis;
    int valueSecStart;
    int valueMinStart;
    int valueSecondCurrent;
    int valueMinutCurrent;
    unsigned long valueMsCurrent;
    
    unsigned long flashTme;
    bool viewTimeDisplay = true;
    byte flashCount = 0;

    ModeType type();
    byte subModeIndexCount();
    void loop() override;
    void refeshDisplay() override;
    void refeshLevel() override;
    void initMode() override;
    void prepareStorage() override;
    void save() override;
    void ToggleEditState();
    void input_wheel(int change) override;
    void input_action(bool value) override;
    void input_setReset(bool value) override;
    void input_ResetEvent() override;
};

#endif