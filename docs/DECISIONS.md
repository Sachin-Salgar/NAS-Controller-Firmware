# Firmware Decisions

This summary records implemented decisions only.

- Firmware source code is the source of truth for protocol and behavior.
- Host communication uses a binary USB packet protocol, not the older ASCII line-oriented draft.
- Commands are identified by 16-bit numeric IDs grouped by high byte.
- Packet integrity uses CRC16 with polynomial `0xA001` and initial value `0xFFFF`.
- Services are initialized by `ServiceManager` in a fixed sequence and owned as static singleton-style service state.
- Scheduler execution is cooperative and task-based.
- Configuration persistence is a binary record in flash through `StorageDriver`; key names are not mapped to distinct flash addresses.
- Unsupported features must return firmware result codes instead of being documented as available.
