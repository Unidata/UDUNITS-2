---
title: Unit Name & Symbols Mapping API
sidebar: sidebar
toc: true
permalink: unit-name-symbols-mapping-api.html
---

Functions for managing mappings between units and their names or symbols.
Mappings are stored within the unit system and affect how units are parsed,
formatted, and displayed.

## Functions

| Function | Description |
| ---------- | ------------- |
| [`ut_add_name_prefix()`](ut-add-name-prefix.html) | Registers a textual prefix in a [`ut_system`](ut-system.html) for constructing derived unit names associated with compatible [`ut_unit`](ut-unit.html) objects |
| [`ut_add_symbol_prefix()`](ut-add-symbol-prefix.html) | Registers a symbolic prefix in a [`ut_system`](ut-system.html) for constructing derived unit symbols associated with compatible [`ut_unit`](ut-unit.html) objects |
| [`ut_map_name_to_unit()`](ut-map-name-to-unit.html) | Associates a textual name with a [`ut_unit`](ut-unit.html) within a [`ut_system`](ut-system.html), enabling lookup of units by canonical name |
| [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html) | Associates a symbolic abbreviation with a [`ut_unit`](ut-unit.html) within a [`ut_system`](ut-system.html), enabling lookup of units by symbol |
| [`ut_map_unit_to_name()`](ut-map-unit-to-name.html) | Defines or overrides the canonical name associated with a [`ut_unit`](ut-unit.html) in a [`ut_system`](ut-system.html) |
| [`ut_map_unit_to_symbol()`](ut-map-unit-to-symbol.html) | Defines or overrides the symbol associated with a [`ut_unit`](ut-unit.html) in a [`ut_system`](ut-system.html) |
| [`ut_unmap_name_to_unit()`](ut-unmap-name-to-unit.html) | Removes an existing name-to-`ut_unit` association from a [`ut_system`](ut-system.html) mapping |
| [`ut_unmap_symbol_to_unit()`](ut-unmap-symbol-to-unit.html) | Removes an existing symbol-to-`ut_unit` association from a [`ut_system`](ut-system.html) mapping |
| [`ut_unmap_unit_to_name()`](ut-unmap-unit-to-name.html) | Removes the canonical name associated with a [`ut_unit`](ut-unit.html) in a [`ut_system`](ut-system.html) |
| [`ut_unmap_unit_to_symbol()`](ut-unmap-unit-to-symbol.html) | Removes the symbol associated with a [`ut_unit`](ut-unit.html) in a [`ut_system`](ut-system.html) |

## Usage Patterns

The **Unit Name & Symbols Mapping API** operates as a progression from  
**`ut_system` → mapping definitions for [`ut_unit`](ut-unit.html) identifiers → prefix
registration rules → updated parsing and formatting behavior**,  enabling
 customization of how [`ut_unit`](ut-unit.html) objects are recognized, resolved, and
 represented within a unit system.

For example:

```text
(1) Obtain ut_system from one of the following
    • ut_read_xml() 
    • ut_new_system() 
                │
                ▼
(2) ╔════════════════════════════════════╗
    ║ Define or override ut_unit         ║
    ║ identifier mappings within         ║
    ║ a ut_system                        ║
    ║ • ut_map_name_to_unit()            ║
    ║ • ut_map_symbol_to_unit()          ║
    ║ • ut_map_unit_to_name()            ║
    ║ • ut_map_unit_to_symbol()          ║
    ╚════════════════════════════════════╝
                │
                ▼
(3) ╔════════════════════════════════════╗
    ║ Remove ut_unit identifier          ║
    ║ mappings (optional)                ║
    ║ • ut_unmap_name_to_unit()          ║
    ║ • ut_unmap_symbol_to_unit()        ║
    ║ • ut_unmap_unit_to_name()          ║
    ║ • ut_unmap_unit_to_symbol()        ║
    ╚════════════════════════════════════╝
                │
                ▼
(4) ╔════════════════════════════════════╗
    ║ Define prefix rules in ut_system   ║
    ║ for constructing derived ut_unit   ║
    ║ identifiers                        ║
    ║ • ut_add_name_prefix()             ║
    ║ • ut_add_symbol_prefix()           ║
    ╚════════════════════════════════════╝
                │
                ▼
(5) Parse or format ut_unit representations
    within the configured ut_system
    • ut_parse()  → ut_unit
    • ut_format() → string
```

