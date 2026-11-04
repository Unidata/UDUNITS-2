---
title: ut_set_second()
sidebar: sidebar
toc: true
permalink: ut-set-second.html
---

Designates a unit as the base unit of time for its unit system.

## Prototype

```c
int ut_set_second(
    const ut_unit* second
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_set_second()` designates a unit as the base time unit ("second")
for the [`ut_system`](ut-system.html) associated with the given unit.

The designated unit serves as the reference time unit for that system
and is used when constructing time-offset units, such as those created
by [`ut_offset_by_time()`](ut-offset-by-time.html).

Each [`ut_unit`](ut-unit.html) belongs to exactly one [`ut_system`](ut-system.html), and the designation
applies only within the system that owns the provided unit. Each
`ut_system` maintains its own independent base time unit, and units
from different systems are not interchangeable and must not be combined
in unit operations.

If a base time unit has already been established for the system and a
different unit is provided, then the function fails.

This function is typically used when constructing a unit system
programmatically and defining its fundamental base units prior to
creating time-offset units.

## Parameters

| Parameter | Description                                |
| --------- | ------------------------------------------ |
| `second`  | Unit that should be designated as the base second of its unit system |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `0`          | Success     |
| `-1`         | Failure   |

## Preconditions

Before calling `ut_set_second()`:

- `second` must not be `NULL`.
- `second` must reference a valid [`ut_unit`](ut-unit.html) associated with a [`ut_system`](ut-system.html).

## Error Handling

Returns a non-zero value on failure. The specific error condition can be
retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_set_second()` does not create, copy, or take ownership of the
provided [`ut_unit`](ut-unit.html). It records the unit as the base time unit within
its associated [`ut_system`](ut-system.html).

All [`ut_unit`](ut-unit.html) instances are owned by the [`ut_system`](ut-system.html) that created them.
The caller must not free the unit independently. The unit will be
released when the associated [`ut_system`](ut-system.html) is freed.

| Memory Allocation | Caller Responsibility                 |
| ----------------- | ------------------------------------- |
| None              | No additional ownership changes occur |

## Thread Safety

`ut_set_second()` is not thread-safe when operating on the same
`ut_system` object.

- The function modifies internal state of the [`ut_system`](ut-system.html) associated
  with the provided unit. Concurrent modification of the same system
  from multiple threads can result in undefined behavior and must be
  externally synchronized.

- UDUNITS-2 maintains a global error status that is accessed via
  [`ut_get_status()`](ut-get-status.html). This status is not thread-local, so concurrent
  calls to UDUNITS-2 functions may overwrite the reported error state.

Concurrent use of separate [`ut_system`](ut-system.html) objects is generally safe,
provided there is no shared state between threads and no reliance on
the global error status without synchronization.

## Example

This example demonstrates how `ut_set_second()` defines the base time
unit for a unit system and safely creates a time-offset unit.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system = ut_new_system();

    if (system == NULL) {
        fprintf(stderr, "Failed to create unit system\n");
        return 1;
    }

    /* Create a base unit representing seconds */
    ut_unit* second = ut_new_base_unit(system);

    if (second == NULL) {
        fprintf(stderr, "Failed to create base unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_set_name(second, "second");
    ut_set_symbol(second, "s");

    /* Register the unit as the system's base time unit */
    if (ut_set_second(second) != 0) {
        fprintf(stderr, "Failed to register second unit\n");
        ut_free_system(system);
        return 1;
    }

    /* Create a timestamp unit relative to a reference time */
    ut_unit* time_origin = ut_offset_by_time(second, 0.0);

    if (time_origin == NULL) {
        fprintf(stderr, "Failed to create time-origin unit\n");
        ut_free_system(system);
        return 1;
    }

    printf("Created a timestamp unit relative to the time origin\n");

    /*
     * Units are owned by the system that created them and will be
     * released when the system is freed.
     */

    ut_free_system(system);
    return 0;
}
```

Example output:

```c
Created a timestamp unit relative to the time origin
```

## See Also

- [`ut_encode_time()`](ut-encode-time.html)
- [`ut_encode_clock()`](ut-encode-clock.html)
- [`ut_encode_date()`](ut-encode-date.html)
- [`ut_decode_time()`](ut-decode-time.html)
