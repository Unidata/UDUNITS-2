---
title: ut_ignore()
sidebar: sidebar
toc: true
permalink: ut-ignore.html
---

Suppresses UDUNITS-2 error messages that match a specified regular expression pattern.

## Prototype

```c
int ut_ignore(
    const char* pattern
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_ignore()` suppresses diagnostic messages that match a specified
regular expression pattern.

Subsequent error messages whose text matches the given pattern are not reported.

This is useful for selectively silencing expected or non-critical warnings without
disabling error handling entirely.

The pattern is interpreted as a POSIX extended regular expression.

The effect is global to the UDUNITS-2 library and persists until changed by another
call to an error-handling function (e.g., [`ut_set_error_message_handler()`](ut-set-error-message-handler.html)).

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `pattern` | A null-terminated string containing a POSIX extended regular expression. Matching error messages will be suppressed. Passing `NULL` results in undefined behavior. |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `0`          | Success     |
| `-1`         | Failure     |

## Preconditions

- `pattern` must be a valid, null-terminated string.
- The string must represent a valid POSIX extended regular expression.

## Error Handling

Returns `-1` on failure.

Additional error information can be retrieved using [`ut_get_status()`](ut-get-status.html).

## Object Ownership

- The function does not take ownership of `pattern`.
- The string is not modified or retained beyond internal use.
- No dynamic memory is exposed to the caller.

## Thread Safety

This function modifies global error-handling state and is not thread-safe.

Applications must use external synchronization when calling this function
in multithreaded contexts.

## Example

This example demonstrates how `ut_ignore()` suppresses UDUNITS-2 error messages
that match a specified pattern.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    /* Suppress messages containing the word "unknown" */
    if (ut_ignore(".*unknown.*") != 0) {
        fprintf(stderr, "Failed to set ignore pattern\n");
        return 1;
    }

    ut_system* system = ut_read_xml(NULL);
    if (system == NULL) {
        fprintf(stderr, "Failed to load unit system\n");
        return 1;
    }

    /* This may generate a suppressed warning if it matches the pattern */
    ut_unit* unit = ut_parse(system, "unknown_unit", UT_UTF8);

    if (unit == NULL) {
        printf("Unit parsing failed (message may have been suppressed)\n");
    }

    ut_free(unit);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_set_error_message_handler()`](ut-set-error-message-handler.html)
- [`ut_write_to_stderr()`](ut-write-to-stderr.html)
- [`ut_handle_error_message()`](ut-handle-error-message.html)
- [`ut_get_status()`](ut-get-status.html)
