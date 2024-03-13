
#include "RotaryEncoder.h"
#include "Input.h"
// #include "Ref.h"
// #include "ModeManager.h"
// #include "SelectMode.h"
#include "BoardLifeMain.h"
#include "Display.h"
#include "Multiplex.h"

//#define TECH_SETUP true

//Multiplex *multiplex;
RotaryEncoder *rotInput;
volatile int stateA3 = LOW;

void setup() {
  Serial.begin(9600);
  Serial.println("INIT");
  
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  
  //DIGITAL PINS USABLE FOR UNO INTERRUPTS 2, 3
  setupRotaryEncoder(3, 2);  

  setupTimerInterrupt();
  Input::Single.init(rotInput);
  BoardLifeMain::Single.init();  
}

void loop() {
 

#if TECH_SETUP
  delay(1000); 
  Multiplex::Single.multiplexNext_Test();
#else
   BoardLifeMain::Single.loop();
#endif
}

void setupRotaryEncoder(uint8_t pinA, uint8_t pinB){
  rotInput = new RotaryEncoder(pinA, pinB);

  pinMode(pinA,INPUT);
  pinMode(pinB,INPUT);
  attachInterrupt(digitalPinToInterrupt(pinA), onRotaryEncoderAChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), onRotaryEncoderBChange, CHANGE);
}


void onRotaryEncoderAChange(){
  rotInput->onAChange();
}

void onRotaryEncoderBChange(){
  rotInput->onBChange();
}

void setupTimerInterrupt() {

  // multiplex = new Multiplex();
  // Multiplex::Single = multiplex;
  // Display::multiplex = multiplex;
  

  Multiplex::Single.setupDisplays();
/* Example code with timer intyerrutp that will create an interruption each
 *  500ms using timer1 and prescalar of 256.
  Calculations (for 500ms):
  System clock 16 Mhz and Prescalar 256;
  Timer 1 speed = 16Mhz/256 = 62.5 Khz    
  Pulse time = 1/62.5 Khz =  16us  
  Count up to = 500ms / 16us = 31250 (so this is the value the OCR register should have)*/  
 
  cli();                      //stop interrupts for till we make the settings
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
 
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescalar 256  
 
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A
 
  /*4. Set the value of register A to 31250*/
  OCR1A = 60; //Tid i updates              //Finally we set compare register A to this value  
  sei();                     //Enable back the interrupts
}
 
//With the settings above, this IRS will trigger each 500ms.
ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
 
  //Run interrupt code
  Multiplex::Single.multiplexNext();    
}