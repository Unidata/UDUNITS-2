---
title: ut_encode_clock()
sidebar: sidebar
toc: true
permalink: ut-encode-clock.html
---


Encodes clock time components into a UDUNITS-2 time value representing time-of-day.

## Prototype

```c
double ut_encode_clock(
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

Encodes clock time components (hour, minute, and second) into a
**time-of-day component** of a UDUNITS-2 encoded time value.

The returned value represents the time-of-day portion of a
calendar-based encoding and is typically combined with
[`ut_encode_date()`](ut-encode-date.html) or used indirectly via [`ut_encode_time()`](ut-encode-time.html).

This value should be treated as an **opaque encoded component**, not as
a directly interpretable quantity such as a simple count of seconds.

This function encodes only the time-of-day component and does not
include any date information. It is not intended to be decoded
independently.

This function does not have a corresponding decode operation; decoding
is only supported via [`ut_decode_time()`](ut-decode-time.html) on complete encoded values.

This function performs no calendar interpretation and does not
interact with a [`ut_system`](ut-system.html).

## Parameters

| Parameter | Description                                                         |
| --------- | ------------------------------------------------------------------- |
| `hour`    | Hour component (typically 0–23)                                     |
| `minute`  | Minute component (typically 0–59)                                   |
| `second`  | Second component, including fractional seconds (typically 0.0–59.0) |

## Returns

| Return Value | Description |
| ----------- | ----------- |
| `double` | Encoded time-of-day component (opaque value) |

## Preconditions

- Inputs should represent a valid time-of-day.
- Typical ranges:
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

- This function uses only local computations.
- It does not access or modify shared or global state.

## Example

This example demonstrates how `ut_encode_clock()` encodes a time-of-day
and combines it with a date.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    double date = ut_encode_date(2024, 3, 1);
    double clock = ut_encode_clock(12, 30, 15.5);

    /* Combine date and time-of-day components */
    double datetime = date + clock;

    printf("Encoded date-time value: %.10f\n", datetime);

    return 0;
}
```

## See Also

- [`ut_encode_time()`](ut-encode-time.html)
- [`ut_encode_date()`](ut-encode-date.html)
- [`ut_decode_time()`](ut-decode-time.html)
