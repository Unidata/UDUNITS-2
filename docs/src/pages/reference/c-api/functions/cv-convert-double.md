---
title: cv_convert_double()
sidebar: sidebar
toc: true
permalink: cv-convert-double.html
---

Converts a double-precision value using a [`cv_converter`](cv-converter.html).

## Prototype

```c
double cv_convert_double(
    const cv_converter* converter,
    double value
);
```

## Header

```c
#include <udunits2/converter.h>
```

## Description

`cv_convert_double()` converts a single `double` value using the transformation defined by a [`cv_converter`](cv-converter.html).

The [`cv_converter`](cv-converter.html) is created by UDUNITS-2 (e.g., via [`ut_get_converter()`](ut-get-converter.html)) and represents the relationship between two compatible units. The transformation is affine, consisting of a scale and an optional offset (e.g., temperature conversions).

The function applies this transformation directly to the input value and returns the resulting value.

A [`cv_converter`](cv-converter.html) is immutable after creation and may be reused for multiple conversions. This function performs only the numeric transformation and does not allocate memory or modify global UDUNITS-2 state.

## Parameters

| Parameter   | Description                                                                           |
| ----------- | ------------------------------------------------------------------------------------- |
| `converter` | Pointer to a [`cv_converter`](cv-converter.html) defining the transformation between two compatible units |
| `value`     | The input value to convert   |

## Returns

| Return Value | Description                                                                                       |
| ------------ | ------------------------------------------------------------------------------------------------- |
| `double`     | The converted numeric value produced by applying the converter transformation to the input value |

## Preconditions

Before calling `cv_convert_double()`:

- `converter` must reference a valid [`cv_converter`](cv-converter.html) object created by UDUNITS-2.
- The converter must not have been freed using [`cv_free()`](cv-free.html).

## Error Handling

This function does not distinguish errors from a valid return value.
Use [`ut_get_status()`](ut-get-status.html) to determine whether an error occurred.

## Object Ownership

`cv_convert_double()` does not allocate or return any dynamically allocated objects.

The function operates on an existing [`cv_converter`](cv-converter.html) provided by the caller and returns the converted value directly as a `double`.

| Memory Allocation | Caller Responsibility                   |
| ----------------- | --------------------------------------- |
| None              | Ensure the [`cv_converter`](cv-converter.html) remains valid |

## Thread Safety

`cv_convert_double()` performs a numeric transformation and does not
modify global UDUNITS-2 state. The function is thread-safe provided that
the supplied [`cv_converter`](cv-converter.html) object is not concurrently freed or otherwise
invalidated.

## Numeric Behavior

The function applies the affine transformation defined by the converter
to the input value.

If the input value is `NaN` or `±Inf`, the result typically follows IEEE
floating-point propagation rules. Extremely large input values may
overflow depending on the converter transformation.

## Example

In this example, a value in meters is converted to kilometers using a [`cv_converter`](cv-converter.html).

```c
#include <stdio.h>
#include <udunits2.h>
#include <udunits2/converter.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        fprintf(stderr, "Failed to initialize unit system\n");
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
        fprintf(stderr, "Units are not convertible\n");
        ut_free_system(system);
        return 1;
    }

    double result = cv_convert_double(converter, 500.0);
    printf("Converted value: %f\n", result);

    cv_free(converter);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`cv_convert_doubles()`](cv-convert-doubles.html)
- [`cv_convert_float()`](cv-convert-float.html)
- [`cv_convert_floats()`](cv-convert-floats.html)
- [`ut_get_converter()`](ut-get-converter.html)
- [`cv_combine()`](cv-combine.html)
- [`cv_free()`](cv-free.html)
