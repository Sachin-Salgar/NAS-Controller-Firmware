# Implementation Order

**NAS Controller Firmware V3**

This document specifies the execution order for all future development tasks. This ordering is required to maintain system integrity and prevent rework.

---

## Current Status

**Phase 3:** Protocol Verification & Governance (In Progress)  
**Next Phase:** 4 (Daemon Implementation)

---

## Phase 4 — Daemon Implementation (Next Session)

**Duration:** 2-3 weeks  
**Dependencies:** Protocol freeze (Rule 14 completion)  
**Deliverable:** Functional daemon passing all canonical test vectors

### 4.1 Daemon Infrastructure
- [ ] Create daemon project structure
- [ ] Set up build system (language TBD: Python, Rust, C++, etc.)
- [ ] Create USB/serial transport abstraction
- [ ] Document daemon architecture

### 4.2 Protocol Implementation
- [ ] Implement packet builder (mirror firmware)
- [ ] Implement packet validator (mirror firmware)
- [ ] Implement CRC16-Modbus identical to firmware
- [ ] Create packet parser for responses

### 4.3 Protocol Verification
- [ ] Load all canonical test vectors
- [ ] Implement vector validation harness
- [ ] Verify against 5 minimum vectors
- [ ] Report results: PASS/FAIL per vector
- [ ] Document any discrepancies

### 4.4 Command API
- [ ] Implement command builder interface
- [ ] Implement response parser
- [ ] Map response codes to exceptions/errors
- [ ] Create examples for each command type

### 4.5 Integration Testing
- [ ] Test with actual firmware over USB
- [ ] Send each command type
- [ ] Verify response structure
- [ ] Test error scenarios
- [ ] Document integration test results

---

## Phase 5 — Frontend Development (Subsequent Session)

**Duration:** 3-4 weeks  
**Dependencies:** Daemon completion and API stability  
**Deliverable:** Functional GUI connecting to daemon

### 5.1 Frontend Architecture
- [ ] Select frontend framework
- [ ] Design application structure
- [ ] Create component hierarchy
- [ ] Document UI patterns

### 5.2 Daemon Integration
- [ ] Create daemon client library
- [ ] Implement event subscription model
- [ ] Handle connection lifecycle
- [ ] Implement reconnection logic

### 5.3 Dashboard Implementation
- [ ] Relay status display
- [ ] Fan speed display
- [ ] Temperature monitoring
- [ ] LED status and control
- [ ] Drive health monitoring

### 5.4 Command Execution
- [ ] Relay on/off buttons
- [ ] Fan speed slider
- [ ] LED color picker
- [ ] Configuration UI
- [ ] Command response display

### 5.5 Monitoring & Logging
- [ ] Event history display
- [ ] Statistics viewer
- [ ] System health indicators
- [ ] Debug log viewer
- [ ] Export data capability

---

## Phase 6 — Testing & Validation (Future)

**Duration:** 1-2 weeks  
**Dependencies:** Phases 4-5 completion  
**Deliverable:** Comprehensive test suite and validation report

### 6.1 Unit Testing
- [ ] Protocol layer unit tests
- [ ] Daemon API unit tests
- [ ] Frontend component tests

### 6.2 Integration Testing
- [ ] End-to-end command flow tests
- [ ] All command types tested
- [ ] Error scenario testing
- [ ] Recovery scenario testing

### 6.3 System Testing
- [ ] Multi-client scenarios
- [ ] Long-running stability tests
- [ ] Performance benchmarking
- [ ] Hardware compatibility validation

### 6.4 Compliance Verification
- [ ] Protocol compliance re-verification
- [ ] Architecture compliance audit
- [ ] Documentation completeness review
- [ ] Code quality assessment

---

## Phase 7 — Stability & Optimization (Future)

**Duration:** 1-2 weeks  
**Dependencies:** Phase 6 completion  
**Deliverable:** Production-ready firmware

### 7.1 Performance Optimization
- [ ] Identify bottlenecks
- [ ] Optimize CRC calculation (if applicable)
- [ ] Optimize USB communication
- [ ] Optimize daemon packet handling

### 7.2 Stability Hardening
- [ ] Crash recovery testing
- [ ] Edge case handling
- [ ] Memory leak analysis
- [ ] Resource exhaustion testing

### 7.3 Documentation Completion
- [ ] Complete API documentation
- [ ] Write deployment guide
- [ ] Write troubleshooting guide
- [ ] Create quick-start guide

### 7.4 Release Preparation
- [ ] Version numbering finalization
- [ ] Release notes preparation
- [ ] License verification
- [ ] Archive and backup creation

---

## Task Sequencing Rules

### Rule 1: No Parallelization Within Protocol Layer
**All protocol work must be sequential.**

