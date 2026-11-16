---
title: Converters
sidebar: sidebar
toc: true
permalink: concepts-converters.html
---

A **converter** transforms numeric values between compatible units.

Converters are the final stage of the UDUNITS-2 workflow.

After unit expressions have been parsed and their dimensions verified as compatible, UDUNITS-2 creates a converter that performs the mathematical transformation between them.

Applications typically create a converter once and reuse it to convert many values efficiently.

## Conversion Compatibility

Converters can only be created between units that represent the same physical dimension.
Attempts to convert incompatible units fail because no mathematical relationship exists between their dimensions.

| From      | To     | Converter? |
| --------- | ------ | ---------- |
| meter     | foot   | ✓          |
| kilometer | mile   | ✓          |
| kilogram  | pound  | ✓          |
| meter     | second | ✗          |
| kilogram  | kelvin | ✗          |
| watt      | meter  | ✗          |

## Linear and Offset Conversions

Many conversions involve only a scaling factor.

Examples include:

```text
meters     ↔ feet

kilometers ↔ miles

hours      ↔ seconds
```

Other conversions require both a scale and an offset.

Examples include:

```text
degrees Celsius ↔ degrees Fahrenheit

degrees Celsius ↔ kelvin
```

UDUNITS-2 automatically applies the appropriate conversion formula based on the units involved.

## Reusing Converters

Creating a converter requires UDUNITS-2 to analyze the source and destination units.
Once created, the same converter can efficiently convert many values, making it well suited for processing large datasets.

## Converters in the C API

In the C API, converters are represented by the opaque `cv_converter` type.
Applications obtain a converter from two compatible `ut_unit` objects, use it to convert one or more values, and release it when it is no longer needed.

## See Also

- **[Concepts: Dimensions](concepts-dimensions.html)**
- **[Concepts: Unit Expressions](concepts-unit-expressions.html)**
