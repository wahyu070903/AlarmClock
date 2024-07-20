#include "eeprom.h"

void EEPROM::storeData(int ID){
  int start_addr = ((ID-1)) * wordCap;
  // eeprom.eeprom_write(start_addr);
  int x = 0;
  Serial.println(sizeof(x));

}

void EEPROM::getDatas(){

}