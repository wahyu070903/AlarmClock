#include <Arduino.h>
#include "Wire.h"
#include "uEEPROMLib.h"

#ifndef EEPROM_H
  #define EEPROM_H
  class EEPROM {
    private:
      uEEPROMLib eeprom;
      int wordCap = 33;
    public:
      void storeData(int ID);
      void getDatas();
    EEPROM::EEPROM() : eeprom(0x57) {}
  }; 
#endif