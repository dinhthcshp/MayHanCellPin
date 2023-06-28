#include <EEPROM.h>
#include <stdio.h>
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
#define CS_PIN A4
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
unsigned int type = 0;
unsigned int data[10][6];
unsigned int data1[6];
bool home[3] = { 0, 0, 0 };
bool RUN = 0;

void setup() {
  lcd.init();       //Kh?i d?ng mï¿½n hï¿½nh. B?t d?u cho phï¿½p Arduino s? d?ng mï¿½n hï¿½nh
  lcd.backlight();  //B?t dï¿½n n?n
  lcd.setCursor(2, 0);
  lcd.print("MAY HAN CELL PIN");
  lcd.setCursor(1, 1);
  lcd.print("KhacDinh - 19146174");
  lcd.setCursor(1, 2);
  lcd.print("DucDuy - 19146174");
  lcd.setCursor(1, 3);
  lcd.print("VanThieu - 19146174");

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
  data[0][0] = EEPROM.read(0);
  data[0][1] = EEPROM.read(1);
  data[0][2] = EEPROM.read(2);
  data[0][3] = EEPROM.read(3);
  data[0][4] = EEPROM.read(4);
  data[0][5] = EEPROM.read(5);
  data1[3] = data1[4] = 1;
  // EEPROM.write(0, 100);
  // delay(5);
  // EEPROM.write(1, 100);
  // delay(5);
  // EEPROM.write(2, 133);
  // delay(5);
  // EEPROM.write(3, 2);
  // delay(5);
  // EEPROM.write(4, 4);
  // delay(5);
  // EEPROM.write(5, 20);
}
void runx(int mu) {
  digitalWrite(STEP_X, 0);
  delayMicroseconds(mu);
  digitalWrite(STEP_X, 1);
  delayMicroseconds(mu);
}
void runy(int mu) {
  digitalWrite(STEP_Y, 0);
  delayMicroseconds(mu);
  digitalWrite(STEP_Y, 1);
  delayMicroseconds(mu);
}
void runz(int mu) {
  digitalWrite(STEP_Z, 0);
  delayMicroseconds(mu);
  digitalWrite(STEP_Z, 1);
  delayMicroseconds(mu);
}
void bao() {
  digitalWrite(COI_PIN, 1);
  delay(100);
  digitalWrite(COI_PIN, 0);
  delay(100);
}
void AutoProgram(int distanceX, int distanceY, int cellY, int cellX) {
  digitalWrite(ENB_X, 1);
  digitalWrite(ENB_Y, 1);
  home[0] = 0;
  EEPROM.write(100, home[0]);
  if (cellX % 2 == 0) home[1] = 1;
  else home[1] = 0;
  EEPROM.write(101, home[1]);
  for (int cot = 0; cot < cellX; cot++) {
    for (int hang = 0; hang < cellY; hang++) {
      unsigned short int TrucZ = 0;
      while (digitalRead(CTHT_HAN_PIN) == 1 && TrucZ == 0) {
        digitalWrite(DIR_Z, 0);
        runz(800);
        if (digitalRead(CTHT_HAN_PIN) == 0) TrucZ = 1;
      }
      while (digitalRead(KHOP_Z) == 1 && TrucZ == 1) {
        digitalWrite(DIR_Z, 1);
        runz(800);
        if (digitalRead(KHOP_Z) == 0) TrucZ = 1;
      }
      if (hang < cellY - 1) {
        if (cot % 2 == 0) {
          digitalWrite(ENB_Y, 0);
          digitalWrite(DIR_Y, 1);
          for (int xungy = 0; xungy < distanceY; xungy++) {
            runy(600);
          }
          digitalWrite(ENB_Y, 1);
        } else {
          digitalWrite(ENB_Y, 0);
          digitalWrite(DIR_Y, 0);
          for (int xungy = 0; xungy < distanceY; xungy++) {
            runy(600);
          }
          digitalWrite(ENB_Y, 1);
        }
      }
    }
    if (cot == cellX - 1) {
      for (int i = 0; i < 3; i++) {
        bao();
      }
      break;
    } else {
      digitalWrite(ENB_X, 0);
      digitalWrite(DIR_X, 0);
      for (int xungx = 0; xungx < distanceX; xungx++) {
        runx(600);
      }
      digitalWrite(ENB_X, 1);
    }
  }
  Reset(distanceX * (cellX - 1), distanceY * (cellY - 1));
  RUN = 0;
}
void ManProgram() {
  int qt = 0;
  digitalWrite(ENB_Z, 0);
  if (digitalRead(START_PIN) == 0) {
    qt = 1;
  }
  while (qt == 1 && digitalRead(S1_PIN) == 1) {
    if (digitalRead(START_PIN) == 0) {
      digitalWrite(DIR_Z, 0);
      while (digitalRead(CTHT_HAN_PIN) == 1 & digitalRead(START_PIN) == 0) runz(800);
    }
    if (digitalRead(CTHT_HAN_PIN) == 0) qt = 2;
  }
  while (qt == 2) {
    if (digitalRead(SET_PIN) == 0) {
      bao();
      qt = 3;
    }
  }
  while (qt == 3) {
    digitalWrite(DIR_Z, 1);
    while (digitalRead(KHOP_Z) == 1) runz(800);
    if (digitalRead(KHOP_Z == 0)) qt = 0;
  }
}
void Home() {
  ready = 0;
  RS = 0;
  unsigned short int tmp = 0;
  while (RS == 0) {
    digitalWrite(ENB_Z, 0);
    digitalWrite(DIR_Z, 1);
    while (digitalRead(KHOP_Z) == 1) {
      runz(1000);
    }
    if (digitalRead(KHOP_Z) == 0) RS = 1;
  }
  while (RS == 1) {
    digitalWrite(ENB_X, 0);
    digitalWrite(DIR_X, 1);
    while (digitalRead(KHOP_X) == 1 && tmp == 0) {
      runx(600);
      if (digitalRead(KHOP_X) == 0) tmp = 1;
    }
    while (tmp == 1) {
      digitalWrite(ENB_X, 0);
      digitalWrite(DIR_X, 0);
      for (int i = 0; i < 300; i++) {
        runx(600);
      }
      RS = 2;
      tmp = 2;
      home[0] = 1;
      EEPROM.write(100, home[0]);
      break;
    }
  }
  while (RS == 2) {
    digitalWrite(ENB_Y, 0);
    digitalWrite(DIR_Y, 1);
    while (digitalRead(KHOP_Y) == 1 && tmp == 2) {
      runy(1000);
      if (digitalRead(KHOP_Y) == 0) tmp = 3;
    }
    while (tmp == 3) {
      digitalWrite(ENB_Y, 0);
      digitalWrite(DIR_Y, 0);
      for (int i = 0; i < 1381; i++) {
        runy(800);
      }
      RS++;
      tmp = 4;
      home[1] = 1;
      EEPROM.write(101, home[1]);
      break;
    }
    for (int i = 0; i < 3; i++) {
      bao();
    }
    ready = 1;
  }
}
void Reset(int distanceX, int distanceY) {
  while (digitalRead(KHOP_Z) == 1) {
    digitalWrite(DIR_Z, 1);
    digitalWrite(ENB_Z, 0);
    runz(800);
  }
  home[2] = 1;
  if (home[0] == 0) {
    digitalWrite(DIR_X, 1);
    digitalWrite(ENB_X, 0);
    for (int i = 0; i < distanceX; i++) {
      runx(600);
    }
    home[0] = 1;
  }
  if (home[1] == 0) {
    digitalWrite(DIR_Y, 0);
    digitalWrite(ENB_Y, 0);
    for (int i = 0; i < distanceY; i++) {
      runy(600);
    }
    home[1] = 1;
  }
  EEPROM.write(100, home[0]);
  EEPROM.write(101, home[1]);
  EEPROM.write(102, home[2]);
}
void Show(int loai, int distanceX, int distanceY, int distanceZ, int cellY, int cellX, int tg) {
  lcd.setCursor(4, 0);
  lcd.print("Pin loai");
  lcd.setCursor(13, 0);
  lcd.print(loai);
  lcd.setCursor(0, 1);
  lcd.print("Khoang cach:");
  lcd.setCursor(2, 2);
  lcd.print("X:");
  lcd.setCursor(4, 2);
  lcd.print(distanceX);
  lcd.setCursor(8, 2);
  lcd.print("Y:");
  lcd.setCursor(10, 2);
  lcd.print(distanceY);
  lcd.setCursor(14, 2);
  lcd.print("Z:");
  lcd.setCursor(16, 2);
  lcd.print(distanceZ);
  lcd.setCursor(0, 3);
  lcd.print("Cell:");
  lcd.setCursor(5, 3);
  lcd.print(cellX);
  lcd.setCursor(7, 3);
  lcd.print("x");
  lcd.setCursor(9, 3);
  lcd.print(cellY);
  lcd.setCursor(14, 3);
  lcd.print("T=");
  lcd.setCursor(16, 3);
  lcd.print(tg);
  lcd.setCursor(18, 3);
  lcd.print("ms");
}
void ShowSelect(int loai, int distanceX, int distanceY, int distanceZ, int cellY, int cellX, int tg) {
  lcd.setCursor(0, 0);
  lcd.print("Dieu chinh pin loai");
  lcd.setCursor(19, 0);
  lcd.print(loai);
  lcd.setCursor(0, 1);
  lcd.print("Khoang cach:");
  lcd.setCursor(2, 2);
  lcd.print("X:");
  lcd.setCursor(4, 2);
  lcd.print(distanceX);
  lcd.setCursor(8, 2);
  lcd.print("Y:");
  lcd.setCursor(10, 2);
  lcd.print(distanceY);
  lcd.setCursor(14, 2);
  lcd.print("Z:");
  lcd.setCursor(16, 2);
  lcd.print(distanceZ);
  lcd.setCursor(0, 3);
  lcd.print("Cell:");
  lcd.setCursor(5, 3);
  lcd.print(cellX);
  lcd.setCursor(7, 3);
  lcd.print("x");
  lcd.setCursor(9, 3);
  lcd.print(cellY);
  lcd.setCursor(14, 3);
  lcd.print("T=");
  lcd.setCursor(16, 3);
  lcd.print(tg);
  lcd.setCursor(18, 3);
  lcd.print("ms");
}
void loop() {
  while (1) {
    while (digitalRead(S1_PIN) == 0) {
      //AUTO
      if (EEPROM.read(100) == 0 || EEPROM.read(101) == 0) {
        Home();
      } else {
        Reset(0, 0);
        ready = 1;
      }
      lcd.clear();
      while (ready == 1 && digitalRead(S1_PIN) == 0) {
        while (Serial.available()) {
          RUN = Serial.read();
          delay(200);
        }
        type = EEPROM.read(70);
        Show(type + 1, EEPROM.read(type * 6 + 0) * 3, EEPROM.read(type * 6 + 1) * 3, EEPROM.read(type * 6 + 2) * 3, EEPROM.read(type * 6 + 3), EEPROM.read(type * 6 + 4), EEPROM.read(type * 6 + 5));
        Serial.print(EEPROM.read(type * 6 + 0) * 3);
        Serial.print(";");
        Serial.println(EEPROM.read(type * 6 + 4) * 3);
        if (digitalRead(START_PIN) == 0) {
          RUN = 1;
        }
        if (RUN == 1) {
          digitalWrite(DEN_DO_PIN, 1);
          digitalWrite(DEN_XANH_PIN, 0);
          AutoProgram(EEPROM.read(type * 6 + 0) * 3, EEPROM.read(type * 6 + 1) * 3, EEPROM.read(type * 6 + 3), EEPROM.read(type * 6 + 4));
        } else {
          digitalWrite(DEN_DO_PIN, 0);
          digitalWrite(DEN_XANH_PIN, 1);
        }
        if (digitalRead(SET_PIN) == 0) {
          bao();
          int SET = 1;
          int SELECT = 0;
          lcd.clear();
          while (SET == 1) {
            if (digitalRead(PHAI_PIN) == 0) {
              delay(20);
              if (digitalRead(PHAI_PIN) == 0) {
                type++;
                if (type > 10) type = 1;
                lcd.clear();
                EEPROM.write(70, type);
              }
            }
            if (digitalRead(TRAI_PIN) == 1) {
              delay(20);
              if (digitalRead(TRAI_PIN) == 1) {
                type--;
                if (type < 1) type = 10;
                lcd.clear();
                EEPROM.write(70, type);
              }
            }
            if (digitalRead(RESET_PIN) == 0) {
              SET = 3;
            }
            type = EEPROM.read(70);
            ShowSelect(type + 1, EEPROM.read(type * 6 + 0) * 3, EEPROM.read(type * 6 + 1) * 3, EEPROM.read(type * 6 + 2) * 3, EEPROM.read(type * 6 + 3), EEPROM.read(type * 6 + 4), EEPROM.read(type * 6 + 5));
            if (digitalRead(SET_PIN) == 0) {
              if (type == 1 || type == 2) {
                for (int i = 0; i < 3; i++) {
                  bao();
                }
              } else {
                for (int i = 0; i < 7; i++) {
                  data1[i] = EEPROM.read(type * 6 + i) * 3;
                }
                SET = 2;
                break;
              }
            }
          }
          while (SET == 2) {
            type = EEPROM.read(70);
            ShowSelect(type + 1, data1[0], data1[1], data1[2], data1[3], data1[4], data1[5]);
            SELECT = int((float(analogRead(TG_PIN)) / 1023.0) * 5.0);
            switch (SELECT) {
              case 0:
                if (unsigned(millis()) - time > 500) {
                  lcd.setCursor(4, 2);
                  lcd.print("    ");
                  time = millis();
                } else {
                  lcd.setCursor(4, 2);
                  lcd.print(data1[0]);
                }
                digitalWrite(ENB_X, 1);
                if (digitalRead(TRAI_PIN) == 1) {
                  home[0] = 0;
                  EEPROM.write(100, home[0]);
                  digitalWrite(ENB_X, 0);
                  digitalWrite(DIR_X, 0);
                  while (digitalRead(TRAI_PIN) == 1) {
                    runx(1200);
                    data1[0]++;
                  }
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  if (home[0] == 0) {
                    digitalWrite(ENB_X, 0);
                    digitalWrite(DIR_X, 1);
                    while (digitalRead(PHAI_PIN) == 0 & digitalRead(KHOP_X) == 1) {
                      runx(1200);
                      data1[0]--;
                    }
                  } else {
                    data1[0] = 0;
                    home[0] = 1;
                    EEPROM.write(100, home[0]);
                    for (int i = 0; i < 3; i++) {
                      bao();
                    }
                  }
                }
                break;
              case 1:
                if (unsigned(millis()) - time > 500) {
                  lcd.setCursor(10, 2);
                  lcd.print("    ");
                  time = millis();
                } else {
                  lcd.setCursor(10, 2);
                  lcd.print(data1[1]);
                }
                if (digitalRead(TRAI_PIN) == 1) {
                  if (data1[1] < 3000) {
                    home[1] = 0;
                    EEPROM.write(101, home[1]);
                    digitalWrite(ENB_Y, 0);
                    digitalWrite(DIR_Y, 1);
                    while (digitalRead(TRAI_PIN) == 1) {
                      runy(1000);
                      data1[1]++;
                    }
                  } else {
                    digitalWrite(ENB_Y, 1);
                    for (int i = 0; i < 3; i++) {
                      bao();
                    }
                  }
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  if (home[1] == 0) {
                    digitalWrite(ENB_Y, 0);
                    digitalWrite(DIR_Y, 0);
                    while (digitalRead(PHAI_PIN) == 0 & digitalRead(KHOP_Y) == 1) {
                      runy(1000);
                      data1[1]--;
                    }
                  } else {
                    home[1] = 1;
                    EEPROM.write(101, home[1]);
                    digitalWrite(ENB_Y, 1);
                    data1[1] = 0;
                    for (int i = 0; i < 3; i++) {
                      bao();
                    }
                  }
                }
                break;
              case 2:
                if (unsigned(millis()) - time > 500) {
                  lcd.setCursor(16, 2);
                  lcd.print("    ");
                  time = millis();
                } else {
                  lcd.setCursor(16, 2);
                  lcd.print(data1[2]);
                }
                if (digitalRead(TRAI_PIN) == 1) {
                  digitalWrite(ENB_Z, 0);
                  digitalWrite(DIR_Z, 0);
                  while (digitalRead(TRAI_PIN) == 1) {
                    runz(1000);
                    data1[2]++;
                  }
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  //Serial.println("Z Len");
                  if (digitalRead(KHOP_Z) == 1) {
                    digitalWrite(ENB_Z, 0);
                    digitalWrite(DIR_Z, 1);
                    while (digitalRead(PHAI_PIN) == 0 & digitalRead(KHOP_Z) == 1) {
                      runz(1000);
                      data1[2]--;
                    }
                  } else {
                    digitalWrite(ENB_Z, 0);
                    data1[2] = 0;
                    for (int i = 0; i < 3; i++) {
                      bao();
                    }
                  }
                }
                break;
              case 3:
                if (unsigned(millis()) - time > 500) {
                  lcd.setCursor(5, 3);
                  lcd.print("  ");
                  time = millis();
                } else {
                  lcd.setCursor(5, 3);
                  lcd.print(data1[4]);
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  delay(20);
                  if (digitalRead(PHAI_PIN) == 0) {
                    data1[4]++;
                    if (data1[4] > 15) data1[4] = 15;
                  }
                }
                if (digitalRead(TRAI_PIN) == 1) {
                  delay(20);
                  if (digitalRead(TRAI_PIN) == 1) {
                    data1[4]--;
                    if (data1[4] < 1) data1[4] = 1;
                  }
                }
                break;
              case 4:
                if (unsigned(millis()) - time > 500) {
                  lcd.setCursor(9, 3);
                  lcd.print("   ");
                  time = millis();
                } else {
                  lcd.setCursor(9, 3);
                  lcd.print(data1[3]);
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  delay(20);
                  if (digitalRead(PHAI_PIN) == 0) {
                    data1[3]++;
                    if (data1[3] > 15) data1[3] = 15;
                  }
                }
                if (digitalRead(TRAI_PIN) == 1) {
                  delay(20);
                  if (digitalRead(TRAI_PIN) == 1) {
                    data1[3]--;
                    if (data1[3] < 1) data1[3] = 1;
                  }
                }
                break;
              case 5:
                if (unsigned(millis()) - time > 500) {
                  lcd.setCursor(16, 3);
                  lcd.print("  ");
                  time = millis();
                } else {
                  lcd.setCursor(16, 3);
                  lcd.print(data1[5]);
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  delay(20);
                  if (digitalRead(PHAI_PIN) == 0) {
                    data1[5] = data1[5] + 5;
                    if (data1[5] > 50) data1[5] = 50;
                  }
                }
                if (digitalRead(TRAI_PIN) == 1) {
                  delay(20);
                  if (digitalRead(TRAI_PIN) == 1) {
                    data1[5] = data1[5] - 5;
                    if (data1[5] < 20) data1[5] = 20;
                  }
                }
                break;
            }
            if (digitalRead(SET_PIN) == 0) {
              delay(1000);
              if (digitalRead(SET_PIN) == 0) {
                SET = 3;
                break;
              }
            }
          }
          while (SET == 3) {
            lcd.clear();
            type = EEPROM.read(70);
            for (int i = 0; i < 6; i++) {
              if (i < 3) {
                EEPROM.write(i + (type * 6), data1[i] / 3);
                delay(5);
              } else {
                EEPROM.write(i + (type * 6), data1[i]);
                delay(5);
              }
            }
            for (int i = 0; i < 2; i++) bao();
            Reset(data1[0], data1[1]);
            break;
          }
        }
        if (digitalRead(PHAI_PIN) == 0) {
          delay(20);
          if (digitalRead(PHAI_PIN) == 0) {
            type++;
            if (type > 10) type = 1;
            lcd.clear();
            EEPROM.write(70, type);
          }
        }
        if (digitalRead(TRAI_PIN) == 1) {
          delay(20);
          if (digitalRead(TRAI_PIN) == 1) {
            type--;
            if (type < 1) type = 10;
            lcd.clear();
            EEPROM.write(70, type);
          }
        }
        if (digitalRead(S2_PIN) == 0) {
        } else {
          //Serial.println("Z Axis");
        }
      }
    }                                   // AUTO
    while (digitalRead(S1_PIN) == 1) {  //MAN
      lcd.clear();                      //May da san sang hoat dong che do MAN
      ready = 0;
      data1[6] = 0;
      ready = 1;
      while (ready == 1 && digitalRead(S1_PIN) == 1) {
        if (digitalRead(TRAI_PIN) == 1) {
          home[0] = 0;
          EEPROM.write(100, home[0]);
          if (data1[6] < 5000) {
            digitalWrite(ENB_X, 0);
            digitalWrite(DIR_X, 0);
            while (digitalRead(TRAI_PIN) == 1) {
              runx(600);
              data1[6]++;
            }
          } else {
            for (int i = 0; i < 3; i++) bao();
          }
        }
        if (digitalRead(PHAI_PIN) == 0) {
          if (home[0] == 0) {
            digitalWrite(ENB_X, 0);
            digitalWrite(DIR_X, 1);

            while (digitalRead(PHAI_PIN) == 0) {
              if (data1[6] <= 0) {
                home[0] = 1;
                EEPROM.write(100, home[0]);
                break;
              }
              runx(600);
              data1[0]--;
            }
          } else {
            data1[6] = 0;
            for (int i = 0; i < 3; i++) bao();
          }
        }
        ManProgram();
      }
    }
  }
}