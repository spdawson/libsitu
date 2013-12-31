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

SUBDIRS = src demos docs utils
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsitu.pc
dist_doc_DATA = AUTHORS ChangeLog NEWS README COPYING COPYING.LESSER

ACLOCAL_AMFLAGS = -I m4

if HAVE_GIT
if HAVE_GITLOG_TO_CHANGELOG
.PHONY: update-ChangeLog
update-ChangeLog:
	if test -d $(top_srcdir)/.git ; then \
		test -f $(top_srcdir)/.last-cl-gen && \
			REVISION_RANGE=$$(cat $(top_srcdir)/.last-cl-gen) ; \
		test -n "$$REVISION_RANGE" && REVISION_RANGE=$$REVISION_RANGE.. ; \
		gitlog-to-changelog \
			--format='* %s%n%n%b%n' --no-cluster \
			--strip-tab --strip-cherry-pick \
			-- $$REVISION_RANGE \
			>$(top_srcdir)/ChangeLog.tmp ; \
		test -s $(top_srcdir)/ChangeLog.tmp && \
			echo >>$(top_srcdir)/ChangeLog.tmp ; \
		cat $(top_srcdir)/ChangeLog >>$(top_srcdir)/ChangeLog.tmp \
		&& mv -f $(top_srcdir)/ChangeLog.tmp $(top_srcdir)/ChangeLog ; \
		git rev-list -n 1 HEAD >$(top_srcdir)/.last-cl-gen ; \
	fi

.last-cl-gen: update-ChangeLog

EXTRA_DIST = .last-cl-gen
endif
endif
