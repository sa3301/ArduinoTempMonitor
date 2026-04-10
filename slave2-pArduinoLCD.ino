#include <Wire.h>
// Code adapted from: https://howtomechatronics.com/tutorials/arduino/lcd-tutorial/, Author: Dejan Nedelkovski
#include <LiquidCrystal.h>

// Code adapted from: https://forum.arduino.cc/t/convert-float-to-byte-and-back-again/240031, Author: Arrch
typedef union
{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;
FLOATUNION_t c;

// LCD
LiquidCrystal lcd(3, 4, 8, 9, 10, 11);
String indicators[3] = {"It's too cold.", "", "It's too hot."};
int choice;
unsigned long lastUpdate;
unsigned long updateCD = 500;
bool canSleep = true;


void setup() {
  Serial.begin(9600);

  // I2C
  Wire.begin(2);
  Wire.onReceive(DataReceived);

  // LCD
  lcd.begin(16, 2);
}

void loop() {
  if (millis() > lastUpdate + 2000 && canSleep) { // "sleep" after 2 seconds of no updates (system turned off)
    canSleep = false;
    Sleep();
  }
}


void DataReceived() {
  canSleep = true;
  for (int i = 0; i < 4; i++) {
    c.bytes[i] = Wire.read();
  }

  if (millis() > lastUpdate + updateCD) { // update LCD screen after cooldown
      UpdateLCD(c.number);
  }
}


void UpdateLCD(float temperature) {
  lastUpdate = millis();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(temperature) + " " + (char)223 +"C"); // Degree symbol: https://forum.arduino.cc/t/print-degree-symbol-on-lcd/19073, Author: mem
  lcd.setCursor(0, 1);
  if (temperature < 20.00){
    choice = 0;
  } else if (temperature > 22.00) {
    choice = 2;
  } else {
    choice = 1;
  }
  lcd.print(indicators[choice]);
}


void Sleep() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-_-_-_-_-_-_-_-_");
  lcd.setCursor(0, 1);
  lcd.print("-_-_-_-_-_-_-_-_");
}
