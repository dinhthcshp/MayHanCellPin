#include <OneButton.h>
#include <EEPROM.h>
#include <Wire.h>;
#include <LiquidCrystal_I2C.h>;
LiquidCrystal_I2C lcd(0x27,20,4);

///////////////////////TP6600
#define STEP_X 43
#define DIR_X 45
#define ENB_X 47
#define STEP_Y 49
#define DIR_Y 51
#define ENB_Y 53
#define STEP_Z 48
#define DIR_Z 46
#define ENB_Z 44
///////////////////////DIEU_KHIEN
//NUT_NHAN
#define PHAI_PIN 29   //Phai
#define TRAI_PIN 8     //Trai
#define START_PIN 24
#define SET_PIN 25
#define RESET_PIN 27
//SWITCH
#define S1_PIN 4  // 0 - Auto; 1 - Man
#define S2_PIN 9  // 0 - x; 1 - y
//CONG_TAC_HANH_TRINH
#define KHOP_Z 2
#define KHOP_X 3
#define KHOP_Y 5
#define CTHT_HAN_PIN 19
//CHIET_AP
#define TG_PIN A1
#define CELL_PIN A2
//ACS
#define ACS_OUT A3
///////////////////////WORK
#define COI_PIN 23
#define HAN_PIN 22
///////////////////////DEN_BAO
#define DEN_XANH_PIN 14
#define DEN_DO_PIN 15
///////////////////////Max7219_HIEN_THI
#define DIN_PIN 12
#define LOAD_PIN 11
#define CLK_PIN 10
unsigned long time;
int RS = 0;
bool ready = 0;
bool Auto = 0;
int doc[3] = {0, 0, 0};

