# LED Subsystem Specification

**Project:** NAS Controller Firmware  
**Subsystem:** LED Status Indicator  
**Document Version:** 1.0  
**Status:** FROZEN  
**Last Updated:** 18 July 2026

---

# 1. Purpose

The LED subsystem provides a simple visual indication of NAS drive status.

It is intended to:

- Show individual drive state.
- Indicate system status.
- Display boot and shutdown animations.
- Display warning and fault conditions.
- Operate independently from the user interface.

The LED subsystem shall remain fully functional even when no web interface, display, or host application is connected.

---

# 2. Hardware Specification

Communication Frequency

800 kHz

Color Order

GRB

Driver Method

ESP32 RMT Peripheral

Software Library

Adafruit NeoPixel

## LED Type

WS2812B Addressable RGB LED Strip

---

## Quantity

- Total LEDs on strip: **60 LEDs**
- LEDs currently used by firmware: **8 LEDs**

Remaining LEDs are reserved for future expansion.

---

## Power

Supply Voltage:

- +5V

Power Source:

- NAS Power Supply

Maximum brightness shall be software limited.

---

## Data Interface

Single data line.

Connection:

ESP32 GPIO → WS2812 DIN

---

## ESP32 GPIO

Reserved GPIO:

**GPIO18**

Status:

**Reserved exclusively for LED subsystem.**

No other subsystem may use this GPIO.

---

# 3. LED Mapping

Current firmware uses the first eight LEDs only.

| LED | Function |
|------|----------|
| LED0 | Drive 1 |
| LED1 | Drive 2 |
| LED2 | Drive 3 |
| LED3 | Drive 4 |
| LED4 | Drive 5 |
| LED5 | Drive 6 |
| LED6 | Drive 7 |
| LED7 | Drive 8 |

Remaining LEDs are unused.

---

# 4. Brightness

Global brightness control shall be supported.

Brightness affects every LED equally.

Default brightness:

**40%**

Reason:

- Lower power consumption
- Reduced heat
- Comfortable indoor visibility
- Longer LED lifetime

Brightness shall be configurable in future firmware.

---

# 5. Color Definitions

The following colors are reserved.

| State | Color |
|---------|--------|
| Off | Black |
| Drive Idle | Blue |
| Drive Reading | Green |
| Drive Writing | Yellow |
| Drive Error | Red |
| Drive Missing | Purple |
| Drive Rebuilding | Cyan |
| Reserved | White |

These colors are considered firmware constants.

---

# 6. Boot Behaviour

After system power-on:

1. Initialize LED subsystem.
2. Clear all LEDs.
3. Execute boot animation.
4. Display drive states.

Boot animation shall not exceed **3 seconds**.

---

# 7. Shutdown Behaviour

On shutdown:

- Save required state.
- Turn off all LEDs.

No animation is required.

---

# 8. Error Behaviour

If the LED subsystem fails:

- System boot shall continue.
- Failure shall be logged.
- LED subsystem shall be marked unavailable.

The LED subsystem shall never prevent firmware startup.

---

# 9. Future Expansion

The remaining LEDs are reserved for future firmware features.

Possible future assignments include:

- Network activity
- System heartbeat
- Temperature indication
- Fan status
- RAID status
- USB status
- Power status
- Backup progress

No assumptions shall be made about unused LEDs.

---

# 10. Firmware Architecture

The LED subsystem follows the standard firmware architecture.

```
Platform
    ↓
Drivers
    ↓
Objects
    ↓
Services
```

Responsibilities:

### Platform

Responsible for:

- ESP32 RMT initialization
- WS2812 protocol generation
- Pixel transmission
- Strip brightness
- Hardware initialization

The Platform layer shall not implement animations, drive logic, or NAS-specific behavior.

---

### Drivers

Responsible for:

- LED framebuffer
- Brightness scaling
- Pixel updates
- Animation support

No drive knowledge.

---

### Objects

Responsible for:

- Mapping drive state to LED state.

No hardware access.

---

### Services

Responsible for:

- Listening to firmware events.
- Updating drive objects.
- Triggering LED refresh.

---

# 11. Memory Allocation

The subsystem shall:

- use static allocation only
- use no heap allocation
- perform no dynamic memory allocation

---

# 12. Performance Requirements

LED updates shall be:

- deterministic
- non-blocking where possible
- low CPU overhead

Animations shall not interfere with critical firmware tasks.

---

# 13. Timing Requirements
WS2812 communication shall operate at 800 kHz.

A complete refresh of the 60 LED strip shall complete in approximately 2 ms.

LED updates shall be initiated only when framebuffer contents change.

The subsystem shall avoid unnecessary strip refreshes.

---

# 14. Design Constraints

The subsystem shall:

