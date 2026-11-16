---
title: UDUNITS-1 Data Structures
sidebar: sidebar
toc: true
permalink: udunits-1-data-structures.html
---

The UDUNITS-1 API exposes a minimal set of data structures. These types are
retained for backward compatibility and internally wrap the UDUNITS-2 object
model.

Unlike UDUNITS-2, the legacy API does not provide a rich set of opaque object
types. Instead, most functionality operates on a single primary structure
representing a unit definition.

Because UDUNITS-1 is deprecated, new applications should prefer the
UDUNITS-2 API, which provides a more flexible and fully encapsulated
object model.

## Data Structures

The following types are declared in the public header file
[`udunits.h`](https://github.com/Unidata/UDUNITS-2/blob/main/lib/udunits.h):

| Type | Description |
| ------ | ------------- |
| [`utUnit`](utunit.html) | Represents a unit definition. Internally wraps a UDUNITS-2 [`ut_unit`](ut-unit.html) object |
| `UtFactor` | Floating-point scale factor used in unit conversions |
| `UtOrigin` | Floating-point origin offset used in time-based units |

### Structural Characteristics

- **Compatibility wrapper** — `utUnit` internally references a UDUNITS-2 `ut_unit`
- **Limited object model** — the legacy API exposes only a single primary object
- **Library-managed lifecycle** — objects should only be created, modified, and destroyed using UDUNITS-1 API functions
- **Not intended for direct field access** — internal structure may change between versions
- **Migration encouraged** — the [UDUNITS-2 API](c-api-overview.html#api-organization) provides fully opaque object types and expanded functionality

## Relationship to UDUNITS-2 Objects

The UDUNITS-1 data structures are implemented as thin wrappers over the
UDUNITS-2 object model:

| UDUNITS-1 | UDUNITS-2 equivalent |
| ----------- | ---------------------- |
| `utUnit` | [`ut_unit`](ut-unit.html) |
| conversion coefficients | [`cv_converter`](cv-converter.html) (implicit) |
| global unit context | [`ut_system`](ut-system.html) (implicit) |

## See Also

- [UDUNITS-2 Data Structures](data-structures.html)
- [`utScan()`](utscan.html)
- [`utConvert()`](utconvert.html)
- [`utPrint()`](utprint.html)
- [`utFree()`](utfree.html)
