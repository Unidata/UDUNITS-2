---
title: Basic Examples
sidebar: sidebar
toc: true
permalink: basic-examples.html
---

This page demonstrates common UDUNITS-2 tasks using both the `udunits2` command-line utility and the C library.
These examples are intended to help you get started quickly.
For complete documentation, see the User Guide and C API reference.

## Command-Line Examples

### Convert Length

```text
You have: 1 m
You want: ft

    x/0.3048
```

### Convert Speed

```text
You have: 10 m/s
You want: km/h
```

### Convert Temperature

```text
You have: 25 degC
You want: degF
```

### Convert Pressure

```text
You have: 1013.25 hPa
You want: Pa
```

### Convert Reference Time Units

```text
You have:
hours since 2024-01-01 00:00:00

You want:
seconds since 2024-01-01 00:00:00
```

## Common Unit Expressions

The following are valid unit expressions recognized by UDUNITS-2.

| Quantity | Expression |
| -------- | ---------- |
| Velocity | `m/s` |
| Acceleration | `m/s2` |
| Force | `N` |
| Energy | `J` |
| Power | `W` |
| Pressure | `Pa` |
| Density | `kg/m3` |
| Reference Time | `days since 1970-01-01` |

## C Library Examples

### Parse a Unit

```c
ut_system* sys = ut_read_xml(NULL);
ut_unit* meter = ut_parse(sys, "m", UT_UTF8);
```

### Convert Between Units

```c
ut_unit* foot = ut_parse(sys, "ft", UT_UTF8);
cv_converter* cv = ut_get_converter(meter, foot);

double value = cv_convert_double(cv, 1.0);
```

### Check Unit Compatibility

```c
if (ut_are_convertible(meter, foot)) {
    /* Units are compatible */
}
```

### Clean Up Resources

```c
cv_free(cv);
ut_free(meter);
ut_free(foot);
ut_free_system(sys);
```

## Next Steps

- Learn the **[Unit Expression Language](unit-expression-language.html)** to understand supported unit syntax.
- Explore the **[Units Database](units-database.html)** to customize or extend available unit definitions.
- Learn how to use the **[C API](using-the-c-api.html)** to integrate UDUNITS-2 into your applications.
- Refer to the **CLI Reference** for command-line options and usage.
