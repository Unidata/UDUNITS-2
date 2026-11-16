---
title: Data Structures
sidebar: sidebar
toc: true
permalink: data-structures.html
---

UDUNITS-2 exposes a small set of core data structures through its public API.
These structures are intentionally defined as **opaque types**, meaning their
internal representation is hidden from user code.

This design provides:

- **Encapsulation** — internal implementation details remain private
- **ABI stability** — applications remain compatible across library versions
- **Implementation flexibility** — internal data structures can evolve without
  breaking user code
- **Controlled object lifecycle** — objects are created and destroyed exclusively
  through API functions

Because these types are opaque, they are accessed only through pointers and
manipulated exclusively using [UDUNITS-2 API](c-api-overview.html#api-organization) functions.

## Core Object Types

The following data structures are declared in the public header
[`udunits2.h`](https://github.com/Unidata/UDUNITS-2/blob/main/lib/udunits2.h)
as incomplete struct types:

| Type | Description |
| ------ | ------------- |
| [`ut_system`](ut-system.html) | Represents a unit system containing defined units and relationships |
| [`ut_unit`](ut-unit.html) | Represents an individual unit definition belonging to a unit system |
| [`cv_converter`](cv-converter.html) | Represents a converter capable of transforming numeric values between compatible units |

These types **cannot be instantiated or modified directly**. Instead, they are
created, queried, and destroyed exclusively through the UDUNITS-2 API.

## Structural Characteristics

- **Opaque struct types**  
  [`ut_system`](ut-system.html), [`ut_unit`](ut-unit.html), and
  [`cv_converter`](cv-converter.html) are declared but not defined in the public
  header. Their internal fields are intentionally hidden.

- **Pointer-based usage**  
  Objects are created, returned, and passed through the API using pointers.

- **API-mediated interaction**  
  Objects are created and used exclusively through [UDUNITS-2 API](c-api-overview.html#api-organization) functions rather
  than direct struct manipulation.

## Object Relationships

The three opaque types form a simple dependency structure:

```text
ut_system
└── ut_unit
└── cv_converter
```

- A **unit system** defines available units and relationships between them
- A **unit** is defined within a unit system
- A **converter** transforms values between compatible units
