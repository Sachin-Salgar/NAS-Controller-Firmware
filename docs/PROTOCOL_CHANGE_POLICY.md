# Future Protocol Change Policy

**Status:** Active  
**Version:** 1.0.0  
**Date:** 2026-07-20  
**Owner:** Architecture Review  
**Scope:** Guidelines for any modifications to the NAS Controller binary protocol

---

## Overview

This document defines the process for modifying the NAS Controller binary protocol after its initial v1.0.0 release. The goal is to enable evolution while maintaining backward compatibility and ensuring all implementations remain synchronized.

---

## Core Principle

**The protocol is the contract.** All implementations (firmware, daemon, tools, clients) must agree exactly. Changes require careful coordination.

---

## Process for Protocol Modifications

Any change to the protocol must follow this sequence in order:

### 1. Architecture Decision Record (ADR)

**Requirement:** Create a new ADR documenting the proposed change.

**ADR must include:**
- **Problem:** What issue necessitates this change?
- **Decision:** What solution are we implementing?
- **Consequences:** What are the impacts (positive and negative)?
- **Alternatives:** What else was considered and why rejected?
- **Backward Compatibility:** How does this affect existing implementations?

**Example ADR:**
```markdown
# ADR-0004: Extended Temperature Sensor Support

## Problem
Current protocol supports only 4 temperature sensors. Hardware can support 8.

## Decision
Extend TEMP_READ command to support sensor IDs 0-7 (currently 0-3).

## Consequences
- Existing firmware (supports IDs 0-3) remains compatible
- New firmware can support IDs 0-7
- Old daemon can talk to new firmware (gets NAK for ID 4-7)
- New daemon can talk to old firmware

## Backward Compatibility
- Old command requests still work (IDs 0-3)
- New command requests (IDs 4-7) cause NAK on old firmware
- All responses identical format (compatible)
```

**Location:** `docs/adr/00XX-title.md`

### 2. Protocol Version Increment

**Requirement:** Update the protocol version in the specification.

**Versioning scheme:** Semantic Versioning (MAJOR.MINOR.PATCH)

**When to increment:**

| Change Type | Version Update | Example |
|-------------|---|---------|
| **Breaking change** | MAJOR | Remove command, change packet format incompatibly |
| **Backward-compatible addition** | MINOR | Add new command, add new optional field, extend enum |
| **Bug fix** | PATCH | Correct documentation, fix algorithm bug |

**Update locations:**
- `shared/docs/PROTOCOL_SPEC.md` - Update version number
- `shared/src/version.ts` - Update ProtocolVersion constant
- `docs/PROJECT_STATUS.md` - Update protocol version in status

**Example:**
```typescript
// Before (v1.0.0)
export const ProtocolVersion = {
  MAJOR: 1,
  MINOR: 0,
  PATCH: 0,
};

// After (v1.1.0 - backward-compatible addition)
export const ProtocolVersion = {
  MAJOR: 1,
  MINOR: 1,
  PATCH: 0,
};
```

### 3. Canonical Test Vector Updates

**Requirement:** If the change affects packet structure or CRC calculation, generate new test vectors.

**What to do:**

If the change is:
- **Adding a new command:** Create new test vectors for the new command (vectors follow same schema)
- **Extending a command payload:** Generate test vectors for the new payload format
- **Changing CRC algorithm:** Regenerate all existing vectors with new algorithm
- **Changing byte order:** Regenerate all vectors with new byte order
- **Changing packet structure:** Regenerate all affected vectors

**New vector naming:**
- Store in same `shared/docs/protocol/test_vectors/` directory
- Name format: `vector_NNN.json` (sequential)
- Update `README.md` in that directory with new vector list

**Verification steps:**
1. Execute firmware with new input bytes
2. Record actual CRC output from firmware
3. Document firmware version tested against
4. Store in JSON format with full metadata
5. Do NOT calculate manually or assume

**Important:** Test vectors are frozen once verified. Do not modify existing verified vectors; create new ones instead.

### 4. Specification Update

**Requirement:** Update PROTOCOL_SPEC.md with detailed changes.

**Update sections:**
- **Packet Structure** - If packet format changed
- **Command Categories** - If new commands added or existing commands modified
- **Error Codes** - If new error conditions defined
- **CRC Validation** - If algorithm changed
- **State Synchronization** - If connection sequence affected
- **Interoperability Requirement** - Update list of canonical test vectors

