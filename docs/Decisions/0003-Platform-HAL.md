# ADR-0003: Namespace-Based Platform Hardware Abstraction Layer

Status: Accepted

Date: 2026-07-17

Decision ID: ADR-0003

---

# Context

The original Platform implementation used instantiable C++ classes.

ESP32 hardware peripherals are globally unique resources and do not require object instances.

Platform should expose hardware capabilities only.

---

# Decision

Platform SHALL expose namespace-based APIs.

Example:

- NAS::Platform::GPIO::Configure(...)
- NAS::Platform::PWM::SetDutyCycle(...)
- NAS::Platform::USB::Write(...)

Platform SHALL:

- Abstract ESP32 hardware.
- Hide Arduino implementation.
- Hide ESP-IDF implementation.
- Return Core::Result.
- Contain no application logic.
- Contain no protocol logic.
- Contain no business logic.
- Contain no service logic.

Platform SHALL NOT:

- Allocate dynamic memory.
- Use new/delete.
- Know about relays.
- Know about fans.
- Know about LEDs.
- Know about temperature sensors.
- Know about storage logic.
- Know about protocol parsing.
- Know about System Manager.

---

# Consequences

## Benefits

- Cleaner architecture.
- Easier maintenance.
- Easier migration to future ESP32 hardware.
- Arduino updates isolated.
- Drivers remain hardware independent.
- Services remain platform independent.

## Trade-offs

- Slightly more wrapper functions.
- Platform owns all hardware access.

---

# Scope

This decision applies to:

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

This ADR is frozen for Firmware Version 1.x.

---

# Approval

Status: Accepted

This decision establishes the namespace-based Platform HAL contract for NAS Controller Firmware Version 1.x.
