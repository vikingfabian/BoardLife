
#ifndef REFH

// Defines X if above
// conditions fails
#define REFH
enum ModeType : byte
 {
  Life,
  Timer, 
  ChessTimer,
  Dice,
  Settings_Sound,
  Settings_Reset,
  NUM
};



class Ref{
public:
 
 static const byte CHAR_NUM = 1;
 static const byte CHAR_MINUS = 11;
  static const byte CHAR_A = 12;
  static const byte CHAR_b = 13;
  static const byte CHAR_C = 14;
  static const byte CHAR_d = 15;
  static const byte CHAR_E = 16;
  static const byte CHAR_F = 17;
  static const byte CHAR_H = 18;
  static const byte CHAR_L = 19;
  static const byte CHAR_n = 20;
  static const byte CHAR_P = 21;
  static const byte CHAR_r = 22;
  static const byte CHAR_t = 23;
  static const byte CHAR_U = 24;
  static const byte CHAR_J = 25;
  static const byte CHAR_y = 26;
  static const byte CHAR_Z = 27;

  static const bool KeyDown = true;
  static const bool KeyUp = false;
  static const int TimerMinMax = 90;

  //static BoardLifeMain bfmain;
  static const byte MEM_ADRESS_CHECK1 = 0;
  static const byte MEM_ADRESS_CHECK2 = 1;
  static const byte MEM_ADRESS_MODE_TYPE = 2;
  static const byte MEM_ADRESS_MODE_INDEX = 3;
  static const byte MEM_ADRESS_MODE_BUTTON = 4;

  static const byte MEM_ADRESS_LIFE_VALUESTART = 5; //2 bytes

  static const byte MEM_ADRESS_TIMER_VALUEMINSTART = 7; //2 bytes
  static const byte MEM_ADRESS_TIMER_VALUESECSTART = 9; //2 bytes

  static const byte MEM_ADRESS_DICE_MAX = 11; //2 bytes
  
};

#endif