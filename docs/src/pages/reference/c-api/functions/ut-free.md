---
title: ut_free()
sidebar: sidebar
toc: true
permalink: ut-free.html
---

Frees a unit object previously allocated by the UDUNITS-2 library.

## Prototype

```c
void ut_free(
    ut_unit* unit
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_free()` releases the memory associated with a [`ut_unit`](ut-unit.html) object.

The function should be used to destroy unit objects that were created or
returned by UDUNITS-2 functions (e.g., parsing, arithmetic, or
transformation operations). Once freed, the pointer must not be reused.

The function releases the provided object and does not return a value.
Passing a `NULL` pointer is safe and results in no action.

## Parameters

| Parameter | Description                                                 |
| --------- | ----------------------------------------------------------- |
| `unit`    | Pointer to the [`ut_unit`](ut-unit.html) object to be freed. May be `NULL`. |

## Returns

| Return Value | Description                            |
| ------------ | -------------------------------------- |
| `void`       | This function does not return a value. |

## Preconditions

Before calling `ut_free()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html) object obtained from the
    UDUNITS-2 API, or be `NULL`.
- The object should not have already been freed.

## Error Handling

This function does not fail and does not set the UDUNITS-2 error status.

## Object Ownership

`ut_free()` destroys the provided [`ut_unit`](ut-unit.html) object.

Only [`ut_unit`](ut-unit.html) objects owned by the caller should be passed to this
function. Any owned unit should be released using `ut_free()` when no
longer needed.

| Memory Allocation | Caller Responsibility          |
| ----------------- | ------------------------------ |
| Yes               | Call `ut_free(unit)` when done |

## Thread Safety

`ut_free()` operates on the provided object.

UDUNITS-2 maintains a global error status that may be overwritten by
concurrent calls to other library functions. Applications that rely on
[`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure
deterministic behavior.

## Example

This example demonstrates how to create a unit and properly release it
using `ut_free()`.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* unit = ut_parse(system, "m/s", UT_ASCII);
    if (unit == NULL) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    /* Use the unit (omitted) */

    ut_free(unit);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_parse()`](ut-parse.html)
- [`ut_clone()`](ut-clone.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_get_status()`](ut-get-status.html)
