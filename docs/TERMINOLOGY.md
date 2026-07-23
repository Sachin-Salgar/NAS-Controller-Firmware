# Firmware Terminology

- Packet: binary protocol frame with header, sequence, command, payload length, payload, and CRC16.
- Command ID: 16-bit numeric command identifier from `NAS::Protocol::Commands`.
- Payload: raw bytes interpreted by the dispatcher using native C++ scalar layout.
- Response: packet built with the original sequence and command. On error, payload is a 16-bit `ResultCode`.
- Service: static subsystem initialized by `ServiceManager`.
- Driver: hardware-specific wrapper above platform modules.
- Platform: ESP32/Arduino-facing hardware abstraction.
- Configuration record: persisted binary record with magic, version, configuration sections, and CRC.
- Implementation-defined: behavior directly dependent on current firmware C++ layout, target endian behavior, or platform implementation.
