Document Version: 1.0  
Project Version: 1.0  
Last Updated: 2026-07-19  
Owner: NAS Controller  
Status: Frozen

# NOT_PLANNED.md

This document explicitly defines what the NAS Controller project intentionally will NOT include. This prevents scope creep and helps future contributors understand the project boundaries.

---

## Phase 1-3 Exclusions (v1.0)

### Network & Remote Access

**❌ Remote Internet Control**
- No cloud service integration
- No remote internet access to daemon
- **Reason:** Local network only simplifies security model and infrastructure
- **Roadmap:** Phase 4+ (optional, requires security review)

**❌ Multi-User / User Accounts**
- No user authentication or authorization
- No role-based access control (RBAC)
- No multi-user sync or conflict resolution
- **Reason:** Single-operator scenario for v1
- **Roadmap:** Phase 4+ (if multiple operators needed)

**❌ Network Protocol (TCP/IP Daemon)**
- Daemon listens only on localhost (127.0.0.1)
- No network interface exposure in v1
- **Reason:** Simplifies security and deployment for single-machine scenarios
- **Roadmap:** Phase 4+ (with proper authentication and TLS)

**❌ MQTT, REST API Publishing, or Pub/Sub**
- No MQTT broker integration
- No automatic state publishing to external systems
- **Reason:** Simplifies architecture for v1
- **Roadmap:** Phase 4+ (if external system integration needed)

---

### Database & Persistence

**❌ Database Server**
- No PostgreSQL, MySQL, MongoDB, etc.
- State stored in daemon memory + firmware EEPROM
- Event logs stored in JSON files or memory
- **Reason:** Adds deployment complexity without benefit for v1
- **Roadmap:** Phase 4+ (if analytics/historical analysis needed)

**❌ Cloud Sync / Cloud Backup**
- No AWS, Google Cloud, Azure storage
- No automatic configuration backup to cloud
- **Reason:** Local-first design, user responsible for backups
- **Roadmap:** Phase 4+ (if distributed management needed)

---

### Hardware & Platforms

**❌ Firmware Changes**
- Existing ESP32 firmware used as-is
- No firmware modifications
- **Reason:** Daemon provides abstraction without firmware changes
- **Architecture:** Separation of concerns ensures this remains true

**❌ Additional Microcontrollers**
- v1.0 targets ESP32 only
- No Arduino, STM32, Raspberry Pi firmware in v1
- **Reason:** Simplifies protocol and testing
- **Roadmap:** Phase 4+ (with transport layer abstraction - already prepared)

**❌ OLED Display, GPIO Buttons, or Hardware UI**
- No physical display on daemon hardware
- No hardware buttons to control daemon
- **Reason:** Web UI is the control surface in v1
- **Roadmap:** Phase 4+ (if standalone operation needed)

---

### Mobile & Cross-Platform

**❌ Native Mobile Apps (v1)**
- No iOS or Android app in v1
- No React Native or Flutter implementation
- **Reason:** Web UI serves all platforms (responsive design)
- **Roadmap:** Phase 4+ (if native app UX justifies effort)

**❌ Desktop App (Electron)**
- No Electron wrapper around web UI
- **Reason:** Web UI runs in any browser
- **Roadmap:** Phase 4+ (if offline desktop app needed)

---

### Advanced Features

**❌ Firmware Update Over USB**
- No OTA (Over-The-Air) firmware updates from daemon
- Users update firmware via PlatformIO directly
- **Reason:** Firmware is stable, manual updates are safe
- **Roadmap:** Phase 4+ (if frequent firmware updates needed)

**❌ Automation Rules Engine**
- No conditional logic (IF temperature > X, THEN fan ON)
- No scheduled tasks (cron-like scheduling)
- No event-driven workflows
- **Reason:** MVP is manual control only
- **Roadmap:** Phase 2-3 (simple rules) or Phase 4+ (complex automation)

**❌ Alerts & Notifications**
- No email alerts
- No SMS notifications
- No push notifications
- No Slack/Discord webhooks
- **Reason:** Event log provides visibility; monitoring is manual
- **Roadmap:** Phase 4+ (if 24/7 monitoring needed)

**❌ Firmware Telemetry / Analytics**
- No metrics collection to external service
- No performance dashboards
- No anomaly detection
- **Reason:** Local diagnostics sufficient for v1
- **Roadmap:** Phase 4+ (if distributed fleet management needed)

---

### Manufacturing & Testing

**❌ Batch Testing / Burn-in Suite**
- No pre-programmed test sequences
- No pass/fail validation matrix
- No automated test report generation
- **Reason:** Manual testing sufficient for small batches
- **Roadmap:** Phase 4+ (if high-volume manufacturing)

