# ADR-0004: Drivers Layer

- Status: Accepted
- Date: 2026-07-18

---

## Context

The firmware requires a dedicated hardware driver layer responsible for controlling physical devices while remaining independent of firmware business logic.

Direct hardware access from Services or Objects would tightly couple application logic to specific peripherals and reduce maintainability.

A dedicated Driver layer provides a clean separation between hardware implementation and firmware functionality.

---

## Decision

The Drivers layer shall contain one driver for each hardware device.

Implemented drivers include:

- RelayDriver - Relay control via GPIO
- PWMFanDriver - PWM-controlled fan speed control
- TemperatureDriver - DS18B20 temperature sensor via OneWire
- AddressableLedDriver - WS2812 addressable LED control via SPI
- DriveDetectionDriver - Hard drive detection via GPIO
- StorageDriver - Persistent storage via Flash memory
- WatchdogDriver - System watchdog management

Drivers expose hardware functionality through clean interfaces while internally using only the Platform HAL.

Every driver returns standardized `Core::Result` values.

---

## Responsibilities

Drivers are responsible for:

- Initializing hardware devices.
- Reading hardware state.
- Writing hardware state.
- Converting Platform HAL operations into device-level operations.
- Handling device-specific implementation details.

Drivers are **not** responsible for:

- Business logic.
- Communication protocols.
- Configuration management.
- Event processing.
- User commands.

---

## Consequences

### Advantages

- Hardware logic is isolated.
- Drivers remain reusable.
- Services become hardware independent.
- Easier testing using mocked Platform interfaces.
- Easier hardware replacement.

### Limitations

Drivers must never:

- Include protocol code.
- Include application logic.
- Maintain global firmware state.
- Access Objects directly.

---

## Dependency Rules

Allowed dependencies:

```
Drivers
│
├── Core
└── Platform
```

Drivers must never depend on:

- Objects
- Services
- Protocol
- System

Only the Services layer may communicate with Drivers.

---

## Architecture

Hardware access follows this flow:

```
Application
      │
Services
      │
Drivers
      │
Platform HAL
      │
ESP32 Hardware
```

This ensures every hardware operation passes through the HAL before reaching the physical device.

---

## Status

Frozen for Firmware Version 1.x.
