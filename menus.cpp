#include "menus.h"

void Menus::init(){
  lcd.init();
  lcd.backlight();
  URTCLIB_WIRE.begin();
  time_now = millis();
};

int* Menus::getDetailTime(){
  int hour = rtc.hour();
  int minute = rtc.minute();
  int second = rtc.second();
  static int dateData[3];
  dateData[0] = hour;
  dateData[1] = minute;
  dateData[2] = second;

  //use for debug only
  // Serial.print(hour);
  // Serial.print(":");
  // Serial.print(minute);
  // Serial.print(":");
  // Serial.println(second);
  return dateData;
};

void Menus::mainMenus(){
  rtc.refresh();
  if(millis() > time_now + 1000){       //display time data to lcd
    int* timeData = getDetailTime();
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("current time");
    for(int i = 0 ; i < 3 ; i++){
      lcd.setCursor(0, 1);
      lcd.print(timeData[0]);
      lcd.print(":");
      lcd.print(timeData[1]);
      lcd.print(":");
      lcd.print(timeData[2]);
    }
    time_now = millis();    //update last value
  }
};

int lastActiveOption;
void Menus::optionMenus(int activeOption){
  if(lastActiveOption != activeOption) lcd.clear();
  lastActiveOption = activeOption;
  switch(activeOption){
    case 0:
      lcd.setCursor(0,0);
      lcd.print(">Set Curr. Time");
      lcd.setCursor(0,1);
      lcd.print("set alarm");
      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("Set Curr. Time");
      lcd.setCursor(0,1);
      lcd.print(">set alarm");
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("set alarm");
      lcd.setCursor(0,1);
      lcd.print(">back");
      break;
  }
}

int setTime[2] = {0,0};
bool isfilled = false;
bool Menus::setTimeMenus(int activeCursor,bool inc, bool dec, bool press){
  if(!isfilled){
    setTime[0] = rtc.hour();
    setTime[1] = rtc.minute();
    isfilled = true;
    lcd.clear();
  };
  lcd.setCursor(0,0);
  if(setTime[0] < 10) lcd.print("0");
  lcd.print(setTime[0]);
  lcd.print(":");
  if(setTime[1] < 10) lcd.print("0");
  lcd.print(setTime[1]);
  lcd.setCursor(13,0);
  lcd.print("OK");
  
  if(activeCursor == 0){
    lcd.setCursor(0,1);
    lcd.print("^");
    if(inc && setTime[0] < 24){
      setTime[0] += 1;
      lcd.clear();
    }
    if(dec && setTime[0] > 0){
      setTime[0] -= 1;
      lcd.clear();
    }
  }

  if(activeCursor == 1){
    lcd.setCursor(0,1);
    lcd.print("   ^");
    if(inc && setTime[1] < 59){
      setTime[1] += 1;
      lcd.clear();
    }
    if(dec && setTime[1] > 0){
      setTime[1] -= 1;
      lcd.clear();
    }
  }

  if(activeCursor == 2){
    lcd.setCursor(0,1);
    lcd.print("             ^");
  }

  if(activeCursor == 3){
    rtc.set(rtc.second(),setTime[1],setTime[0],0,0,0,0);
    isfilled = false;
    return true;
  }
  // Serial.print(inc);
  // Serial.println(dec);
  // Serial.println(activeCursor);
  // Serial.println("sds");
  return false;
}

  