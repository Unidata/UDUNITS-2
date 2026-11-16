---
title: ut_free_system()
sidebar: sidebar
toc: true
permalink: ut-free-system.html
---

Releases a unit system and all associated resources.

## Prototype

```c
void ut_free_system(
    ut_system* system
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_free_system()` releases a [`ut_system`](ut-system.html) object and all resources
associated with it.

A [`ut_system`](ut-system.html) represents a complete unit system, including all units,
prefixes, and internal data structures (such as unit registries, prefix
tables, and related system metadata) created either programmatically
(e.g., via [`ut_new_system()`](ut-new-system.html) or returned by functions such as
[`ut_read_xml()`](ut-read-xml.html).

As part of system teardown, unit objects managed by the system are
released through its internal data structures. Because UDUNITS-2 may use
reference counting for [`ut_unit`](ut-unit.html) objects, units are fully deallocated
only when no remaining references exist.

After calling this function, the [`ut_system`](ut-system.html) and any objects obtained
from it (such as `ut_unit`) must not be used.

This function should be called when a unit system is no longer needed to
ensure that all associated memory is released.

## Parameters

| Parameter | Description            |
| --------- | ---------------------- |
| `system`  | Unit system to release |

## Error Handling

This function does not report errors. The behavior is undefined if
an invalid argument is supplied.

## Preconditions

Before calling `ut_free_system()`:

- `system` must not be `NULL`. Behavior is undefined if `system == NULL`
   unless documented otherwise.
- `system` must reference a valid [`ut_system`](ut-system.html) object created by UDUNITS-2
  (e.g., via [`ut_new_system()`](ut-new-system.html) or [`ut_read_xml()`](ut-read-xml.html)).
- The unit system must not have already been freed.
- No other part of the program should continue to use the `ut_system`
  object or any [`ut_unit`](ut-unit.html) objects associated with it after this call.
- The [`ut_system`](ut-system.html) must not be accessed concurrently by other threads.
  The caller must ensure no concurrent access occurs.

Once `ut_free_system()` is called, all pointers to the system and its
associated units become invalid.

## Object Ownership

`ut_free_system()` releases a [`ut_system`](ut-system.html) object and all resources owned
by that system.

This includes all [`ut_unit`](ut-unit.html) objects and internal structures associated
with the unit system. The function does not require the caller to
individually free units that are owned by the system.  Units may use
internal reference counting; destruction occurs when no references remain.

After calling `ut_free_system()`, ownership of the system and all
associated objects is relinquished, and their memory is released.

Any pointers to the [`ut_system`](ut-system.html) or to [`ut_unit`](ut-unit.html) objects obtained from the
system become invalid and must not be used.

| Memory Allocation | Caller Responsibility                                                                   |
| ----------------- | --------------------------------------------------------------------------------------- |
| [`ut_system`](ut-system.html)       | Release with `ut_free_system()` when no longer needed                                   |
| [`ut_unit`](ut-unit.html)         | Lifetime is managed by the associated `ut_system`; do not use after the system is freed |

## Thread Safety

`ut_free_system()` deallocates a [`ut_system`](ut-system.html) object and its associated resources.

This function is **not thread-safe with respect to the provided system**.  
Concurrent access to the same [`ut_system`](ut-system.html) or any [`ut_unit`](ut-unit.html) objects derived
from it while the system is being freed results in undefined behavior.

The caller is responsible for ensuring that no other threads are accessing
the [`ut_system`](ut-system.html) or its associated objects at the time `ut_free_system()` is called.

UDUNITS-2 maintains a global error state accessible via [`ut_get_status()`](ut-get-status.html).  
This function does not report errors or modify that state.

## Example

This example demonstrates how to create a unit system, retrieve a unit,
and release all associated resources using `ut_free_system()` when the
system is no longer needed.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    /* Load the default UDUNITS-2 XML database */
    ut_system* system = ut_read_xml(NULL);

    if (system == NULL) {
        fprintf(stderr, "Failed to load unit system\n");
        return 1;
    }

    /* Retrieve a unit from the system */
    ut_unit* meter = ut_get_unit_by_name(system, "meter");

    if (meter == NULL) {
        fprintf(stderr, "Failed to retrieve unit\n");
        ut_free_system(system);
        return 1;
    }

    printf("Successfully retrieved unit: meter\n");

    /* Release the unit system and all associated units */
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Successfully retrieved unit: meter
```

## See Also

- [`ut_new_system()`](ut-new-system.html)
- [`ut_read_xml()`](ut-read-xml.html)
