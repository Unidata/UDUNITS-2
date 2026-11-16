---
title: Using the C API
sidebar: sidebar
toc: true
permalink: using-the-c-api.html
---

The UDUNITS-2 C API provides the programming interface for applications that need to parse, manipulate, compare, and convert scientific units.

The API supports unit systems loaded from XML databases, construction and manipulation of unit objects, conversion between compatible units, dimensional analysis, and time-related operations.

Applications using the C API include the public header:

```c
#include <udunits2.h>
```

and link against the UDUNITS-2 library.

See **[Building Applications](building-applications.html)** for information about compiling and linking an application against UDUNITS-2.

A typical application follows this general workflow:

1. Create or load a unit system.
2. Obtain or parse the units required by the application.
3. Examine or manipulate the resulting unit objects.
4. Create a converter when values need to be converted between compatible units.
5. Perform the conversion.
6. Release the objects when they are no longer needed.

For example, a simple application can load the standard units database and parse a unit expression:

```c
ut_system* sys = ut_read_xml(NULL);
ut_unit* meter = ut_parse(sys, "m", UT_UTF8);
```

A converter can then be created between compatible units:

```c
ut_unit* foot = ut_parse(sys, "ft", UT_UTF8);
cv_converter* cv = ut_get_converter(meter, foot);

double value = cv_convert_double(cv, 1.0);
```

Objects created by the API should be released when they are no longer needed:

```c
cv_free(cv);
ut_free(meter);
ut_free(foot);
ut_free_system(sys);
```

See **[Basic Examples](basic-examples.html)** for additional examples.

## Core API Objects

The C API uses opaque types to represent its primary objects.
Their internal structure is hidden from applications and objects are manipulated through API functions.

| Type | Purpose |
| --- | --- |
| `ut_system` | Represents a unit system and its unit definitions |
| `ut_unit` | Represents an individual unit |
| `cv_converter` | Represents a conversion between compatible units |

See **[Data Structures](data-structures.html)** for more information about these types, their relationships, and object ownership.

## API Documentation

The detailed C API documentation is organized by functional area.

| Topic | Description |
| --- | --- |
| [Data Structures](data-structures.html) | Core opaque types and object relationships |
| [Unit Systems API](unit-systems-api.html) | Create, load, and manage unit systems |
| [Unit Creation & Destruction API](unit-creation-destruction-api.html) | Create, clone, and release unit objects |
| [Unit Parsing & Formatting API](unit-parsing-formatting-api.html) | Parse unit expressions and format units as strings |
| [Unit Arithmetic API](unit-arithmetic-api.html) | Multiply, divide, invert, scale, offset, and otherwise manipulate units |
| [Unit Conversion API](unit-conversion-api.html) | Determine compatibility and create and use converters |
| [Unit Query & Introspection API](unit-query-introspection-api.html) | Inspect units, dimensions, names, symbols, and systems |
| [Unit Name & Symbols Mapping API](unit-name-symbols-mapping-api.html) | Map names and symbols to units and units to names and symbols |
| [Unit Time Utilities API](unit-time-utilities-api.html) | Encode and decode date and time values |
| [Extended Unit API](extended-unit-api.html) | Advanced unit operations, including logarithmic units |
| [Error Handling API](error-handling-api.html) | Handle status information and error messages |
| [Troubleshooting API Issues](troubleshooting-api-issues.html) | Diagnose common problems when using the C API |
| [C API Function Index](c-api-function-index.html) | Alphabetical index of public C API functions |

## Object Ownership

The C API manages the memory associated with its opaque objects.
Applications should use the appropriate API function to release objects rather than attempting to free them directly.

For example:

```c
cv_free(cv);
ut_free(unit);
ut_free_system(sys);
```

The detailed API documentation identifies ownership and lifetime requirements for individual functions.

See **[Data Structures](data-structures.html)** for the general object-lifecycle model.

## Error Handling

Many C API functions communicate failure through their return value and the UDUNITS-2 status and error-handling mechanisms.

When a function returns a pointer, applications should check for `NULL` before using the returned object.

For example:

```c
ut_unit* unit = ut_parse(sys, "m", UT_UTF8);

if (unit == NULL) {
    /* Handle the error */
}
```

See **[Error Handling API](error-handling-api.html)** for details about status values, error messages, and error handlers.

## Legacy API

UDUNITS-2 also provides a compatibility interface for applications written against UDUNITS-1.

The **UDUNITS-1 Legacy API** is retained for migration and compatibility purposes.
New applications should use the UDUNITS-2 API.

See **[UDUNITS-1 Legacy API](udunits-1-legacy-api.html)** for documentation of the legacy interface.

## Reference

The Developer Guide provides an introduction to using the C API.
For complete function-level documentation, including prototypes, parameters, return values, preconditions, error handling, ownership, thread-safety considerations, and examples, use the **[C API Function Index](c-api-function-index.html)** and the individual function reference pages.

## See Also

- **[Developer Guide: Building Applications](building-applications.html)**
- **[Getting Started: Basic Examples](basic-examples.html)**
- **[User Guide: Unit Expression Language](unit-expression-language.html)**
- **[User Guide: Units Database](units-database.html)**
- **[Reference: C API Function Index](c-api-function-index.html)**
