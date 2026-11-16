---
title: utFree()
sidebar: sidebar
toc: true
permalink: utFree.html
---

Releases internal resources associated with a UDUNITS-1 `utUnit` structure.

{% include warning.html content="
**`utFree()` is part of the UDUNITS-1 legacy compatibility interface.**  

New applications should use the UDUNITS-2 object model and functions such as
[`ut_free()`](ut-free.html), which provide clearer ownership semantics and
improved integration with the UDUNITS-2 API.
"%}

## Prototype

```c
int utFree(
    utUnit* unit
);
```

## Header

```c
#include <udunits.h>
```

## Description

`utFree()` releases any internal resources associated with a UDUNITS-1
`utUnit` structure.

The function invalidates the contents of the provided structure by releasing
internal references to the unit system representation. The structure itself is
not deallocated, as storage is provided by the caller.

`utFree()` should be called when a `utUnit` object is no longer needed in order
to prevent resource leaks in applications using the UDUNITS-1 compatibility
interface.

Calling `utFree()` on an already-freed or uninitialized unit results in
undefined behavior.

## Parameters

| Parameter | Description                                                                 |
| --------- | --------------------------------------------------------------------------- |
| `unit`    | Pointer to the `utUnit` structure whose internal resources will be released |

## Returns

| Return Value | Description                     |
| ------------ | ------------------------------- |
| `0`          | Resources successfully released |
| `-1`         | An error occurred               |

## Error Handling

`utFree()` returns `-1` if an error occurs.

Typical error conditions include:

- `unit` is an invalid pointer
- `unit` does not reference a properly initialized `utUnit`

The UDUNITS-1 compatibility interface provides limited diagnostic detail.

Applications requiring improved diagnostics and memory safety should use the
UDUNITS-2 API.

## Object Ownership

`utFree()` releases only the internal resources associated with the `utUnit`
structure. It does not deallocate the structure itself.

The caller is responsible for managing storage of the `utUnit` object.

| Memory Allocation | Caller Responsibility |
|------------------|----------------------|
| Internal references released | Caller manages storage for `utUnit` structure |

After calling `utFree()`, the `utUnit` structure must not be used unless it is
reinitialized.

## Thread Safety

`utFree()` does not modify global state directly, but the UDUNITS-1
compatibility layer relies on shared internal resources and is not guaranteed
to be thread-safe without external synchronization.

Applications requiring reliable thread safety should use the UDUNITS-2 API.

## Example

The following example demonstrates how `utFree()` releases internal resources
associated with a previously parsed `utUnit` definition.

```c
#include <stdio.h>
#include <udunits.h>

int main(void)
{
    utUnit meters;

    if (utScan("m", &meters) != 0) {
        printf("Failed to parse unit\n");
        return 1;
    }

    if (utFree(&meters) != 0) {
        printf("Failed to free unit resources\n");
        return 1;
    }

    return 0;
}
```

## See Also

- [`utScan()`](utScan.html)
- [`utCopy()`](utCopy.html)
- [`utCompare()`](utcompare.html)
- [`utPrint()`](utPrint.html)
- [UDUNITS-2 Unit Creation & Destruction API](unit-creation-destruction-api.html)
- [UDUNITS-2 Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [UDUNITS-2 Unit Systems API](unit-systems-api.html)
