---
title: cv_convert_doubles()
sidebar: sidebar
toc: true
permalink: cv-convert-doubles.html
---

Converts an array of double-precision values using a [`cv_converter`](cv-converter.html).

## Prototype

```c
void cv_convert_doubles(
    const cv_converter* converter,
    size_t count,
    const double* in,
    double* out
);
```

## Header

```c
#include <udunits2/converter.h>
```

## Description

`cv_convert_doubles()` converts an array of double-precision values using the
transformation defined by a [`cv_converter`](cv-converter.html).

The converter object is typically created with [`ut_get_converter()`](ut-get-converter.html) and defines
an affine transformation (scale and offset) between two units. The function
applies this transformation independently to each element of the input array and
writes the converted values to the output array.

The [`cv_converter`](cv-converter.html) must have been successfully created using [`ut_get_converter()`](ut-get-converter.html)
from two compatible units. If the units are not compatible, a converter cannot be
created.

The units used to create the [`cv_converter`](cv-converter.html) must belong to the same [`ut_system`](ut-system.html).
Units from different systems are not compatible and must not be used together
when creating converters.

## Parameters

| Parameter   | Description                                                             |
| ----------- | ----------------------------------------------------------------------- |
| `converter` | Pointer to a valid [`cv_converter`](cv-converter.html)                                       |
| `count`     | Number of elements to convert                                           |
| `in`        | Pointer to a read-only array containing at least `count` input values   |
| `out`       | Pointer to an array that will receive at least `count` converted values |

## Returns

| Return Value | Description                                                                                             |
| ------------ | ------------------------------------------------------------------------------------------------------- |
| `void`       | This function does not return a value. Converted values are written to the caller-provided output array |

## Preconditions

Before calling `cv_convert_doubles()`:

- `converter` must reference a valid [`cv_converter`](cv-converter.html) object created by UDUNITS-2.
- The converter must not have been freed using [`cv_free()`](cv-free.html).
- `in` must reference an array containing at least `count` elements.
- `out` must reference an array capable of storing at least `count` elements.
- The input (`in`) and output (`out`) arrays must not overlap in memory in an unsafe manner.

The input and output arrays may refer to the same memory location (in-place conversion).
However, if they partially overlap, the behavior is undefined.

## Error Handling

This function does not distinguish errors from a valid return value.
Use [`ut_get_status()`](ut-get-status.html) to determine whether an error occurred.

## Object Ownership

`cv_convert_doubles()` does not allocate or return any dynamically allocated objects.

The function operates on an existing [`cv_converter`](cv-converter.html) provided by the caller and writes
the converted values into the caller-provided output array.

| Memory Allocation | Caller Responsibility                                                          |
| ----------------- | ------------------------------------------------------------------------------ |
| None              | Provide valid input and output arrays; ensure the [`cv_converter`](cv-converter.html) remains valid |

## Thread Safety

`cv_convert_doubles()` is thread-safe under the following conditions:

- The provided [`cv_converter`](cv-converter.html) object is not modified or freed while the function
  is executing.
- The input (`in`) and output (`out`) arrays are not concurrently accessed in a
  conflicting manner by other threads.

The function performs read-only operations on the [`cv_converter`](cv-converter.html) and applies its
transformation independently to each element. It does not require synchronization
when used with independent data and properly managed converter objects.

## Numeric Behavior

`cv_convert_doubles()` applies the affine transformation defined by the
`cv_converter` to each input value independently.

- Floating-point computations follow IEEE 754 double-precision semantics.
- If an input value is `NaN` or `±Inf`, the corresponding output value is
  produced according to standard floating-point propagation rules.
- Conversions involving large scale factors or offsets may result in overflow
  or underflow.
- Floating-point rounding may occur, and results are subject to the precision
  limits of double-precision representation.
- Repeated conversions or chaining of multiple converters may accumulate
  floating-point error.

The function does not perform range checking, overflow detection, or error
reporting for numerical conditions. Any such effects are reflected directly in
the computed output values.

## Example

This example demonstrates how to convert an array of values from meters to
kilometers using a [`cv_converter`](cv-converter.html).

```c
#include <stdio.h>
#include <udunits2.h>
#include <udunits2/converter.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* meter = ut_parse(system, "meter", UT_ASCII);
    ut_unit* kilometer = ut_parse(system, "kilometer", UT_ASCII);
    if (!meter || !kilometer) {
        fprintf(stderr, "Failed to parse units\n");
        ut_free_system(system);
        return 1;
    }

    cv_converter* converter = ut_get_converter(meter, kilometer);
    if (!converter) {
        fprintf(stderr, "Units are not convertible\n");
        ut_free(meter);
        ut_free(kilometer);
        ut_free_system(system);
        return 1;
    }

    double input[] = {1000.0, 2000.0, 3000.0};
    double output[3];

    cv_convert_doubles(converter, 3, input, output);

    for (size_t i = 0; i < 3; i++) {
        printf("%f meters = %f kilometers\n", input[i], output[i]);
    }

    cv_free(converter);
    ut_free(meter);
    ut_free(kilometer);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`cv_convert_double()`](cv-convert-double.html)
- [`cv_convert_float()`](cv-convert-float.html)
- [`cv_convert_floats()`](cv-convert-floats.html)
- [`ut_get_converter()`](ut-get-converter.html)
- [`cv_combine()`](cv-combine.html)
- [`cv_free()`](cv-free.html)
