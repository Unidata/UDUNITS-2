---
title: ut_unmap_unit_to_symbol()
sidebar: sidebar
toc: true
permalink: ut-unmap-unit-to-symbol.html
---

Removes an association between a unit and a textual symbol within a unit system.

## Prototype

```c
int ut_unmap_unit_to_symbol(
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

`ut_unmap_unit_to_symbol()` removes a previously established association between
a [`ut_unit`](ut-unit.html) and a textual symbol within a unit system.

This function updates the internal unit-to-symbol mapping of the specified
[`ut_system`](ut-system.html), removing the association between the provided unit and symbol.

**Key Semantics:**

- The operation applies only to the specified [`ut_system`](ut-system.html).
- The function removes the association between the provided unit and symbol.
- The behavior when the association does not exist is implementation-defined.
- The function does not modify the provided [`ut_unit`](ut-unit.html).
- The internal representation and storage of mappings are not specified by the API.
- This function complements [`ut_map_unit_to_symbol()`](ut-map-unit-to-symbol.html) but does not guarantee modification of symbol-to-unit mappings created via [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html).

## Parameters

| Parameter | Description                                                                   |
| --------- | ----------------------------------------------------------------------------- |
| `system`  | Pointer to a valid [`ut_system`](ut-system.html) object.                                        |
| `unit`    | Pointer to the [`ut_unit`](ut-unit.html) whose associated symbol should be removed.           |
| `symbol`  | Null-terminated string representing the symbol to disassociate from the unit. |

## Returns

| Return Value | Description                                           |
| ------------ | ----------------------------------------------------- |
| `0`          | Success. The operation completed successfully.        |
| non-zero     | Failure. The operation did not complete successfully. |

## Preconditions

Before calling `ut_unmap_unit_to_symbol()`:

- `system` should reference a valid [`ut_system`](ut-system.html) object.
- `unit` should reference a valid [`ut_unit`](ut-unit.html) associated with the same system.
- `symbol` should be a non-`NULL`, null-terminated string.

## Error Handling

Returns a non-zero value on failure. The specific error condition can be retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_unmap_unit_to_symbol()` does not create, destroy, or modify any objects.

The function does not transfer ownership; the caller retains responsibility for managing the lifetime of `system`, `unit`, and `symbol`.

| Memory Allocation | Caller Responsibility                  |
|------------------|----------------------------------------|
| None             | Caller retains ownership of all arguments |

## Thread Safety

This function modifies internal state of the [`ut_system`](ut-system.html) and should not be assumed to be thread-safe.

Concurrent access or modification of the same unit system should be externally synchronized.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_unmap_unit_to_symbol()` removes a previously defined unit-to-symbol association.

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

    if (ut_unmap_unit_to_symbol(system, meter, "my_m") != 0) {
        fprintf(stderr, "Failed to unmap unit-to-symbol\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    /* Lookup behavior after unmapping is implementation-dependent and
     * may still resolve if a symbol-to-unit mapping exists */
    ut_unit* retrieved = ut_get_unit_by_symbol(system, "my_m");
    if (retrieved == NULL) {
        printf("Symbol association removed (implementation-dependent)\n");
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
Symbol association removed (implementation-dependent)
```

## See Also

- [`ut_map_unit_to_symbol()`](ut-map-unit-to-symbol.html)
- [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html)
- [`ut_unmap_symbol_to_unit()`](ut-unmap-symbol-to-unit.html)
- [`ut_get_unit_by_symbol()`](ut-get-unit-by-symbol.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