**Documentation standards:**
- Include examples with byte-by-byte breakdown
- Document all parameters and their ranges
- Specify error conditions
- Include relevant test vectors

### 5. PROTOCOL_REGISTRY.md Update

**Requirement:** Update command registry with new/modified commands.

**For each changed command:**
- Update description
- Update request/response payload format
- Update error codes
- Update firmware support version
- Update protocol version
- Include before/after examples if applicable

**Example:**
```markdown
### CMD_TEMP_READ (Extended Support)

| Property | Value |
|----------|-------|
| **Command Byte** | 0x40 |
| **Protocol Version** | 1.1+ (extended in 1.1, backward-compat with 1.0) |
| **Firmware Support** | v1.0+ (IDs 0-3), v1.1+ (IDs 0-7) |
| **Request Payload** | sensor_id (1 byte: 0-7, previously 0-3 only) |
| **Response Payload** | sensor_id (1 byte), temperature (2 bytes: int16) |
| **Error Codes** | 0x01: Invalid sensor ID (now range 0-7), 0x02: Sensor error |
| **Changes in v1.1** | Extended sensor_id range from 0-3 to 0-7 |
```

### 6. Shared Package Types Update

**Requirement:** Update TypeScript types to reflect changes.

**Location:** `shared/src/protocol.ts` and related files

**Update:**
- New command codes in `CommandCode` enum
- New request types (interfaces)
- New response types (interfaces)
- New error codes in `ErrorCode` enum
- New feature flags if applicable
- Union types (`Command`, `Response`, etc.)

**Example:**
```typescript
// shared/src/protocol.ts

// 1. Add new command code (backward-compatible addition)
export const CommandCode = {
  // ... existing ...
  STORAGE_WRITE: 0x82,  // New in v1.1
};

// 2. Add request type
export interface StorageWriteRequest extends BaseCommand {
  command: typeof CommandCode.STORAGE_WRITE;
  address: number;
  data: Uint8Array;
}

// 3. Update union type
export type Command = 
  | SystemPingRequest
  | RelaySetRequest
  | ... 
  | StorageWriteRequest;  // New
```

### 7. Backward Compatibility Review

**Requirement:** Document how old and new implementations interact.

**Create compatibility matrix:**

| Old Firmware | New Firmware | Old Daemon | New Daemon | Status |
|---|---|---|---|---|
| v1.0 | v1.1 | v1.0 | v1.1 | ✅ All compatible |
| v1.0 | v1.0 | v1.1 | v1.1 | ✅ Works fine |
| v1.0 | v1.0 | v1.0 | v1.0 | ✅ Original version |

**For breaking changes:**

Explicitly document:
- When old implementations become incompatible
- What happens if old sends command to new (error response? silent failure?)
- Migration path for users on old versions
- Deprecation timeline (if applicable)

**For backward-compatible additions:**

Document:
- Old implementations ignore new fields gracefully
- New implementations handle missing new fields
- Test vectors prove compatibility

### 8. All Layer Implementation

**Requirement:** Update all implementations in sequence.

**Sequence:**
1. **Firmware** - Reference implementation (executes first to verify vectors)
2. **Daemon** - Must match firmware output exactly for test vectors
3. **Test utilities** - Verify consistency between layers
4. **Frontend** - Update UI to support new features
5. **Documentation** - Update API documentation for new commands

**Verification at each step:**
- New implementation passes all test vectors (old + new)
- Interoperability with other layer versions documented
- Code review against specification

### 9. Integration Testing

**Requirement:** Verify all layer combinations work.

**Test matrix:**
- Old firmware ↔ new daemon ✅
- New firmware ↔ old daemon ✅
- New firmware ↔ new daemon ✅
- Old firmware ↔ old daemon ✅

**Integration tests verify:**
- Commands sent successfully
- Responses parsed correctly
- Errors handled appropriately
- State stays synchronized

### 10. Documentation & Release Notes

**Requirement:** Document changes for all audiences.

**Update:**
- `docs/PROJECT_STATUS.md` - Version history section
- Release notes - User-facing explanation of changes
- Migration guide (if breaking) - How to upgrade
- API documentation - New command usage examples
- CHANGELOG.md in each affected component

