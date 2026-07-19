# NAS Controller Firmware
# Hardware Specification

Document ID: NCF-HARDWARE-001

Hardware Version: 1.0

Document Version: 1.0

Status: Released

Copyright © NAS Controller Firmware Project

---

# Revision History

| Version | Date | Description |
|----------|------|-------------|
| 1.0 | Initial Release | Development Hardware Specification |

---

# Table of Contents

1. Purpose
2. Scope
3. System Overview
4. Hardware Architecture
5. Development Platform
6. Component Inventory
7. Power Architecture
8. GPIO Allocation
9. Relay Module
10. MOSFET Module
11. Temperature Sensors
12. LED System
13. Communication Interfaces
14. Reserved Interfaces
15. Wiring Overview
16. Hardware Characteristics
17. Hardware Limits
18. Future Expansion
19. Bill of Materials
20. Hardware Freeze Notice

---

# 1. Purpose

This document defines the complete hardware platform used for development and testing of the NAS Controller Firmware.

It serves as the hardware contract between firmware and physical hardware.

Firmware Version 1.x SHALL target the hardware described in this document.

---

# 2. Scope

This document specifies

- ESP32 Development Board
- Power Architecture
- Relay Module
- MOSFET Module
- Temperature Sensors
- LED System
- GPIO Allocation
- Communication Interfaces
- Development Hardware Limits

PCB implementation details are intentionally excluded.

---

# 3. System Overview

The NAS Controller is an external hardware controller connected to the host computer using USB CDC Serial.

The controller manages hardware independently of the operating system.

Primary responsibilities include

- Relay Control
- Cooling Fan Control
- Temperature Monitoring
- LED Status Indication
- Hardware Safety
- Future Hardware Expansion

The host computer reports system events while the ESP32 controls hardware.

---

# 4. Hardware Architecture

```
                    Dell OptiPlex 7090
                    (Windows / TrueNAS / OMV)
                              │
                     USB CDC Serial
                              │
                    ESP32-WROOM-32 Dev Board
                              │
        ┌─────────────┬──────────────┬──────────────┐
        │             │              │              │
     Relay Module   MOSFET PWM   DS18B20 Bus   WS2812B LEDs
        │             │              │              │
   Drive Power     Cooling Fans   Temperature    Status Display
```

---

# 5. Development Platform

## Controller

ESP32-WROOM-32 Development Board

## USB Interface

USB Type-C

CP2102 USB-to-UART

## MCU

Espressif ESP32

Dual Core Xtensa LX6

240 MHz

## Wireless

Wi-Fi

Bluetooth

(Not used by Firmware Version 1.x)

## Flash

4 MB

## Power

Powered exclusively through USB Type-C.

The custom hardware SHALL NOT power the ESP32.

---

# 6. Component Inventory

| Component | Quantity |
|-----------|---------:|
| ESP32-WROOM-32 Dev Board | 1 |
| 30A SPDT Relay Module | 2 Channels |
| MOSFET PWM Module | 2 Channels |
| DS18B20 Temperature Sensors | 3 |
| WS2812B Addressable LEDs | 13 Initial |
| ATX Power Supply | 1 |

---

# 7. Power Architecture

## ATX Supply Rails

12V

Used for

- Cooling Fans
- Relay Controlled Outputs

5V

Used for

- Relay Controlled Outputs
- WS2812B LEDs

3.3V

Generated onboard by the ESP32.

No external 3.3V regulator is required.

Ground SHALL be common between

- ATX PSU
- ESP32
- Relay Module
- MOSFET Module
- Temperature Sensors
- LED Strip

---

# 8. GPIO Allocation

## Assigned GPIOs

| GPIO | Function |
|------|----------|
| GPIO18 | Fan PWM Channel 1 |
| GPIO19 | Spare PWM Channel 2 |
| GPIO21 | DS18B20 OneWire Bus |
| GPIO22 | Relay 1 (5V) |
| GPIO23 | WS2812B Data |
| GPIO25 | Relay 2 (12V) |

## Reserved GPIOs

GPIO16

GPIO17

GPIO26

GPIO27

GPIO32

GPIO33

Reserved for future expansion.

Boot strap pins SHALL NOT be used for application hardware unless explicitly approved.

---

# 9. Relay Module

## Quantity

2

## Type

30A SPDT Relay Module

Optocoupler Isolated

## Logic

Active Low

## Assignment

Relay 1

5V Output Switching

Relay 2

12V Output Switching

The relay module SHALL receive commands exclusively from firmware.

---

# 10. MOSFET Module

## Quantity

2 PWM Outputs

