# Baby Monitor System (Arduino-Based)

## Overview
This project is a multi-Arduino baby monitoring system designed to track environmental conditions and provide parents with remote interaction capabilities. The system monitors temperature and noise levels, alerts caregivers when thresholds are exceeded, and allows remote activation of a soothing mobile.

The architecture uses a master-slave configuration with communication handled via the I2C protocol.

---

## System Architecture

The prototype consists of three Arduino units:

### 1. Master Arduino (Parent Unit)
- Acts as the central controller
- Receives and processes data from both slave Arduinos
- Sends instructions back to slaves via I2C
- Displays information on an LCD screen

**Features:**
- Live temperature display
- Alerts indicating if the room is too hot or too cold
- Potentiometer-controlled LCD brightness
- Converts voltage readings into Celsius

---

### 2. Slave Arduino 1 (Parent Control Unit)
Provides user controls and alerts.

**Components:**
- Two buttons:
  - System ON/OFF
  - Servo (mobile) ON/OFF
- LED indicator for system state
- Buzzer for alerts

**Functionality:**
- Sends system state and servo status to master (as an array)
- Alerts parents when:
  - Temperature exceeds limits
  - Noise threshold is exceeded

---

### 3. Slave Arduino 2 (Baby Room Unit)
Monitors the baby’s environment.

**Components:**
- Temperature sensor
- Sound sensor ("reverse buzzer")
- Potentiometer (noise sensitivity control)
- Servo motor (controls soothing mobile)

**Functionality:**
- Detects:
  - Temperature outside 20°C – 22°C
  - Noise above threshold
- Sends environmental data to master
- Receives servo control instructions

---

## Key Features
- Real-time temperature monitoring
- Noise detection with adjustable sensitivity
- Remote activation of a servo-powered baby mobile
- Audio alerts for environmental disturbances
- LCD feedback system for parents
- Efficient I2C communication

---

## Technical Highlights
- Built using C++ for Arduino
- Implemented User-Defined Functions (UDFs) for modularity
- Reduced I2C bottlenecks through distributed processing
- Designed for real-time responsiveness

---

## How It Works
1. Baby unit monitors temperature and sound continuously
2. Data is sent to the master Arduino via I2C
3. Master processes and displays the data
4. Parent unit allows manual control and triggers alerts
5. Servo motor can be activated remotely to soothe the baby

---

## Conclusion
This project demonstrates how a distributed embedded system can solve a real-world problem. It enables environmental monitoring, remote interaction, and efficient device communication.

---

## Team & Development
- Developed in a team of four
- Focused on embedded systems and real-time control
- Implemented efficient inter-device communication

---

## Future Improvements
- Software integration
- Wireless communication (Wi-Fi/Bluetooth)
- Improved UI/UX
- Smart alert scheduling
