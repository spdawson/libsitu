#  Copyright 2013 Simon Dawson
#
#  This file is part of libsitu.
#
#  libsitu is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  libsitu is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with libsitu.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.69])
AC_INIT([libsitu], [0.0.0], [spdawson@gmail.com], [libsitu])
AM_INIT_AUTOMAKE(no-dist-gzip dist-xz)
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER([config.h])

AC_SUBST(PACKAGE_AUTHOR, ["Simon Dawson"])
AC_SUBST(PACKAGE_DESCRIPTION, ["GPS client library"])
AC_SUBST(PACKAGE_HOMEPAGE, ["https://github.com/spdawson/libsitu"])
AC_SUBST(PACKAGE_MONTH, [$(LANG=C date '+%B')])
AC_SUBST(PACKAGE_YEAR, [$(LANG=C date '+%Y')])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Test the C++ compiler.
AC_LANG_CPLUSPLUS

# Checks for libraries.
DEP_MODULES=libgps
PKG_CHECK_MODULES(DEPS, $DEP_MODULES)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)
AC_CHECK_LIB([pthread], [pthread_create],,
  [AC_MSG_ERROR([library 'pthread' is required])]
)

# Checks for header files.
AC_STDC_HEADERS
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h],,
  [AC_MSG_ERROR([required header(s) not found])]
)
AC_CHECK_HEADER(gmp.h,,
  [AC_MSG_ERROR([header 'gmp.h' is required])]
)
AC_CHECK_HEADER(mpfr.h,,
  [AC_MSG_ERROR([header 'mpfr.h' is required])]
)
AC_CHECK_HEADERS([getopt.h],,
  [AC_MSG_ERROR([header 'getopt.h' is required])]
)

AC_CHECK_FUNCS([getopt_long],,
  [AC_MSG_ERROR([library function 'getopt_long' is required])]
)

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([alarm memset strdup strtol])
AC_CHECK_LIB(gmp, __gmpz_init,,
  [AC_MSG_ERROR([library 'gmp' is required])]
)
AC_CHECK_LIB(mpfr,mpfr_init,,
  [AC_MSG_ERROR([library 'mpfr' is required])]
)

AC_CONFIG_FILES([Makefile
                 demos/Makefile
                 src/Makefile
                 utils/Makefile
                 libsitu.pc])
AC_OUTPUT
