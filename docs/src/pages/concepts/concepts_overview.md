---
title: Concepts Overview
sidebar: sidebar
toc: true
permalink: concepts-overview.html
---

UDUNITS-2 provides a consistent framework for describing, interpreting, and converting scientific units.
Whether you are using the `udunits2` command-line utility or the C library, the same core concepts apply.

Understanding these concepts will help you use UDUNITS-2 more effectively and make it easier to understand the User Guide and C API documentation.

## How UDUNITS-2 Processes Units

UDUNITS-2 works by:

1. Loading a **unit system** from an XML database.
2. Parsing **unit expressions** into internal unit objects.
3. Determining the **dimensions** represented by those units.
4. Comparing units for compatibility.
5. Creating **converters** that transform values between compatible units.

The following example illustrates this process when converting meters to feet:

{% include image.html file="../assets/images/concept_overview_example.png" alt="Example: converting meters to feet in UDUNITS-2" %}

## Core Concepts

**[Units](concepts-units.html)**
: A unit is a standard measurement of a physical quantity, such as a meter, kilogram, second, or pascal.

**[Dimensions](concepts-dimensions.html)**
: A dimension describes the physical quantity represented by a unit, independent of its scale.
Dimensions determine whether two units are compatible for conversion.

**[Unit Systems](concepts-unit-systems.html)**
: A unit system is a collection of unit definitions loaded from the UDUNITS XML database.
It provides the vocabulary used to interpret unit expressions.

**[Unit Expressions](concepts-unit-expressions.html)**
: A unit expression is the textual representation of a unit, such as `m`, `m/s`, or `days since 1970-01-01`.
UDUNITS-2 parses these expressions into internal unit objects.

**[Converters](concepts-converters.html)**
: A converter transforms numeric values between two compatible units.
Once created, a converter can be reused efficiently to convert many values.

## How the Concepts Relate

The following diagram shows how the core concepts build upon one another.

{% include image.html file="../assets/images/concept_overview_workflow.png" alt="UDUNITS-2 Workflow" %}

Understanding this workflow provides the foundation for the remaining pages in this section, each of which explores one concept in greater detail.

## Next Steps

The remainder of this section explores each concept in greater detail:

- **[Units](concepts-units.html)**: Physical measurements and unit definitions
- **[Dimensions](concepts-dimensions.html)**: Physical compatibility between units
- **[Unit Systems](concepts-unit-systems.html)**: Collections of unit definitions
- **[Unit Expressions](concepts-unit-expressions.html)**: How units are represented as text
- **[Converters](concepts-converters.html)**: Converting values between compatible units