Order:
1. Specification (done in Phase 3)
2. Firmware implementation (done in Phase 2)
3. Test vectors (to be done in Phase 3)
4. Daemon implementation (Phase 4)
5. Integration testing (Phase 4)
6. Frontend development (Phase 5)

Cannot skip to daemon while test vectors incomplete. No exceptions.

### Rule 2: Specification Before Implementation
**Every feature must be specified in PROTOCOL_SPEC.md before any code is written.**

### Rule 3: Test Vectors Precede Independent Implementation
**Daemon and frontend must reference test vectors during development.**

### Rule 4: Integration Before Optimization
**Don't optimize until everything works end-to-end.**

### Rule 5: Verification Before Release
**Protocol compliance must be demonstrated before marking phases complete.**

---

## Phase 4 Task Dependencies

```
Daemon Infrastructure
        ↓
Protocol Implementation
        ↓
Protocol Verification (canonical vectors)
        ↓
Command API
        ↓
Integration Testing
        ↓
PHASE 4 COMPLETE
```

No task can proceed until predecessors complete.

---

## Phase 5 Task Dependencies

```
Protocol freeze (Phase 4 complete)
        ↓
Daemon API stability
        ↓
Frontend Architecture
        ↓
Daemon Integration
        ↓
UI Implementation (Dashboard + Commands + Monitoring)
        ↓
PHASE 5 COMPLETE
```

---

## Estimated Timeline

| Phase | Name | Duration | Status |
|-------|------|----------|--------|
| 1 | Architecture Foundation | ✅ Complete | Frozen |
| 2 | Firmware Implementation | ✅ Complete | Frozen |
| 3 | Protocol Verification | 🔄 1-2 weeks | In Progress |
| 4 | Daemon Implementation | ⬜ 2-3 weeks | Pending Phase 3 |
| 5 | Frontend Development | ⬜ 3-4 weeks | Pending Phase 4 |
| 6 | Testing & Validation | ⬜ 1-2 weeks | Pending Phase 5 |
| 7 | Stability & Optimization | ⬜ 1-2 weeks | Pending Phase 6 |

**Total Remaining:** ~10-14 weeks

---

## Current Phase 3 Breakdown

**Status:** ~90% complete

**Remaining Tasks:**

1. ✅ Read all required documentation
2. ✅ Verify CRC16-Modbus implementation
3. 🔄 Generate canonical protocol test vectors (minimum 5)
4. 🔄 Create protocol freeze checklist
5. 🔄 Create protocol interoperability requirement
6. 🔄 Enhance Rule 14 documentation
7. 🔄 Update PROJECT_STATUS.md with completion
8. 🔄 Create verification report

**Definition of Done for Phase 3:**
- ✅ Firmware verification complete
- ✅ Canonical vectors exist and are documented
- ✅ Protocol interoperability requirement formalized
- ✅ Protocol Freeze Checklist created
- ✅ Rule 14 reflects complete verification chain
- ✅ All governance documents finalized
- ✅ Ready for Phase 4 to begin

---

## Handoff Criteria

**Phase 3 → Phase 4 Handoff Requires:**

1. Protocol Freeze Checklist completed with ✓ marks
2. All 5+ canonical test vectors generated and documented
3. Protocol interoperability requirement in place
4. Rule 14 fully documented with complete verification chain
5. PROJECT_STATUS.md marked "Protocol Frozen"
6. No open issues in protocol specification
7. Technical lead approval for freeze

**Failure to meet all criteria = Do not begin Phase 4.**

---

## Change Control

**To modify this document:**

1. Identify the task or phase to change
2. Document rationale (time savings, dependency change, etc.)
3. Obtain technical lead approval
4. Update this document
5. Communicate change to all teams
6. Note change in PROJECT_STATUS.md

**No ad-hoc reordering. No "just this once" changes.**

---

## Metrics & Tracking

### Phase 3 Completion Metrics

| Metric | Target | Current |
|--------|--------|---------|
| Documentation | 100% | 80% |
| CRC Verification | Complete | In Progress |
| Test Vectors | 5+ | 0 (being created) |
| Governance Rules | All enhanced | Rule 14 enhancement in progress |

### Phase 4 Success Criteria

| Criterion | Requirement |
|-----------|-------------|
| Vector Pass Rate | 100% (all 5+ vectors) |
| CRC Calculation | Byte-for-byte match with firmware |
| Integration Tests | All command types validated |
| Documentation | Complete API specification |

---

## Reference Documents

- **PROJECT_STATUS.md** — Current phase and metrics
- **PROJECT_RULES.md** — Development rules (especially Rule 14)
- **PROTOCOL_SPEC.md** — Protocol specification (frozen for 1.0.0)
- **shared/docs/protocol/test_vectors/** — Canonical vectors (Phase 3 output)

---

## Last Updated

- **Date:** July 2026
- **Phase:** 3 (Protocol Verification)
- **Next Review:** End of Phase 3
