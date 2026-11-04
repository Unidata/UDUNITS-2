---
title: Building Applications
sidebar: sidebar
toc: true
permalink: building-applications.html
---

This page describes how to build an application that uses the UDUNITS-2 C library.

{% include note.html content="
This assumes you have UDUNITS-2 installed.
If you need to build UDUNITS-2 itself from source, see **[Building from Source](building-from-source.html)**.
"%}

## Development Files

An application that uses the UDUNITS-2 library requires the development files, including:

- The `udunits2.h` header file
- The `libudunits2` library
- The UDUNITS-2 XML units database at runtime

System package managers typically provide these through a development package
For example, Debian and Ubuntu use `libudunits2-dev`, while Fedora, RHEL, Rocky Linux, and AlmaLinux use `udunits2-devel`.

A source installation typically places these files under the installation prefix:

```text
<prefix>/include/udunits2.h
<prefix>/lib/libudunits2.*
<prefix>/share/udunits/udunits2.xml
```

## Include the UDUNITS-2 Header

Include the public header in source files that use the UDUNITS-2 C API:

```c
#include <udunits2.h>
```

The compiler must be able to find the directory containing `udunits2.h`.

For a standard installation, the compiler normally searches the system include directories automatically.
If UDUNITS-2 is installed in a non-standard location, specify the include directory with `-I`:

```bash
cc -I/path/to/udunits/include -c example.c
```

## Link Against the UDUNITS-2 Library

Applications must link against `libudunits2`.

For example:

```bash
cc -I/path/to/udunits/include \
   -L/path/to/udunits/lib \
   -o example example.c \
   -ludunits2
```

The library option should follow the source or object files that use UDUNITS-2 functions.

If the linker reports:

```text
ld: cannot find -ludunits2
```

verify the library installation location and add the appropriate directory with `-L`.
An error such as:

```text
undefined reference to ut_*
```

usually indicates that the application was not linked against `libudunits2`.

For additional compiler and linker options, see **[Compiler & Linker Flags](compiler-linker-flags.html)**.

## Runtime Library

When an application uses a shared UDUNITS-2 library, the operating system must be able to locate the library when the application starts.

On Linux, for example, use:

```bash
ldd example
```

to inspect the libraries required by the application.

If the runtime loader cannot locate `libudunits2.so`, configure the system's library search path or use the appropriate platform-specific mechanism.
On Linux, this can include configuring the system library cache with `ldconfig` or using `LD_LIBRARY_PATH`.

## Units Database

The UDUNITS-2 library also requires access to its XML units database.

The standard database is normally installed at:

```text
<prefix>/share/udunits/udunits2.xml
```

If the database cannot be found, an application may report:

```text
Couldn't open units database
```

Verify that `udunits2.xml` is installed and accessible.
If necessary, use the `UDUNITS2_XML_PATH` environment variable to specify the database location.

See **[Units Database](units-database.html)** for information about the database and **[Environment Variables](environment-variables.html)** for information about `UDUNITS2_XML_PATH`.

## Minimal Application

A simple application can create a unit system, parse units, create a converter, and perform a conversion:

```c
#include <stdio.h>
#include <udunits2.h>

int main(void)
{
    ut_system* sys = ut_read_xml(NULL);
    ut_unit* meter;
    ut_unit* foot;
    cv_converter* cv;
    double result;

    meter = ut_parse(sys, "m", UT_UTF8);
    foot = ut_parse(sys, "ft", UT_UTF8);

    if (meter == NULL || foot == NULL) {
        return 1;
    }

    cv = ut_get_converter(meter, foot);

    if (cv == NULL) {
        return 1;
    }

    result = cv_convert_double(cv, 1.0);
    printf("1 m = %g ft\n", result);

    cv_free(cv);
    ut_free(meter);
    ut_free(foot);
    ut_free_system(sys);

    return 0;
}
```

Compile and link the application using the compiler and library paths appropriate for your installation:

```bash
cc -o example example.c -ludunits2
```

This follows the basic C library workflow demonstrated in the **[Basic Examples](basic-examples.html)** page.

## Troubleshooting

### Header Not Found

This problem occurs when the compiler cannot locate the UDUNITS-2 header file.

**Error Message:** `udunits2.h: No such file or directory`

: **Cause:** The compiler cannot find the UDUNITS-2 header, or the development package is not installed.
: **Solution:** Install the appropriate development package and verify the include path.
If UDUNITS-2 is installed in a non-standard location, add its include directory with `-I/path/to/include`.
{: .symptom}

### Library Not Found

This problem occurs when the linker cannot locate the UDUNITS-2 library.

**Error Message:** `ld: cannot find -ludunits2`

: **Cause:** The linker cannot find the UDUNITS-2 library.
: **Solution:** Verify the library installation location and add its directory to the linker search path with `-L/path/to/lib`.
{: .symptom}

### Undefined UDUNITS-2 References

This problem occurs when the linker cannot resolve references to UDUNITS-2 functions.

**Error Message:** `undefined reference to ut_*`

: **Cause:** The application is not linked against `libudunits2`.
: **Solution:** Add `-ludunits2` to the link command, after the source or object files that use the library.
{: .symptom}

### Runtime Library Not Found

This problem occurs when the operating system cannot locate the UDUNITS-2 shared library when the application starts.

**Symptom:** The application cannot locate `libudunits2.so` at runtime.

: **Cause:** The dynamic linker cannot find the installed UDUNITS-2 shared library.
: **Solution:** Use `ldd` to inspect the application's library dependencies, then configure the system library path or `LD_LIBRARY_PATH` as appropriate.
{: .symptom}

### Units Database Not Found

This problem occurs when the application cannot locate or access the UDUNITS-2 XML units database.

**Error Message:** `Couldn't open units database`

: **Cause:** The XML units database cannot be found or the `UDUNITS2_XML_PATH` environment variable points to an incorrect location.
: **Solution:** Verify the location of `udunits2.xml` and correct `UDUNITS2_XML_PATH` if necessary.
{: .symptom}

### Multiple Installations

This problem occurs when multiple UDUNITS-2 installations cause the application to use an unintended version.

**Symptom:** The application uses an unexpected version of UDUNITS-2.

: **Cause:** Multiple UDUNITS-2 installations are present, and the compiler, linker, or runtime loader is finding a different installation than intended.
: **Solution:** Check the include and library search paths and use `ldd` to determine which library is loaded at runtime.
Adjust the paths to reference the intended installation.
{: .symptom}

## See Also

- **[Getting Started: Building from Source](building-from-source.html)**
- **[Developer Guide: Using the C API](using-the-c-api.html)**
- **[Getting Started: Basic Examples](basic-examples.html)**
- **[Reference: Compiler & Linker Flags](compiler-linker-flags.html)**
- **[Reference: Environment Variables](environment-variables.html)**
