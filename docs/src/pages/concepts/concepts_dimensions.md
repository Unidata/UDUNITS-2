---
title: Dimensions
sidebar: sidebar
toc: true
permalink: concepts-dimensions.html
---

A **dimension** describes the physical quantity represented by a unit, independent of its name, symbol, or scale.

For example, both meters and feet represent **length**, while kilograms and pounds represent **mass**.
Although they differ in scale, they describe the same physical quantity.

UDUNITS-2 uses dimensions to determine whether two units are compatible and can be converted.

## Base Dimensions

| Physical Quantity         | Example Unit    |
| ------------------------- | --------------- |
| Length                    | meter (`m`)     |
| Mass                      | kilogram (`kg`) |
| Time                      | second (`s`)    |
| Electric current          | ampere (`A`)    |
| Thermodynamic temperature | kelvin (`K`)    |
| Amount of substance       | mole (`mol`)    |
| Luminous intensity        | candela (`cd`)  |

**Base dimensions** are the fundamental physical quantities from which all other dimensions are derived.

## Derived Dimensions

| Quantity     | Example Unit |
| ------------ | ------------ |
| Velocity     | `m/s`        |
| Acceleration | `m/s²`       |
| Force        | `N`          |
| Pressure     | `Pa`         |
| Energy       | `J`          |
| Power        | `W`          |

**Derived dimensions** are formed by combining one or more base dimensions.

## Compatibility

Two units are **compatible** if they represent the same dimension.

| Unit 1 | Unit 2 | Compatible |
| ------ | ------ | ---------- |
| `m`    | `ft`   | ✓          |
| `kg`   | `lb`   | ✓          |
| `Pa`   | `psi`  | ✓          |
| `m/s`  | `km/h` | ✓          |
| `m`    | `s`    | ✗          |
| `kg`   | `m`    | ✗          |
| `W`    | `J`    | ✗          |

Only compatible units can be converted.

## Dimensionless Quantities

Dimensionless units represent pure numbers rather than physical quantities.
Examples include `1`, `rad`, and `sr`.

## Dimensions in Practice

When UDUNITS-2 parses a unit expression, it determines the unit's dimension.
That information is used to:

- test compatibility,
- create converters,
- reject invalid conversions.

Applications normally work with units rather than dimensions directly.

## See Also

- **[Concepts: Units](concepts-units.html)**
- **[Concepts: Unit Expressions](concepts-unit-expressions.html)**
- **[Concepts: Converters](concepts-converters.html)**
