# NAS Controller Firmware - Changelog

All notable changes to this project will be documented in this file.

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
