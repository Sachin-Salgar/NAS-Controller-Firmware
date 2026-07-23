# Project Status

Firmware is the authoritative implementation for daemon and web-interface work.

## Current firmware contract

- Firmware version: 1.0.0.
- Protocol version: 0x0100.
- Command IDs registered: 28.
- Transport path: USB service receives bytes and protocol service treats each non-empty receive as one complete binary packet.
- Packet format: 0x55AA header, sequence, command, payload length, payload, CRC16.
- CRC: polynomial 0xA001, initial 0xFFFF.

## Implemented services

Configuration, Statistics, System, Relay, Fan, Temperature, LED, Drive, USB, Protocol, Health, Event, and Scheduler services initialize through ServiceManager.

## Unsupported or partial protocol features

- `LedGet` and `LedSetColor` are registered but permanently return `NotSupported` for firmware version 1.0.0.
- No ASCII protocol is implemented.
- No protocol negotiation, authentication, firmware update, file transfer, packet fragmentation, or asynchronous USB event packet mechanism is implemented.
- `ProtocolService::ExecuteCommand()` is a limited helper supporting only Ping and is not the normal packet command path.

## Freeze Status

**OFFICIAL FIRMWARE FREEZE**

This firmware (v1.0.0) is now the authoritative implementation. The binary protocol, packet format, CRC algorithm, command IDs, configuration format, and public firmware behavior are frozen. Future daemon and web interface development must consume this firmware exactly as implemented.

## Project Status

| Item | Status |
| --- | --- |
| Firmware Phase | COMPLETE |
| Firmware Status | FROZEN |
| Protocol Status | FROZEN |
| Architecture Status | FROZEN |
| Documentation Status | FROZEN |
| Verification | PASSED |
| PlatformIO Build | PASSED |
| Compiler Warnings | NONE |
| Ready for Daemon Development | YES |
