---
title: ut_clone()
sidebar: sidebar
toc: true
permalink: ut-clone.html
---

Creates a deep copy of a unit.

## Prototype

```c
ut_unit* ut_clone(
    const ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Creates a **deep copy** of an existing unit.

This function allocates and returns a new [`ut_unit`](ut-unit.html) object that is equivalent to
the input unit. The cloned unit has the same dimensionality, scale, and internal
representation as the original but is a separate object with its own lifetime.

The returned unit can be modified or freed independently of the original.

The cloned unit remains associated with the same [`ut_system`](ut-system.html) as the source unit.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit`    | Pointer to the unit to clone. Must be a non-NULL pointer to a valid [`ut_unit`](ut-unit.html) |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| non-NULL     | Pointer to a newly allocated [`ut_unit`](ut-unit.html) equivalent to `unit` |
| `NULL`       | Failure. An error occurred (e.g., invalid argument, memory allocation failure, or internal error) |

## Preconditions

- `unit` must be a non-NULL pointer to a valid [`ut_unit`](ut-unit.html).

## Error Handling

- Returns `NULL` if `unit` is `NULL`.
- Returns `NULL` if memory allocation fails or an internal error occurs.

Additional error details may be available via the UDUNITS-2 error status mechanism.

## Object Ownership

- The returned `ut_unit*` is **owned by the caller**.
- The caller is responsible for releasing it using [`ut_free()`](ut-free.html).
- The input `unit` remains owned by the caller and is not modified.

## Thread Safety

Thread safety is not guaranteed by the library.

This function does not modify the input unit or its associated system.
However, because units are associated with a shared [`ut_system`](ut-system.html), external
synchronization may be required when units from the same system are used
concurrently across threads.

## Example

This example demonstrates how `ut_clone()` creates an independent copy of a unit.

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

    ut_unit* meter_copy = ut_clone(meter);
    if (!meter_copy) {
        fprintf(stderr, "Failed to clone unit\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    printf("Successfully cloned unit\n");

    ut_free(meter_copy);
    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_free()`](ut-free.html)
- [`ut_compare()`](ut-compare.html)
- [`ut_get_system()`](ut-get-system.html)
