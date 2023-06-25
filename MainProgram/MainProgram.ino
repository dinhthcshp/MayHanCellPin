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
unsigned int type = 1;
unsigned int data[10][6];
unsigned int data1[6];

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
  data[0][0] = 200;
  data[0][1] = 200;
  data[0][2] = 400;
  data[0][3] = 2;
  data[0][4] = 4;
  data[0][5] = 20;
  data1[3] = data1[4] = 1;
  EEPROM.write(0, 100);
  delay(5);
  EEPROM.write(1, 100);
  delay(5);
  EEPROM.write(2, 133);
  delay(5);
  EEPROM.write(3, 2);
  delay(5);
  EEPROM.write(4, 4);
  delay(5);
  EEPROM.write(5, 20);

  lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình
  lcd.backlight();  //Bật đèn nền
}
void ManProgram();
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
void AutoProgram(int distanceX, int distanceY, int distanceZ, int cellY, int cellX) {
  digitalWrite(ENB_X, 1);
  digitalWrite(ENB_Y, 1);
  for (int cot = 0; cot < cellX; cot++) {
    for (int hang = 0; hang < cellY; hang++) {
      digitalWrite(DIR_Z, 0);
      for (int xungz = 0; xungz < distanceZ; xungz++) {
        runz(800);
      }
      digitalWrite(DIR_Z, 1);
      for (int xungz = 0; xungz < distanceZ; xungz++) {
        runz(800);
      }
      if (hang < cellY - 1) {
        if (cot % 2 == 0) {
          digitalWrite(ENB_Y, 0);
          digitalWrite(DIR_Y, 1);
          for (int xungy = 0; xungy < distanceY; xungy++) {
            runy(1000);
          }
          digitalWrite(ENB_Y, 1);
        } else {
          digitalWrite(ENB_Y, 0);
          digitalWrite(DIR_Y, 0);
          for (int xungy = 0; xungy < distanceY; xungy++) {
            runy(1000);
          }
          digitalWrite(ENB_Y, 1);
        }
      }
    }
    if (cot == cellX - 1) {
      Reset();
      if (cot % 2 == 0) {
        digitalWrite(ENB_Y, 0);
        digitalWrite(DIR_Y, 0);
        for (int i = 0; i < distanceY; i++) {
          runy(1000);
        }
      }
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
}
void ManProgram() {
  int qt = 0;
  digitalWrite(ENB_Z, 0);
  if (digitalRead(START_PIN) == 0) {
    qt = 1;
  }
  Serial.print("QT = ");
  Serial.println(qt);
  while (qt == 1 && digitalRead(S1_PIN) == 1) {
    if (digitalRead(START_PIN) == 0) {
      digitalWrite(DIR_Z, 0);
      while (digitalRead(CTHT_HAN_PIN) == 1 & digitalRead(START_PIN) == 0) runz(1000);
    }
    if (digitalRead(CTHT_HAN_PIN) == 0) qt = 2;
    Serial.print("QT = ");
    Serial.println(qt);
  }
  while (qt == 2) {
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
  while (qt == 3) {
    Serial.print("QT = ");
    Serial.println(qt);
    digitalWrite(DIR_Z, 1);
    while (digitalRead(KHOP_Z) == 1) runz(1000);
    if (digitalRead(KHOP_Z == 0)) qt = 0;
  }
}
void Reset() {
  ready = 0;
  RS = 0;
  bool tmp = 0;
  while (RS == 0) {
    digitalWrite(ENB_Z, 0);
    digitalWrite(DIR_Z, 1);
    while (digitalRead(KHOP_Z) == 1) {
      runz(1000);
    }
    if (digitalRead(KHOP_Z) == 0) RS++;
  }
  while (RS == 1) {
    digitalWrite(ENB_X, 0);
    digitalWrite(DIR_X, 1);
    while (digitalRead(KHOP_X) == 1) {
      runx(600);
    }
    if (digitalRead(KHOP_X) == 0) RS++;
    for (int i = 0; i < 3; i++) {
      bao();
    }
    ready = 1;
  }
  // while (RS == 2) {
  //   digitalWrite(ENB_Y, 0);
  //   digitalWrite(DIR_Y, 1);
  //   while (digitalRead(KHOP_Y) == 1 && tmp == 0) {
  //     runy(1000);
  //     if (digitalRead(KHOP_Y) == 0) tmp = 1;
  //   }
  //   while (tmp == 1) {
  //     digitalWrite(ENB_Y, 0);
  //     digitalWrite(DIR_Y, 0);
  //     for (int i = 0; i < 400; i++) {
  //       runy(1000);
  //     }
  //     RS++;
  //     tmp = 2;
  //     break;
  //   }
  //   for (int i = 0; i < 3; i++) {
  //     bao();
  //   }
  //   ready = 1;
  // }
}
void Show(int loai, int distanceX, int distanceY, int distanceZ, int cellY, int cellX, int tg) {
  lcd.setCursor(2, 0);
  lcd.print("Pin loai");
  lcd.setCursor(11, 0);
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
      Reset();
      while (ready == 1 && digitalRead(S1_PIN) == 0) {
        //Show(type, EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3), EEPROM.read(4), EEPROM.read(5));
        Show(type, EEPROM.read((type - 1) * 6 + 0) * 3, EEPROM.read((type - 1) * 6 + 1) * 3, EEPROM.read((type - 1) * 6 + 2) * 3, EEPROM.read((type - 1) * 6 + 3), EEPROM.read((type - 1) * 6 + 4), EEPROM.read((type - 1) * 6 + 5));
        if (digitalRead(START_PIN) == 0) {
          digitalWrite(DEN_DO_PIN, 1);
          digitalWrite(DEN_XANH_PIN, 0);
          AutoProgram(EEPROM.read((type - 1) * 6 + 0) * 3, EEPROM.read((type - 1) * 6 + 1) * 3, EEPROM.read((type - 1) * 6 + 2) * 3, EEPROM.read((type - 1) * 6 + 3), EEPROM.read((type - 1) * 6 + 4));
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
            if (digitalRead(START_PIN) == 0) {
              delay(20);
              if (digitalRead(START_PIN) == 0) {
                lcd.clear();
                type++;
                if (type > 10) type = 1;
              }
            }
            ShowSelect(type, EEPROM.read(type * 6 + 0), EEPROM.read(type * 6 + 1), EEPROM.read(type * 6 + 2), EEPROM.read(type * 6 + 3), EEPROM.read(type * 6 + 4), EEPROM.read(type * 6 + 5));
            if (digitalRead(SET_PIN) == 0) {
              for (int i = 0; i < 7; i++) {
                data1[i] = EEPROM.read(type * 6 + i);
              }
              SET = 2;
              break;
            }
          }
          while (SET == 2) {
            ShowSelect(type, data1[0], data1[1], data1[2], data1[3], data1[4], data1[5]);
            SELECT = int((float(analogRead(TG_PIN)) / 1023.0) * 5.0);

            switch (SELECT) {
              case 0:
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(4, 2);
                  lcd.print("    ");
                  time = millis();
                }
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(4, 2);
                  lcd.print(data1[0]);
                  time = millis();
                }
                digitalWrite(ENB_X, 1);
                if (digitalRead(TRAI_PIN) == 1) {
                  digitalWrite(ENB_X, 0);
                  digitalWrite(DIR_X, 0);
                  while (digitalRead(TRAI_PIN) == 1) {
                    runx(1200);
                    data1[0]++;
                  }
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  if (digitalRead(KHOP_X) == 1) {
                    digitalWrite(ENB_X, 0);
                    digitalWrite(DIR_X, 1);
                    while (digitalRead(PHAI_PIN) == 0 & digitalRead(KHOP_X) == 1) {
                      runx(1200);
                      data1[0]--;
                    }
                  } else {
                    data1[0] = 0;
                    for (int i = 0; i < 3; i++) {
                      bao();
                    }
                  }
                }
                break;
              case 1:
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(10, 2);
                  lcd.print("    ");
                  time = millis();
                }
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(10, 2);
                  lcd.print(data1[1]);
                  time = millis();
                }
                if (digitalRead(TRAI_PIN) == 1) {
                  digitalWrite(ENB_Y, 0);
                  digitalWrite(DIR_Y, 0);
                  while (digitalRead(TRAI_PIN) == 1) {
                    runy(1000);
                    data1[1]++;
                  }
                }
                if (digitalRead(PHAI_PIN) == 0) {
                  if (digitalRead(KHOP_Y) == 1) {
                    digitalWrite(ENB_Y, 0);
                    digitalWrite(DIR_Y, 1);
                    while (digitalRead(PHAI_PIN) == 0 & digitalRead(KHOP_Y) == 1) {
                      runy(1000);
                      data1[1]--;
                    }
                  } else {
                    digitalWrite(ENB_Y, 1);
                    data1[1] = 0;
                    for (int i = 0; i < 3; i++) {
                      bao();
                    }
                  }
                }
                break;
              case 2:
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(16, 2);
                  lcd.print("    ");
                  time = millis();
                }
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(16, 2);
                  lcd.print(data1[2]);
                  time = millis();
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
                  Serial.println("Z Len");
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
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(5, 3);
                  lcd.print("  ");
                  time = millis();
                }
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(5, 3);
                  lcd.print(data1[4]);
                  time = millis();
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
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(9, 3);
                  lcd.print("   ");
                  time = millis();
                }
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(9, 3);
                  lcd.print(data1[3]);
                  time = millis();
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
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(16, 3);
                  lcd.print("  ");
                  time = millis();
                }
                if (unsigned(millis()) - time > 1000) {
                  lcd.setCursor(16, 3);
                  lcd.print(data1[5]);
                  time = millis();
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
                lcd.clear();
                for (int i = 0; i < 6; i++) {
                  data[type - 1][i] = data1[i];
                  if (i < 3) {
                    EEPROM.write(i + ((type - 1) * 6), data1[i] / 3);
                    delay(5);
                  } else {
                    EEPROM.write(i + ((type - 1) * 6), data1[i]);
                    delay(5);
                  }
                }
                for (int i = 0; i < 2; i++) bao();
                break;
              }
            }
          }
          //EEPROM.write(0, doc[0]);
          //Serial.println(EEPROM.read(0));
        }
        if (digitalRead(PHAI_PIN) == 0) {
          type++;
          if (type > 10) type = 1;
          lcd.clear();
        }
        if (digitalRead(TRAI_PIN) == 1) {
          type--;
          if (type < 1) type = 10;
          lcd.clear();
        }
        if (digitalRead(S2_PIN) == 0) {
          //Serial.println("X Axis");
        } else {
          Serial.println("Z Axis");
        }
      }
    }                                   // AUTO
    while (digitalRead(S1_PIN) == 1) {  //MAN
      Serial.println("MAN");
      ready = 0;
      Reset();
      while (ready == 1 && digitalRead(S1_PIN) == 1) {  //May da san sang hoat dong che do MAN
        ManProgram();
      }
    }
    Serial.print("\nEND\n");
  }
}