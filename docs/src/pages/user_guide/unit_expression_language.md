---
title: Unit Expression Language
sidebar: sidebar
toc: true
permalink: unit-expression-language.html
---

UDUNITS-2 unit expressions describe simple and compound units using unit names, symbols, prefixes, operators, exponents, and other supported syntax.

This page provides a user-oriented guide to constructing unit expressions.
For the complete lexical and syntactic specification, see the **[Unit Expression Language Specification](unit-expression-language-specification.html)**.

## Basic Syntax

A simple unit expression consists of a unit name or symbol defined in the units database.

```text
m
meter
s
second
Pa
```

Unit names and symbols are interpreted using the units defined in the loaded units database.
An expression containing an unknown unit cannot be parsed.

Compound unit expressions combine multiple units using multiplication, division, exponents, and parentheses.

```text
m/s
kg m2 s-2
W/m2
kg/(m s2)
```

These expressions can represent quantities such as velocity, energy, pressure, and density.

### Multiplication

Units placed next to each other are multiplied
Whitespace is the most common way to indicate multiplication.

```text
kg m
N m
kg m2 s-2
```

UDUNITS-2 also supports explicit multiplication operators:

```text
kg*m
kg.m
kg-m
kg·m
```

### Division

Use `/` to divide one unit by another.

```text
m/s
kg/m3
J/kg
```

The `per` operator may also be used:

```text
m per s
```

The `per` operator requires surrounding spaces.

Negative exponents provide another way to express division:

```text
m/s
m s-1
```

### Exponents

An exponent indicates a power of a unit.

```text
m2
m3
s-1
```

Explicit exponent operators may also be used:

```text
m^2
m**2
s^-1
```

Unicode superscript characters are supported:

```text
m²
s⁻¹
```

### Parentheses

Parentheses group parts of an expression.

```text
kg/(m s2)
W/(m2 K)
m/(s kg)
```

Parentheses are particularly useful when a denominator contains multiple units.

## Prefixes

A prefix can be combined with a unit name or symbol to create a scaled unit.
Prefixes are written immediately before the unit without spaces.

```text
km
cm
mm
µm
kg
ms
MHz
```

Prefixes can also be used in compound expressions:

```text
km/h
mm/day
µg/m3
kW
```

UDUNITS-2 supports standard SI prefixes
Prefixes are case-sensitive; for example, `m` and `M` represent different prefixes.

The micro prefix is commonly written using `µ`:

```text
µm
µg
µmol
```

The complete list of supported prefixes and their factors is provided in the **[Units Database](units-database.html)**.

## Reference Time Units

Reference time units express a time interval relative to a specified reference date and time.

```text
seconds since 1970-01-01 00:00:00
days since 2000-01-01
hours since 2024-01-01 12:00:00
```

The time unit before `since` must be a valid time unit recognized by UDUNITS-2.

A reference date may optionally include a time of day:

```text
days since 2000-01-01
hours since 2024-01-01 12:00:00
```

Reference times may also include a UTC offset or time-zone identifier:

```text
hours since 2024-01-01 00:00:00 UTC
hours since 2024-01-01 12:00:00 -07:00
```

Reference time units can be converted when their timelines are compatible.
For example:

```text
You have:
hours since 2024-01-01 00:00:00

You want:
seconds since 2024-01-01 00:00:00
```

For more information about reference time expressions and examples, see the **[Unit Expression Language Specification](unit-expression-language-specification.html)**.

## Common Examples

Unit expressions can be combined to describe common physical quantities.

| Quantity        | Expression |
| --------------- | ---------- |
| Length          | `m`        |
| Area            | `m2`       |
| Volume          | `m3`       |
| Velocity        | `m/s`      |
| Acceleration    | `m/s2`     |
| Density         | `kg/m3`    |
| Force           | `kg m/s2`  |
| Energy          | `kg m2/s2` |
| Power           | `kg m2/s3` |
| Pressure        | `Pa`       |
| Specific energy | `J/kg`     |

Numeric values can be written with unit expressions:

```text
10 m/s
1013.25 hPa
```

For additional examples of using UDUNITS-2, see **[Basic Examples](basic-examples.html)**.

## Troubleshooting

These problems occur when a unit expression cannot be parsed or does not represent a valid or compatible unit.

**Error Message:** `Unknown unit`
: **Cause:** The expression contains a unit name or symbol that is not defined in the loaded units database.
: **Solution:** Verify the unit name or symbol and check the [Units Database](units-database.html) for the available units.
For example, `foobar` cannot be parsed unless `foobar` is defined in the loaded units database.
{: .symptom}

**Error Message:** `Invalid unit expression syntax`
: **Cause:** The expression contains malformed operators, missing operands, an incomplete exponent, or incorrect grouping.
: **Solution:** Check the expression for syntax errors.
For example, `m//` contains a repeated division operator, while `m^` contains an incomplete exponent.
{: .symptom}

**Error Message:** `Invalid prefix`
: **Cause:** A prefix is not recognized, is incorrectly capitalized, or does not form a valid prefixed unit.
: **Solution:** Verify that the prefix is supported and correctly capitalized.
For example, `km`, `mm`, `µg`, and `kW` are valid prefixed units.
See [Prefixes](unit-expression-language.html#prefixes).
{: .symptom}

**Error Message:** `Invalid reference time expression`
: **Cause:** A reference time expression does not contain a valid time unit before the reference-time specification.
: **Solution:** Specify a valid time unit followed by a reference date.
For example, `days since 1970-01-01` is valid, while `since 1970-01-01` is missing the base time unit.
{: .symptom}

**Error Message:** `Units are not convertible`
: **Cause:** The two unit expressions represent different dimensions and therefore cannot be converted directly.
: **Solution:** Verify that the source and destination units are dimensionally compatible.
For example, `m` and `ft` are compatible length units, while `m` and `s` represent different dimensions and cannot be converted directly.
{: .symptom}

## Formal Syntax

UDUNITS-2 defines a formal lexical and syntactic language for unit expressions.
This page intentionally provides a user-oriented description rather than reproducing the complete grammar.

For the complete specification, including identifiers, numbers, operators, dates, times, time zones, logarithmic expressions, and grammar productions, see the **[Unit Expression Language Specification](unit-expression-language-specification.html)**.

If an expression is syntactically valid but cannot be converted to another unit, see the documentation for unit conversion and compatibility in the C API Reference.

## See Also

- **[Concepts: Unit Expressions](concepts-unit-expressions.html)**
- **[User Guide: Units Database](units-database.html)**
- **[Getting Started: Basic Examples](basic-examples.html)**
- **[Reference: Unit Expression Language Specification](unit-expression-language-specification.html)**
