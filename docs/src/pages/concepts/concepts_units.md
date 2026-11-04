---
title: Units
sidebar: sidebar
toc: true
permalink: concepts-units.html
---

A **unit** is a standardized way of expressing the magnitude of a physical quantity.
Examples include meters for length, seconds for time, kilograms for mass, and pascals for pressure.

In UDUNITS-2, units are the fundamental building blocks used to describe and convert measurements.
The library recognizes hundreds of predefined units from its **[XML units database](units-database.html)**.
These units can be parsed, combined into **[unit expressions](concepts-unit-expressions.html)**, compared by their **[dimensions](concepts-dimensions.html)**, and used to create **[converters](concepts-converters.html)** for transforming values between compatible units.

## Unit Categories

UDUNITS-2 supports several categories of units.

### Base Units

A **base unit** defines a fundamental physical quantity from which other units are derived.

Examples include:

| Quantity | Unit | Symbol |
| -------- | ---- | ------ |
| Length | meter | `m` |
| Mass | kilogram | `kg` |
| Time | second | `s` |
| Electric current | ampere | `A` |
| Thermodynamic temperature | kelvin | `K` |
| Amount of substance | mole | `mol` |
| Luminous intensity | candela | `cd` |

Base units provide the foundation for defining all other units.

### Derived Units

A **derived unit** is defined by combining one or more other units through multiplication, division, or exponentiation.

Examples include:

| Unit | Equivalent Expression |
| ------ | ------------------------ |
| newton (`N`) | `kg·m/s²` |
| joule (`J`) | `N·m` |
| watt (`W`) | `J/s` |
| pascal (`Pa`) | `N/m²` |

Derived units may have their own names and symbols or be written as combinations of other units.

### Dimensionless Units

Some units represent pure numbers rather than measurable physical quantities.

Examples include:

- `1`
- radian (`rad`)
- steradian (`sr`)

Dimensionless units are commonly used for ratios, angles, and scaling factors.

### Scaled Units

Many units differ only by a constant scale factor.

For example:

| Unit | Equivalent |
| ------ | ------------ |
| kilometer | 1000 meters |
| centimeter | 0.01 meters |
| inch | 0.0254 meters |
| hour | 3600 seconds |

UDUNITS-2 automatically handles these scale relationships during conversions.

### Offset Units

Some units differ from one another by both a scale and an offset.

For example:

- degrees Celsius
- degrees Fahrenheit

Unlike simple scaled units, offset units require both scaling and an offset during conversion.

## Unit Names and Symbols

Most units have both a **name** and a **symbol**.

Examples include:

| Name | Symbol |
| ------ | -------- |
| meter | `m` |
| second | `s` |
| kilogram | `kg` |
| pascal | `Pa` |

UDUNITS-2 recognizes both forms when parsing unit expressions.

The XML units database also defines aliases and alternative spellings for many units.

## Compound Units

Units can be combined through multiplication, division, and exponents to represent more complex quantities.

Examples include:

```text
m/s
kg/m3
N·m
W/m2
m2
s-1
```

These combinations are called **unit expressions**.
See **[Unit Expressions](concepts-unit-expressions.html)** for an overview of the syntax and additional examples.

## Unit Compatibility

Units that represent the same physical dimension are considered **compatible**.

For example:

- meters and feet are compatible.
- kilograms and pounds are compatible.
- hours and seconds are compatible.

Units representing different physical quantities are not compatible.

For example:

- meters and seconds
- kilograms and kelvin
- pascals and volts

Compatibility is determined by a unit's **dimension**, not by its name or symbol.

## Units in the C API

Within the C API, parsed units are represented by the opaque `ut_unit` type.
Applications obtain `ut_unit` objects by parsing unit expressions or creating units programmatically, then use them for comparison, arithmetic, and conversion.
When no longer needed, applications are responsible for releasing these objects.

## See Also

- **[Concepts: Dimensions](concepts-dimensions.html)**
- **[Concepts: Unit Expressions](concepts-unit-expressions.html)**
