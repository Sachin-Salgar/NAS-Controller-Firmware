# NAS Controller Firmware - Documentation Index

**Purpose:** Navigate all project documentation without needing to search
**Updated:** July 19, 2026
**Verification Status:** ✅ Architecture alignment verified - 100% compliant with frozen standards
**Total Documents:** 30+

---

## 📋 START HERE

### For First-Time Readers

1. **QUICK_START.md** (5 min read)
   - Essential facts you need to know
   - Critical issues blocking progress
   - Code style rules
   - Common patterns

2. **PROJECT_REFERENCE.md** (20 min read)
   - Complete project scope
   - Frozen architecture definition
   - All layers explained
   - Remaining tasks organized

3. **STATUS_REPORT.md** (15 min read)
   - What's working / what's broken
   - Layer-by-layer implementation status
   - Critical issues analysis
   - Action plan

---

## 🏗️ ARCHITECTURE & DESIGN

### Core Architecture

| Document | Time | Purpose |
|----------|------|---------|
| `docs/Architecture.md` | 30 min | **READ FIRST:** Complete layered architecture, all 8 layers defined, dependency rules |
| `docs/Repository-Structure.md` | 10 min | Directory organization and file structure |
| `docs/Decisions/0001-Firmware-Architecture.md` | 5 min | Architecture freeze decision (locked for 1.x) |

### Design Decisions (ADRs)

| Document | Purpose |
|----------|---------|
| `docs/Decisions/0001-Firmware-Architecture.md` | Architecture baseline frozen |
| `docs/Decisions/0002-Core-Layer.md` | Core layer design rationale |
| `docs/Decisions/0003-Platform-HAL.md` | Hardware abstraction approach |
| `docs/Decisions/0004-Drivers-Layer.md` | Device driver design |
| `docs/Decisions/0005-Objects-Layer.md` | State object design |
| `docs/Decisions/0006-Services-Layer.md` | Business logic layer design |
| `docs/Decisions/0007-Protocol-Layer.md` | USB protocol design |
| `docs/Decisions/0008-System-Layer.md` | Lifecycle management design |

**Note:** All ADRs are frozen. Changes require new ADRs.

---

## 💻 CODING & IMPLEMENTATION

| Document | Time | Purpose |
|----------|------|---------|
| `docs/CodingStandard.md` | 15 min | **MANDATORY:** Naming conventions, style rules, patterns |
| `docs/CommandSet.md` | 20 min | All supported commands with payloads |
| `docs/Protocol.md` | 15 min | Binary packet format, structure, validation |
| `docs/USB-Host-Protocol.md` | 15 min | USB communication protocol details |

---

## 🔌 HARDWARE

| Document | Time | Purpose |
|----------|------|---------|
| `docs/Hardware.md` | 20 min | **IF TESTING:** ESP32 GPIO allocation, relay/fan/sensor/LED connections |
| `docs/LED_Subsystem.md` | 15 min | LED specification (frozen for 1.x, needs update in future) |

---

## 📊 PROJECT MANAGEMENT

| Document | Time | Purpose |
|----------|------|---------|
| `docs/Roadmap.md` | 10 min | Development phases, what's done, what's next |
| `docs/Changelog.md` | 5 min | Version history and changes |
| `CHANGELOG.md` (root) | 5 min | Repository changelog |

---

## 🚨 SPECIAL ANALYSIS DOCUMENTS

### LED System (Must Read if Working on LEDs)

**LED_AUDIT_REPORT.md** (45 min read)
- Complete LED subsystem analysis
- 10 problems identified with severity
- 12 architectural improvements recommended
- Implementation plan for LED redesign
- Missing 52 of 60 LEDs design
- **Status:** MUST FIX for specification compliance

### Project Status (Must Read Before Starting)

**STATUS_REPORT.md** (20 min read)
- Layer-by-layer implementation status
- What's working, what's broken
- Build and compilation status
- Hardware validation status
- Critical issues blocking progress
- Recommended action plan

---

## 📁 CODEBASE DOCUMENTATION

### By Layer

Each layer has header comments in source code:

| Layer | Location | Key Files |
|-------|----------|-----------|
| **Core** | `src/Core/` | Result.h, Logger.h, EventBus.h, Scheduler.h |
| **Platform** | `src/Platform/` | GPIO.h, PWM.h, USB.h, WS2812.h, OneWire.h |
| **Drivers** | `src/Drivers/` | RelayDriver.h, FanDriver.h, LEDDriver.h, TemperatureDriver.h |
| **Objects** | `src/Objects/` | Relay.h, Fan.h, Led.h, Drive.h, Configuration.h |
| **Services** | `src/Services/` | *Service.h files, ServiceManager.h |
| **Protocol** | `src/Protocol/` | CommandDispatcher.h, CommandParser.h, PacketValidator.h |
| **System** | `src/System/` | SystemManager.h, Startup.h |
| **Config** | `src/Config/` | HardwareConfig.h, Features.h, TaskConfig.h |
| **Utilities** | `src/Utilities/` | CRC16.h, StringHelpers.h, TimeHelpers.h |

