/*
 * Copyright 2026 University Corporation for Atmospheric Research
 *
 * This file is part of the UDUNITS-2 package.  See the file COPYRIGHT
 * in the top-level source-directory for copying and redistribution
 * conditions.
 *
 * Constants and limits shared between translation units of the library.
 *
 * This header is deliberately small and is not installed.  It exists so that
 * a value used in more than one place has a single definition rather than a
 * copy per user.
 *
 * What belongs here: shared internal constants and limits -- a number, a
 * range, a size -- together with a comment explaining what the value means
 * and, where the value is a policy rather than a property of a type, that it
 * may be changed.
 *
 * What does not belong here: anything substantial.  A set of related
 * declarations, a data structure and its operations, or an internal interface
 * of any size belongs in its own header, as converter.h, idToUnitMap.h,
 * prefix.h, systemMap.h, unitAndId.h and unitToIdMap.h already do.  This file
 * is not a place to accumulate declarations that have not been given a home.
 */

#ifndef UT_UDUNITS2_INTERNAL_H_INCLUDED
#define UT_UDUNITS2_INTERNAL_H_INCLUDED

/*
 * Greatest magnitude permitted for the power of a unit, in a specification and
 * in the result of an operation on units.
 *
 * This is a software policy limit rather than a bound arising from quantity
 * calculus or from the representation type.  Dimensional exponents have no
 * mathematical upper bound, and no rationale for this particular value is
 * recorded anywhere in the source; it is retained for compatibility and for
 * round-trip consistency, and may be changed.  Two constraints on any
 * replacement:
 *
 *   - it must be representable in the type of ProductUnit.powers, so that a
 *     permitted power can actually be stored; and
 *
 *   - formatted output must remain parseable, so the same limit has to apply
 *     to what may be written in a specification and to what an operation on
 *     units may produce.  Permitting a larger result than a specification may
 *     express would yield units that cannot be read back.
 *
 * The limit applies to the power of a unit, and only to that.  The scale
 * factor of a Galilean unit is a different quantity and is bounded separately,
 * by whether it is representable as a double.  The two are distinguished at
 * the dispatch in ut_raise(): a numeric factor is raised by galileanRaise(),
 * a unit power by productRaise().  A Galilean unit over a dimensioned base
 * passes through both, and is judged by both.
 *
 * Enforced on the powers resulting from raising or multiplying units, rather
 * than on the exponents written, because composition can drive a power beyond
 * the limit even when every exponent written was inside it.
 */
#define UT_MAX_UNIT_POWER 255

#endif
