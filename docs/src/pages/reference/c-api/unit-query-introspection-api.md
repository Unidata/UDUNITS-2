---
title: Unit Query & Introspection API
sidebar: sidebar
toc: true
permalink: unit-query-introspection-api.html
---

Functions for retrieving unit properties, metadata, and performing comparisons.

Some returned values are system-owned and must not be freed by the caller.

## Functions

| Function | Description |
| ---------- | ------------- |
| [`ut_compare()`](ut-compare.html) | Compares two [`ut_unit`](ut-unit.html) objects for equivalence or ordering relationship |
| [`ut_get_name()`](ut-get-name.html) | Retrieves the canonical name associated with a [`ut_unit`](ut-unit.html) |
| [`ut_get_path_xml()`](ut-get-path-xml.html) | Returns the filesystem path of the XML database used to initialize a [`ut_system`](ut-system.html) |
| [`ut_get_symbol()`](ut-get-symbol.html) | Retrieves the standard symbol associated with a [`ut_unit`](ut-unit.html) |
| [`ut_get_unit_by_name()`](ut-get-unit-by-name.html) | Returns a [`ut_unit`](ut-unit.html) from a [`ut_system`](ut-system.html) based on a canonical unit name |
| [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html) | Returns a [`ut_unit`](ut-unit.html) from a [`ut_system`](ut-system.html) based on a standard unit symbol |
| [`ut_is_dimensionless()`](ut-is-dimensionless.html) | Indicates whether a [`ut_unit`](ut-unit.html) represents a dimensionless quantity |

## Usage Patterns

The **Unit Query & Introspection API** operates as a progression from
**identifier or existing [`ut_unit`](ut-unit.html) → property inspection → identifier lookup
or comparison → formatted output or system metadata**, enabling structured
discovery, validation, and presentation of unit characteristics.

For example:

```text
(1) Input (string): "kg·m^2/s^2"
                │
                ▼
(2) Parse string and construct ut_unit object
    • ut_parse()
                │
                ▼
(3) ut_unit representation available
                │
                ├──────────────────────────────────┐
                │                                  │
                ▼                                  ▼
(4) ╔════════════════════════════╗  (5) ╔══════════════════════════╗
    ║ Property introspection     ║      ║ Lookup by identifier     ║
    ║ • ut_get_name()            ║      ║ • ut_get_unit_by_name()  ║
    ║ • ut_get_symbol()          ║      ║ • ut_get_unit_by_symbol()║
    ╚════════════════════════════╝      ╚══════════════════════════╝
                │
                ▼
(6) ╔═════════════════════════════╗
    ║ Validate or compare units   ║
    ║ • ut_compare()              ║
    ║ • ut_is_dimensionless()     ║
    ╚═════════════════════════════╝
                │
                ▼
(7) ╔═════════════════════════════╗
    ║ Format or display unit      ║
    ║ metadata                    ║
    ║ • ut_format()               ║
    ╚═════════════════════════════╝
                │
                ▼
(8) ╔═════════════════════════════╗
    ║ Access system-level         ║
    ║ metadata                    ║
    ║ • ut_get_path_xml()         ║
    ╚═════════════════════════════╝
                │
                ▼
(9) Output metadata or comparison results
```

1. **Input unit expression**

   Operations begin with a unit expressed as a string.

   Example:

   `"kg·m^2/s^2"`

   This string representation typically originates from user input,
   configuration data, or external files.

2. **Parse string and construct [`ut_unit`](ut-unit.html) object**

   The unit expression is converted into the UDUNITS-2 internal representation.

   - [`ut_parse()`](ut-parse.html) converts the string into a [`ut_unit`](ut-unit.html) within
     a `ut_system`

   This step produces the [`ut_unit`](ut-unit.html) object required for introspection,
   comparison, and formatting operations.

3. **`ut_unit` representation available**

   A [`ut_unit`](ut-unit.html) instance provides a structured representation of the unit’s
   dimensional composition and scaling relationships.

   The [`ut_unit`](ut-unit.html) may represent a base unit, derived unit, or compound expression
   and serves as the primary object used throughout Query and Introspection workflows.

4. **Property introspection**

   Descriptive attributes associated with the [`ut_unit`](ut-unit.html) can be retrieved.

   - [`ut_get_name()`](ut-get-name.html) returns the canonical name associated
     with the `ut_unit`
   - [`ut_get_symbol()`](ut-get-symbol.html) returns the standard symbol
     associated with the `ut_unit`

   These functions support labeling, diagnostics, and display logic.

5. **Lookup by identifier**

   A [`ut_system`](ut-system.html) can resolve identifiers such as canonical names or symbols
   into corresponding [`ut_unit`](ut-unit.html) objects.

   - [`ut_get_unit_by_name()`](ut-get-unit-by-name.html) returns a `ut_unit`
     from a [`ut_system`](ut-system.html) using a canonical name
   - [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html) returns a `ut_unit`
     from a [`ut_system`](ut-system.html) using a standard symbol

   This supports dynamic resolution of unit identifiers to structured objects.

6. **Validate or compare units**

   Structural or relational properties of [`ut_unit`](ut-unit.html) objects can be evaluated.

- [`ut_compare()`](ut-compare.html) compares two [`ut_unit`](ut-unit.html) objects for
  equivalence or ordering relationships
- [`ut_is_dimensionless()`](ut-is-dimensionless.html) determines whether
  a [`ut_unit`](ut-unit.html) has no physical dimensions

   These operations support validation logic and compatibility checks.

1. **Format or display unit metadata**

   A [`ut_unit`](ut-unit.html) can be converted into a readable string representation.

   - [`ut_format()`](ut-format.html) formats a [`ut_unit`](ut-unit.html) into a textual representation

   This is commonly used for logging, reporting, or UI display.

2. **Access system-level metadata**

   Metadata describing the originating [`ut_system`](ut-system.html) can be retrieved.

   - [`ut_get_path_xml()`](ut-get-path-xml.html) returns the filesystem path of
     the XML database used to initialize the `ut_system`

   This supports diagnostics and configuration transparency.

3. **Output metadata or comparison results**

   The application receives structured information derived from `ut_unit`
   and [`ut_system`](ut-system.html) objects, such as:

   - canonical name or symbol
   - formatted unit expression
   - comparison results between [`ut_unit`](ut-unit.html) objects
   - dimensional characteristics
   - unit system configuration details

## See Also

- [Unit Name & Symbols Mappings API](unit-name-symbols-mappings-api.html)
- [Unit Systems API](unit-systems-api.html)
- [Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
