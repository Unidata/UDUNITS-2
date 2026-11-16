---
title: Unit Parsing & Formatting API
sidebar: sidebar
toc: true
permalink: unit-parsing-formatting-api.html
---

Functions for converting between textual representations and unit objects.  

Parsing operations require a valid unit system (see [`ut_read_xml()`](ut-read-xml.html)).

## Functions

| Function | Description |
| -------- | ----------- |
| [`ut_format()`](ut-format.html) | Formats a [`ut_unit`](ut-unit.html) as a string for display or storage |
| [`ut_parse()`](ut-parse.html) | Parses a unit expression string in a [`ut_system`](ut-system.html) into a [`ut_unit`](ut-unit.html) |
| [`ut_trim()`](ut-trim.html) | Normalizes a [`ut_unit`](ut-unit.html) representation for consistent formatting or conversion |

## Usage Patterns

The **Unit Parsing & Formatting API** operates as a progression from **string →
`ut_unit` (within a `ut_system`) → arithmetic operations → optional
`cv_converter` → normalized [`ut_unit`](ut-unit.html) → formatted string**, enabling reliable
transformation between textual unit expressions and structured unit representations.

```text
(1) Input unit expression (string)
    "kg·m^2/s^2"
                │
                ▼
(2) ╔══════════════════════════════════╗
    ║ Parse string within ut_system    ║
    ║ • ut_parse()                     ║
    ╚══════════════════════════════════╝
                │
                ▼
(3) Obtain ut_unit objects
    (structured representation of units)
    [conceptual form: kg:1, m:2, s:-2]
                │
                ▼
(4) Perform unit arithmetic (optional)
    • ut_multiply()
    • ut_divide()
                │
                ▼
(5) (Optional) obtain cv_converter
    between compatible ut_unit objects
    • ut_get_converter()
                │
                ▼
(6) ╔══════════════════════════════════╗
    ║ Normalize ut_unit representation ║
    ║ • ut_trim()                      ║
    ╚══════════════════════════════════╝
                │
                ▼
(7) ╔══════════════════════════════════╗
    ║ Format ut_unit for output        ║
    ║ • ut_format()                    ║
    ╚══════════════════════════════════╝
                │
                ▼
(8) Output formatted unit string
    "kg·m^2/s^2"
```

1. **Input unit expression (string)**

   Operations begin with a textual unit representation such as `"kg·m²/s²"`.
   At this stage, the unit exists only as a string and has no structural meaning.

2. **Parse string within a [`ut_system`](ut-system.html) using [`ut_parse()`](ut-parse.html)**

   The unit expression is interpreted within a [`ut_system`](ut-system.html), producing a
   [`ut_unit`](ut-unit.html) object that encodes the dimensional structure of the unit.

3. **Obtain structured [`ut_unit`](ut-unit.html) representation**

   The resulting [`ut_unit`](ut-unit.html) provides a normalized internal representation of the
   unit, where base units are associated with exponents that describe their
   dimensional relationships.

4. **Perform arithmetic operations on [`ut_unit`](ut-unit.html) objects (optional)**

   Units may be combined or manipulated using arithmetic functions such as
   [`ut_multiply()`](ut-multiply.html) or [`ut_divide()`](ut-divide.html),
   producing derived [`ut_unit`](ut-unit.html) objects.

5. **Create a [`cv_converter`](cv-converter.html) between compatible [`ut_unit`](ut-unit.html) objects (optional)**

   When conversion between dimensionally compatible units is required,
   [`ut_get_converter()`](ut-get-converter.html) produces a `cv_converter`
   representing the numeric transformation between [`ut_unit`](ut-unit.html) objects.

6. **Normalize [`ut_unit`](ut-unit.html) representation using [`ut_trim()`](ut-trim.html)**

   The internal structure of the [`ut_unit`](ut-unit.html) may be simplified or normalized to
   ensure consistent formatting and comparison behavior.

7. **Format [`ut_unit`](ut-unit.html) as a string using [`ut_format()`](ut-format.html)**

   The normalized [`ut_unit`](ut-unit.html) is converted into a standardized textual
   representation suitable for display, storage, or communication.

## See Also

- [Unit Systems API](unit-systems-api.html)  
- [Unit Arithmetic API](unit-arithmetic-api.html)  
- [Unit Conversion API](unit-conversion-api.html)  
- [Unit Name & Symbols Mappings API](unit-name-symbols-mappings-api.html)  
