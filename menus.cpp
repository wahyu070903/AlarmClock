#include "menus.h"
#include "eeprom.h"

void Menus::init(){
  lcd.init();
  lcd.backlight();
  URTCLIB_WIRE.begin();
  time_now = millis();
}

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
}

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
    //Serial.println(rtc.hour());
  }
}

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
    if(inc && setTime[0] < 23){
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

  return false;
}

bool setAlarm_first = false;
int lastScrollIndex = 0;
int activeTrack[2] = {0,1};
int Menus::setAlarm(int scrollIndex, int buttonState){
  if(!setAlarm_first){
    lcd.clear();
    setAlarm_first = true;
    lastScrollIndex = scrollIndex;
    activeTrack[0] = scrollIndex;
    activeTrack[1] = scrollIndex + 1;
  }

  if(activeTrack[0] == 0){
    lcd.setCursor(0, 0);
    lcd.print("<= back"); 
  }else{
    lcd.setCursor(0, 0);
    lcd.print("Track_");
    lcd.setCursor(6,0);
    lcd.print(activeTrack[0]);
  }

  if(activeTrack[0] != 10){
    lcd.setCursor(0, 1);
    lcd.print("Track_");
    lcd.setCursor(6,1);
    lcd.print(activeTrack[1]);
  }

  lcd.setCursor(14,0);
  lcd.print("<"); 

  if(scrollIndex != lastScrollIndex) setAlarm_first = false; 

  if(buttonState == 1 && activeTrack[0] == 0){
    setAlarm_first = false;
    lastScrollIndex = 0;
    return -1;
  }

  if(buttonState == 1 && activeTrack[0] != 0){
    setAlarm_first = false;
    return activeTrack[0];
  }
  return false;
}

bool detailSetAlarm_refresh = false;
int detailSetAlarm_hour = 0;
int detailSetAlarm_minutes = 0;
int detailSetAlarm_ActiveCursor = 0;
int detailSetAlarm_Accept = 1;
int Menus::detailSetAlarm(int alarmID, int encDir, int button, unsigned int* param_time){
  EEPROM eeprom;
  if(!detailSetAlarm_refresh){
    int eepromtime[2];
    eeprom.getDatas(alarmID,eepromtime);
    detailSetAlarm_hour = eepromtime[0];
    detailSetAlarm_minutes = eepromtime[1];
    //Serial.println(eepromtime[0]);
    //Serial.println(eepromtime[1]);
    lcd.clear();
    detailSetAlarm_refresh = true;
  }

  lcd.setCursor(0,0);
  lcd.print("trig. time");
  lcd.setCursor(11,0);
  lcd.print("ID:");
  lcd.print(alarmID);
  lcd.setCursor(0,1);
  if(detailSetAlarm_hour < 10) lcd.print("0");
  lcd.print(detailSetAlarm_hour); 
  lcd.print(":");
  if(detailSetAlarm_minutes < 10) lcd.print("0");
  lcd.print(detailSetAlarm_minutes); 
  lcd.setCursor(10,1);
  lcd.print("ok");
  lcd.setCursor(14,1);
  lcd.print("no");
  if(encDir != 0 && detailSetAlarm_ActiveCursor == 0){
    if(encDir == 200 && detailSetAlarm_hour < 23){
      detailSetAlarm_hour ++;
    }
    if(encDir == 201 && detailSetAlarm_hour > 0){
      detailSetAlarm_hour --;
    }
  }

  if(encDir != 0 && detailSetAlarm_ActiveCursor == 1){
    if(encDir == 200 && detailSetAlarm_minutes < 59){
      detailSetAlarm_minutes ++;
    }
    if(encDir == 201 && detailSetAlarm_minutes > 0){
      detailSetAlarm_minutes --;
    }
  }

  if(detailSetAlarm_ActiveCursor == 2){
    if(encDir == 200) detailSetAlarm_Accept = 0;
    if(encDir == 201) detailSetAlarm_Accept = 1;
    lcd.setCursor(13,1);
    lcd.print(" ");
    lcd.setCursor(9,1);
    lcd.print(" ");
    (detailSetAlarm_Accept == 0) ? lcd.setCursor(13,1) : lcd.setCursor(9,1);
    lcd.print(">");
  }

  if(detailSetAlarm_ActiveCursor == 2 && button && detailSetAlarm_Accept == 0){
    detailSetAlarm_refresh = false;
    detailSetAlarm_hour = 0;
    detailSetAlarm_minutes = 0;
    detailSetAlarm_ActiveCursor = 0;
    detailSetAlarm_Accept = 1;
    return -1;
  }
  if(detailSetAlarm_ActiveCursor == 2 && button && detailSetAlarm_Accept == 1){
    int* ptr;
    ptr = param_time;
    ptr[0] = detailSetAlarm_hour;
    ptr[1] = detailSetAlarm_minutes;
    eeprom.storeData(alarmID, detailSetAlarm_hour, detailSetAlarm_minutes);
    detailSetAlarm_refresh = false;
    detailSetAlarm_hour = 0;
    detailSetAlarm_minutes = 0;
    detailSetAlarm_ActiveCursor = 0;
    detailSetAlarm_Accept = 1;
    return alarmID;
  }

  if(button && detailSetAlarm_ActiveCursor < 2){
    detailSetAlarm_ActiveCursor ++;
  }
  // if(encDir != 0 || button != 0) detailSetAlarm_refresh = false;
  return 0;
}

bool listNotification_first = false;



  