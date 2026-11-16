---
title: Installation Layout
sidebar: sidebar
toc: true
permalink: installation-layout.html
---

This page describes the standard filesystem layout of an installed UDUNITS-2 distribution.

It is a reference for understanding where UDUNITS-2 executables, headers, libraries, data files, and documentation are normally installed. The exact locations depend on the installation method and platform.

For installation instructions, see:

* [Building from Source](building-from-source.html)
* [Package Manager Installation](package-manager-installation.html)
* [Conda Installation](conda-installation.html)

## Installation Prefix

A source installation is organized beneath an **installation prefix**.

The default prefix is typically:

```text
/usr/local
```

A custom prefix can be selected when configuring a source build, for example:

```bash
./configure --prefix=/opt/udunits
```

The prefix forms the root of the installation layout:

```text
<prefix>/
├── bin/
├── include/
├── lib/
└── share/
```

See [Configure Script Options](configure-options.html) for options that change individual installation directories.

## Directory Layout

| Directory        | Contents                                        |
| ---------------- | ----------------------------------------------- |
| `bin/`           | Executable programs                             |
| `include/`       | C header files                                  |
| `lib/`           | Libraries                                       |
| `share/`         | Architecture-independent data and documentation |
| `share/udunits/` | UDUNITS-2 units database                        |
| `share/man/`     | Manual pages, when installed                    |
| `share/doc/`     | Documentation, when installed                   |

The primary UDUNITS-2 installation therefore has the following structure:

```text
<prefix>/
├── bin/
│   └── udunits2
├── include/
│   └── udunits2.h
├── lib/
│   └── libudunits2.*
└── share/
    ├── udunits/
    │   └── udunits2.xml
    ├── man/
    └── doc/
```

Not every installation contains every directory. For example, documentation and manual pages may be omitted by some packages.

## Installed Files

The principal UDUNITS-2 files are:

| File                         | Purpose                        |
| ---------------------------- | ------------------------------ |
| `bin/udunits2`               | UDUNITS-2 command-line utility |
| `include/udunits2.h`         | Public C API header            |
| `lib/libudunits2.*`          | UDUNITS-2 library              |
| `share/udunits/udunits2.xml` | Standard units database        |

The library and executable filenames vary by platform and library type. For example, a shared library may use a platform-specific extension such as `.so`, `.dylib`, or `.dll`.

## Units Database

The standard units database is installed separately from the library and header:

```text
<prefix>/share/udunits/udunits2.xml
```

The database defines the units, prefixes, aliases, constants, and other definitions available to UDUNITS-2.

See [Units Database](units-database.html) for information about the database itself and [XML Units Database Reference](xml-units-database-reference.html) for its contents.

## Platform and Installation-Method Variations

The directory structure described above is the conventional layout for a source installation.

System package managers generally integrate UDUNITS-2 into the operating system's existing filesystem hierarchy. Consequently, the files may be distributed across system directories rather than sharing a single visible prefix.

Conda installations place the complete installation inside the active Conda environment. The environment prefix serves the same general role as the source-installation prefix:

```text
$CONDA_PREFIX/
├── bin/
├── include/
├── lib/
└── share/
```

On Windows, Conda uses its platform-specific `Library` directories.

For the actual paths used by each installation method, see the corresponding installation guide:

* [Building from Source](building-from-source.html)
* [Package Manager Installation](package-manager-installation.html)
* [Conda Installation](conda-installation.html)

## Custom Installation Layouts

Source installations can place individual classes of files in directories different from the default layout.

For example:

```bash
./configure \
    --prefix=/opt/udunits \
    --libdir=/opt/udunits/lib64
```

This produces a layout in which the libraries are installed under:

```text
/opt/udunits/lib64/
```

while the other components remain relative to the configured prefix unless separately overridden.

See [Configure Script Options](configure-options.html) for the available installation-directory options.

## Multiple Installations

More than one UDUNITS-2 installation can exist on the same system. Common examples include:

* a system package-manager installation;
* a source installation under a custom prefix;
* a Conda environment.

The installations are independent filesystem trees and may contain different UDUNITS-2 versions.

When working with multiple installations, the relevant executable, header, library, and database must all come from the intended installation.

For compiler and linker search paths, see [Compiler & Linker Flags](compiler-linker-flags.html).

For environment variables that affect runtime behavior, see [Environment Variables](environment-variables.html).

## Summary

A source installation follows this general pattern:

```text
<prefix>/
├── bin/udunits2
├── include/udunits2.h
├── lib/libudunits2.*
└── share/udunits/udunits2.xml
```

The **installation prefix** identifies the root of the installation, while the standard subdirectories identify the type of installed component.

The precise paths may differ for system packages, Conda environments, operating systems, and custom source installations.

## See Also

* [Installation: Building from Source](building-from-source.html)
* [Installation: Package Manager Installation](package-manager-installation.html)
* [Installation: Conda Installation](conda-installation.html)
* [Reference: Configure Script Options](configure-options.html)
* [Reference: Compiler & Linker Flags](compiler-linker-flags.html)
* [Reference: Environment Variables](environment-variables.html)
* [User Guide: Units Database](units-database.html)