- support exactly one LED strip
- support one global brightness value
- support eight active drive LEDs
- remain independent of UI
- remain independent of networking
- remain independent of USB communication

---

# 15. Future Compatibility

The architecture shall support future expansion without breaking the current API.

Potential future additions include:

- Multiple LED strips
- Per-LED brightness
- RGBW LEDs
- Multiple animations
- User configurable themes
- Event-driven lighting effects

These features are outside the scope of Version 1.0.

---

# 16. Frozen Decisions

The following decisions are frozen for Version 1.0.

| Item | Value |
|------|-------|
| LED Type | WS2812B |
| LED Strip Count | 1 |
| Total LEDs | 60 |
| Active LEDs | 8 |
| Power Supply | 5V |
| Control Method | Direct ESP32 GPIO |
| Reserved GPIO | GPIO18 |
| Default Brightness | 40% |
| Drive Mapping | LED0–LED7 |
| Memory Allocation | Static Only |

---

# 17. Revision History

| Version | Date | Description |
|----------|------|-------------|
| 1.0 | 18 Jul 2026 | Initial frozen specification |


# Implementation Freeze

**Status:** **FROZEN**

This section defines the implementation details for the LED subsystem. These decisions are considered final and shall not be changed unless an architecture review explicitly approves the modification.

---

## Hardware Configuration

| Item               | Value                              |
| ------------------ | ---------------------------------- |
| LED Type           | WS2812B Intelligent RGB LED Strip  |
| LED Count          | 60 LEDs (1 meter strip, 60 LEDs/m) |
| Supply Voltage     | 5 VDC                              |
| Data Interface     | Single-wire serial                 |
| ESP32 Controller   | ESP32-WROOM-32                     |
| Data GPIO          | GPIO18                             |
| Color Order        | GRB                                |
| Signal Frequency   | 800 kHz                            |
| LED Driver Method  | ESP32 RMT Peripheral               |
| Software Library   | Adafruit NeoPixel                  |
| Brightness Control | Software                           |
| Default Brightness | 40% (102/255)                      |

---

## Reserved LED Mapping

Only the first eight LEDs are currently allocated.

| LED Index | Purpose        |
| --------- | -------------- |
| LED 0     | Drive 1 Status |
| LED 1     | Drive 2 Status |
| LED 2     | Drive 3 Status |
| LED 3     | Drive 4 Status |
| LED 4     | Drive 5 Status |
| LED 5     | Drive 6 Status |
| LED 6     | Drive 7 Status |
| LED 7     | Drive 8 Status |

All remaining LEDs are reserved for future expansion and shall remain unused until officially assigned.

---

## Software Architecture

The LED subsystem shall follow the standard firmware architecture.

### Platform Layer

Responsible only for low-level WS2812 hardware communication.

Responsibilities:

* Initialize LED hardware
* Set individual LED color
* Set strip brightness
* Clear strip
* Transfer framebuffer to LEDs

The Platform layer shall contain **no animations, effects, drive logic, or application behavior**.

---

### Driver Layer

Responsible for LED buffer management and hardware abstraction.

Responsibilities:

* Maintain LED framebuffer
* Update individual LEDs
* Apply brightness
* Send framebuffer to Platform layer

---

### Object Layer

Responsible for logical LED ownership.

Responsibilities:

* Map LEDs to physical drives
* Maintain drive-to-LED relationships
* Convert drive state into LED color requests

---

### Service Layer

Responsible for system behavior.

Responsibilities:

* React to system events
* Update drive LEDs
* Execute startup, shutdown, warning, and fault indications
* Coordinate LED updates requested by higher-level firmware

---

## Memory Policy

* Static allocation only.
* Dynamic memory allocation is prohibited.
* No heap usage is permitted inside the LED subsystem.

---

## Threading Policy

The LED hardware shall be accessed only through the Driver layer.

Application code shall never communicate directly with the Platform layer.

---

## Brightness Policy

Default startup brightness:

* **40% (102 / 255)**

Brightness shall be configurable by firmware but initialized to the default value during system startup.

---

## Future Expansion

The remaining LEDs are reserved for future features, including but not limited to:

* System status
* Network activity
* Temperature indication
* RAID status
* Backup progress
* Power state
* User-defined effects

These LEDs shall remain unassigned until a future design revision.

---

# Frozen Design Decisions

The following implementation choices are frozen for Version 1.x of the firmware:

* WS2812B LED strip
* 60 LEDs
* 5 V supply
* GPIO18 data pin
* GRB color order
* 800 kHz communication
* ESP32 RMT peripheral
* Adafruit NeoPixel library
* Static memory allocation
* Platform → Driver → Object → Service architecture
* Default brightness of 40%
* First 8 LEDs reserved for drive status

These decisions establish the baseline implementation and shall not be modified without a formal architecture review and documentation update.
