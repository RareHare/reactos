/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <msvcrti.h>


long
atol(const char *str)
{
  return strtol(str, 0, 10);
}
