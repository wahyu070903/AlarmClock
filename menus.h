#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"

#ifndef MENUS_H
  #define MENUS_H
  class Menus{
    private:
      unsigned long time_now;
      LiquidCrystal_I2C lcd;
      uRTCLib rtc;
    public:
      void init();
      int* getDetailTime();
      void mainMenus();
      void optionMenus(int activeOption);
      bool setTimeMenus(int activeCursor,bool inc = false, bool dec = false, bool press = false);

    Menus::Menus() : lcd(0x27, 16, 2), rtc(0x68) {}
  };
#endif