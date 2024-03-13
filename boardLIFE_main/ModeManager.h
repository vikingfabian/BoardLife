#include "Mode.h"
#include "ModeLife.h"
#include "ModeTimer.h"
#include "ModeDice.h"

#ifndef MODEMANAGERH

#define MODEMANAGERH

class ModeManager{
  public:
      Mode *activeMode;
      ModeLife *modeLifeIns;
      ModeTimer *modeTimerIns;
      ModeDice *modeDiceIns;

      ModeManager();
      void prepareStorage();
      void setMode(ModeType type, byte buttonIndex);
};

#endif