#include "eeprom.h"

void EEPROM::storeData(int id, int data1, int data2){
  //encode data to float  data1.data2
  float wholenum = data1;
  float fracnum = data2 * 0.01;
  float finalnum = wholenum + fracnum;
  int start_addr = (id - 1) * sizeof(finalnum);
  int trychange = 2;
  int trycnt = 0;

  Serial.println(finalnum);
  while(!eeprom.eeprom_write(start_addr,finalnum)){
    if(trycnt == trychange) break;
    Serial.println("failed to store data");
    trycnt++;
  }
}

void EEPROM::getDatas(int id, int* res){
  float result = 0;
  int start_addr = (id - 1) * sizeof(result);
  eeprom.eeprom_read(start_addr,&result);

  // Serial.print("result");
  // Serial.println(result);
  //decode the result 
  int integerPart = static_cast<int>(result);
  float fractionalPart = result - integerPart;
  // Serial.println(fractionalPart);
  int fraction = static_cast<int>(round(fractionalPart * 100));
  res[0] = integerPart;
  res[1] = fraction;

  // Serial.print("after calc");
  // Serial.print(integerPart);
  // Serial.print(",");
  // Serial.println(fraction);
}