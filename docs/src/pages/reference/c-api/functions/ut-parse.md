---
title: ut_parse()
sidebar: sidebar
toc: true
permalink: ut-parse.html
---

Parses a unit specification string into a [`ut_unit`](ut-unit.html) object.

## Prototype

```c
ut_unit* ut_parse(
    const ut_system* system,
    const char* spec,
    ut_encoding encoding
);
```

## Header

```c
#include <udunits2.h>
```

## Description

`ut_parse()` parses a textual unit specification and returns a
corresponding [`ut_unit`](ut-unit.html) object within the given unit system.

The function interprets the string `spec` according to the specified
character `encoding` and resolves unit identifiers and prefixes defined
in the given system. The specification must conform to the UDUNITS-2
unit expression grammar.

The specification may include:

- Base units (e.g., `"meter"`, `"second"`)
- Derived expressions (e.g., `"m/s^2"`, `"kg·m^2/s^2"`)
- Scaled units and prefixes (e.g., `"kilometer"`, `"cm"`)

The function performs both syntactic and semantic parsing. If
successful, it returns a newly allocated unit representing the parsed
expression.

The input string is not modified.

## Parameters

| Parameter  | Description                                                                                        |
| ---------- | -------------------------------------------------------------------------------------------------- |
| `system`   | Pointer to the unit system used to resolve unit definitions. Should reference a valid [`ut_system`](ut-system.html). |
| `spec`     | Null-terminated string containing the unit specification to parse. Should be a valid string.       |
| `encoding` | Encoding of the input string (e.g., `UT_UTF8`, `UT_ASCII`, `UT_LATIN1`).                           |

## Returns

| Return Value | Description                                                                                               |
| ------------ | --------------------------------------------------------------------------------------------------------- |
| non-NULL     | Pointer to a newly allocated [`ut_unit`](ut-unit.html) representing the parsed specification                              |
| `NULL`       | Parsing failed due to invalid syntax, unknown identifiers, unsupported encoding, or other internal errors |

## Preconditions

Before calling `ut_parse()`:

- `system` should reference a valid [`ut_system`](ut-system.html).
- `spec` should be a valid, null-terminated string.
- `encoding` should be a valid `ut_encoding` value supported by
    UDUNITS-2.

## Error Handling

Returns `NULL` on failure (e.g., syntax error, unknown unit, or encoding
issue). The specific error condition can be retrieved using
[`ut_get_status()`](ut-get-status.html).

## Object Ownership

`ut_parse()` creates a new [`ut_unit`](ut-unit.html) object.

- The returned `ut_unit*` is newly allocated and owned by the caller.
- The caller is responsible for freeing the returned unit using
    [`ut_free()`](ut-free.html).
- The input `system` and `spec` are not modified.

| Memory Allocation | Caller Responsibility               |
| ----------------- | ----------------------------------- |
| Yes               | Free returned unit with [`ut_free()`](ut-free.html) |

## Thread Safety

`ut_parse()` is not thread-safe when used with a shared [`ut_system`](ut-system.html).

UDUNITS-2 maintains a global error status that may be overwritten by
concurrent calls to other library functions. Applications that rely on
[`ut_get_status()`](ut-get-status.html) should use external synchronization to ensure
deterministic behavior.

## Example

This example demonstrates how `ut_parse()` converts a string into a unit
object.

```c
#include <stdio.h>
#include <udunits2.h>

int main(void) {
    ut_system* system = ut_read_xml(NULL);
    if (!system) {
        fprintf(stderr, "Failed to initialize unit system\n");
        return 1;
    }

    ut_unit* unit = ut_parse(system, "m/s^2", UT_UTF8);
    if (!unit) {
        fprintf(stderr, "Failed to parse unit\n");
        ut_free_system(system);
        return 1;
    }

    char buffer[128];
    if (ut_format(unit, UT_UTF8, buffer, sizeof(buffer)) >= 0) {
        printf("Parsed unit: %s\n", buffer);
    }

    ut_free(unit);
    ut_free_system(system);

    return 0;
}
```

Example output:

```c
Parsed unit: m.s-2
```

## See Also

- [`ut_format()`](ut-format.html)
- [`ut_read_xml()`](ut-read-xml.html)
- [`ut_get_status()`](ut-get-status.html)
- [`ut_free()`](ut-free.html)
- [`ut_clone()`](ut-clone.html)
- [`ut_compare()`](ut-compare.html)