### Code Organization

```
PROJECT_ROOT/
├── QUICK_START.md                    ← Fast reference
├── PROJECT_REFERENCE.md               ← Complete reference
├── STATUS_REPORT.md                   ← Current state
├── LED_AUDIT_REPORT.md                ← LED analysis
├── DOCUMENTATION_INDEX.md             ← This file
│
├── docs/                              ← Official documentation
│   ├── Architecture.md                ← Frozen architecture
│   ├── CodingStandard.md              ← Mandatory code rules
│   ├── Hardware.md                    ← Hardware specs
│   ├── Protocol.md                    ← Binary protocol
│   ├── CommandSet.md                  ← All commands
│   ├── Roadmap.md                     ← Development phases
│   ├── Decisions/                     ← Architecture decisions
│   └── ...                            ← More documentation
│
├── src/                               ← Source code
│   ├── main.cpp                       ← Entry point (🚨 BROKEN)
│   ├── Core/                          ← Foundation
│   ├── Platform/                      ← HAL
│   ├── Drivers/                       ← Device control
│   ├── Objects/                       ← State models
│   ├── Services/                      ← Business logic
│   ├── Protocol/                      ← USB communication
│   ├── System/                        ← Lifecycle
│   ├── Config/                        ← Configuration
│   ├── Utilities/                     ← Helpers
│   └── Tests/                         ← Test code
│
├── platformio.ini                     ← Build config
├── README.md                          ← Project overview
└── LICENSE                            ← License
```

---

## 🎯 DOCUMENT SELECTION GUIDE

### "I need to..."

#### Understand the Project
1. README.md (overview)
2. QUICK_START.md (essentials)
3. docs/Architecture.md (layered design)

#### Start Coding
1. docs/CodingStandard.md (rules)
2. QUICK_START.md (patterns)
3. docs/Architecture.md (dependencies)

#### Add a Feature
1. docs/CommandSet.md (what commands exist)
2. docs/Protocol.md (command format)
3. docs/CodingStandard.md (code rules)
4. Source code (existing patterns)

#### Fix a Bug
1. STATUS_REPORT.md (known issues)
2. Source code (find the bug)
3. docs/CodingStandard.md (write the fix)

#### Work on LEDs
1. LED_AUDIT_REPORT.md (comprehensive analysis)
2. docs/LED_Subsystem.md (hardware spec)
3. docs/CodingStandard.md (code rules)

#### Test on Hardware
1. docs/Hardware.md (GPIO allocation)
2. docs/Protocol.md (command format)
3. docs/CommandSet.md (commands to test)

