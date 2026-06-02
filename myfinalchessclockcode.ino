#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD ADDRESS
LiquidCrystal_I2C lcd(0x27, 16, 2);

// BUTTON PINS
const int startButton = 4;
const int resetButton = 5;

// VARIABLES
bool running = false;

unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup() {

  // BUTTON INPUTS
  pinMode(startButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);

  // LCD START
  lcd.init();
  lcd.backlight();

  // START SCREEN
  lcd.setCursor(0, 0);
  lcd.print("STOPWATCH");

  lcd.setCursor(0, 1);
  lcd.print("READY");

  delay(2000);

  lcd.clear();
}

void loop() {

  // START / PAUSE BUTTON
  if (digitalRead(startButton) == LOW) {

    delay(200);

    running = !running;

    if (running) {
      startTime = millis() - elapsedTime;
    }
  }

  // RESET BUTTON
  if (digitalRead(resetButton) == LOW) {

    delay(200);

    running = false;
    elapsedTime = 0;
  }

  // UPDATE TIME
  if (running) {
    elapsedTime = millis() - startTime;
  }

  // TIME CALCULATION
  unsigned long totalSeconds = elapsedTime / 1000;

  int minutes = totalSeconds / 60;
  int seconds = totalSeconds % 60;

  // LCD DISPLAY
  lcd.setCursor(0, 0);
  lcd.print("STOPWATCH     ");

  lcd.setCursor(0, 1);

  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);

  lcd.print(":");

  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);

  lcd.print("          ");

  delay(100);
}