---
title: ut_get_converter()
sidebar: sidebar
toc: true
permalink: ut-get-converter.html
---

Returns a converter object for transforming numeric values between two compatible units.

## Prototype

```c
cv_converter* ut_get_converter(
    const ut_unit* from,
    const ut_unit* to
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_converter()` creates a converter object that transforms numeric
values from one unit (`from`) to another (`to`).

The function verifies that the two units are **dimensionally compatible**,
meaning they represent the same physical quantity (e.g., length to length,
time to time).

Both units must belong to the same [`ut_system`](ut-system.html). Units from different
systems are not compatible and cannot be converted.

If the units are compatible, a [`cv_converter`](cv-converter.html) is returned that encodes the
required transformation.

Conversions may involve:

- **Scaling** (e.g., meters to kilometers)
- **Offset adjustments** (e.g., Celsius to Kelvin)
- A combination of both

The returned converter can be used with functions such as:

- [`cv_convert_double()`](cv-convert-double.html)
- [`cv_convert_float()`](cv-convert-float.html)

to transform numeric values.

If the units are not compatible, the function returns `NULL`.

This function does not modify the input units and does not take ownership
of them.

## Parameters

| Parameter | Description                                        |
| --------- | -------------------------------------------------- |
| `from`    | Source unit from which values will be converted    |
| `to`      | Destination unit to which values will be converted |

## Returns

| Return Value    | Description                                                               |
| --------------- | ------------------------------------------------------------------------- |
| `cv_converter*` | Pointer to a converter object for transforming values from `from` to `to` |
| `NULL`          | Units are incompatible, invalid, or an error occurred                     |

## Preconditions

Before calling `ut_get_converter()`:

- `from` and `to` must reference valid [`ut_unit`](ut-unit.html) objects.
- Both units must belong to the **same [`ut_system`](ut-system.html)**.
- The units must be dimensionally compatible for conversion to succeed.

Passing `NULL`, incompatible units, or units from different systems will
cause the function to fail.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_get_converter()` allocates a new [`cv_converter`](cv-converter.html) object.

Ownership of the returned object is transferred to the caller.

The caller is responsible for releasing the converter using [`cv_free()`](cv-free.html).

The input units (`from`, `to`) are not modified and remain owned by the caller
or their originating unit system.

| Memory Allocation | Caller Responsibility |
| ---------------- | -------------------- |
| Allocated | Must call [`cv_free()`](cv-free.html) |

## Thread Safety

`ut_get_converter()` is thread-safe provided that:

- The input [`ut_unit`](ut-unit.html) objects are not being modified concurrently
- The associated [`ut_system`](ut-system.html) is not being modified

The function does not modify its input parameters.

However, UDUNITS-2 maintains a **global error status**, which may be
overwritten by concurrent calls. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization in multithreaded environments.

The returned [`cv_converter`](cv-converter.html) is immutable after creation and can be safely
used across threads, provided that its lifetime is properly managed and
destruction is synchronized.

## Example

This example demonstrates how `ut_get_converter()` creates a converter
to transform numeric values from one unit to another (meters to kilometers),
and then uses that converter to perform the conversion.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        fprintf(stderr, "Failed to load UDUNITS-2 database\n");
        return 1;
    }

    ut_unit* meter = ut_get_unit_by_name(system, "meter");
    ut_unit* kilometer = ut_get_unit_by_name(system, "kilometer");

    if (meter == NULL || kilometer == NULL) {
        fprintf(stderr, "Failed to retrieve units\n");
        ut_free_system(system);
        return 1;
    }

    cv_converter* converter = ut_get_converter(meter, kilometer);
    if (converter == NULL) {
        fprintf(stderr, "Failed to create converter\n");
        ut_free(meter);
        ut_free(kilometer);
        ut_free_system(system);
        return 1;
    }

    double meters = 1500.0;
    double km = cv_convert_double(converter, meters);

    printf("%f meters = %f kilometers\n", meters, km);

    /* Clean up */
    cv_free(converter);
    ut_free(meter);
    ut_free(kilometer);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_are_convertible()`](ut-are-convertible.html)
- [`cv_free()`](cv-free.html)
- [`cv_convert_double()`](cv-convert-double.html)
- [`cv_convert_doubles()`](cv-convert-doubles.html)
- [`cv_convert_float()`](cv-convert-float.html)
- [`cv_convert_floats()`](cv-convert-floats.html)
- [`cv_combine()`](cv-combine.html)
