// #include "Display.h"
  
//   Display Display::Single = Display();

//   Multiplex *Display::multiplex = nullptr;
  
//   void Display::setDisplay1(String text){
//     this->display1 = text;
//     print();
//   }

//   void Display::setDisplay2(String text){
//     this->display2 = text;
//     print();
//   }

//   void Display::setAllDisplay1Dots(bool value){
//     for(int i =0; i < 4; ++i){
//       dots[i] = value;
//     }
//     print();
//   }
//   void Display::setAllDisplay2Dots(bool value){
//     for(int i =4; i < 7; ++i){
//       dots[i] = value;
//     }
//     print();
//   }

//   void Display::setDisplay1Dot(int pos, bool dot){
//     this->dots[pos] = dot;
//     print();
//   }

//   void Display::setDisplay2Dot(int pos, bool dot){
//     this->dots[pos+4] = dot;
//     print();
//   }

//   void Display::print(){
//     Serial.print("D1 (");
//     Serial.print(display1);
//     Serial.print("), D2 (");
//     Serial.print(display2);
//     Serial.println(")");

//     Serial.print("dot(");
//     for(int i =0; i < 4; ++i){
//       if (dots[i]){
//         Serial.print(".");
//       }
//       else{
//         Serial.print("-");
//       }
//     }
    
//     Serial.print("), D2 (");
//     for(int i =4; i < 7; ++i){
//       if (dots[i]){
//         Serial.print(".");
//       }
//       else{
//         Serial.print("-");
//       }
//     }
//     Serial.println(")");

//   }
