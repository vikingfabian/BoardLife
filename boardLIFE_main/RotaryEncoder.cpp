#include "RotaryEncoder.h"

#define HALFBYTE 127
#define ENCODER_BOUNCE_TIME 20

RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB) {
    this->pinA = pinA;
    this->pinB = pinB;
}

void RotaryEncoder::onAChange(){
  if(millis()> bounceProtectionA)
  {
    lastInputTime = millis();
    bounceProtectionA = lastInputTime + ENCODER_BOUNCE_TIME;
    onChange(1);
  }
}
void RotaryEncoder::onBChange(){
  if(millis()> bounceProtectionB)
  {
    lastInputTime = millis();
    bounceProtectionB = lastInputTime + ENCODER_BOUNCE_TIME;
    onChange(-1);
  }
}

void RotaryEncoder::onChange(int dir){
  if (digitalRead(pinA) == digitalRead(pinB)) 
  { 
    counter -= dir;
  } 
  else 
  {
    counter += dir;
  }
  Serial.print("counter");
  Serial.println(counter);
}

const int RotaryEncoder::ValueBetweenSnaps = 4;

int RotaryEncoder::readSnaps()
{
  int diff = counter - prevCounter;
  int aDiff = abs(diff);

  if (aDiff >= ValueBetweenSnaps) 
  {
    int snaps = diff / ValueBetweenSnaps;
    prevCounter += diff;
 
    return snaps;
  }
  else if (aDiff >= ValueBetweenSnaps / 2 && (millis() - lastInputTime) > 50)
  {
    prevCounter += diff;
    int snaps = diff < 0 ? -1 : 1;
    return snaps;
  }
  
  
  return 0;
}