1. **Obtain [`ut_system`](ut-system.html)**

   Operations begin with an existing or newly created [`ut_system`](ut-system.html) that will
   store name and symbol mappings associated with [`ut_unit`](ut-unit.html) objects.

   Typical entry points include:

   - [`ut_read_xml()`](ut-read-xml.html) loads a unit system definition into a [`ut_system`](ut-system.html)
   - [`ut_new_system()`](ut-new-system.html) creates an empty [`ut_system`](ut-system.html)

   The [`ut_system`](ut-system.html) acts as the container for all identifier mappings and
   prefix definitions applied to [`ut_unit`](ut-unit.html) objects.

2. **Define or override mappings**

   Custom identifiers can be associated with [`ut_unit`](ut-unit.html) objects to support
   domain-specific terminology, abbreviations, or legacy naming conventions
   within a [`ut_system`](ut-system.html).

   - [`ut_map_name_to_unit()`](ut-map-name-to-unit.html) associates a textual
     name with a [`ut_unit`](ut-unit.html) in a [`ut_system`](ut-system.html)
   - [`ut_map_symbol_to_unit()`](ut-map-symbol-to-unit.html) associates a
     symbol with a [`ut_unit`](ut-unit.html) in a [`ut_system`](ut-system.html)
   - [`ut_map_unit_to_name()`](ut-map-unit-to-name.html) defines the preferred
     canonical name for a [`ut_unit`](ut-unit.html)
   - [`ut_map_unit_to_symbol()`](ut-map-unit-to-symbol.html) defines the preferred
     symbol for a [`ut_unit`](ut-unit.html)

   These mappings influence how unit expressions are interpreted and how
   [`ut_unit`](ut-unit.html) objects are represented as strings.

3. **Remove or modify existing mappings (optional)**

   Existing identifier mappings within a [`ut_system`](ut-system.html) can be removed or replaced
   when adapting terminology or resolving conflicts between unit names or symbols.

   - [`ut_unmap_name_to_unit()`](ut-unmap-name-to-unit.html) removes a
     name-to-`ut_unit` association from a [`ut_system`](ut-system.html)
   - [`ut_unmap_symbol_to_unit()`](ut-unmap-symbol-to-unit.html) removes a
     symbol-to-`ut_unit` association from a [`ut_system`](ut-system.html)
   - [`ut_unmap_unit_to_name()`](ut-unmap-unit-to-name.html) removes the
     canonical name associated with a [`ut_unit`](ut-unit.html)
   - [`ut_unmap_unit_to_symbol()`](ut-unmap-unit-to-symbol.html) removes the
     symbol associated with a [`ut_unit`](ut-unit.html)

   This allows applications to override default definitions or eliminate
   ambiguous representations within the unit system.

4. **Define prefix rules**

   Prefix definitions extend the [`ut_system`](ut-system.html) with scalable naming and symbol
   construction rules that apply to compatible [`ut_unit`](ut-unit.html) objects.

   - [`ut_add_name_prefix()`](ut-add-name-prefix.html) registers a textual prefix
     used when forming derived unit names
   - [`ut_add_symbol_prefix()`](ut-add-symbol-prefix.html) registers a symbolic
     prefix used when forming derived unit symbols

   Prefix rules support consistent generation of related identifiers such as
   kilo-, milli-, micro-, or domain-specific naming conventions.

5. **Use updated mappings in parsing and formatting**

   Once mappings and prefixes are defined, they influence how unit expressions
   are interpreted within the [`ut_system`](ut-system.html) and how [`ut_unit`](ut-unit.html) objects are represented.

   - [`ut_parse()`](ut-parse.html) resolves mapped names and symbols to produce a [`ut_unit`](ut-unit.html)
   - [`ut_format()`](ut-format.html) generates string representations using mapped
     identifiers associated with a [`ut_unit`](ut-unit.html)

   These mappings ensure consistent terminology across parsing, formatting,
   and workflows that may subsequently create [`cv_converter`](cv-converter.html) objects for
   transformations between compatible units.

## See Also

- [`Unit Parsing & Formatting API`](unit-parsing-formatting-api.html)  
- [`Unit Systems API`](unit-systems-api.html)  
- [`Unit Query & Introspection API`](unit-query-introspection-api.html)
