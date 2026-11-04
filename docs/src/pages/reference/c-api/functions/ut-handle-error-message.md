---
title: ut_handle_error_message()
sidebar: sidebar
toc: true
permalink: ut-handle-error-message.html
---

Formats an error message and dispatches it to the currently configured UDUNITS-2 error message handler.

## Prototype

```c
void ut_handle_error_message(
    const char* fmt,
    ...
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_handle_error_message()` formats and dispatches an error message using the
currently configured UDUNITS-2 error message handler.

The function accepts a `printf`-style format string and corresponding arguments,
formats the message, and immediately forwards it to the active error message
handler. The handler is typically set via [`ut_set_error_message_handler()`](ut-set-error-message-handler.html).

This function is used internally by the UDUNITS-2 library to report errors, but
it is also available for application code that wishes to emit messages consistent
with the library’s error-handling mechanism.

The function does not modify the global error status directly; it is responsible
only for message reporting.

## Parameters

| Parameter | Description                                                          |
| --------- | -------------------------------------------------------------------- |
| `fmt`     | A `printf`-style format string describing the error message          |
| `...`     | Additional arguments corresponding to the format specifiers in `fmt` |

## Returns

| Return Value | Description                           |
| ------------ | ------------------------------------- |
| `void`       | This function does not return a value |

## Preconditions

- `fmt` must be a valid, null-terminated format string.
- Any additional arguments must match the format specifiers in `fmt`.
- A custom error message handler can be set via [`ut_set_error_message_handler()`](ut-set-error-message-handler.html).
- Otherwise, the default handler will be used.

## Error Handling

This function does not set or return an error status.

Instead, it formats and forwards an error message to the currently configured error message handler.

The global error status (retrievable via [`ut_get_status()`](ut-get-status.html)) is managed independently by other UDUNITS-2 functions.

If no custom handler has been set, the default handler is used.

Improper format strings or mismatched arguments result in undefined behavior (as with standard `printf`).

## Object Ownership

`ut_handle_error_message()` does not allocate or return objects.

| Memory Allocation | Caller Responsibility |
| ----------------- | --------------------- |
| None              | None                  |

## Thread Safety

`ut_handle_error_message()` relies on a **global error message handler**.

As a result:

- The function itself can be called safely
- However, behavior may be **non-deterministic in multithreaded environments** if:
  - Multiple threads modify the handler
  - Multiple threads emit messages concurrently

In multithreaded applications:

- Set the error handler once during initialization
- Use external synchronization if consistent message ordering is required

## Example

This example demonstrates how `ut_handle_error_message()` is used to report errors
when UDUNITS-2 operations fail.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        ut_handle_error_message("Failed to initialize UDUNITS-2 system\n");
        return 1;
    }

    ut_unit* unit = ut_get_unit_by_name(system, "invalid_unit");

    if (unit == NULL) {
        ut_handle_error_message("Could not find unit: %s\n", "invalid_unit");

        ut_status status = ut_get_status();
        fprintf(stderr, "Status code: %d\n", status);
    }

    ut_free_system(system);
    return 0;
}
```

## See Also

- [`ut_set_error_message_handler()`](ut-set-error-message-handler.html)
- [`ut_get_status()`](ut-get-status.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
