#include <M5StickC.h>

const int DIR  = 26;
const int STEP = 36;

void setup() {
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  digitalWrite(DIR, LOW);
  digitalWrite(STEP, LOW);
}

void loop() {

  digitalWrite(DIR, HIGH);

  for (int i=0; i<200; i++) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(2000);
  }

  delay(1000);

  digitalWrite(DIR, LOW);

  for (int i=0; i<200; i++) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(2000);
  }

  delay(1000);
}

// void setup() {
//   M5.begin();

//   pinMode( PIN, INPUT);
//   M5.Lcd.setRotation(3);
//   M5.Lcd.fillScreen(BLACK);
//   M5.Lcd.setTextSize(1);
// }

// void loop() {
//   delay(500);
//   M5.Lcd.setCursor(0, 70);
//   M5.Lcd.printf("%04d\n", digitalRead(PIN));
// }