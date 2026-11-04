---
title: Unit Expression Language Specification
sidebar: sidebar
toc: true
permalink: unit-expression-language-specification.html
---
This page provides the complete lexical and syntactic reference for the unit expression language understood by UDUNITS-2.

The grammar definitions describe how tokens combine to form unit expressions.
The lexical definitions describe how individual tokens, such as identifiers, numbers, operators, dates, and times, are formed.

For a user-oriented introduction to unit expressions, see [Basic Syntax](unit-expression-syntax.html).
For a condensed overview of the formal grammar, see [Grammar](unit-expression-grammar.html).

## Grammar Conventions

The grammar uses the following conventions:

- *Italic names* represent **non-terminals**.
- `UPPERCASE` names represent **terminals**.
- `<angle-bracketed names>` represent **lexical elements**.

The productions use the form:

```text
<non-terminal>: one of
        alternative
        alternative
```

Comments following `//` describe individual lexical values or implementation behavior.

## Main Grammar Structure

### Unit Specification

```text
<Unit-Spec>: one of
        nothing
        <Shift-Spec>
```

### Shift Specification

```text
<Shift-Spec>: one of
        <Product-Spec>
        <Product-Spec> SHIFT REAL
        <Product-Spec> SHIFT INT
        <Product-Spec> SHIFT <Timestamp>
```

A shift specification consists of a product specification with an optional shift.
Shifts are used for unit offsets and reference-time expressions.

### Product Specification

```text
<Product-Spec>: one of
        <Power-Spec>
        <Product-Spec> <Power-Spec>
        <Product-Spec> MULTIPLY <Power-Spec>
        <Product-Spec> DIVIDE <Power-Spec>
```

A product specification combines power specifications through adjacency, multiplication, or division.

### Power Specification

```text
<Power-Spec>: one of
        <Basic-Spec>
        <Basic-Spec> INT
        <Basic-Spec> EXPONENT
```

A power specification consists of a basic specification, optionally followed by an exponent.

### Basic Specification

```text
<Basic-Spec>: one of
        ID
        "(" <Shift-Spec> ")"
        LOGREF <Product-Spec> ")"
        <Number>
```

A basic specification can be a unit identifier, a parenthesized expression, a logarithmic expression, or a number.

The `LOGREF` token includes the logarithm function and reference marker, but does not include the closing parenthesis.
The closing parenthesis is part of the enclosing `Basic-Spec` production.

## Lexical Building Blocks

### Whitespace

```text
<space>: one of
        U+0020      // space
        U+0009      // horizontal tab
        U+000D      // carriage return
        U+000C      // form feed
        U+000B      // vertical tab
```

These characters represent space, horizontal tab, carriage return, form feed, and vertical tab.

Newline (`U+000A`) is not part of `<space>`.

Non-breaking space (`U+00A0`) is also not whitespace, although it is permitted as part of an identifier.

### Digits

```text
<digit>:
        [0-9]
```

### Alphabetic Characters

```text
<alpha>: one of
        [A-Za-z_]
        <iso-8859-1-alpha>
```

The supported Latin-1 characters are:

```text
<iso-8859-1-alpha>: one of
        U+00A0          // non-breaking space
        U+00AD          // soft hyphen
        U+00B0          // degree sign
        U+00B5          // micro sign
        U+00C0-U+00D6   // À-Ö, excluding U+00D7
        U+00D8-U+00F6   // Ø-ö, excluding U+00F7
        U+00F8-U+00FF   // ø-ÿ
```

These ranges include characters such as the degree sign, micro sign, and Latin letters with diacritics.

```text
<alphanum>: one of
        <alpha>
        <digit>
```

## Numbers

```text
<Number>: one of
        INT
        REAL
```

### Integers

```text
INT:
        [+-]? <digit>+
```

An integer consists of an optional sign followed by one or more digits.

Examples:

```text
5
-5
42
```

### Real Numbers

```text
REAL:
        [+-]? <real-value>
```

```text
<real-value>: one of
        <digit>+ <exponent>
        <mantissa> <exponent>?
```

Examples include:

```text
5e10
3E-2
3.14
.5
2.
3.14e-10
```

### Mantissas

```text
<mantissa>: one of
        <digit>+ "."
        "." <digit>+
        <digit>+ "." <digit>+
```

### Exponents

```text
<exponent>:
        ("e" | "E") [+-]? <digit>+
```

`NaN` and `Inf`/`Infinity` are not valid numeric values.

## Identifiers

Unit names and symbols are represented by `ID`.

```text
ID: one of
        <id>
        "%"
        "'"
        "\""
        U+00B0
        U+00B5
```

