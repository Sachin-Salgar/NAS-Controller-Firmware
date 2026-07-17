# ADR-0003: Platform Hardware Abstraction Layer

- Status: Accepted
- Date: 2026-07-18

---

## Context

The firmware targets an ESP32 using the Arduino framework today, but future hardware revisions or MCU changes should require minimal modifications.

Without a Hardware Abstraction Layer (HAL), hardware-specific code would spread throughout Drivers and Services, creating tight coupling to Arduino and ESP-IDF.

A dedicated Platform layer is required to isolate all hardware implementation details.

---

## Decision

The Platform layer shall be the only layer permitted to access:

- Arduino Framework APIs
- ESP-IDF APIs
- GPIO peripherals
- Timers
- PWM
- USB
- Flash memory
- Watchdog
- ADC
- UART
- SPI
- I2C
- OneWire

The Platform layer exposes lightweight namespace-based APIs and returns standardized Core::Result objects.

Implemented modules include:

- GPIO - Digital input/output pin control
- Timer - Hardware timer operations
- PWM - Pulse Width Modulation for variable control
- USB - USB CDC serial communication (character and binary modes)
- OneWire - OneWire protocol for temperature sensors
- ADC - Analog to Digital conversion
- I2C - I2C communication protocol
- SPI - Serial Peripheral Interface for addressable LEDs
- UART - Serial communication
- Flash - Non-volatile memory operations
- Restart - System restart functionality
- Watchdog - Watchdog timer for system supervision

### USB Platform Operations

The USB platform layer provides both character and binary operations:

**Character Operations** (for debugging and text-based communication):
- `Write(const char* text)` - Send text
- `WriteLine(const char* text)` - Send text with newline
- `Read(char& character)` - Receive single character
- `ReadLine(char* buffer, size_t length)` - Receive line of text

**Binary Operations** (for protocol packet communication):
- `Write(const uint8_t* data, size_t length)` - Send binary data
- `Read(uint8_t* data, size_t bufferSize, size_t& bytesReceived)` - Receive binary data

This dual interface supports both debugging and binary protocol operations through the same abstraction layer.

---

## Consequences

### Advantages

- Hardware is isolated.
- Drivers remain portable.
- Arduino dependencies exist in one layer only.
- ESP-IDF dependencies exist in one layer only.
- Easier migration to future hardware.
- Easier unit testing using mock HAL implementations.

### Limitations

Platform is not responsible for:

- Business logic
- Firmware state
- Communication protocols
- Configuration management

---

## Dependency Rules

Allowed dependencies:

Platform
↓

Core

Platform must never depend on:

- Drivers
- Objects
- Services
- Protocol
- System

Drivers are the only layer permitted to communicate directly with Platform.

---

## Architecture

The Platform layer provides thin wrappers around hardware functionality and contains no business logic.

All hardware interaction flows through this layer before reaching higher firmware components.

---

## Status

Frozen for Firmware Version 1.x.
