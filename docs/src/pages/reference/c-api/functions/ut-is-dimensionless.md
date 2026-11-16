---
title: ut_is_dimensionless()
sidebar: sidebar
toc: true
permalink: ut-is-dimensionless.html
---

Determines whether a unit is dimensionless.

## Prototype

```c
int ut_is_dimensionless(
    const ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_is_dimensionless()` determines whether a given unit is dimensionless.

In UDUNITS-2, a dimensionless unit is one that has no physical dimensions.
Such units behave as scalar quantities in unit expressions.

This function evaluates the dimensionality of the provided [`ut_unit`](ut-unit.html) and returns
a boolean-like result indicating whether the unit is dimensionless.

The function performs a read-only inspection of the unit and does not modify
the input object.

## Parameters

| Parameter | Description                                         |
| --------- | --------------------------------------------------- |
| `unit`    | Pointer to the unit to be tested for dimensionality |

## Returns

| Return Value | Description                                              |
| ------------ | -------------------------------------------------------- |
| `1`          | The unit is dimensionless                                |
| `0`          | The unit is not dimensionless or an error occurred       |

## Preconditions

Before calling `ut_is_dimensionless()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html) object.

## Error Handling

Returns `0` if the unit is not dimensionless or if an error occurs.
The specific error condition can be retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_is_dimensionless()` does not create, destroy, or modify any objects.

The input [`ut_unit`](ut-unit.html) remains owned by the caller or the unit system from which
it was obtained.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| None | No ownership changes |

## Thread Safety

`ut_is_dimensionless()` is a read-only function and does not modify the provided
`ut_unit`.

It is safe to call concurrently from multiple threads, provided that no other
threads are simultaneously modifying the same unit object or its associated
unit system.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_is_dimensionless()` determines whether a unit
has no physical dimensions.

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

    ut_unit* meter = ut_get_unit_by_name(system, "meter");
    ut_unit* one = ut_get_dimensionless_unit_one(system);

    if (meter == NULL || one == NULL) {
        fprintf(stderr, "Failed to retrieve units\n");
        ut_free_system(system);
        return 1;
    }

    printf("meter is %sdimensionless\n",
           ut_is_dimensionless(meter) ? "" : "not ");

    printf("one is %sdimensionless\n",
           ut_is_dimensionless(one) ? "" : "not ");

    ut_free_system(system);
    return 0;
}
```

Example output:

```c
meter is not dimensionless
one is dimensionless
```

## See Also

- [`ut_get_dimensionless_unit_one()`](ut-get-dimensionless-unit-one.html)
- [`ut_compare()`](ut-compare.html)
- [`ut_multiply()`](ut-multiply.html)
- [`ut_get_status()`](ut-get-status.html)
