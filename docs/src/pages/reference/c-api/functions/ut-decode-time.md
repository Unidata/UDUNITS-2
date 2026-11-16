---
title: ut_decode_time()
sidebar: sidebar
toc: true
permalink: ut-decode-time.html
---


Decodes a UDUNITS-2 encoded time value into calendar date and time components.

## Prototype

```c
void ut_decode_time(
    double value,
    int* year,
    int* month,
    int* day,
    int* hour,
    int* minute,
    double* second
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Decodes a **UDUNITS-2 encoded time value** into its corresponding
calendar date and time components.

The input `value` must be a value produced by [`ut_encode_time()`](ut-encode-time.html).
It represents a calendar-based encoded timestamp containing year,
month, day, hour, minute, and seconds.

This function extracts all components simultaneously. UDUNITS-2 does not
provide separate decoding functions for date-only or time-only values.

The decoded result follows UDUNITS-2 internal calendar rules
(Gregorian-based) and typically represents UTC.

This function performs no unit conversion and does not interact with a
`ut_system`.

## Parameters

| Parameter | Description |
| -------- | ----------- |
| `value` | Encoded time value produced by [`ut_encode_time()`](ut-encode-time.html) |
| `year` | Pointer to output year (must not be `NULL`) |
| `month` | Pointer to output month (1–12, must not be `NULL`) |
| `day` | Pointer to output day (must not be `NULL`) |
| `hour` | Pointer to output hour (0–23, must not be `NULL`) |
| `minute` | Pointer to output minute (0–59, must not be `NULL`) |
| `second` | Pointer to output seconds (must not be `NULL`) |

## Returns

| Return Value | Description                           |
| ------------ | ------------------------------------- |
| `void`       | This function does not return a value |

## Preconditions

- `value` should be a value produced by [`ut_encode_time()`](ut-encode-time.html) for
  meaningful results.
- All output pointers (`year`, `month`, `day`, `hour`, `minute`,
  `second`) must be non-NULL.

## Error Handling

This function does not return a value. Use [`ut_get_status()`](ut-get-status.html) to
determine whether an error occurred.

## Object Ownership

- The caller retains ownership of all provided pointers.
- No memory is allocated or freed by this function.
- Results are written directly into caller-provided storage.

## Thread Safety

**Thread-safe.**

- This function uses only local variables.
- It does not access or modify shared or global state.

## Example

This example demonstrates how `ut_decode_time()` decodes a value created
by [`ut_encode_time()`](ut-encode-time.html).

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    double value = ut_encode_time(2024, 3, 1, 12, 0, 0.0);

    int year, month, day, hour, minute;
    double second;

    ut_decode_time(value, &year, &month, &day, &hour, &minute, &second);

    printf("Decoded time: %04d-%02d-%02d %02d:%02d:%06.3f\n",
           year, month, day, hour, minute, second);

    return 0;
}
```

## See Also

- [`ut_encode_time()`](ut-encode-time.html)
- [`ut_encode_date()`](ut-encode-date.html)
- [`ut_encode_clock()`](ut-encode-clock.html)
