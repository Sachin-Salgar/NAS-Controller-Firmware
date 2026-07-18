# LED Subsystem Audit Report
## ESP32 NAS Controller Firmware

**Date:** July 18, 2026  
**Scope:** Complete LED architecture analysis  
**Status:** AUDIT PHASE (NO CODE MODIFICATIONS)

---

## SECTION 1: EXISTING LED-RELATED FILES

### Current LED Implementation Files

| File | Location | Type | Purpose |
|------|----------|------|---------|
| WS2812.H | `src/Platform/` | Header | Hardware abstraction for WS2812 LED strip |
| WS2812.cpp | `src/Platform/` | Implementation | WS2812 driver implementation using Adafruit NeoPixel |
| LEDDriver.h | `src/Drivers/` | Header | Driver layer abstraction |
| LEDDriver.cpp | `src/Drivers/` | Implementation | Driver layer implementation |
| Led.h | `src/Objects/` | Header | Logical LED object for drive status |
| Led.cpp | `src/Objects/` | Implementation | Drive LED state management |
| LedService.h | `src/Services/` | Header | Service layer for LED management |
| LedService.cpp | `src/Services/` | Implementation | Service layer implementation |
| LedHardwareTest.h | `src/Tests/` | Header | Basic hardware test |
| LedHardwareTest.cpp | `src/Tests/` | Implementation | Low-level LED test |
| LedAnimation.cpp | `src/Tests/` | Implementation | Animation demo (NOT integrated) |
| LedAnimationDemo.cpp | `src/Tests/` | Implementation | Animation demo using Adafruit methods |
| LED_Subsystem.md | `docs/` | Documentation | Frozen specification |

**Configuration Files:**
- `src/Config/HardwareConfig.h` - Contains `LedCount = 60`
- `src/Config/Features.h` - Contains `EnableLedControl = true`
- `src/Config/TaskConfig.h` - Contains `LedTaskIntervalMs = 100`

**Event System:**
- `src/Core/Event.h` - Defines `EventType::LedStateChanged`

---

## SECTION 2: PURPOSE OF EACH FILE

### Platform Layer

**WS2812.H/WS2812.cpp**
- Low-level hardware interface to WS2812B LED strip
- Direct control over Adafruit NeoPixel library
- Manages GPIO18 data pin initialization
- Handles pixel color updates and strip refresh
- Provides brightness control
- **Scope:** Hardware only, no application logic

### Driver Layer

**LEDDriver.h/LEDDriver.cpp**
- Thin wrapper around Platform::WS2812
- Provides initialization gate-keeping
- Routes all hardware calls through this single interface
- Maintains initialization state
- **Scope:** Hardware abstraction, no drive or animation logic

### Object Layer

**Led.h/Led.cpp**
- Represents a single logical LED mapped to a drive
- Manages DriveLedState enum (Off, Idle, Reading, Writing, Error, Missing, Rebuilding)
- Maps drive state to RGB color
- Color definitions hardcoded in `StateToColor()` function
- **Current limitation:** Only handles drive status LEDs

### Service Layer

**LedService.h/LedService.cpp**
- Public API for LED control
- Manages array of 8 drive LED objects
- Provides `SetDriveState()` and `Refresh()` methods
- Integrated into ServiceManager initialization chain
- Currently only manages drive LEDs, no system/status LEDs
- **Current limitation:** Hardcoded to 8 LEDs only

### Test Files

**LedHardwareTest.cpp**
- Direct Adafruit NeoPixel initialization
- Simple all-red fill test
- NOT integrated into firmware (standalone test)

**LedAnimation.cpp**
- Uses Platform::WS2812 API
- Implements multiple animations:
  - Rainbow Wipe
  - Knight Rider
  - Breathing Purple
  - Sparkles
  - Meteor
  - RGB Flash
  - Fade Out
- NOT compiled into firmware
- Currently running in `main.cpp` during setup()

**LedAnimationDemo.cpp**
- Uses direct Adafruit NeoPixel calls
- Implements:
  - Rainbow (using ColorHSV)
  - Theater Chase
  - Sparkle
  - Breathing White
- NOT integrated into firmware

---

## SECTION 3: CURRENT ARCHITECTURE

