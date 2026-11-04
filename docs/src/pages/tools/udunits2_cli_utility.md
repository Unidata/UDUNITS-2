---
title: udunits2 CLI Utility
sidebar: sidebar
toc: true
permalink: udunits2-cli-utility.html
---

The **`udunits2` command-line utility** provides an interactive and non-interactive interface to the UDUNITS-2 library.

It can:

- Parse and validate unit expressions.
- Convert numeric values between compatible units.
- Display the definition of a unit expression.
- Use a specified XML units database.
- Report problems in the units database.

The utility is useful for testing unit expressions, checking conversions, inspecting unit definitions, and verifying a UDUNITS-2 installation.

For a complete description of command-line options, see the **[CLI Reference](cli-reference.html)**.

## Basic Usage

Run `udunits2` without options to start interactive mode:

```bash
udunits2
```

The utility prompts for the unit you have:

```text
You have:
```

and then for the unit you want:

```text
You want:
```

For example:

```text
You have: 1 m
You want: ft

    1 m = 3.28084 ft
    x/ft = 0.3048*x/m
```

The exact conversion expression displayed depends on the units being converted.

If the installation is working correctly, starting `udunits2` displays the `You have:` prompt.

## Non-Interactive Mode

Use `-H` to specify the input value and unit on the command line:

```bash
udunits2 -H "1 m"
```

Use `-W` to specify the desired output unit:

```bash
udunits2 -H "1 m" -W "ft"
```

This performs the conversion without prompting for input:

```text
    1 m = 3.28084 ft
    x/ft = 0.3048*x/m
```

This form is useful in shell scripts and automated validation workflows.

### Examples

Convert a length:

```bash
udunits2 -H "1 m" -W "ft"
```

Convert a speed:

```bash
udunits2 -H "10 m/s" -W "km/h"
```

Convert a temperature:

```bash
udunits2 -H "25 degC" -W "degF"
```

Convert a pressure:

```bash
udunits2 -H "1013.25 hPa" -W "Pa"
```

Convert a reference time:

```bash
udunits2 -H "1 hours since 2024-01-01 00:00:00" \
         -W "seconds since 2024-01-01 00:00:00"
```

Because unit expressions can contain spaces, quote them when passing them as command-line arguments.

## Display a Unit Definition

The `-W` option can also be used with an empty value to request the definition of the unit specified with `-H`.

For example:

```bash
udunits2 -H "N" -W ""
```

The utility prints the definition of the unit rather than performing a conversion.

This is useful for determining how a unit is represented in terms of other units.

For example, a derived unit such as `N` can be displayed in terms of its underlying units.

## Interactive Mode

Interactive mode is useful when exploring unit expressions or performing multiple conversions.

Start the utility:

```bash
udunits2
```

Enter the quantity and unit at the first prompt:

```text
You have: 10 m/s
```

Then enter the desired units:

```text
You want: km/h
```

The utility reports the converted value and the conversion expression.

A blank response at the `You want:` prompt requests the definition of the unit entered at the `You have:` prompt.

## Command-Line Options

The most commonly used options are:

| Option       | Description                                                                                                                        |
| ------------ | ---------------------------------------------------------------------------------------------------------------------------------- |
| `-H <have>`  | Specify the input value and unit. If omitted, the utility prompts for them.                                                        |
| `-W <want>`  | Specify the desired output unit. If omitted, the utility prompts for it. An empty value requests the definition of the input unit. |
| `-A`         | Use ASCII character encoding.                                                                                                      |
| `-L`         | Use ISO-8859-1 (Latin-1) character encoding.                                                                                       |
| `-U`         | Use UTF-8 character encoding.                                                                                                      |
| `-r`         | Reveal problems in the units database.                                                                                             |
| `-h`         | Display command-line help.                                                                                                         |
| `<XML_file>` | Use the specified XML units database instead of the default database.                                                              |

For the complete option descriptions and usage details, see the **[CLI Reference](cli-reference.html)**.

## Character Encoding

The utility supports three character encodings:

- **ASCII** — selected with `-A`
- **ISO-8859-1 (Latin-1)** — selected with `-L`
- **UTF-8** — selected with `-U`

ASCII is the default when no encoding is explicitly selected.

UTF-8 is useful when unit expressions contain Unicode characters supported by UDUNITS-2, such as:

```text
µm
°C
m²
```

For example:

```bash
udunits2 -U -H "1 µm" -W "m"
```

The selected encoding also affects how the XML units database and unit expressions are interpreted.

## Using a Specific XML Database

By default, `udunits2` uses the standard XML units database installed with UDUNITS-2.

A different database can be supplied as the final command-line argument:

```bash
udunits2 -H "1 foo" -W "bar" /path/to/custom.xml
```

This is useful when testing a custom units database or verifying definitions that are not included in the standard database.

For information about the structure and contents of the XML database, see:

- **[User Guide: Units Database](units-database.html)**
- **[Reference: XML Database Reference](xml-database-reference.html)**

For information about the standard database installation location, see **[Reference: Installation Layout](installation-layout.html)**.

## Checking the Units Database

The `-r` option causes the utility to reveal problems encountered while reading the units database:

```bash
udunits2 -r
```

This can be useful when diagnosing a malformed or customized XML database.

For normal use, `-r` is not required.

## Unit Expressions

The `udunits2` utility accepts the same unit expression language provided by the UDUNITS-2 library.

Examples include:

```text
m
m/s
kg m2 s-2
W/m2
km/h
hours since 1970-01-01
```

Unit expressions can use unit names, symbols, prefixes, multiplication, division, exponents, parentheses, and reference-time expressions.

