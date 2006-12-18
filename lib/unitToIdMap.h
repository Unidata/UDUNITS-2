#ifndef UT_UNIT_TO_ID_MAP_H_INCLUDED
#define UT_UNIT_TO_ID_MAP_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Frees resources associated with a unit-system.
 *
 * Arguments:
 *	system		Pointer to the unit-system to have its associated
 *			resources freed.
 */
void
utimFreeSystem(
    utSystem*	system);


#ifdef __cplusplus
}
#endif

#endif
