# NAS Controller

A complete ecosystem for controlling and monitoring external NAS hardware via an ESP32 microcontroller with a modern web interface.

## Project Structure

This is a multi-component project organized into distinct subsystems:

```
├── firmware/                  # ESP32 Firmware (embedded C++17)
│   ├── src/                   # Main firmware source code
│   ├── include/               # Header files
│   ├── lib/                   # External libraries
│   ├── test/                  # Unit tests
│   ├── docs/                  # Firmware-specific documentation
│   ├── platformio.ini         # PlatformIO build configuration
│   └── README.md              # Firmware README
│
├── daemon/                    # Host Daemon (Node.js/Python/C#)
│   └── ...                    # Daemon source code (TBD)
│
├── frontend/                  # Web Dashboard (React/Vue)
│   ├── docs/
│   │   ├── WEB_INTERFACE_PLAN.md    # UI/UX specifications
│   │   └── README.md
│   └── ...                    # Frontend source code (TBD)
│
├── shared/                    # Shared Components & Documentation
│   ├── docs/
│   │   ├── PROTOCOL_SPEC.md   # Binary protocol specification (shared contract)
│   │   ├── README.md
│   │   └── adr/               # Architecture Decision Records
│   └── ...                    # Shared utilities (TBD)
│
├── docs/                      # Project-wide Documentation
│   ├── Architecture.md        # System architecture
│   ├── DEVELOPER_GUIDE.md     # Development setup and guidelines
│   ├── CODING_STANDARDS.md    # Code conventions and standards
│   ├── Protocol.md            # Detailed protocol documentation
│   ├── Hardware.md            # Hardware specifications
│   ├── CONTRIBUTING.md        # Contribution guidelines
│   └── ...                    # Additional documentation
│
├── LICENSE                    # License
├── README.md                  # This file
└── .gitignore, .gitattributes, .vscode/

```

## Quick Start

### For Firmware Development

Navigate to the `firmware/` directory:

```bash
cd firmware
platformio run              # Build
platformio run --target upload  # Deploy to ESP32
platformio test            # Run tests
```

See `firmware/README.md` for detailed firmware instructions.

### For Host Daemon Development

See `daemon/README.md` (when available) for daemon setup and development.

### For Web Interface Development

See `frontend/README.md` and `frontend/docs/WEB_INTERFACE_PLAN.md` for UI/UX specifications and development instructions.

## Architecture Overview

The NAS Controller consists of three main components:

### 1. **Firmware (ESP32)**
- Embedded C++17 application running on an ESP32
- Manages hardware: relays, fans, temperature sensors, LEDs
- Communicates via USB CDC serial protocol
- Located in: `firmware/`

### 2. **Host Daemon**
- Bridges between the ESP32 firmware and web interface
- REST API endpoints and WebSocket server
- USB protocol translator and state management
- Located in: `daemon/`

### 3. **Web Dashboard**
- Modern web-based control interface
- Real-time monitoring and control
- LED animation builder
- Configuration editor
- Located in: `frontend/`

### 4. **Shared Contract**
- Binary protocol specification (`shared/docs/PROTOCOL_SPEC.md`)
- Ensures firmware and daemon synchronization
- Located in: `shared/`

## Key Documents

- **`WEB_INTERFACE_PLAN.md`** → `frontend/docs/WEB_INTERFACE_PLAN.md` - Web interface specifications
- **`PROTOCOL_SPEC.md`** → `shared/docs/PROTOCOL_SPEC.md` - Communication protocol
- **`docs/Architecture.md`** - System design and layering
- **`docs/DEVELOPER_GUIDE.md`** - Development setup
- **`docs/CODING_STANDARDS.md`** - Coding conventions

## Firmware Features

- **Power Control** – GPIO-based relay switching
- **Fan Management** – PWM-based speed control
- **Temperature Monitoring** – OneWire sensor integration
- **LED Control** – WS2812B addressable LED strips
- **USB Communication** – Binary protocol with CRC16 validation
- **Configuration** – Persistent EEPROM storage
- **Statistics** – Runtime metrics tracking
- **Watchdog** – System health recovery

See `firmware/README.md` for complete firmware documentation.

## Current Status

- ✅ Firmware source code: **Reorganized to `firmware/` directory**
- ✅ Documentation: **Organized by component**
- 🔄 Host Daemon: **Structure prepared (implementation TBD)**
- 🔄 Web Frontend: **Structure prepared (implementation TBD)**

## Technology Stack

| Component | Technology |
|-----------|-----------|
| **Firmware** | C++17, PlatformIO, Arduino framework |
| **Microcontroller** | ESP32-WROOM-32 |
| **Daemon** | Node.js / Python / C# (TBD) |
| **Frontend** | React / Vue (TBD) |

## Contributing

Please refer to `docs/CONTRIBUTING.md` for contribution guidelines.

## License

See `LICENSE` file for details.

## Getting Help

- 📖 **Architecture Questions**: See `docs/Architecture.md`
- 🛠️ **Development Setup**: See `docs/DEVELOPER_GUIDE.md`
- 💻 **Coding Standards**: See `docs/CODING_STANDARDS.md`
- 🚀 **Firmware Setup**: See `firmware/README.md`
- 🌐 **Web Interface Design**: See `frontend/docs/WEB_INTERFACE_PLAN.md`
