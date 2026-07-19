# NAS Controller Firmware - Quick Start Reference

**Use this when you need fast answers without reading full documents.**

---

## WHAT IS THIS PROJECT?

Embedded firmware for ESP32 that manages external NAS hardware:
- Power control (relays)
- Cooling fans (PWM)
- Temperature monitoring (OneWire)
- LED status lights (WS2812B)
- Drive health tracking
- USB communication

---

## CRITICAL FACTS

✅ **FIRMWARE FIXED:** main.cpp now calls SystemManager correctly
✅ **LED SYSTEM COMPLETE:** All 60 LEDs designed, animations, system status LEDs implemented
🟡 **NOT COMPILED:** Build status unknown, needs verification
🟡 **NOT TESTED:** No hardware validation yet  

---

## FROZEN ARCHITECTURE (CANNOT CHANGE)

```
Application
    ↓ (only downward)
Protocol
    ↓
Services
    ↓
Objects
    ↓
Drivers
    ↓
Platform (Hardware)
    ↓
Core (Utilities)
```

**Rule:** No upward dependencies. Ever.

---

## MUST-READ DOCUMENTS (In Order)

1. **PROJECT_REFERENCE.md** ← Start here
2. **docs/Architecture.md** ← Layer definitions
3. **docs/CodingStandard.md** ← Code rules
4. **LED_AUDIT_REPORT.md** ← If working on LEDs

---

## CRITICAL FIXES COMPLETED

### #1: Fix main.cpp ✅ COMPLETE

main.cpp has been fixed to properly initialize and run SystemManager.

**What was changed:**
- Removed test code that ran RunLedAnimation() forever
- Added proper SystemManager initialization in setup()
- Added SystemManager run loop in loop()
- Removed unused Tests/SelfTest.h include

### #2: Fix LED System ✅ COMPLETE

Complete redesign with all features implemented:
- ✅ LedMap.h (centralized LED indices for all 60 LEDs)
- ✅ System status LEDs (Power, Health, USB, Temperature) fully implemented
- ✅ Support for 10 drives (expandable to 12)
- ✅ Non-blocking animation framework (Boot, Shutdown, Idle, Error)
- ✅ EventBus integration for reactive updates
- ✅ Test code archived separately

**See LED_REDESIGN_COMPLETE.md for implementation details**

### #3: Test Compilation (NEXT)

```bash
platformio run -e esp32dev
```

Verify build succeeds before proceeding to hardware testing.

---

## LAYER QUICK REFERENCE

| Layer | Purpose | Location | Key Class |
|-------|---------|----------|-----------|
| **Core** | Foundation (no HW) | `src/Core/` | `Result`, `EventBus`, `Scheduler` |
| **Platform** | Hardware HAL | `src/Platform/` | `GPIO`, `PWM`, `USB`, `WS2812` |
| **Drivers** | Device control | `src/Drivers/` | `RelayDriver`, `FanDriver`, `LEDDriver` |
| **Objects** | State models | `src/Objects/` | `Relay`, `Fan`, `Led`, `Drive` |
| **Services** | Business logic | `src/Services/` | `RelayService`, `FanService`, `LEDService` |
| **Protocol** | USB packets | `src/Protocol/` | `CommandDispatcher` |
| **System** | Lifecycle | `src/System/` | `SystemManager` |

---

## CODE STYLE RULES

**Mandatory:**
- Classes: `PascalCase` → `class RelayDriver`
- Methods: `PascalCase` → `GetState()`
- Members: `snake_case_` → `state_`, `enabled_`
- Locals: `camelCase` → `ledIndex`, `maxValue`
- Constants: `kName` → `kDefaultBrightness`
- Return type: **ALWAYS** `[[nodiscard]] Result` for public methods
- Exception handling: **NEVER** use exceptions
- Memory: **ONLY** static allocation (no new, malloc, unique_ptr)
- Overhead: **ALL** methods `noexcept`

---

## HARDWARE FACTS

| Item | Value |
|------|-------|
| MCU | ESP32-WROOM-32 |
| Power | USB Type-C only |
| Relay 1 (GPIO22) | 5V switching |
| Relay 2 (GPIO25) | 12V switching |
| PWM (GPIO18) | 5 cooling fans |
| Temperature (GPIO21) | 3 DS18B20 sensors |
| LEDs (GPIO23) | 60 × WS2812B |

