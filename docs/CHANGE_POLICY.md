# Firmware Change Policy

Firmware Version: **1.0.0**
Status: **FROZEN**
Date Frozen: 2026-07-18

This firmware version is the authoritative implementation for the NAS Controller protocol.

## Frozen Items

The following items are immutable in firmware v1.0.0:

- Packet format (header, sequence, command, payload length, CRC)
- Command IDs (0x0001–0x1702)
- CRC algorithm (CRC-16/Modbus, polynomial 0xA001, initial 0xFFFF)
- Configuration persistence format (magic, version, record layout)
- Statistics format (boot count, USB counters, relay cycles, fan runtime, uptime)
- Event format (type, parameter1, parameter2)
- Response format (sequence echo, command echo, payload or error code)
- Error codes (ResultCode enum)
- Protocol timing (5ms USB/protocol polling, 1000ms temperature/health polling)
- Service interfaces (initialization order, command dispatch groups)

## Protocol Contract

Daemon and web-interface development must consume this firmware **exactly as implemented**. The public protocol is defined in:

- `docs/PROTOCOL_SPEC.md`
- `docs/PROTOCOL_REGISTRY.md`
- `docs/ARCHITECTURE.md`

Any behavior not documented in these specifications is implementation-defined and must not be relied upon.

## Future Changes

### Breaking Protocol Changes

No breaking protocol changes are allowed in v1.0.x.

Breaking changes (new command IDs, packet format changes, CRC algorithm changes) require:

- Protocol version increment (e.g., 0x0101 for v1.1.0, 0x0200 for v2.0.0)
- New firmware version tag
- Updated documentation
- Daemon compatibility review
- Web interface compatibility review

### Bug Fixes

Bug fixes are allowed **only if they do not change the public protocol**.

Examples of allowed fixes:
- Internal state machine correctness
- Hardware driver stability
- Configuration parsing robustness
- Service initialization ordering

Examples of disallowed changes:
- Modifying command payloads
- Changing packet format
- Altering CRC calculation
- Changing error codes
- Adding/removing commands

### New Features in v1.1.0+

If new commands or protocol features are needed, increment the protocol version:

```
Firmware v1.1.0 → Protocol 0x0101
Firmware v2.0.0 → Protocol 0x0200
```

Update the protocol documentation and tag the firmware commit.

## Daemon Development

Daemon development may proceed independently using the frozen protocol contract. The daemon must:

1. Use only documented commands and payloads
2. Handle unsupported commands gracefully (e.g., `LedGet`, `LedSetColor`)
3. Validate responses against the protocol specification
4. Implement timeout/retry logic for unreliable USB communication

## Git Tags

Firmware versions are marked with Git tags:

```
firmware-v1.0.0  ← Current stable protocol
```

Future versions will create new tags:

```
firmware-v1.0.1  ← Bug fix
firmware-v1.1.0  ← New features, compatible protocol
firmware-v2.0.0  ← Breaking changes, new protocol
```

## Review Process

Changes to frozen firmware must go through:

1. Code review (verify no protocol changes)
2. Build verification (`pio run` zero warnings)
3. Protocol audit (compare against PROTOCOL_SPEC.md)
4. Daemon compatibility assessment
5. Git tag creation and documentation update

## Contact

For questions about this policy or proposed changes, consult the project maintainers before modifying frozen firmware.
