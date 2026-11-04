---
title: cv_convert_float()
sidebar: sidebar
toc: true
permalink: cv-convert-float.html
---

Converts a single-precision value using a [`cv_converter`](cv-converter.html).

## Prototype

```c
float cv_convert_float(
    const cv_converter* converter, 
    float value
);
```

## Header

```c
#include <udunits2/converter.h>
```

## Description

`cv_convert_float()` converts a single `float` value using the transformation defined
by a [`cv_converter`](cv-converter.html).

The converter object is typically created with [`ut_get_converter()`](ut-get-converter.html) and defines the
mathematical transformation between two compatible units. Compatibility between units
is established at converter creation time; this function does not perform compatibility
checks.

The units used to create the converter must originate from the same [`ut_system`](ut-system.html). Units
from different systems are not interchangeable, and using a converter derived from mixed
systems results in undefined behavior.

The transformation may represent an affine relationship (scale and offset) supported by
UDUNITS-2. The function applies this transformation to the input value and returns the
converted result.

## Parameters

| Parameter   | Description                                                                           |
| ----------- | ------------------------------------------------------------------------------------- |
| `converter` | Pointer to a [`cv_converter`](cv-converter.html) defining the transformation between two compatible units |
| `value`     | The input value to convert   |

## Returns

| Return Value | Description                                                                                      |
| ------------ | ------------------------------------------------------------------------------------------------ |
| `float`      | The converted numeric value produced by applying the converter transformation to the input value |

## Preconditions

Before calling `cv_convert_float()`:

- `converter` must reference a valid [`cv_converter`](cv-converter.html) object created by UDUNITS-2 (e.g., via [`ut_get_converter()`](ut-get-converter.html)).
- The converter must not have been freed using [`cv_free()`](cv-free.html).
- The units used to create the converter must be compatible and must originate from the same [`ut_system`](ut-system.html).
- The underlying [`ut_system`](ut-system.html) associated with the converter must remain valid (i.e., it must not have been freed).

## Error Handling

This function does not distinguish errors from a valid return value.
Use [`ut_get_status()`](ut-get-status.html) to determine whether an error occurred.

## Object Ownership

`cv_convert_float()` does not allocate or return any dynamically allocated objects.

The function operates on an existing [`cv_converter`](cv-converter.html) provided by the caller and returns the converted value directly as a `float`.

| Memory Allocation | Caller Responsibility                   |
| ----------------- | --------------------------------------- |
| None              | Ensure the [`cv_converter`](cv-converter.html) remains valid |

## Thread Safety

`cv_convert_float()` performs a read-only numeric transformation using the provided
`cv_converter` and does not modify any global UDUNITS-2 state.

The function is thread-safe when used concurrently, provided that the same `cv_converter`
instance is not freed while in use by another thread. Callers are responsible for ensuring
the lifetime of the converter across threads.

## Numeric Behavior

`cv_convert_float()` applies the mathematical transformation defined by the `cv_converter`
to the input value. Conversions in UDUNITS-2 are typically affine transformations (scale
and offset).

Internally, the conversion may be performed using double-precision arithmetic, and the
result is then cast to `float`. This can introduce rounding and precision loss compared
to [`cv_convert_double()`](cv-convert-double.html).

If the input value is `NaN` or `±Inf`, the result follows standard IEEE 754 floating-point
propagation rules.

Overflow or underflow may occur depending on the magnitude of the input value and the
converter transformation. These conditions are not detected or reported by the function
and follow the standard floating-point behavior of the platform.

The function is deterministic and has no side effects: for a given converter and input value,
it will always produce the same result.

## Example

In this example, `cv_convert_float()` converts **500.0 meters to kilometers**, producing the result **0.5**.

```c
#include <stdio.h>
#include <udunits2.h>
#include <udunits2/converter.h>

int main(void) {
    /* Create and initialize a unit system */
    ut_system* system = ut_read_xml(NULL);

    /* Obtain units from the SAME system */
    ut_unit* meter = ut_get_unit_by_name(system, "meter");
    ut_unit* kilometer = ut_get_unit_by_name(system, "kilometer");

    /* Ensure units are compatible before creating a converter */
    if (!ut_are_convertible(meter, kilometer)) {
        fprintf(stderr, "Units are not convertible\n");
        return 1;
    }

    /* Create a converter (guaranteed valid for these units) */
    cv_converter* converter = ut_get_converter(meter, kilometer);

    /* Convert a value */
    float input = 500.0f;
    float result = cv_convert_float(converter, input);

    printf("%f meters = %f kilometers\n", input, result);

    /* Clean up */
    cv_free(converter);
    ut_free(meter);
    ut_free(kilometer);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`cv_convert_double()`](cv-convert-double.html)
- [`cv_convert_doubles()`](cv-convert-doubles.html)
- [`cv_convert_floats()`](cv-convert-floats.html)
- [`ut_get_converter()`](ut-get-converter.html)
- [`cv_combine()`](cv-combine.html)
- [`cv_free()`](cv-free.html)
