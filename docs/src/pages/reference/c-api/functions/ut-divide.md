---
title: ut_divide()
sidebar: sidebar
toc: true
permalink: ut-divide.html
---

Divides one unit by another, producing a derived unit.

## Prototype

```c
ut_unit* ut_divide(
    const ut_unit* numer,
    const ut_unit* denom
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Divides one unit by another, returning a new unit that represents the quotient:

```
result = numer / denom
```

The resulting unit is a derived unit whose dimensionality and scale are computed
from the ratio of the two input units.

Both units must belong to the same [`ut_system`](ut-system.html). The function performs a purely
algebraic operation and does not require the units to be convertible.

The returned unit is a newly allocated object and must be freed by the caller
using [`ut_free()`](ut-free.html).

## Parameters

| Parameter | Description                                                          |
| --------- | -------------------------------------------------------------------- |
| `numer`   | Pointer to the numerator unit. Must be a valid, non-NULL [`ut_unit`](ut-unit.html)   |
| `denom`   | Pointer to the denominator unit. Must be a valid, non-NULL [`ut_unit`](ut-unit.html) |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| non-NULL     | Pointer to a newly allocated unit representing `numer / denom` |
| `NULL`       | An error occurred (invalid arguments, system mismatch, or allocation failure) |

## Preconditions

- `numer` must be a valid, non-NULL [`ut_unit`](ut-unit.html).
- `denom` must be a valid, non-NULL [`ut_unit`](ut-unit.html).
- Both units must belong to the same [`ut_system`](ut-system.html).

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

- The returned `ut_unit*` is newly allocated and owned by the caller.
- The caller is responsible for freeing the returned unit using [`ut_free()`](ut-free.html).
- The input units (`numer`, `denom`) remain owned by the caller and are not modified.

## Thread Safety

**Not thread-safe.**

This function is not thread-safe when used with a shared [`ut_system`](ut-system.html).
External synchronization is required if multiple threads access the same unit system.

## Example

This example demonstrates how `ut_divide()` creates a derived unit representing meters per second.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* meter = ut_parse(system, "meter", UT_UTF8);
    ut_unit* second = ut_parse(system, "second", UT_UTF8);

    if (!meter || !second) {
        fprintf(stderr, "Failed to parse units\n");
        ut_free(meter);
        ut_free(second);
        ut_free_system(system);
        return 1;
    }

    ut_unit* mps = ut_divide(meter, second);
    if (!mps) {
        fprintf(stderr, "Failed to divide units\n");
        ut_free(meter);
        ut_free(second);
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(mps, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Resulting unit: %s\n", buffer);
    }

    ut_free(mps);
    ut_free(meter);
    ut_free(second);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_multiply()`](ut-multiply.html)
- [`ut_invert()`](ut-invert.html)
- [`ut_clone()`](ut-clone.html)
- [`ut_compare()`](ut-compare.html)
