---
title: Unit Creation & Destruction API
sidebar: sidebar
toc: true
permalink: unit-creation-destruction-api.html
---

Functions for creating, duplicating, and freeing unit objects.

Units follow a strict lifecycle in which they are created or obtained, used in
computations or conversions, and then explicitly freed when no longer needed to
avoid memory leaks.

## Functions

| Function | Description |
| ---------- | ------------- |
| [`ut_clone()`](ut-clone.html) | Create a copy of an existing [`ut_unit`](ut-unit.html) |
| [`ut_free()`](ut-free.html) | Release memory associated with a [`ut_unit`](ut-unit.html) |
| [`ut_get_dimensionless_unit_one()`](ut-get-dimensionless-unit-one.html) | Retrieve the canonical [`ut_unit`](ut-unit.html) representing the dimensionless quantity one |
| [`ut_new_base_unit()`](ut-new-base-unit.html) | Create a new base [`ut_unit`](ut-unit.html) within a unit system |
| [`ut_new_dimensionless_unit()`](ut-new-dimensionless-unit.html) | Create a new dimensionless [`ut_unit`](ut-unit.html) with no associated physical dimension |

## Usage Patterns

The **Unit Creation & Destruction API** operates as a progression from
**`ut_system` context → creation or parsing of [`ut_unit`](ut-unit.html) objects → optional
duplication → application of unit arithmetic or conversion relationships →
explicit resource cleanup**, enabling controlled lifecycle management of unit
representations used throughout parsing, computation, and conversion workflows.

```text
(1) ╔════════════════════════════════════╗
    ║ Create or obtain ut_unit           ║
    ║ • ut_new_base_unit()               ║
    ║ • ut_new_dimensionless_unit()      ║
    ║ • ut_get_dimensionless_unit_one()  ║
    ║ • ut_parse()                       ║
    ╚════════════════════════════════════╝
                      │
                      ▼
(2) ╔════════════════════════════════════╗
    ║ Duplicate unit (optional)          ║
    ║ • ut_clone()                       ║
    ╚════════════════════════════════════╝
                      │
                      ▼
(3) Apply unit logic
    • ut_multiply()
    • ut_divide()
    • ut_scale()
                      │
                      ▼
(4) Create converter (optional)
    • ut_get_converter()
                      │
                      ▼
(5) ╔════════════════════════════════════╗
    ║ Free units when no longer needed   ║
    ║ • ut_free()                        ║
    ╚════════════════════════════════════╝
```

1. **Create or obtain [`ut_unit`](ut-unit.html)**

   Unit creation begins by defining new units within a unit system or obtaining
   existing ones.   Functions such as [`ut_new_base_unit()`](ut-new-base-unit.html), [`ut_new_dimensionless_unit()`](ut-new-dimensionless-unit.html),
   and  [`ut_get_dimensionless_unit_one()`](ut-get-dimensionless-unit-one.html) create canonical or custom units
   associated with a [`ut_system`](ut-system.html).

   Alternatively, [`ut_parse()`](ut-parse.html) converts a unit expression string into a
   corresponding [`ut_unit`](ut-unit.html).

2. **Duplicate unit (optional)**

   `ut_clone()` creates an independent copy of an existing [`ut_unit`](ut-unit.html). Cloning
   is useful when the original unit must remain unchanged while a modified
   version is used in subsequent operations.

3. **Apply unit logic**

   Units may be combined or transformed using arithmetic operations such as  
   [`ut_multiply()`](ut-multiply.html) and [`ut_divide()`](ut-divide.html).  These operations produce derived
   [`ut_unit`](ut-unit.html) instances representing compound quantities or scaled relationships.

4. **Create converter (optional)**

   [`ut_get_converter()`](ut-get-converter.html) establishes a transformation between compatible units.
   This produces a [`cv_converter`](cv-converter.html) that can be used to convert numeric values
   between the corresponding [`ut_unit`](ut-unit.html) definitions.

5. **Free units when no longer needed**

   [`ut_free()`](ut-free.html) releases memory associated with a [`ut_unit`](ut-unit.html) once it is no longer
   required.  Explicit deallocation ensures proper lifecycle management of [`ut_unit`](ut-unit.html)
   objects created within a [`ut_system`](ut-system.html).

## See Also

- [Unit Arithmetic API](unit-arithmetic-api.html)  
- [Unit Conversion API](unit-conversion-api.html)  
- [Unit Systems API](unit-systems-api.html)  
