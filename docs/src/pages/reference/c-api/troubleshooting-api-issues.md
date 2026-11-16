---
title: Troubleshooting API Issues
sidebar: sidebar
toc: true
permalink: troubleshooting-api-issues.html
---

Most **UDUNITS-2 C API usage issues** fall into a small number of common categories.

Before using the troubleshooting tables below, ensure that the **unit system
context**, **unit definitions**, and **conversion workflow** follow the expected
lifecycle described in the UDUNITS-2 C API reference.

## Unit Parsing Issues

These errors occur when converting text unit specifications into
[`ut-unit`](ut-unit.html) objects using [`ut-parse()`](ut-parse.html).

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| [`ut_parse()`](ut-parse.html) returns NULL | Unit string not recognized by grammar | Enable error handler using [`ut_set_error_message_handler()`](ut-set-error-message-handler.html) and inspect message | Verify spelling and grammar using `udunits2 -H` |
| Valid-looking unit fails to parse using [`ut_parse()`](ut-parse.html) | String uses unsupported syntax | Compare with canonical examples | Rewrite using UDUNITS grammar conventions |
| Prefix interpreted incorrectly | Ambiguous prefix usage | Inspect parsed output using [`ut_format()`](ut-format.html) | Use explicit unit symbols |
| Expression parses but produces unexpected unit | Operator precedence misunderstood | Review parentheses in unit expression | Add explicit grouping |
| Parsing fails for exponent notation in [`ut_parse()`](ut-parse.html) | Unsupported superscript characters used | Check character encoding | Use ASCII exponent syntax (`m^2`) |

## Unit-System Consistency Issues

These issues occur when units belong to different `ut_system*` contexts.

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| [`ut_are_convertible()`](ut-are-convertible.html) returns 0 unexpectedly | Units created from different [`ut_system`](ut-system.html) instances | Verify system pointer used when calling [`ut_parse()`](ut-parse.html) | Ensure all units originate from the same [`ut_system`](ut-system.html) |
| Conversion fails between equivalent units | Units defined in separate XML databases | Compare system objects created by [`ut_read_xml()`](ut-read-xml.html) | Load units from the same XML database file |
| Custom unit cannot convert with standard unit | Unit added to different system instance | Trace system creation calls using [`ut_new_base_unit()`](ut-new-base-unit.html) | Define custom units within the same [`ut_system`](ut-system.html) context |

## Conversion Semantics Issues

These problems occur when computing numeric conversions using [`ut_get_converter()`](ut-get-converter.html).

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| Conversion produces incorrect numeric results | Units not dimensionally compatible | Verify compatibility using [`ut_are_convertible()`](ut-are-convertible.html) before calling [`ut_get_converter()`](ut-get-converter.html) | Confirm dimensional equivalence of both [`ut_unit`](ut-unit.html) objects |
| Temperature conversion produces unexpected offset | Affine conversion required for temperature scales | Inspect converter created by [`ut_get_converter()`](ut-get-converter.html) and evaluate results using [`cv_convert_double()`](cv-convert-double.html) | Ensure correct temperature units (e.g., absolute vs relative) are used |
| Converter creation fails | Units not compatible | Check compatibility using [`ut_are_convertible()`](ut-are-convertible.html) | Convert through a compatible intermediary unit or revise unit definitions |
| Conversion works but values appear scaled incorrectly | Prefix or exponent misinterpreted during parsing | Inspect parsed representation using [`ut_format()`](ut-format.html) after [`ut_parse()`](ut-parse.html) | Validate input unit string and exponent syntax |

## Memory Ownership Issues

These issues occur when managing object lifetimes for [`ut_unit`](ut-unit.html)
and converter objects returned by [`ut_get_converter()`](ut-get-converter.html).

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| Memory leak detected | Unit objects not freed | Audit lifecycle of objects created by [`ut_parse()`](ut-parse.html), [`ut_scale()`](ut-scale.html), or [`ut_multiply()`](ut-multiply.html) | Release units using [`ut_free()`](ut-free.html) |
| Crash during cleanup | Object freed multiple times | Trace ownership of objects returned by [`ut_clone()`](ut-clone.html) or [`ut_parse()`](ut-parse.html) | Ensure each [`ut_unit`](ut-unit.html) object is freed exactly once using [`ut_free()`](ut-free.html) |
| Converter object persists unexpectedly | Converter not explicitly freed | Inspect lifecycle of converter created by [`ut_get_converter()`](ut-get-converter.html) | Free converter using [`cv_free()`](cv-free.html) |
| Application frees memory incorrectly | Using `free()` instead of API lifecycle functions | Search code for incorrect deallocation of [`ut_unit`](ut-unit.html) or converter objects | Use [`ut_free()`](ut-free.html) and [`cv_free()`](cv-free.html) instead of `free()` |

