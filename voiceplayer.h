#include "Arduino.h"
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

#ifndef VOICE_H
  #define VOICE_H
  class Voice{
    private:
      SoftwareSerial comm;
      DFRobotDFPlayerMini player;
    public:
      void init();
      void listFiles();
      void playFile(int ID);
    
    Voice::Voice() : comm(10,11) {};
  };
#endif