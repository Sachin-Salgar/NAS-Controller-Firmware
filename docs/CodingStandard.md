# NAS Controller Firmware
# Coding Standard

Document ID: NCF-CODING-001

Document Version: 1.0

Status: Released

Applies To: Firmware Version 1.x

Language: C++17

Copyright © NAS Controller Firmware Project

---

# Revision History

| Version | Date | Description |
|----------|------|-------------|
| 1.0 | Initial Release | Coding Standard |

---

# Table of Contents

1. Purpose
2. Design Philosophy
3. Language Standard
4. Project Architecture
5. Naming Conventions
6. File Organization
7. Class Design Rules
8. Memory Management
9. Error Handling
10. Logging
11. Concurrency
12. Dependencies
13. Coding Rules
14. Documentation
15. Testing
16. Performance Guidelines
17. Compiler Settings
18. Code Review Checklist
19. Coding Freeze Rules

---

# 1. Purpose

This document defines the mandatory coding standards for NAS Controller Firmware.

All source code SHALL comply with this document.

No source file may intentionally violate these rules without an approved Architecture Decision Record (ADR).

---

# 2. Design Philosophy

The firmware SHALL prioritize

- Reliability
- Readability
- Maintainability
- Deterministic behaviour
- Hardware safety
- Modular architecture

Code readability SHALL always take precedence over writing shorter or clever code.

---

# 3. Language Standard

Language

C++17

Compiler

PlatformIO Toolchain

Exceptions

Disabled

RTTI

Disabled unless required

Dynamic Casting

Prohibited

C-style casts

Prohibited

Use

static_cast

reinterpret_cast

const_cast

only when technically justified.

---

# 4. Project Architecture

The dependency direction SHALL always be

Application
↓
Protocol
↓
Services
↓
Objects
↓
Drivers
↓
Platform

Lower layers SHALL NEVER depend on higher layers.

Circular dependencies are prohibited.

---

# 5. Naming Conventions

## Classes

PascalCase

Example

RelayService

FanController

TemperatureSensor

---

## Interfaces

Prefix

I

Example

IRelay

ILogger

ITransport

---

## Structures

PascalCase

Example

SystemStatus

DriveInfo

---

## Enumerations

enum class

Example

enum class RelayState

---

## Functions

PascalCase

Example

Initialize()

Process()

Update()

ReadTemperature()

---

## Variables

camelCase

Example

relayState

fanSpeed

temperatureValue

---

## Member Variables

Suffix

_

Example

relayState_

logger_

temperatureSensor_

---

## Constants

Prefix

k

Example

kMaxFans

kDefaultTimeout

---

## Macros

UPPER_CASE

Macros SHALL be avoided whenever possible.

---

## Namespaces

NAS::<Layer>

Example

NAS::Drivers

NAS::Services

NAS::Protocol

---

# 6. File Organization

One class per file.

Example

RelayService.h

RelayService.cpp

File names SHALL match class names.

No unrelated classes in the same source file.

---

# 7. Class Design Rules

Every class SHALL have one clearly defined responsibility.

Every public class SHALL document

Purpose

Responsibilities

Dependencies

Thread Safety

Public API

Classes SHOULD remain below approximately 500 lines of implementation where practical.

Large responsibilities SHALL be divided into smaller classes.

---

# 8. Memory Management

Dynamic memory allocation SHALL NOT occur after system initialization.

Prohibited

new

delete

malloc()

free()

Preferred

Static allocation

Stack allocation

Fixed-size containers

Heap fragmentation SHALL be avoided.

---

# 9. Error Handling

Boolean return values SHALL NOT be used to communicate complex errors.

Preferred

Result

InitResult

StatusCode

Example

Result Initialize();

Every failure SHALL provide sufficient diagnostic information.

---

# 10. Logging

Direct use of

Serial.print()

Serial.println()

is prohibited outside the Platform layer.

All logging SHALL use the Logger interface.

Log Levels

Trace

Debug

Info

Warning

Error

Fatal

Logging SHALL be configurable.

---

# 11. Concurrency

Interrupt Service Routines SHALL

Execute quickly

Avoid blocking

Avoid dynamic allocation

Avoid logging

Avoid complex processing

Long-running work SHALL be deferred to scheduled tasks.

Shared resources SHALL be protected appropriately.

---

# 12. Dependencies

Dependency Injection SHALL be preferred over global access.

Global variables are prohibited except for hardware startup requirements.

Singletons SHALL be avoided unless technically justified.

Modules SHALL communicate through well-defined interfaces.

---

# 13. Coding Rules

Magic numbers are prohibited.

Example

Incorrect

delay(137);

Correct

delay(kStartupDelayMs);

Prefer constexpr over macros.

Prefer enum class over integer constants.

Use explicit constructors unless implicit conversion is intended.

Mark overridden functions with

override

Deleted copy operations SHALL be explicit where required.

Use nullptr instead of NULL.

Prefer const correctness.

Use references instead of pointers when null is not valid.

Use std::array instead of raw arrays where appropriate.

Avoid recursive functions.

Avoid hidden side effects.

---

# 14. Documentation

Every public class SHALL contain a documentation block describing

Purpose

Responsibilities

Dependencies

Thread Safety

Example Usage

Every public function SHALL include

Description

Parameters

Return Value

Failure Conditions

Complex algorithms SHOULD include explanatory comments.

Comments SHALL explain WHY rather than WHAT.

---

# 15. Testing

Business logic SHALL be independent of hardware whenever practical.

Drivers SHALL be testable through abstraction.

Unit tests SHALL be written for non-hardware logic.

Regression tests SHALL accompany bug fixes where applicable.

---

# 16. Performance Guidelines

Avoid unnecessary copying.

Prefer pass-by-reference for large objects.

Avoid blocking delays.

Use non-blocking state machines.

Prefer compile-time evaluation using constexpr where possible.

Minimize RAM usage.

Optimize for deterministic execution rather than maximum throughput.

---

# 17. Compiler Settings

Warnings SHALL be enabled.

Warnings SHALL be treated as errors where practical.

Unused variables SHALL be removed.

Unused includes SHALL be removed.

Compiler optimization SHALL prioritize stability.

---

# 18. Code Review Checklist

Every pull request SHALL verify

✓ Compiles successfully

✓ No warnings

✓ No memory leaks

✓ Layering rules respected

✓ No circular dependencies

✓ Naming conventions followed

✓ Documentation updated

✓ Logging appropriate

✓ Error handling implemented

✓ No magic numbers

✓ No unnecessary globals

✓ Public API documented

✓ Tests updated where applicable

---

# 19. Coding Freeze Rules

These standards are frozen for Firmware Version 1.x.

Changes to this document SHALL require

- Architecture Decision Record (ADR)

or

- Major Firmware Version increment

No developer SHALL intentionally violate these standards without documented approval.