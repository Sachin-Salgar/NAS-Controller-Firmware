# Shared Package Refactoring - Architectural Corrections

**Status:** ✅ COMPLETE  
**Date:** 2026-07-19  
**Reason:** Move runtime logic out of contract library

## Executive Summary

The initial implementation of the shared package mixed **contracts** (type definitions) with **runtime behavior** (error classes, helper functions, state models). This violated the principle that shared should be a pure contract library.

**Refactoring moved:**
- ❌ OUT of shared: Error classes, helper functions, state interfaces, transport abstractions
- ✅ INTO shared: Protocol contracts, enums, DTOs, event payloads, Result<T> type

**Result:** Clean separation where shared provides only contracts, and daemon implements runtime behavior.

---

## What Changed

### Removed from shared/src/errors.ts ❌

**Status:** FILE DELETED

The following runtime logic was removed:
- Base `NASControllerError` class
- All 27+ specific error classes (TransportError, SerialDisconnectedError, etc.)
- Error classification enum (TRANSPORT, PROTOCOL, FIRMWARE, etc.)
- Helper functions:
  - `mapFirmwareErrorCode()`
  - `isRetryableError()`
  - `getErrorClassification()`

**Why:** These are runtime concerns that belong in the daemon layer where error handling actually occurs.

**Migration Path:** Daemon will implement these error classes locally, using ErrorCode enum from shared as the contract.

### Refactored shared/src/protocol.ts ✅

**From:** 332 lines (mixed contracts and types)  
**To:** 560 lines (pure contracts, better organized)

**Consolidated Protocol Metadata:**
```typescript
export const ProtocolInfo = {
  Version: "1.0.0",
  Header: 0x55AA,
  Footer: 0xAA,
  CRC: "CRC16-CCITT",
  CRCInitialValue: 0xFFFF,
  CRCPolynomial: 0x1021,
  MaxPacketSize: 256,
  CommandTimeout: 1000,
  MaxRetryAttempts: 3,
} as const;
```

**Single source of truth** for all protocol parameters - every module references this.

**Improved Structure:**

1. **ProtocolInfo** - Centralized metadata
2. **ResponseBit** - Response code masks (ACK=0x80, NAK=0x40)
3. **CommandCode** - All 16 command codes organized by category
4. **Enumerations** - ErrorCode, RelayState, FanMode, LEDAnimation, EventType
5. **Packet Structures** - PacketHeader, PacketMetadata, PacketFooter, Packet, ParsedPacket
6. **DTOs** - Configuration, Status, Capability objects
7. **Command Types** - Separate interface for each command (type-safe)
8. **Response Types** - Discriminated unions (AckResponse, NakResponse, DataResponse)
9. **Event Definitions** - Strongly-typed events with payloads
10. **Result<T> Type** - For success/failure handling (replacing thrown exceptions)

### Refactored shared/src/types.ts ✅

**From:** 245 lines (interfaces, state models, abstractions)  
**To:** 38 lines (pure type aliases)

**Removed:**
- State types (FirmwareState, HardwareState, ApplicationState)
- Transport interface (ITransport)
- Configuration store interface (IConfigurationStore)
- Logger interface
- All behavior patterns

**Kept:**
- Numeric type aliases (Uint8, Uint16, Uint32, Int16)
- Value type aliases (RelayId, FanId, SensorId, Timestamp, Temperature)
- Simple metadata types (CommandMetadata, ResponseMetadata)

**Reason:** These are pure type definitions with no semantic meaning beyond clarity.

---

## Architecture After Refactoring

### Shared Package (Pure Contracts)

```
shared/
├── protocol.ts
│   ├── ProtocolInfo (centralized metadata)
│   ├── CommandCode (all 16 commands)
│   ├── ErrorCode (error classification)
│   ├── Enums (RelayState, FanMode, LEDAnimation, EventType)
│   ├── Packet structures (Header, Metadata, Footer)
│   ├── DTOs (CapabilityInfo, ConfigurationData, SystemStatus)
│   ├── Command types (SystemPingRequest, RelaySetRequest, etc.)
│   ├── Response types (AckResponse, NakResponse, DataResponse)
│   ├── Event payloads (DeviceConnectedEvent, RelayStateChangedEvent, etc.)
│   └── Result<T> (for error handling)
└── types.ts
    └── Type aliases (Uint8, RelayId, Temperature, etc.)
```

