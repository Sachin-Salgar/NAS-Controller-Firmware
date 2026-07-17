# Changelog

## Unreleased

---

# Core Layer

## Added

- Result framework
- ResultCode enumeration
- BuildInfo module
- Version module
- Event system
- EventBus
- Logger
- Scheduler
- SchedulerTask
- SchedulerTaskId

## Architecture

- Core is completely hardware independent.
- Core contains no Arduino dependencies.
- Core contains no ESP-IDF dependencies.
- Core provides the common foundation used by every higher firmware layer.
- Core is frozen for Firmware Version 1.x.

---

# Platform Hardware Abstraction Layer

## Added

- PlatformManager
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

## Changed

- Converted Platform from class-based interfaces to namespace-based APIs.
- Removed unnecessary runtime state.
- Removed dynamic memory allocation.
- Isolated Arduino and ESP-IDF implementation details inside Platform.

## Architecture

- Drivers communicate only with Platform.
- Platform is the only layer allowed to access Arduino APIs.
- Platform is the only layer allowed to access ESP-IDF APIs.
- Hardware implementation is isolated inside Platform.
- Platform is frozen for Firmware Version 1.x.

---

# Drivers Layer

## Added

- DriverManager
- RelayDriver
- FanDriver
- TemperatureDriver
- LedDriver
- DriveDriver
- PersistentStorageDriver

## Architecture

- Drivers contain hardware-specific logic.
- Drivers use Platform HAL exclusively.
- Drivers never communicate directly with Services.
- Drivers expose hardware through clean interfaces.
- Drivers are frozen for Firmware Version 1.x.

---

# Objects Layer

## Added

- ObjectManager
- Relay
- Fan
- TemperatureSensor
- Led
- Drive
- Configuration
- Statistics
- SystemStatus

## Architecture

- Objects contain only firmware state.
- Objects contain no hardware access.
- Objects contain no protocol logic.
- Objects provide the shared data model used by Services.
- Objects are frozen for Firmware Version 1.x.

---

# Services Layer

## Added

- ServiceManager
- RelayService
- FanService
- TemperatureService
- LedService
- DriveService
- ConfigurationService
- StatisticsService
- SystemService
- USBService
- ProtocolService
- SchedulerService
- HealthService
- EventService

## Architecture

- Services implement firmware business logic.
- Services coordinate Drivers and Objects.
- Services never access hardware directly.
- Services communicate through defined interfaces.
- Services are frozen for Firmware Version 1.x.

---

# Protocol Layer

## Added

- Commands
- PacketValidator
- PacketParser
- PacketBuilder
- ResponseBuilder
- CommandDispatcher

## Architecture

- Protocol is responsible only for packet processing.
- Protocol is transport independent.
- Protocol communicates only with Services.
- Protocol contains no hardware-specific implementation.
- Protocol is frozen for Firmware Version 1.x.

---

# System Layer

## Added

- Startup
- Application
- SystemManager
- Firmware startup sequence
- Main application loop

## Architecture

- Defines firmware initialization order.
- Owns complete firmware lifecycle.
- Coordinates startup and runtime execution.
- System layer is frozen for Firmware Version 1.x.

---

# Configuration Layer

## Added

- Features configuration
- Hardware configuration
- Protocol configuration
- Task configuration
- Build configuration

## Architecture

- Header-only constexpr configuration.
- Centralized compile-time configuration.
- No runtime allocation.
- Configuration layer is frozen for Firmware Version 1.x.

---

# Utilities Layer

## Added

- CRC16
- ByteBuffer
- BitHelpers
- StringHelpers
- TimeHelpers

## Changed

- Centralized CRC implementation for Protocol layer.
- Added reusable helper library for all firmware layers.

## Architecture

- Utilities are hardware independent.
- Utilities contain reusable helper functions only.
- Utilities are available to every firmware layer.
- Utilities layer is frozen for Firmware Version 1.x.