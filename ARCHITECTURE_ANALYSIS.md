# Boot Self-Test Framework - Architectural Analysis

## Executive Summary

The current self-test framework has a **fundamental architectural flaw**: it treats all failures identically, causing **any single failed component in any layer to halt the entire firmware**. This is problematic because USB (currently failing) is an optional feature, yet its failure blocks boot. The framework needs to distinguish between critical (must-pass) and optional (can-fail) tests.

---

## 1. Current Execution Flow

```
main.cpp setup()
    ↓
[1/3] SystemManager::Initialize()
    ├─ Startup::Initialize()
    └─ Application::Initialize()
    ↓ (returns Result)
[Result check: if !success → Halt("SystemManager initialization FAILED")]
    ↓
[2/3] SelfTest::Run()
    ├─ TestCore()
    │   ├─ TestLogger()
    │   ├─ TestResult()
    │   ├─ TestVersion()
    │   ├─ TestBuildInfo()
    │   ├─ TestEvent()
    │   ├─ TestEventBus()
    │   └─ TestScheduler()
    │   ↓
    │   LayerResult { result, failedComponent, failureCode, passCount, failCount, skippedCount }
    │
    ├─ TestPlatform()
    │   ├─ TestGpio()
    │   ├─ TestFlash()
    │   ├─ TestUsb()          ← CURRENTLY FAILING (optional)
    │   ├─ TestPwm()
    │   ├─ TestUart()
    │   ├─ TestI2c()
    │   ├─ TestSpi()
    │   ├─ TestAdc()
    │   ├─ TestTimer()
    │   ├─ TestOneWire()
    │   └─ TestWatchdog()
    │   ↓
    │   LayerResult { result=FAIL, failedComponent="USB", ... }
    │
    ├─ TestDrivers()
    │   └─ [7 driver tests]
    │   ↓
    │   LayerResult { result=PASS, ... }
    │
    ├─ TestObjects()
    │   └─ [8 object tests]
    │   ↓
    │   LayerResult { result=PASS, ... }
    │
    ├─ TestServices()
    │   └─ [14 service tests]
    │   ↓
    │   LayerResult { result=PASS, ... }
    │
    ├─ TestProtocol()
    │   └─ [protocol tests]
    │   ↓
    │   LayerResult { result=PASS, ... }
    │
    └─ TestSystem()
        └─ [system tests]
        ↓
        LayerResult { result=PASS, ... }
    
    ↓
    [Failure Analysis: if failureCount > 0 → return Result(Failed)]
    ↓
[Result check: if !success → Halt("Boot Self Test FAILED")]
    ↓
[3/3] Starting Firmware...
```

---

## 2. Key Classes & Their Roles

### **Result (src/Core/Result.h)**
- Lightweight status object with `ResultCode` enum
- Single constructor: `Result(ResultCode code = ResultCode::Success)`
- Methods: `IsSuccess()`, `IsFailure()`, `Code()`, `operator bool()`
- No severity/criticality information

### **LayerResult (src/Tests/TestResult.h)**
```cpp
struct LayerResult {
    NAS::Core::Result result;              // Overall layer status
    const char* failedComponent;           // Name of first failure
    NAS::Core::ResultCode failureCode;     // Code from first failure
    int passCount;                         // Number of passed tests
    int failCount;                         // Number of failed tests
    int skippedCount;                      // Number of skipped tests
};
```
- Struct (not a class) with no methods
- Contains only the first failure, ignores subsequent failures
- No indication of component criticality

### **SelfTest (src/Tests/SelfTest.cpp)**
- **Single responsibility**: Orchestrate all layer tests and aggregate results
- Calls 7 layer test functions: `TestCore()`, `TestPlatform()`, `TestDrivers()`, etc.
- All layer tests run regardless of previous failures (good)
- Collects failures into `FailureRecord` array (max 7 entries)
- **Critical decision point**: Returns `Result(Failed)` if `failureCount > 0`

