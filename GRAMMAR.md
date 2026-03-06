Here is the unit-syntax understood by the UDUNITS-2 package. Words printed \_Thusly\_ indicate non-terminals; words printed THUSLY indicate terminals; and words printed \<thusly\> indicate lexical elements.

// Main Grammar Structure

    _Unit-Spec_: one of
            nothing
            _Shift-Spec_

    _Shift-Spec_: one of
            _Product-Spec_
            _Product-Spec_ SHIFT REAL
            _Product-Spec_ SHIFT INT
            _Product-Spec_ SHIFT _Timestamp_

    _Product-Spec_: one of
            _Power-Spec_
            _Product-Spec_ _Power-Spec_
            _Product-Spec_ MULTIPLY _Power-Spec_
            _Product-Spec_ DIVIDE _Power-Spec_

    _Power-Spec_: one of
            _Basic-Spec_
            _Basic-Spec_ INT
            _Basic-Spec_ EXPONENT

    _Basic-Spec_: one of
            ID
            "(" _Shift-Spec_ ")"
            LOGREF _Product-Spec_ ")"      // token includes "<log>(" and base marker; parser provides the closing ")"
            _Number_

// Lexical Building Blocks

    <space>: one of
            U+0020      // space
            U+0009      // horizontal tab
            U+000D      // carriage return
            U+000C      // form feed
            U+000B      // vertical tab
            // Note: newline (U+000A) is NOT included
            // Note: non-breaking space (U+00A0) is NOT included

    <digit>:
            [0-9]

    <alpha>:
            [A-Za-z_]
            <iso-8859-1-alpha>

    <iso-8859-1-alpha>: one of
            U+00A0          // non-breaking space (sic!) allowed in identifiers
            U+00AD          // soft hyphen
            U+00B0          // degree sign
            U+00B5          // micro sign
            U+00C0-U+00D6   // Latin capital letters with diacritics (À-Ö, excluding U+00D7 multiplication sign)
            U+00D8-U+00F6   // Latin capital and small letters with diacritics (Ø-ö, excluding U+00F7 division sign)
            U+00F8-U+00FF   // Latin small letters with diacritics (ø-ÿ)

    <alphanum>: one of
            <alpha>
            <digit>

// Numbers

    _Number_: one of
            INT
            REAL

    INT:
            [+-]? <digit>+                    // optional sign followed by one or more digits

    REAL:
            [+-]? <real-value>

    <real-value>: one of
            <digit>+ <exponent>               // e.g., 5e10, 3E-2
            <mantissa> <exponent>?            // e.g., 3.14, .5, 2., 3.14e-10

    <mantissa>: one of
            <digit>+ "."                      // e.g., 2.
            "." <digit>+                      // e.g., .5
            <digit>+ "." <digit>+             // e.g., 3.14

    <exponent>:
            ("e" | "E") [+-]? <digit>+

    // Note: NaN and Inf[inity] are explicitly excluded (cf. PR #136)

// Identifiers

    ID: one of
            <id>
            "%"
            "'"         // single quote
            "\""        // double quote
            U+00B0      // degree sign
            U+00B5      // micro sign

    <id>: one of
            <alpha>
            <alpha> <alphanum>* <alpha>
            // Multi-character identifiers must end with <alpha>, not <digit>

// Operators

    SHIFT:
            <space>* <shift-op> <space>*

    <shift-op>: one of
            "@"
            "after"
            "from"
            "since"
            "ref"

    MULTIPLY: one of
            "-"         // accepted as multiply operator in running text as per the SI Brochure
            "."
            "*"
            <space>+
            U+00B7      // centered middot

    DIVIDE:
            <space>* <divide-op> <space>*

    <divide-op>: one of
            " per "      // surrounding space required, cf. PR #135
            " PER "      // surrounding space required, cf. PR #135
            "/"

    EXPONENT: one of
            <raise-with-int>
            <utf8-exponent>

    <raise-with-int>:
            ("^" | "**") INT

    <utf8-exponent>:
            <utf8-exp-sign>? <utf8-exp-digit>+

    <utf8-exp-sign>: one of
            U+207A      // superscript plus sign
            U+207B      // superscript minus sign

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

// Date/Time Elements

    _Timestamp_: one of
            DATE
            DATE CLOCK
            DATE CLOCK CLOCK    // timezone offset as a second CLOCK, which cannot have <second>
            DATE CLOCK ID       // timezone ID (UTC/GMT/Z)
            DATE ID             // date-only with timezone ID (only Z)

    DATE: one of
            <date-broken> ("T" | <space>*)
            <date-packed> ("T" | <space>*)
            // NOTE: The trailing "T" or <space>* is included in the DATE token and serves as a separator when followed by CLOCK
            // NOTE: Packed dates only return DATE token when appearing after time-related units; otherwise parsed as REAL

    <date-broken>:
            <year> "-" <month> ("-" <day>)?  // allow truncation from the right

    <date-packed>:
            <year> (<month> <day>)?
            // Allow truncation from the right:
            // Length-based interpretation for <date-packed> (not counting a sign):
            //   len = 1-4  → Y, YY, YYY, YYYY  → YYYY0101
            //   len = 5    → YYYYM             → YYYY0M01
            //   len = 6    → YYYYMM            → YYYYMM01
            //   len = 7    → YYYYMMD           → YYYYMM0D
            //   len = 8    → YYYYMMDD          → YYYYMMDD
            //   decimal point and decimals are accepted but then <date-packed> is interpreted as a REAL and not as a DATE

    CLOCK: one of
            <clock-broken> <space>*
            <clock-packed> <space>*
            // NOTE: The trailing <space>* is included in the CLOCK token

    <clock-broken>:
            <hour> ":" <minute> (":" <second>)?  // allow truncation from the right

    <clock-packed>:
            <hour> (<minute> (<second>)?)?
            // Allow truncation from the right:
            // Length-based interpretation for <clock-packed>.
            // Length is measured on the INTEGER part (before any decimal point).
            //   len = 1  → H       → 0H0000   decimals are not allowed
            //   len = 2  → HH      → HH0000   decimals are not allowed
            //   len = 3  → HHM     → HH0M00   decimals accepted: HH0M00.dddd...
            //   len = 4  → HHMM    → HHMM00   decimals accepted: HHMM00.dddd...
            //   len = 5  → HHMMS   → HHMM0S   decimals accepted: HHMM0S.dddd...
            //   len = 6  → HHMMSS  → HHMMSS   decimals accepted: HHMMSS.dddd...

    <year>:
            [+-]? [0-9]{1,4}    // year 0 is silently interpreted as year 1

    <month>:
            "0"?[1-9] | "1"[0-2]

    <day>:
            "0"?[1-9] | [1-2][0-9] | "30" | "31"

    <hour>:
            [+-]?[0-1]?[0-9] | 2[0-3]
            // In principle sign may appear in both time-of-day and timezone offsets. However,
            // sign only applies to 0-19, not 20-23. This is correct: time-of-day (0-23) should
            // be unsigned whereas timezone offsets that are in the range (±0-14) should have signs.

    <minute>:
            [0-5]?[0-9]

    <second>:
            (<minute> | "60") (\. <digit>*)?
            // leap second (= 60) is silently interpreted as the same as 00 of the immediately succeeding minute)

// Logarithmic Elements

    LOGREF:
            <log> <space>* "(" <space>* <re> ":"? <space>*   // NOTE: LOGREF token spans through the base marker (e.g., "lg(re" or "ln(re:"), but not the closing ")"

    <log>: one of
            "log"
            "lg"
            "ln"
            "lb"

    <re>: one of
            "re"
            "RE"
