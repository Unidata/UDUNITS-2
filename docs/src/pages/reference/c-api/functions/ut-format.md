---
title: ut_format()
sidebar: sidebar
toc: true
permalink: ut-format.html
---

Formats a unit into a string representation.

## Prototype

```c
int ut_format(
    const ut_unit* unit,
    char* buf,
    size_t size,
    unsigned opts
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_format()` converts a [`ut_unit`](ut-unit.html) into a formatted string representation.

It writes the textual form of the unit into a caller-provided buffer, using
the specified formatting options to control aspects such as encoding and
symbol vs. name representation.

- The output is written into the provided buffer; no memory is allocated.
- The result is null-terminated if `size > 0`, consistent with `snprintf()` semantics.
- If the buffer is too small, the output is truncated but still null-terminated.
- The return value indicates the total length of the formatted string,
  regardless of truncation.

## Parameters

| Parameter | Description |
| --------- | ----------- |
| `unit`    | Pointer to the unit to format. Must be a valid, non-NULL [`ut_unit`](ut-unit.html) |
| `buf`     | Pointer to the output buffer that will receive the formatted string |
| `size`    | Size of the buffer in bytes, including space for the null terminator |
| `opts`    | Formatting options (e.g., `UT_ASCII`, `UT_UTF8`, `UT_NAMES`) controlling output representation |

## Returns

| Return Value | Description |
| ------------ | ----------- |
| `>= 0`       | Number of characters that would have been written (excluding the null terminator), even if the output was truncated |
| `< 0`        | Failure. An error occurred (e.g., invalid arguments or internal error) |

## Preconditions

- `unit` must be a non-NULL pointer to a valid [`ut_unit`](ut-unit.html).
- `buf` must be a non-NULL pointer to writable memory.
- `size` should be greater than zero to ensure output is written.

## Error Handling

Returns a negative value on failure. The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

- The caller retains ownership of all inputs.
- The buffer `buf` is owned and managed by the caller.
- This function does not allocate or free any [`ut_unit`](ut-unit.html) objects.

## Thread Safety

Thread safety is not guaranteed by the library.

- This function does not modify the input unit.
- Units are associated with a shared [`ut_system`](ut-system.html), so concurrent use across
  threads may require external synchronization.

## Example

This example demonstrates how `ut_format()` converts a [`ut_unit`](ut-unit.html) into a formatted string.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* unit = ut_parse(system, "m/s", UT_UTF8);
    if (!unit) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    char buffer[64];
    int len = ut_format(unit, buffer, sizeof(buffer), UT_UTF8);

    if (len < 0) {
        fprintf(stderr, "Failed to format unit\n");
        ut_free(unit);
        ut_free_system(system);
        return 1;
    }

    printf("Formatted unit: %s\n", buffer);

    ut_free(unit);
    ut_free_system(system);

    return 0;
}
```

## See Also

- [`ut_parse()`](ut-parse.html)
- [`ut_clone()`](ut-clone.html)
