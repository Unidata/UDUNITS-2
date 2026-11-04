---
title: ut_map_unit_to_name()
sidebar: sidebar
toc: true
permalink: ut-map-unit-to-name.html
---

Associates a textual name with a unit within a unit system.

## Prototype

```c
int ut_map_unit_to_name(
    ut_system* system,
    const ut_unit* unit,
    const char* name
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_map_unit_to_name()` associates a textual name with a [`ut_unit`](ut-unit.html) within a unit system.

This function registers the provided name with the specified unit in the context
of the given [`ut_system`](ut-system.html). The association is stored internally by the unit system
and may be used by the implementation when representing or resolving units.

This function does not guarantee that the associated name will be used by
[`ut_get_unit_by_name()`](ut-get-unit-by-name.html) unless supported by the implementation.

### Key Semantics

- The mapping applies only to the specified [`ut_system`](ut-system.html).
- The function does not modify the provided [`ut_unit`](ut-unit.html).
- The function establishes an association between the provided name and the unit within the system’s internal mapping structures.
- The behavior when the unit already has an associated name or when the name already exists is implementation-defined.
- The extent to which this association participates in name-based lookup operations (e.g., via [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)) is implementation-defined.
- This function complements [`ut_map_name_to_unit()`](ut-map-name-to-unit.html) but does not guarantee identical lookup behavior.

## Parameters

| Parameter | Description                                                             |
| --------- | ----------------------------------------------------------------------- |
| `system`  | Pointer to a valid [`ut_system`](ut-system.html) object                                   |
| `unit`    | Pointer to the [`ut_unit`](ut-unit.html) to associate with a name                       |
| `name`    | Null-terminated string representing the name to associate with the unit |

## Returns

| Return Value | Description                                          |
| ------------ | ---------------------------------------------------- |
| `0`          | Success. The association was created successfully    |
| non-zero     | Failure. The operation did not complete successfully |

## Preconditions

Before calling `ut_map_unit_to_name()`:

- `system` should reference a valid [`ut_system`](ut-system.html) object.
- The [`ut_system`](ut-system.html) should be properly initialized (e.g., via [`ut_read_xml()`](ut-read-xml.html)).
- `unit` should reference a valid [`ut_unit`](ut-unit.html) associated with the same [`ut_system`](ut-system.html).
- `name` should be a non-`NULL`, null-terminated string.

## Error Handling

Returns a non-zero value on failure. The specific error condition can be retrieved using [`ut_get_status()`](ut-get-status.html).

Failure may result from invalid arguments or an inability to update the unit system’s internal mapping structures.

## Object Ownership

`ut_map_unit_to_name()` does not create, destroy, or modify the provided objects.

The function does not transfer ownership; the caller retains responsibility for managing the lifetime of `system`, `unit`, and `name`.

| Memory Allocation | Caller Responsibility                  |
|------------------|----------------------------------------|
| None             | Caller retains ownership of all arguments |

## Thread Safety

This function modifies internal state of the [`ut_system`](ut-system.html) and should not be assumed to be thread-safe.

Concurrent access or modification of the same unit system should be externally synchronized.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_map_unit_to_name()` associates a custom name with a unit.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* meter = ut_get_unit_by_symbol(system, "m");
    if (meter == NULL) {
        fprintf(stderr, "Failed to retrieve unit\n");
        ut_free_system(system);
        return 1;
    }

    if (ut_map_unit_to_name(system, meter, "my_meter") != 0) {
        fprintf(stderr, "Failed to map unit to name\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    /* Behavior of lookup depends on implementation */
    ut_unit* retrieved = ut_get_unit_by_name(system, "my_meter");
    if (retrieved != NULL) {
        printf("Custom name mapped successfully\n");
        ut_free(retrieved);
    }

    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Custom name mapped successfully
```

## See Also

- [`ut_map_name_to_unit()`](ut-map-name-to-unit.html)
- [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html)
- [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
- [`ut_get_name()`](ut-get-name.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
