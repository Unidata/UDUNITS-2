---
title: ut_set_error_message_handler()
sidebar: sidebar
toc: true
permalink: ut-set-error-message-handler.html
---

Sets the UDUNITS-2 error message handler and returns the previously configured handler.

## Prototype

```c
ut_error_message_handler ut_set_error_message_handler(
    ut_error_message_handler handler
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_set_error_message_handler()` sets the function used by UDUNITS-2 to handle
error messages and returns the previously installed handler.

The error message handler is a global callback used by the library to report
diagnostic messages. Functions such as [`ut_handle_error_message()`](ut-handle-error-message.html) format and
dispatch messages through this handler.

By default, UDUNITS-2 uses an internal handler. Applications can override this
behavior by installing a custom handler to redirect messages (e.g., to a log
file, GUI, or structured logging system).

Passing `NULL` restores the library’s internal default error message handler.

This function does not generate or dispatch error messages; it only configures
how such messages are handled.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `handler` | Pointer to a function with signature `void handler(const char* message)` that processes formatted error messages, or `NULL` to restore the default handler |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `ut_error_message_handler` | The previously installed error message handler (may be `NULL` if none was set) |

## Preconditions

Before calling `ut_set_error_message_handler()`:

- `handler` should be a valid function pointer matching the expected handler signature, or `NULL`.
- The function may be called at any time, but is typically set during application initialization.

## Error Handling

This function does not fail and does not set an error status.

It always returns the previously configured handler. The global error status
(retrievable via [`ut_get_status()`](ut-get-status.html)) is not affected.

## Object Ownership

`ut_set_error_message_handler()` does not allocate or free objects.

It stores a function pointer in internal global state.

| Memory Allocation | Caller Responsibility |
| ---------------- | -------------------- |
| None | None |

## Thread Safety

`ut_set_error_message_handler()` modifies a global error message handler.

As a result:

- Concurrent calls that modify the handler are not inherently synchronized.
- Changing the handler while other threads are emitting messages may result in
  non-deterministic behavior.

The error message handler operates independently of the global error status, but
both rely on shared global state and may be affected by concurrent UDUNITS-2 calls.

Applications should:

- Set the handler once during initialization when possible.
- Use external synchronization if modifying the handler at runtime.

## Example

This example demonstrates how `ut_set_error_message_handler()` installs a custom error message handler and restores the previous handler.

```c
#include <stdio.h>
#include <udunits2.h>

void my_error_handler(const char* message)
{
    fprintf(stderr, "UDUNITS-2 ERROR: %s", message);
}

int main(void)
{
    /* Install custom handler */
    ut_error_message_handler previous =
        ut_set_error_message_handler(my_error_handler);

    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        /* Error will be routed through custom handler */
        return 1;
    }

    ut_unit* unit = ut_get_unit_by_name(system, "invalid_unit");
    if (unit == NULL) {
        /* Message handled by custom handler */
    }

    ut_free_system(system);

    /* Restore previous handler */
    ut_set_error_message_handler(previous);

    return 0;
}
```

## See Also

- [`ut_handle_error_message()`](ut-handle-error-message.html)
- [`ut_get_status()`](ut-get-status.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_get_unit_by_name()`](ut-get-unit-by-name.html)
