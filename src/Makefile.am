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

lib_LTLIBRARIES = libsitu.la
include_HEADERS = libsitu.h

libsitu_la_SOURCES = libsitu.h libsitu.cpp gpswatch.h gpswatch.cpp gpsdebug.h gpsmath.h gpsmath.cpp gpsutil.cpp gpspoller.cpp
libsitu_la_CPPFLAGS = -I. $(DEPS_CFLAGS)
libsitu_la_CXXFLAGS = -Wall -Wextra -Weffc++
libsitu_la_LIBADD = $(DEPS_LIBS) -lpthread