### Daemon Package (Runtime Implementation) — to be built

```
daemon/
├── src/
│   ├── errors/
│   │   ├── NASControllerError.ts (base class)
│   │   ├── TransportError.ts (network/serial errors)
│   │   ├── ProtocolError.ts (packet errors)
│   │   ├── FirmwareError.ts (device errors)
│   │   ├── ValidationError.ts (input errors)
│   │   └── errorHelpers.ts (mapFirmwareErrorCode, isRetryable)
│   ├── core/
│   │   ├── protocol/
│   │   │   ├── crc16.ts (implements CRC16-CCITT)
│   │   │   ├── encoder.ts (command → packet)
│   │   │   ├── decoder.ts (packet → response)
│   │   │   └── validator.ts (packet validation)
│   │   ├── transport/
│   │   │   ├── ITransport.ts (abstraction)
│   │   │   ├── SerialTransport.ts (USB CDC)
│   │   │   └── MockTransport.ts (testing)
│   │   └── queue/
│   │       └── CommandQueue.ts (queued execution)
│   └── state/
│       └── StateStore.ts (FirmwareState, HardwareState)
```

**Key Point:** Daemon imports ErrorCode enum from shared, but implements error classes locally.

---

## Benefits of This Architecture

### 1. **Clear Contracts**
- Shared defines the protocol (how devices communicate)
- Firmware implements packet handlers
- Daemon implements transport and error handling
- UI consumes the API

### 2. **Loose Coupling**
- Daemon never imports from UI
- UI never imports from daemon internals
- Both reference shared contracts
- Changes in daemon don't break UI logic

### 3. **Easy Testing**
- Protocol types testable in isolation (no dependencies)
- Daemon error handling testable with mock errors
- No need for complex error hierarchy in shared

### 4. **Flexibility**
- Daemon can add domain-specific errors (COMPortNotFoundError, FirmwareVersion2Error)
- These don't pollute the shared contract
- New transports (TCP, Bluetooth) add their own error types

### 5. **Consistency**
- Single ProtocolInfo source of truth
- All values come from one place
- Changes to protocol update one place, propagate everywhere

---

## Migration Guide for Next Tasks

### Task 2: CRC16 Implementation

**What to do:**
1. Create `daemon/package.json` with TypeScript/Jest
2. Create `daemon/src/core/protocol/crc16.ts`
3. Import `ProtocolInfo` from shared for CRC parameters
4. Implement CRC16-CCITT algorithm
5. Add unit tests with test vectors
6. No need to define errors in CRC module

**Example:**
```typescript
// daemon/src/core/protocol/crc16.ts
import { ProtocolInfo } from "@nas-controller/shared";

export function calculateCRC16(data: Uint8Array): number {
  let crc = ProtocolInfo.CRCInitialValue;
  // ... algorithm
  return crc;
}
```

### Task 3: Packet Encoder

**What to do:**
1. Create `daemon/src/core/protocol/encoder.ts`
2. Import `Command` type from shared
3. Import CRC16 function
4. Implement packet encoding
5. Return `Uint8Array` matching PROTOCOL_SPEC.md
6. Add unit tests

### Task 4: Packet Decoder

**What to do:**
1. Create `daemon/src/core/protocol/decoder.ts`
2. Import `Response` type from shared
3. Implement packet parsing
4. Return parsed commands or errors
5. Add unit tests

### Task 5: Protocol Validator

**What to do:**
1. Create `daemon/src/core/protocol/validator.ts`
2. Validate packet structure (header, footer, length)
3. Validate CRC16
4. Create first daemon error type: `ProtocolError`
5. Add unit tests

### Task 6: Mock Transport

**What to do:**
1. Create `daemon/src/core/transport/` directory
2. Define `ITransport` interface (abstraction, not in shared)
3. Implement `MockTransport` (simulates firmware)
4. Add unit tests
5. This is where daemon-specific architecture lives

