---
title: utConvert()
sidebar: sidebar
toc: true
permalink: utConvert.html
---

Computes the linear transformation required to convert values between two
compatible UDUNITS-1 units.

{% include warning.html content="
**`utConvert()` is part of the UDUNITS-1 legacy compatibility interface.**  

New
applications should use [`ut_get_converter()`](ut-get-converter.html) and
the UDUNITS-2 [Unit Conversion API](unit-conversion-api.html), as they provide
a more robust and composable conversion model and support efficient scalar and
vectorized numeric transformations.
"%}

## Prototype

```c
int utConvert(
    const utUnit* from,
    const utUnit* to,
    double* slope,
    double* intercept
);
```

## Header

```c
#include <udunits.h>
```

## Description

`utConvert()` computes the linear transformation needed to convert values
between two compatible units.

```c
y = slope * x + intercept
```

Where `slope` defines the multiplicative scale factor and `intercept` defines
any additive offset. For purely multiplicative conversions (for example, meters
to kilometers), `intercept` is zero. For offset conversions (for example, Celsius
to Kelvin or Fahrenheit), both parameters are required.

Both units must belong to the same unit system and be dimensionally compatible.
If the units are not compatible, the function returns an error and the output
parameters are not modified.

## Parameters

| Parameter | Description |
| ----------- | ------------- |
| `from` | Pointer to the source `utUnit` |
| `to` | Pointer to the destination `utUnit` |
| `slope` | Receives the multiplicative conversion factor |
| `intercept` | Receives the additive conversion offset |

## Returns

| Return Value | Description                                    |
| ------------ | ---------------------------------------------- |
| `0`          | Conversion parameters successfully computed    |
| `-1`         | Units are not convertible or an error occurred |

## Error Handling

`utConvert()` returns `-1` if the units are not convertible or if an error occurs.

Typical error conditions include:

- Units are not dimensionally compatible
- Either unit pointer is invalid
- Units belong to different unit systems

UDUNITS-1 provides limited diagnostic detail for conversion failures.
Applications requiring more robust error reporting should use the
[UDUNITS-2 Unit Conversion API](unit-conversion-api.html).

## Object Ownership

`utConvert()` does not allocate memory or create new objects.

The caller retains ownership of all provided objects.

| Memory Allocation | Caller Responsibility                                                          |
| ----------------- | ------------------------------------------------------------------------------ |
| None              | Caller provides valid `utUnit` objects and storage for `slope` and `intercept` |

## Thread Safety

`utConvert()` does not modify the provided `utUnit` objects. However,
UDUNITS-1 relies on global state and is not guaranteed to be thread-safe
without external synchronization.

Applications requiring stronger thread-safety guarantees should use the
UDUNITS-2 API.

## Example

The following example demonstrates how `utConvert()` computes the slope and
intercept required to convert temperature values from degrees Celsius to Kelvin.

```c
#include <stdio.h>
#include <udunits.h>

int main(void)
{
    utUnit celsius;
    utUnit kelvin;

    utScan("degC", &celsius);
    utScan("K", &kelvin);

    double slope;
    double intercept;

    if (utConvert(&celsius, &kelvin, &slope, &intercept) == 0) {
        double c = 25.0;
        double k = slope * c + intercept;

        printf("25 degC = %f K\n", k);
    }
    else {
        printf("Units are not convertible\n");
    }

    return 0;
}
```

## See Also

- [`utScan()`](utScan.html)
- [`utIsConvertible()`](utisconvertible.html)
- [`utScale()`](utscale.html)
- [`utOffset()`](utoffset.html)
- [`utInvert()`](utinvert.html)
- [`utMultiply()`](utmultiply.html)
- [`utDivide()`](utdivide.html)
- [`utPrint()`](utPrint.html)
- [UDUNITS-2 Unit Conversion API](unit-conversion-api.html)
