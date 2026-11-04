---
title: ut_set_status()
sidebar: sidebar
toc: true
permalink: ut-set-status.html
---

Sets the global UDUNITS-2 error status and returns the previous status value.

## Prototype

```c
ut_status ut_set_status(
    ut_status status
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_set_status()` sets the global error status maintained by the UDUNITS-2
library and returns the previously stored status value.

The error status is a global indicator used by UDUNITS-2 functions to report
success or failure conditions. While most library functions set this value
internally when an error occurs, this function allows applications to explicitly
override or initialize the status.

The status value is of type `ut_status`, an enumeration representing success
and error conditions defined by the library.

This function modifies global library state and does not interpret or enforce
constraints on the provided status value.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `status`  | The new `ut_status` value to assign as the global error status |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `ut_status`  | The previous global UDUNITS-2 status value |

## Preconditions

Before calling `ut_set_status()`:

- `status` should be a valid `ut_status` enumeration value defined by UDUNITS-2.

## Error Handling

This function does not fail and does not set an additional error condition.

It always returns the previously stored status. The global error status
(retrievable via [`ut_get_status()`](ut-get-status.html)) is updated to the
provided value.

## Object Ownership

`ut_set_status()` does not create, destroy, or manage any objects.

It modifies a global status value stored internally by the UDUNITS-2 library.

| Memory Allocation | Caller Responsibility |
| ---------------- | -------------------- |
| None | None |

## Thread Safety

`ut_set_status()` modifies a **global error status variable** maintained by
the UDUNITS-2 library.

As a result:

- Concurrent calls from multiple threads may overwrite the global status.
- The resulting status value may be non-deterministic in multithreaded environments.

UDUNITS-2 maintains a global error status that may be overwritten by concurrent
calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html)
should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how `ut_set_status()` sets and restores the global
error status.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    /* Save current status */
    ut_status previous = ut_get_status();

    /* Set a new status */
    ut_set_status(UT_SUCCESS);

    /* Verify change */
    ut_status current = ut_get_status();
    printf("Current status: %d\n", current);

    /* Restore previous status */
    ut_set_status(previous);

    return 0;
}
```

Example output:

```c
Current status: 0
```

## See Also

- [`ut_get_status()`](ut-get-status.html)
- [`ut_set_error_message_handler()`](ut-set-error-message-handler.html)
- [`ut_handle_error_message()`](ut-handle-error-message.html)
