#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "uRTCLib.h"
#include "menus.h"

#define ENC_CLK 2
#define ENC_DT 3
#define ENC_SW 4
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class Encoder { 
  public:
    int lastCLKState;
    int currentCLKState;
    unsigned long lastButtonState = 0;
    void init(){
      pinMode(ENC_CLK, INPUT);
      pinMode(ENC_DT, INPUT);
      pinMode(ENC_SW, INPUT_PULLUP);
      lastCLKState = digitalRead(ENC_CLK);
    }
    int getDirection(){
      /* return code 200 meand CW, 201 mean CCW */
      currentCLKState = digitalRead(ENC_CLK);
      int direction;
      if(currentCLKState != lastCLKState && currentCLKState == HIGH){
        if(digitalRead(ENC_DT) != currentCLKState){
          direction = 200;
        }else{
          direction = 201;
        }
      }else{
        direction = 0;
      }
      lastCLKState = currentCLKState;
      return direction;
    }

    int getButtonState(){
      // 1 mean pressed 0 mean otherwise
      int result = 0;
      if(digitalRead(ENC_SW) == LOW){
        if(millis() - lastButtonState > 50){
          result = 1;
        }
        lastButtonState = millis();
      }
      return result;
    }
};

//LiquidCrystal_I2C lcd(0x27,16,2);
Encoder enc;
uRTCLib rtc(0x68);

int* getDetailTime(){
  int hour = rtc.hour();
  int minute = rtc.minute();
  int second = rtc.second();
  
  static int dateData[3];
  dateData[0] = hour;
  dateData[1] = minute;
  dateData[2] = second;

  //use for debug only
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
  
  return dateData;
}

unsigned long time_now;
void setup() {
  lcd.init();
  enc.init();
  URTCLIB_WIRE.begin();
  Serial.begin(9600);

  lcd.backlight();
  // lcd.setCursor(3,0);
  // lcd.print("Hello, world!");
  //rtc.set(0, 39, 1, 5, 16, 7, 24);

  time_now = millis();
}

void loop() {
  rtc.refresh();

  // if(millis() > time_now + 1000){       //display time data to lcd
  //   int* timeData = getDetailTime();
  //   for(int i = 0 ; i < 3 ; i++){
  //     lcd.setCursor(0, 0);
  //     lcd.print(timeData[0]);
  //     lcd.print(":");
  //     lcd.print(timeData[1]);
  //     lcd.print(":");
  //     lcd.print(timeData[2]);
  //   }
  //   time_now = millis();    //update last value
  // }
  
  mainMenus(time_now);

  int encDirection = enc.getDirection();

  if(encDirection != 0){
    Serial.println(encDirection);
  }

  if(enc.getButtonState()){
    Serial.println("pressed");
  }
}
