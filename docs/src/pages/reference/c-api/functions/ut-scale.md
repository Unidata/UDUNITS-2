---
title: ut_scale()
sidebar: sidebar
toc: true
permalink: ut-scale.html
---

Scales a unit by a constant numeric factor.

## Prototype

```c
ut_unit* ut_scale(
    double factor,
    const ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_scale()` returns a new unit that represents the input unit scaled by a constant numeric factor.

The resulting unit has the same dimensionality as the input unit but with its magnitude multiplied by `factor`. For example, scaling meters by 1000 produces a unit equivalent in magnitude to kilometers (though not necessarily represented as a named unit).

This is a purely algebraic transformation and does not perform any unit conversion or normalization beyond applying the scale factor.

The function does not modify the input unit and returns a newly allocated [`ut_unit`](ut-unit.html) object associated with the same [`ut_system`](ut-system.html).

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `factor`  | Scaling factor applied to the unit |
| `unit`    | Pointer to the unit to scale. Should be a valid, non-NULL [`ut_unit`](ut-unit.html) |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| non-NULL     | Pointer to a newly allocated unit representing `factor × unit` |
| `NULL`       | Failure. An error occurred (e.g., invalid argument or memory allocation failure) |

## Preconditions

Before calling `ut_scale()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html).
- `factor` should be a valid numeric value appropriate for scaling.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_scale()` creates a new unit object.

- The returned `ut_unit*` is owned by the caller.
- The caller is responsible for freeing it using [`ut_free()`](ut-free.html).
- The input unit remains owned by the caller and is not modified.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| Yes | Free returned unit with [`ut_free()`](ut-free.html) |

## Thread Safety

This function does not modify the input unit but operates within a shared [`ut_system`](ut-system.html).

External synchronization may be required when units from the same system are used concurrently.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_scale()` creates a scaled unit.

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
    if (!meter) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* kilometer = ut_scale(1000.0, meter);
    if (!kilometer) {
        fprintf(stderr, "Failed to scale unit\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(kilometer, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Scaled unit: %s\n", buffer);
    }

    ut_free(kilometer);
    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

Example output (format may vary):

```c
Scaled unit: 1000 m
```

## See Also

- [`ut_offset()`](ut-offset.html)
- [`ut_multiply()`](ut-multiply.html)
- [`ut_divide()`](ut-divide.html)
- [`ut_raise()`](ut-raise.html)
- [`ut_clone()`](ut-clone.html)
