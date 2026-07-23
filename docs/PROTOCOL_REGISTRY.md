# Firmware Protocol Registry

This registry mirrors `src/Protocol/Commands.h` and `src/Protocol/CommandDispatcher.cpp`.

## Command IDs

| ID | Name | Request payload | Success response | Notes |
| --- | --- | --- | --- | --- |
| `0x0001` | Ping | empty | empty | Supported |
| `0x0002` | GetVersion | empty | `uint16 major`, `uint16 minor`, `uint16 patch` | Supported |
| `0x0003` | GetBuildInfo | empty | Eight length-prefixed NUL-terminated strings: firmware name, build date, build time, build configuration, compiler, target platform, target board, git commit | Supported |
| `0x0004` | GetSystemStatus | empty | `uint8 state`, `bool usbConnected`, `uint32 freeHeap`, `float cpuTemperature`, `uint32 uptime` | Supported |
| `0x0005` | Restart | empty | empty if platform soft restart returns | Calls platform soft restart |
| `0x1001` | RelayGet | `uint8 id` | `uint8 id`, `bool on`, `bool enabled` | IDs 1-2 |
| `0x1002` | RelaySet | `uint8 id`, `bool on` | empty | IDs 1-2; increments relay cycle statistic on success |
| `0x1003` | RelayToggle | `uint8 id` | empty | IDs 1-2; increments relay cycle statistic on success |
| `0x1101` | FanGet | `uint8 id` | `uint8 channel`, `uint8 speed`, `uint8 mode` | IDs 1-5 |
| `0x1102` | FanSetSpeed | `uint8 id`, `uint8 speed` | empty | IDs 1-5 |
| `0x1103` | FanSetMode | `uint8 id`, `uint8 mode` | empty | Mode values 0-1 |
| `0x1201` | TemperatureGet | `uint8 id` | `float temperature` | IDs 0-4 |
| `0x1202` | TemperatureGetAll | empty | five `float temperature` values | Supported |
| `0x1301` | LedGet | `uint8 id` | error | Registered but returns `NotSupported` |
| `0x1302` | LedSetColor | `uint8 id`, remaining payload ignored by dispatcher after ID | error | Registered but returns `NotSupported` |
| `0x1303` | LedSetMode | `uint8 id`, `uint8 state` | empty | LED IDs 0-59; state values 0-6 |
| `0x1304` | LedOff | `uint8 id` | empty | LED IDs 0-59 |
| `0x1401` | DriveGet | `uint8 id` | drive record | IDs 1-10 |
| `0x1402` | DriveGetAll | empty | ten drive records | Supported |
| `0x1403` | DrivePowerOn | `uint8 id` | empty | Sets drive present flag true |
| `0x1404` | DrivePowerOff | `uint8 id` | empty | Sets drive present flag false |
| `0x1501` | ConfigurationLoad | empty | empty | Loads persistent configuration |
| `0x1502` | ConfigurationSave | empty | empty | Saves persistent configuration |
| `0x1503` | ConfigurationReset | empty | empty | Resets in RAM to defaults |
| `0x1601` | StatisticsGet | empty | statistics record | Supported |
| `0x1602` | StatisticsReset | empty | empty | Supported |
| `0x1701` | EventRead | empty | `uint16 type`, `uint32 parameter1`, `uint32 parameter2` | Returns next queued event or service result |
| `0x1702` | EventClear | empty | empty | Clears event queue |

## Composite response records

Drive record: `uint8 id`, `bool present`, `uint8 state`, `uint8 health`, `float temperature`, `uint64 capacity`, length-prefixed NUL-terminated serial string.

Statistics record: `uint32 bootCount`, `uint32 usbPacketsReceived`, `uint32 usbPacketsTransmitted`, `uint32 protocolErrors`, `uint32 relay1Cycles`, `uint32 relay2Cycles`, `uint32 fanRuntime`, `uint32 systemUptime`.

Length-prefixed string format: `uint16 length` followed by `length` bytes including the terminating NUL.

## Command dispatch groups

The dispatcher routes by the high byte: `0x0000` system, `0x1000` relay, `0x1100` fan, `0x1200` temperature, `0x1300` LED, `0x1400` drive, `0x1500` configuration, `0x1600` statistics, and `0x1700` event. Registered commands outside their implemented switch cases return `NotSupported`.
