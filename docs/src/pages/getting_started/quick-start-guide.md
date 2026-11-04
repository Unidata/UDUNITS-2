---
title: Quick Start Guide
sidebar: sidebar
toc: true
permalink: quick-start-guide.html
---

This guide demonstrates the basic workflow for using UDUNITS-2.

If you have already installed UDUNITS-2, you can use the examples below to verify your installation, perform a few unit conversions, and become familiar with the **[unit expression language](unit-expression-language.html)**.

## Verify Your Installation

Start the `udunits2` command-line utility:

```bash
udunits2
```

If the installation is successful, you should see the prompt:

```text
You have:
```

If the command is not found or the program does not start, refer to the appropriate installation guide for your platform.

## Convert Units

At the `You have:` prompt, enter a quantity and unit. At the `You want:` prompt, enter the desired output units.

### Length

```text
You have: 1 m
You want: ft

    x/0.3048
```

### Distance

```text
You have: 100 km
You want: mi
```

### Temperature

```text
You have: 25 degC
You want: degF
```

## Unit Expressions

UDUNITS-2 understands both simple and compound unit expressions.

```text
m/s
kg m2 s-2
W/m2
hours since 1970-01-01
```

See the **[Unit Expression Language](unit-expression-language.html)** guide for an overview of the syntax and additional examples.
For the complete formal specification, see the [**Unit Expression Language Specification**](unit-expression-language-specification.html).

## Next Steps

- Continue to **[Basic Examples](basic-examples.html)** for more command-line and C programming examples.
- Learn the **[Unit Expression Language](unit-expression-language.html)** to understand supported unit syntax.
- Explore the **[Units Database](units-database.html)** to customize or extend available unit definitions.
- Learn how to use the **[C API](using-the-c-api.html)** to integrate UDUNITS-2 into your applications.
