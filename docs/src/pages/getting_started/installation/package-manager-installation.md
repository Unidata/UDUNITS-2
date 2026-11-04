---
title: Package Manager Installation
sidebar: sidebar
toc: true
permalink: package-manager-installation.html
---
This page describes how to **install UDUNITS-2 using system package managers** provided by Linux distributions and macOS package ecosystems.

## Supported Platforms

| Platform | Status             | Notes                                                                                                                                    |
| -------- | ------------------ | ---------------------------------------------------------------------------------------------------------------------------------------- |
| Linux    | 🟢 Fully supported | Available through distribution package managers                                                                                          |
| macOS    | 🟢 Fully supported | Available through Homebrew and MacPorts                                                                                                  |
| Windows  | 🔴 Not supported   | Use the **[Conda Installation](conda-installation.html)** or **[Building from Source](building-from-source.html)** instructions instead. |

## Install

Most Linux distributions separate UDUNITS-2 into **runtime** and **development** packages.

- Install the **runtime package** if you only need to use the `udunits2` command-line utility or run software that depends on UDUNITS-2.
- Install the **development package** if you are compiling software that links against the UDUNITS-2 library.

Use the appropriate command for your platform:

| Platform | Installation Command |
| -------- | -------------------- |
| Ubuntu / Debian | `sudo apt install libudunits2-dev` |
| Fedora | `sudo dnf install udunits2-devel` |
| RHEL / Rocky / AlmaLinux | `sudo dnf install udunits2-devel` |
| openSUSE | `sudo zypper install udunits2-devel` |
| Arch Linux | `sudo pacman -S udunits` |
| macOS (Homebrew) | `brew install udunits` |
| macOS (MacPorts) | `sudo port install udunits2` |

Development packages include the runtime components along with the headers and libraries needed to build software against UDUNITS-2.

{% capture rmd %}{% includefile pages/shared/verify-installation.inc %}{% endcapture %}
{{ rmd | liquify | markdownify }}

## Installed Components

A package manager installation typically installs the following components. See **[Installation Layout](installation-layout.html)** for an overview of the standard UDUNITS-2 filesystem layout and how installation methods differ.

| Component | Typical Location |
| --------- | ---------------- |
| Command-line utility | System executable directory (for example, `/usr/bin` or `/usr/local/bin`) |
| Header file | System include directory (development packages only) |
| Shared library | System library directory |
| XML units database | System shared data directory (for example, `/usr/share/udunits`) |

The exact installation locations depend on your operating system, distribution, and package manager.

## Troubleshooting

### Package Installation Issues

These issues are related to installing UDUNITS-2 using a system package manager.

**Error Message:** `Unable to locate package` or `No package matching 'udunits2'`
: **Cause:** The repository providing UDUNITS-2 is not enabled, or the package name differs for your distribution.
: **Solution:** Search for available UDUNITS packages using your package manager (for example, `apt search udunits`, `dnf search udunits`, or `zypper search udunits`)
Enable the appropriate repository if necessary (such as EPEL on older RHEL-based systems), then install the package appropriate for your distribution.
{: .symptom}

**Symptom:** The installed package is older than the current UDUNITS-2 release
: **Cause:** The distribution repositories provide an older version of UDUNITS-2 than the current upstream release.
: **Solution:** Check the package version available for your distribution.
If you require newer features, bug fixes, or the latest release, consider installing UDUNITS-2 using Conda or building it from source.
{: .symptom}

### Environment Issues

These problems occur after installation when multiple UDUNITS-2 installations exist or the wrong executable is being used.

**Symptom:** An unexpected version of `udunits2` is executed
: **Cause:** Multiple UDUNITS-2 installations are present (for example, a package manager installation, a manually built version, or a Homebrew installation), and a different version appears earlier in the system `PATH`.
: **Solution:** Verify which executable is being used with `which udunits2` (Linux/macOS), then remove duplicate installations or adjust your `PATH` so the intended installation is used.
{: .symptom}

### Development Issues

These problems occur when compiling applications against a package manager installation of UDUNITS-2.

**Error Message:** `udunits2.h: No such file or directory`
: **Cause:** The UDUNITS-2 development package is not installed, or the compiler is not searching the system include directories.
: **Solution:** Install the appropriate development package for your distribution (for example, `libudunits2-dev` or `udunits2-devel`), then verify the compiler include search path. See [Compiler & Linker Flags](compiler-linker-flags.html) for information about specifying non-standard include directories.
{: .symptom}

**Error Message:** `ld: cannot find -ludunits2`
: **Cause:** The UDUNITS-2 development package is not installed, or the linker cannot locate the shared library.
: **Solution:** Install the appropriate development package, then verify that the library directory is included in the linker's search path. See [Compiler & Linker Flags](compiler-linker-flags.html) for information about specifying non-standard include directories.
{: .symptom}

### Runtime Issues

These problems occur after UDUNITS-2 has been installed successfully.

**Error Message:** `Couldn't open units database`
: **Cause:** The XML units database cannot be located, or the `UDUNITS2_XML_PATH` environment variable points to an incorrect location.
: **Solution:** Verify that the XML database is installed by your package manager, then remove or correct `UDUNITS2_XML_PATH` if necessary. See [Environment Variables](environment-variables.html) for information about configuring this variable.
{: .symptom}

### Platform-Specific Issues

These problems occur when using Homebrew-installed packages on macOS.

**Symptom:** Compiler cannot locate Homebrew-installed UDUNITS-2 headers or libraries
: **Cause:** On Apple Silicon systems, Homebrew installs packages under `/opt/homebrew` rather than `/usr/local`.
: **Solution:** Run `brew --prefix udunits` to determine the installation location, then use the reported prefix when configuring compiler include and library search paths.
{: .symptom}
