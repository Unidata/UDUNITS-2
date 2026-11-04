---
title: ut_get_symbol()
sidebar: sidebar
toc: true
permalink: ut-get-symbol.html
---

Returns the canonical symbol of a unit, if available.

## Prototype

```c
const char* ut_get_symbol(
    const ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_symbol()` returns the canonical symbol associated with a given [`ut_unit`](ut-unit.html), if such a symbol exists.

In UDUNITS-2, units defined in the unit database may have associated symbols. Base and named units (e.g., "m" for meter, "s" for second) commonly define symbols, while derived or composite units may not have a canonical symbol.

If the unit has a defined symbol, the function returns a pointer to a null-terminated string representing that symbol.

If the unit does not have a symbol, the function returns `NULL`.

The returned string is managed internally by the UDUNITS-2 library and must be treated as read-only.

This function does not modify the input unit and does not create or duplicate any objects.

## Parameters

| Parameter | Description                                      |
| --------- | ------------------------------------------------ |
| `unit`    | Unit whose canonical symbol should be retrieved  |

## Returns

| Return Value  | Description                            |
| ------------- | -------------------------------------- |
| `const char*` | Pointer to the unit's canonical symbol |
| `NULL`        | The unit has no associated symbol      |

## Preconditions

Before calling `ut_get_symbol()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html) object obtained from the UDUNITS-2 library.

## Error Handling

This function does not have defined error-reporting behavior.

A `NULL` return value does not necessarily indicate an error; it may simply indicate that the unit has no canonical symbol.

## Object Ownership

`ut_get_symbol()` returns a pointer to an internal string representing the unit's canonical symbol.

The function does not allocate or copy the string.

Ownership of the returned string remains with the UDUNITS-2 library.

The caller must:

- Treat the returned string as read-only
- Not attempt to modify or free it

The returned pointer remains valid as long as the associated unit and its underlying unit system remain valid.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| None              | Do not modify or free |

## Thread Safety

`ut_get_symbol()` is a read-only accessor and does not modify the provided [`ut_unit`](ut-unit.html).

The function is safe to call concurrently from multiple threads, provided that:

- The [`ut_unit`](ut-unit.html) and its associated [`ut_system`](ut-system.html) are not being modified

The returned string is immutable and may be safely shared across threads as long as the underlying unit remains valid.

As with other UDUNITS-2 functions, the global error status may be overwritten by concurrent calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external synchronization.

## Example

This example demonstrates how `ut_get_symbol()` retrieves the canonical symbol of a unit.

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

    const char* symbol = ut_get_symbol(meter);

    if (symbol != NULL) {
        printf("Unit symbol: %s\n", symbol);
    } else {
        printf("Unit has no canonical symbol\n");
    }

    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Unit symbol: m
```

## See Also

- [`ut_get_name()`](ut-get-name.html)
- [`ut_format()`](ut-format.html)
- [`ut_get_system()`](ut-get-system.html)
- [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
