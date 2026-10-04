# Smart Transformer Fault Detection and Automatic Load Sharing

This project uses **ESP32, IoT, and embedded systems** to monitor transformer health and automatically share load during faults.

## Features
- Real-time monitoring of voltage, current, and temperature
- LCD display for local monitoring
- IoT integration with ThingSpeak for remote monitoring
- Automatic load sharing using relay control
- Alerts via buzzer for over-temperature, overload, and low voltage

## Hardware
- ESP32 microcontroller
- INA219 current sensor
- DHT11 temperature sensor
- LCD (I2C)
- Relay module
- Buzzer
- DC motor (load simulation)

## Software
- Arduino IDE (ESP32 board support)
- ThingSpeak IoT platform

## Results
- Faults detected early
- Load automatically shifted to backup transformer
- Remote monitoring enabled via IoT
