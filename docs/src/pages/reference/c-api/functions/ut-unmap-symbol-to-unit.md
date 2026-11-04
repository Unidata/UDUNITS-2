---
title: ut_unmap_symbol_to_unit()
sidebar: sidebar
toc: true
permalink: ut-unmap-symbol-to-unit.html
---

Removes an association between a textual unit symbol and a unit within a unit system.

## Prototype

```c
int ut_unmap_symbol_to_unit(
    ut_system* system,
    const char* symbol
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_unmap_symbol_to_unit()` removes a previously established mapping between a
textual unit symbol and a [`ut_unit`](ut-unit.html) within a unit system.

This function updates the internal symbol-to-unit mapping of the specified
`ut_system`.

**Key semantics:**

- The operation applies only to the specified [`ut_system`](ut-system.html).
- The function removes the association for the given symbol.
- The behavior when the symbol is not currently mapped is implementation-defined.
- The function does not modify any [`ut_unit`](ut-unit.html) objects.
- The internal representation and storage of mappings are not specified by the API.
- This function complements [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html).

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `system`  | Pointer to a valid [`ut_system`](ut-system.html) object. |
| `symbol`  | Null-terminated string representing the unit symbol to remove from the mapping. |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `0`          | Success. The operation completed successfully. |
| non-zero     | Failure. The operation did not complete successfully. |

## Preconditions

Before calling `ut_unmap_symbol_to_unit()`:

- `system` should reference a valid [`ut_system`](ut-system.html) object.
- `symbol` should be a non-NULL, null-terminated string.

## Error Handling

Returns a non-zero value on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_unmap_symbol_to_unit()` does not create, destroy, or modify any objects.

The function does not transfer ownership; the caller retains responsibility
for managing the lifetime of `system` and `symbol`.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| None | Caller retains ownership of all arguments. |

## Thread Safety

This function modifies internal state of the [`ut_system`](ut-system.html) and should not be
assumed to be thread-safe.

Concurrent access or modification of the same unit system should be externally
synchronized.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_unmap_symbol_to_unit()` removes a previously
defined unit symbol mapping.

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

    if (ut_unmap_symbol_to_unit(system, "my_m") != 0) {
        fprintf(stderr, "Failed to unmap symbol\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    /* Behavior of lookup after unmapping is implementation-defined */
    ut_unit* retrieved = ut_get_unit_by_symbol(system, "my_m");
    if (retrieved == NULL) {
        printf("Lookup no longer resolves the symbol (implementation-dependent)\n");
    } else {
        ut_free(retrieved);
    }

    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Lookup no longer resolves the symbol (implementation-dependent)
```

## See Also

- [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html)
- [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)
- [`ut_map_name_to_unit()`](ut-map-name-to-unit.html)
- [`ut_unmap_name_to_unit()`](ut-unmap-name-to-unit.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
