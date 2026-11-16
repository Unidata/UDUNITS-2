---
title: ut_invert()
sidebar: sidebar
toc: true
permalink: ut-invert.html
---

Inverts a unit, producing its reciprocal.

## Prototype

```c
ut_unit* ut_invert(
    const ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Returns a new unit that represents the reciprocal of the given unit:

```
result = 1 / unit
```

The resulting unit is a derived unit whose dimensionality is the inverse of the
input unit. For example, inverting meters yields per-meter (`m⁻¹`).

This is a purely algebraic operation and does not require any unit conversion.

The returned unit is a newly allocated object and must be freed by the caller
using [`ut_free()`](ut-free.html).

## Parameters

| Parameter | Description                                                        |
| --------- | ------------------------------------------------------------------ |
| `unit`    | Pointer to the unit to invert. Must be a valid, non-NULL [`ut_unit`](ut-unit.html) |

## Returns

| Return Value | Description                                                |
| ------------ | ---------------------------------------------------------- |
| non-NULL     | Pointer to a newly allocated unit representing `1 / unit`  |
| `NULL`       | An error occurred (invalid argument or allocation failure) |

## Preconditions

- `unit` must be a valid, non-NULL [`ut_unit`](ut-unit.html).

## Error Handling

Returns `NULL` on failure (e.g., invalid argument or memory allocation failure).
The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

- The returned `ut_unit*` is newly allocated and owned by the caller.
- The caller is responsible for freeing the returned unit using [`ut_free()`](ut-free.html).
- The input unit (`unit`) remains owned by the caller and is not modified.

## Thread Safety

**Not thread-safe.**

This function is not thread-safe when used with a shared [`ut_system`](ut-system.html).
External synchronization is required if multiple threads access the same unit system.

## Example

This example demonstrates how `ut_invert()` creates a reciprocal unit.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* second = ut_parse(system, "second", UT_UTF8);
    if (!second) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* per_second = ut_invert(second);
    if (!per_second) {
        fprintf(stderr, "Failed to invert unit\n");
        ut_free(second);
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(per_second, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Resulting unit: %s\n", buffer);
    }

    ut_free(per_second);
    ut_free(second);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_divide()`](ut-divide.html)
- [`ut_multiply()`](ut-multiply.html)
- [`ut_raise()`](ut-raise.html)
- [`ut_clone()`](ut-clone.html)
