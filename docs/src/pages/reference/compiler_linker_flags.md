---
title: Compiler & Linker Flags
sidebar: sidebar
toc: true
permalink: compiler-linker-flags.html
---

This page provides a reference for compiler, preprocessor, and linker flags commonly used when building software with the UDUNITS-2 C library.

For instructions on compiling and linking an application, see [Building Applications](building-applications.html).
For instructions on building UDUNITS-2 itself, see [Building from Source](building-from-source.html).

## Quick Reference

The following options are the most commonly used when building an application against UDUNITS-2:

| Option               | Purpose                                              |
| -------------------- | ---------------------------------------------------- |
| `-I/path/to/include` | Add a directory to the compiler's header search path |
| `-L/path/to/lib`     | Add a directory to the linker's library search path  |
| `-ludunits2`         | Link against the UDUNITS-2 library                   |
| `CC`                 | Select the C compiler                                |
| `CFLAGS`             | Specify C compiler options                           |
| `CPPFLAGS`           | Specify C preprocessor options                       |
| `LDFLAGS`            | Specify linker options                               |
| `LIBS`               | Specify additional libraries                         |

For a standard installation, these options are usually unnecessary:

```bash
cc -o example example.c -ludunits2
```

For a non-standard installation:

```bash
cc -I/opt/udunits/include \
   -L/opt/udunits/lib \
   -o example example.c \
   -ludunits2
```

## Compiler and Preprocessor Flags

### `CC`

`CC` selects the C compiler used by Autotools.

For example:

```bash
CC=clang ./configure
```

The UDUNITS-2 development workflow uses both `cc` and `clang` for build checks.

When compiling an application directly, the compiler can instead be specified by invoking it explicitly:

```bash
clang -o example example.c -ludunits2
```

### `CFLAGS`

`CFLAGS` specifies options passed to the C compiler.

Common uses include selecting optimization levels, enabling debugging information, and enabling compiler warnings.

For example:

```bash
CFLAGS="-O2" ./configure
```

For a debugging build of UDUNITS-2, the Autotools configuration uses:

```text
-g -O0
```

when `--enable-debug` is specified.

`CFLAGS` can also be used when compiling application source files directly:

```bash
cc -O2 -c example.c
```

### `CPPFLAGS`

`CPPFLAGS` specifies options passed to the C preprocessor.

It is commonly used to specify additional header search directories or preprocessor definitions.

For example:

```bash
CPPFLAGS="-I/opt/local/include" ./configure
```

When compiling an application directly, the equivalent include-path option is:

```bash
cc -I/opt/local/include -c example.c
```

The UDUNITS-2 Autotools build uses `-DNDEBUG` by default when `CPPFLAGS` has not been explicitly set.

### `-I`

The `-I` option adds a directory to the compiler's header search path.

Use it when `udunits2.h` is installed outside the standard system include directories.

For example:

```bash
cc -I/opt/udunits/include -c example.c
```

If the header is installed at:

```text
/opt/udunits/include/udunits2.h
```

the corresponding include option is:

```text
-I/opt/udunits/include
```

An error such as:

```text
udunits2.h: No such file or directory
```

usually indicates that the compiler cannot find the header.

## Linker Flags

### `LDFLAGS`

`LDFLAGS` specifies options passed to the linker.

It is commonly used to specify library search directories.

For example:

```bash
LDFLAGS="-L/opt/udunits/lib" ./configure
```

When compiling an application directly, the equivalent option is:

```bash
cc -L/opt/udunits/lib -o example example.c -ludunits2
```

### `-L`

The `-L` option adds a directory to the linker's library search path.

For example:

```bash
cc -L/opt/udunits/lib -o example example.c -ludunits2
```

If the UDUNITS-2 library is installed at:

```text
/opt/udunits/lib/libudunits2.so
```

the corresponding library search option is:

```text
-L/opt/udunits/lib
```

An error such as:

```text
ld: cannot find -ludunits2
```

usually indicates that the linker cannot find the library.

### `-ludunits2`

The `-ludunits2` option links an application against the UDUNITS-2 library.

For example:

```bash
cc -o example example.c -ludunits2
```

The library option should follow the source or object files that use UDUNITS-2 functions:

```bash
cc -o example example.o -ludunits2
```

This ordering is important on platforms and linkers that process libraries from left to right.

### `LIBS`

`LIBS` specifies additional libraries to be passed to the linker.

For example:

```bash
LIBS="-lm" ./configure
```

`LIBS` is primarily useful when configuring software with Autotools.

Applications normally do not need to specify UDUNITS-2's own dependencies separately when linking dynamically against a correctly installed UDUNITS-2 library.

## Autotools

When using Autotools, compiler and linker settings can be supplied as environment variables when running `configure`.

For example:

```bash
CC=clang \
CFLAGS="-O2" \
CPPFLAGS="-I/opt/udunits/include" \
LDFLAGS="-L/opt/udunits/lib" \
./configure
```

The principal variables are:

| Variable   | Purpose                |
| ---------- | ---------------------- |
| `CC`       | C compiler             |
| `CFLAGS`   | C compiler options     |
| `CPPFLAGS` | C preprocessor options |
| `LDFLAGS`  | Linker options         |
| `LIBS`     | Additional libraries   |

These variables are also used by the UDUNITS-2 Autotools build.
The project's configure options provide additional controls for debugging, optimization, shared and static libraries, and installation locations.
See [Configure Script Options](configure-options.html).

## CMake

CMake uses its own variables for compiler and build configuration.

The most common equivalent to selecting a compiler is:

```bash
cmake -B build \
    -DCMAKE_C_COMPILER=clang \
    -S .
```

A build type can be selected with `CMAKE_BUILD_TYPE`:

```bash
cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -S .
```

For a debugging build:

```bash
cmake -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -S .
```

The UDUNITS-2 CMake build requires C99.

## Common Flag Combinations

### Standard Installation

When UDUNITS-2 is installed in a standard system location:

```bash
cc -o example example.c -ludunits2
```

### Non-Standard Installation

When UDUNITS-2 is installed under `/opt/udunits`:

```bash
cc -I/opt/udunits/include \
   -L/opt/udunits/lib \
   -o example example.c \
   -ludunits2
```

### Debug Compilation

To compile an application with debugging information and no optimization:

```bash
cc -g -O0 -o example example.c -ludunits2
```

### Optimization

To compile an application with optimization enabled:

```bash
cc -O2 -o example example.c -ludunits2
```

The appropriate optimization level depends on the compiler and application.
The UDUNITS-2 Autotools build uses its configured defaults unless the user supplies `CFLAGS`.

## Flag Ordering

A typical compilation and link command places source or object files before libraries:

```bash
cc -o example example.o -ludunits2
```

For a non-standard installation:

```bash
cc -I/opt/udunits/include \
   -L/opt/udunits/lib \
   -o example example.o \
   -ludunits2
```

The include option affects compilation, the library search option affects library discovery, and the `-ludunits2` option specifies the library to link.

## See Also

- **[Developer Guide: Building Applications](building-applications.html)**
- **[Getting Started: Building from Source](building-from-source.html)**
- **[Developer Guide: Developing UDUNITS-2](developing-udunits.html)**
- **[Reference: Configure Script Options](configure-options.html)**
- **[Reference: Environment Variables](environment-variables.html)**