### **TestFormatter (src/Tests/TestFormatter.h)**
- Static utility class for formatting test output
- Provides: `PrintHeader()`, `PrintPass()`, `PrintFail()`, `PrintSkipped()`, `PrintFooter()`
- No role in pass/fail logic, only display

### **Layer Test Functions** (CoreTests.cpp, PlatformTests.cpp, etc.)
- Each returns `LayerResult` struct
- Each runs multiple component tests sequentially
- **Implementation pattern** (from PlatformTests.cpp):
  ```cpp
  LayerResult TestPlatform() {
      LayerResult layerResult = {Result::Ok(), nullptr, 
          ResultCode::Success, 0, 0, 0};
      
      // For each component test:
      auto result = TestGpio();
      if (!result) {
          if (!layerResult.result) {  // Store only first failure
              layerResult.failedComponent = "GPIO";
              layerResult.failureCode = result.Code();
          }
          layerResult.failCount++;
      } else {
          layerResult.passCount++;
      }
      
      // Set overall layer status
      if (layerResult.failCount == 0) {
          layerResult.result = Result::Ok();
      } else {
          layerResult.result = Result(ResultCode::Failed);
      }
      return layerResult;
  }
  ```

---

## 3. Which Function Decides Firmware Should HALT?

**Two decision points:**

### **Point 1: main.cpp setup() - Line 90-93**
```cpp
result = NAS::Tests::SelfTest::Run();
if (!result.IsSuccess()) {
    Halt("Boot Self Test FAILED.");
}
```
- Checks the boolean result from `SelfTest::Run()`
- **HALTS if any failure anywhere in any layer**

### **Point 2: SelfTest::Run() - Line 265-275**
```cpp
if (failureCount == 0) {
    return NAS::Core::Result::Ok();  // Boot continues
}
// failureCount > 0
return NAS::Core::Result(NAS::Core::ResultCode::Failed);  // Boot halts
```
- **ROOT CAUSE**: Any layer failure → propagates to SelfTest → halts boot
- No mechanism to mark tests as non-critical

---

## 4. Which Functions Currently Force Boot Failure?

1. **SelfTest::Run()** (SelfTest.cpp:265-275)
   - Returns `Result(Failed)` if any layer has failures
   - No filtering or criticality logic

2. **Each Layer Test Function** (e.g., TestPlatform:247-254)
   - Sets `layerResult.result = Result(Failed)` if `failCount > 0`
   - No distinction between critical/optional components

3. **Each Component Test** (e.g., TestUsb:122-131)
   - Returns `Result(Failed)` if operation fails
   - No criticality information attached

---

## 5. Individual Test Failure Behavior

### **Does one failed component fail its whole layer?**

**YES** — but indirectly and imperfectly.

Example from PlatformTests.cpp:
```cpp
result = TestUsb();
if (!result) {
    TestFormatter::PrintFail("USB");
    if (!layerResult.result) {  // Only update if first failure
        layerResult.failedComponent = "USB";
        layerResult.failureCode = result.Code();
    }
    layerResult.failCount++;  // ← Increment fail count
}

// Later, at layer completion:
if (layerResult.failCount == 0) {
    layerResult.result = Result::Ok();
} else {
    layerResult.result = Result(Failed);  // ← Any fail → layer fails
}
```

- One failed component increments `failCount`
- If `failCount > 0`, layer becomes FAILED
- **Impact**: Layer passes only if ALL components pass

---

## 6. Layer Failure Propagation to Firmware

### **Does one failed layer fail the entire firmware?**

**YES** — absolutely.

Example from SelfTest.cpp:
```cpp
if (!layerResults[1].result) {  // TestPlatform failed
    failures[failureCount].layer = "PLATFORM";
    failures[failureCount].component = layerResults[1].failedComponent;
    failures[failureCount].code = layerResults[1].failureCode;
    failureCount++;  // ← Increment failure count
}

// Later:
if (failureCount == 0) {
    return Result::Ok();  // Boot continues
} else {
    return Result(Failed);  // ← Boot halts
}
```