## Error Handling Issues

These issues occur when diagnostic messages are not captured or interpreted correctly.

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| Function returns NULL with no explanation | Error handler not configured | Register custom handler using [`ut_set_error_message_handler()`](ut-set-error-message-handler.html) | Configure error reporting before calling functions such as [`ut_parse()`](ut-parse.html) |
| Unexpected error messages | Handler state persists across calls | Inspect global error handler configuration set by [`ut_set_error_message_handler()`](ut-set-error-message-handler.html) | Reset or replace handler prior to subsequent API calls |
| Error messages missing context | Handler does not include sufficient diagnostic detail | Enable structured logging within custom handler registered via [`ut_set_error_message_handler()`](ut-set-error-message-handler.html) | Add contextual logging around calls to [`ut_parse()`](ut-parse.html) or [`ut_get_converter()`](ut-get-converter.html) |

## Unit Definition Issues

These issues arise when working with built-in or custom unit definitions.

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| Expected unit symbol not recognized | Symbol not defined in unit database | Inspect definitions loaded via [`ut_read_xml()`](ut-read-xml.html) | Use alternative symbol or define custom unit using [`ut_new_base_unit()`](ut-new-base-unit.html) |
| Unit interpreted differently than expected | Symbol conflict or ambiguity | Inspect parsed structure using [`ut_parse()`](ut-parse.html) and format using [`ut_format()`](ut-format.html) | Use explicit unit expression (e.g., products via [`ut_multiply()`](ut-multiply.html)) |
| Alias from UDUNITS-1 not recognized | Definition removed or renamed in UDUNITS-2 database | Compare legacy symbols with units loaded via [`ut_read_xml()`](ut-read-xml.html) | Use updated canonical symbol recognized by [`ut_parse()`](ut-parse.html) |
| Dimensionless unit behaves unexpectedly | Unit defined incorrectly or missing dimensionless representation | Inspect unit structure returned by [`ut_parse()`](ut-parse.html) or constructed using [`ut_scale()`](ut-scale.html) | Define explicitly dimensionless unit using [`ut_scale()`](ut-scale.html) or equivalent identity unit |

## Encoding Issues

These problems occur when unit strings contain unsupported character encodings.

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| Parsing fails for valid-looking symbols | UTF-8 characters not recognized by parser | Inspect encoding flags used with [`ut_parse()`](ut-parse.html) | Use ASCII equivalents when calling [`ut_parse()`](ut-parse.html) |
| Superscript characters fail to parse | Non-ASCII exponent notation used | Examine byte representation of input passed to [`ut_parse()`](ut-parse.html) | Replace superscripts with ASCII exponent syntax (`^`) before calling [`ut_parse()`](ut-parse.html) |
| Unit string behaves differently across environments | Locale differences affect parsing behavior | Compare locale and encoding assumptions when invoking [`ut_parse()`](ut-parse.html) | Normalize input encoding prior to parsing |

## Legacy Migration Issues

These issues occur when porting legacy application from UDUNITS-1 to UDUNITS-2.

| Symptom | Cause | Diagnosis | Fix |
| ------ | ----- | --------- | --- |
| Previously valid unit no longer recognized | Database definitions updated in UDUNITS-2 | Compare definitions loaded via [`ut_read_xml()`](ut-read-xml.html) | Update unit expressions passed to [`ut_parse()`](ut-parse.html) |
| Legacy API assumptions fail | Function behavior changed between UDUNITS-1 and UDUNITS-2 | Review semantics of [`ut_parse()`](ut-parse.html), [`ut_get_converter()`](ut-get-converter.html), and [`ut_are_convertible()`](ut-are-convertible.html) | Refactor code to follow UDUNITS-2 unit-system lifecycle |
| Conversion results differ from UDUNITS-1 | Dimensional analysis rules more strictly enforced | Compare dimensional compatibility using [`ut_are_convertible()`](ut-are-convertible.html) | Update conversion workflow using [`ut_get_converter()`](ut-get-converter.html) |
| Symbol interpreted differently | Legacy alias removed or renamed in XML database | Inspect definitions loaded via [`ut_read_xml()`](ut-read-xml.html) | Use canonical symbol recognized by [`ut_parse()`](ut-parse.html) |
