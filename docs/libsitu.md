GPS client library {#libsitu}
==================

SYNOPSIS
========

    #include <libsitu.h>

    void alarm(const char *name,
               double distance, libsitu::Event event, void *data) { ... }

    libsitu::Gps gps("localhost", "gpsd", 2000000, 500000);
    gps.add_watch("HOME", 52.6835, -1.82653, 500, &alarm, NULL);
    libsitu::Fix fix;
    gps.get_last_fix(fix);
    libsitu::Util::dump_fix_json(fix);

DESCRIPTION
===========

NOT YET WRITTEN!

AUTHOR
======

Written by Simon Dawson.

COPYRIGHT
=========

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
