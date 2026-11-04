---
title: ut_add_symbol_prefix()
sidebar: sidebar
toc: true
permalink: ut-add-symbol-prefix.html
---

Adds a symbolic prefix and scale factor to a unit system.

## Prototype

```c
int ut_add_symbol_prefix(
    ut_system* system,
    const char* symbol,
    double value
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Adds a **symbol-based prefix** to a unit system.

This function registers a prefix identified by a short symbol (e.g., `"k"`)
and associates it with a numeric scale factor (e.g., `1000`). Once added, the
prefix can be used in unit parsing and formatting within the specified [`ut_system`](ut-system.html).

For example, after adding the prefix `"k"` with value `1e3`, expressions such as
`"km"` can be parsed as `"k"` applied to `"m"`.

The prefix represents a multiplicative scale factor and does not impose any
dimensional constraints on units.

The prefix is only recognized during parsing operations that use this specific
`ut_system` instance. Prefix definitions are not shared between systems and do
not affect parsing in other [`ut_system`](ut-system.html) instances.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `system`  | Pointer to the unit system to which the prefix will be added. Must be a valid, initialized [`ut_system`](ut-system.html) and not `NULL` |
| `symbol`  | The symbol of the prefix (e.g., `"k"`). Must be a non-NULL, non-empty string |
| `value`   | The numeric scale factor associated with the prefix (e.g., `1e3` for kilo). Must be a finite, non-zero value |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `0` | Success. The prefix was added to the system |
| `UT_EXISTS` | A prefix with the same symbol already exists in the system |
| `UT_BAD_ARG` | One or more arguments are invalid |
| `UT_OS` | A system-level error occurred (e.g., memory allocation failure) |

## Preconditions

- `system` must be a valid, initialized [`ut_system`](ut-system.html) object.
- `symbol` must be non-NULL and non-empty.
- The prefix symbol should not already exist in the system.
- `value` should be a finite, non-zero numeric scale factor.

## Error Handling

Returns a non-zero value on failure. The specific error condition can
be retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

- The `system` object remains owned by the caller.
- The function internally manages any storage associated with `symbol`; the caller retains ownership of the original string.
- No new user-managed objects are returned.

## Thread Safety

**Not thread-safe.**

This function modifies the internal state of the [`ut_system`](ut-system.html). External
synchronization is required when accessing or modifying a shared system.

## Example

This example demonstrates how `ut_add_symbol_prefix()` registers the `"k"`
prefix in a unit system, enabling parsing of prefixed units such as `"km"`.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    if (ut_add_symbol_prefix(system, "k", 1e3) != 0) {
        fprintf(stderr, "Failed to add symbol prefix\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* unit = ut_parse(system, "km", UT_UTF8);
    if (unit) {
        printf("Successfully parsed 'km'\n");
        ut_free(unit);
    } else {
        fprintf(stderr, "Failed to parse 'km'\n");
    }

    ut_free_system(system);
    return 0;
}
```

## See Also

- [`ut_add_name_prefix()`](ut-add-name-prefix.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_new_system()`](ut-new-system.html)
