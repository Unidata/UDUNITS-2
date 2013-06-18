/////////////////////////////////////////////////////////////////////////// 
/* 
  Copyright 2001 Ronald S. Burkey 
 
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
 
  Filename:     winglob.h 
  Purpose:      Header file for a Win32 scaled-down equivalent of glob.h. 
  Mods:         12/01/01 RSB    Began. 
 
  These versions of glob/globfree are "scaled-down" in the sense that 
  while they accept parameters for the error-function and the flags, 
  those parameters are not used for anything.  Actually, it always 
  treats them as 
        flags = GLOB_NOSORT 
        &errfunc = NULL 
*/

/////////////////////////////////////////////////////////////////////////// 
#ifdef _WIN32
#ifndef _INCLUDED_WINGLOB_H
#define _INCLUDED_WINGLOB_H

//--------------------------------------------------------------------- 
// Constants. 
#define GLOB_ERR 0
#define GLOB_MARK 0
#define GLOB_NOSORT 0
#define GLOB_DOOFFS 0
#define GLOB_NOCHECK 0
#define GLOB_APPEND 0
#define GLOB_NOESCAPE 0
#define GLOB_PERIOD 0
#define GLOB_ALTDIRFUNC 0
#define GLOB_BRACE 0
#define GLOB_NOMAGIC 0
#define GLOB_TILDE 0
#define GLOB_ONLYDIR
#define GLOB_NOSPACE 1
#define GLOB_ABORTED 2
#define GLOB_NOMATCH 4
#define GLOB_MAGCHAR 8

//--------------------------------------------------------------------- 
// Type defs 
typedef struct
{
  int gl_pathc;
  char **gl_pathv;
  int gl_offs;
  int MaxGlobs;
}
glob_t;

//--------------------------------------------------------------------- 
// Function prototypes. 
int glob (const char *pattern, int flags,
	  int errfunc (const char *epath, int eerrno), glob_t * pglob);
void globfree (glob_t * pglob);
#endif // _INCLUDED_WINGLOB_H
#endif // _WIN32
