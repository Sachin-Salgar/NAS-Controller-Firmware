# Changelog

## Unreleased

### Core Layer

#### Added

- Result framework
- ResultCode enumeration
- BuildInfo module
- Version module
- Event system
- EventBus
- Logger

#### Architecture

- Core is completely hardware independent.
- Core contains no Arduino dependencies.
- Core contains no ESP-IDF dependencies.
- Core provides the common foundation used by every higher firmware layer.
- Core is frozen for Firmware Version 1.x.

### Platform Hardware Abstraction Layer

#### Added

- GPIO HAL
- Timer HAL
- PWM HAL
- USB CDC HAL
- OneWire HAL
- ADC HAL
- I2C HAL
- SPI HAL
- UART HAL
- Flash HAL
- Restart HAL
- Watchdog HAL

#### Changed

- Converted Platform from class-based interfaces to namespace-based APIs.
- Removed unnecessary runtime state.
- Removed dynamic memory allocation.
- Isolated Arduino and ESP-IDF implementation details inside Platform.

#### Architecture

- Drivers communicate only with Platform.
- Platform is the only layer allowed to access Arduino APIs.
- Platform is the only layer allowed to access ESP-IDF APIs.
- Hardware implementation is isolated inside Platform.
