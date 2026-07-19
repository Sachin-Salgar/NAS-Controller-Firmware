# NAS Controller Web Dashboard

The web-based user interface for controlling and monitoring the NAS Controller hardware.

## Status

Currently in the **planning phase**. Complete specifications are in `docs/WEB_INTERFACE_PLAN.md`.

## Features (Planned)

- **Dashboard** - Real-time hardware status overview
- **Control Panel** - Direct control of relays, fans, and LEDs
- **Configuration Editor** - Edit and save controller settings
- **LED Animation Builder** - Visual LED animation designer
- **Test Console** - Run firmware test sequences
- **Statistics & Logging** - Historical data and event logs
- **WebSocket Integration** - Real-time updates from daemon

## Architecture

```
Browser (Chrome, Firefox, Safari)
    ↓ HTTP/WebSocket (localhost:3000)
    ↓
Host Daemon (REST API + WebSocket server)
    ↓ USB CDC Serial (115200 baud)
    ↓
ESP32 Firmware
```

## Documentation

- **Architecture & Design**: See `docs/WEB_INTERFACE_PLAN.md`
- **Technology Stack**: See `docs/WEB_INTERFACE_PLAN.md` (Section: Technology Stack)
- **Feature Breakdown**: See `docs/WEB_INTERFACE_PLAN.md` (Section: Feature Categories)
- **Integration Points**: See `docs/WEB_INTERFACE_PLAN.md` (Section: Integration Points)

## Getting Started

(Instructions TBD once implementation begins)

## Contributing

Please refer to `/docs/CONTRIBUTING.md` for contribution guidelines.
