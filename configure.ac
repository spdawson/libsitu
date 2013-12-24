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

AC_INIT([libsitu], [0.0.0], [spdawson@gmail.com], [libsitu])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE(no-dist-gzip dist-bzip2)

AC_SUBST(PACKAGE_AUTHOR, ["Simon Dawson"])
AC_SUBST(PACKAGE_DESCRIPTION, ["GPS client library"])
AC_SUBST(PACKAGE_HOMEPAGE, ["https://github.com/spdawson/libsitu"])
AC_SUBST(PACKAGE_MONTH, [$(LANG=C date '+%B')])
AC_SUBST(PACKAGE_YEAR, [$(LANG=C date '+%Y')])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for libraries.
DEP_MODULES=libgps
PKG_CHECK_MODULES(DEPS, $DEP_MODULES)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

# Checks for header files.
AC_STDC_HEADERS
AC_CHECK_HEADER(gmp.h)
AC_CHECK_HEADER(mpfr.h)

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_LIB(gmp, __gmpz_init)
AC_CHECK_LIB(mpfr,mpfr_init)

AC_CONFIG_FILES([Makefile src/Makefile demos/Makefile utils/Makefile libsitu.pc])
AC_OUTPUT