See the **[User Guide: Unit Expression Language](unit-expression-language.html)** for a user-oriented description of the syntax.

For the complete lexical and syntactic specification, see the **[Reference: Unit Expression Language Specification](unit-expression-language-specification.html)**.

## Troubleshooting

These common errors can occur when using the `udunits2` command-line utility.

**Error Message:** `Unknown unit`
: **Cause:** The specified unit name or symbol is not defined in the loaded units database, or the unit expression contains an unrecognized name or symbol.
: **Solution:** Verify the spelling and capitalization of the unit expression and check the **[Units Database](units-database.html)** for the available units.
For example, `foobar` cannot be parsed unless `foobar` is defined in the loaded units database.
{: .symptom}

**Error Message:** `Invalid unit expression syntax`
: **Cause:** The unit expression contains malformed syntax, such as an invalid operator, incomplete exponent, missing operand, or incorrect grouping.
: **Solution:** Check the expression against the syntax described in the **[Unit Expression Language](unit-expression-language.html)** guide.
For the complete lexical and syntactic rules, see the **[Unit Expression Language Specification](unit-expression-language-specification.html)**.
{: .symptom}

**Error Message:** `Units are not convertible`
: **Cause:** The input and output units represent different dimensions and therefore cannot be converted directly.
: **Solution:** Verify that the source and destination units are dimensionally compatible.
For example, `m` and `ft` are compatible length units, while `m` and `s` represent different dimensions and cannot be converted directly.
{: .symptom}

**Error Message:** `Couldn't open units database`
: **Cause:** The XML units database cannot be located or opened.
: **Solution:** Verify that the units database is installed and accessible.
See **[Reference: Installation Layout](installation-layout.html)** for the expected database location and **[Reference: Environment Variables](environment-variables.html)** for information about `UDUNITS2_XML_PATH`.
{: .symptom}

**Error Message:** `Unknown option`
: **Cause:** The command contains an option that is not supported by the installed version of `udunits2`, or the option was entered incorrectly.
: **Solution:** Run `udunits2 -h` to display the options supported by the installed version.
See the **[CLI Reference](cli-reference.html)** for command syntax and option descriptions.
{: .symptom}

**Error Message:** `Usage: udunits2 ...`
: **Cause:** The command-line arguments are incomplete, invalid, or in an unsupported combination.
: **Solution:** Run `udunits2 -h` to display the command syntax and required arguments.
When unit expressions contain spaces, enclose them in quotation marks. For example:
    ```bash
    udunits2 -H "10 m/s" -W "km/h"
    ```

{: .symptom}

**Error Message:** Unit expression contains unexpected characters
: **Cause:** The expression contains characters that are not recognized under the selected character encoding, or the shell has interpreted special characters before passing the expression to `udunits2`.
: **Solution:** Use the appropriate character-encoding option (`-A`, `-L`, or `-U`) and quote expressions containing spaces or shell-special characters.
For example, use `-U` when working with UTF-8 characters such as `µ`, `°`, or Unicode superscripts.
{: .symptom}

**Error Message:** Conversion produces an unexpected result
: **Cause:** The input or output unit expression does not represent the intended unit, or a unit alias, prefix, offset, or database definition has been interpreted differently than expected.
: **Solution:** Check the unit definitions in the **[Units Database](units-database.html)** and verify the unit expressions being supplied to `-H` and `-W`.
If necessary, specify the XML database explicitly to ensure that the intended database is being used.
{: .symptom}

**Error Message:** A custom unit is not recognized
: **Cause:** The custom XML units database is not being loaded, or the unit definition is invalid.
: **Solution:** Verify the custom database and specify its path explicitly when testing it.
For example:
    ```bash
    udunits2 -H "1 foo" -W "bar" /path/to/custom.xml
    ```
Use the `-r` option to reveal problems encountered while reading the database.
See the **[Units Database](units-database.html)** documentation for information about custom databases.
{: .symptom}

## Using `udunits2` in Scripts

The non-interactive form is suitable for shell scripts and validation workflows:

```bash
udunits2 -H "1 m" -W "ft"
```

For example, a script can use the command to verify that a unit expression is recognized and that two units are convertible.

When automating conversions:

* Quote unit expressions containing spaces.
* Use an explicit XML database when a particular database must be tested.
* Select the required character encoding when expressions contain non-ASCII characters.
* Check the command's exit status when using it as part of an automated workflow.

See **[Validation Workflows](validation-workflows.html)** for examples of incorporating UDUNITS-2 checks into automated validation.

## Verification

The `udunits2` utility provides a convenient way to verify a successful installation.

Display the help message:

```bash
udunits2 -h
```

Start interactive mode:

```bash
udunits2
```

Then test a simple conversion:

```text
You have: 1 m
You want: ft
```

You can also perform the same test non-interactively:

```bash
udunits2 -H "1 m" -W "ft"
```

If these commands work, the executable can locate and load the XML units database and successfully use the UDUNITS-2 library.

See the appropriate **[installation guide](installation.html)** for platform-specific installation and troubleshooting information.

## Related Topics

- **[Getting Started: Quick Start](quick-start-guide.html)**
- **[Getting Started: Basic Examples](basic-examples.html)**
- **[User Guide: Unit Expression Language](unit-expression-language.html)**
- **[User Guide: Units Database](units-database.html)**
- **[Tools: Validation & Testing](validation-and-testing.html)**
- **[Reference: CLI Reference](cli-reference.html)**
- **[Reference: Installation Layout](installation-layout.html)**
- **[Reference: Environment Variables](environment-variables.html)**
