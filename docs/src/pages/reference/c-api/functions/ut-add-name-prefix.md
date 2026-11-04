---
title: ut_add_name_prefix()
sidebar: sidebar
toc: true
permalink: ut-add-name-prefix.html
---

Adds a named prefix and scale factor to a unit system.

## Prototype

```c
int ut_add_name_prefix(
    ut_system* system,
    const char* name,
    double value
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Adds a **name-based prefix** to a unit system.

This function registers a prefix identified by a textual name (e.g., `"kilo"`)
and associates it with a numeric scale factor (e.g., `1000`). Once added, the
prefix can be used in unit parsing and formatting within the specified [`ut_system`](ut-system.html).

The prefix becomes part of the system’s symbol table and affects subsequent unit
parsing operations performed using that system.

The prefix represents a multiplicative scale factor and does not impose any
dimensional constraints on units.

This function modifies only the provided unit system and does not affect other systems.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `system`  | Pointer to the unit system to which the prefix will be added. Must be a valid, initialized [`ut_system`](ut-system.html) and not `NULL` |
| `name`    | The name of the prefix (e.g., `"kilo"`). Must be a non-NULL, non-empty string |
| `value`   | The numeric scale factor associated with the prefix (e.g., `1e3` for kilo) |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `0` | Success. The prefix was added to the system |
| `UT_EXISTS` | A prefix with the same name already exists in the system |
| `UT_BAD_ARG` | One or more arguments are invalid |
| `UT_OS` | A system-level error occurred (e.g., memory allocation failure) |

## Preconditions

- `system` must be a valid, initialized [`ut_system`](ut-system.html) object.
- `name` must be non-NULL and non-empty.
- The prefix name should not already exist in the system.

## Error Handling

Returns a non-zero value on failure. The specific error condition can
be retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

- The `system` object remains owned by the caller.
- The function internally manages any storage associated with `name`; the caller retains ownership of the original string.
- No new user-managed objects are returned.

## Thread Safety

**Not thread-safe.**

This function modifies the internal state of the [`ut_system`](ut-system.html). External synchronization is required when accessing or modifying a [`ut_system`](ut-system.html).

Thread safety is not guaranteed by the library.

## Example

This example demonstrates how `ut_add_name_prefix()` registers the `"kilo"` prefix in a unit system, enabling the parsing of prefixed units such as `"kilometer"`.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    if (ut_add_name_prefix(system, "kilo", 1e3) != 0) {
        fprintf(stderr, "Failed to add prefix\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* unit = ut_parse(system, "kilometer", UT_UTF8);
    if (unit) {
        printf("Successfully parsed 'kilometer'\n");
        ut_free(unit);
    } else {
        fprintf(stderr, "Failed to parse 'kilometer'\n");
    }

    ut_free_system(system);
    return 0;
}
```

## See Also

- [`ut_add_symbol_prefix()`](ut-add-symbol-prefix.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_new_system()`](ut-new-system.html)
