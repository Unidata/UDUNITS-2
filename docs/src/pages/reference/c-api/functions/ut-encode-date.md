---
title: ut_encode_date()
sidebar: sidebar
toc: true
permalink: ut-encode-date.html
---


Encodes calendar date components into a UDUNITS-2 time value representing a calendar date.

## Prototype

```c
double ut_encode_date(
    int year,
    int month,
    int day
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Encodes a calendar date (year, month, and day) into a
**UDUNITS-2 encoded time value** representing the date component of a
calendar-based encoding.

The returned value is an **opaque encoded representation** and should
not be interpreted as a numeric quantity such as a count of days.

This function encodes only the date component and is typically combined
with [`ut_encode_clock()`](ut-encode-clock.html) or used indirectly via [`ut_encode_time()`](ut-encode-time.html).  

This value is not intended to be decoded independently; decoding is
performed using [`ut_decode_time()`](ut-decode-time.html) on complete encoded values.

Calendar handling follows UDUNITS-2 internal rules (Gregorian-based).

This function performs no validation of input values and does not
interact with a [`ut_system`](ut-system.html).

## Parameters

| Parameter | Description |
| -------- | ------------------------------------------------ |
| `year` | Year component (e.g., 2024) |
| `month` | Month component (typically 1–12) |
| `day` | Day of month (typically valid for given month) |

## Returns

| Return Value | Description |
| ----------- | ----------- |
| `double` | Encoded date component (opaque value) |

## Preconditions

- Inputs should represent a valid calendar date.
- Typical ranges:
  - month: 1–12
  - day: valid for given month and year
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

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    double date = ut_encode_date(2024, 3, 1);
    double clock = ut_encode_clock(12, 30, 0.0);

    /* Combine date and time-of-day components */
    double datetime = date + clock;

    printf("Encoded date-time value: %.10f\n", datetime);

    return 0;
}
```

## See Also

- [`ut_encode_clock()`](ut-encode-clock.html)
- [`ut_encode_time()`](ut-encode-time.html)
- [`ut_decode_time()`](ut-decode-time.html)
