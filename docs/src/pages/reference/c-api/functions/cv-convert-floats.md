---
title: cv_convert_floats()
sidebar: sidebar
toc: true
permalink: cv-convert-floats.html
---

Converts an array of single-precision values using a [`cv_converter`](cv-converter.html).

## Prototype

```c
void cv_convert_floats(
    const cv_converter* converter,
    size_t count,
    const float* in,
    float* out
);
```

## Header

```c
#include <udunits2/converter.h>
```

## Description

`cv_convert_floats()` applies the transformation defined by a `cv_converter`
to each element of an input array of `float` values and writes the results to
an output array.

A [`cv_converter`](cv-converter.html) is typically created using [`ut_get_converter()`](ut-get-converter.html) and represents
a mathematical mapping between two units. Converters are only created for
compatible units; therefore, this function assumes that the provided converter
defines a valid transformation and performs no compatibility checks.

The transformation may represent linear scaling, affine (offset) conversions,
or other relationships supported by UDUNITS-2.

The input and output arrays may refer to the same memory region. In-place
conversion is supported.

## Parameters

| Parameter   | Description                                                                     |
| ----------- | ------------------------------------------------------------------------------- |
| `converter` | Pointer to a valid [`cv_converter`](cv-converter.html) defining the transformation between two units |
| `count`     | Number of elements to convert                                                   |
| `in`        | Pointer to the input array containing at least `count` elements                 |
| `out`       | Pointer to the output array capable of storing at least `count` elements        |

## Returns

| Return Value | Description                                                                                              |
| ------------ | -------------------------------------------------------------------------------------------------------- |
| `void`       | This function does not return a value. Converted values are written to the caller-provided output array |

## Preconditions

Before calling `cv_convert_floats()`:

- `converter` must reference a valid [`cv_converter`](cv-converter.html) object created by UDUNITS-2.
- The converter must not have been freed using [`cv_free()`](cv-free.html).
- The converter must have been created for compatible units.
- `in` must reference an array containing at least `count` elements.
- `out` must reference an array capable of storing at least `count` elements.
- If `in` and `out` overlap, they must do so safely for element-wise processing.

## Error Handling

This function does not distinguish errors from a valid return value.
Use [`ut_get_status()`](ut-get-status.html) to determine whether an error occurred.

## Object Ownership

`cv_convert_floats()` does not allocate, transfer, or release ownership of any
objects.

The caller retains ownership of:

- the [`cv_converter`](cv-converter.html) object
- the input array
- the output array

| Memory Allocation | Caller Responsibility                                                       |
| ----------------- | --------------------------------------------------------------------------- |
| None              | Ensure all inputs are valid and that the converter remains valid during use |

## Thread Safety

`cv_convert_floats()` does not modify global UDUNITS-2 state.

`cv_converter` instances are immutable after creation and may be safely shared
across threads. This function is thread-safe provided that:

- the supplied [`cv_converter`](cv-converter.html) is not freed concurrently
- the input and output arrays are not concurrently modified by other threads

## Numeric Behavior

Each input value is transformed according to the mapping defined by the
converter.

Conversions are typically performed using double-precision arithmetic internally,
with results cast to `float` before being written to the output array. This may
introduce rounding.

Special floating-point values are handled according to IEEE rules:

- `NaN` propagates to the output
- `+Inf` and `-Inf` propagate according to the transformation

Overflow or underflow may occur depending on the magnitude of the input values
and the conversion.

## Example

This example demonstrates how `cv_convert_floats()` converts an array of
single-precision values from one unit to another by applying a `cv_converter`
to each element and storing the results in an output array.

```c
#include <stdio.h>
#include <udunits2.h>
#include <udunits2/converter.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);

    ut_unit* meter = ut_parse(system, "m", UT_ASCII);
    ut_unit* kilometer = ut_parse(system, "km", UT_ASCII);

    cv_converter* converter = ut_get_converter(meter, kilometer);

    float values[] = {1000.0f, 2000.0f, 3000.0f};
    float result[3];

    cv_convert_floats(converter, 3, values, result);

    for (size_t i = 0; i < 3; i++)
        printf("%f\n", result[i]);

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
- [`cv_convert_float()`](cv-convert-float.html)
- [`ut_get_converter()`](ut-get-converter.html)
- [`cv_combine()`](cv-combine.html)
- [`cv_free()`](cv-free.html)