```
Platform Layer
    └─ WS2812 (namespace)
        ├─ Initialize()
        ├─ SetPixel(index, color)
        ├─ Fill(color)
        ├─ Clear()
        ├─ SetBrightness(value)
        ├─ Show()
        ├─ IsInitialized()
        └─ GetLedCount()

Driver Layer
    └─ LEDDriver (static class)
        ├─ Initialize()
        ├─ SetPixel()
        ├─ Fill()
        ├─ Clear()
        ├─ SetBrightness()
        └─ Show()

Object Layer
    └─ Led (class)
        ├─ Initialize(ledIndex)
        ├─ SetState(DriveLedState)
        ├─ Refresh()
        ├─ GetIndex()
        ├─ GetState()
        └─ StateToColor(DriveLedState) [PRIVATE]

Service Layer
    └─ LedService (static class)
        ├─ Initialize()
        ├─ SetDriveState(driveId, state)
        ├─ Refresh()
        └─ ClearAll()

Integration
    └─ ServiceManager::Initialize()
        └─ calls LedService::Initialize()
```

### Current LED Allocation
- LED0-LED7: Drive status (8 drives)
- LED8-LED59: Unused (52 LEDs)
- Total strip: 60 LEDs

### Hardware Constants (Currently in Platform)
- Data Pin: GPIO18
- LED Count: 60
- Color Order: GRB
- Frequency: 800 kHz
- Library: Adafruit NeoPixel
- Default Brightness: 102/255 (40%)

---

## SECTION 4: CURRENT INITIALIZATION FLOW

```
main.cpp::setup()
    └─ NAS::Tests::RunLedAnimation()  ⚠️ PROBLEM: Animation runs instead of firmware
        └─ WS2812::Initialize()
```

**Expected Flow (Not Currently Implemented):**
```
main.cpp::setup()
    └─ SystemManager::Initialize()
        └─ ServiceManager::Initialize()
            ├─ ConfigurationService::Initialize()
            ├─ LedService::Initialize()
            │   └─ LEDDriver::Initialize()
            │       └─ WS2812::Initialize()
            └─ ... other services
    └─ SystemManager::Start()
```

### Problems
1. **main.cpp runs animation test instead of firmware** - LedAnimation.cpp is executed directly
2. **No SystemManager integration** - LED service not called by actual firmware
3. **No boot animation** - Only drive LEDs are initialized, no system startup sequence
4. **No event handling** - LED service doesn't listen to firmware events
5. **Hardcoded LED indices** - No configuration mapping

---

## SECTION 5: CURRENT API

### Platform Level (WS2812 namespace)
```cpp
NAS::Core::Result Initialize() noexcept;
NAS::Core::Result SetPixel(uint16_t index, const Color& color) noexcept;
NAS::Core::Result Fill(const Color& color) noexcept;
NAS::Core::Result Clear() noexcept;
NAS::Core::Result SetBrightness(uint8_t brightness) noexcept;
NAS::Core::Result Show() noexcept;
bool IsInitialized() noexcept;
uint16_t GetLedCount() noexcept;
```

### Driver Level (LEDDriver static class)
```cpp
static NAS::Core::Result Initialize() noexcept;
static NAS::Core::Result SetPixel(uint16_t index, const Color& color) noexcept;
static NAS::Core::Result Fill(const Color& color) noexcept;
static NAS::Core::Result Clear() noexcept;
static NAS::Core::Result SetBrightness(uint8_t brightness) noexcept;
static NAS::Core::Result Show() noexcept;
```

### Object Level (Led class)
```cpp
NAS::Core::Result Initialize(uint16_t ledIndex) noexcept;
NAS::Core::Result SetState(DriveLedState state) noexcept;
NAS::Core::Result Refresh() noexcept;
uint16_t GetIndex() const noexcept;
DriveLedState GetState() const noexcept;
```

### Service Level (LedService static class)
```cpp
static constexpr uint16_t DriveLedCount = 8;

static NAS::Core::Result Initialize() noexcept;
static NAS::Core::Result SetDriveState(
    uint8_t driveId,
    NAS::Objects::DriveLedState state) noexcept;
static NAS::Core::Result Refresh() noexcept;
static NAS::Core::Result ClearAll() noexcept;
```

### Enum: DriveLedState
```cpp
enum class DriveLedState : uint8_t
{
    Off = 0,
    Idle,           // Blue
    Reading,        // Green
    Writing,        // Yellow
    Error,          // Red
    Missing,        // Purple
    Rebuilding      // Cyan
};
```