---

## Change Categories & Examples

### Category 1: New Command (Backward Compatible)

**Example:** Add new STORAGE_READ command

**Process:**
1. ADR: Justify need for storage support
2. Version: Update to 1.1.0 (MINOR bump)
3. Test vectors: Create vectors for new command
4. Specification: Document new command in PROTOCOL_SPEC.md
5. Registry: Add STORAGE_READ to PROTOCOL_REGISTRY.md
6. Types: Add CommandCode.STORAGE_READ, StorageReadRequest, StorageReadResponse
7. Compatibility review: Old firmware returns NAK for unknown command, other commands still work
8. Implementation: Update firmware, daemon, clients
9. Integration test: Verify old/new combinations
10. Release: Update version numbers and documentation

**Impact:** MINOR version bump (v1.0.0 → v1.1.0)

### Category 2: Extend Command Payload (Backward Compatible)

**Example:** Extend TEMP_READ to support sensor IDs 0-7 (was 0-3)

**Process:**
1. ADR: Hardware support enables more sensors
2. Version: Update to 1.1.0 (MINOR bump)
3. Test vectors: Create vectors for new payload format (IDs 4-7)
4. Specification: Document extended sensor_id range
5. Registry: Update TEMP_READ error codes and range
6. Types: Update TemperatureReadRequest (sensor_id now 0-7)
7. Compatibility: Old requests (0-3) still work, new requests (4-7) get NAK on v1.0
8. Implementation: Update firmware, daemon
9. Integration test: Old daemon can use 0-3, new daemon can use 0-7
10. Release: Version update and documentation

**Impact:** MINOR version bump (v1.0.0 → v1.1.0)

### Category 3: Packet Structure Change (Breaking)

**Example:** Change packet header from 0x55AA to 0xABCD due to protocol collision

**Process:**
1. ADR: Justify breaking change (critical issue)
2. Version: Update to 2.0.0 (MAJOR bump)
3. Test vectors: Regenerate all vectors with new header
4. Specification: Update packet structure section completely
5. Registry: Update all command documentation
6. Types: Update PacketHeader, all packet-related types
7. Compatibility: v1.x and v2.x are **incompatible** - document clearly
8. Implementation: Update firmware, daemon, all clients
9. Integration test: v1 ↔ v1 (old path), v2 ↔ v2 (new path)
10. Release: Major version, migration guide, deprecation plan for v1

**Impact:** MAJOR version bump (v1.0.0 → v2.0.0)
**Effort:** Very high, only for critical issues

### Category 4: Bug Fix in CRC Algorithm

**Example:** CRC calculation incorrect in specification

**Process:**
1. ADR: Document the bug and fix
2. Version: Update to 1.0.1 (PATCH bump)
3. Test vectors: Regenerate with correct algorithm
4. Specification: Correct the CRC algorithm section
5. Types: CRC constants unchanged (only correction)
6. Compatibility: If bug was in firmware already, document actual behavior
7. Implementation: Fix firmware, daemon, tests
8. Integration test: Verify all layers now produce same output
9. Release: PATCH version, "Critical bug fix"

**Impact:** PATCH version bump (v1.0.0 → v1.0.1)

### Category 5: New Error Code (Backward Compatible)

**Example:** Add new error code DEVICE_LOCKED for configuration operations

**Process:**
1. ADR: Configuration locking behavior
2. Version: Update to 1.1.0 (MINOR bump)
3. Test vectors: Create vector for command that returns new error
4. Specification: Add error code to error codes table
5. Registry: Update commands that can return new error
6. Types: Add ErrorCode.DEVICE_LOCKED
7. Compatibility: Old daemon won't recognize new error, but can treat as generic error
8. Implementation: Update firmware, daemon error handling
9. Integration test: Verify new error is returned and handled
10. Release: Version update

**Impact:** MINOR version bump (v1.0.0 → v1.1.0)

---

## Change Approval Process

Before implementing a protocol change:

### 1. Stakeholder Review
- [ ] Architecture lead reviews ADR
- [ ] Protocol owner approves change
- [ ] Backward compatibility verified
- [ ] Implementation effort estimated

### 2. Technical Design
- [ ] Test vectors designed (input/expected output)
- [ ] Specification section drafted
- [ ] Type changes identified
- [ ] Integration test approach defined

