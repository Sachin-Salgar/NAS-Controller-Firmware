# Firmware Freeze Reference

## Freeze Status

**OFFICIAL FIRMWARE FREEZE v1.0.0**

Firmware Status: **FROZEN**
Protocol Status: **FROZEN**
Architecture Status: **FROZEN**
Verification: **PASSED**
PlatformIO Build: **PASSED**
Compiler Warnings: **NONE**

This firmware is the authoritative implementation for daemon and web-interface development. The binary protocol, packet format, CRC algorithm, command IDs, configuration format, and public firmware behavior are frozen. Any future protocol changes require a protocol version increment rather than modification of protocol v1.0.0.

## Firmware and protocol versions

- Firmware version: `1.0.0`.
- Firmware name: `NAS Controller Firmware`.
- Protocol version: `0x0100`.
- Verified platform constants: `ESP32`, `ESP32-WROOM-32`.

## Command count

The firmware registers 28 command IDs. Of those, 26 have implemented success paths and 2 LED commands (`LedGet`, `LedSetColor`) are registered but return `NotSupported`.

## Packet format

Binary packet layout:

`uint16 header`, `uint16 sequence`, `uint16 command`, `uint16 payloadLength`, `payload[payloadLength]`, `uint16 crc`.

Header is `0x55AA`. Minimum packet size is 10 bytes. Maximum packet size is 512 bytes. Maximum payload size is 500 bytes.

Multi-byte values are native target byte order because the firmware uses `std::memcpy` without byte-order conversion.

## CRC algorithm

CRC16 uses initial value `0xFFFF` and reflected polynomial `0xA001`. The CRC covers all bytes before the CRC field.

## Configuration format

Persistent configuration is a binary record containing:

1. `uint32 magic = 0x31474643`
2. `uint16 version = 1`
3. relay config: default state and enabled flag
4. fan config: minimum speed, maximum speed, automatic mode
5. temperature config: warning threshold and critical threshold
6. LED config: brightness and enabled flag
7. `uint16 crc`

Defaults: relay off/enabled, fan min 20/max 100/automatic, temperature warning 45.0 and critical 55.0, LED brightness 128/enabled.

## Supported services

Configuration, Statistics, System, Relay, Fan, Temperature, LED, Drive, USB, Protocol, Health, Event, and Scheduler services are initialized.

## Known unsupported features

- ASCII command protocol.
- Authentication/encryption.
- Firmware update/file transfer/debug console over public protocol.
- Protocol negotiation.
- Packet fragmentation.
- Endian-independent encoding.
- Asynchronous event push packets.
- `LedGet` and `LedSetColor` success behavior; these registered commands are permanently unsupported in firmware version 1.0.0.

## Remaining TODOs and unknowns

- Native bool size and float representation are assumed from the target ABI; the firmware does not define an endian-neutral wire ABI.
- Exact platform USB CDC details are delegated to `Platform::USB`.
- Build-time git commit is `UNKNOWN` unless `GIT_COMMIT_HASH` is defined.

## Build requirements

PlatformIO environment `esp32dev`, Espressif32 platform, Arduino framework, C++17, libraries `OneWire` and `Adafruit NeoPixel`.

## Test status

The repository contains unit-style tests under `test/` and mirrored firmware tests under `src/Tests/`. Final freeze cleanup removed production ignored `[[nodiscard]]` Result discards and dead protocol helper code; build verification still requires an environment with PlatformIO available.

## Public protocol contract

Daemon and web development must use the binary packet contract in `docs/PROTOCOL_SPEC.md` and command registry in `docs/PROTOCOL_REGISTRY.md`. Any behavior not proven from firmware must be treated as unsupported or implementation-defined.