void setup() {
  pinMode(STEP_X, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  pinMode(ENB_X, OUTPUT);

  pinMode(STEP_Y, OUTPUT);
  pinMode(DIR_Y, OUTPUT);
  pinMode(ENB_Y, OUTPUT);

  pinMode(STEP_Z, OUTPUT);
  pinMode(DIR_Z, OUTPUT);
  pinMode(ENB_Z, OUTPUT);

  digitalWrite(ENB_X, 1); // Tat dong co
  digitalWrite(ENB_Y, 1); // Tat dong co
  digitalWrite(ENB_Z, 1); // Tat dong co
  pinMode(HAN_PIN, OUTPUT);
  digitalWrite(HAN_PIN, 0);

  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(S1_PIN, INPUT_PULLUP);  //Auto
  pinMode(S2_PIN, INPUT_PULLUP);  
  pinMode(PHAI_PIN, INPUT_PULLUP);
  pinMode(TRAI_PIN, INPUT_PULLUP);
  pinMode(KHOP_X, INPUT_PULLUP);
  pinMode(KHOP_Z, INPUT_PULLUP);
  pinMode(KHOP_Y, INPUT_PULLUP);

  pinMode(SET_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(CTHT_HAN_PIN, INPUT_PULLUP);
  //////////////////////////////////////
  pinMode(DEN_XANH_PIN, OUTPUT);
  pinMode(DEN_DO_PIN, OUTPUT);
  pinMode(COI_PIN, OUTPUT);
  digitalWrite(COI_PIN, 0);

  Serial.begin(9600);
  time = millis();

  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình
  lcd.backlight();   //Bật đèn nền
}
void ManProgram();
void runx() {
  digitalWrite(STEP_X, 0);
  delayMicroseconds(600);
  digitalWrite(STEP_X, 1);
  delayMicroseconds(600);
}
void runy() {
  digitalWrite(STEP_Y, 0);
  delayMicroseconds(1000);
  digitalWrite(STEP_Y, 1);
  delayMicroseconds(1000);
}
void runz(){
  digitalWrite(STEP_Z, 0);
  delayMicroseconds(1000);
  digitalWrite(STEP_Z, 1);
  delayMicroseconds(1000);
}
void bao(){
  digitalWrite(COI_PIN, 1);
  delay(100);
  digitalWrite(COI_PIN, 0);
  delay(100);
}
void ManProgram() {
  int qt = 0;
        digitalWrite(ENB_Z, 0);
        if(digitalRead(START_PIN) == 0){
          qt = 1;
        }
        Serial.print("QT = ");
        Serial.println(qt);
        while (qt == 1 && digitalRead(S1_PIN) == 1) {
          if (digitalRead(START_PIN) == 0) {
            digitalWrite(DIR_Z, 0);
            while(digitalRead(CTHT_HAN_PIN) == 1 & digitalRead(START_PIN) == 0) runz();
          }
          if (digitalRead(CTHT_HAN_PIN) == 0) qt = 2;
          Serial.print("QT = ");
          Serial.println(qt);
        }
        while(qt == 2) {
          Serial.print("QT = ");
          Serial.println(qt);
          if (digitalRead(SET_PIN) == 0) {
            //digitalWrite(HAN_PIN, 1);
            //delay(50);
            //digitalWrite(HAN_PIN, 0);
            bao();
            qt = 3;
          }
        }
        while(qt == 3) {
          Serial.print("QT = ");
          Serial.println(qt);
          digitalWrite(DIR_Z, 1);
          while(digitalRead(KHOP_Z) == 1) runz();
          if (digitalRead(KHOP_Z == 0)) qt = 0;
        }
}
void Reset() {
  RS = 0;
  while(RS == 0) {
    digitalWrite(ENB_Z, 0);
    digitalWrite(DIR_Z, 1);
    while(digitalRead(KHOP_Z) == 1) {
      runz();
    }
    if(digitalRead(KHOP_Z) == 0) RS++;
  }
  while(RS == 1) {
    digitalWrite(ENB_X, 0);
    digitalWrite(DIR_X, 1);
    while(digitalRead(KHOP_X) == 1) {
      runx();
    }
    if(digitalRead(KHOP_X) == 0) RS++;
    for(int i = 0; i < 3; i++) {
      bao();
    }
    ready = 1;
  }
}
void loop() {
  while(1) {
    while (digitalRead(S1_PIN) == 0) {
      //AUTO
      Serial.println("AUTO");
      ready = 0;
      Reset();
      while(ready == 1 && digitalRead(S1_PIN) == 0) {
        if(digitalRead(START_PIN) == 0) {
        }
        if (digitalRead(S2_PIN) == 0) {
          Serial.println("X Axis");
          digitalWrite(ENB_X, 1);
          if (digitalRead(TRAI_PIN) == 1) {
            digitalWrite(ENB_X, 0);
            digitalWrite(DIR_X, 0);
            while(digitalRead(TRAI_PIN) == 1) {
              runx();
              doc[0]++;
            }
          }
          if (digitalRead(PHAI_PIN) == 0) {
            if (digitalRead(KHOP_X) == 1) {
              digitalWrite(ENB_X, 0);
              digitalWrite(DIR_X, 1);
              while(digitalRead(PHAI_PIN) == 0 & digitalRead(KHOP_X) == 1) {
                runx();
                doc[0]--;
              }
            }
            else {
              doc[0] = 0;
              for(int i = 0; i < 3; i++) {
                bao();
              }
            }
          }
          if (digitalRead(SET_PIN) == 0) {
            EEPROM.write(0, doc[0]);
            Serial.print("doc x: ");
            Serial.println(EEPROM.read(0));
            bao();
          }
        }
        else {
          Serial.println("Z Axis");
          if (digitalRead(TRAI_PIN) == 1) {
            digitalWrite(ENB_Z, 0);
            digitalWrite(DIR_Z, 0);
            while(digitalRead(TRAI_PIN) == 1) {
              runz();
              doc[1]++;
            }
          }
          if (digitalRead(PHAI_PIN) == 0) {
            Serial.println("Z Len");
            if ( digitalRead(KHOP_Z) == 1) {
              digitalWrite(ENB_Z, 0);
              digitalWrite(ENB_Y, 0);
              digitalWrite(DIR_Z, 1);
              digitalWrite(DIR_Y, 1);
              while(digitalRead(PHAI_PIN) == 0 & digitalRead(KHOP_Z) == 1) {
                runz();
                doc[1]--;
              }
            }
            else {
              doc[1] = 0;
              digitalWrite(ENB_Z, 0);
              for(int i = 0; i < 3; i++) {
                bao();
              }
            }
          }
          if (digitalRead(SET_PIN) == 0) {
            EEPROM.write(1, doc[1]);
            Serial.print("doc z: ");
            Serial.println(EEPROM.read(1));
            bao();
          }
        }
      }
      
    }// AUTO
    while (digitalRead(S1_PIN) == 1) {//MAN
      Serial.println("MAN");
      ready = 0;
      Reset();
      while (ready == 1 && digitalRead(S1_PIN) == 1) {//May da san sang hoat dong che do MAN
        ManProgram();
      }
    }
    Serial.print("\nEND\n");
  }
}