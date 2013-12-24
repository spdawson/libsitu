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

#include <stdio.h>
#include <stdlib.h>

#include <libsitu.h>

#include <unistd.h>

void alarm(const char *name,
           double distance, libsitu::Event event, void *data)
{
  printf("alarm %s \"%s\" (%fm) [%p]\n",
         libsitu::Util::event_str(event),
         name,
         distance,
         data);
}

int main(int UNUSED(argc), char *UNUSED(argv[]))
{
  const int sleep_us = 500000;
  const int poll_us = 2000000;
  libsitu::Gps gps("localhost", "gpsd", poll_us, sleep_us);

  /* N.B. Maps onto FGW route code 0387 */
  gps.add_watch("NEWBURY", 51.398, -1.323, 500, &alarm, NULL);
  gps.add_watch("NEWBRYR", 51.398, -1.308, 500, &alarm, NULL);
  gps.add_watch("THATCHM", 51.394, -1.243, 500, &alarm, NULL);
  gps.add_watch("MIDGHAM", 51.396, -1.178, 500, &alarm, NULL);
  gps.add_watch("ALDMSTN", 51.402, -1.139, 500, &alarm, NULL);
  gps.add_watch( "THEALE", 51.433, -1.075, 500, &alarm, NULL);
  gps.add_watch("REDGWST", 51.455, -0.990, 500, &alarm, NULL);
  gps.add_watch("RDNGSTN", 51.459, -0.972, 500, &alarm, NULL);

  sleep(10);

  libsitu::Fix fix;
  gps.get_last_fix(fix);
  libsitu::Util::dump_fix_json(fix);

	return EXIT_SUCCESS;
}
