---
title: cv_converter
sidebar: sidebar
toc: true
permalink: cv-converter.html
---

The `cv_converter` object represents a compiled conversion transformation between
two compatible [`ut_unit`](ut-unit.html) objects. Once created, a converter can
efficiently transform scalar or array values.

## Key Characteristics

- Created only when two  [`ut_unit`](ut-unit.html) objects are **dimensionally compatible**
- Encapsulates the mathematical transformation required to convert values
  between units
- May represent:
  - **linear scaling** (multiplicative factor)
  - **affine transformation** (multiplicative factor plus additive offset)
- Designed for repeated use to efficiently convert multiple values
- Supports both **scalar** and **array-based** numeric conversions
- Avoids repeated compatibility checks once created
- Must be explicitly released using [`cv_free()`](cv-free.html) to prevent
  memory leaks.

## Object Lifecycle

The `cv_converter` lifecycle follows a simple **create → use → free** pattern.  

For example:

```text
(1) Input units
    ut_unit* from_unit
    ut_unit* to_unit
                │
                ▼
(2) Validate compatibility
    • ut_are_convertible()
                │
                ▼
(3) ╔══════════════════════════════╗
    ║ Create converter object      ║
    ║ • ut_get_converter()         ║
    ╚══════════════════════════════╝
                │
                ▼
(4) ╔══════════════════════════════╗
    ║ cv_converter representation  ║
    ║ available                    ║
    ╚══════════════════════════════╝
                │
                ▼
(5) Value conversion
    • cv_convert_double()
    • cv_convert_float()
    • cv_convert_doubles()
    • cv_convert_floats()
                │
                ▼
(6) Output converted numeric values
                │
                ▼
(7) ╔══════════════════════════════╗
    ║ Release converter resources  ║
    ║ • cv_free()                  ║
    ╚══════════════════════════════╝
                │
                ▼
(8) ╔══════════════════════════════╗
    ║ Converter object destroyed   ║
    ╚══════════════════════════════╝
```

1. **Input units**  

   Two [`ut_unit`](unit-arithmetic-api.html) objects are provided as inputs:
   - `from_unit` — the unit associated with the original numeric values
   - `to_unit` — the unit to which values will be converted  

   Both units must belong to the same [`ut_system`](unit-systems-api.html)
   and represent compatible physical dimensions.

2. **Validate compatibility**  

   Units may be checked for dimensional compatibility using:
   - [`ut_are_convertible()`](ut-are-convertible.html)

   This function determines whether a valid mathematical transformation exists
   between the units. If the units are not compatible (for example, meters vs
   seconds), conversion is not possible.

3. **Create converter object**  

   A `cv_converter` object is created using:
   - [`ut_get_converter()`](ut-get-converter.html)

   The converter encapsulates the transformation required to convert values from
   `from_unit` to `to_unit`. Depending on the relationship between units, the
   transformation may involve:
   - linear scaling (e.g., meters → kilometers)
   - affine transformation with offset (e.g., kelvin → degrees Celsius)  

   If the units are not compatible, this function returns `NULL`.

4. **`cv_converter` representation available**  

   Once created, the converter object can be reused to efficiently transform
   one or more numeric values. The converter internally stores the mathematical
   relationship between the two units, avoiding repeated compatibility checks
   or recomputation.

5. **Value conversion**  

   Numeric values can be converted using scalar or array-based functions:

   - [`cv_convert_double()`](cv-convert-double.html)
   - [`cv_convert_float()`](cv-convert-float.html)
   - [`cv_convert_doubles()`](cv-convert-doubles.html)
   - [`cv_convert_floats()`](cv-convert-floats.html)

   These functions apply the converter transformation to one or more numeric
   values. Array-based functions improve performance when converting datasets,
   time series, or other collections of values.

6. **Output converted numeric values**  

   Conversion functions return numeric values expressed in the target unit
   (`to_unit`). The original input values are not modified unless explicitly
   overwritten by the calling code.

7. **Release converter resources**  

   When the converter is no longer needed, memory should be released using:
   - [`cv_free()`](cv-free.html)

   Each converter created by [`ut_get_converter()`](ut-get-converter.html)
   should be freed to avoid memory leaks.

8. **Converter object destroyed**  

   After calling [`cv_free()`](cv-free.html), the converter pointer becomes
   invalid and should not be used again. If additional conversions are required,
   a new converter must be created using
   [`ut_get_converter()`](ut-get-converter.html).

## Relationship to Other Objects

### `ut_unit`

A [`ut_unit`](ut-unit.html) represents a physical unit definition within a
[`ut_system`](ut-system.html). A `cv_converter` is created from two compatible
[`ut_unit`](ut-unit.html) objects using [`ut_get_converter()`](ut-get-converter.html),
which derives the mathematical transformation required to convert values between
units.

### `ut_system`

Both [`ut_unit`](ut-unit.html) objects used to create a `cv_converter` must belong
to the same [`ut_system`](ut-system.html), which defines the dimensional relationships
that make conversion possible.

## See Also

- [Unit Conversion API](unit-conversion-api.html)
- [Unit Arithmetic API](unit-arithmetic-api.html)
- [Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [Unit Systems API](unit-systems-api.html)
- [`ut_system`](ut-system.html)
- [`ut_unit`](ut-unit.html)