### Missing APIs
- ❌ System status LEDs (Power, Health, USB, Temperature)
- ❌ Animation control
- ❌ Boot sequence
- ❌ Shutdown sequence
- ❌ Effect/Animation framework
- ❌ LED mapping configuration
- ❌ Event-driven updates

---

## SECTION 6: PROBLEMS IDENTIFIED

### Critical Issues

1. **Main firmware not running**
   - `main.cpp` calls `RunLedAnimation()` instead of starting SystemManager
   - Firmware is stuck in animation loop
   - No actual NAS controller logic executing
   - **Impact:** Firmware non-functional

2. **Hardcoded LED indices**
   - Drive LEDs hardcoded to indices 0-7 in LedService::Initialize()
   - No centralized LED mapping
   - Changes require modifying multiple files
   - **Impact:** Difficult to reconfigure

3. **Single-purpose LED service**
   - LedService only handles drive status LEDs
   - No system status LED support
   - No animation support
   - Cannot extend without major refactor
   - **Impact:** Limited functionality

4. **No animation framework**
   - Animation code not integrated into firmware
   - Tests (LedAnimation.cpp) use blocking delays
   - No way to run animations without freezing firmware
   - **Impact:** Cannot implement required animations

5. **No configuration mapping**
   - LED indices scattered throughout code
   - Magic numbers in multiple files
   - Configuration not centralized
   - **Impact:** Hard to maintain

### Architectural Issues

6. **Missing system/status LED support**
   - Only 8 drive LEDs mapped
   - No support for Power, Health, USB, Temperature LEDs
   - Design doesn't scale to 9+ drives mentioned in requirements
   - **Impact:** Cannot implement full specification

7. **No event integration**
   - LED service not listening to system events
   - Must be explicitly called by application code
   - Reactive updates not possible
   - **Impact:** Poor system integration

8. **Animation blocking architecture**
   - LedAnimation uses blocking `delay()` calls
   - Cannot run animations alongside firmware tasks
   - Only one animation can run at a time
   - **Impact:** Cannot achieve required animations

9. **Color definitions scattered**
   - Colors hardcoded in `Led::StateToColor()`
   - Duplication in animation test files
   - No central color palette
   - **Impact:** Maintenance nightmare

10. **Brightness control unclear**
    - Default hardcoded in WS2812.cpp (102/255)
    - Can be changed at runtime
    - No persistence or recovery
    - **Impact:** No consistent brightness management

### Code Quality Issues

11. **Test code in firmware**
    - Multiple test files (LedAnimation.cpp, LedAnimationDemo.cpp)
    - Only one is disabled, but code exists
    - Bloats compiled size
    - **Impact:** Unnecessary binary size

12. **Unused LED animation code**
    - `LedAnimation.cpp` and `LedAnimationDemo.cpp` exist but not called
    - Adafruit library used directly in tests
    - Circumvents driver layer
    - **Impact:** Code quality, unused complexity

13. **Incomplete Object layer**
    - Led class only handles drive states
    - Cannot represent system status LEDs
    - Reusable design not applied
    - **Impact:** Architectural inconsistency

14. **Missing refresh semantics**
    - `Refresh()` method on individual Led objects
    - Each call to SetState immediately writes to hardware
    - No buffering or batching
    - **Impact:** Performance, potential flicker

---

## SECTION 7: MISSING COMPONENTS

### Required Features Not Implemented

**System Status LEDs**
- [ ] Power LED (LED0)
- [ ] System Health LED (LED1)
- [ ] USB Status LED (LED2)
- [ ] Temperature LED (LED3)
- [ ] Objects to represent these LEDs
- [ ] Service methods to control them

**Animation Framework**
- [ ] Non-blocking animation system
- [ ] Animation scheduler
- [ ] Multiple simultaneous animations
- [ ] Animation priority system
- [ ] Animation interrupt mechanism
- [ ] State machine for animations

**Boot/Shutdown Sequences**
- [ ] Boot animation (required by spec, 3-second max)
- [ ] Shutdown animation
- [ ] Error animation
- [ ] Backup animation
- [ ] Idle animation

**Configuration & Mapping**
- [ ] Centralized LED mapping file
- [ ] No magic numbers
- [ ] Easy drive count configuration
- [ ] LED region definitions (status vs. animation)

**Effect System**
- [ ] LED effect abstractions
- [ ] Rainbow effect
- [ ] Breathing effect
- [ ] Pulse effect
- [ ] Chase effect
- [ ] Meteor effect

