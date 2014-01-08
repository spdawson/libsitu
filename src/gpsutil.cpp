/*
  Copyright 2013-2014 Simon Dawson

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

#include <errno.h>
#include <limits.h> /* LONG_MIN and LONG_MAX */
#include <stdio.h>
#include <stdlib.h>

#include <gpsdebug.h>
#include <libsitu.h>
#include <gpsmath.h>

namespace libsitu {
  namespace Util {

    const char* event_str(Event event)
    {
      return EVENT_ARRIVE == event ? "APPROACH" :
        EVENT_DEPART == event ? "DEPART" :
        "INVALID";
    }

    void dump_fix_json(const Fix &fix)
    {
      printf("{\n");

      if (Math::is_finite(fix.latitude) && Math::is_finite(fix.longitude)) {
        printf("  \"latitude\": %.3f,\n", fix.latitude);
        printf("  \"longitude\": %.3f,\n", fix.longitude);
        printf("  \"eph\": %.3f,\n", fix.eph);
      } else {
        printf("  \"latitude\": null,\n");
        printf("  \"longitude\": null,\n");
        printf("  \"eph\": null,\n");
      }
      if (fix.has_speed) {
        printf("  \"speed\": %.3f,\n", fix.speed);
        printf("  \"eps\": %.3f,\n", fix.eps);
      } else {
        printf("  \"speed\": null,\n");
        printf("  \"eps\": null,\n");
      }
      if (fix.has_track) {
        printf("  \"track\": %.3f,\n", fix.track);
      } else {
        printf("  \"track\": null,\n");
      }
      printf("  \"satellites_used\": %d\n", fix.satellites_used);

      printf("}\n");
    }

    bool parse_string_to_integer(
      const char *str,
      int *val
    )
    /* Parse the specified string to an integer value

       Returns true on success
    */
    {
      bool retval = false;

      if (NULL == str || NULL == val) {
        LIBSITU_WARN("Invalid input to parse_string_to_integer()\n");
        retval = false;
      } else {
        char *endptr = NULL;
        errno = 0; /* To distinguish success/failure after call */
        const long parsed_val = strtol(str, &endptr, 10);

        /* Check for various possible errors */
        if ((ERANGE == errno &&
             (LONG_MAX == parsed_val || LONG_MIN == parsed_val)) ||
            (0 != errno && 0 == parsed_val)) {
          perror("strtol");
          retval = false;
        } else if (endptr == str) {
          LIBSITU_WARN("No digits were found\n");
          retval = false;
        } else if ('\0' != *endptr) {
          /* Not necessarily an error... */
          LIBSITU_WARN("Further characters after number: %s\n", endptr);
          retval = false;
        } else {
          /* If we got here, strtol() successfully parsed a number */
          LIBSITU_DBGV("strtol() returned %ld\n", parsed_val);

          *val = parsed_val;
          retval = true;
        }
      }

      return retval;
    }

  }
}
