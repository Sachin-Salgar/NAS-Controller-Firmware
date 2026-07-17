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

- PlatformManager
- GPIO
- Timer
- PWM
- USB
- OneWire
- ADC
- I2C
- SPI
- UART
- Flash
- Restart
- Watchdog

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