#include "Mode.h"
#include "Multiplex.h"
//extern struct Mode;

//Mode state
//1: (1) Person1 Orginal
//2: (2) Person2 Orginal
//3: (3) Person3 Orginal
//4: (4) Person4 Orginal

//5: (2P) två spelare

#ifndef MODELIFEH

#define MODELIFEH

struct ModeLife : Mode
{
    static const byte SUBMODE_INDIRECT;
    static const byte SUBMODE_DIRECT;
    static const byte SUBMODE_2PLAYERS;
    static const byte SUBMODE_4PLAYERS;

    int valueStart = 20;
    int valueCurrent[4] = {valueStart, valueStart, valueStart, valueStart}; 
    byte currentPlayer = 0;
    int valueChange = 0;
    unsigned long valueChangeTime = 0;
    
    ModeType type();
    byte subModeIndexCount();
    void ToggleEditState();
    void loop() override;
    void refeshDisplay() override;
    void refeshLevel() override;
    void initMode() override;
    void prepareStorage() override;
    void save() override;

    void input_wheel(int change) override;
    //void input_levelarrows(int arrowdir, bool value) override;
    void input_action(bool value) override;
    void input_setReset(bool value) override;
    void input_ResetEvent() override;
};
#endif