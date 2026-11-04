---
title: ut_new_system()
sidebar: sidebar
toc: true
permalink: ut-new-system.html
---

Creates a new empty unit system.

## Prototype

```c
ut_system* ut_new_system(
    void
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_new_system()` creates and returns a new, empty unit system.

A [`ut_system`](ut-system.html) is a container that manages units, prefixes, and the
relationships between them. The system returned by this function is
initially empty and contains no predefined units or prefixes. It can be
populated programmatically (e.g., via [`ut_new_base_unit()`](ut-new-base-unit.html)) or used as a
target for loading unit definitions from external sources.

Each [`ut_unit`](ut-unit.html) is associated with exactly one [`ut_system`](ut-system.html). Units from
different systems are not interchangeable and must not be used together
in unit operations. All related units must be created within the same
`ut_system`.

The caller is responsible for releasing the returned system and all
associated resources by calling [`ut_free_system()`](ut-free-system.html) when it is no longer
needed.

## Returns

| Return Value | Description                     |
| ------------ | ------------------------------- |
| `ut_system*` | Newly created empty unit system |
| `NULL`       | Memory allocation failure       |

## Preconditions

Before calling `ut_new_system()`:

- The UDUNITS-2 library must be properly linked and available to the calling program.
- Sufficient system memory must be available to allocate a new [`ut_system`](ut-system.html) object.

No unit database or external resources are required. The function creates an empty unit system that can be populated programmatically or used as a basis for defining new units.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_new_system()` allocates and returns a new [`ut_system`](ut-system.html) object.

The caller assumes ownership of the returned [`ut_system`](ut-system.html) and is
responsible for releasing it by calling [`ut_free_system()`](ut-free-system.html) when it is no
longer needed.

All [`ut_unit`](ut-unit.html) objects created within a system are owned and managed by
that [`ut_system`](ut-system.html). These units must not be freed directly by the caller;
they are automatically destroyed when the system is freed.

Freeing a [`ut_system`](ut-system.html) releases all associated units and internal
resources.

| Resource     | Ownership            | Caller Responsibility                     |
| ------------ | -------------------- | ----------------------------------------- |
| [`ut_system`](ut-system.html)  | Caller-owned         | Call [`ut_free_system()`](ut-free-system.html)                   |
| [`ut_unit`](ut-unit.html)    | Owned by [`ut_system`](ut-system.html) | Do not free directly (freed with system)  |

## Thread Safety

`ut_new_system()` creates a new [`ut_system`](ut-system.html) object without modifying any
existing unit systems and can be called independently.

However, UDUNITS-2 maintains global state for error reporting via
[`ut_get_status()`](ut-get-status.html), which is not thread-local. Concurrent calls to
UDUNITS-2 functions may overwrite this global status value.

Additionally, the UDUNITS-2 library does not guarantee full thread safety
for all internal operations. Applications that require thread safety
should use external synchronization when invoking UDUNITS-2 functions.

## Example

This example demonstrates how `ut_new_system()` creates an empty UDUNITS-2 unit system and then defines a custom base unit within that system.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system = ut_new_system();

    if (system == NULL) {
        fprintf(stderr, "Failed to create unit system\n");
        return 1;
    }

    ut_unit* widget = ut_new_base_unit(system);

    if (widget == NULL) {
        fprintf(stderr, "Failed to create base unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_set_symbol(widget, "wdg");
    ut_set_name(widget, "widget");

    printf("Created a custom base unit in a new unit system\n");

    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Created a custom base unit in a new unit system
```

## See Also

- [`ut_free_system()`](ut-free-system.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_set_path_xml()`](ut-set-path-xml.html)
- [`ut_get_path_xml()`](ut-get-path-xml.html)
