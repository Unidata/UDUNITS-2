---
title: Unit Arithmetic API
sidebar: sidebar
toc: true
permalink: unit-arithmetic-api.html
---

Functions for constructing derived units through mathematical operations.

All functions in this API return new unit instances rather than modifying
existing ones.

## Functions

| Function | Description |
| ---------- | ------------- |
| [`ut_divide()`](ut-divide.html) | Returns a new [`ut_unit`](ut-unit.html) representing the quotient of two units in the same [`ut_system`](ut-system.html) |
| [`ut_invert()`](ut-invert.html) | Returns a new [`ut_unit`](ut-unit.html) representing the multiplicative inverse of a unit |
| [`ut_log()`](ut-log.html) | Returns a logarithmic [`ut_unit`](ut-unit.html) defined relative to a reference unit in the same [`ut_system`](ut-system.html) |
| [`ut_multiply()`](ut-multiply.html) | Returns a new [`ut_unit`](ut-unit.html) representing the product of two units in the same [`ut_system`](ut-system.html) |
| [`ut_offset()`](ut-offset.html) | Returns a new [`ut_unit`](ut-unit.html) defined by applying an additive offset to an existing unit |
| [`ut_offset_by_time()`](ut-offset-by-time.html) | Returns a new time-offset [`ut_unit`](ut-unit.html) relative to a temporal origin in the same [`ut_system`](ut-system.html) |
| [`ut_raise()`](ut-raise.html) | Returns a new [`ut_unit`](ut-unit.html) raised to a specified power within the same [`ut_system`](ut-system.html) |
| [`ut_root()`](ut-root.html) | Returns a new [`ut_unit`](ut-unit.html) representing the specified root of a unit within the same [`ut_system`](ut-system.html) |
| [`ut_scale()`](ut-scale.html) | Returns a new scaled [`ut_unit`](ut-unit.html) defined by applying a constant factor within the same [`ut_system`](ut-system.html) |

## Usage Patterns

```text
(1) Obtain ut_unit objects
    from one of the following:
    • ut_parse()
    • ut_get_unit_by_name()
    • ut_get_unit_by_symbol()
                │
                ▼
(2) ╔══════════════════════════════════╗
    ║ Construct derived ut_unit        ║
    ║ via arithmetic operations        ║
    ║ • ut_multiply()                  ║
    ║ • ut_divide()                    ║
    ║ • ut_invert()                    ║
    ║ • ut_raise()                     ║
    ║ • ut_root()                      ║
    ╚══════════════════════════════════╝
                │
                ▼
(3) Intermediate ut_unit objects
    represent dimensional structure
                │
                ▼
(4) ╔══════════════════════════════════╗
    ║ Apply scale or transformation    ║
    ║ to ut_unit                       ║
    ║ • ut_scale()                     ║
    ║ • ut_offset()                    ║
    ║ • ut_offset_by_time()            ║
    ║ • ut_log()                       ║
    ╚══════════════════════════════════╝
                │
                ▼
(5) Derived ut_unit
    (normalized dimensional form)
                │
                ▼
(6) (Optional) obtain cv_converter
    between compatible ut_unit objects
    • ut_get_converter()
                │
                ▼
(7) Use derived ut_unit or cv_converter
    in application workflow
    • conversion
    • formatting
    • comparison
```

1. **Obtain [`ut_unit`](ut-unit.html) objects**

   Operations begin with one or more existing [`ut_unit`](ut-unit.html) objects within a
   [`ut_system`](ut-system.html).

   [`ut_unit`](ut-unit.html) objects may originate from parsing a unit expression string or
   retrieving predefined units stored in the [`ut_system`](ut-system.html).

   Typical entry points include:

   - [`ut_parse()`](ut-parse.html)
   - [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
   - [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)

   These functions produce structured [`ut_unit`](ut-unit.html) representations that can be
   combined using arithmetic operations.

2. **Construct derived [`ut_unit`](ut-unit.html) objects**

   Arithmetic functions operate on compatible [`ut_unit`](ut-unit.html) objects belonging to
   the same [`ut_system`](ut-system.html) to produce new derived units.

   - [`ut_multiply()`](ut-multiply.html)
   - [`ut_divide()`](ut-divide.html)
   - [`ut_invert()`](ut-invert.html)
   - [`ut_raise()`](ut-raise.html)
   - [`ut_root()`](ut-root.html)

   Each function returns a newly allocated [`ut_unit`](ut-unit.html) representing the resulting
   dimensional structure within the same [`ut_system`](ut-system.html).

3. **Intermediate dimensional representation**

   The resulting [`ut_unit`](ut-unit.html) objects encode dimensional relationships independent
   of textual representation.

   Intermediate [`ut_unit`](ut-unit.html) objects may represent partially constructed derived
   units and can be reused in additional arithmetic operations within the same
   [`ut_system`](ut-system.html).

4. **Apply transformations to [`ut_unit`](ut-unit.html)**

   Transformation functions modify magnitude or reference basis while preserving
   dimensional compatibility within the same [`ut_system`](ut-system.html).

   - [`ut_scale()`](ut-scale.html)
   - [`ut_offset()`](ut-offset.html)
   - [`ut_offset_by_time()`](ut-offset-by-time.html)
   - [`ut_log()`](ut-log.html)

   These operations produce new [`ut_unit`](ut-unit.html) objects suitable for use in conversion
   or comparison workflows.

5. **Derived [`ut_unit`](ut-unit.html) available**

   The resulting [`ut_unit`](ut-unit.html) represents a fully constructed derived unit defined
   within the same [`ut_system`](ut-system.html).

   This structured representation supports consistent use across parsing,
   conversion, formatting, and comparison workflows.

6. **Create [`cv_converter`](cv-converter.html) (optional)**

   When numeric transformation between compatible units is required,
   [`ut_get_converter()`](ut-get-converter.html) produces a `cv_converter`
   describing the relationship between two compatible [`ut_unit`](ut-unit.html) objects within
   the same [`ut_system`](ut-system.html).

   The [`cv_converter`](cv-converter.html) enables transformation of numeric values while preserving
   dimensional consistency.

7. **Use derived [`ut_unit`](ut-unit.html) or [`cv_converter`](cv-converter.html)**

   The resulting [`ut_unit`](ut-unit.html) or [`cv_converter`](cv-converter.html) can be used in application workflows
   requiring unit-aware computation or representation.

   Typical uses include:

   - numeric value transformation
   - formatting derived unit expressions
   - comparing dimensional compatibility
   - integrating derived units into application logic

## See Also

- [Unit Creation & Destruction API](unit-creation-destruction-api.html)  
- [Unit Conversion API](unit-conversion-api.html)
