---
title: utIsTime()
sidebar: sidebar
toc: true
permalink: utIsTime.html
---

Determines whether a UDUNITS-1 `utUnit` structure represents a temporal unit.

{% include warning.html content="
**`utIsTime()` is part of the UDUNITS-1 legacy compatibility interface.**  

New applications should prefer the [UDUNITS-2 API](c-api-overview.html#api-organization) and explicitly evaluate
dimensional compatibility using functions such as
[`ut_are_convertible()`](ut-are-convertible.html) in combination with
canonical time units defined in the UDUNITS-2 unit system.
"%}

## Prototype

```c
int utIsTime(
    const utUnit* unit
);
```

## Header

```c
#include <udunits.h>
```

## Description

`utIsTime()` determines whether a UDUNITS-1 `utUnit` structure represents a
unit of time.

The function evaluates the dimensional structure of the provided unit and
determines whether it is dimensionally compatible with the base time unit
defined in the unit system. It returns a non-zero value if the unit corresponds
to a temporal quantity (e.g., seconds, hours, days, or derived time units).

The evaluation is based solely on dimensional analysis. The function does not
require the unit to include a time origin (for example, `"seconds since 1970-01-01"`).

This function is commonly used when working with calendar-based conversions
or when distinguishing temporal coordinate variables in scientific datasets.

## Parameters

| Parameter | Description |
|-----------|-------------|
| `unit` | Pointer to a `utUnit` structure to evaluate |

## Returns

| Return Value | Description |
|--------------|-------------|
| non-zero | Unit represents a time quantity |
| `0` | Unit does not represent a time quantity |

## Error Handling

`utIsTime()` does not provide detailed diagnostic information.

Behavior is undefined if `unit` is not a valid initialized `utUnit`
structure.

The UDUNITS-1 compatibility interface does not expose structured error
reporting for classification functions.

Applications requiring explicit dimensional analysis should use the
UDUNITS-2 API.

## Object Ownership

`utIsTime()` does not allocate memory or modify the provided `utUnit`.

The caller retains ownership of the object.

| Memory Allocation | Caller Responsibility |
|------------------|----------------------|
| None | Caller provides a valid initialized `utUnit` |

## Thread Saftey

`utIsTime()` does not modify the provided `utUnit` structure.

However, the UDUNITS-1 compatibility layer relies on shared global state and
is not guaranteed to be thread-safe without external synchronization.

Applications requiring stronger thread safety guarantees should use the
UDUNITS-2 API.

## Example

The following example demonstrates how `utIsTime()` determines whether a
previously parsed unit definition represents a temporal unit.

```c
#include <stdio.h>
#include <udunits.h>

int main(void)
{
    utUnit seconds;
    utUnit meters;

    utScan("s", &seconds);
    utScan("m", &meters);

    if (utIsTime(&seconds))
        printf("seconds is a time unit\n");

    if (!utIsTime(&meters))
        printf("meters is not a time unit\n");

    return 0;
}
```

## See Also

- [`utScan()`](utScan.html)
- [`utCalendar()`](utCalendar.html)
- [`utInvCalendar()`](utInvCalendar.html)
- [`utConvert()`](utConvert.html)
- [`utIsDimensionless()`](utIsDimensionless.html)
- [UDUNITS-2 Unit Systems API](unit-systems-api.html)
- [UDUNITS-2 Unit Conversion API](unit-conversion-api.html)
