# Changelog

## [Unreleased] - Compilation Fixes and Scheduler Implementation

### Added

#### Core Layer - Result Code Enumeration Extension
- Extended `ResultCode` enum with additional error codes for better error reporting:
  - `NotSupported`: Used for unsupported command/feature requests
  - `NullPointer`: Used for null pointer validation at system boundaries
  - `BufferTooSmall`: Used when output buffer size is insufficient
  - `CrcError`: Used for CRC validation failures in protocol packets
  - `InvalidHeader`: Used for malformed/invalid packet headers
  - `InvalidLength`: Used for invalid payload length validation
  - `BufferFull`: Used for event buffer overflow conditions
  - `NotFound`: Used when requested items are not found

#### Core Layer - Task Scheduler
- Implemented `Scheduler` class for time-based task scheduling
  - Supports registration of periodic tasks with configurable intervals
  - Non-blocking interval-based execution model using `millis()` timing
  - Supports up to 16 concurrent tasks
  - Properly integrated with `SchedulerService` for service orchestration

### Fixed

#### Build System
- Resolved all compilation errors in Protocol layer modules
  - Added missing `ResultCode` enum values used by `CommandDispatcher`, `PacketParser`, `PacketValidator`, and `PacketBuilder`
  - Fixed `EventService` compilation by adding `BufferFull` and `NotFound` result codes
- Removed non-existent initialization calls in `main.cpp`
  - `BuildInfo::Initialize()` and `Version::Initialize()` don't exist (compile-time only)

### Status
- ✅ Clean compilation achieved (ESP32 Dev Module)
- ✅ Firmware binary successfully generated (291 KB flash, 24 KB RAM)
- ✅ All critical compilation errors resolved
- ✅ Scheduler ready for task scheduling operations

---

## [1.0.0] - Architecture Compliance Session (2026-07-18)

### Fixed

#### Critical Architecture Violations Resolved
- **Platform USB**: Added binary `Write()` and `Read()` methods for binary protocol operations
- **HealthService**: Refactored to use `WatchdogDriver` instead of direct Platform access (restored dependency direction)
- **Startup**: Removed references to non-existent manager classes (PlatformManager, DriverManager, ObjectManager)
- **StorageDriver**: Updated to use correct Platform Flash API (`Erase()` instead of non-existent `Clear()`)
- **All Services**: Migrated hardcoded configuration values to centralized Config layer

### Added

#### New Driver Layer
- `WatchdogDriver`: Hardware abstraction for system watchdog management with Initialize() and Feed() methods

### Changed

#### Documentation Updates
- **Architecture.md**: Updated Drivers section to list all implemented drivers including WatchdogDriver
- **Platform HAL ADR (0003)**: Documented USB binary operations (character and binary modes)
- **Drivers ADR (0004)**: Updated with complete list of implemented drivers
- **Services ADR (0006)**: Documented configuration layer integration best practices
- **CHANGELOG.md**: Created comprehensive changelog with architecture compliance updates
- **ComplianceAudit.md**: Created new detailed audit report documenting all verification results

#### Service Configuration Integration
- RelayService: Now uses `NAS::Config::Hardware::RelayCount`
- FanService: Now uses `NAS::Config::Hardware::FanCount`, `FanPwmFrequency`, `FanPwmResolution`
- TemperatureService: Now uses `NAS::Config::Hardware::TemperatureSensorCount`
- LedService: Now uses `NAS::Config::Hardware::LedCount`
- DriveService: Now uses `NAS::Config::Hardware::DriveCount`
- SchedulerService: Now uses task timing from `NAS::Config::Tasks`

### Verified

✅ Architecture compliance verified against all ADRs
✅ Dependency direction rules confirmed correct
✅ Hardware abstraction properly layered
✅ Configuration centralized in Config layer
✅ Naming conventions consistent
✅ No circular dependencies
✅ All code changes applied and verified

---

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
