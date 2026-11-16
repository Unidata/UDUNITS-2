---
title: UDUNITS-1 Legacy API
sidebar: sidebar
toc: true
permalink: udunits-1-legacy-api.html
---

The **UDUNITS-1 Legacy API** provides backward-compatibility functions
retained for applications originally written against the UDUNITS-1
interface.

These functions internally adapt legacy workflows to the UDUNITS-2
object model, which is based on [`ut_system`](ut-system.html),
[`ut_unit`](ut-unit.html), and [`cv_converter`](cv-converter.md) objects.

Because these functions predate the explicit [`ut_system`](ut-system.html) object
model, system management is handled internally.

{% include note.html content="
**New applications should prefer the [UDUNITS-2 API](c-api-overview.html#api-organization) directly. Legacy
functions are primarily intended to support incremental migration of
existing codebases.**
"%}

## Functions

| Function | Description |
| ---------- | ------------- |
| [`utConvert()`](utConvert.html) | Computes slope and intercept needed to convert numeric values between compatible units |
| [`utCopy()`](utCopy.html) | Creates a copy of a legacy unit structure |
| [`utFree()`](utFree.html) | Releases resources associated with a legacy unit structure |
| [`utIsTime()`](utIsTime.html) | Determines whether a parsed unit represents a time quantity |
| [`utPrint()`](utPrint.html) | Formats a legacy unit structure as a string representation |
| [`utScan()`](utScan.html) | Parses a unit specification string into a legacy unit structure |

## Usage Patterns

Legacy workflows generally follow the pattern:

The **UDUNITS-1 Legacy API** operates as a progression from
**string expressions → legacy unit structures → compatibility validation
→ conversion coefficients → formatted output**, enabling older
applications to continue functioning while internally leveraging
UDUNITS-2 functionality.

For example:

``` text
(1) Input unit expressions (strings)
    "m"
    "km"
                │
                ▼
(2) ╔══════════════════════════════╗
    ║ Parse unit specification     ║
    ║ using legacy interface       ║
    ║ • utScan()                   ║
    ╚══════════════════════════════╝
                │
                ▼
(3) Legacy unit structures created
                │
                ├─────────────────────────────┐
                ▼                             ▼
(4) ╔══════════════════════════════╗   ╔════════════════════════════╗
    ║ Inspect unit characteristics ║   ║ Duplicate unit structure   ║
    ║ • utIsTime()                 ║   ║ • utCopy()                 ║
    ╚══════════════════════════════╝   ╚════════════════════════════╝
                │
                ▼
(5) ╔══════════════════════════════╗
    ║ Determine conversion         ║
    ║ relationship                 ║
    ║ • utConvert()                ║
    ╚══════════════════════════════╝
                │
                ▼
(6) Conversion coefficients
    slope, intercept
                │
                ▼
(7) ╔══════════════════════════════╗
    ║ Format unit representation   ║
    ║ • utPrint()                  ║
    ╚══════════════════════════════╝
                │
                ▼
(8) Output formatted unit string
                │
                ▼
(9) ╔══════════════════════════════╗
    ║ Release legacy resources     ║
    ║ • utFree()                   ║
    ╚══════════════════════════════╝
```

1. **Input unit expressions**

   Operations begin with unit expressions represented as strings, typically
   originating from application input, configuration data, or external sources.

   Examples:

   - `"m"`
   - `"km"`
   - `"kg·m^2/s^2"`

   These textual representations describe the dimensional structure of the unit.

2. **Parse unit specification using [`utScan()`](ut-scan.html)**

   The unit expression is interpreted by:

   - [`utScan()`](ut-scan.html)

   `utScan()` parses the string and produces a legacy unit structure that internally
   corresponds to a UDUNITS-2 `ut_unit`.

   This structure encodes the dimensional relationships of the unit independent
   of its textual representation.

3. **Legacy unit structure available**

   The resulting structure provides a normalized internal representation of the
   unit, capturing dimensional composition and scaling relationships.

   This representation enables compatibility evaluation and conversion operations.

4. **Inspect or duplicate unit structures**

   Applications may evaluate characteristics of the parsed unit or create copies
   for reuse in subsequent operations.

   - [`utIsTime()`](ut-is-time.html) determines whether the unit represents a
     temporal quantity
   - [`utCopy()`](ut-copy.html) duplicates an existing unit structure

   These operations support validation, reuse, and preservation of original unit
   definitions.

5. **Determine conversion relationship using [`utConvert()`](ut-convert.html)**

   Compatibility between units is evaluated implicitly when conversion is requested.

   - [`utConvert()`](ut-convert.html)

   `utConvert()` computes linear transformation coefficients describing the relationship
   between two compatible unit structures.

   The transformation is expressed as:

    ```c
    y = slope * x + intercept
    ```

    These coefficients correspond internally to relationships represented by
    [`cv_converter`](cv-converter.html) objects in UDUNITS-2 [Unit Conversion API](unit-conversion-api.html).

6. **Conversion coefficients available**

    The computed coefficients define the mathematical transformation required to
    convert numeric values between compatible units.

    These values may be applied directly within application logic.

7. **Format unit representation using [`utPrint()`](ut-print.html)**

    Legacy unit structures may be converted into standardized string representations.

    - [`utPrint()`](ut-print.html)

    Formatting behavior corresponds conceptually to [`ut_format()`](ut-format.html)
    in the [Unit Parsing & Formatting API](unit-parsing-formatting-api.html).

    Formatted output may be used for display, logging, or serialization.

8. **Output formatted unit string**

    The resulting string representation reflects the normalized dimensional structure
    of the unit.

    This output may differ from the original input expression while preserving
    dimensional equivalence.

9. **Free legacy unit structure using [`utFree()`](ut-free.html)**

    Legacy unit structures should be released when no longer needed.

    - [`utFree()`](ut-free.html)

    Explicit resource cleanup ensures proper lifecycle management consistent with
    memory handling expectations of legacy applications.

## Migration To UDUNITS-2 API

The UDUNITS-1 legacy interface maps directly to the UDUNITS-2 object model,
which is based on `ut_system`, `ut_unit`, and `cv_converter` objects.

Applications using the UDUNITS-1 interface can typically migrate incrementally
by replacing legacy functions with their UDUNITS-2 equivalents.

| UDUNITS-1 Concept | UDUNITS-2 Equivalent |
| ------------------- | --------------------- |
| legacy unit structure | [`ut_unit`](ut-unit.html) |
| implicit system context | [`ut_system`](ut-system.html) |
| conversion slope/intercept | [`cv_converter`](cv-converter.html) |
| [`utConvert()`](ut-convert.html) | [`ut_get_converter()`](ut-get-converter.html) |
| [`utCopy()`](ut-copy.html) | [`ut_clone()`](ut-clone.html) |
| [`utFree()`](ut-free.html) | [`ut_free()`](ut-free.html) |
| [`utPrint()`](ut-print.html) | [`ut_format()`](ut-format.html) |
| [`utScan()`](ut-scan.html) | [`ut_parse()`](ut-parse.html) |

Migrating to the UDUNITS-2 API provides:

- explicit [`ut_system`](ut-system.html) management
- improved dimensional consistency validation through explicit compatibility
  checks in the [Unit Conversion API](unit-conversion-api.html) and dimensional
  operations in the [Unit Arithmetic API](unit-arithmetic-api.html)
- extended [unit arithmetic](unit-arithmetic-api.html) capabilities
- improved [error handling](error-handling-api.html) configuration
- clearer [object lifecycle](unit-creation-destruction-api.html) semantics

## See Also

- [UDUNITS-2 Unit Creation & Destruction API](unit-creation-destruction-api.html)
- [UDUNITS-2 Unit Arithmetic API](unit-arithmetic-api.html)
- [UDUNITS-2 Unit Systems API](unit-systems-api.html)
- [UDUNITS-2 Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [UDUNITS-2 Unit Conversion API](unit-conversion-api.html)
- [UDUNITS-2 Error Handling API](error-handling-api.html)
