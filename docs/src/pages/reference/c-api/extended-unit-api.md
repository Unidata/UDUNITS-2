---
title: Extended Unit API
sidebar: sidebar
toc: true
permalink: extended-unit-api.html
---

This API is intended for internal use cases, enabling custom traversal,
inspection, or extension of unit structures beyond standard operations.

## Functions

| Function | Description |
| -------- | ----------- |
| [`ut_accept_visitor()`](ut-accept-visitor.html) | Traverses a unit using a visitor that invokes type-specific callbacks |

## Usage Patterns

Unlike simple string-to-string processing, the **Extended Unit API** operates
as a progression from **string → [`ut_unit`](ut-unit.html) → applied logic → output**
(such as a string, [`cv_converter`](cv-converter.html),comparison result, or custom structure),
where the **intermediate structured form enables advanced inspection,
validation, and transformation**.

For example:

```text
(1) Input unit expression (string)
    "kg·m^2/s^2"
                │
                ▼
(2) Parse string and construct ut_unit object
    • ut_parse()
                │
                ▼
(3) Obtain ut_unit object
    (structured representation of the unit)
    [conceptual form: kg:1, m:2, s:-2]
                │
                ▼
(4) ╔══════════════════════════════╗
    ║ Traverse unit structure      ║
    ║ and apply custom logic       ║
    ║ • ut_accept_visitor()        ║
    ╚══════════════════════════════╝
                │
                ▼
(5) Apply custom logic
    • Inspect components
    • Validate structure
    • Transform or extract info
                │
                ▼
(6) Output:
    (string | conversion | comparison | custom result)
 ```

1. **Input String**

   The process begins with a textual unit representation, such as `"kg·m²/s²"`.
   At this stage, the unit is simply a string with no structural meaning.

2. **String parsing via [`ut_parse()`](ut-parse.html)**

   The string is parsed within a [`ut_system`](ut-system.html) using
   [`ut_parse()`](ut-parse.html), which interprets the syntax and constructs a
   [`ut_unit`](ut-unit.html) object representing the unit.

3. **Structured representation in [`ut_unit`](ut-unit.html) object**

   The resulting [`ut_unit`](ut-unit.html) object provides a structured, normalized representation
   of the unit:

   - Base units are associated with integer exponents (e.g., `kg: 1`, `m: 2`,
     `s: -2`)
   - Positive exponents represent units in the numerator, while negative exponents
     represent units in the denominator

   This representation captures the dimensional meaning of the unit independent
   of its textual form, enabling reliable analysis, comparison, and transformation.

4. **Extension point via [`ut_accept_visitor()`](ut-accept-visitor.html)**

   [`ut_accept_visitor()`](ut-accept-visitor.html) enables traversal of the
   [`ut_unit`](ut-unit.html) object, providing a mechanism to apply custom logic to the unit's
   internal representation. This function serves as the primary extension point
   for advanced or specialized processing of [`ut_unit`](ut-unit.html) objects.

5. **Apply custom logic**

   During traversal, user-defined logic can:

   - Inspect components of the `ut_unit`
   - Validate structural or domain-specific constraints
   - Extract dimensional information
   - Transform or reinterpret unit content

   This is where the flexibility of the **Extended Unit API** is realized.

6. **Output**

   The applied logic produces an output appropriate to the use case, such as:

   - a formatted unit string via [`ut_format()`](ut-format.html)
   - a comparison result via [`ut_compare()`](ut-compare.html)
   - a conversion relationship represented by a [`cv_converter`](cv-converter.html) (e.g.,
     via [`ut_get_converter()`](ut-get-converter.html))
   - a custom structure or result defined by the application

## See Also

- [Unit Query & Introspection API](unit-query-introspection-api.html)
- [Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [Unit Arithmetic API](unit-arithmetic-api.html)
