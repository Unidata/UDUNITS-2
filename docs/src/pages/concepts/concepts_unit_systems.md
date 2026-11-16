---
title: Unit Systems
sidebar: sidebar
toc: true
permalink: concepts-unit-systems.html
---

A **unit system** is a collection of unit definitions that UDUNITS-2 uses to interpret, compare, and convert units.

Rather than hard-coding unit definitions, UDUNITS-2 loads them from an XML units database.
The resulting unit system defines the vocabulary and relationships that the library recognizes.

Every unit used by UDUNITS-2 belongs to exactly one unit system.

## The XML Units Database

Most applications use the standard XML database distributed with UDUNITS-2.

The database defines:

- units
- prefixes
- aliases
- physical constants
- reference time units

When the database is loaded, UDUNITS-2 builds an in-memory unit system that applications use throughout their lifetime.

## Units Belong to a Unit System

Every unit is associated with the unit system from which it was created.

For example:

```text
XML Database
      │
      ▼
 Unit System
      │
      ├── meter
      ├── foot
      ├── second
      ├── kilogram
      └── ...
```

Units created by parsing unit expressions belong to the same unit system.

This ensures that all units being compared or converted are interpreted consistently.

## Compatibility Between Unit Systems

Units can only be combined or converted when they originate from the same unit system.

For example:

```text
Unit System A
    ├── meter
    └── foot

Unit System B
    ├── meter
    └── custom_length
```

Even if two unit systems define units with the same names or symbols, those units belong to different unit systems and are treated independently.

Separate unit systems prevent accidental comparisons between units defined under different assumptions or custom databases.

## Custom Unit Systems

Although most applications use the standard XML database, UDUNITS-2 also supports custom XML databases for creating unit systems.

Custom XML databases can:

- add new units,
- define aliases,
- introduce prefixes,
- modify existing definitions, or
- support domain-specific measurements.

This flexibility allows organizations and scientific communities to extend the standard unit vocabulary without modifying the library itself.

## Unit Systems in the C API

In the C API, a unit system is represented by the opaque `ut_system` type.
Most applications load a unit system during initialization, reuse it to parse and manipulate units throughout execution, and release it during shutdown.

## See Also

- **[Concepts: Units](concepts-units.html)**
- **[Concepts: Unit Expressions](concepts-unit-expressions.html)**
