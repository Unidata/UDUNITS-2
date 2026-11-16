---
title: ut_multiply()
sidebar: sidebar
toc: true
permalink: ut-multiply.html
---

Multiplies two units, producing a derived unit.

## Prototype

```c
ut_unit* ut_multiply(
    const ut_unit* unit1,
    const ut_unit* unit2
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_multiply()` returns a new unit that represents the product of two input units:

```
result = unit1 * unit2
```

The resulting unit is a derived unit whose dimensionality and scale are derived
from the product of the input units.

Both units must belong to the same [`ut_system`](ut-system.html). This function performs a purely
algebraic operation and does not require the units to be convertible.

The input units are not modified.

The returned unit is a newly allocated object and must be freed by the caller
using [`ut_free()`](ut-free.html).

## Parameters

| Parameter | Description                                                       |
| --------- | ----------------------------------------------------------------- |
| `unit1`   | Pointer to the first unit. Should be a valid, non-NULL [`ut_unit`](ut-unit.html)  |
| `unit2`   | Pointer to the second unit. Should be a valid, non-NULL [`ut_unit`](ut-unit.html) |

## Returns

| Return Value | Description                                                                   |
| ------------ | ----------------------------------------------------------------------------- |
| non-NULL     | Pointer to a newly allocated unit representing `unit1 * unit2`                |
| `NULL`       | An error occurred (invalid arguments, system mismatch, or allocation failure) |

## Preconditions

Before calling `ut_multiply()`:

- `unit1` should reference a valid [`ut_unit`](ut-unit.html).
- `unit2` should reference a valid [`ut_unit`](ut-unit.html).
- Both units should belong to the same [`ut_system`](ut-system.html).

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_multiply()` creates a new unit object.

- The returned `ut_unit*` is newly allocated and owned by the caller.
- The caller is responsible for freeing the returned unit using [`ut_free()`](ut-free.html).
- The input units (`unit1`, `unit2`) remain owned by the caller and are not modified.

| Memory Allocation | Caller Responsibility  |
| ----------------- | --------------------- |
| Yes               | Free with [`ut_free()`](ut-free.html) |

## Thread Safety

**Conditionally thread-safe.**

This function performs a read-only algebraic operation on its inputs.
However, it is not safe to call concurrently with other UDUNITS-2 functions
that modify the same [`ut_system`](ut-system.html).

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external
synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_multiply()` creates a derived unit representing
newton-meter (torque).

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* newton = ut_parse(system, "newton", UT_UTF8);
    ut_unit* meter = ut_parse(system, "meter", UT_UTF8);

    if (!newton || !meter) {
        fprintf(stderr, "Failed to parse units\n");
        ut_free(newton);
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    ut_unit* result = ut_multiply(newton, meter);
    if (!result) {
        fprintf(stderr, "Failed to multiply units\n");
        ut_free(newton);
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(result, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Resulting unit: %s\n", buffer);
    }

    ut_free(result);
    ut_free(newton);
    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Resulting unit: N.m
```

## See Also

- [`ut_divide()`](ut-divide.html)
- [`ut_invert()`](ut-invert.html)
- [`ut_raise()`](ut-raise.html)
- [`ut_clone()`](ut-clone.html)
