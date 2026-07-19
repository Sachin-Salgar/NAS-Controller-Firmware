# NAS Controller Firmware

This directory contains the complete ESP32 firmware source code for the NAS Controller project.

## Directory Structure

```
firmware/
├── src/              # Main firmware source code
│   ├── Config/       # Configuration management
│   ├── Core/         # Core system components
│   ├── Drivers/      # Hardware drivers (relays, fans, LEDs)
│   ├── Objects/      # Data objects and structures
│   ├── Platform/     # Platform-specific code
│   ├── Protocol/     # USB communication protocol
│   ├── Services/     # System services
│   ├── System/       # System management
│   ├── Tests/        # Embedded test framework
│   ├── Utilities/    # Utility functions
│   └── main.cpp      # Firmware entry point
├── include/          # Header files and includes (if any external dependencies)
├── lib/              # External libraries and dependencies
├── test/             # Unit tests and test framework
├── docs/             # Firmware-specific documentation
├── platformio.ini    # PlatformIO build configuration
└── README.md         # This file
```

## Building the Firmware

### Prerequisites
- PlatformIO CLI or IDE installed
- Python 3.x
- ESP32 development board

### Build Commands

```bash
# Build the firmware
platformio run

# Upload to ESP32
platformio run --target upload

# Run tests
platformio test

# Clean build artifacts
platformio run --target clean
```

## Key Components

### Protocol
The firmware communicates with the host daemon via USB CDC serial protocol. See `/docs/Protocol.md` for detailed protocol specification.

### Hardware Control
- **Relay Control**: GPIO-based relay switching
- **Fan Management**: PWM-based fan speed control
- **LED Control**: Configurable LED animations and control

### Configuration
Configuration is stored in EEPROM and can be modified through the host daemon's web interface.

## Testing

The firmware includes a comprehensive self-test suite:
- Core system tests
- Driver tests
- Protocol tests
- Service tests
- System tests

Run tests with:
```bash
cd /root/app/code/firmware
platformio test
```

## Documentation

Detailed documentation is available in:
- `/docs/Protocol.md` - Protocol specification
- `/docs/DEVELOPER_GUIDE.md` - Development guide
- `/docs/CODING_STANDARDS.md` - Coding standards
- `/shared/docs/PROTOCOL_SPEC.md` - Shared protocol specification

## Contributing

Please refer to `/docs/CONTRIBUTING.md` for contribution guidelines.

## License

See `/LICENSE` at the project root.
