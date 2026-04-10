#include <Wire.h>
#include <Servo.h>

// Code adapted from: https://forum.arduino.cc/t/convert-float-to-byte-and-back-again/240031, Author: Arrch
typedef union
{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

// I2C
int shout = 1; // send

// system
bool systemOn = true;

// mobile
Servo mobile;
int servoPin = 14;
unsigned long mobileAlternateTime = 5000;
unsigned long startSpinTime;
bool canSpin = false;
byte spin = 0;
bool mobileAtDefault = true;

// potentiometer
int potentiometerPin = A1;
int limit;

// reverse piezo buzzer
int revPiezoPin = A2;
int volume;
byte soundDetected;

// temperature
int thermPin = A3;
float f;
FLOATUNION_t c;

// mobile and system
byte onOffSpinArray[2];


void setup() {
  Serial.begin(9600);

  // I2c
  Wire.begin();

  // mobile
  mobile.attach(servoPin);
  mobile.write(0);
  
}

void loop() {    
  TakeInputs(); // get sensor data

  if (systemOn == 1) { // If system is on, send data to slave Arduinos and allow spinning
    Shout();
  }

  Listen(); // request data from slave Arduino
  SpinMobile(spin); // spin mobile
}


void Shout() {
  // Code adapted from: https://www.instructables.com/Arduino-I2C-and-Multiple-Slaves/, Author: Robottronic
  switch (shout) {
    case 1:
      Wire.beginTransmission(1);
      Wire.write(soundDetected);
      Wire.endTransmission();
      shout = 2;
      break;

    case 2:
      Wire.beginTransmission(2);
      for (int i = 0; i < 4; i++) {
        Wire.write(c.bytes[i]);
      }
      Wire.endTransmission();
      shout = 1;
      break;
      
    default:
      break;
  }
}


void Listen() {
  // Code adapted from: https://forum.arduino.cc/t/i2c-communication-slave-to-master/473119, Author: Delta_G
  int n = Wire.requestFrom(1, 2);
  for (int i = 0; i < n; i++) {
    onOffSpinArray[i] = Wire.read();
  }
  systemOn = onOffSpinArray[0];
  spin = onOffSpinArray[1];
}


void TakeInputs() {
  // temperature
  // Code adapted from: https://www.tutorialspoint.com/arduino/arduino_temperature_sensor.htm, Author: tutorialspoint
  f = (analogRead(thermPin) * 0.48828125);
  c.number = float((f - 32.00) * (5.00 / 9.00));

  // noise
  limit = analogRead(potentiometerPin);
  volume = analogRead(revPiezoPin);

  if (volume >= limit) {
    soundDetected = 1;
  } else {
    soundDetected = 0;
  }
}


void SpinMobile(bool t) {
  if (t && canSpin && systemOn == 1) {
    canSpin = false;
    startSpinTime = millis();

    if (mobileAtDefault) {
      for (float i = 0; i < 180; i++){
        mobile.write(i);
      }
      mobileAtDefault = false;
    } else {
      for (float i = 180; i > 0; i--){
        mobile.write(i);
      }
      mobileAtDefault = true;
    }
  }
  if (!canSpin && t &&(millis() > startSpinTime + mobileAlternateTime)){
    canSpin = true;
    SpinMobile(t);
  }
}