---

## SERVICE INITIALIZATION ORDER

From ServiceManager (don't change this):
1. ConfigurationService
2. StatisticsService
3. SystemService
4. RelayService
5. FanService
6. TemperatureService
7. **LedService** ← 🚨 Needs redesign
8. DriveService
9. USBService
10. ProtocolService
11. HealthService
12. EventService
13. SchedulerService

---

## COMMON PATTERNS

### Adding a Command

1. Define enum in `Commands.h`
2. Add handler in `CommandDispatcher.cpp`
3. Handler calls appropriate Service
4. Service updates Objects
5. Objects trigger Drivers

### Publishing an Event

```cpp
EventBus::Publish({
    EventType::TemperatureCritical,
    sensorId,
    currentTemp
});
```

### Subscribing to Events

```cpp
EventBus::Subscribe(EventType::TemperatureCritical, [](const Event& e) {
    auto temp = e.Value();
    // React to critical temperature
});
```

### Returning from a Method

```cpp
[[nodiscard]]
Result MyFunction() noexcept {
    if (error_condition) {
        return Result(ResultCode::InvalidArgument);
    }
    return Result::Ok();
}
```

---

## COMMON MISTAKES (DON'T DO THESE)

❌ Lower layer using higher layer (Drivers calling Services)  
❌ Circular dependencies (A→B→A)  
❌ Upward dependencies (Platform using Services)  
❌ Dynamic memory allocation (`new`, `malloc`, `unique_ptr`)  
❌ Exceptions and try/catch  
❌ Blocking the main loop (use Scheduler instead)  
❌ GPIO access outside Platform layer  
❌ No error handling (ignoring Result)  
❌ Magic numbers (use config constants)  
❌ Accessing hardware without Result type  

---

## QUICK COMPILATION

```bash
# Build
platformio run -e esp32dev

# Monitor serial
platformio run -e esp32dev --monitor

# Upload
platformio run -e esp32dev -t upload

# Clean rebuild
platformio run -e esp32dev --target clean
platformio run -e esp32dev
```

---

## DEBUG LOGGING

```cpp
#include "../Core/Logger.h"

// In code:
Logger::Log("Temperature: %d", temp);
Logger::Error("Sensor failed");
Logger::Warn("High temperature warning");
```

---

## FILE LOCATIONS FOR COMMON CHANGES

| Change | File |
|--------|------|
| Add command | `src/Protocol/Commands.h` |
| Handle command | `src/Protocol/CommandDispatcher.cpp` |
| Add service method | `src/Services/XxxService.h` |
| Implement service | `src/Services/XxxService.cpp` |
| Change GPIO | `src/Platform/GPIO.h` + `src/Config/HardwareConfig.h` |
| Change LED | `src/Objects/Led.h` + `src/Objects/Led.cpp` |
| Change relay | `src/Objects/Relay.h` + `src/Objects/Relay.cpp` |
| Add event type | `src/Core/Event.h` |
| LED mapping | **NEW:** `src/Config/LedMap.h` (doesn't exist yet) |

---

## WHEN STUCK

1. **Compilation error?** Check `CodingStandard.md` for naming/style
2. **Dependency error?** Check `Architecture.md` for layer rules
3. **LED issue?** Check `LED_AUDIT_REPORT.md`
4. **Hardware issue?** Check `Hardware.md`
5. **Command issue?** Check `docs/CommandSet.md`
6. **Protocol issue?** Check `docs/Protocol.md`
7. **General question?** Check `PROJECT_REFERENCE.md`

---

## NEXT STEPS

1. ✅ Read `PROJECT_REFERENCE.md` (scope, frozen architecture, tasks)
2. ✅ Read `docs/Architecture.md` (understand layers)
3. ✅ Fix main.cpp (stop animation test)
4. ✅ Fix LED subsystem (all features implemented)
5. ⬜ Try compilation (`platformio run -e esp32dev`)
6. ⬜ Fix any build errors
7. ⬜ Implement remaining feature handlers (Relay, Fan, Temperature, etc.)
8. ⬜ Hardware bring-up and testing

---

**Status:** Development  
**Architecture:** FROZEN for Version 1.x  
**Last Updated:** July 18, 2026