**Extended Drive Support**
- [ ] Support for 9+ drives (currently hardcoded to 8)
- [ ] Drive LED state enum for 9 states (adds STANDBY)
- [ ] Scalable architecture

**System Events Integration**
- [ ] Listen to SystemStartup event
- [ ] Listen to SystemReady event
- [ ] Listen to SystemShutdown event
- [ ] Listen to TemperatureWarning/Critical events
- [ ] Listen to UsbConnected/Disconnected events
- [ ] React appropriately to events

---

## SECTION 8: UNUSED CODE

### Files Not Compiled Into Firmware

1. **LedAnimation.cpp**
   - Implements 7 different animations
   - Uses Platform::WS2812 API correctly
   - Blocking delay-based timing
   - Could be refactored for use
   - **Status:** Source exists but not linked

2. **LedAnimationDemo.cpp**
   - Implements 5 different animations
   - Uses direct Adafruit NeoPixel (bypasses driver)
   - Blocking delay-based timing
   - **Status:** Source exists but not linked

3. **LedHardwareTest.cpp**
   - Basic hardware verification
   - Creates separate Adafruit_NeoPixel instance
   - Bypasses driver layer
   - **Status:** Source exists but not linked

### Code Inside Compiled Files Not Used

1. **LedAnimation and LedAnimationDemo exist but:**
   - Not called by any production code
   - Not tested by unit tests
   - Only manually invoked for debugging
   - Referenced in main.cpp but not integrated

### Duplication

- Color definitions duplicated in:
  - `Led.cpp::StateToColor()` hardcoded RGB values
  - `LedAnimation.cpp` hardcoded color arrays
  - `LedAnimationDemo.cpp` hardcoded color values
  - No single source of truth

---

## SECTION 9: DUPLICATE FUNCTIONALITY

### Duplicate LED Strip Initialization

1. **WS2812::Initialize()** - Platform layer initialization
2. **LEDDriver::Initialize()** - Driver wrapper around platform
3. **LedHardwareTest::RunLedHardwareTest()** - Test creates own Adafruit_NeoPixel
4. **LedAnimationDemo::RunLedAnimationDemo()** - Test creates own Adafruit_NeoPixel

### Duplicate Color Definitions

1. **Led::StateToColor()** - Hardcoded RGB for drive states
2. **LedAnimation.cpp** - Hardcoded color arrays for animations
3. **LedAnimationDemo.cpp** - Hardcoded color values for animations

### Duplicate Animation Logic

1. **LedAnimation.cpp** - 7 animations using Platform::WS2812
2. **LedAnimationDemo.cpp** - 5 animations using Adafruit directly

### Duplicate Brightness Control

1. **WS2812.cpp** - Hardware brightness tracking
2. **LedAnimation.cpp** - Manual brightness manipulation
3. **LedAnimationDemo.cpp** - Manual brightness manipulation

---

## SECTION 10: RECOMMENDED ARCHITECTURE IMPROVEMENTS

### 1. Fix Main Firmware Flow
**Problem:** `main.cpp` runs animation test instead of firmware  
**Solution:** Replace test code with proper SystemManager integration
```cpp
// CURRENT (BROKEN)
void setup() {
    Serial.begin(115200);
    delay(2000);
    NAS::Tests::RunLedAnimation();  // ❌ Firmware never starts
}

// PROPOSED (CORRECT)
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

### 2. Create Centralized LED Mapping
**Problem:** LED indices hardcoded throughout codebase  
**Solution:** Create `LedMap.h` with all indices
```cpp
// src/Config/LedMap.h
namespace NAS::Config::LedMap {
    // Status Region
    constexpr uint16_t Power = 0;
    constexpr uint16_t Health = 1;
    constexpr uint16_t USB = 2;
    constexpr uint16_t Temperature = 3;
    
    // Drive Region
    constexpr uint16_t DriveStart = 4;
    constexpr uint16_t DriveCount = 9;  // Expandable
    constexpr uint16_t DriveEnd = DriveStart + DriveCount - 1;  // LED12
    
