# Documentation Index

**NAS Controller Firmware V3**

This document provides a comprehensive index of all project documentation, organized by purpose and audience.

---

## 🎯 Quick Navigation

### For New Contributors
- Start with: **README.md** → **PROJECT_OVERVIEW.md** → **docs/Architecture.md**
- Then: **docs/CodingStandard.md** and **PROJECT_RULES.md**

### For Protocol Developers
- Start with: **shared/docs/PROTOCOL_SPEC.md**
- Then: **shared/docs/PROTOCOL_REGISTRY.md** → **shared/docs/protocol/test_vectors/**
- Reference: **docs/USB-Host-Protocol.md**

### For Firmware Developers
- Start with: **docs/Architecture.md**
- Then: **docs/Hardware.md** → **docs/CommandSet.md**
- Reference: **docs/Decisions/** for architectural choices

### For Daemon/Integration Developers
- Start with: **shared/docs/PROTOCOL_SPEC.md**
- Then: **shared/docs/protocol/test_vectors/** (for validation)
- Reference: **docs/CommandSet.md**

---

## 📚 Core Documentation

### Root Level
| File | Purpose |
|------|---------|
| **README.md** | Project overview, technology stack, quick start |
| **DOCUMENTATION.md** | This file (documentation index) |
| **CHANGELOG.md** | Version history and notable changes |
| **LICENSE** | Project license (MIT) |

---

## 🏗️ Architecture & Design

### Main Architecture Documents
| Path | Purpose | Audience |
|------|---------|----------|
| **docs/Architecture.md** | Complete layered architecture design | All developers |
| **docs/Repository-Structure.md** | Directory organization and module layout | All developers |
| **PROJECT_OVERVIEW.md** | High-level system overview | Product/technical |
| **PROJECT_RULES.md** | Development constraints and rules | All developers |

### Architecture Decision Records (ADRs)
Located in `docs/Decisions/`

| File | Decision |
|------|----------|
| **0001-Firmware-Architecture.md** | Layered architecture pattern |
| **0002-Core-Layer.md** | Core layer design and utilities |
| **0003-Platform-HAL.md** | Hardware abstraction approach |
| **0004-Drivers-Layer.md** | Driver layer responsibilities |
| **0005-Objects-Layer.md** | Runtime state and models |
| **0006-Services-Layer.md** | Business logic orchestration |
| **0007-Protocol-Layer.md** | Binary protocol handling |
| **0008-System-Layer.md** | Lifecycle and initialization |

---

## 🔌 Hardware Documentation

| Path | Purpose |
|------|---------|
| **docs/Hardware.md** | ESP32 pinouts, electrical specs, component specifications |
| **docs/LED_Subsystem.md** | WS2812B LED control and animation system |

---

## 📡 Protocol & Communication

### Specification & Registry
| Path | Purpose |
|------|---------|
| **shared/docs/PROTOCOL_SPEC.md** | Complete protocol specification (versioned) |
| **shared/docs/PROTOCOL_REGISTRY.md** | Enumeration of all protocol elements (commands, codes, etc.) |
| **shared/docs/protocol/PROTOCOL_FREEZE_CHECKLIST.md** | Requirements for protocol version freeze |
| **docs/adr/0003-crc16-modbus.md** | CRC16-Modbus decision and verification |

### Test Vectors
Located in `shared/docs/protocol/test_vectors/`

| Type | Purpose |
|------|---------|
| **vector_*.json** | Canonical protocol test vectors (at least 5) |

### Implementation Documentation
| Path | Purpose |
|------|---------|
| **docs/Protocol.md** | Protocol format, packet structure, CRC validation |
| **docs/USB-Host-Protocol.md** | USB CDC communication specification |

---

## 🎮 Commands & API

| Path | Purpose |
|------|---------|
| **docs/CommandSet.md** | Complete command reference with payloads |

---

## 💻 Development

### Standards & Practices
| Path | Purpose |
|------|---------|
| **docs/CodingStandard.md** | Code conventions, style, naming rules |
| **PROJECT_RULES.md** | Development rules and governance |
| **IMPLEMENTATION_ORDER.md** | Task execution sequence and priorities |

### Audits & Verification
| Path | Purpose |
|------|---------|
| **docs/CodeVerificationReport.md** | Code review and verification results |
| **docs/DocumentationVerification.md** | Documentation completeness audit |
| **docs/ComplianceAudit.md** | Compliance and standards adherence |
| **VERIFICATION_SESSION_PLAN.md** | Protocol verification execution plan |

---

## 📋 Planning & Roadmap

| Path | Purpose |
|------|---------|
| **docs/Roadmap.md** | Development phases and timeline |
| **IMPLEMENTATION_ORDER.md** | Sequence of tasks for upcoming phases |

---

## 🔍 Verification & Quality

### Verification Assets
- **Firmware:** Source files in `src/` with inline documentation
- **Protocol Vectors:** JSON test fixtures in `shared/docs/protocol/test_vectors/`
- **Compliance:** Tracked in `docs/ComplianceAudit.md`

### Audit Trail
- All architectural decisions documented in ADRs
- All protocol decisions documented in PROTOCOL_SPEC.md
- All verification results in appropriate audit documents

---

## 📌 Document Ownership

### By Phase
| Phase | Responsible Documents |
|-------|----------------------|
| **Architecture & Design** | ADRs, Architecture.md, PROJECT_OVERVIEW.md |
| **Protocol Definition** | PROTOCOL_SPEC.md, PROTOCOL_REGISTRY.md, USB-Host-Protocol.md |
| **Firmware Implementation** | Hardware.md, CommandSet.md, source code docs |
| **Verification & Governance** | VERIFICATION_SESSION_PLAN.md, Protocol test vectors, checklist |
| **Future Phases** | IMPLEMENTATION_ORDER.md guides next work |

---

## 🔄 Document Relationships

```
Project Status & Planning
├── PROJECT_STATUS.md (current state)
├── PROJECT_OVERVIEW.md (system design)
├── IMPLEMENTATION_ORDER.md (phase sequence)
└── VERIFICATION_SESSION_PLAN.md (current work)

Architecture
├── docs/Architecture.md (complete design)
├── docs/Decisions/ (decision records)
└── docs/Repository-Structure.md (file organization)

Protocol (Shared)
├── shared/docs/PROTOCOL_SPEC.md (specification)
├── shared/docs/PROTOCOL_REGISTRY.md (elements)
├── shared/docs/protocol/test_vectors/ (fixtures)
└── docs/adr/0003-crc16-modbus.md (CRC decision)

Development
├── docs/CodingStandard.md (conventions)
├── PROJECT_RULES.md (constraints)
└── docs/ComplianceAudit.md (verification)

Hardware
├── docs/Hardware.md (specifications)
└── docs/LED_Subsystem.md (detailed subsystem)

Implementation
├── docs/CommandSet.md (API reference)
├── docs/USB-Host-Protocol.md (communication)
└── src/ (firmware source)
```

---

## 📖 Reading Recommendations

### Before Writing Any Code
1. README.md
2. PROJECT_OVERVIEW.md
3. PROJECT_RULES.md
4. docs/CodingStandard.md

### Before Working on Protocol
1. shared/docs/PROTOCOL_SPEC.md
2. shared/docs/PROTOCOL_REGISTRY.md
3. shared/docs/protocol/test_vectors/
4. docs/USB-Host-Protocol.md

### Before Working on Firmware
1. docs/Architecture.md
2. docs/Hardware.md
3. docs/Decisions/ (relevant ADRs)
4. docs/CommandSet.md

### Before Working on Daemon/Integration
1. shared/docs/PROTOCOL_SPEC.md
2. shared/docs/protocol/test_vectors/
3. docs/USB-Host-Protocol.md
4. docs/CommandSet.md

---

## 🔐 Frozen Documents

The following documents are **locked for the 1.x release cycle** and can only be modified through formal ADR process:

- docs/Architecture.md (layered structure)
- docs/Hardware.md (pinout assignments)
- docs/CodingStandard.md (naming conventions)
- shared/docs/PROTOCOL_SPEC.md (protocol version 1.0.0)
- docs/Decisions/ (all ADRs)

Any changes to these documents require:
1. ADR approval
2. Technical lead review
3. Documentation of rationale
4. Version increment where applicable

---

## ✅ Last Updated

- **Date:** July 2026
- **Phase:** Protocol Verification & Governance
- **Reviewer:** Technical Team
- **Status:** In Progress (Protocol Verification 90% complete)
