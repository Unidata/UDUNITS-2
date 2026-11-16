---
title: ut_read_xml()
sidebar: sidebar
toc: true
permalink: ut-read-xml.html
---

Creates a new unit system by reading a UDUNITS-2 XML unit definition database.

## Prototype

```c
ut_system* ut_read_xml(
    const char* path
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_read_xml()` creates a new unit system by parsing a UDUNITS-2 XML
unit database and returning the corresponding [`ut_system`](ut-system.html) object.

The XML database defines units, prefixes, dimensions, and their
relationships, which are used to construct a complete and internally
consistent unit system.

If `path` is `NULL`, the function attempts to locate and load the default
UDUNITS-2 XML database using environment variables and built-in default
search paths.

Each invocation returns a newly constructed, independent [`ut_system`](ut-system.html).
All [`ut_unit`](ut-unit.html) objects retrieved from this system are bound to it and are
owned by the system for their entire lifetime.

Units are strictly scoped to the [`ut_system`](ut-system.html) from which they originate.
Units obtained from different systems are not compatible and must not be
used together in operations such as conversion, comparison, or arithmetic.

The returned system remains valid until it is explicitly released using
[`ut_free_system()`](ut-free-system.html), at which point all associated units become invalid.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `path`    | Path to the UDUNITS-2 XML database file (e.g., `udunits2.xml`). If `NULL`, the function attempts to locate the default XML database using environment variables and built-in search paths |

## Returns

| Return Value | Description                                       |
| ------------ | ------------------------------------------------- |
| `ut_system*` | Pointer to a newly created unit system            |
| `NULL`       | Failure (see [`ut_get_status()`](ut-get-status.html) for error details) |

## Preconditions

Before calling `ut_read_xml()`:

- `path` may be `NULL`. If non-`NULL`, it must reference a readable XML file.
- The XML document must conform to the UDUNITS-2 unit database format.

## Error Handling

Returns `NULL` on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_read_xml()` creates and returns a new [`ut_system`](ut-system.html) object representing
the unit system defined by the XML database.

The returned [`ut_system`](ut-system.html) owns all units, prefixes, and related definitions
parsed from the XML file. The system remains valid until it is explicitly
released.

All [`ut_unit`](ut-unit.html) objects obtained from the system are owned by it and must
not be freed directly. These objects are valid only for the lifetime of
the associated [`ut_system`](ut-system.html) and become invalid once the system is released.

The caller is responsible for releasing the returned system using
[`ut_free_system()`](ut-free-system.html).

| Memory Allocation | Caller Responsibility                                |
| ----------------- | ---------------------------------------------------- |
| [`ut_system`](ut-system.html)       | Release the returned system using [`ut_free_system()`](ut-free-system.html) |

## Thread Safety

`ut_read_xml()` is not thread-safe.

The function performs file I/O, constructs internal data structures, and
relies on global library state (including error reporting via
[`ut_get_status()`](ut-get-status.html)). Concurrent calls to this function, or to other
UDUNITS-2 functions, may result in race conditions or undefined behavior.

Applications using UDUNITS-2 in a multithreaded environment must ensure
that calls to `ut_read_xml()` (and any other [UDUNITS-2 API](c-api-overview.html#api-organization) functions)
are externally synchronized.

UDUNITS-2 does not guarantee thread safety for any operations.
All use of the library in multithreaded applications must be
externally synchronized.

## Example

This example demonstrates how `ut_read_xml()` loads the default UDUNITS-2 XML unit database, retrieves two units from the system, and converts a value from meters to kilometers.

```c
#include <stdio.h>
#include <udunits2.h>
#include <udunits2/converter.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);

    if (system == NULL) {
        fprintf(stderr, "Failed to load UDUNITS-2 database\n");
        return 1;
    }

    ut_unit* meter = ut_get_unit_by_name(system, "meter");
    ut_unit* kilometer = ut_get_unit_by_name(system, "kilometer");

    if (meter == NULL || kilometer == NULL) {
        fprintf(stderr, "Failed to retrieve units\n");
        ut_free_system(system);
        return 1;
    }

    cv_converter* converter = ut_get_converter(meter, kilometer);

    if (converter == NULL) {
        fprintf(stderr, "Units are not convertible\n");
        ut_free_system(system);
        return 1;
    }

    double result = cv_convert_double(converter, 1500.0);

    printf("1500 meters = %f kilometers\n", result);

    cv_free(converter);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
1500 meters = 1.500000 kilometers
```

## See Also

- [`ut_new_system()`](ut-new-system.html)
- [`ut_free_system()`](ut-free-system.html)
- [`ut_set_path_xml()`](ut-set-path-xml.html)
- [`ut_get_path_xml()`](ut-get-path-xml.html)
