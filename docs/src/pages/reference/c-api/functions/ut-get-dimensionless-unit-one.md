---
title: ut_get_dimensionless_unit_one()
sidebar: sidebar
toc: true
permalink: ut-get-dimensionless-unit-one.html
---

Returns the canonical dimensionless unit representing the multiplicative identity.

```c
ut_unit* ut_get_dimensionless_unit_one(
    const ut_system* system
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_dimensionless_unit_one()` returns the canonical dimensionless unit
that represents the multiplicative identity within a specified [`ut_system`](ut-system.html).

In UDUNITS-2, dimensionless quantities are represented by a special unit
that has no physical dimensions and acts as the multiplicative identity
for unit expressions.

This unit corresponds to the scalar value **1** and is used internally
as the multiplicative identity in unit expressions.

The returned unit belongs to the specified unit system and may be used in
unit arithmetic, conversions, or comparisons where a dimensionless
quantity is required.

The function does not create a new unit object. Instead, it returns the
existing canonical dimensionless unit associated with the provided unit
system.

## Parameters

| Parameter | Description                                                                 |
| --------- | --------------------------------------------------------------------------- |
| `system`  | Unit system from which the canonical dimensionless unit should be retrieved |

## Returns

| Return Value | Description                                                                                                  |
| ------------ | ------------------------------------------------------------------------------------------------------------ |
| `ut_unit*`   | Pointer to the canonical dimensionless unit                                                                  |
| `NULL`       | Returns `NULL` only if `system` is `NULL` or invalid. Otherwise, this function is expected to always succeed |

## Preconditions

Before calling `ut_get_dimensionless_unit_one()`:

- `system` must not be `NULL`.
- `system` must reference a valid [`ut_system`](ut-system.html) object created by the UDUNITS-2 library.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_get_dimensionless_unit_one()` returns a pointer to the canonical
dimensionless unit associated with the specified unit system.

The function does not create or duplicate the unit. It simply returns the
existing unit object referenced by the provided [`ut_system`](ut-system.html).

Ownership of the returned [`ut_unit`](ut-unit.html) remains with the unit system that
contains it. The caller must **not** attempt to modify or free the returned
pointer.

Each [`ut_system`](ut-system.html) maintains its own canonical dimensionless unit. Units
obtained from different systems are not interchangeable and must not be
mixed in unit operations.

| Memory Allocation | Caller Responsibility       |
| ----------------- | --------------------------- |
| None              | No ownership is transferred |

## Thread Safety

`ut_get_dimensionless_unit_one()` is a read-only accessor and does not modify
the provided [`ut_system`](ut-system.html) object. As such, it is safe to call concurrently from
multiple threads, provided that no other threads are simultaneously modifying
the same [`ut_system`](ut-system.html).

The returned `ut_unit*` refers to an internal object owned by the unit system.
Callers must not modify or free this object. Concurrent use of the returned
pointer is safe as long as the associated [`ut_system`](ut-system.html) remains valid and is not
being modified.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. While this function is not expected to set an
error status, applications that rely on [`ut_get_status()`](ut-get-status.html) should use external
synchronization to ensure deterministic results in multithreaded environments.

## Example

This example demonstrates how `ut_get_dimensionless_unit_one()` retrieves
the canonical dimensionless unit and uses it in a unit arithmetic
operation.

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

    /* Multiply a unit by the dimensionless unit (identity operation) */
    ut_unit* result = ut_multiply(meter, one);
    if (result == NULL) {
        fprintf(stderr, "Failed to multiply units\n");
        ut_free_system(system);
        return 1;
    }

    printf("Successfully combined meter with the dimensionless unit\n");

    /* Free the result of ut_multiply() */
    ut_free(result);

    /* Free the unit system (also releases internally managed units) */
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Successfully combined meter with the dimensionless unit
```

## See Also

- [`ut_get_system()`](ut-get-system.html)
- [`ut_clone()`](ut-clone.html)
- [`ut_free()`](ut-free.html)
- [`ut_compare()`](ut-compare.html)
