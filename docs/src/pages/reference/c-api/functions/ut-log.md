---
title: ut_log()
sidebar: sidebar
toc: true
permalink: ut-log.html
---

Creates a logarithmic unit relative to a reference unit using a specified base.

## Prototype

```c
ut_unit* ut_log(
    double base,
    const ut_unit* reference
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_log()` returns a new unit that represents a logarithmic relationship relative
to a reference unit using the specified base.

The resulting unit expresses values on a logarithmic scale referenced to the
given unit. This is commonly used to construct logarithmic units (e.g.,
decibel-like units) based on a reference quantity.

The resulting unit represents logarithmic quantities relative to the reference
unit rather than a direct transformation of the unit itself.

This operation creates a logarithmic unit derived from the reference unit and
does not modify the input unit.

The returned unit is a newly allocated object and must be freed by the caller
using [`ut_free()`](ut-free.html).

## Parameters

| Parameter   | Description                                                                  |
| ----------- | ---------------------------------------------------------------------------- |
| `base`      | The logarithmic base (e.g., 10 for common logarithms, `M_E` for natural log) |
| `reference` | Pointer to the reference unit. Should be a valid, non-NULL [`ut_unit`](ut-unit.html)         |

## Returns

| Return Value | Description                                                       |
| ------------ | ----------------------------------------------------------------- |
| non-NULL     | Pointer to a newly allocated logarithmic unit                     |
| `NULL`       | An error occurred (invalid argument or memory allocation failure) |

## Preconditions

Before calling `ut_log()`:

- `reference` should reference a valid [`ut_unit`](ut-unit.html) object.
- `reference` should belong to a valid [`ut_system`](ut-system.html).
- `base` should typically be positive and not equal to 1.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_log()` creates a new unit object.

- The returned `ut_unit*` is newly allocated and owned by the caller.
- The caller is responsible for freeing the returned unit using [`ut_free()`](ut-free.html).
- The input unit (`reference`) remains owned by the caller and is not modified.

| Memory Allocation | Caller Responsibility  |
| ----------------- | --------------------- |
| Yes               | Free with [`ut_free()`](ut-free.html) |

## Thread Safety

**Not thread-safe.**

This function is not thread-safe when used with a shared [`ut_system`](ut-system.html).

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_log()` creates a logarithmic unit.

```c
#include <stdio.h>
#include <udunits2.h>
#include <math.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* watt = ut_parse(system, "watt", UT_UTF8);
    if (!watt) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* log_unit = ut_log(10.0, watt);
    if (!log_unit) {
        fprintf(stderr, "Failed to create logarithmic unit\n");
        ut_free(watt);
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(log_unit, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Logarithmic unit: %s\n", buffer);
    }

    ut_free(log_unit);
    ut_free(watt);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_scale()`](ut-scale.html)
- [`ut_offset()`](ut-offset.html)
- [`ut_multiply()`](ut-multiply.html)
- [`ut_divide()`](ut-divide.html)
- [`ut_invert()`](ut-invert.html)
