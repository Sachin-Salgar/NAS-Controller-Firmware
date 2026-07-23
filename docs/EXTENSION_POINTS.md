# Extension Points

No extension described here is part of the frozen public protocol unless implemented in firmware.

Implemented extension boundaries:

- Add protocol commands only by updating `Commands.h`, `Commands.cpp`, dispatcher handling, tests, and this documentation together.
- Add scheduled work only by registering tasks in `SchedulerService::Initialize()`.
- Add persistent configuration fields only by changing the binary configuration record version and migration behavior.
- Add hardware behavior behind platform modules and drivers before exposing it through services.

Not currently implemented as dynamic extension systems:

- Runtime plugin loading.
- Protocol capability negotiation.
- Variable command registry discovery.
- Multiple named storage keys mapped to distinct flash locations.
