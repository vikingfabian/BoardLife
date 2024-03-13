#include "Mode.h"
#include "Multiplex.h"

#ifndef MODEDICEH

#define MODEDICEH
struct ModeDice : Mode
{
    static const byte SUBMODE_ONE;
    static const byte SUBMODE_TWO;
    static const byte SUBMODE_BUZZERS; //Frågesport, varje knapp är en spelare 
    // static const byte SUBMODE_SETTINGS_BATTERY;
    static const byte SUBMODE_DEBUG;

    static unsigned long stateTime;
    unsigned long rollUpdateTime = 0;
    byte state_restHoldRollResult = 0;
    int diceMax = 6;
    int result = 0;
    byte buzzPlayer = 0;
    //byte buzzState = 0; //0: waiting for input, 1: flash player, 2: soft lock
    unsigned long buzzSoftLockTime = 0;
    int softUnlockInput = 0;
    int buzzTotalSec = 0;
    int wheelTotalChange = 0;

    byte debugMode_red_green_both = 2; //action
    byte debugDisplayNumber = 0; //wheel
    byte debugDisplayPosition = 0; //set
    byte debugDisplayCharCount = 1; //arrows
    

    ModeType type();
    byte subModeIndexCount();
    void initMode() override;
    void loop() override;
    void roll();
    void display_Debug(bool display1);
    void refeshDisplay() override;
    void input_wheel(int change) override;
    void changeMax(int change);
    void buzzKeydown(byte player);
    void input_levelarrows(int arrowdir, bool value) override;
    void input_action(bool value) override;
    void input_setReset(bool value) override;
    void input_ResetEvent() override;

    void prepareStorage() override;
    void save() override;
};
#endif