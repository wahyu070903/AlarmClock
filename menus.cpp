#include "menus.h"
LiquidCrystal_I2C lcd(0x27,16,2);

void mainMenus(int time_now){
  if(millis() > time_now + 1000){       //display time data to lcd
    int* timeData = getDetailTime();
    for(int i = 0 ; i < 3 ; i++){
      lcd.setCursor(0, 0);
      lcd.print(timeData[0]);
      lcd.print(":");
      lcd.print(timeData[1]);
      lcd.print(":");
      lcd.print(timeData[2]);
    }
    time_now = millis();    //update last value
  }
}