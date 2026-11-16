---
title: ut_unit
sidebar: sidebar
toc: true
permalink: ut-unit.html
---


The `ut_unit` object represents a **physical unit definition** within a
`ut_system`. It encodes dimensional structure, scale, and relationships to
other units, enabling parsing, arithmetic operations, compatibility checks,
and conversion workflows.

A `ut_unit` is the fundamental object used throughout UDUNITS for describing
quantities such as meters, seconds, kelvin, or derived expressions like
meters per second or joules per mole.

## Key Characteristics

- Represents the dimensional structure of a physical quantity
- Created within the context of a [`ut_system`](unit-systems-api.html)
- Supports both **base units** and **derived units**
- May include:
  - scaling factors (e.g., kilometers vs meters)
  - offsets (e.g., Celsius vs Kelvin)
  - compound expressions (e.g., m/s, kg·m²/s²)
- Enables dimensional compatibility checks between units
- Supports algebraic manipulation of unit expressions
- Can be formatted into human-readable strings
- Must be explicitly released using [`ut_free()`](ut-free.html) to prevent
  memory leaks
- Immutable after creation; operations return new `ut_unit` objects

## Object Lifecycle

The `ut_unit` lifecycle generally follows a **create → use → free** pattern.

For example:

```text
(1) Obtain unit system context
    ut_system*
                │
                ▼
(2) ╔════════════════════════════════════╗
    ║ Create or retrieve unit definition ║
    ║ • ut_parse()                       ║
    ║ • ut_get_unit_by_name()            ║
    ║ • ut_get_unit_by_symbol()          ║
    ║ • ut_new_base_unit()               ║
    ╚════════════════════════════════════╝
                │
                ▼
(3) ╔════════════════════════════════════╗
    ║ ut_unit representation available   ║
    ╚════════════════════════════════════╝
                │
                ├─────────────────────────────────┐
                │                                 │
                ▼                                 ▼
(4) Unit arithmetic operations        (5) Unit compatibility checks
    • ut_multiply()                       • ut_are_convertible()
    • ut_divide()                         • ut_compare()
    • ut_raise()
    • ut_root()
                │
                ▼
(6) String formatting and display
    • ut_format()
                │
                ▼
(7) Use in conversion workflows
    • ut_get_converter()
                │
                ▼
(8) ╔════════════════════════════════════╗
    ║ Release unit resources             ║
    ║ • ut_free()                        ║
    ╚════════════════════════════════════╝
                │
                ▼
(9) ╔════════════════════════════════════╗
    ║ Unit object destroyed              ║
    ╚════════════════════════════════════╝
```

1. **Obtain unit system context**

   All `ut_unit` objects belong to a [`ut_system`](unit-systems-api.html),
   which defines the namespace of known units and their relationships.

   A system may be created or loaded using:

   - [`ut_new_system()`](ut-new-system.html)
   - [`ut_read_xml()`](ut-read-xml.html)

2. **Create or retrieve unit definition**

   Units may be created or retrieved in several ways:

   - [`ut_parse()`](ut-parse.html)  
     Parses a textual unit expression (e.g., `"m/s"`)

   - [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)  
     Retrieves a unit by canonical name

   - [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)  
     Retrieves a unit by symbol

   - [`ut_new_base_unit()`](ut-new-base-unit.html)  
     Creates a new base unit within a system

3. **`ut_unit` representation available**

   Once created, the unit object internally stores:

   - dimensional exponents
   - scale factors
   - offsets where applicable
   - relationships to other units in the same system

   The object may be reused across multiple operations.

4. **Unit arithmetic operations**

   Units may be combined algebraically to produce derived units:

   - [`ut_multiply()`](ut-multiply.html)
   - [`ut_divide()`](ut-divide.html)
   - [`ut_raise()`](ut-raise.html)
   - [`ut_root()`](ut-root.html)

   Each operation returns a new `ut_unit` object.

5. **Unit compatibility checks**

   Dimensional relationships between units can be examined using:

   - [`ut_are_convertible()`](ut-are-convertible.html)
   - [`ut_compare()`](ut-compare.html)

   Compatibility is required before conversion can occur.

6. **String formatting and display**

   Units may be formatted into readable text using:

   - [`ut_format()`](ut-format.html)

   Formatting options support symbolic or textual output suitable for display,
   logging, or serialization.

7. **Use in conversion workflows**

   Units participate in numeric conversions by serving as inputs to:

   - [`ut_get_converter()`](ut-get-converter.html)

   This produces a [`cv_converter`](cv-converter.html) object capable of transforming numeric values
   between compatible units.

8. **Release unit resources**

   When a unit object is no longer needed, memory must be released using:

   - [`ut_free()`](ut-free.html)

   Each unit created or cloned should be freed to prevent memory leaks.

9. **Unit object destroyed**

   After calling [`ut_free()`](ut-free.html), the pointer becomes invalid and
   should not be used again.

   Additional unit objects may be created as needed using parsing,
   arithmetic, or system lookup functions.

## Relationship To Other Objects

### `ut_system`

A [`ut_system`](ut-system.html) provides the namespace and dimensional basis
for all `ut_unit` objects. Every `ut_unit` belongs to exactly one system,
ensuring consistent relationships, compatibility checks, and arithmetic
operations across units.

### `cv_converter`

A [`cv_converter`](cv-converter.html) represents a compiled mathematical
transformation between two compatible `ut_unit` objects defined within the
same [`ut_system`](ut-system.html).

## See Also

- [Unit Systems API](unit-systems-api.html)
- [Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [Unit Arithmetic API](unit-arithmetic-api.html)
- [Unit Conversion API](unit-conversion-api.html)
- [`cv_converter`](cv-converter.html)
- [`ut_system`](ut-system.html)
