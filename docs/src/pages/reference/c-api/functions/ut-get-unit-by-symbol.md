---
title: ut_get_unit_by_symbol()
sidebar: sidebar
toc: true
permalink: ut-get-unit-by-symbol.html
---

Returns a unit object corresponding to a specified symbol defined in the unit database.

## Prototype

```c
ut_unit* ut_get_unit_by_symbol(
    const ut_system* system,
    const char* symbol
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_unit_by_symbol()` retrieves a unit from a given [`ut_system`](ut-system.html) using a symbol defined in the unit database.

The function searches the unit database associated with the provided unit system for a unit whose symbol exactly matches the specified string.

Unit symbols (e.g., "m", "s", "kg") are typically defined in the UDUNITS-2 XML database and represent standardized abbreviations for units.

If a matching unit is found, the function returns a [`ut_unit`](ut-unit.html) object representing that unit.

If no matching unit exists, the function returns `NULL`.

The returned unit can be used in unit operations such as conversion, comparison, or formatting.

This function does not modify the input [`ut_system`](ut-system.html).

## Parameters

| Parameter | Description |
| -------- | ----------- |
| `system` | Unit system from which the unit should be retrieved |
| `symbol` | Null-terminated string containing the exact unit symbol to match |

## Returns

| Return Value | Description |
| ----------- | ----------- |
| `ut_unit*` | Pointer to a unit corresponding to `symbol` |
| `NULL` | A unit could not be retrieved |

## Preconditions

Before calling `ut_get_unit_by_symbol()`:

- `system` must reference a valid [`ut_system`](ut-system.html) object
- `symbol` must be a non-NULL, null-terminated string
- The unit system should be properly initialized (e.g., via [`ut_read_xml()`](ut-read-xml.html))

Passing `NULL` or invalid arguments may result in failure.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_get_unit_by_symbol()` returns a [`ut_unit`](ut-unit.html) object.

Ownership of the returned object is transferred to the caller.

The caller is responsible for releasing the unit using [`ut_free()`](ut-free.html).

The input [`ut_system`](ut-system.html) remains owned by the caller or its creator.

| Memory Allocation | Caller Responsibility |
| ---------------- | -------------------- |
| Returned object | Must call [`ut_free()`](ut-free.html) |

## Thread Safety

`ut_get_unit_by_symbol()` is thread-safe provided that:

- The input [`ut_system`](ut-system.html) is not being modified concurrently

The function performs a lookup within the unit system and does not modify it.

However, UDUNITS-2 maintains a global error status, which may be overwritten by concurrent calls. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external synchronization in multithreaded environments.

The returned [`ut_unit`](ut-unit.html) may be safely used across threads, provided its lifetime is properly managed.

## Example

This example demonstrates how `ut_get_unit_by_symbol()` retrieves a unit from a unit system using its symbol and accesses its associated symbol string.

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

    ut_unit* meter = ut_get_unit_by_symbol(system, "m");
    if (meter == NULL) {
        fprintf(stderr, "Failed to retrieve unit\n");
        ut_free_system(system);
        return 1;
    }

    const char* symbol = ut_get_symbol(meter);
    if (symbol != NULL) {
        printf("Retrieved unit symbol: %s\n", symbol);
    }

    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_get_status()`](ut-get-status.html)
- [`ut_get_name()`](ut-get-name.html)
- [`ut_get_symbol()`](ut-get-symbol.html)
- [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_free()`](ut-free.html)
- [`ut_get_system()`](ut-get-system.html)
