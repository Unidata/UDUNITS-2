---
title: Unit Expressions
sidebar: sidebar
toc: true
permalink: concepts-unit-expressions.html
---

A **unit expression** is the textual representation of a unit or combination of units.
Examples include simple units such as `m` and `kg`, compound units such as `m/s` and `W/m²`, and reference time units such as `days since 1970-01-01`.

UDUNITS-2 parses these expressions into internal unit objects that can be compared, combined, and converted.

## Simple Unit Expressions

A **simple unit expression** consists of a single unit name or symbol.

Examples include:

```text
m
kg
s
Pa
K
```

These expressions correspond directly to unit definitions contained in the loaded unit system.

## Compound Unit Expressions

A **compound unit expression** combines multiple units using multiplication, division, or exponents.

Examples include:

```text
m/s
kg/m3
N·m
W/m2
m2
s-1
```

Compound expressions represent derived quantities such as velocity, density, energy, and pressure.

## Prefixes

Prefixes modify a unit by applying a fixed scaling factor.

UDUNITS-2 recognizes prefixes as part of a unit expression and automatically applies the corresponding scale factor.
The supported prefixes are defined by the loaded unit system.

## Reference Time Unit Expressions

**Reference time units** combine a time unit with a reference date or time.

Examples include:

```text
seconds since 1970-01-01

hours since 2000-01-01 12:00

days since 1858-11-17
```

Unlike ordinary unit expressions, reference time units combine a time unit with a reference date or time.
They are widely used by netCDF and the CF Conventions to represent dates as numeric values.

UDUNITS-2 can convert between compatible reference time units while preserving the represented point in time.

## Parsing Unit Expressions

When UDUNITS-2 parses a unit expression, it resolves unit names, prefixes, arithmetic operators, and derived units to create an internal representation associated with the current unit system.

If the expression is invalid or references an unknown unit, parsing fails.

## Relationship to Unit Systems

Unit expressions are interpreted within the context of a **unit system**, which defines the available units, prefixes, aliases, and constants.
Different unit systems may therefore interpret the same expression differently.

## See Also

- **[Concepts: Unit Systems](concepts-unit-systems.html)**
- **[User Guide: Unit Expression Language](unit-expression-language.html)**
- **[User Guide: Units Database](units-database.html)**
