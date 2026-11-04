---
title: ut_map_symbol_to_unit()
sidebar: sidebar
toc: true
permalink: ut-map-symbol-to-unit.html
---


Associates a unit symbol with a unit within a unit system.

## Prototype

```c
int ut_map_symbol_to_unit(
    ut_system* system,
    const char* symbol,
    const ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_map_symbol_to_unit()` associates a textual unit symbol with a `ut_unit`
within a unit system.

This function updates the symbol-to-unit mapping used by the unit system so
that subsequent lookups by symbol (e.g., via [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)) may
resolve to the provided unit.

**Key semantics:**

- The mapping applies only to the specified [`ut_system`](ut-system.html).
- The function does not modify the provided [`ut_unit`](ut-unit.html) object.
- The behavior when the symbol already exists is implementation-defined.
- The internal storage and representation of the mapping are not specified
  by the API.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `system`  | Pointer to a valid [`ut_system`](ut-system.html) object. |
| `symbol`  | Null-terminated string representing the unit symbol to associate. |
| `unit`    | Pointer to the [`ut_unit`](ut-unit.html) that the symbol should resolve to. |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `0`          | Success. The operation completed successfully. |
| non-zero     | Failure. The operation did not complete successfully. |

## Preconditions

Before calling `ut_map_symbol_to_unit()`:

- `system` should reference a valid [`ut_system`](ut-system.html) object.
- `symbol` should be a non-NULL, null-terminated string.
- `unit` should reference a valid [`ut_unit`](ut-unit.html) associated with the same system.

## Error Handling

Returns a non-zero value on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_map_symbol_to_unit()` does not create, destroy, or modify the provided objects.

The function does not transfer ownership; the caller retains responsibility
for managing the lifetime of `system`, `symbol`, and `unit`.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| None | Caller retains ownership of all arguments. |

## Thread Safety

This function modifies internal state of the [`ut_system`](ut-system.html) and should not be
assumed to be thread-safe.

Concurrent access or modification of the same unit system should be
externally synchronized.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_map_symbol_to_unit()` associates a custom
symbol with a unit and retrieves it.

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

    if (ut_map_symbol_to_unit(system, "my_m", meter) != 0) {
        fprintf(stderr, "Failed to map symbol\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

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

- [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)
- [`ut_map_name_to_unit()`](ut-map-name-to-unit.html)
- [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
