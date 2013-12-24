/*
  Copyright 2013 Simon Dawson

  This file is part of libsitu.

  libsitu is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  libsitu is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with libsitu.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _LIBSITU_DEBUG_H_
#define _LIBSITU_DEBUG_H_

#include <stdio.h>

/* #define DEBUG */
#ifdef DEBUG
/*#  define LIBSITU_DBG_VERBOSE */
#  define LIBSITU_DBG(...) fprintf(stdout, "DEBUG: " __VA_ARGS__)
#  ifdef LIBSITU_DBG_VERBOSE
#    define LIBSITU_DBGV(...) LIBSITU_DBG(__VA_ARGS__)
#  else /* LIBSITU_DBG_VERBOSE */
#    define LIBSITU_DBGV(...)
#  endif /* LIBSITU_DBG_VERBOSE */
#else /* DEBUG */
#  define LIBSITU_DBG(...)
#  define LIBSITU_DBGV(...)
#endif /* DEBUG */

#define LIBSITU_WARN(...) fprintf(stderr, "WARNING: " __VA_ARGS__)

#endif
