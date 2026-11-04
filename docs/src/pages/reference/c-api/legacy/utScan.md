---
title: utScan()
sidebar: sidebar
toc: true
permalink: utScan.html
---

Parses a textual unit specification into the contents of a UDUNITS-1 `utUnit` structure.

{% include warning.html content="
**`utScan()` is part of the UDUNITS-1 legacy compatibility interface.**  

New applications should use the UDUNITS-2 parsing API, particularly
[`ut_parse()`](ut-parse.html), which provides improved error diagnostics,
thread safety, and explicit control of the unit system context.
"%}

## Prototype

```c
int utScan(
    const char* spec,
    utUnit* unit
);
```

## Header

```c
#include <udunits.h>
```

## Description

`utScan()` parses a textual unit specification and initializes the contents of a
UDUNITS-1 `utUnit` structure representing the corresponding unit definition.

The function interprets the provided string according to the UDUNITS grammar,
supporting base units, derived units, scale factors, offset units, products,
quotients, and exponentiation.

Examples of supported specifications include:

- `"m"`
- `"kg.m/s^2"`
- `"K"`
- `"degC"`
- `"s"`
- `"W/m^2"`
- `"m^3/s"`

The resulting `utUnit` structure contains an internal representation of the unit's
dimensional composition and scaling relationships within the active unit system.

The parsed unit may subsequently be used with other UDUNITS-1 functions such as:

- `utConvert()` for conversion coefficient computation
- `utPrint()` for formatting
- `utCopy()` for duplication
- `utIsTime()` for dimensional classification
- arithmetic operations such as `utMultiply()` or `utDivide()`

The UDUNITS unit system must be initialized prior to calling `utScan()`.
Initialization is typically performed using `utInit()` in the UDUNITS-1 interface
or [`ut_read_xml()`](ut-read-xml.html) in the UDUNITS-2 API.

If parsing fails, the contents of the provided `utUnit` structure are undefined.

## Parameters

| Parameter | Description |
|-----------|-------------|
| `spec` | NULL-terminated string containing a UDUNITS unit specification |
| `unit` | Pointer to a `utUnit` structure that receives the parsed unit definition |

## Returns

| Return Value | Description |
|--------------|-------------|
| `0` | Unit successfully parsed |
| `-1` | Parsing failed or an error occurred |

## Error Handling

`utScan()` returns `-1` if the unit specification cannot be parsed.

Typical error conditions include:

- invalid or unrecognized unit symbol
- malformed unit expression
- use of unsupported syntax
- `spec` is NULL
- `unit` is NULL or does not reference valid storage
- unit system not initialized

Passing invalid pointers results in undefined behavior.

The UDUNITS-1 compatibility interface provides limited diagnostic detail for
parsing failures.

Applications requiring structured error reporting and improved diagnostics
should use the UDUNITS-2 parsing API.

## Object Ownership

`utScan()` initializes the contents of the caller-provided `utUnit` structure.

The function does not allocate memory for the structure itself.

Internal references associated with the unit system may be shared.

| Memory Allocation | Caller Responsibility |
|------------------|----------------------|
| Initializes internal unit representation | Caller provides storage for `utUnit` structure |

The caller should call `utFree()` when the unit is no longer needed in order to
release internal resources.

## Thread Saftey

`utScan()` relies on shared global unit-system state and is not guaranteed to
be thread-safe without external synchronization.

Concurrent parsing operations may require application-level locking when using
the UDUNITS-1 compatibility interface.

Applications requiring reliable thread safety should use the UDUNITS-2 API,
which provides improved isolation of unit-system contexts.

## Example

The following example demonstrates how `utScan()` parses a unit specification
into a `utUnit` structure.

```c
#include <stdio.h>
#include <udunits.h>

int main(void)
{
    utUnit meters;
    utUnit acceleration;

    if (utScan("m", &meters) != 0) {
        printf("Failed to parse unit\n");
        return 1;
    }

    if (utScan("m/s^2", &acceleration) != 0) {
        printf("Failed to parse derived unit\n");
        return 1;
    }

    char buffer[64];

    if (utPrint(&acceleration, buffer) == 0) {
        printf("Parsed unit: %s\n", buffer);
    }

    utFree(&meters);
    utFree(&acceleration);

    return 0;
}
```

## See Also

- [`utPrint()`](utPrint.html)
- [`utCopy()`](utCopy.html)
- [`utFree()`](utFree.html)
- [`utIsTime()`](utIsTime.html)
- [`utConvert()`](utConvert.html)
- [`utMultiply()`](utMultiply.html)
- [`utDivide()`](utDivide.html)
- [`utScale()`](utScale.html)
- [`utOffset()`](utOffset.html)
- [`ut_parse()`](ut-parse.html)
- [UDUNITS-2 Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [UDUNITS-2 Unit Systems API](unit-systems-api.html)
