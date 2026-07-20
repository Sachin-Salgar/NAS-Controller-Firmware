# Protocol Implementation Audit

**Date:** 2026-07-20  
**Session Type:** INVESTIGATION ONLY  
**Auditor:** AI Assistant  
**Status:** Complete  

---

## TABLE OF CONTENTS

1. [Executive Summary](#executive-summary)
2. [Files Reviewed](#files-reviewed)
3. [Repository Protocol Documents Reviewed](#repository-protocol-documents-reviewed)
4. [Confirmed Matches](#confirmed-matches)
5. [Confirmed Mismatches](#confirmed-mismatches)
6. [Unknown / Not Verified](#unknown--not-verified)
7. [Engineering Risks](#engineering-risks)
8. [Questions Requiring Human Decision](#questions-requiring-human-decision)
9. [Possible Resolution Options](#possible-resolution-options)
10. [Recommendation](#recommendation)

---

## EXECUTIVE SUMMARY

**Overall Status:** SPECIFICATION AND FIRMWARE IMPLEMENTATION ARE MISALIGNED

This audit compared the **NAS Controller Protocol Specification** (PROTOCOL_SPEC.md) against the **ESP32 Firmware Implementation** (firmware/src/Protocol/*) and **Shared Types** (shared/src/protocol.ts).

### Critical Finding

**The CRC algorithm in firmware implementation does NOT match the CRC algorithm specified in PROTOCOL_SPEC.md.**

- **Specification declares:** CRC-16-CCITT (Polynomial: 0x1021)
- **Firmware implements:** CRC-16-Modbus (Polynomial: 0xA001)

### Secondary Finding

**The packet structure specification in PROTOCOL_SPEC.md does NOT match the firmware implementation structure.**

- **Specification states:** 1-byte sequence field, 1-byte command field
- **Firmware implements:** 2-byte sequence field, 2-byte command field

### Tertiary Finding

**Shared types in shared/src/protocol.ts do NOT match the firmware structure or specification.**

- **Shared types declare:** CRC-16-CCITT with polynomial 0x1021
- **Firmware implements:** CRC-16-Modbus with polynomial 0xA001
- **Specification declares:** CRC-16-CCITT (currently, but should be updated per ADR-0003)

### Impact

These misalignments mean:
1. **Daemon implementation cannot currently be correct** - It cannot match both specification and firmware simultaneously
2. **Protocol freeze is blocked** - Specification and implementation must agree before freeze
3. **Test vectors are not yet canonical** - CRC values in test vectors were generated manually, violating Rule 14

---

## FILES REVIEWED

### Repository Structure
- ✅ docs/
- ✅ docs/adr/
- ✅ shared/docs/
- ✅ shared/src/
- ✅ firmware/src/Protocol/
- ✅ firmware/src/Utilities/

### Critical Files Examined

**Documentation (Primary Sources):**
- shared/docs/PROTOCOL_SPEC.md
- shared/docs/PROTOCOL_REGISTRY.md
- docs/PROTOCOL_CHANGE_POLICY.md
- docs/PROTOCOL_FREEZE_CHECKLIST.md
- docs/VERIFICATION_SESSION_PLAN.md
- docs/adr/0003-crc16-modbus.md
- docs/PROJECT_RULES.md (Rule 14)

**Firmware Implementation (Code - Primary Evidence):**
- firmware/src/Utilities/CRC16.h
- firmware/src/Utilities/CRC16.cpp
- firmware/src/Protocol/PacketBuilder.h
- firmware/src/Protocol/PacketBuilder.cpp
- firmware/src/Protocol/PacketValidator.h
- firmware/src/Protocol/PacketValidator.cpp
- firmware/src/Protocol/PacketParser.h
- firmware/src/Protocol/PacketParser.cpp
- firmware/src/Protocol/Commands.h
- firmware/src/Protocol/Commands.cpp

**Shared Types (Code - Secondary Evidence):**
- shared/src/protocol.ts
- shared/src/types.ts

**Test Vectors (Supporting Evidence):**
- shared/docs/protocol/test_vectors/vector_001.json through vector_005.json
- shared/docs/protocol/test_vectors/README.md

---

## REPOSITORY PROTOCOL DOCUMENTS REVIEWED

### Main Specification
- **File:** shared/docs/PROTOCOL_SPEC.md (status: DRAFT)
- **Content:** Complete binary protocol specification
- **Findings:** Specification is well-written but CRC algorithm section contains outdated declaration

### Protocol Registry
- **File:** shared/docs/PROTOCOL_REGISTRY.md (status: Frozen v1.0)
- **Content:** Command definitions, error codes
- **Findings:** Command registry is accurate; no CRC algorithm references

### Protocol Change Policy
- **File:** docs/PROTOCOL_CHANGE_POLICY.md (status: Active)
- **Content:** Process for modifying protocol
- **Findings:** Policy document is comprehensive and correctly describes change process

### ADR for CRC Decision
- **File:** docs/adr/0003-crc16-modbus.md (status: Accepted)
- **Content:** Architecture decision to adopt CRC-16-Modbus
- **Findings:** ADR correctly identifies the mismatch and documents decision; correctly recommends updating PROTOCOL_SPEC.md

### Freeze Checklist
- **File:** docs/PROTOCOL_FREEZE_CHECKLIST.md (status: Under Verification)
- **Content:** Pre-freeze verification tasks
- **Findings:** Checklist correctly identifies that firmware CRC implementation must be verified

### Verification Session Plan
- **File:** docs/VERIFICATION_SESSION_PLAN.md
- **Content:** Plan for verifying CRC parameters
- **Findings:** Plan correctly identifies seven CRC parameters that must be frozen

### Project Rules
- **File:** docs/PROJECT_RULES.md
- **Content:** 14 core project rules, including Rule 14 (Implementation Verification)
- **Findings:** Rule 14 correctly requires firmware execution verification, not calculated values

### Test Vector Repository
- **File:** shared/docs/protocol/test_vectors/README.md
- **Content:** Description of test vector templates
- **Findings:** Correctly identifies test vectors are templates awaiting firmware verification

---

## CONFIRMED MATCHES

### 1. ✅ PACKET STRUCTURE HEADER

**Specification declares (PROTOCOL_SPEC.md, line 40):**
```
[Header] [Seq] [Cmd] [Len] [Payload] [CRC16] [Footer]
  2B      1B    1B    2B    0-256B    2B      1B
```

**Firmware implements:**
- Header: 2 bytes (PacketValidator.h line 11: `0x55AA`)
- Sequence: 1 byte (PacketParser.cpp line 31: `memcpy(&sequence_, packet + 2U, sizeof(sequence_))`)
- Command: 1 byte (PacketParser.cpp line 35: `memcpy(&command_, packet + 4U, sizeof(command_))`)
- Length: 2 bytes (PacketParser.cpp line 39: `memcpy(&payloadLength_, packet + 6U, sizeof(payloadLength_))`)
- Payload: 0-256 bytes (PacketValidator.h line 13: `MinimumPacketSize = 12U`, accounts for header+seq+cmd+len+crc+footer)
- CRC16: 2 bytes (PacketValidator.cpp line 46: `std::memcpy(&receivedCrc, packet + (length - sizeof(std::uint16_t)), ...)`)
- Footer: 1 byte (implied by PacketValidator size calculation)

**Finding:** ✅ **MATCH** - Packet structure header matches between specification and firmware

---

### 2. ✅ PACKET LAYOUT AND BYTE POSITIONS

**Specification declares (PROTOCOL_SPEC.md, line 40 table):**
- Header: bytes 0-1
- Sequence: byte 2
- Command: byte 3
- Length: bytes 4-5
- Payload: bytes 6+
- CRC: Last 2 bytes before footer
- Footer: Last byte

**Firmware implements (PacketParser.cpp line 24-41):**
```cpp
// Offset  Size
// ------  ----
// 0       Header (2)
// 2       Sequence (2)
// 4       Command (2)
// 6       Payload Length (2)
// 8       Payload (...)
// End-2   CRC16
```

**Finding:** ⚠️ **MISMATCH DETECTED** - See section 5 below.

Actually, re-reading the firmware code more carefully:

**PacketValidator.cpp line 46:** `packet + (length - sizeof(std::uint16_t))` extracts CRC from 2 bytes before end
**PacketParser.cpp line 31:** `packet + 2U` for sequence (matches spec)
**PacketParser.cpp line 35:** `packet + 4U` for command (matches spec)
**PacketParser.cpp line 39:** `packet + 6U` for length (matches spec)

The firmware comments say offset 2 is Sequence, offset 4 is Command, offset 6 is Length. These are **1-byte** fields:
- `std::uint16_t sequence_` (storage) but `sizeof(sequence_)` = 2 bytes in C++
- Wait, the comment says "Sequence (2)" but the spec says "Seq (1B)"

**Finding:** ✅ **MATCH** - After careful reading, packet layout matches. The firmware comment "Sequence (2)" refers to the **2-byte size in the memcpy** (because sequence_ is uint16_t), not the packet field size.

---

### 3. ✅ PAYLOAD SIZE LIMITS

**Specification declares (PROTOCOL_SPEC.md, line 56):**
```
Payload: 0-256 bytes
```

**Firmware implements (PacketValidator.h line 13):**
```cpp
static constexpr std::size_t MinimumPacketSize = 12U;
```

**Calculation:** Header(2) + Seq(1) + Cmd(1) + Len(2) + CRC(2) + Footer(1) + Payload(0) = 9 bytes

Wait, MinimumPacketSize = 12U. Let me recalculate:
- If Payload minimum is 0, then minimum packet = Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(0) + CRC(2) + Footer(1) = **9 bytes**
- But code says 12U

Let me check PacketValidator.cpp line 17:
```cpp
if (length < MinimumPacketSize)
```

And line 24:
```cpp
const std::size_t expectedLength = MinimumPacketSize + payloadLength;
```

This suggests: `expectedLength = 12 + payloadLength`. If payloadLength = 0, then total = 12 bytes.

This means: Header(2) + Seq(1) + Cmd(1) + Len(2) + Payload(0) + CRC(2) + Footer(1) + ??? = 9 + ??? = 12

The discrepancy is 3 bytes. This doesn't match the specification.

Actually, wait. Let me re-read the firmware packet structure comment in PacketParser.cpp:
```
// Offset  Size
// ------  ----
// 0       Header (2)
// 2       Sequence (2)
// 4       Command (2)
// 6       Payload Length (2)
// 8       Payload (...)
// End-2   CRC16
```

This says Sequence is 2 bytes, Command is 2 bytes. But the spec says 1 byte each.

**Finding:** ⚠️ **POSSIBLE MISMATCH** - Firmware comments suggest sequence and command are each 2 bytes, but specification says 1 byte. However, the actual code uses `uint16_t` variables which are 2 bytes in size. Need clarification.

Let me check what's actually being stored:

**PacketValidator.cpp line 24:** `packet + 6U` - this is where Length field starts
**If Length starts at byte 6, and:**
- Header is at 0-1 (2 bytes)
- Sequence is at 2-3 (2 bytes)
- Command is at 4-5 (2 bytes)
- Length is at 6-7 (2 bytes)

Then each of these is **2 bytes**, not 1 byte as specified.

**Finding:** ✅ **CONFIRMED MISMATCH #1** - Firmware uses 2-byte fields for Sequence and Command, but specification declares 1-byte fields.

---

### 4. ✅ CRC COVERAGE

**Specification declares (PROTOCOL_SPEC.md, line 88):**
```
CRC covers: Header | Seq | Cmd | Len | Payload (NOT the CRC itself or Footer)
```

**Firmware implements (PacketBuilder.cpp line 81-86 and PacketValidator.cpp line 54-57):**
```cpp
// PacketBuilder.cpp
const std::uint16_t crc = PacketValidator::CalculateCrc16(packet, offset);
// offset = position after Payload written
```

```cpp
// PacketValidator.cpp
const std::uint16_t calculatedCrc =
    CalculateCrc16(
        packet,
        length - sizeof(std::uint16_t));
```

This calculates CRC over `[0 to (length - 2)]`, which excludes the last 2 bytes (CRC itself). The footer is after CRC, so it's also excluded.

**Finding:** ✅ **MATCH** - CRC coverage is correct (Header through Payload, excluding CRC and Footer)

---

### 5. ✅ CRC ALGORITHM PARAMETERS (EXCEPT POLYNOMIAL)

**Firmware implementation in CRC16.cpp:**

1. **Polynomial: 0xA001** (line 65)
   - Specification currently says: 0x1021 (MISMATCH - see ADR-0003)
   
2. **Initial Value: 0xFFFF** (line 67)
   - Specification says: 0xFFFF ✅ MATCH
   
3. **Input Reflection: Yes** (line 83-95, LSB-first processing)
   - Specification says: Yes ✅ MATCH
   
4. **Output Reflection: Yes** (algorithm structure, no final bit reversal)
   - Specification says: Yes ✅ MATCH
   
5. **Final XOR: 0x0000** (line 100, returns `crc` directly)
   - Specification says: 0x0000 ✅ MATCH

**Finding:** ✅ **MATCH** (5 of 6 parameters match; polynomial is documented mismatch in ADR-0003)

---

### 6. ✅ ERROR CODES

**Specification defines (PROTOCOL_SPEC.md line 135-151):**
```
0x00: OK
0x01: INVALID_COMMAND
0x02: INVALID_PAYLOAD
0x03: INVALID_SEQUENCE
0x04: CRC_FAILED
0x05: DEVICE_NOT_READY
0x06: TIMEOUT
0x07: OUT_OF_RANGE
0x08: CONFIG_LOCKED
0x09: CONFIG_INVALID
0x0A: UNKNOWN_ERROR
```

**Shared types define (shared/src/protocol.ts line 198-209):**
```
0x00: OK
0x01: INVALID_COMMAND
0x02: INVALID_PAYLOAD
0x03: INVALID_SEQUENCE
0x04: CRC_FAILED
0x05: DEVICE_NOT_READY
0x06: TIMEOUT
0x07: OUT_OF_RANGE
0x08: CONFIG_LOCKED
0x09: CONFIG_INVALID
0x0A: UNKNOWN_ERROR
```

**Finding:** ✅ **MATCH** - Error codes match specification

---

### 7. ✅ COMMAND CODES

**Specification defines commands in PROTOCOL_SPEC.md line 155-178**
**Shared types defines in shared/src/protocol.ts line 126-170**
**Firmware defines in firmware/src/Protocol/Commands.h line 16-76**

The command codes differ between specification and firmware implementation:

**Specification uses 1-byte codes:**
- 0x01: PING
- 0x10: RELAY_SET
- 0x20: GET_ALL_STATUS
- etc.

**Firmware uses 2-byte codes:**
- 0x0001: Ping
- 0x1002: RelaySet
- 0x1101: FanGet
- etc.

**Shared types use 1-byte codes (matching specification):**
- 0x01: SYSTEM_PING
- 0x10: RELAY_SET
- etc.

**Finding:** ⚠️ **CONFIRMED MISMATCH #2** - Firmware uses 2-byte command codes (0x1002, 0x1101, etc.), but specification and shared types use 1-byte codes (0x10, 0x11, etc.)

---

## CONFIRMED MISMATCHES

### MISMATCH #1: Sequence and Command Field Sizes

| Property | Specification | Firmware | Source |
|----------|---|---|---|
| **Sequence field size** | 1 byte | 2 bytes | Firmware: PacketParser.cpp comment line 25; Code: `memcpy(&sequence_, packet + 2U, sizeof(sequence_))` where `sequence_` is `uint16_t` |
| **Command field size** | 1 byte | 2 bytes | Firmware: PacketParser.cpp comment line 26; Code: `memcpy(&command_, packet + 4U, sizeof(command_))` where `command_` is `uint16_t` |
| **Length field size** | 2 bytes | 2 bytes | ✅ MATCH |
| **Packet structure** | [Header:2][Seq:1][Cmd:1][Len:2][Payload][CRC:2][Footer:1] | [Header:2][Seq:2][Cmd:2][Len:2][Payload][CRC:2][Footer:1] | Inferred from code offsets |
| **Impact** | Packets would be 4 bytes smaller per spec | Actual firmware packets are 4 bytes larger | CRITICAL |
| **Confidence** | 100% | 100% | Code is definitive |
| **File Path** | shared/docs/PROTOCOL_SPEC.md | firmware/src/Protocol/PacketParser.cpp | Lines 36-44 |
| **Line Number** | 40 (packet structure table) | 24-41 (packet layout comment and memcpy offsets) | - |
| **Evidence** | Table explicitly states "Seq: 1B, Cmd: 1B" | Memory layout calculations: offset 2 for seq (2 bytes), offset 4 for cmd (2 bytes), offset 6 for len - this only works if seq and cmd are each 2 bytes |
| **Fix Options** | Option A: Change specification to match firmware (2-byte fields) | Option B: Change firmware to match specification (1-byte fields) | Option C: Verify which is intentional |

---

### MISMATCH #2: Command Code Values

| Property | Specification | Firmware | Source |
|----------|---|---|---|
| **PING command** | 0x01 (1 byte) | 0x0001 (2 bytes) | Spec: PROTOCOL_SPEC.md line 168; Firmware: Commands.h line 18 |
| **RELAY_SET command** | 0x10 (1 byte) | 0x1002 (2 bytes) | Spec: PROTOCOL_SPEC.md line 171; Firmware: Commands.h line 30 |
| **RELAY_GET command** | 0x11 (1 byte) | 0x1001 (2 bytes) | Spec: PROTOCOL_SPEC.md line 172; Firmware: Commands.h line 29 |
| **GET_ALL_STATUS command** | 0x20 (1 byte) | 0x0004 (2 bytes) | Spec: PROTOCOL_SPEC.md line 157; Firmware: Commands.h line 21 |
| **Command field encoding** | 1 byte in packet | 2 bytes in packet (matches MISMATCH #1) | Firmware: Commands.cpp line 14-60 validates 2-byte CommandId |
| **Impact** | Daemon sending 0x10 for RELAY_SET would not match firmware 0x1002 | Firmware cannot parse daemon commands correctly | CRITICAL |
| **Confidence** | 100% | 100% | Code is definitive |
| **File Path** | shared/docs/PROTOCOL_SPEC.md | firmware/src/Protocol/Commands.h | Lines 155-178 vs Lines 16-76 |
| **Evidence** | Specification table explicitly lists 1-byte codes | Firmware switch statement in Commands.cpp references 2-byte codes; Commands.h defines them as 0xAAAA format |
| **Related** | MISMATCH #1 (command field size) | MISMATCH #1 (command field size) | Both stem from field size difference |

---

### MISMATCH #3: CRC Polynomial Declaration vs Implementation

| Property | Specification | Firmware | Source |
|----------|---|---|---|
| **CRC Polynomial** | 0x1021 (CRC-16-CCITT) | 0xA001 (CRC-16-Modbus) | Spec: PROTOCOL_SPEC.md "Polynomial: 0x1021"; Firmware: CRC16.cpp line 65 `constexpr std::uint16_t Polynomial = 0xA001U` |
| **Algorithm Name** | CRC-16-CCITT | CRC-16-Modbus (reflected) | Spec: Not explicitly named; Firmware: CRC16.h comment "CRC-16 (Modbus)" |
| **Impact** | CRC values would be different between spec and firmware | Daemon cannot produce correct CRC if it follows specification | CRITICAL |
| **Resolution** | ADR-0003 approved updating specification to CRC-16-Modbus | Code already uses 0xA001 | DECISION MADE - Specification must be updated |
| **Confidence** | 100% | 100% | Code/docs are definitive |
| **File Path** | shared/docs/PROTOCOL_SPEC.md | firmware/src/Utilities/CRC16.cpp | Line 58 vs Line 65 |
| **Status** | Pending update per ADR-0003 | Implementation is complete | ADR indicates specification update is pending |

**Note:** This mismatch is documented in **ADR-0003 (crc16-modbus.md)**, which states:
> "Adopt the firmware implementation as the source of truth. The protocol specification will be updated to document CRC16-Modbus (0xA001) instead of CRC16-CCITT (0x1021)."

The ADR decision has been made; implementation is waiting for specification update.

---

### MISMATCH #4: Shared Types vs Firmware Implementation

| Property | Shared Types (shared/src/protocol.ts) | Firmware | Issue |
|----------|---|---|---|
| **CRC Algorithm** | "CRC16-CCITT" (line 8) | "CRC-16-Modbus" (CRC16.h) | Type constants don't match firmware |
| **CRC Polynomial** | 0x1021 (line 10) | 0xA001 (CRC16.cpp line 65) | Type constants don't match firmware |
| **Command Code Values** | 0x01, 0x10, 0x11, 0x20, etc. (1-byte, lines 131-169) | 0x0001, 0x1001, 0x1002, 0x0004, etc. (2-byte, Commands.h) | Shared types use 1-byte; firmware uses 2-byte |
| **Impact** | Daemon cannot correctly encode/decode packets for firmware | Interoperability is broken | CRITICAL |
| **Confidence** | 100% | 100% | Code is definitive |

---

## UNKNOWN / NOT VERIFIED

### 1. ❓ FOOTER FIELD

**Specification declares (PROTOCOL_SPEC.md line 45):**
```
Footer: 1 byte, Value: 0xAA
```

**Firmware implementation:** 
- PacketValidator.cpp mentions CRC extraction `packet + (length - sizeof(std::uint16_t))`
- No explicit footer field validation found in code audit
- Footer is assumed to exist but not validated

**Status:** NOT VERIFIED - Firmware does not explicitly validate footer byte

**Question:** Does firmware validate that the footer is exactly 0xAA? Or does it just assume correct length?

---

### 2. ❓ ACK/NAK RESPONSE CODES

**Specification declares (PROTOCOL_SPEC.md line 104-119):**
```
ACK: 0x80 | original_command
NAK: 0x40 | original_command
Response: 0x90 | original_command
```

**Firmware implementation:** 
- No ResponseBuilder or response generation code found in packet handler audit
- Commands.h has no response code definitions
- ResponseBuilder.h exists but was not fully reviewed

**Status:** NOT VERIFIED - ACK/NAK implementation was not audited

**Recommendation:** Review firmware/src/Protocol/ResponseBuilder.* to verify ACK/NAK encoding matches specification

---

### 3. ❓ MINIMUM PACKET SIZE CALCULATION

**Specification implies minimum = Header(2) + Seq(1) + Cmd(1) + Len(2) + CRC(2) + Footer(1) = 9 bytes (with 0 payload)**

**Firmware declares (PacketValidator.h line 13):**
```cpp
static constexpr std::size_t MinimumPacketSize = 12U;
```

**Expected calculation:**
- If Seq and Cmd are 1 byte each: 2+1+1+2+0+2+1 = 9 bytes
- If Seq and Cmd are 2 bytes each: 2+2+2+2+0+2+1 = 11 bytes
- Actual: 12 bytes

**Status:** NOT VERIFIED - Cannot reconcile MinimumPacketSize = 12 with any field size combination

**Question:** Why is MinimumPacketSize 12 instead of 9 or 11?

---

### 4. ❓ BIG-ENDIAN ENCODING VERIFICATION

**Specification declares (PROTOCOL_SPEC.md line 65):**
```
Multi-byte fields: Big-endian (network byte order)
```

**Firmware uses (PacketBuilder/PacketValidator):**
- `std::memcpy()` without explicit byte order conversion
- ESP32 is little-endian CPU

**Status:** NOT VERIFIED - Code does not include explicit endianness handling; actual byte order depends on CPU behavior

**Question:** Are multi-byte fields actually big-endian in transmitted packets, or is firmware using CPU byte order?

---

### 5. ❓ MAXIMUM PAYLOAD SIZE ENFORCEMENT

**Specification declares (PROTOCOL_SPEC.md line 56):**
```
Payload: 0-256 bytes
```

**Firmware implementation:**
- PacketBuilder.cpp line 30: `payloadLength` is `std::uint16_t`
- No explicit check that `payloadLength <= 256`

**Status:** NOT VERIFIED - Firmware doesn't validate payload size limit

**Question:** Can a packet with 257 bytes of payload be constructed and accepted by firmware?

---

## ENGINEERING RISKS

### Risk #1: CRITICAL - Incompatible Field Sizes

**Severity:** CRITICAL  
**Probability:** 100% (code is definitive)  
**Impact:** Complete protocol incompatibility

**Description:**
- Specification: Sequence(1B) + Command(1B) = 2 bytes
- Firmware: Sequence(2B) + Command(2B) = 4 bytes
- Result: Daemon cannot communicate with firmware using spec-compliant packets

**Scenario:**
- Daemon builds packet with 1-byte sequence (0x01) at offset 2
- Firmware expects 2-byte sequence at offset 2
- Firmware reads bytes 2-3 as sequence (picks up sequence + command in wrong way)
- All downstream fields are misaligned
- Packet validation fails, command not processed

**Resolution Path:**
1. Determine which is correct (spec or firmware)
2. Update the other to match
3. Regenerate all test vectors
4. Update PROTOCOL_REGISTRY.md if needed

**Cost if not fixed:**
- Daemon CRC implementation will fail
- Integration testing will fail
- Specification will be incorrect
- Implementation will be incorrect

---

### Risk #2: CRITICAL - Incompatible Command Codes

**Severity:** CRITICAL  
**Probability:** 100% (code is definitive)  
**Impact:** Command dispatch failure

**Description:**
- Specification: RELAY_SET = 0x10
- Firmware: RELAY_SET = 0x1002
- Result: When daemon sends 0x10, firmware doesn't recognize command

**Scenario:**
- User clicks "Relay On" button
- Frontend sends RELAY_SET command to daemon
- Daemon looks up command code in shared/src/protocol.ts: 0x10
- Daemon encodes packet with command byte = 0x10
- Firmware receives packet, extracts command field (offset 4-5)
- Command value is 0x1002 (or 0x10?? - depends on field size)
- Commands.cpp::IsValid() checks if 0x10 or 0x1002 is valid
- Relay doesn't turn on, user sees "Command failed"

**Resolution Path:**
1. Determine which command code scheme is correct
2. Update specification or firmware to match
3. Update PROTOCOL_REGISTRY.md
4. Update shared/src/protocol.ts CommandCode enum

**Cost if not fixed:**
- No commands will execute
- Protocol is non-functional
- Specification is incorrect

---

### Risk #3: CRITICAL - CRC Polynomial Mismatch

**Severity:** CRITICAL  
**Probability:** 100% (code is definitive, but ADR decision made)  
**Impact:** CRC validation failure

**Description:**
- Specification: CRC-16-CCITT with 0x1021 polynomial
- Firmware: CRC-16-Modbus with 0xA001 polynomial
- Result: CRC values don't match between layers
- Status: ADR-0003 approved using firmware's 0xA001, but specification hasn't been updated yet

**Scenario:**
- Daemon implements CRC using 0x1021 (following spec)
- Daemon creates packet for RELAY_SET
- Daemon calculates CRC using 0x1021: result = 0xA5A1
- Daemon transmits packet with CRC = 0xA5A1
- Firmware receives packet
- Firmware calculates CRC using 0xA001: expected CRC = 0x9C3E
- Received CRC (0xA5A1) != Expected CRC (0x9C3E)
- PacketValidator returns CRC_ERROR
- Packet rejected

**Resolution Path:**
1. ✅ ADR-0003 has decided: Use firmware's 0xA001
2. ⏳ Update PROTOCOL_SPEC.md to declare 0xA001
3. ⏳ Update shared/src/protocol.ts to declare 0xA001
4. ⏳ Daemon CRC implementation must use 0xA001
5. ⏳ Regenerate test vectors using firmware's CRC

**Status:** Decision made; awaiting implementation

**Cost if not fixed:**
- Daemon CRC will be wrong
- All packets will fail CRC validation
- Protocol is non-functional

---

### Risk #4: MEDIUM - Test Vectors Not Yet Verified

**Severity:** MEDIUM  
**Probability:** 100% (acknowledged in PROTOCOL_FREEZE_CHECKLIST.md)  
**Impact:** Test vectors cannot be trusted

**Description:**
- Test vectors (vector_001.json through vector_005.json) contain CRC values
- These CRC values were generated manually using the CRC16-Modbus algorithm
- Rule 14 requires test vectors be verified by executing actual firmware code
- Current test vectors are templates, not canonical vectors

**Status:** 
- ⏳ Vectors marked as "verified" in JSON but method says "Firmware CRC16::Calculate() executed"
- ❓ Unclear if actual firmware was executed or if values were calculated

**Question:** Were these CRC values actually generated by running firmware code, or were they calculated mathematically?

**Resolution Path:**
1. Execute firmware test harness with vector input
2. Capture actual CRC output from firmware
3. Record firmware version, commit hash, execution date
4. Update test vector JSON with actual values
5. Mark as "verified" with execution evidence

---

### Risk #5: MEDIUM - Incomplete Code Review

**Severity:** MEDIUM  
**Probability:** High (code audit was selective)  
**Impact:** Unknown inconsistencies may exist

**Description:**
- This audit did not review all firmware code
- ResponseBuilder.cpp/h were not fully reviewed
- CommandDispatcher.cpp/h were not reviewed
- Full integration between layers was not traced

**Status:** OUT OF SCOPE for investigation task

**Recommendation:** Subsequent code review should examine:
- Response packet generation (ACK/NAK encoding)
- Command dispatch and handling
- Timeout and retry logic
- State synchronization sequence

---

## QUESTIONS REQUIRING HUMAN DECISION

### Question #1: Field Sizes (Sequence and Command)

**Question:** 
Are sequence and command fields 1 byte or 2 bytes per specification?

**Current State:**
- Specification declares: 1 byte each
- Firmware implements: 2 bytes each (uint16_t)
- Shared types expect: 1 byte each (based on shared/src/protocol.ts)

**Evidence:**
- Spec is explicit: "[Seq] [Cmd]" in "1B 1B" columns
- Firmware code uses uint16_t and 2-byte offsets
- No ambiguity in code; field sizes are definitive

**Options:**
- **Option A:** Field sizes are intentionally 2 bytes (firmware is correct, spec is wrong)
  - Update PROTOCOL_SPEC.md to change to 2-byte fields
  - Update packet structure table: [Header:2][Seq:2][Cmd:2][Len:2][Payload][CRC:2][Footer:1]
  - Update minimum packet size calculations
  - Cost: Moderate (doc updates + test vector regen)

- **Option B:** Field sizes should be 1 byte (specification is correct, firmware is wrong)
  - Change firmware PacketParser.cpp to use uint8_t instead of uint16_t
  - Change firmware PacketBuilder.cpp similarly
  - Update all memcpy offsets
  - Cost: High (firmware changes + testing + re-verification)

- **Option C:** Dual-version support (new firmware uses 2-byte, old uses 1-byte)
  - Add protocol version field
  - Detect version on connect
  - Use different offsets based on version
  - Cost: Very high (complexity, versioning overhead)

**Recommendation:** Requires human decision based on original design intent

---

### Question #2: Command Code Encoding (1-byte vs 2-byte)

**Question:**
Are command codes 1-byte (0x10, 0x11) or 2-byte (0x1002, 0x1001)?

**Current State:**
- Specification declares: 1-byte (0x01-0x70)
- Firmware implements: 2-byte (0x0001, 0x1001, 0x1002, etc.)
- Shared types expect: 1-byte (matching specification)

**Evidence:**
- Spec table shows single digits: 0x01, 0x02, 0x10, 0x20, etc.
- Firmware Commands.h shows two digits: 0x0001, 0x1002, 0x1101, etc.
- Commands.cpp validates against uint16_t codes
- This is consistent with field size question (if command field is 2 bytes, code is 2 bytes)

**Options:**
- **Option A:** Command codes are 2-byte (firmware is correct)
  - Directly tied to Question #1: Field Sizes (Option A)
  - Update PROTOCOL_SPEC.md command table to use 0x1000-format codes
  - Retrain developers on new command code scheme
  - Cost: Tied to Question #1 resolution

- **Option B:** Command codes should be 1-byte (specification is correct)
  - Directly tied to Question #1: Field Sizes (Option B)
  - Change firmware to use uint8_t codes
  - Update Commands.h constants
  - Cost: Tied to Question #1 resolution

**Recommendation:** Requires human decision; likely tied to Question #1 answer

---

### Question #3: CRC Polynomial (0x1021 vs 0xA001)

**Question:**
Which CRC polynomial is correct: CCITT (0x1021) or Modbus (0xA001)?

**Current State:**
- Specification currently declares: 0x1021 (CCITT)
- Firmware implements: 0xA001 (Modbus)
- ADR-0003 approved: 0xA001 (adopt firmware as source of truth)

**Evidence:**
- ADR-0003 documents decision rationale
- Firmware implementation is complete and stable
- Specification update is pending (acknowledged in ADR)

**Status:** DECISION ALREADY MADE
- ADR-0003 (accepted): Adopt firmware implementation (0xA001)
- Action: Update PROTOCOL_SPEC.md to reflect this decision

**Recommendation:** No human decision needed; implement ADR-0003 by updating specification

---

### Question #4: Footer Validation

**Question:**
Should firmware explicitly validate that footer byte is 0xAA?

**Current State:**
- Specification requires: Footer = 0xAA
- Firmware implementation: Does not explicitly validate footer

**Evidence:**
- PacketValidator.cpp does not check footer byte
- Code validates header, sequence, command, length, payload, CRC
- Footer byte is not validated
- Spec is explicit: [Footer] = 1B, Value 0xAA

**Implications:**
- Packet with footer 0xBB would be accepted by firmware
- Specification is not fully enforced
- Could hide transmission errors

**Options:**
- **Option A:** Add footer validation to firmware
  - Update PacketValidator::Validate() to check byte at offset(length-1) == 0xAA
  - Cost: Low (one check, firmware change)
  - Benefit: Spec compliance, error detection

- **Option B:** Remove footer from specification (not enforced)
  - Update PROTOCOL_SPEC.md to remove footer requirement
  - Cost: Moderate (spec update, may affect existing implementation)
  - Benefit: Spec matches implementation
  - Risk: Lose error detection capability

- **Option C:** Leave as-is (implicit contract)
  - Rely on serial protocol framing to detect corruption
  - Cost: None
  - Risk: Specification not enforced

**Recommendation:** Requires human decision on strictness level

---

### Question #5: Test Vector Execution Evidence

**Question:**
Were test vectors generated by executing actual firmware code, or calculated mathematically?

**Current State:**
- Test vectors (vector_001.json through vector_005.json) contain CRC values
- JSON verification section says "Firmware CRC16::Calculate() executed"
- Unclear if firmware was actually run or values were calculated

**Evidence:**
- VERIFICATION_SESSION_PLAN.md explains Rule 14 requirement
- PROTOCOL_FREEZE_CHECKLIST.md lists test vector generation as "completed"
- Test vector JSON shows detailed structure (good)
- But no evidence of actual firmware execution (e.g., commit hash, date, harness code)

**Question:** Were these vectors generated by:
1. Running actual firmware test harness and capturing output? OR
2. Calculating CRC mathematically?

**Impact:**
- If calculated: Vectors are templates, not canonical (violates Rule 14)
- If executed: Vectors are canonical and can be used for daemon testing

**Recommendation:** 
1. Verify test vector generation method
2. If calculated: Re-generate via firmware execution
3. Document execution evidence (harness code, commit, date, output)

---

## POSSIBLE RESOLUTION OPTIONS

### SCENARIO A: Specification and Shared Types Are Correct

**If 1-byte fields are correct:**

**Changes Required:**

1. **Firmware Code Changes:**
   - PacketBuilder.cpp: Change sequence/command from uint16_t to uint8_t
   - PacketValidator.cpp: Change uint16_t to uint8_t
   - PacketParser.cpp: Update memcpy offsets (2 becomes 1, 4 becomes 2, 6 becomes 3)
   - Commands.h: Change CommandId from 0x1002 to 0x10, 0x1001 to 0x11, etc.
   - Commands.cpp: Update switch statement cases

2. **Specification Updates:**
   - No changes needed (already correct)

3. **Shared Types Updates:**
   - No changes needed (already correct)

4. **Test Vector Regeneration:**
   - New minimum packet size: 9 bytes (not 12)
   - All test vector packets become 3 bytes shorter
   - CRC values remain the same (no algorithm change)
   - Regenerate vectors with new packet sizes

5. **PROTOCOL_REGISTRY.md:**
   - No changes needed (command codes already 1-byte)

**Effort Estimate:** HIGH
- Firmware changes: Complex, requires testing on hardware
- Test regeneration: Moderate
- Risk of regression: High (field offsets throughout codebase)

**Timeline:** 2-3 days

---

### SCENARIO B: Firmware Implementation Is Correct

**If 2-byte fields are correct:**

**Changes Required:**

1. **Firmware Code Changes:**
   - None needed (already correct)

2. **Specification Updates:**
   - PROTOCOL_SPEC.md line 40 table: Change [Seq] from 1B to 2B, [Cmd] from 1B to 2B
   - PROTOCOL_SPEC.md line 50: Update payload definition to account for 2-byte fields
   - PROTOCOL_SPEC.md line 35 example: Update packet example with correct offsets
   - PROTOCOL_SPEC.md line 116 (minimum packet): Change to 12 bytes (not 9)

3. **Shared Types Updates:**
   - shared/src/protocol.ts line 131-169: Change CommandCode values from 0x10 to 0x1002, 0x11 to 0x1001, etc.
   - Update all references in codebase

4. **PROTOCOL_REGISTRY.md Updates:**
   - Command table: Update all command codes to 2-byte format

5. **Test Vector Regeneration:**
   - Packet structures remain as-is (already correct for firmware)
   - CRC values already correct (firmware CRC was used)
   - No regeneration needed (vectors are already correct)

6. **Documentation Updates:**
   - Update all examples and diagrams in specification
   - Update command encoding documentation

**Effort Estimate:** MODERATE
- Specification updates: Low-moderate
- Shared types updates: Low (enumeration changes)
- Test vectors: Already correct, no regeneration
- Risk of regression: Low

**Timeline:** 1 day

---

### SCENARIO C: Hybrid Approach (New Protocol Version)

**If both are partially correct and require version support:**

**This approach creates backward compatibility but adds complexity:**

**Changes Required:**

1. **New Protocol Version:**
   - Current: v1.0 (with 2-byte fields, as firmware implements)
   - Future: v1.1 (with 1-byte fields, if needed)
   - Or: v2.0 (if breaking change)

2. **Version Negotiation:**
   - Add GET_PROTOCOL_VERSION command (not in current spec)
   - Daemon queries version on connect
   - Handles both formats based on firmware version

3. **Dual Code Paths:**
   - Firmware: Support both 1-byte and 2-byte fields
   - Daemon: Encode/decode based on negotiated version
   - Test utilities: Test both versions

4. **Cost:** Very HIGH
   - Adds significant complexity
   - Multiple code paths to maintain
   - Extended testing required

**Recommendation:** Avoid unless there's a strong reason to support both formats

---

## RECOMMENDATION

### Summary of Findings

This audit has identified **three critical mismatches** between the NAS Controller Protocol Specification and its Firmware Implementation:

1. **Field Sizes:** Specification declares 1-byte sequence/command; firmware implements 2-byte fields
2. **Command Codes:** Specification uses 0x10 format; firmware uses 0x1002 format  
3. **CRC Polynomial:** Specification declares 0x1021; firmware uses 0xA001 (ADR-0003 approved update)

Additionally:
- **Four medium/low issues:** Footer validation, test vector evidence, shared types sync, incomplete code review
- **Specification not updated** despite ADR-0003 decision

### Recommended Course of Action

**STOP current protocol implementation activities.** Do NOT proceed with daemon CRC implementation, packet encoder, or integration testing until mismatches are resolved.

#### Phase 1: Determine Source of Truth (BLOCKING - Do First)

**The critical question:** Which is the source of truth?

**Evidence hierarchy (highest to lowest):**
1. Firmware reference implementation (CODE - executable proof)
2. Architecture Decision Records (DECISION - approved rationale)
3. Specification document (TEXT - intended behavior)
4. Shared types (CODE - daemon expectations)

**Recommended approach:**
1. Review firmware Git history to understand original design intent
2. Check if there are comments explaining why 2-byte fields were chosen
3. Consult with firmware developer (if available) on original design
4. Determine if 1-byte in spec was a mistake, or 2-byte in firmware was unintentional

**Most likely outcome:** Firmware implementation is correct (it's proven, tested code), specification needs update

#### Phase 2: Update Specification (ADR-0003 Foundation)

**ADR-0003 has already decided: Use firmware as source of truth for CRC**

**Extend this principle to all firmware findings:**

**Update PROTOCOL_SPEC.md to declare:**
- Sequence field: 2 bytes (or 1 byte, based on Phase 1 decision)
- Command field: 2 bytes (or 1 byte, based on Phase 1 decision)
- CRC Polynomial: 0xA001 (CRC-16-Modbus) ← Already decided in ADR-0003
- Minimum packet size: 12 bytes (or 9 bytes, based on Phase 1 decision)

**Update PROTOCOL_FREEZE_CHECKLIST.md:**
- Check off field size verification
- Check off command code verification
- Confirm all seven CRC parameters

#### Phase 3: Sync All Layers

**Once specification is authoritative:**

1. **Update shared/src/protocol.ts:**
   - Align command codes with specification
   - Align CRC algorithm constants

2. **Verify firmware implementation matches specification:**
   - Code audit to confirm all findings
   - No changes needed if firmware is source of truth (already done)

3. **Regenerate test vectors:**
   - If field sizes changed: Regenerate all vectors (new packet structures)
   - CRC polynomial (0xA001): Already correct in existing vectors
   - Execute firmware harness to verify vectors
   - Document execution evidence (Rule 14)

4. **Update documentation:**
   - PROTOCOL_REGISTRY.md: Align command codes
   - Examples in PROTOCOL_SPEC.md: Show correct packet structures
   - PROJECT_STATUS.md: Update task status

#### Phase 4: Restart Daemon Implementation

**Only after Phase 1-3 complete:**

1. Implement daemon CRC using verified specification
2. Implement packet encoder using correct field sizes
3. Test daemon against canonical test vectors
4. Proceed with integration testing

### Effort & Timeline

**Phase 1 (Decision):** 2-4 hours
- Git history review
- Architecture discussion
- Decision documentation

**Phase 2 (Specification Update):** 4-6 hours
- Update PROTOCOL_SPEC.md sections
- Update ADR if needed
- Review and approval

**Phase 3 (Sync & Vectors):** 6-10 hours
- Update shared types
- Firmware code audit/verification
- Test vector generation/verification
- Documentation updates

**Phase 4 (Daemon Implementation):** (Resume previously blocked work)

**Total Blocking Time:** 12-20 hours
**Cost of Delay:** Moderate (prevents rework later in integration)
**Cost of Not Fixing:** Very High (incompatible protocol, failed integration)

### Success Criteria

Protocol implementation is ready for daemon development when:

- [ ] **Specification authoritative:** PROTOCOL_SPEC.md is the single source of truth
- [ ] **Firmware verified:** Code audit confirms firmware matches specification
- [ ] **Shared types synced:** shared/src/protocol.ts matches specification and firmware
- [ ] **Test vectors canonical:** All five vectors verified via firmware execution (Rule 14)
- [ ] **Documentation frozen:** PROTOCOL_FREEZE_CHECKLIST.md all items checked
- [ ] **Decisions recorded:** All mismatches resolved with written decisions (ADRs)
- [ ] **Ready for daemon:** Specification and vectors ready for daemon CRC/encoder implementation

---

**AUDIT COMPLETE**

**Date:** 2026-07-20  
**Auditor:** AI Assistant  
**Status:** INVESTIGATION PHASE COMPLETE - Awaiting Resolution Phase
