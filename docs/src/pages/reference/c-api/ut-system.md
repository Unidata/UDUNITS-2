---
title: ut_system
sidebar: sidebar
toc: true
permalink: ut-system.html
---

The `ut_system` object represents a **unit system context** that defines the
namespace, relationships, and definitions of all units used within UDUNITS-2.

A `ut_system` provides the foundation required to create, retrieve, compare,
and convert [`ut_unit`](ut-unit.html) objects. Every [`ut_unit`](ut-unit.html)
belongs to exactly one `ut_system`, ensuring consistent dimensional relationships
and conversion behavior across units.

Typical systems are created programmatically or loaded from XML definitions
(such as the standard UDUNITS-2 database) using:

- [`ut_new_system()`](ut-new-system.html)
- [`ut_read_xml()`](ut-read-xml.html)

## Key Characteristics

- Defines the **namespace of known units**
- Stores relationships between base and derived units
- Provides context required for parsing textual unit expressions via
  [`ut_parse()`](ut-parse.html)
- Enables lookup of units by **name** or **symbol** using:
  - [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
  - [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)
- Supports creation of custom base units via [`ut_new_base_unit()`](ut-new-base-unit.html)
- Ensures dimensional consistency across all units in the system
- May be initialized from XML unit definition files using [`ut_read_xml()`](ut-read-xml.html)
- Acts as the root object for most UDUNITS-2 workflows
- Must be explicitly released using [`ut_free_system()`](ut-free-system.html)
  to prevent memory leaks
- Shared across multiple [`ut_unit`](ut-unit.html) and
  [`cv_converter`](cv-converter.html) objects

The `ut_system` object is a prerequisite for creating or retrieving [`ut_unit`](ut-unit.html)
objects, which are then used in arithmetic operations (e.g.,
[`ut_multiply()`](ut-multiply.html), [`ut_divide()`](ut-divide.html)) and
conversion workflows via [`ut_get_converter()`](ut-get-converter.html), producing
[`cv_converter`](cv-converter.html) objects.

## Object Lifecycle

The `ut_system` lifecycle generally follows a **create/load → use → free**
pattern.

For example:

```text
(1) ╔══════════════════════════════╗
    ║ Create or load unit system   ║
    ║ • ut_new_system()            ║
    ║ • ut_read_xml()              ║
    ╚══════════════════════════════╝
                │
                ▼
(2) ╔══════════════════════════════╗
    ║ ut_system representation     ║
    ║ available                    ║
    ╚══════════════════════════════╝
                │
                ├──────────────────────────────┐
                │                              │
                ▼                              ▼
(3) Unit definition retrieval      (4) Custom unit creation  
    • ut_get_unit_by_name()            • ut_new_base_unit()  
    • ut_get_unit_by_symbol()         
    • ut_parse()               
                │
                ▼
(5) Unit relationships established
    Units share common dimensional basis
                │
                ▼
(6) Use in unit operations
    • unit arithmetic
    • compatibility checks
    • conversion workflows
                │
                ▼
(7) ╔══════════════════════════════╗
    ║ Release system resources     ║
    ║ • ut_free_system()           ║
    ╚══════════════════════════════╝
                │
                ▼
(8) ╔══════════════════════════════╗
    ║ System object destroyed      ║
    ╚══════════════════════════════╝
```

1. **Create or load unit system**

    A `ut_system` may be created programmatically or loaded from an XML
    definition file.

    - [`ut_new_system()`](ut-new-system.html): Creates an empty `ut_system`

    - [`ut_read_xml()`](ut-read-xml.html): Loads unit definitions from an XML
    file (e.g., the UDUNITS-2 database)

    XML-based systems typically include standard SI units, derived units,
    and commonly used scientific units that produce [`ut_unit`](ut-unit.html) objects.

2. **`ut_system` representation available**

    Once created, the `ut_system` object internally maintains:

    - base unit definitions
    - derived unit relationships
    - mappings between names and symbols
    - dimensional structure metadata

    The system provides the context required for all subsequent operations involving
    [`ut_unit`](ut-unit.html) and [`cv_converter`](cv-converter.html) objects.

3. **Unit definition retrieval**

    Units may be retrieved from a system via [`ut_get_unit_by_name()`](ut-get-unit-by-name.html),
    [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html),
    or [`ut_parse()`](ut-parse.html), each returning a
    [`ut_unit`](ut-unit.html) object.

    These functions return [`ut_unit`](ut-unit.html) objects associated with the
    same `ut_system`.

4. **Custom unit creation**

    New base units may be defined within a system using [`ut_new_base_unit()`](ut-new-base-unit.html).

    Custom units are represented as [`ut_unit`](ut-unit.html) objects whose
    dimensional structure is defined relative to the containing `ut_system`.

5. **Unit relationships established**

    All [`ut_unit`](ut-unit.html) objects within a `ut_system`
    share a consistent dimensional basis, allowing compatibility checks and
    conversions between related units.

    Dimensional compatibility is required before creating a [`cv_converter`](cv-converter.html)
    using [`ut_get_converter()`](ut-get-converter.html).

6. **Use in unit operations**

    A `ut_system` enables unit-based workflows including:

    - parsing and formatting units via [`ut_parse()`](ut-parse.html) and [`ut_format()`](ut-format.html)
    - algebraic manipulation of [`ut_unit`](ut-unit.html) objects via
      [`ut_multiply()`](ut-multiply.html), [`ut_divide()`](ut-divide.html),
      [`ut_raise()`](ut-raise.html), and [`ut_root()`](ut-root.html)
    - dimensional compatibility checks via [`ut_are_convertible()`](ut-are-convertible.html)
    - numeric conversion via [`ut_get_converter()`](ut-get-converter.html),
      producing a [`cv_converter`](cv-converter.html)

    The system ensures that all [`ut_unit`](ut-unit.html) and [`cv_converter`](cv-converter.html)
    objects participating in these operations are internally consistent.

7. **Release system resources**

    When the `ut_system` is no longer needed, memory must be released using
    [`ut_free_system()`](ut-free-system.html).

    Any dependent [`ut_unit`](ut-unit.html) or [`cv_converter`](cv-converter.html)
    objects should be released before destroying the system.

8. **System object destroyed**

    After calling [`ut_free_system()`](ut-free-system.html), the pointer becomes invalid and must not
    be used again.

    A new `ut_system` may be created or loaded as needed to produce
    new [`ut_unit`](ut-unit.html) and [`cv_converter`](cv-converter.html) objects.

## Relationship To Other Objects

### `ut_unit`

A [`ut_unit`](ut-unit.html) represents a physical unit definition within a `ut_system`,
including dimensional structure and scaling relationships.

### `cv_converter`

A [`cv_converter`](cv-converter.html) represents a compiled mathematical
transformation between two compatible [`ut_unit`](ut-unit.html) objects defined
within the same `ut_system`.

## See Also

- [Unit Systems API](unit-systems-api.html)
- [Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [Unit Arithmetic API](unit-arithmetic-api.html)
- [Unit Conversion API](unit-conversion-api.html)
- [`ut_unit`](ut-unit.html)
- [`cv_converter`](cv-converter.html)
