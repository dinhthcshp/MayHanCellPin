#include <EEPROM.h>
#include <stdio.h>
#include <ACS712.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
ACS712 sensor(ACS712_30A, A3);
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
int TrucX = 0, TrucY = 0;
int TrucX1 = 0, TrucY1 = 0;
int Pin[4] = { 157, 160, 620, 450 };
float I = 0.0;

void setup() {
  int Zero = sensor.calibrate();
  delay(200);
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
  EEPROM.write(0, 57);
  EEPROM.write(1, 44);
  EEPROM.write(3, 155);
  EEPROM.write(45, 50);
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
    delayMicroseconds(600);
    TrucX1++;
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
    delayMicroseconds(600);
    TrucX1--;
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
    delayMicroseconds(600);
    TrucY1++;
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
    delayMicroseconds(600);
    TrucY1--;
  }
  digitalWrite(ENB_Y, 1);
}
void AutoProgram(int p0X, int p0Y, int distanceX, int distanceY, int CellX, int CellY, int thoigian, int distanceK) {
  unsigned int buoc = 0;
  TrucX = TrucY = 0;
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
        if (EEPROM.read(43) == 0) {
          Z(thoigian);
          if (j < CellY - 1) {
            if (i % 2 == 0) {
              Y_Vao(distanceY, 600);
            } else Y_Ra(distanceY, 600);
          }
        }
        if (EEPROM.read(43) == 1) {
          if (i % 2 == 0) {
            Z(thoigian);
            Y_Vao(distanceK, 600);
            Z(thoigian);
          } else {
            Z(thoigian);
            Y_Ra(distanceK, 600);
            Z(thoigian);
          }

          if (j < CellY - 1) {
            if (i % 2 == 0) {
              Y_Vao(distanceY - distanceK, 600);
            } else Y_Ra(distanceY - distanceK, 600);
          }
        }
        if (EEPROM.read(43) == 2) {
          if (i % 2 == 0) {
            Z(thoigian);
            Y_Ra(distanceK, 600);
            Z(thoigian);
            Y_Vao(distanceK * 2, 600);
            Z(thoigian);
          } else {
            Z(thoigian);
            Y_Ra(distanceK, 600);
            Z(thoigian);
            Y_Ra(distanceK, 600);
            Z(thoigian);
          }
          if (j < CellY - 1) {
            if (i % 2 == 0) {
              Y_Vao(distanceY - distanceK, 600);
            } else Y_Ra(distanceY - distanceK, 600);
          }
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
    while (TrucY1 > 0) {
      Y_Ra(1, 600);
    }
    // if (CellX % 2 == 1) {
    //   Y_Ra(distanceY * (CellY - 1) + p0Y, 600);
    // } else Y_Ra(p0Y, 600);
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
    I = sensor.getCurrentAC();
    delay(tg);
    digitalWrite(HAN_PIN, 0);
    Serial.print("I = ");
    Serial.print(I);
    Serial.print("A");
    Serial.println("   ");
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
void Z_Xuong(int tg) {
  digitalWrite(DIR_Z, 0);
  if (digitalRead(CTHT_HAN_PIN) == 1) {
    digitalWrite(STEP_Z, 0);
    delayMicroseconds(tg);
    digitalWrite(STEP_Z, 1);
    delayMicroseconds(600);
  } else Bao(2, 80);
}
void Z_Len(int tg) {
  digitalWrite(DIR_Z, 1);
  if (digitalRead(KHOP_Z) == 1) {
    digitalWrite(STEP_Z, 0);
    delayMicroseconds(tg);
    digitalWrite(STEP_Z, 1);
    delayMicroseconds(600);
  } else Bao(2, 80);
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
    TrucX = TrucY = 0;
    //Bao(3, 80);
    buoc++;
  }
}
void HienThiAuto(int loaipin, int cellX, int cellY, int k, int t, int cs) {
  lcd.setCursor(2, 0);
  lcd.print("MAY HAN CELL PIN");
  lcd.setCursor(0, 1);
  lcd.print("Che do: ");
  lcd.setCursor(8, 1);
  lcd.printstr("AUTO");
  lcd.setCursor(0, 2);
  lcd.print("Loai pin:    ");
  lcd.setCursor(10, 2);
  lcd.print(loaipin);

  lcd.setCursor(0, 3);
  lcd.print("So cell:");
  lcd.setCursor(8, 3);
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

  lcd.setCursor(14, 1);
  lcd.print("W:   ");
  lcd.setCursor(16, 1);
  lcd.print(cs);
  lcd.setCursor(19, 1);
  lcd.print("%");
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
void HienThiMan(int tg, int cs) {
  lcd.setCursor(2, 0);
  lcd.print("MAY HAN CELL PIN");
  lcd.setCursor(1, 1);
  lcd.print("Che do: ");
  lcd.setCursor(9, 1);
  lcd.print("MAN");
  lcd.setCursor(1, 2);
  lcd.print("T:");
  lcd.setCursor(3, 2);
  lcd.print(tg);

  lcd.setCursor(1, 3);
  lcd.print("CS:");
  lcd.setCursor(5, 3);
  lcd.print("  ");
  lcd.setCursor(4, 3);
  lcd.print(cs);
}
void HienThiMenuVitri1(int loai) {
  lcd.setCursor(2, 0);
  lcd.print("Loai pin cai dat");
  lcd.setCursor(1, 1);
  lcd.print("Pin loai: ");
  lcd.setCursor(10, 1);
  lcd.print(loai);
}
void HienThiMenuVitri2(int x1, int y1, int x2, int y2, int x3, int y3, int loai) {
  lcd.setCursor(0, 0);
  lcd.print("Cai dat vi tri Pin");
  lcd.setCursor(19, 0);
  lcd.print(loai);

  lcd.setCursor(1, 1);
  lcd.print("P1-");
  lcd.setCursor(4, 1);
  lcd.printstr("X:     ");
  lcd.setCursor(6, 1);
  lcd.print(x1);
  lcd.setCursor(11, 1);
  lcd.printstr("Y:     ");
  lcd.setCursor(13, 1);
  lcd.print(y1);

  lcd.setCursor(1, 2);
  lcd.print("P2-");
  lcd.setCursor(4, 2);
  lcd.printstr("X:     ");
  lcd.setCursor(6, 2);
  lcd.print(x2);
  lcd.setCursor(11, 2);
  lcd.printstr("Y:     ");
  lcd.setCursor(13, 2);
  lcd.print(y2);

  lcd.setCursor(1, 3);
  lcd.print("P3-");
  lcd.setCursor(4, 3);
  lcd.printstr("X:     ");
  lcd.setCursor(6, 3);
  lcd.print(x3);
  lcd.setCursor(11, 3);
  lcd.printstr("Y:     ");
  lcd.setCursor(13, 3);
  lcd.print(y3);

  lcd.setCursor(18, 3);
  lcd.print("Z");
}
void HienThiMenuCell(int CellX, int CellY, int tg, int k) {
  lcd.setCursor(2, 0);
  lcd.print("Cai dat Cell,T,K");

  lcd.setCursor(1, 1);
  lcd.print("Cell:  ");
  lcd.setCursor(6, 1);
  lcd.print(CellX);
  lcd.setCursor(8, 1);
  lcd.print("x    ");
  lcd.setCursor(9, 1);
  lcd.print(CellY);

  lcd.setCursor(3, 2);
  lcd.print("X:  ");
  lcd.setCursor(5, 2);
  lcd.print(CellX);
  lcd.setCursor(7, 2);
  lcd.print("Y:  ");
  lcd.setCursor(9, 2);
  lcd.print(CellY);

  lcd.setCursor(1, 3);
  lcd.print("T:  ");
  lcd.setCursor(3, 3);
  lcd.print(tg);
  lcd.setCursor(5, 3);
  lcd.print("ms");

  lcd.setCursor(10, 3);
  lcd.print("K:");
  lcd.setCursor(12, 3);
  lcd.print(k);
}
void DKConTro(byte giatri) {
  if (digitalRead(START_PIN) == 0) {
    delay(100);
    if (digitalRead(START_PIN) == 0) {
      if (contro < giatri) contro++;
      else contro = 0;
    }
  }
}
void loop() {
  while (1) {
    Home();
    while (digitalRead(S1_PIN) == 0) {
      byte Ready = 0;
      byte Type = EEPROM.read(40);
      Ready = 1;
      lcd.clear();
      while (Ready == 1 && digitalRead(S1_PIN) == 0) {
        if (EEPROM.read(50) == 0) {  //AUTO
          int cs = int((float(analogRead(CS_PIN)) / 102.3)) * 10;
          HienThiAuto(EEPROM.read(40) + 1, EEPROM.read(41), EEPROM.read(42), EEPROM.read(43) + 1, EEPROM.read(44), cs);
          if (digitalRead(TRAI_PIN) == 1) {
            delay(100);
            if (digitalRead(TRAI_PIN) == 1) {
              if (Type > 0) {
                Type--;
              } else Type = 0;
              EEPROM.write(40, Type);
            }
          }
          if (digitalRead(PHAI_PIN) == 0) {
            delay(100);
            if (digitalRead(PHAI_PIN) == 0) {
              if (Type < 9) {
                Type++;
              } else Type = 9;
              EEPROM.write(40, Type);
            }
          }
          if (digitalRead(RESET_PIN) == 0) {
            delay(20);
            if (digitalRead(RESET_PIN) == 0) {
              EEPROM.write(102, 0);
              EEPROM.write(100, 0);
              EEPROM.write(101, 0);
              Home();
            }
          }
          if (digitalRead(START_PIN) == 0) {
            delay(20);
            if (digitalRead(START_PIN) == 0) {
              Serial.println(EEPROM.read(0));
              Serial.println(EEPROM.read(1));
              Serial.println(EEPROM.read(2));
              Serial.println(EEPROM.read(3));
              AutoProgram(int(EEPROM.read(EEPROM.read(40) * 4 + 0) * 3.0), int(EEPROM.read(EEPROM.read(40) * 4 + 1) * 3.0), int(EEPROM.read(EEPROM.read(40) * 4 + 2) * 3.0), int(EEPROM.read(EEPROM.read(40) * 4 + 3) * 3.0), EEPROM.read(41), EEPROM.read(42), EEPROM.read(44), EEPROM.read(45));
            }
          }
        } else {  //Man
          int tg = int(float(analogRead(TG_PIN)) / 1023.0 * 40.0 + 10);
          int cs = int((float(analogRead(CS_PIN)) / 102.3)) * 10;
          HienThiMan(tg, cs);
          int buocman = 0;
          digitalWrite(DIR_Z, 0);
          if (digitalRead(START_PIN) == 0 && buocman == 0) {
            delay(20);
            while (digitalRead(START_PIN) == 0 && buocman == 0) {
              if (digitalRead(CTHT_HAN_PIN) == 1) {
                digitalWrite(STEP_Z, 0);
                delayMicroseconds(800);
                digitalWrite(STEP_Z, 1);
                delayMicroseconds(800);
              } else buocman = 1;
            }
          }
          while (buocman == 1) {
            if (digitalRead(SET_PIN) == 0) {
              delay(20);
              if (digitalRead(SET_PIN) == 0) {
                digitalWrite(HAN_PIN, 1);
                delay(tg);
                digitalWrite(HAN_PIN, 0);
                buocman = 2;
              }
            }
          }
          while (buocman == 2) {
            digitalWrite(DIR_Z, 1);
            while (digitalRead(KHOP_Z) == 1) {
              digitalWrite(STEP_Z, 0);
              delayMicroseconds(800);
              digitalWrite(STEP_Z, 1);
              delayMicroseconds(800);
            }
            if (digitalRead(KHOP_Z) == 0) buocman = 0;
          }
        }
      }
    }
    while (digitalRead(S1_PIN) == 1) {
      byte menu = 0;
      int x1, x2, x3, y1, y2, y3;
      x1 = x2 = x3 = y1 = y2 = y3 = 0;
      int CellX = 0, CellY = 0, tg = 0, k = 0;
      CellX = EEPROM.read(41);
      CellY = EEPROM.read(42);
      k = EEPROM.read(43);
      tg = EEPROM.read(44);
      lcd.clear();
      HienThiCaiDat();
      menu = 1;
      while (menu == 1 && digitalRead(S1_PIN) == 1) {
        DKConTro(3);
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
                contro = 0;
                menu = 2;
                lcd.clear();
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
                contro = 0;
                menu = 3;
                lcd.clear();
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
                contro = 0;
                menu = 4;
                lcd.clear();
              }
            }
            break;
        }
      }
      while (menu == 2) {
        byte buoc = 0;
        contro = EEPROM.read(50);
        buoc++;
        while (buoc == 1) {  //Che do hoat dong
          lcd.setCursor(2, 0);
          lcd.print("Che do hoat dong");
          lcd.setCursor(1, 1);
          lcd.print("AUTO       ");
          lcd.setCursor(1, 2);
          lcd.print("MAN        ");
          lcd.setCursor(1, 3);
          lcd.print("                 ");
          DKConTro(2);
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
      while (menu == 3) {  //Vi tri
        byte SetViTri = 0;
        DKConTro(10);
        HienThiMenuVitri1(contro + 1);
        if (digitalRead(SET_PIN) == 0) {
          delay(100);
          if (digitalRead(SET_PIN) == 0) {
            if (contro > 1) {
              EEPROM.write(40, contro);
              SetViTri = 1;
            } else Bao(3, 80);
          }
        }
        while (SetViTri == 1) {
          HienThiMenuVitri2(x1, y1, x2, y2, x3, y3, EEPROM.read(40) + 1);
          DKConTro(4);
          if (digitalRead(TRAI_PIN) == 1 && digitalRead(S2_PIN) == 0 && contro != 3) {
            if (TrucX < 3000) {
              EEPROM.write(100, 0);
              while (digitalRead(TRAI_PIN) == 1) {
                X_Trai(1, 2000);
                TrucX++;
              }
            } else Bao(2, 80);
          }
          if (digitalRead(PHAI_PIN) == 0 && digitalRead(S2_PIN) == 0 && contro != 3) {
            if (TrucX > 0) {
              while (digitalRead(PHAI_PIN) == 0) {
                X_Phai(1, 2000);
                TrucX--;
              }
            } else Bao(2, 80);
          }

          if (digitalRead(TRAI_PIN) == 1 && digitalRead(S2_PIN) == 1 && contro != 3) {
            if (TrucY < 2300) {
              EEPROM.write(101, 0);
              while (digitalRead(TRAI_PIN) == 1) {
                Y_Vao(1, 2000);
                TrucY++;
              }
            } else Bao(2, 80);
          }
          if (digitalRead(PHAI_PIN) == 0 && digitalRead(S2_PIN) == 1 && contro != 3) {
            if (TrucY > 0) {
              while (digitalRead(PHAI_PIN) == 0) {
                Y_Ra(1, 2000);
                TrucY--;
              }
            } else Bao(2, 80);
          }

          while (digitalRead(TRAI_PIN) == 1 && contro == 3) {
            if (EEPROM.read(102) == 1) {
              EEPROM.write(102, 0);
            }
            Z_Xuong(2000);
          }
          while (digitalRead(PHAI_PIN) == 0 && contro == 3) {
            Z_Len(2000);
          }
          switch (contro) {
            case 0:
              lcd.setCursor(0, 1);
              lcd.print(">");
              lcd.setCursor(0, 2);
              lcd.print(" ");
              lcd.setCursor(0, 3);
              lcd.print(" ");
              lcd.setCursor(17, 3);
              lcd.print(" ");
              x1 = TrucX;
              y1 = TrucY;
              if (digitalRead(TRAI_PIN) == 1 && digitalRead(S2_PIN) == 0) {
                delay(100);
                while (digitalRead(TRAI_PIN) == 1) {
                  if (TrucX < 3000) {
                    X_Trai(1, 600);
                    TrucX++;
                  } else Bao(3, 80);
                }
              }
              if (digitalRead(PHAI_PIN) == 0 && digitalRead(S2_PIN) == 0) {
                delay(100);
                while (digitalRead(PHAI_PIN) == 0) {
                  if (TrucX > 0) {
                    X_Phai(1, 600);
                    TrucX--;
                  } else Bao(3, 80);
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
              lcd.setCursor(17, 3);
              lcd.print(" ");
              x2 = TrucX;
              y2 = TrucY;
              break;
            case 2:
              lcd.setCursor(0, 1);
              lcd.print(" ");
              lcd.setCursor(0, 2);
              lcd.print(" ");
              lcd.setCursor(0, 3);
              lcd.print(">");
              lcd.setCursor(17, 3);
              lcd.print(" ");
              x3 = TrucX;
              y3 = TrucY;
              break;
            case 3:
              lcd.setCursor(0, 1);
              lcd.print(" ");
              lcd.setCursor(0, 2);
              lcd.print(" ");
              lcd.setCursor(0, 3);
              lcd.print(" ");
              lcd.setCursor(17, 3);
              lcd.print(">");
          }
          if (digitalRead(SET_PIN) == 0) {
            delay(100);
            if (digitalRead(SET_PIN) == 0) {
              EEPROM.write(EEPROM.read(40) * 4 + 0, float(x1) / 3.0);
              EEPROM.write(EEPROM.read(40) * 4 + 1, float(y1) / 3.0);
              EEPROM.write(EEPROM.read(40) * 4 + 2, float(x2 - x1) / 3.0);
              EEPROM.write(EEPROM.read(40) * 4 + 3, float(y3 - y2) / 3.0);
              contro = 0;
              menu = 1;
              break;
            }
          }
        }
      }
      while (menu == 4) {

        HienThiMenuCell(CellX, CellY, tg, k + 1);
        DKConTro(4);
        switch (contro) {
          case 0:
            lcd.setCursor(2, 2);
            lcd.print(">");
            lcd.setCursor(6, 2);
            lcd.print(" ");
            lcd.setCursor(0, 3);
            lcd.print(" ");
            lcd.setCursor(9, 3);
            lcd.print(" ");
            if (digitalRead(PHAI_PIN) == 0) {
              delay(20);
              if (digitalRead(PHAI_PIN) == 0) {
                if (CellX < 11) CellX++;
                else CellX = 1;
              }
            }
            if (digitalRead(TRAI_PIN) == 1) {
              delay(20);
              if (digitalRead(TRAI_PIN) == 1) {
                if (CellX > 0) CellX--;
                else CellX = 10;
              }
            }
            break;
          case 1:
            lcd.setCursor(2, 2);
            lcd.print(" ");
            lcd.setCursor(6, 2);
            lcd.print(">");
            lcd.setCursor(0, 3);
            lcd.print(" ");
            lcd.setCursor(9, 3);
            lcd.print(" ");
            if (digitalRead(PHAI_PIN) == 0) {
              delay(20);
              if (digitalRead(PHAI_PIN) == 0) {
                if (CellY < 11) CellY++;
                else CellY = 1;
              }
            }
            if (digitalRead(TRAI_PIN) == 1) {
              delay(20);
              if (digitalRead(TRAI_PIN) == 1) {
                if (CellY > 0) CellY--;
                else CellY = 10;
              }
            }
            break;
          case 2:
            lcd.setCursor(2, 2);
            lcd.print(" ");
            lcd.setCursor(6, 2);
            lcd.print(" ");
            lcd.setCursor(0, 3);
            lcd.print(">");
            lcd.setCursor(9, 3);
            lcd.print(" ");
            HienThiMenuCell(CellX, CellY, tg, k + 1);
            if (digitalRead(PHAI_PIN) == 0) {
              delay(20);
              if (digitalRead(PHAI_PIN) == 0) {
                if (tg < 51) tg = tg + 5;
                else tg = 50;
              }
            }
            if (digitalRead(TRAI_PIN) == 1) {
              delay(20);
              if (digitalRead(TRAI_PIN) == 1) {
                if (tg > 0) tg = tg - 5;
                else tg = 10;
              }
            }
            break;
          case 3:
            lcd.setCursor(2, 2);
            lcd.print(" ");
            lcd.setCursor(6, 2);
            lcd.print(" ");
            lcd.setCursor(0, 3);
            lcd.print(" ");
            lcd.setCursor(9, 3);
            lcd.print(">");
            HienThiMenuCell(CellX, CellY, tg, k + 1);
            if (digitalRead(PHAI_PIN) == 0) {
              delay(20);
              if (digitalRead(PHAI_PIN) == 0) {
                if (k < 2) k++;
                else k = 2;
              }
            }
            if (digitalRead(TRAI_PIN) == 1) {
              delay(20);
              if (digitalRead(TRAI_PIN) == 1) {
                if (k > 0) k--;
                else k = 0;
              }
            }
            break;
        }
        if (digitalRead(SET_PIN) == 0) {
          delay(100);
          if (digitalRead(SET_PIN) == 0) {
            EEPROM.write(41, CellX);
            EEPROM.write(42, CellY);
            EEPROM.write(43, k);
            EEPROM.write(44, tg);
            menu = 1;
          }
        }
      }
    }
  }
}
