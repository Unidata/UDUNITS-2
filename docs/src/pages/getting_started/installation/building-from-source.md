---
title: Building from Source
sidebar: sidebar
toc: true
permalink: building-from-source.html
---

This page describes how to **build and install UDUNITS-2 from source code** for C and C-based applications.

## Supported Build Platforms

UDUNITS-2 can be built from source on:

| Platform | Status | Notes |
| ------ | ------ | ----- |
| Linux (POSIX) | 🟢 Fully supported | Primary development and most common build target |
| macOS | 🟢 Fully supported | Can be built using standard macOS developer tools (Homebrew-installed dependencies) |
| Windows | 🟡 Supported | Build using the provided CMake build system. POSIX-like environments (such as MSYS2 or Cygwin) may also be used with the Autotools build |

## Prerequisites

| Dependency / Tool                                       | Required        | Purpose                                                      |
| ------------------------------------------------------- | --------------- | ------------------------------------------------------------ |
| C compiler (C99+)                                       | **Yes**         | Compiles the UDUNITS-2 library and utilities                 |
| Expat XML parser                                        | **Yes**         | Parses the `udunits2.xml` units database                     |
| CNU Make                                                | **Yes**         | Builds, tests, and installs the software                     |
| Texinfo (`makeinfo`)                                    | **Conditional** | Generates documentation when building from a Git checkout    |
| GNU Autotools (`autoconf`, `automake`, `libtool`, `m4`) | **Conditional** | Generates the build system when building from a Git checkout |
| `pkg-config`                                            | No              | Helps other applications locate and link against UDUNITS-2   |

## Obtaining The Source Code

Obtain the latest version of UDUNITS-2 from GitHub:

```bash
git clone https://github.com/Unidata/UDUNITS-2.git
cd UDUNITS-2
```

## Configure

Configure the build using the standard GNU configure script.

```bash
./configure
```

To install into a custom location:

```bash
./configure --prefix=/opt/udunits
```

Additional build and installation options are documented in the **[Configure Options](configure-options.html)** reference.
For compiler and linker flags used to locate headers and libraries, see the **[Compiler & Linker Flags](compiler-linker-flags.html)** reference.
For environment variables that affect the build and runtime environment, see the **[Environment Variables](environment-variables.html)** reference.

Common examples:

```bash
./configure --enable-debug
./configure --disable-udunits-1
```

## Build

Compile the library and utilities:

```bash
make
```

Optionally run the test suite:

```bash
make check
```

## Install

Install the library, command-line utility, headers, and XML database:

```bash
sudo make install
```

If installing into a user-owned location:

```bash
make install
```

{% capture rmd %}{% includefile pages/shared/verify-installation.inc %}{% endcapture %}
{{ rmd | liquify | markdownify }}

If the library was installed into a non-standard location, ensure your runtime library path and include paths are configured appropriately.

## Installed Components

By default, `make install` installs the following components under the installation prefix (typically `/usr/local`).  See **[Installation Layout](installation-layout.html)** for a reference to the directory structure and installed file locations.

| Component | Typical Location |
| --------- | ---------------- |
| Command-line utility | `<prefix>/bin/udunits2` |
| Header file | `<prefix>/include/udunits2.h` |
| Shared library | `<prefix>/lib/libudunits2.*` |
| XML units database | `<prefix>/share/udunits/udunits2.xml` |

## Troubleshooting

### Prerequisite And Dependency Issues

These problems occur when required build tools or libraries are missing or cannot be found during configuration.

**Error Message:** `configure: cannot find EXPAT function XML_StopParser`
: **Cause:** Expat library missing or not detected.
: **Solution:** Inspect `config.log` to confirm that Expat is missing, then install the appropriate Expat development package.
{: .symptom}

**Error Message:** `expat.h: No such file or directory`
: **Cause:** Expat headers are not installed.
: **Solution:** Verify that the Expat header files are installed (or search your system include directories), then install the Expat development headers if necessary.
{: .symptom}

**Error Message:** `ld: cannot find -lexpat`
: **Cause:** The linker cannot locate the Expat library.
: **Solution:** Verify the library search paths, then install the Expat library or add the appropriate library directory (for example, `-L/path/to/expat/lib`) to the linker flags.
{: .symptom}

### Build Tools And Configuration Issues

These problems occur during the `configure` or CMake stage when required build tools or non-standard paths are not available.

**Symptom:** `configure` fails to detect a required dependency
: **Cause:** A required header or library is installed in a non-standard location, or the compiler and linker search paths do not include its location.
: **Solution:** : **Solution:** Inspect `config.log` to identify the missing dependency, then use `CPPFLAGS` to specify additional include directories and `LDFLAGS` to specify additional library directories.
See [Compiler & Linker Flags](compiler-linker-flags.html) for details about compiler and linker flags, and [Environment Variables](environment-variables.html) for information about the environment variables that configure the build and runtime environment.
{: .symptom}

