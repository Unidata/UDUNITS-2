---
title: ut_trim()
sidebar: sidebar
toc: true
permalink: ut-trim.html
---

Trims leading and trailing whitespace from a string in place.

## Prototype

```c
char* ut_trim(
    char* string
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_trim()` removes leading and trailing whitespace characters from a
null-terminated string.

The trimming is performed **in place**, meaning the input string is
modified directly. Leading whitespace is skipped by advancing the
returned pointer, and trailing whitespace is removed by inserting a null
terminator at the appropriate position.

- The function does not allocate memory.
- The returned pointer may differ from the original pointer if leading
    whitespace is removed.
- Internal (non-leading/trailing) whitespace is preserved.
- Whitespace is determined using standard C character classification
    (e.g., `isspace()`).

This function is typically used to normalize strings prior to parsing.

## Parameters

| Parameter | Description |
|-----------|-------------|
| `string`  | Pointer to a mutable, null-terminated string to be trimmed |

## Returns

| Return Value | Description |
|--------------|-------------|
| `char*`      | Pointer to the trimmed string (may be advanced from the original pointer) |
| `NULL`       | Returned if `string` is `NULL` |

## Preconditions

Before calling `ut_trim()`:

- `string` should be a pointer to a mutable, null-terminated character
    array.
- The memory referenced by `string` should be writable.

## Error Handling

Returns `NULL` if the input is `NULL`. This function does not set an
error status.

## Object Ownership

`ut_trim()` does not create, destroy, or allocate objects.

- The input string is modified in place.
- No new memory is allocated.
- The caller retains ownership of the string.

| Memory Allocation | Caller Responsibility |
|-------------------|----------------------|
| None              | Ensure the string is writable and managed appropriately |

## Thread Safety

Thread-safe provided that:

- The input string is not accessed concurrently by multiple threads.

This function does not use shared global state.

## Example

This example demonstrates how `ut_trim()` removes leading and trailing
whitespace from a string.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    char str[] = "   meter per second   ";

    char* trimmed = ut_trim(str);

    if (trimmed) {
        printf("Trimmed string: '%s'\n", trimmed);
    } else {
        printf("Input string was NULL\n");
    }

    return 0;
}
```

Example output:

```c
Trimmed string: 'meter per second'
```

## See Also

- [`ut_parse()`](ut-parse.html)
- [`ut_format()`](ut-format.html)
