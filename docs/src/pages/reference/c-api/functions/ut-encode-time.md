---
title: ut_encode_time()
sidebar: sidebar
toc: true
permalink: ut-encode-time.html
---

Encodes calendar date and clock time components into a UDUNITS-2 time value representing a complete date-time.

## Prototype

```c
double ut_encode_time(
    int year,
    int month,
    int day,
    int hour,
    int minute,
    double second
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Encodes calendar date and time components into a single
**UDUNITS-2 encoded time value**.

The returned value is an **opaque encoded representation** of a
calendar date-time. It should not be interpreted as a simple numeric
quantity such as seconds or days.

This function conceptually combines the roles of
[`ut_encode_date()`](ut-encode-date.html) and [`ut_encode_clock()`](ut-encode-clock.html).

While equivalent results can often be obtained by combining
[`ut_encode_date()`](ut-encode-date.html) and `ut_encode_clock()`, this function ensures a
consistent, single-step encoding of the complete date-time.

The returned value is intended to be used with [`ut_decode_time()`](ut-decode-time.html)
to recover the original components.

This function performs no validation of input values and does not
interact with a [`ut_system`](ut-system.html).

## Parameters

| Parameter | Description |
| -------- | ----------- |
| `year` | Year component (e.g., 2024) |
| `month` | Month component (typically 1–12) |
| `day` | Day of month (valid for given month/year) |
| `hour` | Hour component (typically 0–23) |
| `minute` | Minute component (typically 0–59) |
| `second` | Seconds component, including fractional seconds (typically 0.0–59.0) |

## Returns

| Return Value | Description |
| ----------- | ----------- |
| `double` | Encoded date-time value (opaque UDUNITS-2 time encoding) |

## Preconditions

- Inputs should represent a valid calendar date and time.
- Typical ranges:
  - month: 1–12
  - day: valid for given month/year
  - hour: 0–23
  - minute: 0–59
  - second: 0.0–59.0
- Inputs are not validated.

## Error Handling

Returns a double-precision value. Use [`ut_get_status()`](ut-get-status.html) to
determine whether an error occurred.

## Object Ownership

- This function does not allocate or manage memory.
- All inputs are passed by value.
- The caller retains full ownership of all data.

## Thread Safety

**Thread-safe.**

- This function performs only local computations.
- It does not access or modify shared or global state.

## Example

This example demonstrates how ut_encode_time() encodes a complete date-time value.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    double datetime = ut_encode_time(2024, 3, 1, 12, 30, 15.5);

    printf("Encoded date-time value: %.10f\n", datetime);

    return 0;
}
```

## See Also

- [`ut_encode_date()`](ut-encode-date.html)
- [`ut_encode_clock()`](ut-encode-clock.html)
- [`ut_decode_time()`](ut-decode-time.html)
