#include "Multiplex.h"


  static byte Multiplex::DisplayLedToRegister[MULTIPLEX_LEDCOUNT]//ej punkt
  {
      2,0,13,14,7,1,10//10, 13, 5,6,15,9,1//5, 6, 4, 3, 1, 0, 7 
  };

  static const byte Multiplex::DisplayDotToRegister = 15; //punkt

  static byte Multiplex::DisplayCommonToRegister[7] //röd, sen grön
  {
      8,3,4,6, 12,11,9//0,11,12,14,  4,3,2//12, 15, 14, 13, 9, 11, 10
  };

  Multiplex Multiplex::Single = Multiplex();

void Multiplex::setupDisplays()
{
    shiftReg = new ShiftRegister( A4, A5, A3, 2);//byte latchPin, byte clockPin, byte dataPin, byte serieLength
    //MSBFIRST - most significant first
  //Placera alla ettor
    


    displayRed = new SevenSegmentDisplay(4);
    displayGreen = new SevenSegmentDisplay(3);
    totalCommonCount = displayRed->CommonCount + displayGreen->CommonCount;
    // displayRed->setNumber(1234);
    // displayGreen->setNumber(567);    
    displayRed->DisplayString[0] = 6;
    displayRed->DisplayString[1] = 13;
    displayRed->DisplayString[2] = 11;
    displayRed->DisplayString[3] = 11;
    
    displayGreen->DisplayString[0] = 12;
    displayGreen->DisplayString[1] = 6;
    displayGreen->DisplayString[2] = 6;
}

void Multiplex::setupDisplayTestMode()
{
shiftReg->dataOut = 0;//4294967295;//255 << 8 + 255;
  
//    A
//  F   B
//    G
//  E   C
//    D   DP
  //shiftReg->setBit(0, true);//f
  //shiftReg->setBit(1, true); //e
  //shiftReg->setBit(2, true); //dp
  //shiftReg->setBit(3, true); //d
  //shiftReg->setBit(4, true); //c
  // shiftReg->setBit(5, true); //a
  // shiftReg->setBit(6, true); //b
  //shiftReg->setBit(7, true); //g
  //shiftReg->setBit(8, true); //-
  // shiftReg->setBit(9, true); //gr1
  // shiftReg->setBit(10, true); //gr3
  // shiftReg->setBit(11, true); //gr2
  //shiftReg->setBit(12, true); //rö1
  // shiftReg->setBit(13, true); //rö4
  // shiftReg->setBit(14, true); //rö3
  // shiftReg->setBit(15, true); //rö2
  //shiftReg->SendData(); // turn on the  next dataOut
  //    delay(500);
  // for (int i = 0; i < 16; i++)   // loop that will turn the dataOut one by one
  // {
  //   shiftReg->setBit(i, true); // assigning the respective dataOut the HIGH values
  //        shiftReg->SendData();// turn off the previous dataOut
  //    delay(2000);
  //  }

}

void Multiplex::multiplexNext()
 {
    //  Serial.println("multiplexNext");
    //digitalWrite(commonIxToPin[currentCommon], HIGH); //Turn off previuos

    ++currentCommon;
    if (currentCommon >= totalCommonCount){
        currentCommon = 0;
    }

    int characterIx;
    bool dot;

    if (currentCommon < displayRed->CommonCount)
    {
        characterIx = displayRed->DisplayString[currentCommon];
        dot = displayRed->Dots[currentCommon];        
    }
    else
    {
        int ix = currentCommon - displayRed->CommonCount;
        characterIx = displayGreen->DisplayString[ix];
        dot = displayGreen->Dots[ix];
    }
    
    
    shiftReg->dataOut = 0;
    //Prepare Register
    for (int i = 0; i < MULTIPLEX_LEDCOUNT; ++i)
    {
        if (SevenSegmentDisplay::chararcterLayouts_charIx_ledOn[characterIx][i])
        {
          //temp
          shiftReg->setBit(Multiplex::DisplayLedToRegister[i], true);
        }
    }

    if (dot)
    {
      shiftReg->setBit(Multiplex::DisplayDotToRegister, true);
    }
 
    //Set common
    shiftReg->setBit(Multiplex::DisplayCommonToRegister[currentCommon], true); //Pull LOW
    

    // Serial.print("shiftReg");
    // Serial.println(shiftReg->dataOut);

    //Send Register
    shiftReg->SendData();
}

void Multiplex::multiplexNext_Test()
 {
    //  Serial.println("multiplexNext");
    //digitalWrite(commonIxToPin[currentCommon], HIGH); //Turn off previuos

    //++currentCommon;
    //if (currentCommon >= totalCommonCount){
        currentCommon = 0;
    //}

    int characterIx;
    bool dot;

    if (currentCommon < displayRed->CommonCount)
    {
        characterIx = displayRed->DisplayString[currentCommon];
        dot = displayRed->Dots[currentCommon];        
    }
    else
    {
        int ix = currentCommon - displayRed->CommonCount;
        characterIx = displayGreen->DisplayString[ix];
        dot = displayGreen->Dots[ix];
    }
    
    
    shiftReg->dataOut =0b11111111111111111111111111111111;//0;
    // //Prepare Register
    // for (int i = 0; i < MULTIPLEX_LEDCOUNT; ++i)
    // {
    //     if (SevenSegmentDisplay::chararcterLayouts_charIx_ledOn[characterIx][i])
    //     {
    //       //temp
    //       shiftReg->setBit(Multiplex::DisplayLedToRegister[i], true);
    //     }
    // }

    // if (dot)
    // {
    //   shiftReg->setBit(Multiplex::DisplayDotToRegister, true);
    // }
 
    // //Set common
    // shiftReg->setBit(Multiplex::DisplayCommonToRegister[currentCommon], true); //Pull LOW
    

    Serial.print("shiftReg");
    Serial.println(shiftReg->dataOut);

    //Send Register
    shiftReg->SendData();
}



 