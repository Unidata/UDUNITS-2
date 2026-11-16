---
title: ut_map_unit_to_symbol()
sidebar: sidebar
toc: true
permalink: ut-map-unit-to-symbol.html
---

Associates a textual symbol with a unit within a unit system.

## Prototype

```c
int ut_map_unit_to_symbol(
    ut_system* system,
    const ut_unit* unit,
    const char* symbol
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_map_unit_to_symbol()` associates a textual symbol with a [`ut_unit`](ut-unit.html) within a unit system.

This function registers the provided symbol with the specified unit in the context
of the given [`ut_system`](ut-system.html). The association is stored internally by the unit system
and may be used by the implementation when representing or resolving units.

This function does not guarantee that the associated symbol will be used by
[`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html) unless supported by the implementation.

**Key semantics:**

- The mapping applies only to the specified [`ut_system`](ut-system.html).
- The function does not modify the provided [`ut_unit`](ut-unit.html).
- The function establishes an association between the provided symbol and the unit within the system’s internal mapping structures.
- The behavior when the unit already has an associated symbol or when the symbol already exists is implementation-defined.
- The extent to which this association participates in symbol-based lookup operations (e.g., via [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)) is implementation-defined.
- This function complements [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html), but the two do not guarantee
equivalent behavior in symbol-based lookup operations.

## Parameters

| Parameter | Description                                                               |
| --------- | ------------------------------------------------------------------------- |
| `system`  | Pointer to a valid [`ut_system`](ut-system.html) object                                     |
| `unit`    | Pointer to the [`ut_unit`](ut-unit.html) to associate with a symbol                       |
| `symbol`  | Null-terminated string representing the symbol to associate with the unit |

## Returns

| Return Value | Description                                          |
| ------------ | ---------------------------------------------------- |
| `0`          | Success. The association was created successfully    |
| non-zero     | Failure. The operation did not complete successfully |

## Preconditions

Before calling `ut_map_unit_to_symbol()`:

- `system` should reference a valid [`ut_system`](ut-system.html) object.
- The [`ut_system`](ut-system.html) should be properly initialized (e.g., via [`ut_read_xml()`](ut-read-xml.html)).
- `unit` should reference a valid [`ut_unit`](ut-unit.html) associated with the same [`ut_system`](ut-system.html).
- `symbol` should be a non-NULL, null-terminated string.

## Error Handling

Returns a non-zero value on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

Failure may result from invalid arguments or an inability to update the unit system’s internal mapping structures.

## Object Ownership

`ut_map_unit_to_symbol()` does not create, destroy, or modify the provided objects.

The function does not transfer ownership; the caller retains responsibility for managing the lifetime of `system`, `unit`, and `symbol`.

| Memory Allocation | Caller Responsibility |
| ---------------- | -------------------- |
| None | Caller retains ownership of all arguments |

## Thread Safety

This function modifies internal state of the [`ut_system`](ut-system.html) and should not be assumed to be thread-safe.

Concurrent access or modification of the same unit system should be externally synchronized.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on `ut_get_status()`
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_map_unit_to_symbol()` associates a custom
symbol with a unit.

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

    ut_unit* meter = ut_get_unit_by_name(system, "meter");
    if (meter == NULL) {
        fprintf(stderr, "Failed to retrieve unit\n");
        ut_free_system(system);
        return 1;
    }

    if (ut_map_unit_to_symbol(system, meter, "my_m") != 0) {
        fprintf(stderr, "Failed to map unit to symbol\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    /* Behavior of lookup depends on implementation */
    ut_unit* retrieved = ut_get_unit_by_symbol(system, "my_m");
    if (retrieved != NULL) {
        printf("Custom symbol mapped successfully\n");
        ut_free(retrieved);
    }

    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Custom symbol mapped successfully
```

## See Also

- [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html)
- [`ut_map_unit_to_name()`](ut-map-unit-to-name.html)
- [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)
- [`ut_get_symbol()`](ut-get-symbol.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
