#include <Wire.h>

// system
byte systemOn = 1;
int onOffButtonPin = 2;
int onOffLEDPin = 3;
bool canTurnOnOff = true;

// buzzer
int buzzerPin = 13;
byte buzz;
int freq = 50;

// mobile
byte spin = 0;
int spinButtonPin = 8;
bool spinButtonPress = false;
unsigned long spinButtonPressTime;
unsigned long spinButtonCD = 500;

// Code adapted from: https://forum.arduino.cc/t/solved-sending-array-data-with-i2c/537012/2, Author: GolamMostafa
byte onOffSpinArray[2] = {0x1, 0x0};


void setup() {
  Serial.begin(9600);
  // I2C
  Wire.begin(1);
  Wire.onReceive(DataReceived);
  Wire.onRequest(DataRequested);

  // system
  pinMode(onOffButtonPin, INPUT);
  pinMode(onOffLEDPin, OUTPUT);
  digitalWrite(onOffLEDPin, HIGH);

  // buzzer
  pinMode(buzzerPin, OUTPUT);
  
  // mobile
  pinMode(spinButtonPin, INPUT);


}

void loop() {
  OnOffToggle();
  CheckMobileButton();
  if (systemOn == 1) {
    BuzzerProcedure(buzz);
  } else {
    noTone(buzzerPin);
  }
}


void OnOffToggle() {
  if (digitalRead(onOffButtonPin) == HIGH && canTurnOnOff) {
    canTurnOnOff = false;
    if (systemOn == 1) {
      systemOn = 0;
      digitalWrite(onOffLEDPin, LOW);
    } else {
      systemOn = 1;
      digitalWrite(onOffLEDPin, HIGH);
    }
    AllowOnOff(millis());
  }
}


void AllowOnOff(unsigned long timeToggled) {
  if (millis() > timeToggled + 500) {
    canTurnOnOff = true;
  } else {
    AllowOnOff(timeToggled);
  }
}


void CheckMobileButton() {
  if (digitalRead(spinButtonPin) == HIGH && millis() > spinButtonPressTime + spinButtonCD) {
    spinButtonPressTime = millis();
    if (spin == 1) {
      spin = 0;
    } else {
      spin = 1;
    }
  }
}


// Code adapted from: https://www.instructables.com/Interfacing-Buzzer-to-Arduino/; Author: Autodesk Instructables; Date used: 3/12/2023
void BuzzerProcedure(byte on){
  Serial.println(on);
  if (on == 1){
    tone(buzzerPin, freq);
  } else {
    noTone(buzzerPin);
  }
}


void DataReceived() {
  buzz = Wire.read();
}


void DataRequested() { // send system on/off state and whether the mobile should spin as a list of bytes
  if (systemOn == 1) {
    onOffSpinArray[0] = 0x1;
  } else {
    onOffSpinArray[0] = 0x0;
  }
  if (spin == 1) {
    onOffSpinArray[1] = 0x1;
  } else {
    onOffSpinArray[1] = 0x0;
  }
  Wire.write(onOffSpinArray, 2);
}
