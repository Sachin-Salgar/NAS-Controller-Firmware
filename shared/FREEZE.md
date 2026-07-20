# Shared Package Architecture Freeze

**Status:** 🔒 FROZEN  
**Date:** 2026-07-19  
**Version:** 1.0.0  
**Purpose:** All subsequent development builds on these stable contracts

---

## What This Means

The shared package is now **frozen**. No further changes unless:
1. **Protocol contract changes** - New command codes, new packet structure, new field
2. **Critical bug fix** - Something is fundamentally broken in existing contract
3. **Versioning issue** - Version needs increment for compatibility

**Do NOT modify shared/** to:
- ❌ Add runtime behavior
- ❌ Add daemon-specific types
- ❌ Add UI-specific types
- ❌ Add firmware-specific types
- ❌ Add error classes
- ❌ Add helper functions
- ❌ Reorganize for convenience

---

## What Shared Now Exports

### Version Information (shared/src/version.ts)
```typescript
ProtocolVersion      // MAJOR.MINOR.PATCH
ProjectVersion       // MAJOR.MINOR.PATCH
CompatibilityVersion // Min/Max protocol versions, isCompatible()
```

### Protocol Constants (shared/src/protocol.ts)

**ProtocolConstants** - Packet format (does not change):
- Header: 0x55AA
- Footer: 0xAA
- CRC algorithm: CRC16-CCITT
- CRC parameters

**ProtocolLimits** - Resource constraints (DEPRECATED):
- MaxPacketSize, MaxPayloadSize
- MaxSequenceNumber, MaxCommandCode
- MaxRelays, MaxFans, MaxTemperatureSensors, MaxDrives
- (Kept for backward compatibility during Phase 1 only)
- **Prefer:** Use PacketLimits and HardwareLimits directly

**PacketLimits** - Protocol structure constraints:
- MaxPacketSize, MaxPayloadSize
- MaxSequenceNumber, MaxCommandCode, MaxErrorCode
- (Inherent to protocol format, do not change)

**HardwareLimits** - Hardware capability constraints:
- MaxRelays, MaxFans, MaxLEDs, MaxTemperatureSensors, MaxDrives
- (May increase with improved hardware)

**FeatureFlag** - Optional capabilities (Bitmask Enum):
- PWM_FAN_CONTROL (0x0001)
- RGB_LED (0x0002)
- TEMPERATURE_SENSOR (0x0004)
- EVENT_LOG (0x0008)
- CONFIGURATION_PERSISTENCE (0x0010)
- REAL_TIME_CLOCK (0x0020)
- FIRMWARE_UPDATE (0x0040)
- NETWORK_INTERFACE (0x0080)
- (Firmware reports as single bitmask in GET_CAPABILITIES response)

### Command Codes (shared/src/protocol.ts)

**CommandCode** - All 16 implemented commands:
```
0x01-0x0F: System (PING, RESET, GET_CAPABILITIES)
0x10-0x1F: Relay (SET, GET)
0x20-0x2F: Status (GET_ALL_STATUS)
0x30-0x3F: Fan (SET_SPEED, GET, SET_MODE)
0x40-0x4F: Temperature (READ)
0x50-0x5F: LED (SET_COLOR, SET_BRIGHTNESS, SET_ANIMATION)
0x60-0x6F: Configuration (GET, SET, BEGIN, COMMIT)
0x70-0x7F: Event Log (GET_EVENT_LOG)
0x80-0x9F: Reserved (response codes)
0xA0-0xBF: Reserved (future: Storage)
0xC0-0xDF: Reserved (future: Network)
0xE0-0xFF: Reserved (future: Advanced)
```

### Enumerations (shared/src/protocol.ts)

- **ErrorCode** (0x00-0x0A) - Firmware error classification
- **RelayState** - OFF (0x00), ON (0x01)
- **FanMode** - MANUAL (0x00), AUTO (0x01), OFF (0x02)
- **LEDAnimation** - BOOT, IDLE, ERROR, SHUTDOWN, CUSTOM
- **EventType** - 13 event types (DEVICE_CONNECTED, RELAY_STATE_CHANGED, etc.)

### Packet Structures (shared/src/protocol.ts)

- **PacketHeader** - Magic bytes (0x55AA)
- **PacketMetadata** - Sequence, command, length
- **PacketFooter** - CRC16 and terminator (0xAA)
- **Packet** - Complete packet structure
- **ParsedPacket** - Decoded command (sequence, command, payload)

### Data Transfer Objects (shared/src/protocol.ts)

**Configuration DTOs:**
- CapabilityInfo
- ConfigurationData
- RelayConfig, FanConfig, TemperatureSensorConfig, etc.

**Status DTOs:**
- RelayStatus, FanStatus, TemperatureStatus, DriveStatus, LEDStatus
- SystemStatus (aggregate of all hardware status)

**Response DTOs:**
- EventLogEntry

### Command Request Types (shared/src/protocol.ts)

Strongly-typed for each command:
- SystemPingRequest
- RelaySetRequest, RelayGetRequest
- FanSetSpeedRequest, FanGetRequest, FanSetModeRequest
- TemperatureReadRequest
- LEDSetColorRequest, LEDSetBrightnessRequest, LEDSetAnimationRequest
- ConfigGetRequest, ConfigSetRequest, ConfigBeginRequest, ConfigCommitRequest
- GetAllStatusRequest
- GetEventLogRequest

Union type: `Command` (all requests)

### Response Types (shared/src/protocol.ts)

Discriminated unions for safe pattern matching:
- **AckResponse** - isAck: true, isNak: false
- **NakResponse** - isAck: false, isNak: true, errorCode: ErrorCode
- **DataResponse** - isAck: false, isNak: false, payload: Uint8Array

Union type: `Response` (all responses)

### Event Definitions (shared/src/protocol.ts)

**BaseEvent** - All events extend:
```typescript
{
  type: EventType
  timestamp: number
  source?: string
}
```

Specific events (13 types):
- DeviceConnectedEvent, DeviceDisconnectedEvent
- RelayStateChangedEvent
- FanSpeedChangedEvent, FanModeChangedEvent
- LEDColorChangedEvent, LEDAnimationChangedEvent
- TemperatureChangedEvent
- ConfigurationChangedEvent
- CommandSentEvent, CommandCompletedEvent, CommandFailedEvent
- ErrorOccurredEvent

Union type: `Event` (all events)

### Result Type (shared/src/protocol.ts)

For predictable error handling (no thrown exceptions):
```typescript
Result<T> = Success<T> | Failure
  ├─ Success<T>: { success: true, value: T }
  └─ Failure: { success: false, error: ErrorInfo }

ErrorInfo: { code, message, retryable? }

// Constructor functions
Ok<T>(value: T): Result<T>
Err(code, message): Result<never>
```

### Type Aliases (shared/src/types.ts)

For clarity:
- Uint8, Uint16, Uint32, Int8, Int16
- RelayId, FanId, SensorId, DriveId, LEDId
- SequenceNumber, Timestamp, PercentageValue, Temperature
- CommandMetadata, ResponseMetadata

---

## Architectural Layers Using Shared

### Daemon (Building Next)
```
daemon/
├── errors/          (Creates runtime error classes)
├── core/
│   ├── protocol/    (CRC16, encoder, decoder, validator)
│   ├── transport/   (SerialTransport, MockTransport, ITransport)
│   └── queue/       (CommandQueue)
└── state/           (StateStore, event handling)
```

**Uses from shared:**
- ProtocolVersion, ProtocolConstants, PacketLimits, HardwareLimits
- CommandCode, ErrorCode, RelayState, FanMode, LEDAnimation, EventType, FeatureFlag
- All packet structures, DTOs, command/response types
- Result<T>, ErrorInfo
- Event types

**Does NOT use from shared:**
- ProtocolTiming (timeout/retry policy is daemon responsibility, not protocol contract)
- Any runtime behavior or helper functions

### Firmware (Already Complete)
```
firmware/
├── Protocol/        (Packet parsing, command dispatch)
├── Services/        (Relay, Fan, LED, Temperature, USB)
└── Objects/         (Hardware models)
```

**Uses from shared:**
- Protocol constants (header, footer, CRC values)
- CommandCode (dispatch commands)
- ErrorCode (return errors in NAK)
- Packet structure (for encoding/decoding)
- All enums (RelayState, FanMode, etc.)

### Frontend (Building Later)
```
frontend/
├── api/             (WebSocket client)
├── hooks/           (State subscriptions)
├── pages/           (Dashboard, Controls, Test Console)
└── components/      (UI widgets)
```

**Uses from shared:**
- EventType (subscribe to events)
- All status DTOs (display state)
- CommandCode, enums (for display/labels)
- Result<T> (error handling in API layer)
- CapabilityInfo (UI adaptation)

---

## Modification Policy

### When Can Shared Change?

✅ **ALLOWED:**
1. **Protocol Changes** - New command codes, new packet fields, new error codes
   - Example: Add new command 0x80 for storage (in reserved range)
   - Update CommandCode enum
   - Add new request/response types
   - Increment ProtocolVersion.MINOR

2. **Feature Flags** - New ProtocolFeatureFlag
   - Example: Add ProtocolFeatureFlag.FIRMWARE_UPDATE
   - Add to ProtocolFeatures.EXTENDED
   - No breaking change

3. **Limits** - Increase hardware limits
   - Example: MaxRelays 16 → 32
   - No breaking change (only expands possibility)

4. **Timing** - Adjust timeout/retry values
   - Example: CommandTimeout 1000 → 1500 ms
   - Might affect reliability, but not protocol

5. **Critical Bug Fix** - Something broken in existing contract
   - Example: Fix enum value mismatch with firmware
   - Must have test verification

### NEVER Do This:

❌ **FORBIDDEN:**
- Add error classes (belong in daemon)
- Add error helpers/mappers (belong in daemon)
- Add state models (belong in daemon)
- Add transport interfaces (belong in daemon)
- Add logger interfaces (belong in daemon)
- Add runtime behavior (any functions with logic)
- Reorganize for convenience
- Add UI-specific types
- Add firmware-specific types
- Refactor without protocol reason

---

## Stability Guarantees

Once shared is frozen, all three layers can proceed in parallel:

```
shared/ ✅ STABLE
  ├─→ daemon/core/protocol/        (CRC, encoder, decoder)
  ├─→ daemon/core/transport/       (Serial, Mock)
  ├─→ daemon/core/queue/           (Command queue)
  ├─→ daemon/state/                (State store)
  ├─→ daemon/api/                  (WebSocket API)
  ├─→ frontend/                    (React UI)
  └─→ firmware/                    (Already complete)
```

**No layer needs to wait for others to catch up.**

---

## How to Extend

### Adding a New Command

**Step 1: Add command code to shared**
```typescript
// shared/src/protocol.ts
export const CommandCode = {
  ...
  STORAGE_READ: 0x81,  // In reserved range 0x80-0x9F
}
```

**Step 2: Define request/response types in shared**
```typescript
export interface StorageReadRequest extends BaseCommand {
  command: typeof CommandCode.STORAGE_READ;
  startAddress: number;
  byteCount: number;
}

export type Command = ... | StorageReadRequest;
```

**Step 3: Add to firmware**
```cpp
case CMD_STORAGE_READ:
  handleStorageRead(...);
  break;
```

**Step 4: Add to daemon**
```typescript
// daemon/core/protocol/encoder.ts
case CommandCode.STORAGE_READ:
  return encodeStorageRead(cmd as StorageReadRequest);
```

**Step 5: Add to frontend**
```typescript
// frontend/hooks/useStorage.ts
const readStorage = (addr, count) => {
  socket.emit("command", { command: CommandCode.STORAGE_READ, ... });
};
```

### Adding a New Event

**Step 1: Add event type to shared**
```typescript
// shared/src/protocol.ts
export enum EventType {
  ...
  STORAGE_FULL = "storage_full",
}

export interface StorageFullEvent extends BaseEvent {
  type: EventType.STORAGE_FULL;
  availableBytes: number;
}

export type Event = ... | StorageFullEvent;
```

**Step 2: Emit from firmware/daemon**
```typescript
// daemon/state/StateStore.ts
const onStorageFull = (availableBytes: number) => {
  this._eventBus.emit({
    type: EventType.STORAGE_FULL,
    timestamp: Date.now(),
    availableBytes,
  });
};
```

**Step 3: Subscribe in UI**
```typescript
// frontend/hooks/useStorage.ts
useEffect(() => {
  socket.on("event", (event: Event) => {
    if (event.type === EventType.STORAGE_FULL) {
      showWarning(`Storage almost full: ${event.availableBytes} bytes`);
    }
  });
}, []);
```

---

## Testing Shared

Every test in shared/ verifies a contract:

```
✅ Command codes match specification
✅ Error codes are in valid range (0x00-0x0A)
✅ Enums have correct values
✅ Packet structures are well-formed
✅ Version numbers are semantic
✅ Compatibility checks work correctly
✅ Result<T> type works correctly
✅ Event payloads extend BaseEvent
✅ No runtime logic is executed
```

**Command:** `cd shared && npm test`

---

## Document Checklist Before Freeze

- ✅ Protocol constants defined (ProtocolConstants, ProtocolLimits, ProtocolTiming)
- ✅ All 16 commands defined with proper ranges
- ✅ All error codes (0x00-0x0A) defined
- ✅ Packet structures defined (Header, Metadata, Footer, Packet)
- ✅ All enumerations defined (ErrorCode, RelayState, FanMode, etc.)
- ✅ Command request types (strongly-typed)
- ✅ Response types (discriminated unions)
- ✅ Event payloads (BaseEvent pattern)
- ✅ Status/Configuration DTOs
- ✅ Result<T> type for error handling
- ✅ Version definitions (ProtocolVersion, ProjectVersion, CompatibilityVersion)
- ✅ Feature flags defined
- ✅ Type aliases for clarity
- ✅ Comprehensive test coverage
- ✅ No runtime behavior
- ✅ No daemon/UI/firmware-specific types

---

## Status

**Shared Package is READY.**

All contracts are defined.
All types are correct.
All tests pass.
Architecture is sound.

**Proceed to Task 2: CRC16 Implementation in daemon/**

The daemon can now import from shared and implement the protocol layer.

---

## Backward Compatibility Guarantee

**Shared contracts must remain backward compatible whenever possible.**

When modifying shared:
1. Never remove existing definitions
2. Never change meanings of existing constants
3. New fields should be additive only (at the end)
4. New command codes go into reserved ranges
5. New error codes extend existing enums (don't replace)

Example:
- ✅ Add new command 0x81 in reserved range
- ✅ Add new feature flag 0x0100
- ❌ Remove command 0x10 (breaks firmware)
- ❌ Change ErrorCode.TIMEOUT from 0x06 to 0x07 (breaks wire protocol)

This ensures old firmware works with new daemon and vice versa.

---

## Strict Instruction

> **Do not modify `shared/` again unless a protocol contract changes.**
>
> Treat the shared package as **frozen**. All subsequent layers (CRC, encoder, decoder, transport, daemon, UI, and firmware) build against these stable contracts.

This is how mature projects evolve: stabilize the interfaces first, then build everything else on top of them.
