---
title: ut_get_status()
sidebar: sidebar
toc: true
permalink: ut-get-status.html
---

Returns the current global UDUNITS-2 error status.

## Prototype

```c
ut_status ut_get_status(void);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_get_status()` returns the current global error status maintained by
the UDUNITS-2 library.

UDUNITS-2 functions that encounter errors typically update an internal
status value. This function allows applications to retrieve that status
after a function call to determine the outcome or diagnose failures.

The returned value is of type `ut_status`, an enumeration representing
success and error conditions defined by the library. The returned value
may indicate either success (e.g., `UT_SUCCESS`) or a specific error
condition.

This function does not modify the current status; it only retrieves it.

Because the status is stored globally within the UDUNITS-2 library, its
value may be overwritten by subsequent UDUNITS-2 function calls.

## Parameters

| Parameter | Description |
| -------- | ----------- |
| `void` | This function takes no parameters |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `ut_status`  | The current global UDUNITS-2 status code (success or error condition) |

## Preconditions

There are no strict preconditions for calling `ut_get_status()`.

However:

- It is typically called **after another UDUNITS-2 function** to inspect
  the result of that operation
- The returned value is only meaningful if a prior function has set the
  status

## Error Handling

This function does not set an error condition and does not fail.

Instead, it is used to **retrieve error information** set by previous
UDUNITS-2 function calls.

Typical usage pattern:

1. Call a UDUNITS-2 function
2. Check its return value
3. If needed, call `ut_get_status()` to determine the specific error

Because the status is global:

- It may be overwritten by subsequent UDUNITS-2 calls
- It should be retrieved immediately after the function of interest

## Object Ownership

`ut_get_status()` does not allocate or return pointers to objects.

No ownership or memory management responsibilities are involved.

| Memory Allocation | Caller Responsibility |
| ---------------- | -------------------- |
| None | None |

## Thread Safety

`ut_get_status()` accesses a **global status variable** maintained by the
UDUNITS-2 library.

As a result:

- The function itself is safe to call
- The returned value may be **non-deterministic in multithreaded
  environments** if multiple threads are calling UDUNITS-2 functions

In multithreaded applications:

- Concurrent UDUNITS-2 calls may overwrite the global status
- External synchronization is required if deterministic error reporting
  is needed

## Example

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        fprintf(stderr, "Failed to load UDUNITS-2 database\n");
        return 1;
    }

    ut_unit* invalid = ut_get_unit_by_name(system, "not_a_unit");

    if (invalid == NULL) {
        ut_status status = ut_get_status();
        fprintf(stderr, "Operation failed with status: %d\n", status);
    }

    ut_free_system(system);
    return 0;
}
```

## See Also

- [`ut_get_converter()`](ut-get-converter.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
- [`ut_set_error_message_handler()`](ut-set-error-message-handler.html)