---

## What Shared Provides Now

| Artifact | Type | Purpose | Status |
|----------|------|---------|--------|
| ProtocolInfo | Object | Centralized protocol metadata | ✅ |
| CommandCode | Object | All 16 command codes | ✅ |
| ErrorCode | Enum | Firmware error classification | ✅ |
| RelayState | Enum | ON/OFF states | ✅ |
| FanMode | Enum | MANUAL/AUTO/OFF | ✅ |
| LEDAnimation | Enum | Boot/Idle/Error/etc. | ✅ |
| EventType | Enum | Device/relay/fan/etc. events | ✅ |
| Packet | Interface | Full packet structure | ✅ |
| PacketMetadata | Interface | Sequence, command, length | ✅ |
| SystemStatus | Interface | Hardware status DTO | ✅ |
| ConfigurationData | Interface | Config DTO | ✅ |
| SystemPingRequest | Interface | Strongly-typed ping command | ✅ |
| RelaySetRequest | Interface | Strongly-typed relay command | ✅ |
| AckResponse | Interface | Acknowledgement response | ✅ |
| NakResponse | Interface | Error response | ✅ |
| DeviceConnectedEvent | Interface | Connection event payload | ✅ |
| RelayStateChangedEvent | Interface | Relay change event payload | ✅ |
| Result<T> | Type | Success/Failure result type | ✅ |

**Total: Pure contracts only. No runtime behavior.**

---

## Testing Strategy

### Shared Package Tests
- ✅ Verify all constants match PROTOCOL_SPEC.md
- ✅ Verify all enums have correct values
- ✅ Verify packet structures are well-formed
- ✅ Verify DTOs can be instantiated
- ✅ Verify Result<T> type works correctly
- ❌ NO tests for error handling (moved to daemon)
- ❌ NO tests for state models (moved to daemon)
- ❌ NO tests for interfaces (moved to daemon)

### Daemon Package Tests (Coming Next)
- Encoder/decoder integration tests
- CRC16 with test vectors
- Error classification logic
- Transport abstraction
- State management
- Command queue behavior

---

## Files Changed

| File | Change | Lines |
|------|--------|-------|
| shared/src/protocol.ts | Refactored | 560 (was 332) |
| shared/src/types.ts | Refactored | 38 (was 245) |
| shared/src/errors.ts | DELETED | - |
| shared/src/__tests__/protocol.test.ts | Refactored | 469 (was 332) |
| shared/src/__tests__/errors.test.ts | DELETED | - |

**Total:** 1,067 lines (cleaner, more focused)

---

## Compliance Verification

✅ **Shared is now pure contracts** - No runtime behavior, no error classes, no helper functions

✅ **Protocol values centralized** - ProtocolInfo is single source of truth

✅ **Packet structure defined** - PacketHeader, PacketMetadata, PacketFooter interfaces

✅ **Event payloads typed** - All events have strong types

✅ **Result<T> for error handling** - No thrown exceptions in contracts

✅ **Enumerations complete** - ErrorCode, RelayState, FanMode, LEDAnimation, EventType

✅ **DTOs for all structures** - SystemStatus, ConfigurationData, CapabilityInfo

✅ **Type aliases for clarity** - Uint8, RelayId, Temperature, etc.

✅ **Test suite updated** - All tests verify contracts only

✅ **No daemon dependencies** - Shared doesn't know about Express, Socket.io, or transports

---

## Summary

**Before:** Shared package was a **catch-all** containing both protocol contracts and runtime implementations.

**After:** Shared package is a **pure contract library** with:
- Protocol definitions (commands, responses, enums)
- Data structures (DTOs, packets)
- Event payloads (strongly-typed events)
- Result type (for error handling)

**Daemon will implement:**
- Error classes and helpers
- Transport abstractions and implementations
- Command queue and state store
- Protocol encoding/decoding/validation

**This gives us:**
- Clean architectural boundaries
- Easier testing (contracts have no dependencies)
- Flexibility (daemon can add domain-specific errors)
- Consistency (ProtocolInfo is single source of truth)

---

**Status:** Ready for Task 2 (CRC16 Implementation in Daemon)
