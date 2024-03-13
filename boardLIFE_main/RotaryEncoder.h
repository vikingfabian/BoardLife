#include <Arduino.h>
#include <Arduino_BuiltIn.h>
#include <stdint.h>

#ifndef RotaryEncoderH

#define RotaryEncoderH

class RotaryEncoder {
public:
  static const int ValueBetweenSnaps;
  int counter = 0; //Två/Fyra värden per snäpp
  int prevCounter = 0;
  uint8_t pinA; uint8_t pinB;
  unsigned long bounceProtectionA = 0, bounceProtectionB = 0;
  unsigned long lastInputTime = 0;

  RotaryEncoder(uint8_t pinA, uint8_t pinB);

  void onAChange();
  void onBChange();

  void onChange(int dir);

  int readSnaps();
};


#endif