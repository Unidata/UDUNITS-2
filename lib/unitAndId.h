#ifndef UT_UNIT_SEARCH_NODE_H_INCLUDED
#define UT_UNIT_SEARCH_NODE_H_INCLUDED

#include "units.h"

typedef struct {
    char*	id;
    utUnit*	unit;
} UnitAndId;

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Arguments:
 *	id	The identifier (name or symbol).  May be freed upon return.
 *	unit	The unit.  Must not be freed upon successful return until the
 *		returned unit-search-node is no longer needed.
 * Returns:
 *	NULL	"id" is NULL.
 *	NULL	"node" is NULL.
 *	NULL	Out of memory.
 *	else	Pointer to the new unit search node.
 */
UnitAndId*
uaiNew(
    const utUnit* const	unit,
    const char* const	id);


void
uaiFree(
    UnitAndId* const	node);


#ifdef __cplusplus
}
#endif

#endif