    // Animation Region
    constexpr uint16_t AnimationStart = 13;
    constexpr uint16_t AnimationEnd = 59;
}
```

### 3. Extend DriveLedState Enum
**Problem:** Missing STANDBY state mentioned in requirements  
**Solution:** Add STANDBY state
```cpp
enum class DriveLedState : uint8_t {
    Off = 0,
    Idle,
    Reading,
    Writing,
    Standby,        // NEW
    Error,
    Missing,
    Rebuilding
};
```

### 4. Create SystemLedState Enum
**Problem:** No abstraction for system status LEDs  
**Solution:** Create new enum class
```cpp
// For LED0 (Power)
enum class PowerLedState : uint8_t {
    Off,
    Booting,
    Ready,
    Shutdown,
    Error
};

// For LED1 (Health)
enum class HealthLedState : uint8_t {
    Healthy,
    Warning,
    Critical
};

// For LED2 (USB)
enum class UsbLedState : uint8_t {
    Disconnected,
    Connected,
    Transferring
};

// For LED3 (Temperature)
enum class TemperatureLedState : uint8_t {
    Normal,
    Warm,
    Hot,
    Critical
};
```

### 5. Create Non-Blocking Animation Framework
**Problem:** Current animations use blocking delays  
**Solution:** Create `LedAnimator` with frame-based timing
```cpp
// src/Services/LedAnimator.h
namespace NAS::Services {
    class LedAnimator {
    public:
        virtual void Update(uint32_t deltaMs) = 0;
        virtual bool IsComplete() const = 0;
        virtual void Stop() = 0;
    };
    
    class BootAnimation : public LedAnimator { ... };
    class IdleAnimation : public LedAnimator { ... };
    // etc.
}
```

### 6. Create LedEffects Static Class
**Problem:** No effect abstraction  
**Solution:** New service layer for effects
```cpp
// src/Services/LedEffects.h
namespace NAS::Services {
    class LedEffects {
    public:
        static Result PlayBootAnimation() noexcept;
        static Result PlayShutdownAnimation() noexcept;
        static Result PlayIdleAnimation() noexcept;
        static Result PlayErrorAnimation() noexcept;
        static Result StopAnimation() noexcept;
        static Result Update(uint32_t deltaMs) noexcept;
    };
}
```

### 7. Create LedManager for Coordination
**Problem:** No central coordinator for status + effects  
**Solution:** New manager class
```cpp
// src/Services/LedManager.h
namespace NAS::Services {
    class LedManager {
    public:
        // Status LED control
        static Result SetPower(PowerLedState state) noexcept;
        static Result SetHealth(HealthLedState state) noexcept;
        static Result SetUsb(UsbLedState state) noexcept;
        static Result SetTemperature(TemperatureLedState state) noexcept;
        
        // Drive control
        static Result SetDriveState(uint8_t driveId, DriveLedState state) noexcept;
        
        // Effects
        static Result PlayAnimation(const char* name) noexcept;
        static Result StopAnimation() noexcept;
        
        // Update
        static Result Update(uint32_t deltaMs) noexcept;
    };
}
```

### 8. Create Single Color Palette
**Problem:** Colors duplicated everywhere  
**Solution:** Centralized color definitions
```cpp
// src/Config/LedColors.h
namespace NAS::Config::LedColors {
    // System colors
    constexpr auto PowerOn = WS2812::Color{0, 255, 0};      // Green
    constexpr auto PowerOff = WS2812::Color{0, 0, 0};       // Black
    
    // Health colors
    constexpr auto Healthy = WS2812::Color{0, 255, 0};      // Green
    constexpr auto Warning = WS2812::Color{255, 165, 0};    // Orange
    constexpr auto Critical = WS2812::Color{255, 0, 0};     // Red
    
    // Drive colors
    constexpr auto DriveIdle = WS2812::Color{0, 0, 255};        // Blue
    constexpr auto DriveReading = WS2812::Color{0, 255, 0};     // Green
    constexpr auto DriveWriting = WS2812::Color{255, 255, 0};   // Yellow
    constexpr auto DriveError = WS2812::Color{255, 0, 0};       // Red
    constexpr auto DriveMissing = WS2812::Color{128, 0, 128};   // Purple
    constexpr auto DriveRebuilding = WS2812::Color{0, 255, 255}; // Cyan
    constexpr auto DriveStandby = WS2812::Color{100, 100, 100}; // Gray
    