- **One layer failure → whole framework fails → firmware halts**
- No prioritization or distinction

---

## 7. Complete Call Chain (Failure Propagation Diagram)

```
USB::Initialize() [FAILS with DeviceNotFound]
    ↓
TestUsb() returns Result(Failed)
    ↓
TestPlatform() at line 198:
    if (!result) {
        layerResult.failCount++;
        if (!layerResult.result) {
            layerResult.failedComponent = "USB";
            layerResult.failureCode = DeviceNotFound;
        }
    }
    ↓
TestPlatform() at line 247:
    if (layerResult.failCount == 0) {
        return LayerResult { Result::Ok(), ... }
    } else {
        return LayerResult { Result(Failed), "USB", DeviceNotFound, ... }
    }
    ↓
SelfTest::Run() at line 164:
    layerResults[1] = TestPlatform();  // Gets LayerResult with Failed
    if (!layerResults[1].result) {
        failures[failureCount].layer = "PLATFORM";
        failures[failureCount].component = "USB";
        failureCount++;  // Now failureCount = 1
    }
    ↓
SelfTest::Run() at line 265:
    if (failureCount == 0) {  // False! failureCount = 1
        return Result::Ok();
    } else {
        return Result(Failed);  // ← Execution goes here
    }
    ↓
main.cpp setup() at line 90:
    result = SelfTest::Run();  // Gets Result(Failed)
    if (!result.IsSuccess()) {
        Halt("Boot Self Test FAILED.");  // ← FIRMWARE HALTS
    }
```

---

## Architectural Problems

### **Problem 1: No Criticality Model**
- `Result` class has no field for criticality/severity
- `LayerResult` has no per-component criticality information
- All failures treated equally regardless of importance
- **Impact**: USB failure (optional) halts boot like a Core failure (critical) would

### **Problem 2: All-or-Nothing Layer Testing**
- Layer passes only if ALL components pass
- No concept of "minimum viable layer"
- One non-critical test failure fails entire layer
- **Impact**: Optional platform tests block optional driver tests
- **Example**: USB fails → Platform layer fails → entire framework fails

### **Problem 3: No Failure Categorization**
- No distinction between:
  - Core system failures (should ALWAYS halt)
  - Optional hardware failures (should be warnings)
  - Expected failures (like missing OneWire sensors)
  - Transient failures (timeout, busy)

### **Problem 4: Single Failure Recording**
- `LayerResult.failedComponent` stores only the FIRST failure
- Subsequent failures in same layer are lost
- Difficult to diagnose multiple issues
- **Example**: If GPIO AND USB both fail, only GPIO is reported

### **Problem 5: Result Code Overloading**
- `ResultCode::Failed` is generic — doesn't indicate component
- USB uses `ResultCode::Failed` (or similar)
- Hard to distinguish "USB not connected (ok)" from "USB driver crash"
- No severity levels (critical, warning, info)

### **Problem 6: Inflexible Boot Decision Logic**
- `failureCount > 0` → halt firmware
- No policy configuration
- No runtime override mechanism
- No configuration file to mark tests as optional
- Hard-coded in source

---

## Recommended Production Design

### **1. Test Criticality Classification**

Add a `TestCriticality` enum:
```cpp
enum class TestCriticality : uint8_t
{
    Critical,      // Must pass to boot
    Required,      // Should pass, boot with warning if failed
    Optional,      // Can fail without affecting boot
};
```

### **2. Enhanced Result Object**

Add severity to `Result`:
```cpp
enum class ResultSeverity : uint8_t
{
    Success = 0,
    Warning,       // Component functional but degraded
    Error,         // Component non-functional
    Critical,      // System cannot boot
};

class Result {
    ResultCode code;
    ResultSeverity severity;  // NEW
    const char* component;     // NEW - which component failed
};
```

### **3. Enhanced LayerResult**

