---
title: ut_offset_by_time()
sidebar: sidebar
toc: true
permalink: ut-offset-by-time.html
---

Creates a time-offset unit relative to a specified origin.

## Prototype

```c
ut_unit* ut_offset_by_time(
    const ut_unit* unit,
    double origin
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_offset_by_time()` returns a new unit that represents the given time
unit offset by a specified origin.

This function is used to construct time units that include an additive
offset (e.g., “seconds since 1970-01-01 00:00:00 UTC”). The resulting
unit retains the same time dimension as the input unit but incorporates
an additive offset.

The `origin` specifies the reference time as a numeric value expressed in
the given unit.

The returned unit is associated with the same [`ut_system`](ut-system.html) as the input
unit and can be used in subsequent unit operations.

This function does not modify the input unit.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit`    | Pointer to a [`ut_unit`](ut-unit.html). Should reference a valid `ut_unit`; typically a time-based unit. |
| `origin`  | Numeric offset representing the reference time, expressed in the given unit. |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `ut_unit*`   | A new unit representing the input unit with the specified time offset |
| `NULL`       | Failure (e.g., invalid argument or internal error) |

## Preconditions

Before calling `ut_offset_by_time()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html) object.
- `unit` should represent a time-based unit.
- The associated [`ut_system`](ut-system.html) should be properly initialized.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_offset_by_time()` creates and returns a new [`ut_unit`](ut-unit.html) object that is
owned by the same [`ut_system`](ut-system.html) as the input unit.

The caller does not take ownership of the returned unit and must not
free it directly. The unit is automatically destroyed when the
associated [`ut_system`](ut-system.html) is freed.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| Yes | Do not free directly; freed with [`ut_system`](ut-system.html) |

## Thread Safety

`ut_offset_by_time()` creates a new unit within the associated
`ut_system` and is not thread-safe when the same [`ut_system`](ut-system.html) is accessed concurrently.

UDUNITS-2 maintains a global error status that may be overwritten by
concurrent calls to other library functions. Applications that rely on
[`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure
deterministic behavior.

## Example

This example demonstrates how `ut_offset_by_time()` creates a time-offset unit.

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

    ut_unit* second = ut_new_base_unit(system);

    if (second == NULL) {
        fprintf(stderr, "Failed to create base unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_set_name(second, "second");
    ut_set_symbol(second, "s");

    ut_unit* shifted = ut_offset_by_time(second, 0.0);

    if (shifted == NULL) {
        fprintf(stderr, "Failed to create offset unit\n");
        ut_free_system(system);
        return 1;
    }

    printf("Created time-offset unit\n");

    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Created time-offset unit
```

## See Also

- [`ut_offset()`](ut-offset.html)
- [`ut_scale()`](ut-scale.html)
- [`ut_new_system()`](ut-new-system.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
