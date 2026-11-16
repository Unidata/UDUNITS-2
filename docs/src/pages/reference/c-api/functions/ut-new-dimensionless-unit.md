---
title: ut_new_dimensionless_unit()
sidebar: sidebar
toc: true
permalink: ut-new-dimensionless-unit.html
---

Creates a new dimensionless unit in a unit system.

## Prototype

```c
ut_unit* ut_new_dimensionless_unit(
    ut_system* system
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_new_dimensionless_unit()` creates and returns a new dimensionless unit associated with the specified unit system.

A dimensionless unit represents a unit with no physical dimension. It is typically used for quantities such as ratios or counts.

The returned unit becomes part of the given [`ut_system`](ut-system.html) and can participate in unit operations (e.g., multiplication or division).

The newly created unit has no assigned name or symbol. These may be set after creation using functions such as `ut_set_name()` and `ut_set_symbol()`.

This function modifies the state of the provided [`ut_system`](ut-system.html) by adding a new unit.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `system`  | Pointer to the [`ut_system`](ut-system.html) in which the dimensionless unit will be created. Should reference a valid unit system. |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `ut_unit*`   | Newly created dimensionless unit associated with the given system |
| `NULL`       | Failure (e.g., memory allocation failure or internal error) |

## Preconditions

Before calling `ut_new_dimensionless_unit()`:

- `system` should reference a valid [`ut_system`](ut-system.html) object.
- The [`ut_system`](ut-system.html) should be properly initialized (e.g., via
  [`ut_new_system()`](ut-new-system.html) or [`ut_read_xml()`](ut-read-xml.html)).

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_new_dimensionless_unit()` creates and returns a new [`ut_unit`](ut-unit.html) object
that is owned by the specified [`ut_system`](ut-system.html).

The caller does not take ownership of the returned unit and must not
free it directly. The unit is automatically destroyed when the
associated [`ut_system`](ut-system.html) is freed.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| Yes | Do not free directly; freed with [`ut_system`](ut-system.html) |

## Thread Safety

`ut_new_dimensionless_unit()` modifies the state of the provided
`ut_system` and is not thread-safe when the same [`ut_system`](ut-system.html) is accessed
concurrently.

UDUNITS-2 maintains a global error status that may be overwritten by
concurrent calls to other library functions. Applications that rely on
[`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure
deterministic behavior.

## Example

This example demonstrates how `ut_new_dimensionless_unit()` creates a
dimensionless unit and assigns it a name and symbol.

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

    ut_unit* unitless = ut_new_dimensionless_unit(system);

    if (unitless == NULL) {
        fprintf(stderr, "Failed to create dimensionless unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_set_symbol(unitless, "1");
    ut_set_name(unitless, "dimensionless");

    printf("Created dimensionless unit\n");

    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Created dimensionless unit
```

## See Also

- [`ut_new_system()`](ut-new-system.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_new_base_unit()`](ut-new-base-unit.html)
- [`ut_set_name()`](ut-set-name.html)
- [`ut_set_symbol()`](ut-set-symbol.html)
- [`ut_multiply()`](ut-multiply.html)
- [`ut_divide()`](ut-divide.html)
- [`ut_get_status()`](ut-get-status.html)
