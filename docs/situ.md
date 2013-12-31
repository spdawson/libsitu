Command line client for libsitu {#situ}
===============================

SYNOPSIS
========

  **situ** [_OPTION_]...

DESCRIPTION
===========

Output GPS fix data gathered by libsitu. By default, a single fix is output.

  -l, --loop

    Run in a loop, outputting fix information

  -t, --timeout=TIMEOUT

    Timeout waiting for GPS data after the specified number of seconds

  -x, --host=HOST

    Connect to gpsd on specified host

  -p, --port=PORT

    Connect to gpsd on specified port

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
