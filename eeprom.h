#include <Arduino.h>
#include "Wire.h"
#include "uEEPROMLib.h"

#ifndef EEPROM_H
  #define EEPROM_H
  class EEPROM {
    private:
      uEEPROMLib eeprom;
    public:
      void storeData(int id, int data1, int data2);
      void getDatas(int id, int* arrayres);
    EEPROM::EEPROM() : eeprom(0x57) {
      Wire.begin();
    }
  }; 
#endif