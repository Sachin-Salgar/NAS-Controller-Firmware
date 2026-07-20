# Quick Reference Guide

**ESP32 NAS Controller Firmware V3 — Protocol v1.0.0**

---

## 📍 Where to Start

### New to the Project?
Start here in order:
1. **README.md** — Project overview
2. **PROJECT_OVERVIEW.md** — System architecture
3. **PROJECT_RULES.md** — Development rules
4. **DOCUMENTATION.md** — Full documentation index

### Working on Protocol?
Start here in order:
1. **shared/docs/PROTOCOL_SPEC.md** — Complete specification
2. **shared/docs/PROTOCOL_REGISTRY.md** — All commands and codes
3. **shared/docs/protocol/test_vectors/** — Test your implementation
4. **docs/adr/0003-crc16-modbus.md** — CRC decision details

### Working on Daemon (Phase 4)?
Start here in order:
1. **IMPLEMENTATION_ORDER.md** — Phase 4 task breakdown
2. **shared/docs/PROTOCOL_SPEC.md** — Specification
3. **shared/docs/protocol/test_vectors/** — Validation fixtures
4. **PROJECT_RULES.md** (Rule 14) — Verification chain

### Working on Frontend (Phase 5)?
Start here:
1. **IMPLEMENTATION_ORDER.md** — Phase 5 section
2. **shared/docs/PROTOCOL_REGISTRY.md** — Available commands
3. **docs/CommandSet.md** — Command details
4. **DOCUMENTATION.md** — All reference materials

---

## 🔑 Key Documents by Purpose

### Understanding the System
| Document | Purpose |
|----------|---------|
| PROJECT_OVERVIEW.md | System architecture and subsystems |
| docs/Architecture.md | Detailed layered architecture |
| README.md | Quick project summary |

### Protocol Work
| Document | Purpose |
|----------|---------|
| PROTOCOL_SPEC.md | Complete packet format and rules |
| PROTOCOL_REGISTRY.md | All commands, codes, constants |
| test_vectors/ | Canonical test fixtures |
| adr/0003-crc16-modbus.md | CRC algorithm decision |

### Development Guidelines
| Document | Purpose |
|----------|---------|
| PROJECT_RULES.md | 20 development rules |
| docs/CodingStandard.md | Code conventions |
| IMPLEMENTATION_ORDER.md | Task sequence |

### Governance & Status
| Document | Purpose |
|----------|---------|
| PROJECT_STATUS.md | Current phase and progress |
| PROTOCOL_FREEZE_CHECKLIST.md | Freeze requirements |
| VERIFICATION_SESSION_PLAN.md | This session details |
| SESSION_SUMMARY.md | Session completion summary |

---

## 📋 Protocol Quick Facts

### Packet Structure
```
Header       0x55AA      2 bytes
Sequence     variable    2 bytes
Command      enumerated  2 bytes
Length       0-500       2 bytes
Payload      variable    N bytes
CRC16        Modbus      2 bytes
─────────────────────────────────
Total Size:  12 min, 512 max
```

### CRC16-Modbus Parameters
```
Polynomial:  0xA001
Initial:     0xFFFF
Reflection:  Yes
Final XOR:   No
Byte Order:  Little-endian
```

### Key Constants
```
Header:              0x55AA
Min Packet Size:     12 bytes
Max Packet Size:     512 bytes
Max Payload:         500 bytes
USB Baud Rate:       115200
```

### Result Codes
```
0x0000 — Ok
0x0001 — InvalidHeader
0x0002 — InvalidLength
0x0003 — CrcError
0x0004 — UnknownCommand
0x0005 — InvalidPayload
0x0006 — Busy
0x0007 — InternalError
```

---

## 🧪 Test Vectors

All vectors in: `shared/docs/protocol/test_vectors/`

| Vector | Purpose | Packet Size |
|--------|---------|------------|
| vector_001 | Minimum packet (Ping) | 10 bytes |
| vector_002 | Relay command | 12 bytes |
| vector_003 | Maximum payload | 512 bytes |
| vector_004 | Random data | 60 bytes |
| vector_005 | Error detection | 10 bytes |

**Requirement:** All implementations must pass all vectors.

---

## 📁 Directory Structure

```
Root
├── README.md                          ← Start here
├── DOCUMENTATION.md                   ← Doc index
├── PROJECT_STATUS.md                  ← Current phase
├── PROJECT_OVERVIEW.md                ← Architecture
├── PROJECT_RULES.md                   ← Development rules
├── IMPLEMENTATION_ORDER.md            ← Task sequence
├── VERIFICATION_SESSION_PLAN.md       ← This session
├── SESSION_SUMMARY.md                 ← Session summary
├── QUICK_REFERENCE.md                 ← This file
│
├── docs/
│   ├── PROJECT_STATUS.md              ← Status details
│   ├── Protocol.md                    ← Protocol overview
│   ├── USB-Host-Protocol.md           ← USB spec
│   ├── CommandSet.md                  ← Commands
│   └── adr/
│       └── 0003-crc16-modbus.md       ← CRC decision
│
├── shared/
│   └── docs/
│       ├── PROTOCOL_SPEC.md           ← Specification
│       ├── PROTOCOL_REGISTRY.md       ← Enumeration
│       └── protocol/
│           ├── PROTOCOL_FREEZE_CHECKLIST.md
│           └── test_vectors/
│               ├── vector_001.json
│               ├── vector_002.json
│               ├── vector_003.json
│               ├── vector_004.json
│               └── vector_005.json
│
├── src/
│   ├── Utilities/
│   │   ├── CRC16.h
│   │   └── CRC16.cpp                  ← Firmware CRC
│   └── Protocol/
│       ├── PacketBuilder.cpp
│       └── PacketValidator.cpp
```

---

## ⚡ Quick Answers

### Q: How do I implement the protocol?
**A:** Read PROTOCOL_SPEC.md, implement CRC16-Modbus, validate against test_vectors/. Must pass all 5 vectors.

### Q: What's the CRC algorithm?
**A:** CRC16-Modbus (0xA001 polynomial, 0xFFFF initial, reflected, little-endian transmission).

### Q: What test vectors do I need?
**A:** All 5 in shared/docs/protocol/test_vectors/. Failure on any = non-compliant.

### Q: Can I change the protocol?
**A:** Not for v1.x. New changes require ADR, version increment to 1.1.0 or 2.0.0, new test vectors, and Phase 4+ work.

### Q: What's Rule 14?
**A:** The 7-stage verification chain. All stages must complete before protocol freeze. See PROJECT_RULES.md.

### Q: When is daemon work (Phase 4)?
**A:** Now. All specifications are locked. Begin implementation and validate against test vectors.

### Q: How do I verify my implementation?
**A:** Test against all 5 canonical test vectors in shared/docs/protocol/test_vectors/. Entire packet bytes must match.

### Q: What if I find a bug in the spec?
**A:** Document it, create an ADR, update PROTOCOL_SPEC.md, create new test vectors, increment protocol version.

---

## 🚀 Phase Timeline

| Phase | Status | Duration | Next |
|-------|--------|----------|------|
| 1 | ✅ Complete | Weeks 1-2 | Frozen |
| 2 | ✅ Complete | Weeks 3-4 | Frozen |
| 3 | ✅ Complete | This week | → Phase 4 |
| 4 | ⬜ Ready | 2-3 weeks | Daemon |
| 5 | ⬜ Planned | 3-4 weeks | Frontend |
| 6+ | ⬜ Planned | TBD | Testing |

---

## 📞 Key Contact Points

### Technical Lead
- **Name:** Sachin Salgar
- **Responsibility:** Architecture, governance, freeze authority
- **For:** Design decisions, rule changes, protocol freeze

### Phase 4 (Daemon Team)
- **Responsibility:** CRC implementation, packet handling, integration tests
- **Start Date:** Immediately (all specifications locked)
- **Success Criteria:** Pass all 5 test vectors

### Phase 5 (Frontend Team)
- **Responsibility:** User interface, command execution, monitoring
- **Start Date:** After Phase 4 completion
- **Dependency:** Daemon API stability

---

## ✅ Verification Checklist

Before shipping anything:

- [ ] Read PROTOCOL_SPEC.md completely
- [ ] Understand CRC16-Modbus algorithm
- [ ] Implement packet builder
- [ ] Implement packet validator
- [ ] Implement CRC16-Modbus
- [ ] Test against vector_001 (minimum packet)
- [ ] Test against vector_002 (typical command)
- [ ] Test against vector_003 (maximum payload)
- [ ] Test against vector_004 (random data)
- [ ] Test against vector_005 (error detection)
- [ ] All packets match exactly (bytes, not just CRC)
- [ ] Ready for Phase 4+ work

---

## 📚 Document Versions

| Document | Version | Status |
|----------|---------|--------|
| PROTOCOL_SPEC.md | 1.0.0 | Locked (Specification) |
| PROTOCOL_REGISTRY.md | 1.0.0 | Locked |
| Test Vectors | 1.0.0 | Canonical (Immutable for v1.x) |
| PROJECT_RULES.md | 1.0 | Active (Rule 14 enhanced) |

---

## 🔗 Important Links

**Within This Repository:**
- Source: `src/` directory
- Firmware docs: `docs/` directory
- Protocol spec: `shared/docs/PROTOCOL_SPEC.md`
- Test vectors: `shared/docs/protocol/test_vectors/`
- ADRs: `docs/adr/` directory

**External References:**
- CRC16 algorithm: https://en.wikipedia.org/wiki/Cyclic_redundancy_check
- Modbus spec: https://en.wikipedia.org/wiki/Modbus
- ESP32 docs: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/

---

## 💡 Pro Tips

1. **Always reference firmware implementation first** — `src/Utilities/CRC16.cpp` is the authoritative source
2. **Test vectors are immutable** — Don't change them, change your implementation
3. **Byte order matters** — Little-endian throughout, no exceptions
4. **Packet bytes must match exactly** — Not just CRC, entire packet
5. **Error path is critical** — Vector 005 tests this, don't skip it
6. **Document all decisions** — Use ADR format for changes
7. **Rule 14 is non-negotiable** — All 7 stages must pass before freeze

---

**Last Updated:** July 2026  
**Phase:** 3 (Protocol Verification — Complete)  
**Status:** Ready for Phase 4
