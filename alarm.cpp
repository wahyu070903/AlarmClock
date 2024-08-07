#include "alarm.h"

int trackConf[10][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};

EEPROM eeprom;
unsigned long lastTrackRefresh = 0;
bool isRefresh = false;
int repeat = 1;
int lastminute = 0;
int repeat_cnt = 0;
bool onlyonce = false;

void Alarm::watch(int hourNow, int minuteNow){
  if(!isRefresh){
    lastTrackRefresh = millis();
    isRefresh = true;
  }
  if(millis() - lastTrackRefresh > 1000){
    //Serial.println("refreshtime");
    for(int i = 0; i < 10; i++){
      int timeData[2];
      eeprom.getDatas(i + 1, timeData);
      trackConf[i][0] = i + 1;
      trackConf[i][1] = timeData[0];
      trackConf[i][2] = timeData[1];
    }
    isRefresh = false;

    for(int i = 0 ; i < 10 ; i++){
      int alarmID = trackConf[i][0];
      int hour = trackConf[i][1];
      int minute = trackConf[i][2];
      if(hourNow == hour && minuteNow == minute && onlyonce == false){
        player.init();
        player.playFile(alarmID);
        //Serial.println(alarmID);
        lastminute = minuteNow;
        repeat_cnt++;
        if(repeat_cnt == repeat){
          onlyonce = true;
        }
      }
    }
    if(onlyonce == true && lastminute != minuteNow){
      onlyonce = false;
      repeat_cnt = 0;
    }
    //check stored datas
    // for(int i = 0 ; i < 10; i++){
    //   Serial.print(trackConf[i][0]);
    //   Serial.print(".");
    //   Serial.print(trackConf[i][1]);
    //   Serial.print(".");
    //   Serial.print(trackConf[i][2]);
    //   Serial.print(",");
    // }
    // Serial.println("");

    // Serial.print(hourNow);
    // Serial.print("x");
    // Serial.println(minuteNow);
  }
}