The general identifier form is:

```text
<id>: one of
        <alpha>
        <alpha> <alphanum>* <alpha>
```

A multi-character identifier must end with an `<alpha>` character rather than a digit.

Examples include:

```text
m
meter
second
degC
µm
```

The complete set of supported identifier characters is defined by `<alpha>` and `<alphanum>` above.

## Operators

### Shift

```text
SHIFT:
        <space>* <shift-op> <space>*
```

```text
<shift-op>: one of
        "@"
        "after"
        "from"
        "since"
        "ref"
```

Shift operators are used for unit offsets and reference-time expressions.

For example:

```text
m @ 10
days since 1970-01-01
```

The user-oriented discussion of reference-time expressions is provided in [Reference Time Units](unit-expression-reference-time-units.html).

### Multiplication

```text
MULTIPLY: one of
        "-"
        "."
        "*"
        <space>+
        U+00B7
```

The supported multiplication forms are:

| Syntax     | Description               |
| ---------- | ------------------------- |
| `-`        | Hyphen                    |
| `.`        | Period                    |
| `*`        | Asterisk                  |
| whitespace | One or more spaces        |
| `U+00B7`   | Centered middle dot (`·`) |

Examples:

```text
kg m
kg*m
kg.m
kg-m
kg·m
```

### Division

```text
DIVIDE:
        <space>* <divide-op> <space>*
```

```text
<divide-op>: one of
        " per "
        " PER "
        "/"
```

The `per` forms require surrounding spaces.

Examples:

```text
m/s
m per s
m PER s
```

### Exponents

```text
EXPONENT: one of
        <raise-with-int>
        <utf8-exponent>
```

An exponent can use an ASCII operator followed by an integer or UTF-8 superscript characters.

```text
<raise-with-int>:
        ("^" | "**") INT
```

Examples:

```text
m^2
m**2
m^-2
```

UTF-8 superscript exponents use:

```text
<utf8-exponent>:
        <utf8-exp-sign>? <utf8-exp-digit>+
```

```text
<utf8-exp-sign>: one of
        U+207A      // superscript plus sign
        U+207B      // superscript minus sign
```

```text
<utf8-exp-digit>: one of
        U+2070      // superscript zero
        U+00B9      // superscript one
        U+00B2      // superscript two
        U+00B3      // superscript three
        U+2074      // superscript four
        U+2075      // superscript five
        U+2076      // superscript six
        U+2077      // superscript seven
        U+2078      // superscript eight
        U+2079      // superscript nine
```

Examples:

```text
m²
s⁻¹
```

For user-oriented guidance on operators, see [Operators](unit-expression-operators.html).

## Date and Time Elements

Reference-time expressions use the `<Timestamp>` non-terminal.

```text
<Timestamp>: one of
        DATE
        DATE CLOCK
        DATE CLOCK CLOCK
        DATE CLOCK ID
        DATE ID
```

The second `CLOCK` represents a numeric time-zone offset.
The `ID` forms represent a time-zone identifier.

### Dates

```text
DATE: one of
        <date-broken> ("T" | <space>*)
        <date-packed> ("T" | <space>*)
```

The trailing `T` or whitespace serves as the separator when a `CLOCK` follows.

Packed dates are recognized as `DATE` only when they occur after time-related units.
Otherwise, they are interpreted as `REAL`.

### Broken-Form Dates

```text
<date-broken>:
        <year> "-" <month> ("-" <day>)?
```

The date may be truncated from the right.

Examples:

```text
2000
2000-01
2000-01-15
```

### Packed Dates

```text
<date-packed>:
        <year> (<month> <day>)?
```

Packed dates may also be truncated from the right.
The interpretation is based on the number of digits, excluding a leading sign:

| Length | Interpretation                        |
| -----: | ------------------------------------- |
|    1–4 | `Y`, `YY`, `YYY`, `YYYY` → `YYYY0101` |
|      5 | `YYYYM` → `YYYY0M01`                  |
|      6 | `YYYYMM` → `YYYYMM01`                 |
|      7 | `YYYYMMD` → `YYYYMM0D`                |
|      8 | `YYYYMMDD` → `YYYYMMDD`               |

A decimal point and decimal digits cause the value to be interpreted as a `REAL` rather than a `DATE`.

### Year

```text
<year>:
        [+-]? [0-9]{1,4}
```

Year zero is silently interpreted as year 1.

### Month

```text
<month>:
        "0"?[1-9] | "1"[0-2]
```

### Day

```text
<day>:
        "0"?[1-9] | [1-2][0-9] | "30" | "31"
```

### Clocks

```text
CLOCK: one of
        <clock-broken> <space>*
        <clock-packed> <space>*
```

