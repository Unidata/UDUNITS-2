---
title: ut_accept_visitor()
sidebar: sidebar
toc: true
permalink: ut-accept-visitor.html
---

Traverses a unit by applying a user-defined visitor that invokes type-specific callbacks
based on the unit's internal representation.

## Prototype

```c
int ut_accept_visitor(
    const ut_unit* unit, 
    const ut_visitor* visitor, 
    void* arg
);
```

## Header

```c
#include <udunits2.h>
```

## Description

Applies a user-defined visitor to a unit, enabling structured traversal
and inspection of the unit using the visitor pattern.

`ut_accept_visitor()` dispatches the provided `ut_visitor` to the specified
unit based on the unit's internal type. The appropriate callback function in
the visitor is invoked for that type (e.g., basic, scaled, offset, logarithmic,
or product units). This function performs only single dispatch on the given unit;
 recursive traversal occurs only if implemented by the visitor.

This function does not modify the unit. It serves as a dispatch mechanism that
allows custom logic—such as formatting, analysis, or transformation—to be applied
without exposing or altering internal representations.

Errors are propagated through the return values of visitor callback functions. If
any callback returns a non-zero value, traversal stops and that value is returned
to the caller.

This function is commonly used internally by UDUNITS-2 for operations such as unit
formatting, comparison, and expression generation, and can be used by applications
to implement custom unit-processing behavior.

## Parameters

| Parameter | Description                                                                                          |
| --------- | ---------------------------------------------------------------------------------------------------- |
| `unit`    | Pointer to a [`ut_unit`](ut-unit.html) object previously created by UDUNITS-2. Must not be `NULL`                    |
| `visitor` | Pointer to a `ut_visitor` structure containing callback functions for unit types. Must not be `NULL` |
| `arg`     | Optional user-defined pointer passed to visitor callback functions. May be `NULL`                    |

## Returns

| Return Value | Description                                                                                 |
| ------------ | ------------------------------------------------------------------------------------------- |
| `0`          | Success. The visitor was applied without error                                              |
| non-zero     | Failure. A visitor callback returned a non-zero value or an error occurred during traversal |

## Preconditions

- `unit` must be a valid [`ut_unit`](ut-unit.html) object created by UDUNITS-2.
- `visitor` must be a valid `ut_visitor` structure.
- The visitor must define callback functions appropriate for the types of units that may be visited.
- The unit system associated with `unit` must remain valid for the duration of the call.

## Error Handling

This function does not distinguish errors from a valid return value.
Use [`ut_get_status()`](ut-get-status.html) to determine whether an error occurred.

## Object Ownership

- The `unit` object is **not modified or freed**.
- The `visitor` object is **not modified or freed**.
- The `arg` pointer is passed through unchanged and is **not managed** by the function.

The caller retains full ownership of all inputs.

## Thread Safety

`ut_accept_visitor()` is **thread-safe**, provided that:

- The `unit` and `visitor` objects are not concurrently modified by other threads.
- Any shared state accessed via `arg` is properly synchronized by the caller.

The function itself maintains no global mutable state.

## Example

This example demonstrates how `ut_accept_visitor()` applies a user-defined visitor
to a unit, dispatching the visitor to the meter unit and invoking the appropriate
callback for its type.

```c
#include <stdio.h>
#include <udunits2.h>

static int visit_basic(const ut_unit* unit, void* arg) {
    printf("Visited a unit\n");
    return 0;
}

int main(void) {
    ut_system* sys = ut_read_xml(NULL);
    ut_unit* meter = ut_parse(sys, "meter", UT_ASCII);

    ut_visitor visitor = {0};
    visitor.visit_basic = visit_basic;

    int status = ut_accept_visitor(meter, &visitor, NULL);

    if (status != 0) {
        fprintf(stderr, "Visitor failed\n");
    }

    ut_free(meter);
    ut_free_system(sys);
    return 0;
}
```

## See Also

- [`ut_new_visitor()`](ut-new-visitor.html)
- [`ut_free_visitor()`](ut-free-visitor.html)
- [`ut_compare()`](ut-compare.html)
