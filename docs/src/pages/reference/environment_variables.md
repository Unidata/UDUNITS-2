---
title: Environment Variables
sidebar: sidebar
toc: true
permalink: environment-variables.html
---

This page provides a reference for environment variables that affect the **configuration, installation, and runtime behavior of UDUNITS-2**.

Most variables listed here are standard operating-system, compiler, or build-system variables rather than UDUNITS-2-specific variables.

## Quick Reference

| Variable            | Purpose                                             | Context   |
| ------------------- | --------------------------------------------------- | --------- |
| `UDUNITS2_XML_PATH` | Select the XML units database                       | Runtime   |
| `PATH`              | Locate the `udunits2` executable and other programs | Runtime   |
| `LD_LIBRARY_PATH`   | Add directories to the shared-library search path   | Runtime   |
| `CC`                | Select the C compiler                               | Autotools |
| `CFLAGS`            | Specify C compiler options                          | Autotools |
| `CPPFLAGS`          | Specify C preprocessor options and include paths    | Autotools |
| `LDFLAGS`           | Specify linker options and library paths            | Autotools |
| `LIBS`              | Specify additional libraries                        | Autotools |
| `CONDA_PREFIX`      | Identify the active Conda environment               | Conda     |

## UDUNITS-2 Environment Variables

### `UDUNITS2_XML_PATH`

`UDUNITS2_XML_PATH` specifies the location of the XML units database used by UDUNITS-2.

It is particularly useful when:

- the database is installed in a non-standard location;
- multiple UDUNITS-2 installations are present;
- testing a custom units database;
- developing UDUNITS-2 from a source tree.

For example:

```bash
export UDUNITS2_XML_PATH=/opt/udunits/share/udunits/udunits2.xml
```

An application that calls:

```c
ut_system* sys = ut_read_xml(NULL);
```

can then use the database specified by the environment variable.

A database can also be selected for a single command without changing the shell environment:

```bash
UDUNITS2_XML_PATH=/path/to/udunits2.xml udunits2
```

If `UDUNITS2_XML_PATH` is not set, UDUNITS-2 uses its normal database lookup behavior.

### Source-Tree Development

The UDUNITS-2 development and test workflows can use `UDUNITS2_XML_PATH` to select the units database in the source tree.

This allows tests to run against the database being developed rather than an installed system database.

### Custom Units Database

Applications can use `UDUNITS2_XML_PATH` when testing or deploying a custom units database.

For example:

```bash
UDUNITS2_XML_PATH=/path/to/custom/udunits2.xml ./my-program
```

See [Units Database](units-database.html) for information about the contents and customization of the units database.

## Runtime Environment Variables

### `PATH`

`PATH` is the standard shell environment variable used to locate executable programs.

It determines which `udunits2` executable is run when the command is entered without an explicit pathname.

Check the executable being used with:

```bash
which udunits2
```

This is especially important when multiple installations are present, such as a system package-manager installation, Conda installation, or manually built installation.

For a custom installation:

```bash
export PATH=/opt/udunits/bin:$PATH
```

The installation guides discuss `PATH` as part of troubleshooting multiple UDUNITS-2 installations.

### `LD_LIBRARY_PATH`

`LD_LIBRARY_PATH` can be used on Linux and other systems with a compatible dynamic loader to add directories to the runtime shared-library search path.

For example:

```bash
export LD_LIBRARY_PATH=/opt/udunits/lib:$LD_LIBRARY_PATH
```

This can be useful when `libudunits2.so` is installed in a non-standard directory.

Use:

```bash
ldd example
```

to inspect the shared libraries required by an application.

For system-wide installations, configuring the system's library search mechanism may be preferable to setting `LD_LIBRARY_PATH`.

`LD_LIBRARY_PATH` is an operating-system mechanism; it is not interpreted directly by the UDUNITS-2 library.
See [Building Applications](building-applications.html) for runtime library configuration.

## Build Environment Variables

UDUNITS-2's Autotools build accepts standard compiler and linker environment variables.

These variables are summarized here for reference.
For detailed descriptions and examples, see [Compiler & Linker Flags](compiler-linker-flags.html).

### `CC`

Selects the C compiler used by `configure`.

```bash
CC=clang ./configure
```

### `CFLAGS`

Specifies options passed to the C compiler.

```bash
CFLAGS="-O2" ./configure
```

### `CPPFLAGS`

Specifies C preprocessor options, commonly including additional header search paths.

```bash
CPPFLAGS="-I/opt/udunits/include" ./configure
```

### `LDFLAGS`

Specifies linker options, commonly including additional library search paths.

```bash
LDFLAGS="-L/opt/udunits/lib" ./configure
```

### `LIBS`

Specifies additional libraries passed to the linker.

```bash
LIBS="-lm" ./configure
```

These variables can be combined:

```bash
CC=clang \
CFLAGS="-O2" \
CPPFLAGS="-I/opt/udunits/include" \
LDFLAGS="-L/opt/udunits/lib" \
./configure
```

The [Compiler & Linker Flags](compiler-linker-flags.html) reference provides detailed information about these variables and their relationship to compiler and linker options.

The [Configure Script Options](configure-options.html) reference also describes their use with `configure`.

## Conda Environment Variables

### `CONDA_PREFIX`

`CONDA_PREFIX` identifies the currently active Conda environment.

When UDUNITS-2 is installed through Conda, its files are normally located relative to this directory:

```text
$CONDA_PREFIX/bin/udunits2
$CONDA_PREFIX/include/udunits2.h
$CONDA_PREFIX/lib/libudunits2.*
$CONDA_PREFIX/share/udunits/udunits2.xml
```

For example:

```bash
echo "$CONDA_PREFIX"
```

The Conda installation guide uses these locations when describing the installed components.

If the `udunits2` command is not found, verify that the environment containing UDUNITS-2 is active:

```bash
conda info --envs
conda activate udunits-env
```

See [Conda Installation](conda-installation.html) for complete installation instructions.

## Environment Variables and Installation Type

The variables most likely to be relevant depend on how UDUNITS-2 was installed.

| Installation                | Commonly relevant variables                    |
| --------------------------- | ---------------------------------------------- |
| System package manager      | `PATH`, `LD_LIBRARY_PATH`, `UDUNITS2_XML_PATH` |
| Source installation         | `PATH`, `LD_LIBRARY_PATH`, `UDUNITS2_XML_PATH` |
| Autotools development build | `CC`, `CFLAGS`, `CPPFLAGS`, `LDFLAGS`, `LIBS`  |
| Conda                       | `CONDA_PREFIX`, `PATH`, `UDUNITS2_XML_PATH`    |

Not every variable is required for every installation.

## See Also

- **[Getting Started: Building from Source](building-from-source.html)**
- **[Getting Started: Package Manager Installation](package-manager-installation.html)**
- **[Getting Started: Conda Installation](conda-installation.html)**
- **[Developer Guide: Building Applications](building-applications.html)**
- **[Developer Guide: Developing UDUNITS-2](developing-udunits.html)**
- **[Reference: Compiler & Linker Flags](compiler-linker-flags.html)**
- **[Reference: Configure Script Options](configure-options.html)**
- **[User Guide: Units Database](units-database.html)**
