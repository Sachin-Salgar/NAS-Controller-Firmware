# NAS Controller Firmware - Changelog

All notable changes to this project will be documented in this file.

## [Unreleased] - Compilation Fixes

### Added

#### Core Layer - Result Code Enumeration Extension
- Added `NotSupported` result code for unsupported operations/commands
- Added `NullPointer` result code for null pointer parameter validation
- Added `BufferTooSmall` result code for insufficient buffer size conditions
- Added `CrcError` result code for CRC validation failures
- Added `InvalidHeader` result code for malformed packet headers
- Added `InvalidLength` result code for invalid packet length validation
- Added `BufferFull` result code for buffer overflow conditions
- Added `NotFound` result code for not-found conditions in data structures

#### Core Layer - Task Scheduler Implementation
- Created `Scheduler.h`: Header file defining task scheduler interface
  - `Initialize()`: Initialize the scheduler
  - `AddTask()`: Register periodic tasks with interval-based execution
  - `Execute()`: Process all scheduled tasks based on elapsed time
- Created `Scheduler.cpp`: Implementation of task scheduler
  - Simple time-based task execution using Arduino `millis()`
  - Support for up to 16 concurrent tasks
  - Non-blocking interval-based execution

### Fixed

#### Build System - Missing Enum Values
- Fixed compilation errors in `Protocol/CommandDispatcher.cpp` by adding missing `NotSupported` result code
- Fixed compilation errors in `Protocol/PacketParser.cpp` by adding missing `NotSupported` result code
- Fixed compilation errors in `Protocol/PacketBuilder.cpp` by adding missing `NullPointer` and `BufferTooSmall` result codes
- Fixed compilation errors in `Protocol/PacketValidator.cpp` by adding missing `InvalidHeader`, `InvalidLength`, and `CrcError` result codes
- Fixed compilation errors in `Services/EventService.cpp` by adding missing `BufferFull` and `NotFound` result codes

#### System Layer - Initialization
- Removed unnecessary `BuildInfo::Initialize()` call from `main.cpp` (BuildInfo contains only compile-time constants)
- Removed unnecessary `Version::Initialize()` call from `main.cpp` (Version is a compile-time interface)

### Verified

- ✅ All compilation errors resolved
- ✅ Firmware successfully builds for ESP32 (esp32dev)
- ✅ Memory usage: RAM 7.3%, Flash 22.2%
- ✅ No blocking compilation warnings in project code
- ✅ Task scheduler properly integrated with SchedulerService

---

## [1.0.0] - Architecture Compliance Session

### Added

#### Platform Layer - USB Binary Operations
- Added binary `Write(const uint8_t* data, size_t length)` method to USB platform abstraction
- Added binary `Read(uint8_t* data, size_t bufferSize, size_t& bytesReceived)` method to USB platform abstraction
- Enables USBService to perform binary protocol operations through proper abstraction layer

#### Drivers Layer - Watchdog Abstraction
- Created `WatchdogDriver` class for watchdog hardware abstraction
- Implements `Initialize()` and `Feed()` methods
- Properly abstracts watchdog access between Platform and Services layers

### Changed

#### Architecture Compliance Fixes
- **HealthService**: Refactored to use `WatchdogDriver` instead of direct Platform access
  - Maintains dependency direction (Services → Drivers → Platform)
  - Follows proper hardware abstraction pattern

- **Startup**: Removed references to non-existent manager classes
  - Cleaned up `PlatformManager`, `DriverManager`, `ObjectManager` references
  - ServiceManager now handles all initialization responsibilities

- **Services Layer - Configuration Integration**
  - RelayService: Now uses `NAS::Config::Hardware::RelayCount`
  - FanService: Now uses `NAS::Config::Hardware::FanCount`, `FanPwmFrequency`, `FanPwmResolution`
  - TemperatureService: Now uses `NAS::Config::Hardware::TemperatureSensorCount`
  - LedService: Now uses `NAS::Config::Hardware::LedCount`
  - DriveService: Now uses `NAS::Config::Hardware::DriveCount`
  - SchedulerService: Now uses task timing configuration from `NAS::Config::Tasks`

- **StorageDriver**: Updated to use correct Platform Flash API
  - Changed from non-existent `Clear()` to proper `Erase()` method
  - Maintains correct address-based Flash API usage

### Verified

- ✅ All code changes applied correctly
- ✅ Architecture compliance verified against documented design
- ✅ Dependency directions maintained (no upward dependencies)
- ✅ Hardware abstraction properly layered
- ✅ Configuration layer properly integrated
- ✅ No circular dependencies introduced

### Documentation Updated

- Architecture.md: Added WatchdogDriver to Drivers layer documentation
- Drivers ADR (0004): Documented WatchdogDriver implementation
- Services ADR (0006): Documented service configuration improvements
- Platform HAL ADR (0003): Documented binary USB operations
- Created ComplianceAudit.md: Comprehensive verification report
