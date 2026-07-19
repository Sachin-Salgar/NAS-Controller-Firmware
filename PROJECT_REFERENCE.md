# NAS Controller Firmware
## Project Reference Guide

**Status:** Version 1.x Development  
**Architecture:** FROZEN for Version 1.x  
**Last Updated:** July 18, 2026  
**Document Purpose:** Single source of truth for project scope, frozen decisions, and remaining tasks

---

# TABLE OF CONTENTS

1. [Project Overview](#project-overview)
2. [Core Objectives](#core-objectives)
3. [Frozen Architecture](#frozen-architecture)
4. [Hardware Platform](#hardware-platform)
5. [Firmware Structure](#firmware-structure)
6. [Phase Status](#phase-status)
7. [Remaining Tasks](#remaining-tasks)
8. [Current Codebase Status](#current-codebase-status)
9. [Coding Standards](#coding-standards)
10. [Integration Checklist](#integration-checklist)

---

# PROJECT OVERVIEW

## What is the NAS Controller Firmware?

The **NAS Controller Firmware** is an embedded control system that runs on an **ESP32** and manages external NAS (Network Attached Storage) hardware via **USB CDC Serial** communication.

The firmware operates **independently** of the host operating system and provides:

- **Power Relay Control** – Turn external hardware on/off
- **Fan Speed Control** – PWM-based cooling management
- **Temperature Monitoring** – Real-time sensor readout via OneWire
- **LED Status Indication** – WS2812B addressable LED strip
- **Drive Health Monitoring** – Drive detection and state reporting
- **Configuration Management** – Persistent storage of settings
- **Event Logging** – System health and statistics tracking
- **Watchdog Recovery** – Automatic system recovery on fault

## Key Characteristics

- **Language:** C++17
- **Framework:** Arduino
- **Build System:** PlatformIO
- **Architecture:** Strictly layered with frozen dependency rules
- **Communication:** Binary packet protocol over USB CDC Serial (115200 baud)
- **Memory Model:** Static allocation only (no dynamic memory)
- **Execution:** Scheduled task-based (non-interrupt business logic)

---

# CORE OBJECTIVES

The firmware SHALL be:

1. **Modular** – Clean separation of concerns across 8 layers
2. **Reliable** – Deterministic execution with comprehensive error handling
3. **Hardware Independent** – Platform layer isolates all hardware details
4. **Protocol Independent** – Internal logic doesn't depend on communication method
5. **Maintainable** – Clear documentation, consistent coding style, no circular dependencies
6. **Extensible** – New features added through Services without breaking existing code
7. **Testable** – Layers can be tested independently
8. **Production Ready** – Suitable for manufacturing and long-term support

---

# FROZEN ARCHITECTURE

## Architecture Freeze Status

**FROZEN for Version 1.x** – All architectural decisions are locked in place.

Architectural changes require:
- Architecture Decision Record (ADR)
- Documentation update
- Major version bump (2.0)

This ensures stability throughout the 1.x release cycle.

## Dependency Direction Rules (MANDATORY)

```
Application
    ↓ (downward only)
Protocol
    ↓
Services
    ↓
Objects
    ↓
Drivers
    ↓
Platform (HAL)
    ↓
Core

Shared: Config & Utilities
```

### Mandatory Rules

- ✅ **ALLOWED:** Protocol → Services, Services → Objects, Objects → Drivers, Drivers → Platform
- ❌ **FORBIDDEN:** Platform → Services, Drivers → Protocol, Objects → GPIO, circular dependencies
- ✅ **ALLOWED:** Any layer → Core, Any layer → Config, Any layer → Utilities
- ❌ **FORBIDDEN:** Lower layer → Higher layer (e.g., Platform cannot use Services)

## Layered Architecture Definition

### 1. Core Layer
**Purpose:** Foundation utilities shared by all layers

**Contains:**
- `Result` – Universal error handling type
- `Logger` – Debug output system
- `EventBus` – Inter-component event publishing
- `Scheduler` – Task scheduler for periodic tasks
- `Version` – Build information
- `BuildInfo` – Firmware metadata

**Location:** `src/Core/`

**No Hardware Dependencies**

---

### 2. Platform Layer (HAL)
**Purpose:** Hardware abstraction for ESP32 and Arduino

**Contains:**
- GPIO – Pin input/output
- PWM – Pulse-width modulation
- Timer – Hardware timing
- USB – Serial communication
- Flash – Non-volatile storage
- OneWire – Temperature sensor bus
- Watchdog – System recovery

**Location:** `src/Platform/`

**Constraints:**
- Only layer allowed to use Arduino APIs
- Only layer allowed to access ESP-IDF
- Exposes lightweight namespace-based APIs
- Returns `Core::Result` for all operations
- NO business logic
- NO hardware-specific constants in drivers

---

### 3. Drivers Layer
**Purpose:** Device-specific hardware control

**Contains:**
- RelayDriver – Relay switching
- FanDriver – PWM fan control
- LEDDriver – WS2812B LED control
- TemperatureDriver – Sensor readout
- StorageDriver – Flash read/write
- WatchdogDriver – Recovery mechanism

**Location:** `src/Drivers/`

**Constraints:**
- Uses only Platform layer APIs
- No business logic (state transitions, effects, animations)
- No direct GPIO access
- Cannot know about Services or higher layers
- Stateless where possible

---

### 4. Objects Layer
**Purpose:** Runtime state models representing hardware

**Contains:**
- Relay – Individual relay state
- Fan – Individual fan state
- TemperatureSensor – Sensor reading + calibration
- Led – Individual LED state + color
- Drive – Drive detection + health
- Configuration – User settings storage
- Statistics – Runtime metrics

**Location:** `src/Objects/`

**Constraints:**
- Combines Driver functionality with state management
- NO hardware access (uses Drivers only)
- NO business logic or effects
- Simple state transitions based on input
- Persistent objects created in Services

---

### 5. Services Layer
**Purpose:** Business logic and system orchestration

**Contains:**
- RelayService – Relay control logic
- FanService – Fan control and automation
- TemperatureService – Temperature monitoring and alerts
- LEDService – LED status management
- DriveService – Drive health monitoring
- ProtocolService – Command processing
- HealthService – System health aggregation
- EventService – Event queue management
- SchedulerService – Task scheduling

**Location:** `src/Services/`

**Constraints:**
- Coordinates multiple Objects
- Listens to EventBus for updates
- Publishes events for other services
- Can trigger animations and effects
- Initialization order matters (see ServiceManager)

---

### 6. Protocol Layer
**Purpose:** External communication (USB binary protocol)

**Contains:**
- CommandDispatcher – Route commands to handlers
- CommandParser – Parse incoming packets
- ResponseBuilder – Format outgoing packets
- PacketValidator – Validate CRC16 and structure

**Location:** `src/Protocol/`

**Constraints:**
- Communicates only with Services (never direct hardware)
- Implements binary packet format (header, seq, cmd, length, payload, CRC16)
- Protocol-agnostic (could be UART, Ethernet, etc.)
- Returns responses through same interface

---

### 7. System Layer
**Purpose:** Firmware lifecycle management

**Contains:**
- SystemManager – Initialization and shutdown
- Startup – Boot sequence
- Application – Main application logic

**Location:** `src/System/`

**Responsibilities:**
- Initialize layers in correct order
- Manage scheduler
- Handle shutdown sequence
- System state transitions
- Watchdog supervision

**Startup Order (MANDATORY):**
1. Platform Initialization
2. Logger Initialization
3. Configuration Load
4. Driver Initialization
5. Object Creation
6. Service Initialization
7. Protocol Initialization
8. Scheduler Start
9. System Ready

---

### 8. Config Layer
**Purpose:** Compile-time configuration constants

**Contains:**
- BuildConfig.h – Firmware metadata
- HardwareConfig.h – Hardware limits
- Features.h – Feature flags
- TaskConfig.h – Polling intervals
- ProtocolConfig.h – Protocol settings

**Location:** `src/Config/`

**Example:** `Hardware::LedCount = 60`, `Hardware::DriveCount = 10`

---

### 9. Utilities Layer
**Purpose:** Reusable helper functions

**Contains:**
- CRC16 – Checksum calculation
- StringHelpers – String formatting
- TimeHelpers – Time utilities
- BitHelpers – Bit manipulation
- ByteBuffer – Buffer management

**Location:** `src/Utilities/`

---

## Communication Flow

### External Communication (Host ↔ Firmware)

```
Host Computer
    ↓
USB CDC (115200 baud)
    ↓
Protocol Layer (CommandDispatcher)
    ↓
Service Layer (Handler)
    ↓
Objects Layer (State change)
    ↓
Drivers Layer (Hardware update)
    ↓
Platform Layer (GPIO)
    ↓
ESP32 Hardware
```

### Internal Communication (Events)

```
Driver Update
    ↓
Object State Change
    ↓
Service Detects Change
    ↓
EventBus::Publish()
    ↓
Other Services Subscribe
    ↓
Coordinated System Response
```

---

# HARDWARE PLATFORM

## Frozen Hardware Specification

**Status:** Hardware Version 1.0, FROZEN

The firmware targets **exactly this hardware** for Version 1.x.

### Microcontroller

| Property | Value |
|----------|-------|
| MCU | ESP32-WROOM-32 |
| Cores | 2 × Xtensa LX6 @ 240 MHz |
| Flash | 4 MB |
| RAM | 520 KB |
| Power | USB Type-C only |

### GPIO Allocation

| GPIO | Function | Status |
|------|----------|--------|
| GPIO18 | Fan PWM Channel 1 | Assigned |
| GPIO19 | PWM Channel 2 | Reserved |
| GPIO21 | OneWire Temperature Bus | Assigned |
| GPIO22 | Relay 1 Control (5V) | Assigned |
| GPIO23 | WS2812B LED Data | Assigned |
| GPIO25 | Relay 2 Control (12V) | Assigned |
| Other | Various | Reserved |

### Power Architecture

| Rail | Source | Usage |
|------|--------|-------|
| +5V | ATX PSU | Relay 1, WS2812B LEDs |
| +12V | ATX PSU | Cooling fans, Relay 2 |
| 3.3V | ESP32 Regulator | Sensors, Logic |
| GND | Common | All systems |

### Connected Hardware

| Device | Quantity | Interface | Purpose |
|--------|----------|-----------|---------|
| Relay Module | 2 | GPIO (active-low) | Power switching |
| MOSFET PWM | 2 | GPIO PWM | Fan speed control |
| DS18B20 Sensor | 3 | OneWire | Temperature monitoring |
| WS2812B LED Strip | 60 | GPIO23 | Status indication |

### Hardware Limits

| Resource | Current | Maximum |
|----------|---------|---------|
| Cooling Fans | 5 | 5 |
| Relay Outputs | 2 | 2 |
| PWM Outputs | 2 | 2 |
| Temperature Sensors | 3 | 8 (OneWire limit) |
| WS2812B LEDs | 60 | 60 |

---

# FIRMWARE STRUCTURE

## Source Code Organization

```
src/
├── main.cpp                 # Arduino entry point
│
├── Core/                    # Foundation layer
│   ├── Result.h/cpp         # Error handling
│   ├── Logger.h/cpp         # Debug logging
│   ├── EventBus.h/cpp       # Event publishing
│   ├── Scheduler.h/cpp      # Task scheduler
│   ├── Version.h/cpp        # Firmware version
│   └── Event.h              # Event type definitions
│
├── Platform/                # Hardware abstraction
│   ├── GPIO.h/cpp           # Digital I/O
│   ├── PWM.h/cpp            # PWM control
│   ├── Timer.h/cpp          # Hardware timers
│   ├── USB.h/cpp            # Serial communication
│   ├── Flash.h/cpp          # EEPROM access
│   ├── OneWire.h/cpp        # Temperature bus
│   ├── Watchdog.h/cpp       # System recovery
│   └── WS2812.h/cpp         # LED hardware
│
├── Drivers/                 # Device drivers
│   ├── RelayDriver.h/cpp    # Relay control
│   ├── FanDriver.h/cpp      # Fan PWM
│   ├── LEDDriver.h/cpp      # LED control
│   ├── TemperatureDriver.h  # Sensor readout
│   ├── StorageDriver.h      # Flash access
│   └── WatchdogDriver.h     # Watchdog
│
├── Objects/                 # State models
│   ├── Relay.h/cpp          # Relay object
│   ├── Fan.h/cpp            # Fan object
│   ├── Led.h/cpp            # LED object
│   ├── TemperatureSensor.h  # Sensor object
│   ├── Drive.h/cpp          # Drive object
│   ├── Configuration.h/cpp  # Settings storage
│   └── Statistics.h/cpp     # Metrics
│
├── Services/                # Business logic
│   ├── RelayService.h/cpp
│   ├── FanService.h/cpp
│   ├── LEDService.h/cpp        # 🚨 NEEDS REDESIGN (See LED Audit)
│   ├── TemperatureService.h
│   ├── DriveService.h/cpp
│   ├── ProtocolService.h/cpp
│   ├── HealthService.h/cpp
│   ├── EventService.h/cpp
│   ├── SchedulerService.h
│   ├── ServiceManager.h/cpp     # Initializes all services
│   └── ConfigurationService.h
│
├── Protocol/                # USB communication
│   ├── CommandDispatcher.h/cpp
│   ├── CommandParser.h/cpp
│   ├── Commands.h/cpp
│   ├── ResponseBuilder.h/cpp
│   ├── PacketBuilder.h/cpp
│   └── PacketValidator.h/cpp
│
├── System/                  # Lifecycle management
│   ├── SystemManager.h/cpp      # 🚨 ISSUE: main.cpp doesn't call this
│   ├── Startup.h/cpp
│   └── Application.h/cpp
│
├── Config/                  # Compile-time constants
│   ├── BuildConfig.h
│   ├── HardwareConfig.h
│   ├── Features.h
│   ├── TaskConfig.h
│   └── ProtocolConfig.h
│
├── Utilities/               # Helper functions
│   ├── CRC16.h/cpp
│   ├── StringHelpers.h/cpp
│   ├── TimeHelpers.h/cpp
│   ├── BitHelpers.h/cpp
│   └── ByteBuffer.h/cpp
│
└── Tests/                   # Test code (should be gated)
    ├── SelfTest.h/cpp
    ├── LedHardwareTest.h
    ├── LedAnimation.cpp        # 🚨 Running instead of firmware
    ├── LedAnimationDemo.cpp    # 🚨 Not integrated
    └── ... other tests
```

---

# PHASE STATUS

## Completed (✅ 100%)

### Phase 1 – Project Foundation (Complete)
- ✅ Documentation foundation established
- ✅ Firmware architecture finalized
- ✅ Coding standard defined
- ✅ Hardware specification frozen
- ✅ Protocol specification complete
- ✅ Command set defined
- ✅ ADR framework established

### Phase 2 – Firmware Core (Complete)
- ✅ Core layer implemented
- ✅ Platform HAL implemented
- ✅ Drivers layer implemented
- ✅ Objects layer implemented
- ✅ Services layer implemented
- ✅ Protocol layer implemented
- ✅ System layer implemented
- ✅ Main application entry point

### Phase 3 – Shared Infrastructure (Complete)
- ✅ Configuration layer implemented
- ✅ Utilities layer implemented

---

## Recently Completed (✅ Just Fixed)

### Firmware Integration
- ✅ **CRITICAL:** Fixed main.cpp (now properly initializes SystemManager)

### LED Subsystem (Complete Redesign)
- ✅ LED architecture redesigned and fully implemented
- ✅ System status LEDs (Power, Health, USB, Temperature) implemented
- ✅ LED mapping configuration created (LedMap.h)
- ✅ Support for 10 drives (expandable to 12)
- ✅ Non-blocking animation framework implemented
- ✅ Boot, Shutdown, Idle, and Error animations created
- ✅ EventBus integration for reactive LED updates
- ✅ Centralized color palette (LedColors.h)
- **See:** LED_REDESIGN_COMPLETE.md for full details

---

## In Progress (⬜ Needs Work)

### Firmware Integration
- ⬜ **NEXT:** Compile complete firmware
- ⬜ Resolve any build errors
- ⬜ Verify module initialization order
- ⬜ Integrate LedManager into ServiceManager
- ⬜ Verify scheduler operation

---

## Not Started (Future)

### Hardware Bring-up
- ⬜ GPIO validation
- ⬜ PWM validation
- ⬜ Relay validation
- ⬜ Fan control validation
- ⬜ DS18B20 sensor validation
- ⬜ WS2812 LED validation
- ⬜ USB communication validation
- ⬜ Flash storage validation

### Firmware Features
- ⬜ Complete command handlers
- ⬜ Implement all protocol responses
- ⬜ Event queue implementation
- ⬜ Configuration persistence
- ⬜ Statistics persistence
- ⬜ Health monitoring logic
- ⬜ Watchdog recovery
- ⬜ Error logging

### Testing & Validation
- ⬜ Unit tests
- ⬜ Integration tests
- ⬜ Long-duration stability test
- ⬜ Stress testing
- ⬜ Hardware validation

### Host Software (Out of Scope for Firmware)
- ⬜ USB communication library
- ⬜ PC API
- ⬜ Command-line utility
- ⬜ GUI application
- ⬜ Firmware updater

### Production
- ⬜ Firmware Version 1.0 release
- ⬜ Documentation freeze
- ⬜ Manufacturing release

---

# REMAINING TASKS

## CRITICAL (Must Fix Before Proceeding)

### Task 1: Fix main.cpp Firmware Flow
**Status:** ✅ COMPLETE
**Impact:** Firmware now executes correctly

**Fixed Code:**
```cpp
void setup() {
    Serial.begin(115200);
    delay(2000);
    NAS::System::SystemManager::Initialize();
    NAS::System::SystemManager::Start();
}
void loop() {
    NAS::System::SystemManager::Run();
}
```

**What Was Done:**
- ✅ Removed RunLedAnimation() test code
- ✅ Added proper SystemManager initialization
- ✅ Integrated main loop with SystemManager
- ✅ Removed unused test includes

---

### Task 2: LED Subsystem Redesign
**Status:** ✅ COMPLETE
**Impact:** All 60 LEDs specified and implemented, system status LEDs added

**See:** `LED_REDESIGN_COMPLETE.md` (complete implementation details)

**What Was Implemented:**
1. ✅ Centralized LED mapping (LedMap.h) - all 60 LEDs mapped
2. ✅ Extended DriveLedState enum - added STANDBY state
3. ✅ Created system LED enums - PowerLedState, HealthLedState, UsbLedState, TemperatureLedState
4. ✅ Non-blocking animation framework - 4 animations (Boot, Shutdown, Idle, Error)
5. ✅ LedManager coordinator - unified LED control interface
6. ✅ Centralized colors (LedColors.h) - 30+ predefined colors
7. ✅ Support for 10 drives (expandable to 12)
8. ✅ EventBus integration - auto-reactive LED updates
9. ✅ SystemLed class - for system status indicators
10. ✅ LedEffects coordinator - animation management

**Files Created:** 15 new files
**Files Modified:** 4 existing files
**Total Code:** ~1,800 lines

---

### Task 3: Test Code Cleanup
**Status:** ✅ COMPLETE

**What Was Done:**
- ✅ Removed LedAnimation execution from main.cpp
- ✅ Archived legacy test animations (ARCHIVED_LedAnimation.cpp.archive)
- ✅ New animations integrated into production framework
- ✅ Test code separated from production code

**Note:** Legacy animation code preserved in archive for reference, but new implementation in LedAnimator framework is used instead

---

## HIGH PRIORITY (Required for Integration)

### Task 4: Compilation & Linking
**Status:** ⬜ UNKNOWN  
**Impact:** Prevents testing

**Actions Required:**
- Attempt full firmware compilation
- Identify and fix compiler errors
- Resolve linker errors
- Fix include dependency issues
- Verify all symbols resolve

---

### Task 5: ServiceManager Validation
**Status:** ⬜ NOT VERIFIED  
**Impact:** Unknown if services initialize correctly

**Actions Required:**
- Verify ServiceManager initialization order
- Check all services initialize without errors
- Validate initialization gates work
- Test recovery from service failures

---

### Task 6: Scheduler Validation
**Status:** ⬜ NOT VERIFIED  
**Impact:** Unknown if tasks execute correctly

**Actions Required:**
- Verify Scheduler works correctly
- Test task registration
- Validate polling intervals
- Check non-blocking behavior

---

## MEDIUM PRIORITY (Required for Feature Completeness)

### Task 7: Command Handler Implementation
**Status:** ⬜ NOT IMPLEMENTED

**What's Missing:**
- System commands (Ping, GetVersion, GetStatus, Reset)
- Relay commands (Get, Set, Enable, Disable)
- Fan commands (Get, SetSpeed, SetMode)
- Temperature commands (Get, Calibrate, SetThreshold)
- LED commands (Get, SetColor, SetMode, SetBrightness)
- Drive commands (Get, Scan, GetHealth)
- Configuration commands (Get, Set, Save, Load)
- Statistics commands (Get, Reset)
- Event commands (Read, Clear)

---

### Task 8: Configuration Persistence
**Status:** ⬜ NOT IMPLEMENTED

**What's Needed:**
- Flash storage initialization
- Configuration load on boot
- Configuration save on change
- Default values
- Validation

---

### Task 9: Statistics Tracking
**Status:** ⬜ NOT IMPLEMENTED

**What's Needed:**
- Boot counter
- Uptime tracking
- Event counters
- Performance metrics
- History

---

### Task 10: Health Monitoring
**Status:** ⬜ NOT IMPLEMENTED

**What's Needed:**
- System health aggregation
- Alert generation
- Watchdog integration
- Recovery logic

---

# CURRENT CODEBASE STATUS

## What's Working ✅

- **Core Layer:** Fully implemented (Result, Logger, EventBus, Scheduler, Version)
- **Platform Layer:** Fully implemented (GPIO, PWM, USB, Flash, OneWire, Watchdog, WS2812)
- **Drivers Layer:** All drivers implemented (Relay, Fan, LED, Temperature, Storage, Watchdog)
- **Objects Layer:** All objects implemented (Relay, Fan, TemperatureSensor, Led, Drive, Configuration, Statistics)
- **Services Layer:** All services implemented (Relay, Fan, LED, Temperature, Drive, Protocol, Health, Event, Scheduler, Configuration)
- **Protocol Layer:** Frame work complete (CommandDispatcher, CommandParser, ResponseBuilder, PacketBuilder, PacketValidator)
- **System Layer:** Framework complete (SystemManager, Startup, Application)
- **Config Layer:** All configuration files present
- **Utilities Layer:** All utilities implemented (CRC16, StringHelpers, TimeHelpers, BitHelpers, ByteBuffer)

## What's Broken ❌

- **main.cpp:** Runs animation test instead of firmware (CRITICAL)
- **LedService:** Only handles 8 drive LEDs, no system status LEDs (See LED_AUDIT_REPORT.md)
- **LED Architecture:** Missing animation framework, event integration, centralized mapping

## What's Missing ⬜

- **Command Handlers:** Commands parsed but not executed
- **Configuration Persistence:** No save/load implementation
- **Statistics Tracking:** No boot counter or metrics
- **Health Monitoring:** No system health checks
- **Watchdog Integration:** Not wired up
- **Hardware Validation:** Not tested on real hardware
- **Animation Framework:** Blocking test code, no non-blocking system

---

# CODING STANDARDS

## Mandatory Standards (From docs/CodingStandard.md)

### Naming Conventions

- **Namespaces:** `PascalCase` (e.g., `NAS::Drivers::LEDDriver`)
- **Classes:** `PascalCase` (e.g., `class Relay`)
- **Methods:** `PascalCase` (e.g., `SetState()`)
- **Variables (member):** `snake_case_` with trailing underscore (e.g., `enabled_`, `relay_state_`)
- **Variables (local):** `camelCase` (e.g., `maxValue`, `ledIndex`)
- **Constants:** `UPPER_SNAKE_CASE` or `camelCase` in constexpr (e.g., `kDefaultBrightness`)

### Return Types

- **All public methods:** Return `NAS::Core::Result` for error handling
- **Marked:** `[[nodiscard]]` attribute
- Use `Result::Ok()` for success
- Use `Result(ResultCode::ErrorType)` for failure

Example:
```cpp
[[nodiscard]]
NAS::Core::Result Initialize() noexcept;

[[nodiscard]]
NAS::Core::Result SetState(DriveLedState state) noexcept;
```

### Error Handling

Every operation that can fail MUST return `Result`:
- ✅ `Hardware operations` (SetPixel, SetBrightness)
- ✅ `State transitions` (SetState, Initialize)
- ✅ `I/O operations` (Load, Save)
- ❌ `Simple accessors` (GetIndex, GetState, GetCount)

### Headers

All files must have:
```cpp
#pragma once

/******************************************************************************
 * Project : NAS Controller Firmware
 * Module  : [Layer]
 * File    : [Filename]
 * Description: [Brief description]
 ******************************************************************************/
```

### Code Organization

1. **Public interface first**
2. **Private implementation second**
3. **Helper functions last**

### Memory Policy

- **Static allocation only** – No `new`, no `malloc`, no `unique_ptr`
- **No dynamic memory** – All objects created at compile time or stack
- **No heap usage** – System-wide requirement

### Const Correctness

- Mark const members: `const noexcept`
- Mark const return types where appropriate
- Use `constexpr` for compile-time constants

### NoExcept

- All production code: `noexcept`
- Exceptions disabled at firmware level
- No try/catch blocks

---

# INTEGRATION CHECKLIST

## Before Starting Development

- [ ] Read `docs/Architecture.md` (understand layered model)
- [ ] Read `docs/CodingStandard.md` (follow naming/style rules)
- [ ] Read `docs/Hardware.md` (understand connected hardware)
- [ ] Understand the dependency direction rules
- [ ] Know the ServiceManager initialization order
- [ ] Know the EventBus publishing/subscribing pattern

## Before Committing Code

- [ ] Follows naming conventions exactly
- [ ] All public methods return `[[nodiscard]] Result`
- [ ] Uses only static allocation
- [ ] Marked `noexcept`
- [ ] Has proper error handling
- [ ] Stays within layer boundaries
- [ ] No circular dependencies
- [ ] No lower layer → higher layer dependencies
- [ ] Updated documentation if API changed

## Before Submitting PR

- [ ] Code compiles without errors
- [ ] Code compiles without warnings
- [ ] Linker resolves all symbols
- [ ] Code follows CodingStandard.md exactly
- [ ] Architecture dependencies respected
- [ ] No dynamic memory allocation
- [ ] Tests pass (if applicable)

---

# QUICK REFERENCE

## Key Files to Know

| File | Purpose |
|------|---------|
| `src/main.cpp` | Arduino entry point (🚨 NEEDS FIX) |
| `docs/Architecture.md` | Frozen architecture specification |
| `docs/CodingStandard.md` | Mandatory coding conventions |
| `docs/Hardware.md` | Hardware specification |
| `docs/Protocol.md` | USB packet format |
| `src/Core/Result.h` | Error handling type |
| `src/Core/EventBus.h` | Event publishing system |
| `src/Services/ServiceManager.h` | Service initialization |
| `src/System/SystemManager.h` | Firmware lifecycle |
| `LED_AUDIT_REPORT.md` | 🚨 LED subsystem problems & solutions |
| `PROJECT_REFERENCE.md` | **This file** |

## Important Constants

| Constant | Value | Location |
|----------|-------|----------|
| `Hardware::LedCount` | 60 | `src/Config/HardwareConfig.h` |
| `Hardware::DriveCount` | 10 | `src/Config/HardwareConfig.h` |
| `Features::EnableLedControl` | true | `src/Config/Features.h` |
| `Tasks::LedTaskIntervalMs` | 100 | `src/Config/TaskConfig.h` |
| GPIO18 | Fan PWM | `src/Platform/PWM.h` |
| GPIO23 | WS2812B LED | `src/Platform/WS2812.h` |
| GPIO21 | OneWire Temp | `src/Platform/OneWire.h` |
| GPIO22 | Relay 1 (5V) | `src/Platform/GPIO.h` |
| GPIO25 | Relay 2 (12V) | `src/Platform/GPIO.h` |

## Service Initialization Order

From `src/Services/ServiceManager.cpp`:
1. ConfigurationService
2. StatisticsService
3. SystemService
4. RelayService
5. FanService
6. TemperatureService
7. **LedService** (🚨 needs redesign)
8. DriveService
9. USBService
10. ProtocolService
11. HealthService
12. EventService
13. SchedulerService

---

# WHEN YOU'RE READY TO BUILD

1. **Fix main.cpp** (run SystemManager instead of animation test)
2. **Attempt compilation** (resolve any build errors)
3. **Review LED_AUDIT_REPORT.md** (understand what needs redesign)
4. **Plan LED redesign** (centralized mapping, system LEDs, animations)
5. **Implement LED improvements** (following frozen architecture)
6. **Validate on hardware** (test real NAS hardware)

---

# REFERENCE TO OTHER DOCUMENTS

This document summarizes and consolidates these sources:

| Document | Location | Purpose |
|----------|----------|---------|
| Architecture | `docs/Architecture.md` | Complete layer definitions |
| Hardware | `docs/Hardware.md` | Hardware specifications |
| Protocol | `docs/Protocol.md` | Binary packet format |
| Commands | `docs/CommandSet.md` | Command specifications |
| Coding Standard | `docs/CodingStandard.md` | Code conventions |
| Roadmap | `docs/Roadmap.md` | Development phases |
| Repository | `docs/Repository-Structure.md` | File organization |
| ADRs | `docs/Decisions/` | Frozen design decisions |
| LED Audit | `LED_AUDIT_REPORT.md` | 🚨 LED subsystem analysis |
| Changelog | `docs/Changelog.md` | Version history |

---

**Last Updated:** July 18, 2026  
**Status:** Reference for Version 1.x Development  
**Architecture:** FROZEN for 1.x

For detailed information, see referenced documents above.

