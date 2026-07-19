# USB Host Protocol

## Overview

The NAS Controller communicates with the host PC using USB CDC (Virtual COM Port).

The communication protocol is binary, packet-based, request/response, and transport-independent.

Future transports (Ethernet, Wi-Fi, BLE, UART) shall use the same packet format.

---

# Communication Model

```
Host Application
        │
USB CDC
        │
Protocol Layer
        │
Services
        │
Drivers
        │
Platform HAL
        │
ESP32 Hardware
```

---

# Communication Type

- Request / Response
- Full duplex
- Binary protocol
- Little-endian
- CRC protected

---

# Packet Format

```
+------------+
| Header     | 2 bytes
+------------+
| Sequence   | 2 bytes
+------------+
| Command    | 2 bytes
+------------+
| Length     | 2 bytes
+------------+
| Payload    | N bytes
+------------+
| CRC16      | 2 bytes
+------------+
```

---

# Header

```
0x55AA
```

Used to identify the beginning of every packet.

---

# Sequence Number

Incremented by the host.

Returned unchanged by the firmware.

Allows matching responses to requests.

---

# Command

16-bit command identifier.

Examples:

```
0x0001  Ping
0x0002  Get Version
0x1001  Relay Get
0x1102  Fan Set Speed
0x1201  Temperature Get
```

Complete command list is documented in **CommandSet.md**.

---

# Payload Length

Number of payload bytes.

Does not include:

- Header
- Sequence
- Command
- CRC

---

# CRC

CRC16 Modbus

Polynomial:

```
0xA001
```

Initial value:

```
0xFFFF
```

---

# Request Flow

```
PC

↓

Build Packet

↓

USB Write

↓

ESP32

↓

Validate CRC

↓

Parse Packet

↓

Dispatch Command

↓

Execute Service

↓

Build Response

↓

USB Write
```

---

# Response Flow

```
ESP32

↓

Result

↓

ResponseBuilder

↓

PacketBuilder

↓

USB Write

↓

Host
```

---

# Error Handling

Possible responses include:

- Invalid Header
- Invalid Length
- Invalid CRC
- Unknown Command
- Invalid Payload
- Busy
- Internal Error

Errors use the standard ResultCode enumeration.

---

# Transport Independence

The packet format shall remain identical for:

- USB CDC
- UART
- TCP/IP
- UDP
- Wi-Fi
- BLE

Only the transport changes.

The Protocol layer remains unchanged.

---

# Future Host Software

The PC application will consist of:

- USB Transport
- Packet Builder
- Packet Parser
- Command API
- GUI

The GUI must never communicate directly with the serial port.

---

# Versioning

Protocol Version:

```
1.0
```

Backward compatibility will be maintained throughout Firmware Version 1.x.

---

# References

- Architecture.md
- Protocol.md
- CommandSet.md
- ADR-0003 Platform HAL
- ADR-0007 Protocol Layer