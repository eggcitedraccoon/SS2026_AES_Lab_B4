# 1. System Overview

## 1.1 General Information

This project proposes an automatic plant watering system using IoT technology. The system is designed to monitor soil
moisture and environmental conditions and to water plants as needed without manual input. It operates in indoor
environments such as homes or offices.

The system consists of sensor and actuator units connected to a central controller (Raspberry Pi). Sensors collect data
on soil moisture, temperature, humidity, and water levels. Based on this data, water is delivered using pumps. A local
web interface allows users to view plant status and control the system manually if needed. The system can also send
remote notifications.

The goal is to reduce the need for manual plant care and to ensure that plants receive water only when necessary

## 1.2 Bill of materials

### Controllers

- **1x Raspberry Pi 3B+** – *needed from uni*
- 3x ESP32-C3
- 2x ESP-01

### Sensors

- 2x DHT11 Temperature and Humidity Sensor
- 3x Soil Moisture Sensor
- 3x Water Level Sensor

### Actuators

- 3x Water Pump
- 3x Relay

### Power

- 3x 12V Power Supply
- 2x 9V Battery
- 3x DC-DC Converter
