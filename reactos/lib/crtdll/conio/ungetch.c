/*
 * COPYRIGHT:   See COPYING in the top level directory
 * 		Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details 
 * PROJECT:     ReactOS system libraries
 * FILE:        lib/crtdll/conio/ungetch.c
 * PURPOSE:     Ungets a character from stdin
 * PROGRAMER:   DJ Delorie
		Boudewijn Dekker [ Adapted from djgpp libc ]
 * UPDATE HISTORY:
 *              28/12/98: Created
 */

#include <process.h>
#define EOF -1

int char_avail = 0;
char ungot_char = 0;


int
ungetch(int c)
{
  if (char_avail)
    return(EOF);
  ungot_char = c;
  char_avail = 1;
  return(c);
}