```cpp
struct LayerResult {
    NAS::Core::Result result;
    int passCount;
    int failCount;
    int skippedCount;
    
    // NEW: Per-component failure details
    struct ComponentFailure {
        const char* name;
        NAS::Core::ResultCode code;
        TestCriticality criticality;
        ResultSeverity severity;
    };
    
    ComponentFailure failures[MAX_FAILURES];  // ALL failures, not just first
    int failureCount;  // Actual number of failures
};
```

### **4. Test Registry Pattern**

```cpp
struct TestDefinition {
    const char* name;
    NAS::Core::Result (*testFn)();
    TestCriticality criticality;
};

// Example:
const TestDefinition platformTests[] = {
    { "GPIO",      TestGpio,     TestCriticality::Critical  },
    { "USB",       TestUsb,      TestCriticality::Optional  },
    { "I2C",       TestI2c,      TestCriticality::Required  },
    { "OneWire",   TestOneWire,  TestCriticality::Optional  },
    ...
};
```

### **5. Failure Severity Determination**

```cpp
enum class BootDecision {
    Continue,          // All critical tests passed
    WarnContinue,      // Required tests failed, can continue
    CriticalFailed,    // Critical test failed, must halt
};

BootDecision DetermineBoot(LayerResult* results, int layerCount) {
    for (each layer) {
        for (each failure in layer.failures) {
            if (failure.criticality == Critical) {
                return BootDecision::CriticalFailed;
            }
        }
    }
    
    // Check if all required tests passed
    bool hasRequiredFailure = ...;
    if (hasRequiredFailure) {
        return BootDecision::WarnContinue;
    }
    
    return BootDecision::Continue;
}
```

### **6. Policy-Based Boot Logic**

```cpp
enum class BootPolicy {
    Strict,              // All tests must pass
    Normal,              // Critical + Required must pass, Optional can fail
    Degraded,            // Only Critical must pass
    ForceAllow,          // Force boot regardless (debug mode)
};

Result SelfTest::Run(BootPolicy policy = BootPolicy::Normal) {
    // ... run tests ...
    
    auto bootDecision = DetermineBoot(layerResults, 7);
    
    switch (policy) {
        case Strict:
            return (bootDecision == CriticalFailed) ? Failed : Ok;
        case Normal:
            return (bootDecision == CriticalFailed) ? Failed : Ok;
        case Degraded:
            return Ok;  // Always boot
        case ForceAllow:
            return Ok;  // Always boot
    }
}
```

### **7. Configuration File Support**

Store test criticality in flash:
```cpp
// tests.conf
[PLATFORM]
GPIO=critical
USB=optional
I2C=required
Flash=critical

[DRIVERS]
PWMFanDriver=critical
StorageDriver=required
UsbDriver=optional
```

---

## Summary of Current Issues

| Issue | Severity | Current Behavior | Required Change |
|-------|----------|------------------|-----------------|
| **No criticality model** | HIGH | USB (optional) blocks boot | Add TestCriticality enum |
| **All-or-nothing layers** | HIGH | One component fails → layer fails | Allow partial layer success |
| **Single failure recording** | MEDIUM | Only first failure logged | Store all failures |
| **No severity levels** | MEDIUM | All failures identical | Add ResultSeverity |
| **Hard-coded policy** | MEDIUM | failureCount > 0 → halt | Make policy configurable |
| **Generic result codes** | LOW | Hard to diagnose issues | Add component info to Result |

---

## Next Steps (For Step 2)

1. Define `TestCriticality` enum
2. Enhance `Result` class with severity and component fields
3. Enhance `LayerResult` to store all failures
4. Create test registry with criticality metadata
5. Implement `DetermineBoot()` logic
6. Update `SelfTest::Run()` to use new policy system
7. Add configuration file support (optional, Phase 2)
8. Update boot decision logic in main.cpp
9. Add comprehensive logging of all failures
10. Test with USB failing and firmware still booting successfully
