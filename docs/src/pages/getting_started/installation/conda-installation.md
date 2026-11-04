---
title: Conda Installation
sidebar: sidebar
toc: true
permalink: conda-installation.html
---

This page describes how to **install UDUNITS-2 using Conda-compatible package managers** such as Conda, Mamba, and Micromamba.

## Supported Platforms

| Platform | Status             | Notes                                                          |
| -------- | ------------------ | -------------------------------------------------------------- |
| Linux    | 🟢 Fully supported | Conda packages available through conda-forge                   |
| macOS    | 🟢 Fully supported | Intel and Apple Silicon packages available through conda-forge |
| Windows  | 🟢 Fully supported | Native Conda packages available through conda-forge            |

## Conda Channels

UDUNITS-2 is distributed through the **[conda-forge](https://anaconda.org/conda-forge)** channel, which provides community-maintained scientific software packages.

When installing UDUNITS-2, always install it from **conda-forge**:

```bash
conda config --add channels conda-forge
conda config --set channel_priority strict
```

{% include note.html content="
Configuring **conda-forge** as the default channel with strict channel priority ensures that all packages in the environment are obtained from a consistent source, reducing the likelihood of dependency conflicts.
"%}

## Install

If desired, create and activate a dedicated Conda environment before installing UDUNITS-2.
Otherwise, install into an existing environment.

```bash
conda create -n udunits-env
conda activate udunits-env
```

Install UDUNITS-2 from the **conda-forge** channel:

```bash
conda install -c conda-forge udunits2
```

This installs UDUNITS-2 and all required dependencies, including the Expat XML parser.
No separate development package is required.

{% capture rmd %}{% includefile pages/shared/verify-installation.inc %}{% endcapture %}
{{ rmd | liquify | markdownify }}

## Installed Components

Within an active Conda environment, UDUNITS-2 installs the following components. See **[Installation Layout](installation-layout.html)** for an overview of the UDUNITS-2 directory structure and installation paths.

| Component | Typical Location |
| --------- | ---------------- |
| Command-line utility | `$CONDA_PREFIX/bin/udunits2`<br>`%CONDA_PREFIX%\Library\bin\udunits2.exe` (Windows) |
| Header file | `$CONDA_PREFIX/include/udunits2.h`<br>`%CONDA_PREFIX%\Library\include\udunits2.h` (Windows) |
| Shared library | `$CONDA_PREFIX/lib/libudunits2.*`<br>`%CONDA_PREFIX%\Library\bin\libudunits2.dll` (Windows) |
| XML units database | `$CONDA_PREFIX/share/udunits/udunits2.xml`<br>`%CONDA_PREFIX%\Library\share\udunits\udunits2.xml` (Windows) |

## Troubleshooting

### Package Installation Issues

These problems occur while installing UDUNITS-2 with Conda.

**Error Message:** `PackagesNotFoundError`
: **Cause:** The `conda-forge` channel is not configured or was not specified during installation.
: **Solution:** Install UDUNITS-2 from the `conda-forge` channel using `conda install -c conda-forge udunits2`, or configure `conda-forge` as a default channel.
{: .symptom}

**Error Message:** `UnsatisfiableError`
: **Cause:** Packages from multiple Conda channels have incompatible dependencies.
: **Solution:** Create a new environment and install all packages from the `conda-forge` channel, or configure Conda to use strict channel priority.
{: .symptom}

### Environment Issues

These problems occur when the Conda environment is not configured correctly or the wrong environment is being used.

**Error Message:** `udunits2: command not found`
: **Cause:** The Conda environment containing UDUNITS-2 is not active.
: **Solution:** Activate the Conda environment before running the `udunits2` command.
{: .symptom}

**Symptom:** System version of `udunits2` is executed instead of the Conda version
: **Cause:** Another UDUNITS-2 installation appears earlier in the system `PATH`.
: **Solution:** Verify which executable is being used with `which udunits2` (Linux/macOS) or `where udunits2` (Windows), then activate the correct Conda environment or adjust your `PATH`.
{: .symptom}

**Symptom:** Conda activate fails
: **Cause:** Conda has not been initialized for the current shell.
: **Solution:** Run `conda init` and restart the shell to activate the environment again.
{: .symptom}

**Symptom:** `udunits2` works in one environment but not another
: **Cause:** UDUNITS-2 is installed in a different Conda environment.
: **Solution:** Verify the active environment with `conda info --envs`, then activate the environment containing UDUNITS-2.
{: .symptom}

### Development Issues

These problems occur when compiling applications against the UDUNITS-2 library.

**Error Message:** `udunits2.h: No such file or directory`
: **Cause:** The compiler is not searching the Conda environment's include directory.
: **Solution:** Add `$CONDA_PREFIX/include` (or `%CONDA_PREFIX%\Library\include` on Windows) to the compiler include path.  See [Compiler & Linker Flags](compiler-linker-flags.html) for information about compiler include paths.
{: .symptom}

**Error Message:** `ld: cannot find -ludunits2`
: **Cause:** The linker cannot locate the UDUNITS-2 library within the Conda environment.
: **Solution:** Remove or correct `UDUNITS2_XML_PATH`, or verify that `udunits2.xml` exists within the active Conda environment. See [Environment Variables](environment-variables.html) for information about configuring this variable.
{: .symptom}

### Runtime Issues

These problems occur after UDUNITS-2 has been installed successfully.

**Error Message:** `Couldn't open units database`
: **Cause:** The XML units database cannot be located or an incorrect `UDUNITS2_XML_PATH` environment variable overrides the Conda installation.
: **Solution:** Remove or correct the `UDUNITS2_XML_PATH` environment variable, or verify that `udunits2.xml` exists within the active Conda environment.
{: .symptom}
