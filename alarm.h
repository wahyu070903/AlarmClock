#include "Arduino.h"
#include "eeprom.h"
#include "voiceplayer.h"
#include "uRTCLib.h"

#ifndef ALARM_H
  #define ALARM_H
  class Alarm{
    private:
      int list_refresh = 1000;
      uRTCLib rtc;
      Voice player;
    public:
      void watch(int hourNow, int minuteNow);
      
    Alarm::Alarm() : rtc(0x68) {};
  };
#endif