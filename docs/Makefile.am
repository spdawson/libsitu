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

if HAVE_DOXYGEN
directory = $(top_srcdir)/docs/man

man_MANS = $(directory)/man3/libsitu.3 $(directory)/man3/situ.3

dist_doc_DATA = libsitu.md situ.md

$(man_MANS): $(dist_doc_DATA)
	$(DOXYGEN) Doxyfile

all-local: $(man_MANS)
clean-local:
	$(RM) -r $(directory)
endif
