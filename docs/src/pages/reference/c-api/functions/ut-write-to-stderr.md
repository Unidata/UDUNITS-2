---
title: ut_write_to_stderr()
sidebar: sidebar
toc: true
permalink: ut-write-to-stderr.html
---

Writes a formatted error message to the standard error stream (`stderr`).

## Prototype

```c
void ut_write_to_stderr(
    const char* message
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_write_to_stderr()` is a built-in UDUNITS-2 error message handler that writes a message directly to the standard error stream (`stderr`).

This function is typically used as the default error message handler for the library. It receives a fully formatted message (usually from [`ut_handle_error_message()`](ut-handle-error-message.html)) and outputs it without modification.

Applications may explicitly install this handler using [`ut_set_error_message_handler()`](ut-set-error-message-handler.html) to restore default behavior or ensure that messages are written to `stderr`.

The function does not format messages itself and does not modify the global error status.

## Parameters

| Parameter | Description                                                                               |
| --------- | ----------------------------------------------------------------------------------------- |
| `message` | A null-terminated string containing the formatted error message to be written to `stderr` |

## Returns

| Return Value | Description                           |
| ------------ | ------------------------------------- |
| `void`       | This function does not return a value |

## Preconditions

Before calling `ut_write_to_stderr()`:

- `message` should be a valid, null-terminated string.
- The function is typically invoked indirectly via [`ut_handle_error_message()`](ut-handle-error-message.html).

## Error Handling

This function does not have defined error-reporting behavior.

It performs a best-effort write to `stderr`. Any I/O errors are not reported through the UDUNITS-2 status mechanism.

The global error status (retrievable via [`ut_get_status()`](ut-get-status.html)) is not modified.

## Object Ownership

`ut_write_to_stderr()` does not allocate, modify, or free objects.

| Memory Allocation | Caller Responsibility |
|------------------|----------------------|
| None             | None                 |

## Thread Safety

`ut_write_to_stderr()` writes to the shared `stderr` stream.

As a result:

- The function itself can be safely called.
- Output from multiple threads may interleave if called concurrently.

UDUNITS-2 also maintains a global error message handler:

- If multiple threads emit messages simultaneously, ordering is not guaranteed.
- External synchronization is recommended for deterministic output.

Additionally, UDUNITS-2 maintains a global error status that may be overwritten by concurrent calls to other library functions. Applications that rely on [`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure deterministic behavior.

## Example

This example demonstrates how to install `ut_write_to_stderr()` as the error message handler.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    /* Set stderr handler explicitly */
    ut_set_error_message_handler(ut_write_to_stderr);

    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        /* Error message automatically written to stderr */
        return 1;
    }

    ut_unit* unit = ut_get_unit_by_name(system, "invalid_unit");
    if (unit == NULL) {
        /* Error message written to stderr via handler */
        ut_status status = ut_get_status();
        fprintf(stderr, "Status code: %d\n", status);
    }

    ut_free_system(system);
    return 0;
}
```

Example output:

```c
<UDUNITS-2 error message written to stderr>
Status code: <non-zero>
```

## See Also

- [`ut_handle_error_message()`](ut-handle-error-message.html)
- [`ut_set_error_message_handler()`](ut-set-error-message-handler.html)
- [`ut_get_status()`](ut-get-status.html)
- [`ut_read_xml()`](ut-read-xml.html)
