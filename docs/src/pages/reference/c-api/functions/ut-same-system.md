---
title: ut_same_system()
sidebar: sidebar
toc: true
permalink: ut-same-system.html
---

Determines whether two units belong to the same unit system.

## Prototype

```c
int ut_same_system(
    const ut_unit* unit1,
    const ut_unit* unit2
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_same_system()` determines whether two [`ut_unit`](ut-unit.html) objects are associated
with the same [`ut_system`](ut-system.html).

Each unit in UDUNITS-2 belongs to exactly one unit system. Units from
different systems are not interchangeable and should not be used together
in unit operations.

This function performs a system-level comparison only. It does not compare
unit structure, dimensionality, or equivalence—only whether both units
originate from the same underlying [`ut_system`](ut-system.html).

The comparison is based on the identity of the associated `ut_system`
objects.

The function does not modify either input unit.

## Parameters

| Parameter | Description            |
| --------- | ---------------------- |
| `unit1`   | First unit to compare  |
| `unit2`   | Second unit to compare |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `1`          | Both units belong to the same [`ut_system`](ut-system.html) |
| `0`          | Units do not belong to the same `ut_system`; may also result from invalid input (undefined behavior) |

## Preconditions

Before calling `ut_same_system()`:

- `unit1` should reference a valid [`ut_unit`](ut-unit.html) object.
- `unit2` should reference a valid [`ut_unit`](ut-unit.html) object.

Passing `NULL` or invalid pointers results in undefined behavior.

## Error Handling

This function does not report errors. A return value of `0` indicates that
the units are not from the same system.

Behavior is undefined if invalid arguments are supplied.

## Object Ownership

`ut_same_system()` does not create, destroy, or modify any objects.

It operates on existing [`ut_unit`](ut-unit.html) objects and returns a scalar result.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| None              | None                 |

## Thread Safety

`ut_same_system()` is a read-only function and does not modify the provided
`ut_unit` objects or their associated [`ut_system`](ut-system.html).

The function itself does not introduce thread-safety concerns when used
with valid, immutable objects.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_same_system()` determines whether two units belong to the same unit system.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system1 = ut_read_xml(NULL);
    ut_system* system2 = ut_new_system();

    if (system1 == NULL || system2 == NULL) {
        fprintf(stderr, "Failed to create unit systems\n");
        return 1;
    }

    ut_unit* meter1 = ut_get_unit_by_name(system1, "meter");
    ut_unit* meter2 = ut_new_base_unit(system2);

    if (meter1 == NULL || meter2 == NULL) {
        fprintf(stderr, "Failed to create units\n");
        ut_free_system(system1);
        ut_free_system(system2);
        return 1;
    }

    if (ut_same_system(meter1, meter2)) {
        printf("Units belong to the same system\n");
    } else {
        printf("Units belong to different systems\n");
    }

    ut_free(meter1);
    ut_free(meter2);

    ut_free_system(system1);
    ut_free_system(system2);

    return 0;
}
```

Example output:

```c
Units belong to different systems
```

## See Also

- [`ut_get_system()`](ut-get-system.html)
- [`ut_new_system()`](ut-new-system.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_compare()`](ut-compare.html)
