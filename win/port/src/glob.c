/////////////////////////////////////////////////////////////////////////// 
/* 
  Copyright 2001-2 Ronald S. Burkey 
 
  This file is part of GutenMark. 
 
  GutenMark is free software; you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation; either version 2 of the License, or 
  (at your option) any later version. 
 
  GutenMark is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
  GNU General Public License for more details. 
 
  You should have received a copy of the GNU General Public License 
  along with GutenMark; if not, write to the Free Software 
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 
  Filename:     winglob.c 
  Purpose:      Win32 scaled-down equivalent of filename globbing. 
                Intended for mingw32, but should work with Borland C. 
  Mods:         12/01/01 RSB    Began. 
  		07/14/02 RSB	Found out that _findXXXX removes the
				directory names, reporting just the
				stripped-down filenames.  This meant
				that globbing worked only from within
				the directory where the files actually
				are stored.  I've fixed this to 
				restore the directory names that had
				been removed.
 
  These versions of glob/globfree are "scaled-down" in the sense that 
  while they accept parameters for the error-function and the flags, 
  those parameters are not used for anything.  Actually, it always 
  treats them as 
        flags = GLOB_NOSORT 
        &errfunc = NULL 
*/

/////////////////////////////////////////////////////////////////////////// 
#ifdef _WIN32
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <glob.h>

// If new space has to be allocated to hold the list of glob pointers, 
// this is the number of new filenames that will be added. 
#define GLOB_QUANTUM 256

//--------------------------------------------------------------------- 
// We have to use the functions _findfirst and _findnext, which are 
// kind of mysterious (not exactly like the Borland functions with the 
// same names), but are described adequately in mingw32's io.h file. 
int
glob (const char *pattern, int flags,
      int errfunc (const char *epath, int eerrno), glob_t * pglob)
{
  struct _finddata_t fb;
  char **New;
  int i;
  const char *ss;
  int DirLength;

  // Figure out which  part of the input path corresponds to the 
  // directory, and which to the filename pattern.
  for (DirLength = 0, ss = pattern; *ss; ss++)
    if (*ss == ':' || *ss == '/' || *ss == '\\')
      DirLength = ss - pattern + 1;

  // Make sure there's some space for globbing. 
  if (pglob == NULL)
    return (GLOB_NOSPACE);
  
  pglob->gl_pathv = (char **) calloc (GLOB_QUANTUM, sizeof (char *));
  if (pglob->gl_pathv == NULL)
    return (GLOB_NOSPACE);
  pglob->MaxGlobs = GLOB_QUANTUM;
  pglob->gl_pathc = 0;
  pglob->gl_offs = 0;

  // Now try to fetch the filenames. 
  for (i = _findfirst (pattern, &fb); i != -1; i = _findnext (i, &fb))
    {
      if (pglob->gl_pathc >= pglob->MaxGlobs)
	{
	  New =
	    (char **) realloc (pglob->gl_pathv,
			       (pglob->MaxGlobs +
				GLOB_QUANTUM) * sizeof (char *));
	  if (New == NULL)
	    return (GLOB_NOSPACE);
	  pglob->gl_pathv = New;
	  pglob->MaxGlobs += GLOB_QUANTUM;
	}
      pglob->gl_pathv[pglob->gl_pathc] =
	(char *) calloc (1, DirLength + 1 + strlen (fb.name));
      if (pglob->gl_pathv[pglob->gl_pathc] == NULL)
	return (GLOB_NOSPACE);
      // _findXXXX apparently returns only the filename, without
      // the leading drive letter and/or directory.  We need to 
      // restore that part.
      strncpy (pglob->gl_pathv[pglob->gl_pathc], pattern, DirLength);
      strcpy (DirLength + pglob->gl_pathv[pglob->gl_pathc], fb.name);
      pglob->gl_pathc++;
    }

  // All done! 
  return (0);
}

//--------------------------------------------------------------------- 
void
globfree (glob_t * pglob)
{
  int i;
  if (pglob == NULL)
    return;
  if (pglob->gl_pathv == NULL)
    return;
  for (i = 0; i < pglob->MaxGlobs; i++)
    if (pglob->gl_pathv[i] != NULL)
      {
	free (pglob->gl_pathv[i]);
	pglob->gl_pathv[i] = NULL;
      }
  free (pglob->gl_pathv);
  pglob->gl_pathv = NULL;
  pglob->MaxGlobs = 0;
  pglob->gl_pathc = 0;
  pglob->gl_offs = 0;
}
#endif // _WIN32