**Symptom:** `configure` script is missing
: **Cause:** The source was obtained from a Git checkout before the build system was generated, or the generated `configure` script is missing.
: **Solution:** Install GNU Autotools (Autoconf, Automake, Libtool, and M4) and regenerate the build system, or build from an official release tarball, which already contains the generated `configure` script.
{: .symptom}

**Error Message:** `makeinfo: command not found`
: **Cause:** Texinfo is not installed.
: **Solution:** Install Texinfo if you are rebuilding the documentation from a Git checkout.
Official release tarballs already include generated documentation, so `makeinfo` is typically not required.
{: .symptom}

**Error Message:** `udunits2.h: No such file or directory`
: **Cause:** The UDUNITS-2 header files are installed in a non-standard location.
: **Solution:** Locate the directory containing `udunits2.h`, then add it to the compiler's include search path using `-I/path/to/include`.
{: .symptom}

### Compilation And Linking Issues

These errors occur when compiling or linking applications against UDUNITS.

**Error Message:** `ld: cannot find -ludunits2`
: **Cause:** The linker cannot locate the UDUNITS-2 library because its installation directory is not in the library search path.
: **Solution:** Verify the library installation location, then add the appropriate library directory (for example, `-L/path/to/lib`) to the linker flags.
{: .symptom}

**Error Message:** `undefined reference to ut_*` functions
: **Cause:** The application is not being linked against the UDUNITS-2 library.
: **Solution:** Verify the link command and ensure that `-ludunits2` is included, preferably after the object files and source files on the command line.
{: .symptom}

### Installation Issues

These problems occur while installing UDUNITS-2 after the build has completed successfully.

**Symptom:** `sudo make install` fails with `Permission denied`
: **Cause:** The installation prefix requires administrator privileges.
: **Solution:** Re-run `sudo make install`, or configure a user-writable installation prefix using `./configure --prefix=<directory>`.
{: .symptom}

**Symptom:** Newly installed version is not the one being executed
: **Cause:** An older installation appears earlier in the system `PATH`.
: **Solution:** Use `which udunits2` to determine which executable is being run, then adjust your `PATH` or remove the older installation.
{: .symptom}

### Environment Issues

These issues occur when multiple installations or versions of UDUNITS-2 exist on the same system.

**Symptom:** An application links against the wrong UDUNITS-2 library
: **Cause:** Multiple UDUNITS-2 installations are present, causing the compiler or linker to use an unintended version.
: **Solution:** Use `ldd` to determine which UDUNITS-2 library is being loaded, then remove duplicate installations or adjust your include and library search paths to reference the desired version.
{: .symptom}

### Runtime Issues

These problems occur when applications are executed after a successful installation.

**Symptom:** Runtime loader cannot locate `libudunits2.so`
: **Cause:** The dynamic linker cannot locate the UDUNITS-2 shared library because its installation directory is not registered.
: **Solution:** Run `ldd program` to verify that `libudunits2.so` cannot be found, then run `ldconfig` or update `LD_LIBRARY_PATH` to include the library directory.
{: .symptom}

**Error Message:** `Couldn't open units database`
: **Cause:** The UDUNITS-2 XML units database cannot be found.
: **Solution:** Locate `udunits2.xml` in your installation, then see [Environment Variables](environment-variables.html) for information about setting `UDUNITS2_XML_PATH` to the database's full pathname.
{: .symptom}

### Windows-Specific Issues

These problems occur when building UDUNITS-2 using **Visual Studio or Windows toolchains**.

**Symptom:** Visual Studio cannot locate `udunits2.h`
: **Cause:** The UDUNITS-2 include directory is not configured in the project's include search paths.
: **Solution:** Verify the project's include directory settings, then add the directory containing `udunits2.h` to the project's additional include directories.
{: .symptom}

**Symptom:** Visual Studio cannot locate `udunits2.lib`
: **Cause:** The linker cannot locate the UDUNITS-2 import library.
: **Solution:** Verify the linker settings, then add the directory containing `udunits2.lib` to the library search paths and add `udunits2.lib` to the project's linker dependencies.
{: .symptom}

**Symptom:** Windows cannot locate `libudunits2.dll`
: **Cause:** The UDUNITS-2 DLL is not in the Windows DLL search path.
: **Solution:** Verify that `libudunits2.dll` is accessible at runtime, then either add its directory to the `PATH` environment variable or place the DLL in the same directory as the executable.
{: .symptom}
