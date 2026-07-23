# Firmware Protocol Specification

Status: **FROZEN v1.0.0** – Official protocol specification, frozen to firmware implementation.

Protocol Version: **1.0.0 (0x0100)**
Protocol Status: **FROZEN**
Verification: **PASSED**

## Source of truth

The protocol described here is derived from `src/Protocol`, `src/Services/ProtocolService.cpp`, `src/Config/ProtocolConfig.h`, and `src/Utilities/CRC16.cpp`. If this document conflicts with firmware, firmware wins.

## Versions and limits

- Firmware version: `1.0.0`.
- Protocol version constant: `0x0100`.
- Packet header constant: `0x55AA`.
- Maximum packet size: 512 bytes.
- Maximum payload size: 500 bytes.
- USB receive/transmit buffer size: 512 bytes.

## Packet format

All multi-byte fields are copied with `std::memcpy` to and from native integer objects. On the verified target this is little-endian; no byte-order conversion is implemented.

| Offset | Size | Field |
| --- | ---: | --- |
| 0 | 2 | Header, `0x55AA` |
| 2 | 2 | Sequence |
| 4 | 2 | Command ID |
| 6 | 2 | Payload length |
| 8 | N | Payload |
| 8 + N | 2 | CRC16 |

Minimum packet size is 10 bytes. Expected total length is `10 + payload_length`.

## CRC

CRC is CRC-16/Modbus-style reflected polynomial `0xA001`, initial value `0xFFFF`, calculated over every packet byte before the CRC field. A null CRC input returns `0xFFFF`.

## Validation pipeline

A received packet is rejected if:

1. Packet pointer is null.
2. Length is less than 10 bytes or greater than 512 bytes.
3. Header is not `0x55AA`.
4. Payload length is greater than 500 bytes.
5. Actual length is not `10 + payload_length`.
6. CRC field does not match the calculated CRC.
7. Command ID is not in the registered command list.

## USB receive path and response behavior

`ProtocolService::Process()` reads up to 512 bytes from `USBService::Receive()`. If zero bytes are read, it returns success without parsing. Otherwise the complete received byte count is treated as one packet.

For parsed packets, `CommandDispatcher::Dispatch()` runs the command. Success responses reuse the request sequence and command and include the command response payload. Error responses reuse the request sequence and command and contain a two-byte `ResultCode` payload.

Protocol errors are counted when parsing fails, response construction fails, or command dispatch returns an error.

## Result codes

`ResultCode` is a 16-bit enum. Current order is implementation-defined and starts at `Success = 0`; subsequent values auto-increment in source order: `Failed`, `InvalidArgument`, `InvalidState`, `NotInitialized`, `AlreadyInitialized`, `Timeout`, `Busy`, `Unsupported`, `NotSupported`, `AccessDenied`, `OutOfMemory`, `NullPointer`, `BufferTooSmall`, `BufferFull`, `CommunicationError`, `ChecksumError`, `CrcError`, `InvalidHeader`, `InvalidLength`, `NotFound`, `HardwareError`, `DeviceNotFound`, `InvalidConfiguration`, `InternalError`.

## Public command contract

There are 28 registered command IDs. Command payloads and responses are packed native C++ values with no schema/version wrapper.

See `docs/PROTOCOL_REGISTRY.md` for command IDs, payloads, responses, and unsupported registered operations.

## Explicitly not implemented in firmware protocol

The firmware does not implement an ASCII line protocol, heartbeat command, authentication, firmware update, file transfer, debug console, host state machine, protocol negotiation, asynchronous USB event packets, packet fragmentation, or endian conversion.
