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
        //Serial.print(digitalRead(ENC_DT));
        //Serial.println(currentCLKState);
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
        if(millis() - lastButtonState > 100){
          result = 1;
        }
        lastButtonState = millis();
      }
      return result;
    }
};

Encoder enc;
Menus menu;

int active_menus = 0;
unsigned int option1_cnt = 0;   //main option list counter
unsigned int option2_cnt = 0;   //current time setter cursor counter
void setup(){
  enc.init();
  menu.init();
  Serial.begin(9600);

}

void loop() {
  int encDirection = enc.getDirection();
  if(encDirection != 0){
    switch(active_menus){
      case 1:
        if(option1_cnt > 0 && encDirection == 201){
          option1_cnt-- ;
        }else if(option1_cnt < 2 && encDirection == 200){
          option1_cnt++ ;
        }
        break;
    }
    //Serial.println(encDirection);
  }

  // Menus sequencing start here 
  if(enc.getButtonState()){
    if(active_menus == 1 && option1_cnt == 0){    //change current time menus
      active_menus = 2;
      option1_cnt = 0;
    }
    if(active_menus == 0) active_menus = 1; // should at end
    if(active_menus == 1 && option1_cnt == 2){  // back option menus
      active_menus = 0;
      option1_cnt = 0;
      Serial.println("back");
    };
    Serial.println("Pressed");
  }
  switch(active_menus){
    case 0:
      menu.mainMenus();
      break;
    case 1:
      menu.optionMenus(option1_cnt);
      break;
    case 2:
      int isdone = false;
      while(!isdone){
        bool increment = false;
        bool decrement = false;
        bool pressed = false;
        int rotation = enc.getDirection();
        if(rotation != 0){
          (rotation == 200) ? increment = true : increment = false;
          (rotation == 201) ? decrement = true : decrement = false;
          Serial.println(rotation);
        }
        if(enc.getButtonState()) option2_cnt++;
        bool finish = menu.setTimeMenus(option2_cnt,increment,decrement,pressed);
        if(finish) isdone = true;
      }
      active_menus = 0;
      option2_cnt = 0;
      break;
  }

  // Serial.print(option1_cnt);
  // Serial.print(",");
  // Serial.println(active_menus);
}
