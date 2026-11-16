---
title: ut_raise()
sidebar: sidebar
toc: true
permalink: ut-raise.html
---

Raises a unit to an integer power, producing a derived unit.

## Prototype

```c
ut_unit* ut_raise(
    const ut_unit* unit,
    int power
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_raise()` returns a new unit that represents the input unit raised to an integer exponent:

result = unit^power

The resulting unit is a derived unit whose dimensionality and scale are computed
by applying the exponent to the input unit. For example, raising meters to the
power of 2 yields square meters (`m²`), while a negative exponent produces a
reciprocal unit (e.g., `s⁻¹`). A power of zero yields a dimensionless unit.

This is a purely algebraic operation and does not require unit conversion. The
operation is performed within the unit's associated [`ut_system`](ut-system.html).

The input unit is not modified.

The returned unit is a newly allocated object and must be freed by the caller
using [`ut_free()`](ut-free.html).

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit`    | Pointer to the unit to raise. Should be a valid, non-NULL [`ut_unit`](ut-unit.html) |
| `power`   | Integer exponent to apply (may be positive, zero, or negative) |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| non-NULL     | Pointer to a newly allocated unit representing `unit^power` |
| `NULL`       | An error occurred (invalid argument or allocation failure) |

## Preconditions

Before calling `ut_raise()`:

- `unit` should reference a valid [`ut_unit`](ut-unit.html) object.
- `unit` should belong to a valid [`ut_system`](ut-system.html).

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_raise()` creates a new unit object.

- The returned `ut_unit*` is newly allocated and owned by the caller.
- The caller is responsible for freeing the returned unit using [`ut_free()`](ut-free.html).
- The input unit (`unit`) remains owned by the caller and is not modified.

| Memory Allocation | Caller Responsibility  |
| ----------------- | --------------------- |
| Yes               | Free with [`ut_free()`](ut-free.html) |

## Thread Safety

**Conditionally thread-safe.**

This function performs a read-only algebraic operation. However, it is not safe
to call concurrently with other UDUNITS-2 functions that access or modify the
same [`ut_system`](ut-system.html).

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external
synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_raise()` creates a squared unit.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* meter = ut_parse(system, "meter", UT_UTF8);
    if (!meter) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    ut_unit* square_meter = ut_raise(meter, 2);
    if (!square_meter) {
        fprintf(stderr, "Failed to raise unit\n");
        ut_free(meter);
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(square_meter, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Resulting unit: %s\n", buffer);
    }

    ut_free(square_meter);
    ut_free(meter);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Resulting unit: m2
```

## See Also

- [`ut_multiply()`](ut-multiply.html)
- [`ut_divide()`](ut-divide.html)
- [`ut_invert()`](ut-invert.html)
- [`ut_clone()`](ut-clone.html)
