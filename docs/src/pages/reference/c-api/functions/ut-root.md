---
title: ut_root()
sidebar: sidebar
toc: true
permalink: ut-root.html
---

Returns a new unit that represents the integral root of a unit.

## Prototype

```c
ut_unit* ut_root(
    const ut_unit* unit,
    int root
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_root()` returns a new unit that represents the *n*-th root of the given unit:

```c
result = unit^(1/root)
```

The resulting unit is a derived unit whose dimensionality is scaled by the
reciprocal of the specified root. For example, taking the square root of a unit
with dimension `m²` yields `m`.

This is a purely algebraic operation. The input unit is not modified.

The function requires that the root be a non-zero integer and that the operation
results in a valid unit.

The returned unit is a newly allocated object and must be freed by the caller
using [`ut_free()`](ut-free.html).

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit`    | Pointer to the unit to transform. Should be a valid, non-NULL [`ut_unit`](ut-unit.html) |
| `root`    | Integer root value. Must be non-zero |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| non-NULL     | Pointer to a newly allocated unit representing `unit^(1/root)` |
| `NULL`       | An error occurred (e.g., `root == 0`, invalid argument, or allocation failure) |

## Preconditions

Before calling `ut_root()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html).
- `root` should be a non-zero integer.
- The operation should result in a valid unit.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_root()` creates a new unit object.

- The returned `ut_unit*` is newly allocated and owned by the caller.
- The caller is responsible for freeing the returned unit using [`ut_free()`](ut-free.html).
- The input unit (`unit`) remains owned by the caller and is not modified.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| Yes | Free with [`ut_free()`](ut-free.html) |

## Thread Safety

**Conditionally thread-safe.**

This function performs a read-only algebraic transformation of the input unit.
However, it is not safe to call concurrently with other UDUNITS-2 functions that
modify the same [`ut_system`](ut-system.html).

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_root()` computes the square root of a unit.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* area = ut_parse(system, "meter^2", UT_UTF8);
    if (!area) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* length = ut_root(area, 2);
    if (!length) {
        fprintf(stderr, "Failed to compute root\n");
        ut_free(area);
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(length, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Resulting unit: %s\n", buffer);
    }

    ut_free(length);
    ut_free(area);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Resulting unit: m
```

## See Also

- [`ut_raise()`](ut-raise.html)
- [`ut_multiply()`](ut-multiply.html)
- [`ut_divide()`](ut-divide.html)
- [`ut_invert()`](ut-invert.html)
- [`ut_clone()`](ut-clone.html)
