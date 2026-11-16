---
title: ut_get_unit_by_name()
sidebar: sidebar
toc: true
permalink: ut-get-unit-by-name.html
---

Returns a unit object corresponding to a specified name defined in the unit database.

## Prototype

```c
ut_unit* ut_get_unit_by_name(
    const ut_system* system,
    const char* name
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_unit_by_name()` retrieves a unit from a given [`ut_system`](ut-system.html) using
a name defined in the unit database.

The function searches the unit database associated with the provided
unit system for a unit whose name matches the specified string.

Named units (e.g., `"meter"`, `"second"`) are typically defined in the
UDUNITS-2 XML database and can be retrieved using this function.

If a matching unit is found, the function returns a [`ut_unit`](ut-unit.html) object
representing that unit.

If no matching unit exists, the function returns `NULL`.

The returned unit can be used in unit operations such as conversion,
comparison, or formatting.

This function does not modify the input [`ut_system`](ut-system.html).

## Parameters

| Parameter | Description                                               |
| --------- | --------------------------------------------------------- |
| `system`  | Unit system from which the unit should be retrieved       |
| `name`    | Null-terminated string containing the unit name           |

## Returns

| Return Value | Description                           |
| ------------ | ------------------------------------- |
| `ut_unit*`   | Pointer to a unit corresponding to `name` |
| `NULL`       | A unit could not be retrieved         |

## Preconditions

Before calling `ut_get_unit_by_name()`:

- `system` must reference a valid [`ut_system`](ut-system.html) object
- `name` must be a non-NULL, null-terminated string
- The unit system should be properly initialized (e.g., via [`ut_read_xml()`](ut-read-xml.html))

Passing `NULL` or invalid arguments may result in failure.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_get_unit_by_name()` returns a [`ut_unit`](ut-unit.html) object.

Ownership of the returned object is transferred to the caller.

The caller is responsible for releasing the unit using [`ut_free()`](ut-free.html).

The input [`ut_system`](ut-system.html) remains owned by the caller or its creator.

| Memory Allocation | Caller Responsibility |
| ---------------- | -------------------- |
| Returned object | Must call [`ut_free()`](ut-free.html) |

## Thread Safety

`ut_get_unit_by_name()` is thread-safe provided that:

- The input [`ut_system`](ut-system.html) is not being modified concurrently

The function performs a lookup within the unit system and does not modify
it.

However, UDUNITS-2 maintains a **global error status**, which may be
overwritten by concurrent calls. Applications that rely on
[`ut_get_status()`](ut-get-status.html) should use external synchronization in multithreaded
environments.

The returned [`ut_unit`](ut-unit.html) may be safely used across threads, provided its
lifetime is properly managed.

## Example

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
        fprintf(stderr, "Failed to retrieve unit\n");
        ut_free_system(system);
        return 1;
    }

    const char* name = ut_get_name(meter);
    if (name != NULL) {
        printf("Retrieved unit: %s\n", name);
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
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_free()`](ut-free.html)
- [`ut_get_system()`](ut-get-system.html)
