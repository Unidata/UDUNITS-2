---
title: Error Handling API
sidebar: sidebar
toc: true
permalink: error-handling-api.html
---

The Error Handling API Functions for managing error reporting, status tracking,
and controlling how UDUNITS-2 reports and responds to errors.

It allows applications to query and set error status, customize error message
handling, and suppress or raise errors as needed.

Error handling behavior is global to the library and not thread-local, so
configuration should typically be performed during initialization to ensure
consistent behavior.

Suppressing errors (e.g., via [`ut_ignore()`](ut-ignore.html)) should be used
cautiously to avoid silent failures.

## Functions

| Function | Description |
| ---------- | ------------- |
| [`ut_get_status()`](ut-get-status.html) | Get current global error status |
| [`ut_handle_error_message()`](ut-handle-error-message.html) | Format and dispatch error message to handler |
| [`ut_ignore()`](ut-ignore.html) | Suppress errors matching a regex pattern |
| [`ut_set_error_message_handler()`](ut-set-error-message-handler.html) | Set error handler and return previous handler |
| [`ut_set_status()`](ut-set-status.html) | Set global error status and return previous value |
| [`ut_write_to_stderr()`](ut-write-to-stderr.html) | Write error message to `stderr` |

## Usage Patterns

The **Error Handling API** operates as a progression from **configuration →
operations on [`ut_system`](ut-system.html), [`ut_unit`](ut-unit.html), and [`cv_converter`](cv-converter.html) objects → error status
evaluation → application response**, enabling controlled detection,
customization, and management of errors that arise during unit parsing,
conversion, and arithmetic workflows.

For example:

```text
(1) ╔══════════════════════════════╗
    ║ Configure error handling     ║
    ║ strategy                     ║
    ║                              ║
    ║ ut_set_error_message_handler ║
    ║ ut_ignore()                  ║
    ╚══════════════════════════════╝
                │
                ▼
(2) Perform UDUNITS-2 library operations  
    • ut_parse() 
    • ut_get_converter() 
    • ut_multiply() 
    • ...                          
                │
                ▼
(3) ╔══════════════════════════════╗
    ║ Generate or dispatch error   ║
    ║ messages                     ║
    ║                              ║
    ║ ut_handle_error_message()    ║
    ║           │                  ║
    ║           ▼                  ║
    ║ ut_write_to_stderr()         ║
    ║          OR                  ║
    ║ custom handler               ║
    ╚══════════════════════════════╝
                │
                ▼
(4) ╔══════════════════════════════╗
    ║ Inspect or control error     ║
    ║ state via status             ║
    ║                              ║
    ║ ut_get_status()              ║
    ║ ut_set_status()              ║
    ╚══════════════════════════════╝
                │
                ▼
(5) Application handles or propagates errors
```

1. Configure error handling strategy  
   Define how errors should be processed by setting a custom handler with
   [`ut_set_error_message_handler()`](ut-set-error-message-handler.html) or
   suppressing selected messages with [`ut_ignore()`](ut-ignore.html).

2. Perform UDUNITS-2 operations  
   Execute library functions that may produce errors (e.g., [`ut_parse()`](ut-parse.html),
   [`ut_get_converter()`](ut-get-converter.html), [`ut_multiply()`](ut-multiply.html)).

3. Generate or dispatch error messages  
   When an error occurs, [`ut_handle_error_message()`](ut-handle-error-message.html)
   formats the message and forwards it to either [`ut_write_to_stderr()`](ut-write-to-stderr.html)
   or a custom handler previously configured via [`ut_set_error_message_handler()`](ut-set-error-message-handler.html).

4. Inspect or control error state  
   Query or modify the global error status using [`ut_get_status()`](ut-get-status.html)
   and [`ut_set_status()`](ut-set-status.html).

5. Application handles or propagates errors  
   The application determines how to respond, recover, or propagate the error condition.

## See Also

- [Unit Systems API](unit-systems-api.html)
- [Unit Parsing & Formatting API](unit-parsing-formatting-api.html)
- [Unit Conversion API](unit-conversion-api.html)
- [Unit Arithmetic API](unit-arithmetic-api.html)
