---
title: utPrint()
sidebar: sidebar
toc: true
permalink: utPrint.html
---

Formats a UDUNITS-1 unit definition into a human-readable string representation.

{% include warning.html content="
**`utPrint()` is part of the UDUNITS-1 legacy compatibility interface.**  

New applications should prefer the UDUNITS-2 formatting functions such as
[`ut_format()`](ut-format.html), which provide improved formatting control,
Unicode support, and clearer separation between unit objects and textual
representations.
"%}

## Prototype

```c
int utPrint(
    const utUnit* unit,
    char* buffer
);
```

## Header

```c
#include <udunits.h>
```

## Description

`utPrint()` writes a textual representation of a UDUNITS-1 `utUnit`
structure into a caller-provided character buffer.

The formatted string represents the unit definition using standard UDUNITS
notation (for example `"m"`, `"kg.m/s^2"`, `"K"`, `"s"`). The function produces
a canonical string representation based on the internal dimensional structure
and scaling relationships of the unit.

The caller must provide a buffer large enough to hold the resulting
`NULL`-terminated string. The function does not perform bounds checking.
Providing an insufficient buffer results in undefined behavior.

The function does not allocate memory and does not modify the input unit.

`utPrint()` is commonly used for:

- displaying unit definitions
- debugging unit transformations
- serializing unit representations to logs or text output
- verifying results of unit parsing or arithmetic operations

## Parameters

| Parameter | Description |
|-----------|-------------|
| `unit` | Pointer to the `utUnit` structure to be formatted |
| `buffer` | Character buffer that receives the formatted unit string |

## Returns

| Return Value | Description |
|--------------|-------------|
| `0` | Unit successfully formatted |
| `-1` | An error occurred |

## Error Handling

Typical error conditions include:

- `unit` is not a valid initialized `utUnit`
- `buffer` is a null pointer
- internal formatting failure

The function does not perform bounds checking on the provided buffer.
Providing insufficient storage results in undefined behavior.

The UDUNITS-1 compatibility interface provides limited diagnostic detail.

## Object Ownership

`utPrint()` does not allocate memory and does not create new unit objects.

The caller retains ownership of all provided objects.

| Memory Allocation | Caller Responsibility |
|------------------|----------------------|
| None | Provide a valid `utUnit` and sufficiently large character buffer |

The output string is written directly into the caller-provided buffer.

## Thread Safety

`utPrint()` does not modify the provided `utUnit` structure.

However, the UDUNITS-1 compatibility layer relies on shared global state and
is not guaranteed to be thread-safe without external synchronization.

Applications requiring stronger thread safety guarantees should use the
UDUNITS-2 API.

## Example

The following example demonstrates how `utPrint()` converts a parsed unit
definition into a human-readable string.

```c
#include <stdio.h>
#include <udunits.h>

int main(void)
{
    utUnit meters;
    char buffer[64];

    if (utScan("m", &meters) != 0) {
        printf("Failed to parse unit\n");
        return 1;
    }

    if (utPrint(&meters, buffer) == 0) {
        printf("Unit representation: %s\n", buffer);
    }
    else {
        printf("Failed to format unit\n");
    }

    return 0;
}
```

## See Also

- [`utScan()`](utScan.html)
- [`utCopy()`](utCopy.html)
- [`utCompare()`](utcompare.html)
- [`utFree()`](utFree.html)
- [`utConvert()`](utConvert.html)
- [`utFormat()`](ut-format.html)
- [UDUNITS-2 Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [UDUNITS-2 Unit Systems API](unit-systems-api.html)
