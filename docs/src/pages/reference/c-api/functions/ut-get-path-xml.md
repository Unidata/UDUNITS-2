---
title: ut_get_path_xml()
sidebar: sidebar
toc: true
permalink: ut-get-path-xml.html
---

Returns the filesystem path of the UDUNITS-2 XML unit definition database.

## Prototype

```c
const char* ut_get_path_xml(
    const char* path,
    ut_status* status
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_path_xml()` determines the filesystem path of the UDUNITS-2 XML
unit definition database.

If `path` is not `NULL`, the function returns that value unchanged.
Otherwise, the function attempts to determine the database path using the
`UDUNITS2_XML_PATH` environment variable. If the environment variable is
not defined, the function returns the default installation path compiled into the UDUNITS-2 library.

If `status` is not `NULL`, the function sets the referenced value to
indicate how the pathname was determined (`UT_OPEN_ARG`, `UT_OPEN_ENV`,
or `UT_OPEN_DEFAULT`).

The returned string must be treated as read-only and must not be modified
or freed by the caller.

The resolved pathname is typically used by [`ut_read_xml()`](ut-read-xml.html) when loading the UDUNITS-2 XML database.

## Parameters

| Parameter | Description                                                                                                                                                              |
| --------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `path`    | Pathname of the XML database file or `NULL`. If `NULL`, the path is determined from the `UDUNITS2_XML_PATH` environment variable or the library's compiled default path |
| `status`  | Pointer to a `ut_status` variable that receives the resolution status (`UT_OPEN_ARG`, `UT_OPEN_ENV`, or `UT_OPEN_DEFAULT`)   |

## Returns

| Return Value  | Description                                                                                                                           |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| `const char*` | Pathname of the XML unit database determined from provided `path`, the environment variable `UDUNITS2_XML_PATH`, or the compiled default path |

## Preconditions

Before calling `ut_get_path_xml()`:

- The caller must treat the returned string as read-only and must not
  attempt to modify or free it.

## Error Handling

This function does not have defined error-reporting behavior. A `NULL` return
value does not necessarily indicate an error.

## Object Ownership

`ut_get_path_xml()` returns a pointer to a string containing the
filesystem path used to locate the UDUNITS-2 XML unit definition database.

The returned pointer may reference the `path` argument provided by the
caller, the value of the `UDUNITS2_XML_PATH` environment variable, or a
string compiled into the UDUNITS-2 library.

The returned string must be treated as read-only and must not be modified
or freed by the caller.

| Memory Allocation | Caller Responsibility |
| ----------------- | -------------------- |
| None              | None                 |

## Thread Safety

`ut_get_path_xml()` determines the XML database pathname using the
provided argument, the `UDUNITS2_XML_PATH` environment variable, or the
compiled default path.

The function reads the UDUNITS2_XML_PATH environment variable but does not
otherwise modify library state.

UDUNITS-2 maintains global library status for error reporting
([`ut_get_status()`](ut-get-status.html)), so concurrent calls to UDUNITS-2 functions may
overwrite this global status value.

Applications that require deterministic error handling in multithreaded
environments should use external synchronization when calling UDUNITS
library functions.

## Example

This example demonstrates how `ut_get_path_xml()` determines the path
to the UDUNITS-2 XML unit definition database.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_status status;
    const char* path = ut_get_path_xml(NULL, &status);

    printf("XML database path: %s\n", path);

    ut_system* system = ut_read_xml(path);

    if (system == NULL) {
        fprintf(stderr, "Failed to load unit database\n");
        return 1;
    }

    ut_free_system(system);
    return 0;
}
```

Example output:

```c
XML database path: /usr/local/share/udunits/udunits2.xml
```

## See Also

- [`ut_set_path_xml()`](ut-set-path-xml.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_new_system()`](ut-new-system.html)
