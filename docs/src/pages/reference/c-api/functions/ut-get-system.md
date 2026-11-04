---
title: ut_get_system()
sidebar: sidebar
toc: true
permalink: ut-get-system.html
---

Returns the unit system associated with a unit.

## Prototype

```c
ut_system* ut_get_system(
    const ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_system()` returns the [`ut_system`](ut-system.html) associated with a specified
`ut_unit`.

Every unit in UDUNITS-2 belongs to a unit system that defines the set of
units, prefixes, and relationships used for unit construction,
interpretation, and conversion. This function allows applications to
retrieve that system from an existing unit object.

Each [`ut_unit`](ut-unit.html) is associated with exactly one [`ut_system`](ut-system.html). Units obtained
from different systems are not interchangeable and must not be mixed in
unit operations.

The returned pointer references the same unit system used when the unit
was created or obtained. The function does not create or duplicate the
unit system.

## Parameters

| Parameter | Description                                          |
| --------- | ---------------------------------------------------- |
| `unit`    | Unit whose associated unit system should be returned |

## Returns

| Return Value | Description                                                                  |
| ------------ | ---------------------------------------------------------------------------- |
| `ut_system*` | The unit system associated with the unit                                     |
| `NULL` | `unit` is `NULL` or invalid (behavior is undefined per [UDUNITS-2 API](c-api-overview.html#api-organization) expectations) |

## Preconditions

Before calling `ut_get_system()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html) object obtained from the UDUNITS-2 library.

Passing `NULL` or an invalid pointer results in undefined behavior.

## Error Handling

This function does not have defined error-reporting behavior. The result is
undefined if an invalid argument is supplied.

## Object Ownership

`ut_get_system()` returns a pointer to the existing `ut_system`
associated with the specified [`ut_unit`](ut-unit.html).

The function does not create, copy, or modify the unit system.

Ownership of the returned [`ut_system`](ut-system.html) is not transferred. The lifetime
of the system is managed by the code that created or loaded it (e.g.,
via [`ut_new_system()`](ut-new-system.html) or [`ut_read_xml()`](ut-read-xml.html)).

The caller must not free the returned pointer unless it is the original
owner of the unit system.

| Memory Allocation | Caller Responsibility                                      |
| ----------------- | ---------------------------------------------------------- |
| None              | Do not free the returned pointer unless you own the system |

## Thread Safety

`ut_get_system()` is a read-only accessor and does not modify the
provided [`ut_unit`](ut-unit.html) or its associated [`ut_system`](ut-system.html).

The function itself does not introduce thread-safety concerns when used
with valid, immutable objects.

However, UDUNITS-2 maintains global error state for some operations.
Applications that rely on [`ut_get_status()`](ut-get-status.html) in multithreaded contexts
should use external synchronization when making UDUNITS-2 library calls.

## Example

This example demonstrates how `ut_get_system()` retrieves the unit
system associated with a unit so that additional units can be obtained
from the same system.

All allocated [`ut_unit`](ut-unit.html) objects are properly released before freeing
the unit system.

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
    if (meter == NULL) {
        fprintf(stderr, "Failed to retrieve meter unit\n");
        ut_free_system(system);
        return 1;
    }

    /* Retrieve the unit system associated with the unit */
    ut_system* retrieved_system = ut_get_system(meter);
    if (retrieved_system == NULL) {
        fprintf(stderr, "Failed to retrieve unit system\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    /* Use the retrieved system to obtain another unit */
    ut_unit* kilometer = ut_get_unit_by_name(retrieved_system, "kilometer");
    if (kilometer == NULL) {
        fprintf(stderr, "Failed to retrieve kilometer unit\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    printf("Successfully retrieved additional units from the same system\n");

    /* Clean up allocated unit objects */
    ut_free(kilometer);
    ut_free(meter);

    /* Free the unit system (invalidates associated units if not already freed) */
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Successfully retrieved additional units from the same system
```

## See Also

- [`ut_new_system()`](ut-new-system.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_clone()`](ut-clone.html)
