#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// BUTTON PINS
const int p1Button = 8;
const int p2Button = 9;
const int modeButton = 7;

// LED PINS
const int p1LED = 4;
const int p2LED = 5;

// BUZZER PINS
const int p1Buzzer = 2;
const int p2Buzzer = 3;

// GAME VARIABLES
int mode = 0;

int p1Time;
int p2Time;

bool player1Turn = true;
bool gameRunning = false;

unsigned long lastMillis = 0;

// BUTTON STATES
bool lastModeState = HIGH;
bool lastP1State = HIGH;
bool lastP2State = HIGH;

void setup() {

  lcd.init();
  lcd.backlight();

  pinMode(p1Button, INPUT_PULLUP);
  pinMode(p2Button, INPUT_PULLUP);
  pinMode(modeButton, INPUT_PULLUP);

  pinMode(p1LED, OUTPUT);
  pinMode(p2LED, OUTPUT);

  pinMode(p1Buzzer, OUTPUT);
  pinMode(p2Buzzer, OUTPUT);

  setModeTime();
  displayTime();
  updateLEDs();
}

void loop() {

  checkModeButton();
  checkPlayerButtons();

  if (gameRunning) {

    if (millis() - lastMillis >= 1000) {

      lastMillis = millis();

      if (player1Turn) {
        p1Time--;
      }
      else {
        p2Time--;
      }

      displayTime();

      if (p1Time <= 0) {

        p1Time = 0;
        gameRunning = false;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PLAYER 2 WINS");

        beepBuzzer(p1Buzzer);
      }

      if (p2Time <= 0) {

        p2Time = 0;
        gameRunning = false;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PLAYER 1 WINS");

        beepBuzzer(p2Buzzer);
      }
    }
  }

  updateLEDs();
}

void checkModeButton() {

  bool currentState = digitalRead(modeButton);

  if (currentState == LOW && lastModeState == HIGH) {

    mode++;

    if (mode > 2) {
      mode = 0;
    }

    setModeTime();

    player1Turn = true;
    gameRunning = false;

    displayTime();

    delay(200);
  }

  lastModeState = currentState;
}

void checkPlayerButtons() {

  bool p1State = digitalRead(p1Button);
  bool p2State = digitalRead(p2Button);

  if (p1State == LOW && lastP1State == HIGH) {

    player1Turn = false;
    gameRunning = true;

    delay(100);
  }

  if (p2State == LOW && lastP2State == HIGH) {

    player1Turn = true;
    gameRunning = true;

    delay(100);
  }

  lastP1State = p1State;
  lastP2State = p2State;
}

void setModeTime() {

  // RAPID 10 MIN
  if (mode == 0) {
    p1Time = 600;
    p2Time = 600;
  }

  // BLITZ 5 MIN
  if (mode == 1) {
    p1Time = 300;
    p2Time = 300;
  }

  // BULLET 1 MIN
  if (mode == 2) {
    p1Time = 60;
    p2Time = 60;
  }
}

void displayTime() {

  lcd.clear();

  if (mode == 0) {
    lcd.setCursor(0, 0);
    lcd.print("RAPID");
  }

  if (mode == 1) {
    lcd.setCursor(0, 0);
    lcd.print("BLITZ");
  }

  if (mode == 2) {
    lcd.setCursor(0, 0);
    lcd.print("BULLET");
  }

  lcd.setCursor(0, 1);

  lcd.print(formatTime(p1Time));
  lcd.print(" ");
  lcd.print(formatTime(p2Time));
}

String formatTime(int seconds) {

  int mins = seconds / 60;
  int secs = seconds % 60;

  String t = "";

  if (mins < 10) t += "0";
  t += String(mins);

  t += ":";

  if (secs < 10) t += "0";
  t += String(secs);

  return t;
}

void updateLEDs() {

  if (player1Turn) {

    digitalWrite(p1LED, HIGH);
    digitalWrite(p2LED, LOW);

  }
  else {

    digitalWrite(p1LED, LOW);
    digitalWrite(p2LED, HIGH);
  }
}

void beepBuzzer(int buzzerPin) {

  for (int i = 0; i < 5; i++) {

    tone(buzzerPin, 1000);

    delay(200);

    noTone(buzzerPin);

    delay(200);
  }
}