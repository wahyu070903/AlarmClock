#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"

#ifndef MENUS_H
  #define MENUS_H
  void mainMenus(int time_now);
  void optionMenus();
#endif