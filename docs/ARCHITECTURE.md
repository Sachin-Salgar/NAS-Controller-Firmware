# Firmware Architecture

Status: **FROZEN v1.0.0** – Official architecture specification, frozen to firmware implementation.

Architecture Version: **1.0.0**
Architecture Status: **FROZEN**
Verification: **PASSED**

This document describes the implemented architecture, not a planned redesign. This architecture is authoritative for firmware v1.0.0 and frozen for future daemon and web-interface development.

## Ownership and relationships

`main.cpp` calls system startup, then application initialization, then the run loop. Startup initializes all services through `ServiceManager::Initialize()`. Application initialization starts services through `ServiceManager::Start()`. Application run repeatedly calls `ServiceManager::Execute()`.

Services own static singleton-style state. Objects hold runtime data. Drivers wrap platform abstractions. The protocol layer validates, parses, builds, and dispatches packets but service logic performs state changes.

## Initialization order

`ServiceManager::Initialize()` initializes services in this exact order:

1. ConfigurationService
2. StatisticsService
3. SystemService
4. RelayService
5. FanService
6. TemperatureService
7. LedService
8. DriveService
9. USBService
10. ProtocolService
11. HealthService
12. EventService
13. SchedulerService

`ServiceManager::Start()` increments boot count, sets system state to Ready, publishes `SystemStarted`, then starts the scheduler.

## Shutdown order

`ServiceManager::Stop()` publishes `SystemShutdown`, sets system state to Shutdown, then stops the scheduler. There is no implemented destructor-style teardown sequence.

## Scheduler

`SchedulerService` initializes the core scheduler and registers five tasks: USB processing every 5 ms, protocol processing every 5 ms, temperature update every 1000 ms, health update every 1000 ms, and statistics update every 1000 ms. No fan-only or LED-only scheduler interval constants are present in the frozen implementation.

The scheduler is cooperative. Each execution compares `millis()` against task last-execution timestamps and invokes due task functions.

## Protocol pipeline

USB bytes flow through `USBService::Receive()` to `ProtocolService::ProcessPacket()`, then to `PacketParser::Parse()`, `PacketValidator::Validate()`, `CommandDispatcher::Dispatch()`, `ResponseBuilder`, `PacketBuilder`, and finally `USBService::Send()`.

## Configuration persistence

Configuration is a binary `ConfigurationRecord` stored via `StorageDriver` at flash address 0, despite the service passing the string key `"config"`. The record contains magic `0x31474643`, version `1`, relay, fan, temperature, LED configuration structs, and CRC. Defaults are restored on initialization before attempting load and also after invalid load data.

## Event queue

Event service exposes a fixed queue size of 32 events. Events contain a 16-bit type and two 32-bit parameters. The service is used for system start/shutdown and by other services where implemented.

## Hardware abstraction

Platform modules wrap Arduino/ESP32-facing GPIO, PWM, USB, Flash, OneWire, SPI, I2C, ADC, UART, timers, watchdog, restart, and WS2812 behavior. Drivers use platform modules; services use drivers and objects.

## Implementation-defined behavior

Packet fields, payload records, and response records use native C++ memory layout. The firmware does not implement cross-endian conversion or ABI-stable struct serialization beyond explicit field writes in the command dispatcher.
