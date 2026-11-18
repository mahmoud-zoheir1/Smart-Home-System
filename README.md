# Smart Home project

## Project Overview

This project is a Smart Home System designed to improve home security, comfort, and energy efficiency through the integration of sensors, actuators, and Arduino microcontrollers.

The system uses two Arduino boards to control different smart home functions independently.

## System Architecture

The project is divided into two main control units:

### Arduino 1 – Main Smart Home Controller

Responsible for door security, temperature control, window monitoring, and motion detection.

**Components:**

- Arduino Uno
- 4×4 Matrix Keypad
- Servo Motor
- DHT11 Temperature Sensor
- Magnetic Reed Switch
- PIR Motion Sensor
- DC Motor (Fan)
- Motor Driver Module
- 16×2 I2C LCD
- Push Button
- Alarm Indicator
- Motion Indicator LED

**Functionality:**

1. **Door Security System:** Uses a keypad password to control the servo motor and lock/unlock the door.
2. **Temperature Control:** Monitors room temperature using DHT11 and activates the fan when the temperature exceeds 30°C.
3. **Window Security:** Monitors window status using a magnetic reed switch and activates the alarm indicator when the window opens during security mode.
4. **Motion Detection:** Detects movement using a PIR sensor and activates the motion indicator LED when security mode is enabled.
5. **LCD Display:** Displays system status, password input, alerts, and temperature readings.

### Arduino 2 – Automatic Lighting Control

Responsible for controlling the lighting system based on ambient light intensity.

**Components:**

- Arduino Uno
- LDR Sensor
- LED

**Functionality:**

- Measures ambient light intensity using the LDR sensor.
- Reads sensor values every 2 seconds.
- Turns ON the LED when the LDR reading is below 400.
- Turns OFF the LED when the reading is equal to or above 400.

## Repository Structure

```text
Smart-Home-System/
│
├── Arduino_Codes/
│   ├── Arduino_1_Main_Controller.ino
│   └── Arduino_2_Lighting_Control.ino
│
└── README.md
```

## Software Requirements

- Arduino IDE
- Arduino Uno
- Required Arduino libraries:
  - Wire
  - LiquidCrystal_I2C
  - DHT
  - Servo
  - Keypad

## Team Members

1. Mahmoud Zoheir Abdelkader
2. Mohamed Walid
