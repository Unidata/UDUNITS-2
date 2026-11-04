---
title: cv_free()
sidebar: sidebar
toc: true
permalink: cv-free.html
---

Releases a [`cv_converter`](cv-converter.html) object.

## Prototype

```c
void cv_free(
    cv_converter* converter
);
```

## Header

```c
#include <udunits2/converter.h>
```

## Description

`cv_free()` releases a [`cv_converter`](cv-converter.html) object previously created by UDUNITS-2.

A [`cv_converter`](cv-converter.html) defines the mathematical transformation between two compatible
units and is typically created with [`ut_get_converter()`](ut-get-converter.html) or constructed from
other converters using functions such as [`cv_combine()`](cv-combine.html).

After calling `cv_free()`, the converter pointer becomes invalid and must not be used
in subsequent conversion operations.

## Parameters

| Parameter   | Description                                                         |
| ----------- | ------------------------------------------------------------------- |
| `converter` | Pointer to a [`cv_converter`](cv-converter.html) object previously created by UDUNITS-2 |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `void`       | This function does not return a value. The converter resources are released |

## Preconditions

Before calling `cv_free()`:

- `converter` must reference a valid [`cv_converter`](cv-converter.html) object created by UDUNITS-2
- The caller must have ownership of the converter
- The converter must not have already been freed
- No other thread or component may be concurrently using or freeing the converter

## Error Handling

This function does not report errors. The behavior is undefined if an
invalid argument is supplied.

## Object Ownership

`cv_free()` releases a [`cv_converter`](cv-converter.html) object that was previously created by UDUNITS-2.

`cv_converter` objects follow a single-owner memory model. The caller that obtains the
converter (e.g., via [`ut_get_converter()`](ut-get-converter.html) or [`cv_combine()`](cv-combine.html)) is solely responsible for
releasing it using `cv_free()`.

UDUNITS-2 does not perform reference counting or shared ownership tracking for [`cv_converter`](cv-converter.html)
objects. Consequently:

- Each converter must be freed exactly once
- Multiple calls to `cv_free()` on the same pointer result in undefined behavior
- The caller must ensure that no other part of the program attempts to use or free the
converter after it has been released

| Memory Allocation | Caller Responsibility                                                 |
| ----------------- | --------------------------------------------------------------------- |
| [`cv_converter`](cv-converter.html)    | Must be released exactly once using `cv_free()` when no longer needed |

## Thread Safety

`cv_free()` releases resources associated with a cv_converter and does not modify global
UDUNITS-2 state.

The function is thread-safe **with respect to independent converter objects**. However,
because [`cv_converter`](cv-converter.html) objects are not reference-counted:

- The caller must ensure **exclusive access** to the converter at the time of the call
- The converter must not be concurrently used or freed by another thread
- Synchronization is required if the converter is shared across threads

## Example

This example creates a converter for meters to kilometers and releases
it when it is no longer needed.

```c
#include <udunits2.h>
#include <udunits2/converter.h>

/* meter and kilometer obtained earlier from ut_parse() or ut_get_unit_by_name() */
cv_converter* converter = ut_get_converter(meter, kilometer);

/* Converter can now be used for conversions */

cv_free(converter);
```

## See Also

- [`cv_combine()`](cv-combine.html)
- [`ut_get_converter()`](ut-get-converter.html)
- [`cv_convert_double()`](cv-convert-double.html)
- [`cv_convert_doubles()`](cv-convert-doubles.html)
- [`cv_convert_float()`](cv-convert-float.html)
- [`cv_convert_floats()`](cv-convert-floats.html)
