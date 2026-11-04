---
title: UDUNITS-2 C API Overview
sidebar: sidebar
toc: true
permalink: c-api-overview.html
---

The UDUNITS-2 C API provides a comprehensive framework for working with scientific
units, including parsing unit expressions, performing dimensional analysis,
converting between compatible units, and managing unit systems defined in XML databases.

The API is designed to support precise, programmatic unit handling in scientific,
engineering, and data-processing applications. It enables applications to interpret
unit strings, construct units algebraically, compute conversions, and validate
dimensional compatibility in a consistent and portable manner.

UDUNITS-2 improves upon the legacy UDUNITS-1 interface with:

- a clearer object model
- improved parsing and formatting behavior
- stronger dimensional consistency rules
- better separation between unit systems and unit objects
- expanded support for logarithmic and time-based units
- improved thread-safety characteristics

**New applications should use the UDUNITS-2 API whenever possible**.
The UDUNITS-1 Legacy API is provided for reference and migration purposes
but is not recommended for new development.

## API Organization

The UDUNITS-2 C API documentation is organized by functional area to help you quickly
locate relevant functions and concepts.

| Section | Description |
| ------ | ----------- |
| [Data Structures](data-structures.html) | Core object types and memory ownership rules |
| [Unit Systems API](unit-systems-api.html) | Loading and managing unit databases |
| [Unit Creation & Destruction API](unit-creation-destruction-api.html) | Constructing and releasing unit objects |
| [Unit Parsing & Formatting API](unit-parsing-formatting-api.html) | Converting between strings and unit objects |
| [Unit Arithmetic API](unit-arithmetic-api.html) | Combining and transforming units mathematically |
| [Unit Conversion API](unit-conversion-api.html) | Creating converters between compatible units |
| [Unit Query & Introspection API](unit-query-introspection-api.html) | Inspecting dimensional structure and compatibility |
| [Unit Name & Symbols Mapping API](unit-name-symbols-mapping-api.html) | Resolving symbols, names, and aliases |
| [Unit Time Utilities API](unit-time-utilities-api.html) | Encoding and decoding time-based units |
| [Extended Unit API](extended-unit-api.html) | Logarithmic and advanced unit functionality |
| [Error Handling API](error-handling-api.html) | Status reporting and diagnostic behavior |
| [Troubleshooting API Issues](troubleshooting-api-issues.html) | Troubleshooting guide for developers using the C API |
| [UDUNITS-1 Legacy API](udunits-1-legacy-api.html) | Deprecated interface retained for migration support |
| [C API Function Index](c-api-function-index.html) | Complete alphabetized list of functions in C API |
