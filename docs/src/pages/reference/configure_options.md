---
title: Configure Script Options
sidebar: sidebar
toc: true
permalink: configure-options.html
---

This page describes the most commonly used **GNU `configure` script options** supported by UDUNITS-2.
These options allow you to customize installation locations, build features, compiler settings, and other aspects of the build process.

For a complete walkthrough of configuring, building, and installing UDUNITS-2, see **[Building from Source](building-from-source.html)**.

## Quick Start

Most installations require no special configuration:

```bash
./configure
```

To install into a custom location:

```bash
./configure --prefix=/opt/udunits
```

To see every option supported by your version of UDUNITS-2:

```bash
./configure --help
```

## Most Common Options

The majority of users only need a small number of configuration options.

| Option | Purpose |
| ------- | ------- |
| `--prefix=DIR` | Install somewhere other than `/usr/local` |
| `--enable-debug` | Build with debugging symbols |
| `--disable-udunits-1` | Disable the legacy UDUNITS-1 compatibility API |
| `--enable-static` | Build static libraries |
| `--disable-shared` | Disable shared libraries |
| `CPPFLAGS` / `LDFLAGS` | Specify non-standard include and library locations |

> **Note**
>
> Unless you have specific requirements, the default `./configure` command is sufficient.

## Configuration Workflow

A typical source build consists of three steps:

```bash
./configure
make
make install
```

Configuration options are supplied during the first step to customize the build.

## Installation Directories

These options control where UDUNITS-2 is installed.

The most commonly used option is:

```bash
./configure --prefix=/opt/udunits
```

| Option | Description | Default |
| ------- | ----------- | ------- |
| `--prefix=PREFIX` | Root installation directory | `/usr/local` |
| `--exec-prefix=PREFIX` | Architecture-dependent files | `PREFIX` |
| `--bindir=DIR` | Executables | `EPREFIX/bin` |
| `--libdir=DIR` | Libraries | `EPREFIX/lib` |
| `--includedir=DIR` | Header files | `PREFIX/include` |
| `--datadir=DIR` | Shared data | `PREFIX/share` |
| `--mandir=DIR` | Manual pages | `PREFIX/share/man` |
| `--docdir=DIR` | Documentation | `PREFIX/share/doc/udunits` |

Example:

```bash
./configure     --prefix=/opt/udunits     --libdir=/opt/udunits/lib64
```

## Build Features

These options enable or disable optional UDUNITS-2 build features.

| Option | Description |
| ------- | ----------- |
| `--enable-shared` | Build shared libraries |
| `--enable-static` | Build static libraries |
| `--enable-debug` | Enable debugging support |
| `--enable-coverage` | Enable code coverage instrumentation |
| `--disable-udunits-1` | Disable the legacy compatibility API |

## Environment Variables

Use environment variables when compilers, headers, or libraries are installed in non-standard locations.

| Variable | Purpose |
| -------- | ------- |
| `CC` | C compiler |
| `CFLAGS` | Compiler options |
| `CPPFLAGS` | Include search paths |
| `LDFLAGS` | Library search paths |
| `LIBS` | Additional libraries |

Examples:

```bash
CC=clang CFLAGS="-O3" ./configure
```

```bash
CPPFLAGS="-I/opt/include" LDFLAGS="-L/opt/lib" ./configure
```

For detailed information about compiler, preprocessor, and linker flags, see [Compiler & Linker Flags](compiler-linker-flags.html).


## System Types

These options are primarily intended for cross-compilation and are rarely required for native builds.

| Option | Description |
| ------- | ----------- |
| `--build=BUILD` | Build system type |
| `--host=HOST` | Target system type |

Example:

```bash
./configure --host=aarch64-linux-gnu
```

## Program Name Customization

These options rename installed executables and are mainly useful when maintaining multiple installations.

| Option | Description |
| ------- | ----------- |
| `--program-prefix=PREFIX` | Add a prefix to executable names |
| `--program-suffix=SUFFIX` | Add a suffix to executable names |
| `--program-transform-name=SED` | Apply a sed transformation |

## Optional Packages

These generic Autotools options are primarily intended for package maintainers.

| Option | Description |
| ------- | ----------- |
| `--with-PACKAGE` | Use a specified package |
| `--without-PACKAGE` | Disable a package |

## Common Configuration Scenarios

### Standard installation

```bash
./configure
```

### Install into a custom location

```bash
./configure --prefix=/opt/udunits
```

### Install into your home directory

```bash
./configure --prefix=$HOME/.local
```

### Enable debugging

```bash
./configure --enable-debug
```

### Disable the legacy UDUNITS-1 API

```bash
./configure --disable-udunits-1
```

### Build static libraries only

```bash
./configure --disable-shared --enable-static
```

### Build shared libraries only

```bash
./configure --enable-shared --disable-static
```

### Use Clang

```bash
CC=clang ./configure
```

### Use custom include and library paths

```bash
CPPFLAGS="-I/opt/local/include" LDFLAGS="-L/opt/local/lib" ./configure
```

### Cross-compile for ARM

```bash
./configure --host=aarch64-linux-gnu
```

## Complete Option List

This page summarizes the configuration options most commonly used when building UDUNITS-2.

For the complete list supported by your version:

```bash
./configure --help
```

## See Also

- **[Installation: Building from Source](building-from-source.html)**
