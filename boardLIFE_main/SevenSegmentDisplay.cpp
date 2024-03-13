#include "SevenSegmentDisplay.h"
 
// 7-seg layout
 
//    A
//  F   B
//    G
//  E   C
//    D   DP
  //static const int DotLed = 7; 

  static bool SevenSegmentDisplay::chararcterLayouts_charIx_ledOn[28][MULTIPLEX_LEDCOUNT] = {
    //Fro A to G (DP is in another array)
    { 0, 0, 0, 0, 0, 0, 0 }, //NONE
    { 1, 1, 1, 1, 1, 1, 0 }, //0
    { 0, 1, 1, 0, 0, 0, 0 }, //1
    { 1, 1, 0, 1, 1, 0, 1 }, //2
    { 1, 1, 1, 1, 0, 0, 1 }, //3
    { 0, 1, 1, 0, 0, 1, 1 }, //4
    { 1, 0, 1, 1, 0, 1, 1 }, //5
    { 1, 0, 1, 1, 1, 1, 1 }, //6
    { 1, 1, 1, 0, 0, 0, 0 }, //7
    { 1, 1, 1, 1, 1, 1, 1 }, //8
    { 1, 1, 1, 1, 0, 1, 1 }, //9  
    { 0, 0, 0, 0, 0, 0, 1 }, //- (11)  

    { 1, 1, 1, 0, 1, 1, 1 }, //A (12)
    { 0, 0, 1, 1, 1, 1, 1 }, //b (13) 
    { 1, 0, 0, 1, 1, 1, 0 }, //C (14)
    { 0, 1, 1, 1, 1, 0, 1 }, //d (15)
    { 1, 0, 0, 1, 1, 1, 1 }, //E (16)
    { 1, 0, 0, 0, 1, 1, 1 }, //F (17)
    { 0, 1, 1, 0, 1, 1, 1 }, //H (18)
    { 0, 0, 0, 1, 1, 1, 0 }, //L (19)
    { 0, 0, 1, 0, 1, 0, 1 }, //n (20)
    { 1, 1, 0, 0, 1, 1, 1 }, //P (21)
    { 0, 0, 0, 0, 1, 0, 1 }, //r 
    { 0, 0, 0, 1, 1, 1, 1 }, //t 
    { 0, 1, 1, 1, 1, 1, 0 }, //U
    { 0, 1, 1, 1, 1, 0, 0 }, //J
    { 0, 1, 1, 1, 0, 1, 1 }, //y 
    { 1, 1, 0, 1, 1, 0, 1 }, //Z 
  };

  SevenSegmentDisplay::SevenSegmentDisplay(byte commonCount){
    
    CommonCount = commonCount;
    DisplayString = new byte[commonCount];
    Dots = new bool[commonCount];
  }

  void SevenSegmentDisplay::setNumber(int number, byte emptyChar = 0){
    int digitIx = 0;

    bool minus = false;
    

    if (number == 0)
    {
        DisplayString[CommonCount - 1] = Ref::CHAR_NUM;
        digitIx = 1;
    }
    else
    {
      if (number < 0)
      {
        minus = true;
        number = -number;
      }

      while (number > 0 && digitIx < CommonCount) {
        int digit = number % 10;
        // do something with digit
        DisplayString[CommonCount - 1 - digitIx] = Ref::CHAR_NUM + digit;
        ++digitIx;

        number /= 10;
      }

      if (minus && digitIx < CommonCount)
      {
        DisplayString[CommonCount - 1 - digitIx] = Ref::CHAR_MINUS;
        ++digitIx;
      }
    }

    numberStringInvLength = CommonCount - digitIx;
    for (int i = 0; i < numberStringInvLength; ++i)
    {
      DisplayString[i] = emptyChar;
    }
  }

  void SevenSegmentDisplay::setAllDots(bool value)
  {
    if (!value)
    {Serial.println("clear dots");}
    
    for(int i = 0; i < CommonCount; ++i){
      Dots[i] = value;
    }
  }

  void SevenSegmentDisplay::setDotsFromLeft(int count)
  {    
    for(int i = 0; i < CommonCount; ++i){
      Dots[i] = i < count;
    }
  }

  void SevenSegmentDisplay::clearChars()
  {
    for(int i = 0; i < CommonCount; ++i){
      DisplayString[i] = 0;
    }
  }
  // void SevenSegmentDisplay::setChar(int char, int position, bool posFromLeft)
  // {
  //   if (!posFromLeft)
  //   {
  //     position = CommonCount - 1 - position;
  //   }

  //   if (position < CommonCount)
  //   {      
  //       DisplayString[position] = char;
  //   }
  // }
