---
title: cv_combine()
sidebar: sidebar
toc: true
permalink: cv-combine.html
---

Creates a new [`cv_converter`](cv-converter.html) representing the composition of two converters.

## Prototype

```c
cv_converter* cv_combine(
    const cv_converter* first,
    const cv_converter* second
);
```

## Header

```c
#include <udunits2/converter.h>
```

## Description

`cv_combine()` creates a new [`cv_converter`](cv-converter.html) representing the
composition of two existing converters.

Each converter defines a mathematical transformation between compatible units
and is typically created with [`ut_get_converter()`](ut-get-converter.html).
`cv_combine()` composes these transformations into a single converter equivalent
to applying the first conversion followed by the second.

The converters must be composable: the output of `first` must be compatible with
the input of `second`. In practice, this means the unit relationship represented
by the first converter must match the expected input of the second converter.

The returned converter is independent of the input converters and can be used to
perform the combined transformation as a single operation.

## Parameters

| Parameter | Description                                                                                                  |
| --------- | ------------------------------------------------------------------------------------------------------------ |
| `first`   | Pointer to a [`cv_converter`](cv-converter.html) defining the first transformation to apply                                       |
| `second`  | Pointer to a [`cv_converter`](cv-converter.html) defining the second transformation applied to the result of the first converter  |

## Returns

| Return Value    | Description                                                                                                                                   |
| --------------- | --------------------------------------------------------------------------------------------------------------------------------------------- |
| `cv_converter*` | Pointer to a newly allocated [`cv_converter`](cv-converter.html) representing the composed transformation. The caller must release the converter using [`cv_free()`](cv-free.html) |

## Preconditions

All pointer arguments must be non-NULL and valid. Units must belong to a properly
initialized unit system.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved
using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

`cv_combine()` allocates a new [`cv_converter`](cv-converter.html) representing the composed
transformation.

The returned converter is independent of the input converters and remains
valid until it is released.

The caller is responsible for freeing the returned converter using
[`cv_free()`](cv-free.html) when it is no longer needed.

| Memory Allocation | Caller Responsibility                            |
| ----------------- | ------------------------------------------------ |
| [`cv_converter`](cv-converter.html)    | Release the returned converter using [`cv_free()`](cv-free.html) |

## Thread Safety

`cv_combine()` does not modify the input [`cv_converter`](cv-converter.html) objects and does not
interact with any [`ut_system`](ut-system.html) instances. It creates a new converter based solely
on the transformations defined by the input converters.

The function may update the global UDUNITS-2 status value (`ut_status`) to
report errors. Consequently, concurrent calls from multiple threads can result
in races when accessing or interpreting the global status.

The function is thread-safe under the following conditions:

- The `first` and `second` converters are not modified or freed concurrently.
- Threads do not rely on [`ut_get_status()`](ut-get-status.html) without external synchronization.

To ensure correct error handling in multi-threaded programs, applications should
synchronize access to [`ut_get_status()`](ut-get-status.html) or avoid depending on the global status
in concurrent contexts.

## Example

This example demonstrates how `cv_combine()` composes two compatible converters,
one converting meters to kilometers and another converting kilometers to centimeters,
into a single converter that performs the equivalent meters to centimeters transformation
in one step:

```c
#include <stdio.h>
#include <udunits2.h>
#include <udunits2/converter.h>

/*
 * Create a unit system and obtain units.
 * All units must come from the same system.
 */
ut_system* system = ut_read_xml(NULL);

ut_unit* meter      = ut_get_unit_by_name(system, "meter");
ut_unit* kilometer  = ut_get_unit_by_name(system, "kilometer");
ut_unit* centimeter = ut_get_unit_by_name(system, "centimeter");

/* Create converters: meters → kilometers → centimeters */
cv_converter* m_to_km  = ut_get_converter(meter, kilometer);
cv_converter* km_to_cm = ut_get_converter(kilometer, centimeter);

/* Compose into a single converter: meters → centimeters */
cv_converter* m_to_cm = cv_combine(m_to_km, km_to_cm);

if (m_to_cm != NULL) {
    double result = cv_convert_double(m_to_cm, 1.0);
    printf("Converted value: %f\n", result); /* Expected: 100.0 */
}

/*
 * The composed converter is equivalent to:
 * result(x) = km_to_cm(m_to_km(x))
 */

/* Clean up */
cv_free(m_to_km);
cv_free(km_to_cm);
cv_free(m_to_cm);

ut_free(meter);
ut_free(kilometer);
ut_free(centimeter);
ut_free_system(system);
```

## See Also

- [`cv_free()`](cv-free.html)
- [`ut_get_converter()`](ut-get-converter.html)
- [`cv_convert_double()`](cv-convert-double.html)
- [`cv_convert_doubles()`](cv-convert-doubles.html)
- [`cv_convert_float()`](cv-convert-float.html)
- [`cv_convert_floats()`](cv-convert-floats.html)
