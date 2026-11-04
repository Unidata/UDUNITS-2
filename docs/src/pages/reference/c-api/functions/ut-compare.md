---
title: ut_compare()
sidebar: sidebar
toc: true
permalink: ut-compare.html
---

Compares two units for ordering and structural equality.

## Prototype

``` c
int ut_compare(
    const ut_unit* unit1,
    const ut_unit* unit2
);
```

## Header

``` c
#include <udunits2.h>
```

## Description

Compares two units and determines their relative ordering based on their
internal representations.

This function defines an ordering over [`ut_unit`](ut-unit.html) objects suitable for
comparison and sorting operations. The comparison is based on the
internal canonical representation of the units, including their
structure, dimensionality, and scaling.

Two units are considered equal only if their internal representations
are identical. Units that are merely convertible (e.g., meters and
kilometers) are not considered equal.

The ordering is implementation-defined and should not be interpreted as
having physical meaning.

This function is typically used for sorting or equality checks rather
than conversion; to determine convertibility, use
[`ut_are_convertible()`](ut-are-convertible.html).

The function does not modify either unit.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit1`   | Pointer to the first unit to compare. Must be a valid, non-NULL [`ut_unit`](ut-unit.html) |
| `unit2`   | Pointer to the second unit to compare. Must be a valid, non-NULL [`ut_unit`](ut-unit.html) |

## Returns

| Return Value | Description                                      |
| ------------ | ------------------------------------------------ |
| `< 0`        | `unit1` is less than `unit2`   |
| `0`          | `unit1` and `unit2` are structurally equal   |
| `> 0`        | `unit1` is greater than `unit2`   |

## Preconditions

- `unit1` must be a valid, initialized [`ut_unit`](ut-unit.html) object.
- `unit2` must be a valid, initialized [`ut_unit`](ut-unit.html) object.
- Neither pointer may be `NULL`.

## Error Handling

This function does not distinguish errors from a valid comparison result.
Use [`ut_get_status()`](ut-get-status.html) to determine whether an error occurred.

## Object Ownership

- Both `unit1` and `unit2` remain owned by the caller.
- The function does not allocate, modify, or free any `ut_unit`
    objects.
- No new objects are created.

## Thread Safety

Thread-safe for read-only use, provided that the referenced [`ut_unit`](ut-unit.html)
objects are not being modified concurrently.

## Example

This example demonstrates how `ut_compare()` compares two units. The
ordering relationship is implementation-defined.

``` c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* unit1 = ut_parse(system, "meter", UT_UTF8);
    ut_unit* unit2 = ut_parse(system, "kilometer", UT_UTF8);

    if (!unit1 || !unit2) {
        fprintf(stderr, "Failed to parse units\n");
        ut_free(unit1);
        ut_free(unit2);
        ut_free_system(system);
        return 1;
    }

    int cmp = ut_compare(unit1, unit2);

    if (cmp == 0) {
        printf("Units are structurally equal\n");
    } else if (cmp < 0) {
        printf("unit1 < unit2\n");
    } else {
        printf("unit1 > unit2\n");
    }

    ut_free(unit1);
    ut_free(unit2);
    ut_free_system(system);
    return 0;
}
```

## See Also

- [`ut_are_convertible()`](ut-are-convertible.html)
- [`ut_accept_visitor()`](ut-accept-visitor.html)
