#include <OneButton.h>
#include <EEPROM.h>
#include <Wire.h>;
#include <LiquidCrystal_I2C.h>;
LiquidCrystal_I2C lcd(0x27, 20, 4);

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
#define PHAI_PIN 29  //Phai
#define TRAI_PIN 8   //Trai
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
bool Auto = 0;
int RS = 0;
int doc[3] = { 0, 0, 0 };
unsigned int page = 1;
void bao(){
  digitalWrite(COI_PIN, 1);
  delay(100);
  digitalWrite(COI_PIN, 0);
  delay(100);
}
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

  digitalWrite(ENB_X, 1);  // Tat dong co
  digitalWrite(ENB_Y, 1);  // Tat dong co
  digitalWrite(ENB_Z, 1);  // Tat dong co
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
  lcd.backlight();  //Bật đèn nền

  lcd.setCursor(1, 0);
  lcd.print("MAY HAN CELL PIN");
  lcd.setCursor(0, 1);
  lcd.print("KhacDinh-19146174");
  lcd.setCursor(0, 2);
  lcd.print("DucDuy-19146174");
  lcd.setCursor(0, 3);
  lcd.print("VanThieu-19146174");
  delay(2000);
  lcd.clear();
}


void loop() {
  if ((unsigned long)(millis() - time) > 500) {
    Serial.print("\n12");
    time = millis();
  }
  RS = 0;
  while(RS == 0) {
    digitalWrite(ENB_Z, 0);
    digitalWrite(DIR_Z, 1);
    while(digitalRead(KHOP_Z) == 1) {
      digitalWrite(STEP_Z, 0);
      delayMicroseconds(800);
      digitalWrite(STEP_Z, 1);
      delayMicroseconds(800);
    }
    if(digitalRead(KHOP_Z) == 0) RS++;
  }
  while(RS == 1) {
    digitalWrite(ENB_X, 0);
    digitalWrite(DIR_X, 1);
    while(digitalRead(KHOP_X) == 1) {
      digitalWrite(STEP_X, 0);
      delayMicroseconds(600);
      digitalWrite(STEP_X, 1);
      delayMicroseconds(600);
    }
    if(digitalRead(KHOP_X) == 0) RS++;
    for(int i = 0; i < 3; i++) {
      bao();
    }
  }
  if(digitalRead(START_PIN) == 0) {
    
  }
  // put your main code here, to run repeatedly:
}
