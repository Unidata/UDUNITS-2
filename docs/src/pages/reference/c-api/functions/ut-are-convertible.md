---
title: ut_are_convertible()
sidebar: sidebar
toc: true
permalink: ut-are-convertible.html
---

Determines whether two units are convertible.

## Prototype

```c
int ut_are_convertible(
    const ut_unit* unit1,
    const ut_unit* unit2
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Determines whether two units are **dimensionally compatible** and can be converted between one another.

Two units are considered convertible if they represent the same physical quantity (i.e., have equivalent dimensionality), even if their scale factors differ (e.g., meters and kilometers).

This function performs a compatibility check only—it does **not** create a converter or perform any numeric conversion.

The comparison is strictly based on unit dimensionality. Units should originate from the same `ut_system`; behavior is undefined if they do not.

The function does not modify either unit and has no side effects.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit1`   | Pointer to the first unit. Must be a valid [`ut_unit`](ut-unit.html) and not `NULL` |
| `unit2`   | Pointer to the second unit. Must be a valid [`ut_unit`](ut-unit.html) and not `NULL` |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `1`          | The units are convertible (dimensionally compatible) |
| `0`          | The units are not convertible, or one or both arguments are invalid. No distinction is made between these cases |

## Preconditions

- `unit1` and `unit2` must be valid, initialized [`ut_unit`](ut-unit.html) objects.
- Units should originate from the same `ut_system`; behavior is undefined if they do not.
- - `unit1` and `unit2` should not be `NULL`.

## Error Handling

This function does not distinguish between an error and a “`false`” result.
Use [`ut_get_status()`](ut-get-status.html) to determine whether an error occurred.

## Object Ownership

- The function does not take ownership of `unit1` or `unit2`.
- No new objects are created.
- The caller retains full responsibility for managing both units.

## Thread Safety

This function is safe for concurrent use provided that the referenced units and their underlying system are not modified concurrently.

Thread safety is not guaranteed by the library.

## Example

This example demonstrates how `ut_are_convertible()` checks whether two units, meters and kilometers, are compatible for conversion.

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
    ut_unit* kilometer = ut_parse(system, "kilometer", UT_UTF8);

    if (!meter || !kilometer) {
        fprintf(stderr, "Failed to parse units\n");
        ut_free(meter);
        ut_free(kilometer);
        ut_free_system(system);
        return 1;
    }

    if (ut_are_convertible(meter, kilometer)) {
        printf("Units are convertible\n");
    } else {
        printf("Units are NOT convertible\n");
    }

    ut_free(meter);
    ut_free(kilometer);
    ut_free_system(system);
    return 0;
}
```

## See Also

- [`ut_get_converter()`](ut-get-converter.html)
- [`ut_compare()`](ut-compare.html)
