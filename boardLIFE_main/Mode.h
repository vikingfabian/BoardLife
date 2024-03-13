#include <Arduino_BuiltIn.h>
#include <Arduino.h>
#include <stdint.h>
#include <EEPROM.h>
#include "Ref.h"
#include "RotaryEncoder.h"
#include "Input.h"
#include "Display.h"
#include "Multiplex.h"



#ifndef MODEH

#define MODEH

struct Mode{
    static int TenLevel[];
    static long introState;
    static bool editState;
    static byte buttonIndex;
    static bool bRefreshDisplay;

    static long flashDots;
    static byte flashDotsDisplay;
    static const unsigned long ResetHoldTime;
    unsigned long resetKeyDownTime = 0;
    bool ignoreSetKeyUp = false;
    //ModeType type;
    byte subModeIndex; //To run multiple versions of the same mode
    byte inputLevel =1;
    bool firstTimeInit = false;
    static byte MaxLevel;
    Mode();
    
    virtual void loop();
    void flashDotRow(bool Display1);
    bool mayUpdateDotRow1();
    bool mayUpdateDotRow2();
    
    void exitIntro();
    virtual void refeshDisplay();
    virtual void refeshLevel();
    void display_edit();
    void display_InputLevel(bool display1);
    void onExit();
    void EEPROM_writeInt(byte address, int value);  
    int EEPROM_readInt(byte address);  
    virtual ModeType type();
    virtual byte subModeIndexCount();

    virtual void initMode();
    virtual void nextSubMode();
    virtual void prepareStorage();
    virtual void save();

    //alla inputs
    virtual void input_wheel(int change);
    virtual void input_levelarrows(int arrowdir, bool value);
    virtual void input_action(bool value);
    virtual void input_setReset(bool value);
    virtual void input_ResetEvent();

    bool isResetKeyUp();
    bool viewResetDisplay();
};






#endif