**❌ Factory Reset / Firmware Recovery Tool**
- No automatic recovery from corrupted firmware
- User must reflash manually via PlatformIO
- **Reason:** Firmware is robust; recovery tools add complexity
- **Roadmap:** Phase 4+ (if recovery frequency warrants)

---

### Security (v1 Simplified Model)

**❌ TLS / HTTPS**
- No HTTPS for localhost connections (unnecessary)
- No certificate management
- **Reason:** localhost-only eliminates network eavesdropping risk
- **Roadmap:** Phase 4+ (if network exposure added)

**❌ API Key / OAuth Authentication**
- No API key requirement for localhost
- No OAuth provider integration
- **Reason:** Single-user, local-only in v1
- **Roadmap:** Phase 4+ (with multi-user or remote access)

**❌ Rate Limiting by User**
- Rate limiting is per-command (prevents flooding)
- No per-user limits
- **Reason:** Single user in v1
- **Roadmap:** Phase 4+ (with multi-user)

**❌ Audit Log for Compliance**
- No audit trail for regulatory (HIPAA, PCI-DSS, etc.)
- System logging for debugging only
- **Reason:** Not a compliance-required system in v1
- **Roadmap:** Phase 4+ (if compliance required)

---

### Frontend Features

**❌ Dark Mode**
- Single light theme in v1
- **Reason:** Simplifies CSS, user can use browser dark mode
- **Roadmap:** Phase 3+ (if user demand warrants)

**❌ Internationalization (i18n)**
- English only in v1
- No support for other languages
- **Reason:** Adds complexity without immediate benefit
- **Roadmap:** Phase 3+ (if international user base)

**❌ Accessibility Features (Advanced)**
- No screen reader optimization in v1
- No keyboard-only navigation
- **Reason:** Basic HTML structure supports assistive tech; advanced optimizations later
- **Roadmap:** Phase 3+ (if accessibility required)

**❌ Advanced Data Visualization**
- No complex charts or 3D visualizations
- Simple line graphs for temperature
- **Reason:** MVP is functional, not fancy
- **Roadmap:** Phase 2-3 (if analytics dashboard added)

**❌ Custom Themes / Skinning**
- No theme editor
- No user-configurable colors
- **Reason:** Single professional theme in v1
- **Roadmap:** Phase 4+ (if branding customization needed)

---

### Infrastructure & DevOps

**❌ Kubernetes Deployment**
- No Helm charts or K8s manifests
- Docker container optional (not required)
- **Reason:** Single-server deployment in v1
- **Roadmap:** Phase 4+ (if distributed deployment needed)

**❌ Load Balancing / High Availability**
- Single daemon instance
- No failover or redundancy
- **Reason:** Single PC/server scenario
- **Roadmap:** Phase 4+ (if HA required)

**❌ Container Orchestration**
- No Docker Compose in v1
- Standalone Docker image only (optional)
- **Reason:** Simplifies setup for v1
- **Roadmap:** Phase 3+ (if multi-container services added)

**❌ CI/CD Pipeline Complexity**
- No complex release workflows
- No blue-green deployments
- Simpler CI/CD in v1
- **Reason:** Appropriate for project maturity
- **Roadmap:** Phase 4+ (as project scales)

---

## Why These Exclusions Matter

1. **Clarity:** Knowing what's NOT planned prevents wasted effort on out-of-scope features
2. **Scope Management:** Clear boundaries enable focused development on priorities
3. **Future Flexibility:** Documented as "Phase 4+" leaves options open without committing now
4. **Architectural Simplicity:** Exclusions let v1.0 be lean and focused

---

## How to Add to In-Scope

If you believe a NOT_PLANNED feature should be included:

1. Document the use case and why it matters
2. Present to stakeholders with impact analysis
3. Update WEB_INTERFACE_PLAN.md with priority
4. Estimate effort and timeline
5. Add to appropriate phase (1, 2, 3, or 4+)
6. Update this document with decision

---

## Related Documents

- [WEB_INTERFACE_PLAN.md](../WEB_INTERFACE_PLAN.md) - What IS planned
- [EXTENSION_POINTS.md](./EXTENSION_POINTS.md) - Where future features can plug in
- [docs/IMPLEMENTATION_CHECKLIST.md](./IMPLEMENTATION_CHECKLIST.md) - Detailed Phase 1-3 tasks

---

**Purpose:** Maintain clear project boundaries and prevent scope creep  
**Status:** Frozen (changes require stakeholder consensus)