## Logic

Active Low

## Assignment

Channel 1

Controls all five cooling fans.

Channel 2

Reserved for future PWM applications.

## PWM Characteristics

Frequency

25 kHz

Resolution

8-bit

Duty Cycle

0–100%

Fan positive supply SHALL remain permanently connected to +12V.

Fan speed SHALL be controlled by low-side switching of the ground connection.

---

# 11. Temperature Sensors

## Sensor Type

DS18B20

## Quantity

3

## Bus

OneWire

Single GPIO

## Pull-up

4.7 kΩ

## Planned Locations

Sensor 1

CPU Area

Sensor 2

Drive Bay

Sensor 3

Ambient Air

Sensor IDs SHALL remain stable throughout firmware operation.

---

# 12. LED System

## Type

WS2812B Addressable RGB LED Strip

## Initial Quantity

13 LEDs

## Maximum Supported

25 LEDs

## Control

Single GPIO

GPIO23

## Supply Voltage

5V

## Control Protocol

Single-wire serial

## Recommended Protection

330 Ω series resistor on Data line.

1000 µF capacitor across 5V and GND at LED strip input.

Logic level shifter MAY be added for improved signal integrity.

## Initial Logical Assignment

LED 1

System Power

LED 2

USB Connected

LED 3

Relay 1

LED 4

Relay 2

LED 5

Fan Status

LED 6

Temperature Status

LED 7

Drive Activity

LED 8

Backup Activity

LED 9–13

Reserved

LED functions are logical and may be reassigned by firmware.

---

# 13. Communication Interfaces

Primary Interface

USB CDC Serial

Used for

Protocol Version 1.x

Future interfaces SHALL remain disabled unless implemented by future firmware versions.

---

# 14. Reserved Interfaces

Reserved for future development.

I²C

SPI

UART

CAN

Ethernet

Wi-Fi

Bluetooth

RTC

OLED Display

Additional Temperature Sensors

UPS Monitoring

Reserved interfaces SHALL NOT affect Firmware Version 1.x.

---

# 15. Wiring Overview

```
ATX PSU
│
├── 12V ─────────────► Relay 2
│                   └── Switched 12V Output
│
├── 12V ─────────────► MOSFET Module
│                   └── Five Cooling Fans
│
├── 5V ─────────────► Relay 1
│                   └── Switched 5V Output
│
├── 5V ─────────────► WS2812B LED Strip
│
└── GND ───────────── Common Ground

ESP32

USB-C ───────────────► Host Computer

GPIO18 ──────────────► MOSFET PWM Channel 1

GPIO19 ──────────────► MOSFET PWM Channel 2

GPIO21 ──────────────► DS18B20 Bus

GPIO22 ──────────────► Relay 1

GPIO23 ──────────────► WS2812B Data

GPIO25 ──────────────► Relay 2
```

---

# 16. Hardware Characteristics

Relay Logic

Active Low

MOSFET Logic

Active Low

PWM Topology

Low-side Switching

Fan Supply

+12V Direct

PWM controls Ground.

Temperature Bus

OneWire

LED Protocol

WS2812B

USB Communication

CDC Serial

---

# 17. Hardware Limits

| Resource | Maximum |
|-----------|--------:|
| Cooling Fans | 5 |
| Relay Outputs | 2 |
| PWM Outputs | 2 |
| Temperature Sensors | 3 |
| WS2812B LEDs | 25 |

---

# 18. Future Expansion

Reserved for

- OLED Display
- Additional LEDs
- Additional Sensors
- Additional PWM Outputs
- Ethernet
- Wi-Fi Services
- MQTT
- Home Assistant Integration
- UPS Monitoring
- RTC Module

Expansion SHALL preserve compatibility with Firmware Version 1.x whenever possible.

---

# 19. Bill of Materials

| Item | Description |
|------|-------------|
| ESP32 | ESP32-WROOM-32 Development Board |
| Relay | 30A SPDT Optocoupler Relay Module |
| MOSFET | 2-Channel PWM MOSFET Module |
| Sensor | DS18B20 Waterproof Temperature Sensor |
| LEDs | WS2812B Addressable RGB Strip |
| PSU | Standard ATX Power Supply |

---

# 20. Hardware Freeze Notice

Hardware Name

NAS Controller Development Hardware

Hardware Version

1.0

Status

Released

This document defines the reference hardware platform for Firmware Version 1.x.

Future hardware revisions SHALL preserve firmware compatibility whenever practical.

Breaking hardware changes SHALL require a Hardware Version increment and an approved Architecture Decision Record (ADR).