The trailing whitespace is part of the `CLOCK` token.

#### Broken-Form Times

```text
<clock-broken>:
        <hour> ":" <minute> (":" <second>)?
```

The time may be truncated from the right.

Examples:

```text
12:00
12:00:30
```

#### Packed Times

```text
<clock-packed>:
        <hour> (<minute> (<second>)?)?
```

The interpretation is based on the length of the integer portion before any decimal point:

| Length | Interpretation      | Decimals    |
| -----: | ------------------- | ----------- |
|      1 | `H` → `0H0000`      | Not allowed |
|      2 | `HH` → `HH0000`     | Not allowed |
|      3 | `HHM` → `HH0M00`    | Allowed     |
|      4 | `HHMM` → `HHMM00`   | Allowed     |
|      5 | `HHMMS` → `HHMM0S`  | Allowed     |
|      6 | `HHMMSS` → `HHMMSS` | Allowed     |

### Hour

```text
<hour>:
        [+-]?[0-1]?[0-9] | 2[0-3]
```

The sign is used for time-zone offsets.
The allowed range for a signed hour is 0–19, while unsigned times may extend through 23.

### Minute

```text
<minute>:
        [0-5]?[0-9]
```

### Second

```text
<second>:
        (<minute> | "60") ("." <digit>*)?
```

The grammar permits a leap-second value of `60`.

According to the reference behavior, a leap second is silently interpreted as the same as `00` of the immediately succeeding minute.

## Reference-Time Expressions

Reference-time expressions combine a time unit, shift operator, and timestamp.

Examples:

```text
seconds since 1970-01-01
hours since 2000-01-01 12:00:00
hours since 2024-01-01 00:00:00 UTC
```

The supported shift operators are:

```text
@
after
from
since
ref
```

For a user-oriented explanation of reference-time expressions, see [Reference Time Units](unit-expression-reference-time-units.html).

For the complete date, time, and time-zone syntax, see the preceding [Date and Time Elements](#date-and-time-elements) section.

## Logarithmic Elements

Logarithmic expressions use the `LOGREF` terminal.

```text
LOGREF:
        <log> <space>* "(" <space>* <re> ":"? <space>*
```

The `LOGREF` token includes the logarithm function, opening parenthesis, reference marker, optional colon, and trailing whitespace, but not the closing parenthesis.

The closing parenthesis is supplied by the enclosing `<Basic-Spec>` production:

```text
<Basic-Spec>: one of
        ID
        "(" <Shift-Spec> ")"
        LOGREF <Product-Spec> ")"
        <Number>
```

### Logarithm Functions

```text
<log>: one of
        "log"
        "lg"
        "ln"
        "lb"
```

### Reference Marker

```text
<re>: one of
        "re"
        "RE"
```

A reference may optionally be followed by a colon.

Examples:

```text
log(re W)
lg(re W)
ln(re W)
lb(re W)
```

## Grammar Hierarchy

The principal grammar structure can be summarized as:

```text
<Unit-Spec>
    └── <Shift-Spec>
          └── <Product-Spec>
                └── <Power-Spec>
                      └── <Basic-Spec>
                            ├── ID
                            ├── parenthesized expression
                            ├── LOGREF
                            └── <Number>
```

The lexical definitions provide the terminals used by these productions, including identifiers, numbers, operators, dates, times, and logarithmic references.

## Interpretation and Implementation Behavior

Some rules in this reference describe how UDUNITS-2 interprets otherwise syntactically valid input rather than defining syntax alone.

These behaviors include:

- Packed dates are interpreted according to their digit length.
- Packed dates are recognized as dates only in the appropriate time-unit context.
- A decimal point causes a numeric value to be interpreted as a `REAL` rather than a packed date.
- Year zero is interpreted as year 1.
- A leap-second value of `60` is interpreted as `00` of the immediately succeeding minute.

These behaviors are documented here because they affect how valid input is interpreted by UDUNITS-2.

## Complete Specification

This page is the authoritative reference for the complete unit expression language.
The [User Guide Grammar](unit-expression-grammar.html) page provides a condensed overview of the same grammar for readers who need to understand the structure without reviewing every lexical production.

The complete specification includes:

- grammar productions
- whitespace and character definitions
- identifier construction
- integer and real number syntax
- multiplication, division, exponent, and shift operators
- parenthesized expressions
- date and time syntax
- packed and broken date and time forms
- time-zone representations
- logarithmic expressions
- terminal and non-terminal definitions

## See Also

- **[User Guide: Unit Expression Language](unit-expression-language.html)**
- **[User Guide: Units Database](units-database.html)**
