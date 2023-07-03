#include <Arduino.h>
#include <EEPROM.h>
#include <stdio.h>
#include <Wire.h>;
#include "C:\Users\Administrator\Documents\Arduino\libraries\LiquidCrystal_I2C-master\LiquidCrystal_I2C.h"
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
#define CS_PIN A4
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


void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("MAY HAN CELL PIN");
  lcd.setCursor(1, 1);
  lcd.print("KhacDinh - 19146174");
  lcd.setCursor(1, 2);
  lcd.print("DucDuy   - 19146169");
  lcd.setCursor(1, 3);
  lcd.print("VanThieu - 19146269");

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
}
void X_Trai(unsigned int xung) {
  digitalWrite(ENB_X, 0);
  digitalWrite(DIR_X, 0);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_X, 0);
    delayMicroseconds(600);
    digitalWrite(STEP_X, 1);
    delayMicroseconds(600);
  }
  digitalWrite(ENB_X, 1);
}
void X_Phai(unsigned int xung) {
  digitalWrite(ENB_X, 0);
  digitalWrite(DIR_X, 1);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_X, 0);
    delayMicroseconds(600);
    digitalWrite(STEP_X, 1);
    delayMicroseconds(600);
  }
  digitalWrite(ENB_X, 1);
}

void Y_Vao(unsigned int xung) {
  digitalWrite(ENB_Y, 0);
  digitalWrite(DIR_Y, 1);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_Y, 0);
    delayMicroseconds(600);
    digitalWrite(STEP_Y, 1);
    delayMicroseconds(600);
  }
  digitalWrite(ENB_Y, 1);
}

void Y_Ra(unsigned int xung) {
  digitalWrite(ENB_Y, 0);
  digitalWrite(DIR_Y, 0);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_Y, 0);
    delayMicroseconds(600);
    digitalWrite(STEP_Y, 1);
    delayMicroseconds(600);
  }
  digitalWrite(ENB_Y, 1);
}

void Z() {
  if (digitalRead(CTHT_HAN_PIN) == 1) {
    digitalWrite(DIR_Z, 0);
    digitalWrite(STEP_Z, 0);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
}
