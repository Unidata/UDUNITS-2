---
title: ut_new_base_unit()
sidebar: sidebar
toc: true
permalink: ut-new-base-unit.html
---

Creates a new base unit in a unit system.

## Prototype

```c
ut_unit* ut_new_base_unit(
    ut_system* system
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_new_base_unit()` creates and returns a new base unit associated with
the specified unit system.

A base unit is a fundamental unit within a [`ut_system`](ut-system.html) that is not
derived from other units. Instead, it introduces a new base dimension
into the system. All derived units are ultimately expressed in terms of
base units.

The newly created unit has no assigned name or symbol. These can be set
after creation using `ut_set_name()` and `ut_set_symbol()`.

The returned unit becomes part of the given [`ut_system`](ut-system.html) and participates
in all subsequent unit operations within that system.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `system`  | Pointer to the [`ut_system`](ut-system.html) in which the base unit will be created. Should reference a valid unit system. |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `ut_unit*`   | Newly created base unit associated with the given system |
| `NULL`       | Failure (e.g., memory allocation failure or internal error) |

## Preconditions

Before calling `ut_new_base_unit()`:

- `system` should reference a valid [`ut_system`](ut-system.html) object.
- The [`ut_system`](ut-system.html) should be properly initialized (e.g., via
    [`ut_new_system()`](ut-new-system.html) or [`ut_read_xml()`](ut-read-xml.html)).

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_new_base_unit()` creates and returns a new [`ut_unit`](ut-unit.html) object that is
owned by the specified [`ut_system`](ut-system.html).

The caller does not take ownership of the returned unit and must not
free it directly. The unit is automatically destroyed when the
associated [`ut_system`](ut-system.html) is freed.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| Yes | Do not free directly; freed with [`ut_system`](ut-system.html) |

## Thread Safety

`ut_new_base_unit()` modifies the state of the provided [`ut_system`](ut-system.html) by
adding a new unit and is not thread-safe when the same [`ut_system`](ut-system.html) is
accessed concurrently.

UDUNITS-2 maintains a global error status that may be overwritten by
concurrent calls to other library functions. Applications that rely on
[`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure
deterministic behavior.

## Example

This example demonstrates how `ut_new_base_unit()` creates a new base
unit and assigns it a name and symbol.

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

    printf("Created base unit: widget (wdg)\n");

    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Created base unit: widget (wdg)
```

## See Also

- [`ut_new_system()`](ut-new-system.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_set_name()`](ut-set-name.html)
- [`ut_set_symbol()`](ut-set-symbol.html)
- [`ut_get_status()`](ut-get-status.html)