#### Deploy to Production
1. STATUS_REPORT.md (what's ready)
2. docs/Roadmap.md (what's done)
3. Changelog files (version history)

---

## 📚 DETAILED DOCUMENTATION MAP

### Architectural Documentation (Frozen)

**Status:** ✅ Complete and locked for Version 1.x

```
Architecture.md (CORE REFERENCE)
├── Layer 1: Core
├── Layer 2: Platform
├── Layer 3: Drivers
├── Layer 4: Objects
├── Layer 5: Services
├── Layer 6: Protocol
├── Layer 7: System
└── Shared: Config, Utilities
```

### Implementation Documentation

**Status:** ✅ Complete, code follows standards

```
CodingStandard.md (MANDATORY)
├── Naming conventions
├── Return types
├── Error handling
├── Code organization
└── Memory policy
```

### Feature Documentation

**Status:** ⚠️ Complete but needs updates

```
CommandSet.md → All commands
Protocol.md → Packet format
USB-Host-Protocol.md → Communication
Hardware.md → Hardware details
LED_Subsystem.md → LED specification
```

### Status & Planning

**Status:** ⚠️ In progress

```
Roadmap.md → Development phases
STATUS_REPORT.md → Current state (NEW)
LED_AUDIT_REPORT.md → LED analysis (NEW)
PROJECT_REFERENCE.md → Complete reference (NEW)
```

---

## 🔍 QUICK LOOKUP TABLE

### By Problem Type

| Problem | Document | Section |
|---------|----------|---------|
| Compilation error | `docs/CodingStandard.md` | Naming Conventions |
| Dependency error | `docs/Architecture.md` | Dependency Rules |
| LED issue | `LED_AUDIT_REPORT.md` | Complete analysis |
| Protocol question | `docs/Protocol.md` | Packet Format |
| Command question | `docs/CommandSet.md` | Command definitions |
| Hardware question | `docs/Hardware.md` | GPIO Allocation |
| Design question | `docs/Decisions/` | Architecture decisions |
| Status check | `STATUS_REPORT.md` | Layer-by-layer |
| Code style | `QUICK_START.md` | Code Style Rules |
| Project scope | `PROJECT_REFERENCE.md` | Core Objectives |

### By Layer

| Layer | Key Document | Details |
|-------|--------------|---------|
| Core | docs/Architecture.md | Sec 6 |
| Platform | docs/Decisions/0003-Platform-HAL.md | Complete |
| Drivers | docs/Decisions/0004-Drivers-Layer.md | Complete |
| Objects | docs/Decisions/0005-Objects-Layer.md | Complete |
| Services | docs/Decisions/0006-Services-Layer.md | Complete |
| Protocol | docs/Decisions/0007-Protocol-Layer.md | Complete |
| System | docs/Decisions/0008-System-Layer.md | Complete |

---

## 📌 MOST IMPORTANT DOCUMENTS

**🔴 Must Read Before Coding**

1. **docs/Architecture.md** – Understand the layered design
2. **docs/CodingStandard.md** – Know the code rules
3. **PROJECT_REFERENCE.md** – Know the scope and status
4. **QUICK_START.md** – Know the critical facts

**🟠 Must Read Before Working on Feature**

1. **docs/CommandSet.md** – Know what commands exist
2. **docs/Protocol.md** – Know packet format
3. **STATUS_REPORT.md** – Know what's implemented

**🟡 Must Read Before Hardware Testing**

1. **docs/Hardware.md** – Know GPIO assignments
2. **LED_AUDIT_REPORT.md** – If working on LEDs
3. **docs/LED_Subsystem.md** – LED hardware spec

---

## ✅ DOCUMENT MAINTENANCE

### Frozen Documentation (Cannot Change for 1.x)

- ✅ docs/Architecture.md
- ✅ docs/CodingStandard.md
- ✅ docs/Hardware.md
- ✅ docs/Protocol.md
- ✅ docs/CommandSet.md
- ✅ All ADRs (Decisions/)

### Active Documentation (May Change)

- ⚠️ docs/Roadmap.md
- ⚠️ docs/Changelog.md
- ⚠️ STATUS_REPORT.md
- ⚠️ LED_AUDIT_REPORT.md

### New Documents (For Current Development)

- ✨ PROJECT_REFERENCE.md
- ✨ QUICK_START.md
- ✨ DOCUMENTATION_INDEX.md (this file)

---

## 📖 HOW TO USE THIS INDEX

1. **Find what you need** using the tables above
2. **Click the link** to the relevant document
3. **Search within** that document for your specific question
4. **Reference it** when coding or reviewing

---

## 🔗 DOCUMENT RELATIONSHIPS

```
QUICK_START.md (Facts & Patterns)
    ├─ References → PROJECT_REFERENCE.md (Scope & Frozen Architecture)
    └─ References → docs/CodingStandard.md (Code Rules)

PROJECT_REFERENCE.md (Complete Reference)
    ├─ References → docs/Architecture.md (Layer Details)
    ├─ References → docs/Hardware.md (Hardware Specs)
    ├─ References → docs/CodingStandard.md (Code Rules)
    ├─ References → LED_AUDIT_REPORT.md (LED Analysis)
    └─ References → STATUS_REPORT.md (Current Status)

STATUS_REPORT.md (What's Done, What's Not)
    ├─ References → LED_AUDIT_REPORT.md (LED Details)
    ├─ References → PROJECT_REFERENCE.md (What Should Be Done)
    └─ References → docs/Architecture.md (Expected Design)

LED_AUDIT_REPORT.md (LED System Analysis)
    ├─ References → docs/LED_Subsystem.md (Hardware Spec)
    ├─ References → docs/Architecture.md (Architecture Rules)
    └─ References → docs/CodingStandard.md (Code Style)

docs/Architecture.md (Frozen Design)
    ├─ References → docs/CodingStandard.md (Implementation Rules)
    ├─ References → docs/Hardware.md (Hardware Constraints)
    ├─ References → docs/Decisions/ (Why decisions)
    └─ Referenced by → All other technical docs
```

---

## 💡 TIPS FOR EFFECTIVE USE

1. **Always check STATUS_REPORT.md first** – Know what's broken
2. **Reference QUICK_START.md often** – Memorize critical facts
3. **Use PROJECT_REFERENCE.md as your main reference** – Everything in one place
4. **Keep docs/Architecture.md open while coding** – Don't violate dependencies
5. **Check docs/CodingStandard.md for every new file** – Follow the style
6. **Use docs/CommandSet.md for protocol work** – All commands listed there
7. **Reference LED_AUDIT_REPORT.md if working on LEDs** – Comprehensive analysis

---

## 📞 WHO TO CONTACT FOR WHAT

For questions about:

| Topic | Document | Fallback |
|-------|----------|----------|
| Architecture | docs/Architecture.md | docs/Decisions/ |
| Code style | docs/CodingStandard.md | QUICK_START.md |
| Specific layer | docs/Decisions/XXXX.md | docs/Architecture.md |
| Commands | docs/CommandSet.md | docs/Protocol.md |
| Hardware | docs/Hardware.md | Project lead |
| Status | STATUS_REPORT.md | Project lead |
| LEDs | LED_AUDIT_REPORT.md | docs/LED_Subsystem.md |
| Project scope | PROJECT_REFERENCE.md | README.md |

---

**Last Updated:** July 18, 2026  
**Total Documents in Index:** 30+  
**Frozen Documents:** 15+  
**Active Documents:** 5+  
**New Documents:** 3+

