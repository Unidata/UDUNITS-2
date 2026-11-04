---
title: Unit Time Utilities API
sidebar: sidebar
toc: true
permalink: unit-time-utilities-api.html
---

Functions for encoding and decoding time values in UDUNITS-2 format.

These utilities are commonly used when working with time-based unit offsets
(e.g., "`seconds since 1970-01-01`"), enabling conversion between structured
date/time components and UDUNITS-compatible numeric representations.

## Functions

| Function | Description |
| ---------- | ------------- |
| [`ut_decode_time()`](ut-decode-time.html) | Decodes a UDUNITS-2 numeric time value into calendar date and clock time components |
| [`ut_encode_clock()`](ut-encode-clock.html) | Encodes clock time components (hour, minute, second) into a UDUNITS-2 numeric time value |
| [`ut_encode_date()`](ut-encode-date.html) | Encodes calendar date components (year, month, day) into a UDUNITS-2 numeric time value |
| [`ut_encode_time()`](ut-encode-time.html) | Encodes complete date and time components into a UDUNITS-2 numeric time value |

## Usage Patterns

The **Unit Time Utilities API** operates as a progression from  
**structured date/time components → encoded UDUNITS-2 time values → `ut_unit`
objects with temporal offsets → optional arithmetic or origin adjustments →
`cv_converter`-mediated transformations → parsed or formatted unit strings
within a [`ut_system`](ut-system.html) → decoded calendar components**, enabling transformation
between calendar-based representations and numeric time values that integrate
with UDUNITS-2 conversion workflows.

For example:

```text
(1) Input date/time components
    year, month, day, hour, minute, second
                │
                ▼
(2) ╔════════════════════════════════╗
    ║ Encode structured time values  ║
    ║ • ut_encode_date()             ║
    ║ • ut_encode_clock()            ║
    ║ • ut_encode_time()             ║
    ╚════════════════════════════════╝
                │
                ▼
(3) Encoded UDUNITS-2 numeric time value
                │
                ▼
(4) Apply arithmetic or offsets
    • ut_add()
    • ut_offset()
                │
                ▼
(5) Parse or format time-based unit expressions
    • ut_parse()
    • ut_format()
                │
                ▼
(6) ╔════════════════════════════════╗
    ║ Decode UDUNITS-2 time value    ║
    ║ into calendar components       ║
    ║ • ut_decode_time()             ║
    ╚════════════════════════════════╝
                │
                ▼
(7) Output date/time components
    year, month, day, hour, minute, second
```

1. **Input date/time components**

   Operations begin with structured calendar values representing a specific
   moment in time. Typical components include:

   - year
   - month
   - day
   - hour
   - minute
   - second

   These values typically originate from application data, timestamps,
   or external data sources.

2. **Encode structured time values**

   Calendar components are converted into UDUNITS-2 numeric time values
   representing elapsed time relative to a reference epoch.

   - [`ut_encode_date()`](ut-encode-date.html) encodes year, month, and day
   - [`ut_encode_clock()`](ut-encode-clock.html) encodes hour, minute, and second
   - [`ut_encode_time()`](ut-encode-time.html) encodes complete date and time values

   These values may later be associated with a [`ut_unit`](ut-unit.html) representing a
   time-offset unit.

3. **Encoded UDUNITS-2 numeric time value available**

   The result of encoding is a numeric value (typically `double`)
   representing a time offset.

   This value can be combined with a parsed time reference unit within a
   [`ut_system`](ut-system.html), for example:

   - `"seconds since 1970-01-01"`
   - `"days since 1850-01-01"`

   The resulting representation links a numeric value with a corresponding
   [`ut_unit`](ut-unit.html) describing the temporal reference frame.

4. **Apply arithmetic or offsets**

   Time-offset [`ut_unit`](ut-unit.html) objects can be constructed or modified relative to
   reference origins.

   - [`ut_offset()`](ut-offset.html) creates a [`ut_unit`](ut-unit.html) with a shifted origin
   - [`ut_add()`](ut-add.html) combines compatible units to produce derived
     [`ut_unit`](ut-unit.html) objects

   These operations support construction of units describing elapsed time
   relative to defined epochs.

5. **Parse or format time-based unit expressions**

   Unit expressions defining temporal reference frames are interpreted or
   generated within a [`ut_system`](ut-system.html).

   - [`ut_parse()`](ut-parse.html) converts a unit expression string into a [`ut_unit`](ut-unit.html)
   - [`ut_format()`](ut-format.html) generates a string representation of a [`ut_unit`](ut-unit.html)

   Parsed units may subsequently be used to create a [`cv_converter`](cv-converter.html) for
   transformations between compatible time representations.

6. **Convert or interpret temporal values**

   Compatible time-offset units can be related using a [`cv_converter`](cv-converter.html).

   - [`ut_get_converter()`](ut-get-converter.html) obtains a [`cv_converter`](cv-converter.html)
     relating two compatible [`ut_unit`](ut-unit.html) objects

   This enables transformation between time representations expressed
   relative to different reference epochs or units.

7. **Decode UDUNITS-2 time value into calendar components**

   Numeric UDUNITS-2 time values can be converted back into structured date
   and time components.

   - [`ut_decode_time()`](ut-decode-time.html) extracts year, month, day,
     hour, minute, and second values

   This enables presentation, reporting, or further calendar-based
   processing.

8. **Output date/time components**

   The application receives structured calendar values derived from the
   UDUNITS-2 numeric representation.

   Typical outputs include:

   - year
   - month
   - day
   - hour
   - minute
   - second

   These values can be displayed, stored, or used in subsequent
   application logic.

## See Also

- [Unit Arithmetic API](unit-arithmetic-api.html)