### 3. Implementation
- [ ] Firmware updated and tested
- [ ] Daemon updated and test against vectors
- [ ] All layer integration tests pass
- [ ] Code review approved

### 4. Verification
- [ ] All canonical test vectors pass
- [ ] Backward compatibility matrix verified
- [ ] Documentation complete and accurate
- [ ] Release notes prepared

### 5. Release
- [ ] Version numbers updated
- [ ] All affected components released
- [ ] Release notes published
- [ ] Deployment plan documented

---

## When NOT to Change the Protocol

❌ **Do NOT modify the protocol for:**
- Internal implementation optimization
- Performance improvement without protocol impact
- Convenience (reorganizing commands)
- Refactoring without behavior change
- Adding debugging features
- Changing internal state representation

✅ **INSTEAD:**
- Optimize within existing packet format
- Use existing commands more efficiently
- Add features using existing command set
- Use diagnostics commands for debugging
- Keep internal state private to each layer

---

## Deprecation Policy

When removing protocol features:

### Phase 1: Deprecation Notice
- Add note to command documentation: "DEPRECATED as of v1.2.0"
- Continue supporting in firmware/daemon
- Recommend users migrate to new approach
- Provide migration guide

### Phase 2: Support Continues
- Keep support for 2+ firmware versions
- Example: Deprecated in v1.2, removed in v2.0
- Users have time to update

### Phase 3: Removal
- Only remove in MAJOR version bump
- Clearly document in breaking changes
- Provide clear error message if old command received

---

## Monitoring Protocol Adherence

### Continuous Verification
- [ ] All test vectors pass on every build
- [ ] Byte-for-byte comparison between layers (daemon vs firmware)
- [ ] No protocol drift (undocumented behavior changes)
- [ ] Version consistency (firmware/daemon versions documented)

### Automated Checks
- Test harness compares layer outputs
- CI/CD validates all test vectors
- Build fails if any vector doesn't match
- Regression tests prevent historical bugs

### Manual Review
- Code review checks spec compliance
- Architecture review approves changes
- Integration testing before release

---

## Document Updates Required

Whenever protocol changes, update these in order:

1. **ADR** (docs/adr/NNXX-title.md)
2. **PROTOCOL_SPEC.md** (shared/docs/)
3. **PROTOCOL_REGISTRY.md** (shared/docs/)
4. **PROTOCOL_TEST_VECTORS.md** (shared/docs/) - Add new vectors
5. **PROTOCOL_FREEZE_CHECKLIST.md** (docs/) - Update status
6. **Shared Types** (shared/src/protocol.ts)
7. **Firmware** (firmware/src/)
8. **Daemon** (daemon/src/)
9. **PROJECT_STATUS.md** (docs/) - Version history
10. **Release Notes** (CHANGELOG.md in affected components)

---

## Examples of Proper Changes

### ✅ GOOD: Backward-Compatible New Command

```
ADR-0004: Storage Access Support

Problem: Need to read/write persistent storage on device
Decision: Add new commands STORAGE_READ/STORAGE_WRITE in reserved range 0x82-0x83
Version: 1.0.0 → 1.1.0 (MINOR bump)
Compatibility: Old firmware rejects with NAK, old daemon doesn't send
Test vectors: New vectors for storage commands
```

### ✅ GOOD: Backward-Compatible Error Code

```
ADR-0005: Configuration Lock Error

Problem: Need to report when config is locked by another operation
Decision: Add new ErrorCode.CONFIG_LOCKED = 0x0B
Version: 1.0.0 → 1.1.0 (MINOR bump)
Compatibility: Old daemon treats as unknown error, continues
Test vectors: Vector for command returning new error
```

### ❌ WRONG: Undocumented Change

```
"Let's change CRC from Modbus to CCITT to match our other projects"
(No ADR, no test vectors, breaks compatibility, no version bump)
```

### ❌ WRONG: Internal Optimization

```
"Reorganize shared/src/protocol.ts to put types in separate files"
(Not a protocol change, reorganizes shared package inappropriately)
```

---

## Contact & Questions

When in doubt about protocol changes, contact the architecture review board.

**Process:** Create ADR, seek feedback before implementation.

---

**Status:** Active and binding for all protocol modifications  
**Effective:** From v1.0.0 forward  
**Last Updated:** 2026-07-20
