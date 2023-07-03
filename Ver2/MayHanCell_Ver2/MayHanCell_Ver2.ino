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
byte contro = 0;
int Pin[4] = { 157, 170, 620, 450 };

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

  digitalWrite(ENB_X, 1);  // Tat dong co X
  digitalWrite(ENB_Y, 1);  // Tat dong co Y
  digitalWrite(ENB_Z, 0);  // Bat dong co Z
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
void Bao(int solan, int thoigian) {
  for (int i = 0; i < solan; i++) {
    digitalWrite(COI_PIN, 1);
    delay(thoigian - 20);
    digitalWrite(COI_PIN, 0);
    delay(thoigian);
  }
}
void X_Trai(unsigned int xung, unsigned int tocdo) {
  digitalWrite(ENB_X, 0);
  digitalWrite(DIR_X, 0);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_X, 0);
    delayMicroseconds(tocdo);
    digitalWrite(STEP_X, 1);
    delayMicroseconds(tocdo);
  }
  digitalWrite(ENB_X, 1);
}
void X_Phai(unsigned int xung, unsigned int tocdo) {
  digitalWrite(ENB_X, 0);
  digitalWrite(DIR_X, 1);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_X, 0);
    delayMicroseconds(tocdo);
    digitalWrite(STEP_X, 1);
    delayMicroseconds(tocdo);
  }
  digitalWrite(ENB_X, 1);
}
void Y_Vao(unsigned int xung, unsigned int tocdo) {
  digitalWrite(ENB_Y, 0);
  digitalWrite(DIR_Y, 1);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_Y, 0);
    delayMicroseconds(tocdo);
    digitalWrite(STEP_Y, 1);
    delayMicroseconds(tocdo);
  }
  digitalWrite(ENB_Y, 1);
}
void Y_Ra(unsigned int xung, unsigned int tocdo) {
  digitalWrite(ENB_Y, 0);
  digitalWrite(DIR_Y, 0);
  for (int i = 0; i < xung; i++) {
    digitalWrite(STEP_Y, 0);
    delayMicroseconds(tocdo);
    digitalWrite(STEP_Y, 1);
    delayMicroseconds(tocdo);
  }
  digitalWrite(ENB_Y, 1);
}
void AutoProgram(int p0X, int p0Y, int distanceX, int distanceY, int CellX, int CellY) {
  unsigned int buoc = 0;
  while (buoc == 0) {
    X_Trai(p0X, 600);
    Y_Vao(p0Y, 600);
    buoc++;
  }
  while (buoc == 1) {
    EEPROM.write(100, 0);
    EEPROM.write(101, 0);
    for (int i = 0; i < CellX; i++) {
      for (int j = 0; j < CellY; j++) {
        Z(0);
        if (j < CellY - 1) {
          if (i % 2 == 0) {
            Y_Vao(distanceY, 600);
          } else Y_Ra(distanceY, 600);
        }
      }
      if (i < CellX - 1) {
        X_Trai(distanceX, 600);
      }
    }
    buoc++;
  }
  while (buoc == 2) {
    X_Phai(distanceX * (CellX - 1) + p0X, 600);
    if (CellX % 2 == 1) {
      Y_Ra(distanceY * CellY - 1 + p0Y, 600);
    } else Y_Ra(p0Y, 600);
    EEPROM.write(100, 1);
    EEPROM.write(101, 1);
    Bao(3, 80);
    buoc++;
  }
}
void Z(int tg) {
  byte z = 1;
  digitalWrite(DIR_Z, 0);
  while (z == 1) {
    digitalWrite(STEP_Z, 0);
    delayMicroseconds(800);
    digitalWrite(STEP_Z, 1);
    delayMicroseconds(800);
    if (digitalRead(CTHT_HAN_PIN) == 0) z = 2;
  }
  while (z == 2) {
    digitalWrite(HAN_PIN, 1);
    delay(tg);
    digitalWrite(HAN_PIN, 0);
    z = 3;
  }
  while (z == 3) {
    digitalWrite(DIR_Z, 1);
    digitalWrite(STEP_Z, 0);
    delayMicroseconds(800);
    digitalWrite(STEP_Z, 1);
    delayMicroseconds(800);
    if (digitalRead(KHOP_Z) == 0) z = 4;
  }
}
void Home() {
  int buoc = 0;
  while (buoc == 0) {
    digitalWrite(ENB_Z, 0);
    digitalWrite(DIR_Z, 1);
    while (digitalRead(KHOP_Z) == 1) {
      digitalWrite(STEP_Z, 0);
      delayMicroseconds(800);
      digitalWrite(STEP_Z, 1);
      delayMicroseconds(800);
    }
    EEPROM.write(102, 1);
    buoc++;
  }
  while (buoc == 1) {
    if (EEPROM.read(100) == 0) {
      byte ResetX = 0;
      while (ResetX == 0) {
        while (digitalRead(KHOP_X) == 1) {
          X_Phai(1, 600);
        }
        if (digitalRead(KHOP_X) == 0) {
          ResetX++;
        }
      }
      while (ResetX == 1) {
        X_Trai(80, 600);
        EEPROM.write(100, 1);
        ResetX++;
        buoc++;
      }
    } else buoc++;
  }
  while (buoc == 2) {
    if (EEPROM.read(101) == 0) {
      byte ResetY = 0;
      while (ResetY == 0) {
        while (digitalRead(KHOP_Y) == 1) {
          Y_Vao(1, 600);
        }
        if (digitalRead(KHOP_Y) == 0) ResetY++;
      }
      while (ResetY == 1) {
        Y_Ra(1650, 600);
        EEPROM.write(101, 1);
        ResetY++;
        buoc++;
      }
    } else buoc++;
  }
  while (buoc == 3) {
    //Bao(3, 80);
    buoc++;
  }
}
void HienThiAuto(int loaipin, int cellX, int cellY, int k, int t) {
  lcd.setCursor(2, 0);
  lcd.print("MAY HAN CELL PIN");
  lcd.setCursor(1, 1);
  lcd.print("Che do: ");
  lcd.setCursor(9, 1);
  lcd.printstr("AUTO");
  lcd.setCursor(1, 2);
  lcd.print("Loai pin: ");
  lcd.setCursor(11, 2);
  lcd.print(loaipin);

  lcd.setCursor(1, 3);
  lcd.print("So cell:");
  lcd.setCursor(9, 3);
  lcd.print(cellX);
  lcd.setCursor(10, 3);
  lcd.print("x");
  lcd.setCursor(11, 3);
  lcd.print(cellY);

  lcd.setCursor(14, 2);
  lcd.print("K:");
  lcd.setCursor(16, 2);
  lcd.print(k);

  lcd.setCursor(14, 3);
  lcd.print("T:");
  lcd.setCursor(16, 3);
  lcd.print(t);
  lcd.setCursor(18, 3);
  lcd.print("ms");
}
void HienThiCaiDat() {
  lcd.setCursor(4, 0);
  lcd.print("CAI DAT");

  lcd.setCursor(1, 1);
  lcd.print("Che do");

  lcd.setCursor(1, 2);
  lcd.print("Vi tri");

  lcd.setCursor(1, 3);
  lcd.print("So cell,T,K");
}
void HienThiMan() {
  lcd.setCursor(2, 0);
  lcd.print("MAY HAN CELL PIN");
  lcd.setCursor(1, 1);
  lcd.print("Che do: ");
  lcd.setCursor(9, 1);
  lcd.printstr("MAN");
}
void HienThiMenuVitri() {
  lcd.setCursor(3, 0);
  lcd.print("Cai dat vi tri");
  lcd.setCursor(1, 1);
  lcd.print("P1-");
  lcd.setCursor(4, 1);
  lcd.printstr("X:  ");
  lcd.setCursor(10, 1);
  lcd.printstr("Y:  ");

  lcd.setCursor(1, 2);
  lcd.print("P2-");
  lcd.setCursor(4, 2);
  lcd.printstr("X:  ");
  lcd.setCursor(10, 2);
  lcd.printstr("Y:  ");

  lcd.setCursor(1, 3);
  lcd.print("P3-");
  lcd.setCursor(4, 3);
  lcd.printstr("X:  ");
  lcd.setCursor(10, 3);
  lcd.printstr("Y:  ");
}
void loop() {
  while (1) {
    while (digitalRead(S1_PIN) == 0) {
      byte Ready = 0;
      int trucx = 0, trucy = 0;
      Home();
      Ready = 1;
      lcd.clear();
      while (Ready == 1 && digitalRead(S1_PIN) == 0) {
        if (EEPROM.read(50) == 0) {  //AUTO
          HienThiAuto(EEPROM.read(40) + 1, EEPROM.read(41), EEPROM.read(42), EEPROM.read(43), EEPROM.read(44));
          if (digitalRead(START_PIN) == 0) {
            delay(20);
            if (digitalRead(START_PIN) == 0) {
              AutoProgram(EEPROM.read(EEPROM.read(20) * 2 + 0), EEPROM.read(EEPROM.read(20) * 2 + 1), EEPROM.read(EEPROM.read(20) * 2 + 2), EEPROM.read(EEPROM.read(20) * 2 + 3), EEPROM.read(41), EEPROM.read(42));
            }
          }
        } else {
          HienThiMan();
          if (digitalRead(START_PIN) == 0) {
            delay(20);
            if (digitalRead(START_PIN) == 0) {
              Z(0);
            }
          }
        }
      }
    }
    while (digitalRead(S1_PIN) == 1) {
      byte menu = 0;
      lcd.clear();
      HienThiCaiDat();
      menu = 1;
      while (menu == 1 && digitalRead(S1_PIN) == 1) {
        if (digitalRead(TRAI_PIN) == 1) {
          delay(100);
          if (digitalRead(TRAI_PIN) == 1) {
            if (contro > 0) contro--;
            else contro = 2;
          }
        }
        if (digitalRead(PHAI_PIN) == 0) {
          delay(100);
          if (digitalRead(PHAI_PIN) == 0) {
            if (contro < 3) contro++;
            else contro = 0;
          }
        }
        switch (contro) {
          case 0:
            lcd.setCursor(0, 1);
            lcd.print(">");
            lcd.setCursor(0, 2);
            lcd.print(" ");
            lcd.setCursor(0, 3);
            lcd.print(" ");
            if (digitalRead(SET_PIN) == 0) {
              delay(100);
              if (digitalRead(SET_PIN) == 0) {
                menu = 2;
              }
            }
            break;
          case 1:
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.setCursor(0, 2);
            lcd.print(">");
            lcd.setCursor(0, 3);
            lcd.print(" ");
            if (digitalRead(SET_PIN) == 0) {
              delay(100);
              if (digitalRead(SET_PIN) == 0) {
                menu = 3;
              }
            }
            break;
          case 2:
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.setCursor(0, 2);
            lcd.print(" ");
            lcd.setCursor(0, 3);
            lcd.print(">");
            if (digitalRead(SET_PIN) == 0) {
              delay(100);
              if (digitalRead(SET_PIN) == 0) {
                menu = 4;
              }
            }
            break;
        }
      }
      while (menu == 2) {
        byte buoc = 0;
        contro = EEPROM.read(50);
        buoc++;
        while (buoc == 1) {
          lcd.setCursor(2, 0);
          lcd.print("Che do hoat dong");
          lcd.setCursor(1, 1);
          lcd.print("AUTO       ");
          lcd.setCursor(1, 2);
          lcd.print("MAN        ");
          lcd.setCursor(1, 3);
          lcd.print("                 ");
          if (digitalRead(TRAI_PIN) == 1) {
            delay(100);
            if (digitalRead(TRAI_PIN) == 1) {
              if (contro > 0) contro--;
              else contro = 1;
            }
          }
          if (digitalRead(PHAI_PIN) == 0) {
            delay(100);
            if (digitalRead(PHAI_PIN) == 0) {
              if (contro < 2) contro++;
              else contro = 0;
            }
          }
          if (contro == 0) {
            lcd.setCursor(0, 1);
            lcd.print(">");
            lcd.setCursor(0, 2);
            lcd.print(" ");
          } else {
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.setCursor(0, 2);
            lcd.print(">");
          }
          if (digitalRead(SET_PIN) == 0) {
            delay(100);
            if (digitalRead(SET_PIN) == 0) {
              EEPROM.write(50, contro);
              menu = 1;
              break;
            }
          }
        }
      }
      while (menu == 3) {
        HienThiMenuVitri();
        if (digitalRead(TRAI_PIN) == 1) {
          delay(100);
          if (digitalRead(TRAI_PIN) == 1) {
            if (contro > 0) contro--;
            else contro = 2;
          }
        }
        if (digitalRead(PHAI_PIN) == 0) {
          delay(100);
          if (digitalRead(PHAI_PIN) == 0) {
            if (contro < 3) contro++;
            else contro = 0;
          }
        }
        switch (contro) {
          case 0:
            lcd.setCursor(0, 1);
            lcd.print(">");
            lcd.setCursor(0, 2);
            lcd.print(" ");
            lcd.setCursor(0, 3);
            lcd.print(" ");
            break;
          case 1:
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.setCursor(0, 2);
            lcd.print(">");
            lcd.setCursor(0, 3);
            lcd.print(" ");
            break;
          case 2:
            lcd.setCursor(0, 1);
            lcd.print(" ");
            lcd.setCursor(0, 2);
            lcd.print(" ");
            lcd.setCursor(0, 3);
            lcd.print(">");
            break;
        }
      }
      while (menu == 4) {
      }
    }
  }
}
