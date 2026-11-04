---
title: utCopy()
sidebar: sidebar
toc: true
permalink: utCopy.html
---

Copies the contents of one UDUNITS-1 unit definition into another `utUnit` structure.

{% include warning.html content="
**`utCopy()` is part of the UDUNITS-1 legacy compatibility interface.**  

New applications should use the UDUNITS-2 object model and functions such as
[`ut_clone()`](ut-clone.html), which provide improved memory management,
clearer ownership semantics, and better integration with the UDUNITS-2 API.
"%}

## Prototype

```c
int utCopy(
    const utUnit* from,
    utUnit* to
);
```

## Header

```c
#include <udunits.h>
```

## Description

`utCopy()` copies the contents of one UDUNITS-1 `utUnit` structure into another.

The destination structure receives an equivalent unit definition, including
dimensional composition and scaling relationships.

Both pointers must reference valid initialized `utUnit` structures.

This function does not allocate memory for the destination structure; the caller
must provide storage for the target `utUnit`.

Any shared internal references associated with the unit system remain shared.

## Parameters

| Parameter | Description |
|-----------|-------------|
| `from` | Pointer to the source `utUnit` to be copied |
| `to` | Pointer to the destination `utUnit` that receives the copied unit definition |

## Returns

| Return Value | Description |
|--------------|-------------|
| `0` | Unit successfully copied |
| `-1` | An error occurred |

## Error Handling

`utCopy()` returns `-1` if an error occurs.

Typical error conditions include:

- `from` is not a valid initialized `utUnit`
- `to` is a null or invalid pointer

The UDUNITS-1 compatibility interface provides limited diagnostic detail.

Applications requiring detailed error reporting should use the UDUNITS-2 API.

## Object Ownership

`utCopy()` does not allocate new `utUnit` structures. The caller is responsible
for providing storage for both the source and destination structures.

After copying, both structures contain equivalent unit definitions.

| Memory Allocation | Caller Responsibility |
|------------------|----------------------|
| None | Caller allocates storage for `utUnit` structures |

## Thread Saftey

`utCopy()` does not modify the provided `utUnit` structures. However, UDUNITS-1
uses shared global state and is not guaranteed to be thread-safe without
external synchronization.

Applications requiring reliable thread safety should use the UDUNITS-2 API.

## Example

The following example demonstrates how `utCopy()` duplicates a previously
parsed unit definition by copying one `utUnit` object into another.

```c
#include <stdio.h>
#include <udunits.h>

int main(void)
{
    utUnit meters;
    utUnit copy;

    utScan("m", &meters);

    if (utCopy(&meters, &copy) == 0) {
        char buffer[64];
        utPrint(&copy, buffer);

        printf("Copied unit: %s\n", buffer);
    }
    else {
        printf("Copy failed\n");
    }

    return 0;
}
```

## See Also

- [`utScan()`](utScan.html)
- [`utPrint()`](utPrint.html)
- [`utCompare()`](utcompare.html)
- [`utClone()`](ut-clone.html)
- [`utFree()`](utFree.html)
- [UDUNITS-2 Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [UDUNITS-2 Unit Systems API](unit-systems-api.html)
