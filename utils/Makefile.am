#  Copyright 2013-2014 Simon Dawson
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

bin_PROGRAMS = situ

situ_SOURCES = situ.cpp client.cpp client.h
situ_CPPFLAGS = -I$(top_srcdir)/src
situ_CXXFLAGS = -Wall -Wextra -Weffc++
situ_LDADD = -L$(top_builddir)/src -lsitu $(DEPS_LIBS) -lpthread
