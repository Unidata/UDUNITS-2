---
title: Unit Systems API
sidebar: sidebar
toc: true
permalink: unit-systems-api.html
---

Functions for creating, loading, managing, and destroying unit systems (`ut_system`).

A [`ut_system`](ut-system.html) object provides the context required for all parsing, lookup, and
conversion operations.

A valid system is required for most UDUNITS-2 operations, and units are bound to
their originating system.

## Functions

| Function | Description |
| -------- | ----------- |
| [`ut_free_system()`](ut-free-system.html) | Free memory associated with a [`ut_system`](ut-system.html), including structures referenced by related [`ut_unit`](ut-unit.html) and [`cv_converter`](cv-converter.html) objects |
| [`ut_get_system()`](ut-get-system.html) | Return the [`ut_system`](ut-system.html) to which a [`ut_unit`](ut-unit.html) belongs |
| [`ut_new_system()`](ut-new-system.html) | Create an empty [`ut_system`](ut-system.html) used to construct [`ut_unit`](ut-unit.html) objects |
| [`ut_read_xml()`](ut-read-xml.html) | Load unit definitions into a [`ut_system`](ut-system.html), enabling parsing into [`ut_unit`](ut-unit.html) objects and creation of compatible [`cv_converter`](cv-converter.html) objects |
| [`ut_same_system()`](ut-same-system.html) | Check whether two [`ut_unit`](ut-unit.html) objects originate from the same [`ut_system`](ut-system.html) for valid conversion via [`cv_converter`](cv-converter.html) |
| [`ut_set_second()`](ut-set-second.html) | Set the reference second used by a [`ut_system`](ut-system.html), affecting time-based [`ut_unit`](ut-unit.html) definitions and related [`cv_converter`](cv-converter.html) relationships |

## Usage Patterns

The **Unit Systems API** operates as a progression from **creation or loading
of a [`ut_system`](ut-system.html) → optional configuration of time reference → use of the system
across parsing and conversion operations → retrieval of the system associated
with [`ut_unit`](ut-unit.html) objects → validation of shared system context → resource cleanup**,
ensuring that all [`ut_unit`](ut-unit.html) and [`cv_converter`](cv-converter.html) objects are defined within a
consistent unit definition environment.

For example:

```text
(1) ╔══════════════════════════════╗
    ║ Create or load ut_system     ║
    ║ • ut_new_system()            ║
    ║            OR                ║
    ║ • ut_read_xml()              ║
    ╚══════════════════════════════╝
                │
                ▼
(2) ╔══════════════════════════════╗
    ║ Configure time reference     ║
    ║ within ut_system             ║
    ║ • ut_set_second()            ║
    ╚══════════════════════════════╝
                │
                ▼
(3) Use ut_system across operations
    • ut_parse()
    • ut_get_converter()
                │
                ▼
(4) ╔══════════════════════════════╗
    ║ Obtain ut_system from        ║
    ║ ut_unit                      ║
    ║ • ut_get_system()            ║
    ╚══════════════════════════════╝
                │
                ▼
(5) ╔══════════════════════════════╗
    ║ Validate compatible context  ║
    ║ for ut_unit objects          ║
    ║ • ut_same_system()           ║
    ╚══════════════════════════════╝
                │
                ▼
(6) ╔══════════════════════════════╗
    ║ Free ut_system resources     ║
    ║ • ut_free_system()           ║
    ╚══════════════════════════════╝
```

1. **Create or load [`ut_system`](ut-system.html)**

   Operations begin by obtaining a [`ut_system`](ut-system.html) that defines the unit namespace
   and relationships used throughout the UDUNITS-2 workflow.

   Typical entry points include:

   - [`ut_new_system()`](ut-new-system.html)
   - [`ut_read_xml()`](ut-read-xml.html)

   The resulting [`ut_system`](ut-system.html) provides the context required to construct
   compatible [`ut_unit`](ut-unit.html) objects and derive [`cv_converter`](cv-converter.html) objects.

2. **Configure time reference within [`ut_system`](ut-system.html)**

   The reference definition of the second may be customized to ensure consistent
   interpretation of time-based units.

   - [`ut_set_second()`](ut-set-second.html) sets the second used by the
     [`ut_system`](ut-system.html), influencing the definition of time-related [`ut_unit`](ut-unit.html) objects
     and associated [`cv_converter`](cv-converter.html) relationships.

   This configuration ensures consistent handling of temporal units within the
   same [`ut_system`](ut-system.html).

3. **Use [`ut_system`](ut-system.html) across parsing and conversion**

   The [`ut_system`](ut-system.html) is used to interpret unit expressions and establish
   relationships between compatible units.

   Common operations include:

   - [`ut_parse()`](ut-parse.html) creates [`ut_unit`](ut-unit.html) objects bound to the [`ut_system`](ut-system.html)
   - [`ut_get_converter()`](ut-get-converter.html) derives a [`cv_converter`](cv-converter.html)
     between compatible [`ut_unit`](ut-unit.html) objects defined within the same [`ut_system`](ut-system.html)

   All [`ut_unit`](ut-unit.html) and [`cv_converter`](cv-converter.html) objects remain associated with the
   originating [`ut_system`](ut-system.html).

4. **Obtain [`ut_system`](ut-system.html) from [`ut_unit`](ut-unit.html)**

   Applications may retrieve the originating [`ut_system`](ut-system.html) associated with a
   [`ut_unit`](ut-unit.html) when system context must be verified or reused.

   - [`ut_get_system()`](ut-get-system.html) returns the [`ut_system`](ut-system.html)
     associated with a given [`ut_unit`](ut-unit.html)

   Access to the originating [`ut_system`](ut-system.html) supports validation of dimensional
   interpretation and reuse of the unit definition context.

5. **Validate shared system context**

   Compatibility between [`ut_unit`](ut-unit.html) objects may be verified by confirming that
   both units originate from the same [`ut_system`](ut-system.html).

   - [`ut_same_system()`](ut-same-system.html) determines whether two
     [`ut_unit`](ut-unit.html) objects share a common [`ut_system`](ut-system.html), ensuring consistent
     dimensional interpretation and valid conversion relationships.

6. **Release [`ut_system`](ut-system.html) resources**

   When unit operations are complete, the [`ut_system`](ut-system.html) should be released to
   free associated resources.

   - [`ut_free_system()`](ut-free-system.html) frees memory associated with the
     [`ut_system`](ut-system.html), including internal structures referenced by related
     [`ut_unit`](ut-unit.html) and [`cv_converter`](cv-converter.html) objects

   Proper cleanup ensures efficient resource management within the application

## See Also

- [Unit Parsing & Formatting API](unit-parsing-formatting-api.html)  
- [Unit Conversion API](unit-conversion-api.html)