    // Special
    constexpr auto Off = WS2812::Color{0, 0, 0};            // Black
}
```

### 9. Extend Support for 9+ Drives
**Problem:** Hardcoded to 8 drives in LedService  
**Solution:** Make drive count configurable
```cpp
// In LedMap.h
constexpr uint16_t DriveCount = 
    Config::Hardware::DriveCount <= 12 
        ? Config::Hardware::DriveCount 
        : 12;

// In LedService
static constexpr uint16_t DriveLedCount = 
    NAS::Config::LedMap::DriveCount;
```

### 10. Implement Event-Driven Updates
**Problem:** LED service not listening to events  
**Solution:** Subscribe to EventBus
```cpp
// In LedService or LedManager::Initialize()
EventBus::Subscribe(EventType::SystemStartup, [](const Event& e) {
    LedEffects::PlayBootAnimation();
});

EventBus::Subscribe(EventType::DriveStateChanged, [](const Event& e) {
    auto driveId = e.ObjectId();
    auto state = static_cast<DriveLedState>(e.Value());
    LedManager::SetDriveState(driveId, state);
});

EventBus::Subscribe(EventType::TemperatureCritical, [](const Event& e) {
    LedManager::SetTemperature(TemperatureLedState::Critical);
});
```

### 11. Separate Status and Animation Regions
**Problem:** All LEDs can be overwritten by animations  
**Solution:** Explicit region protection
```cpp
// src/Drivers/LedRegions.h
namespace NAS::Drivers::LedRegions {
    class StatusRegion {
        // LED0-3 protected from animation writes
        Result SetLed(uint16_t ledIndex, const Color& c);
    };
    
    class AnimationRegion {
        // LED13-59 for animation
        Result SetLed(uint16_t ledIndex, const Color& c);
    };
}
```

### 12. Add Brightness Management
**Problem:** Brightness not persistently managed  
**Solution:** Add brightness configuration
```cpp
// In LedManager
static Result SetBrightness(uint8_t brightness) noexcept;
static uint8_t GetBrightness() noexcept;

// Persist to configuration
Configuration::Led().brightness = brightness;
```

---

## SUMMARY OF FINDINGS

### Current State
- ✅ Platform layer properly abstracts WS2812 hardware
- ✅ Driver layer correctly wraps platform
- ✅ Object layer maps drive states to colors
- ✅ Service layer provides basic LED control
- ✅ Architecture follows project standards (Platform → Driver → Object → Service)
- ✅ Static allocation, no dynamic memory
- ✅ Uses Adafruit NeoPixel library correctly
- ❌ **Firmware not running** (animation test instead)
- ❌ No system status LED support
- ❌ No animation framework
- ❌ No centralized LED mapping
- ❌ Limited to 8 drives (should support 9+)
- ❌ No event integration
- ❌ Missing 52 of 60 LEDs in design

### Risk Assessment

| Issue | Severity | Impact |
|-------|----------|--------|
| Firmware not running | 🔴 CRITICAL | System non-functional |
| No system status LEDs | 🔴 CRITICAL | Spec non-compliance |
| Limited drive support | 🟠 HIGH | Cannot use all drive capacity |
| No animation framework | 🟠 HIGH | Cannot implement animations |
| Scattered LED indices | 🟠 HIGH | Hard to maintain |
| No event integration | 🟠 HIGH | Poor firmware cohesion |
| Test code in source | 🟡 MEDIUM | Code bloat |
| Duplicate code | 🟡 MEDIUM | Maintenance burden |

### Readiness for Implementation

**Status:** Ready for design phase

The codebase provides:
- ✅ Solid foundation (Platform/Driver/Object/Service layers work correctly)
- ✅ Proven Adafruit NeoPixel integration
- ✅ Correct error handling patterns
- ✅ Static allocation strategy
- ✅ Service initialization framework

Blockers identified:
- ⚠️ Main firmware not executing (must fix first)
- ⚠️ Test animation code must be removed/properly integrated
- ⚠️ 8-drive hardcoding must be made dynamic

---

## NEXT STEPS

1. **STEP 2 (User Action):** Proceed with DESIGN REVIEW
2. **STEP 3:** Review REQUIRED FEATURES specification
3. **STEP 4:** Approve ARCHITECTURE design
4. **STEP 5:** Review API DESIGN
5. **STEP 6:** Discuss CONFIGURATION approach
6. **STEP 7:** Review IMPLEMENTATION PLAN
7. **STEP 8:** Begin implementation with IMPLEMENTATION RULES

Ready to proceed to Step 2: DESIGN REVIEW phase.

