---
title: ut_offset()
sidebar: sidebar
toc: true
permalink: ut-offset.html
---

Creates a unit that is offset from another unit by a constant value.

## Prototype

```c
ut_unit* ut_offset(
    const ut_unit* unit,
    double offset
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_offset()` returns a new unit that represents the given unit with an additive offset applied.

This function constructs a derived unit that is linearly related to the input unit by a constant translation (i.e., *y = x + offset*). Unlike [`ut_scale()`](ut-scale.html), which applies a multiplicative factor, this function applies an additive transformation.

The resulting unit retains the same dimensionality as the input unit and is associated with the same [`ut_system`](ut-system.html).

This function does not modify the input unit.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit`    | Pointer to a [`ut_unit`](ut-unit.html). Should reference a valid [`ut_unit`](ut-unit.html) object. |
| `offset`  | Additive offset applied to the unit, expressed in the given unit. |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `ut_unit*`   | A new unit representing the input unit with the specified offset |
| `NULL`       | Failure (e.g., invalid argument or internal error) |

## Preconditions

Before calling `ut_offset()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html) object.
- The associated [`ut_system`](ut-system.html) should be properly initialized.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_offset()` creates and returns a new [`ut_unit`](ut-unit.html) object that is owned by
the same [`ut_system`](ut-system.html) as the input unit.

The caller does not take ownership of the returned unit and must not free
it directly. The unit is automatically destroyed when the associated
`ut_system` is freed.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| Yes | Do not free directly; freed with [`ut_system`](ut-system.html) |

## Thread Safety

`ut_offset()` creates a new unit within the associated [`ut_system`](ut-system.html) and is
not thread-safe if the same [`ut_system`](ut-system.html) is accessed concurrently.

UDUNITS-2 maintains a global error status that may be overwritten by
concurrent calls to other library functions. Applications that rely on
[`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure
deterministic behavior.

## Example

This example demonstrates how `ut_offset()` creates an offset unit.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        fprintf(stderr, "Failed to load UDUNITS-2 database\n");
        return 1;
    }

    ut_unit* kelvin = ut_get_unit_by_name(system, "kelvin");
    if (kelvin == NULL) {
        fprintf(stderr, "Failed to retrieve unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* celsius = ut_offset(kelvin, -273.15);
    if (celsius == NULL) {
        fprintf(stderr, "Failed to create offset unit\n");
        ut_free_system(system);
        return 1;
    }

    printf("Created Celsius-like offset unit from Kelvin\n");

    ut_free_system(system);
    return 0;
}
```

## See Also

- [`ut_offset_by_time()`](ut-offset-by-time.html)
- [`ut_scale()`](ut-scale.html)
- [`ut_multiply()`](ut-multiply.html)
- [`ut_new_system()`](ut-new-system.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
