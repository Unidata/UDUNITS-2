---
title: Unit Conversion API
sidebar: sidebar
toc: true
permalink: unit-conversion-api.html
---

Functions for determining compatibility and converting numeric values between units.

Converters encapsulate the logic required to transform numeric values between
compatible units.

## Functions

| Function | Description |
| ---------- | ------------- |
| [`cv_combine()`](cv-combine.html) | Combine two [`cv_converter`](cv-converter.html) objects into a single converter representing sequential transformations |
| [`cv_convert_double()`](cv-convert-double.html) | Convert one double-precision floating-point value using a [`cv_converter`](cv-converter.html) |
| [`cv_convert_doubles()`](cv-convert-doubles.html) | Convert an array of double-precision floating-point values using a [`cv_converter`](cv-converter.html) |
| [`cv_convert_float()`](cv-convert-float.html) | Convert one single-precision floating-point value using a [`cv_converter`](cv-converter.html) |
| [`cv_convert_floats()`](cv-convert-floats.html) | Convert an array of single-precision floating-point values using a [`cv_converter`](cv-converter.html) |
| [`cv_free()`](cv-free.html) | Release resources associated with a [`cv_converter`](cv-converter.html) object |
| [`ut_are_convertible()`](ut-are-convertible.html) | Determine whether two [`ut_unit`](ut-unit.html) objects are dimensionally compatible for conversion |
| [`ut_get_converter()`](ut-get-converter.html) | Create a [`cv_converter`](cv-converter.html) object that transforms numeric values between compatible [`ut_unit`](ut-unit.html) objects |

## Usage Patterns

The **Unit Conversion API** operates as a progression from **unit expressions →
`ut_unit` objects (within a `ut_system`) → compatibility validation →
`cv_converter` creation → numeric transformation → resource cleanup**, enabling
reliable transformation of numeric values between dimensionally compatible units.

For example:

```text
(1) Input unit expressions (strings)
    • "m"
    • "km"
                │
                ▼
(2) Parse unit expressions into ut_unit objects
    (one for the source unit and one for the target unit)
    • ut_parse()
                │
                ▼
(3) ut_unit representations available
                │
                ▼
(4) ╔════════════════════════════════════╗
    ║ Validate and prepare ut_unit       ║ 
    ║ objects                            ║
    ║ • ut_are_convertible()             ║
    ║ • ut_format()  (optional display)  ║
    ╚════════════════════════════════════╝
                │
                ▼
(5) ╔═══════════════════════════════╗
    ║ Create cv_converter object    ║
    ║ • ut_get_converter()          ║
    ╚═══════════════════════════════╝
                │
                ▼
(6) ╔═══════════════════════════════╗
    ║ Optionally combine            ║
    ║ ut_converters objects         ║
    ║ • cv_combine()                ║
    ╚═══════════════════════════════╝
                │
                ▼
(7) ╔═══════════════════════════════╗
    ║ Convert numeric values        ║
    ║ • cv_convert_double()         ║
    ║ • cv_convert_doubles()        ║
    ║ • cv_convert_float()          ║
    ║ • cv_convert_floats()         ║
    ╚═══════════════════════════════╝
                │
                ▼
(8) Converted numeric values
    (double | float | arrays)
                │
                ▼
(9) ╔═══════════════════════════════╗
    ║ Release converter resources   ║
    ║ • cv_free()                   ║
    ╚═══════════════════════════════╝
```

1. **Input unit expressions**

   Operations begin with unit expressions provided as strings, typically
   originating from user input, configuration data, or external files.
   These expressions are interpreted within the context of a [`ut_system`](ut-system.html)
   containing the unit definitions.

   Examples:

   - `"m"`
   - `"km"`
   - `"kg·m/s²"`

   These textual representations identify the source and destination units
   involved in a conversion.

2. **Parse unit expressions**

   Unit expressions are converted into the UDUNITS-2 internal representation.

   - [`ut_parse()`](ut-parse.html) interprets each unit string within a
     [`ut_system`](ut-system.html) and constructs a corresponding [`ut_unit`](ut-unit.html) object.

   Typically, two [`ut_unit`](ut-unit.html) objects are created:

   - one representing the source unit (`ut_unit`)
   - one representing the destination unit (`ut_unit`)

   These objects provide the structured dimensional representation required for
   compatibility checking and conversion.

3. **`ut_unit` representations available**

   Parsed units exist as structured objects independent of their textual form.

   Each [`ut_unit`](ut-unit.html) captures the dimensional composition of the unit relative to
   the associated [`ut_system`](ut-system.html), enabling consistent comparison and transformation
   regardless of how the unit was originally written.

4. **Validate and prepare units**

   Units are validated to ensure they are compatible for conversion and may
   optionally be formatted for display.

   - [`ut_are_convertible()`](ut-are-convertible.html) determines whether two
     [`ut_unit`](ut-unit.html) objects share compatible dimensions
   - [`ut_format()`](ut-format.html) produces a standardized string
     representation of a [`ut_unit`](ut-unit.html) suitable for logging, display, or diagnostics

   Compatibility checking prevents invalid conversions between unrelated
   dimensions defined within the same [`ut_system`](ut-system.html).

5. **Create converter**

   A converter object is created to represent the mathematical relationship
   between the compatible units.

   - [`ut_get_converter()`](ut-get-converter.html) constructs a [`cv_converter`](cv-converter.html)
     object that transforms numeric values between two compatible [`ut_unit`](ut-unit.html)
     objects

   The resulting [`cv_converter`](cv-converter.html) encapsulates the scaling factors and offsets
   required to perform the conversion.

6. **Optionally combine converters**

   Converter objects may be composed to represent sequential transformations.

   - [`cv_combine()`](cv-combine.html) merges two [`cv_converter`](cv-converter.html) objects into a
     single [`cv_converter`](cv-converter.html) representing the combined transformation

   This supports multi-stage transformations or reuse of intermediate
   conversion logic.

7. **Convert numeric values**

   Numeric values are transformed using a [`cv_converter`](cv-converter.html).

   - [`cv_convert_double()`](cv-convert-double.html) converts one
     double-precision floating-point value using a [`cv_converter`](cv-converter.html)
   - [`cv_convert_doubles()`](cv-convert-doubles.html) converts an array of
     double-precision floating-point values using a [`cv_converter`](cv-converter.html)
   - [`cv_convert_float()`](cv-convert-float.html) converts one
     single-precision floating-point value using a [`cv_converter`](cv-converter.html)
   - [`cv_convert_floats()`](cv-convert-floats.html) converts an array of
     single-precision floating-point values using a [`cv_converter`](cv-converter.html)

   These functions apply the mathematical transformation defined by the
   [`cv_converter`](cv-converter.html) to produce values expressed in the destination `ut_unit`.

8. **Converted numeric values**

   The output consists of numeric values expressed in the destination unit.

   Results may be single values or arrays depending on the conversion function used.
   The dimensional meaning of the result corresponds to the destination [`ut_unit`](ut-unit.html)
   defined within the originating [`ut_system`](ut-system.html).

9. **Release converter resources**

   Converter objects are released when no longer needed.

   - [`cv_free()`](cv-free.html) frees resources associated with a
     [`cv_converter`](cv-converter.html) object

   Proper cleanup ensures efficient memory usage and prevents resource leaks
   within the application.

## See Also

- [`Unit Arithmetic API`](unit-arithmetic-api.html)  
- [`Unit Systems API`](unit-systems-api.html)
