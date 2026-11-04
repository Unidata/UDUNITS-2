---
title: Units Database
sidebar: sidebar
toc: true
permalink: units-database.html
---



UDUNITS-2 uses a units database to define the units, prefixes, aliases, and other definitions that can be used by a unit system.
The standard database is distributed with UDUNITS-2 and provides the vocabulary used by most applications.

## Database Contents

The database can contain definitions for:

- **Base units** — fundamental units such as the meter, second, and kilogram.
- **Derived units** — units defined in terms of other units, such as the newton.
- **Prefixes** — prefixes such as `kilo-`, `milli-`, and `micro-`.
- **Aliases** — alternate names or symbols for units.
- **Constants** — named numerical values used in unit definitions.
- **Reference time definitions** — definitions used for date and time expressions.

See the **[XML Units Database Reference](xml-units-database-reference.html)** for the definitions included in the standard database.

## Using the Standard Database

For normal installations, no database customization is required.
UDUNITS-2 loads its standard units database when a unit system is created.

The database must be installed and accessible to the library.
If it cannot be found or opened, applications can report an error such as:

```text
Couldn't open units database
```

See [Installation Layout](installation-layout.html) for information about where the database is installed.

## Customizing the Database

A custom database can be used when an application needs units or definitions that are not provided by the standard database
Custom definitions can add units, aliases, prefixes, constants, or other definitions supported by the database format.

Custom databases should be used when the required vocabulary is application- or domain-specific rather than changing the meaning of existing standard units.

See the **[XML Units Database Reference](xml-units-database-reference.html)** for the definitions included in the standard database.

## Troubleshooting

### Database Not Found

This problem occurs when UDUNITS-2 cannot locate or access the units database required to create a unit system.

**Error Message:** `Couldn't open units database`
: **Cause:** The UDUNITS-2 XML units database cannot be found or opened.
: **Solution:** Verify that the database is installed and accessible. See [Installation Layout](installation-layout.html) for the expected database location.
{: .symptom}

### Unit Not Recognized

This problem occurs when UDUNITS-2 cannot find a unit name or symbol in the loaded database or the unit expression contains an incorrect name or symbol.

**Symptom:** A unit expression cannot be parsed or a unit name or symbol is not recognized.
: **Cause:** The unit or symbol is not defined in the loaded database, or its spelling or capitalization is incorrect.
: **Solution:** Check the [XML Units Database Reference](xml-units-database-reference.html) to verify that the unit is defined, then check its spelling and capitalization.
{: .symptom}

### Custom Database Issues

These problems occur when a custom units database contains a definition that UDUNITS-2 cannot recognize or when the intended custom database is not being loaded.

**Symptom:** A definition in a custom database is not recognized.
: **Cause:** The custom database definition is invalid, or the intended database is not being loaded.
: **Solution:** Verify the definition and confirm that the intended database is being loaded.
{: .symptom}

## See Also

- **[Concepts: Unit Systems](concepts-unit-systems.html)**
- **[User Guide: Unit Expression Language](unit-expression-language.html)**
- **[Reference: XML Units Database Reference](xml-units-database-reference.html)**
