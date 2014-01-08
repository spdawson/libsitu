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

#ifndef _LIBSITU_GPSMATH_H_
#define _LIBSITU_GPSMATH_H_

namespace libsitu {

  struct Fix;

  namespace Math {

    typedef enum {
      STATE_UNKNOWN = 0,
      STATE_FAR = 1,
      STATE_NEAR = 2
    } State;

    bool calculate_rms(double x, double y, double &rms);

    double distance(const Fix &fix,
                    double there_lat, double there_lon, double there_rad,
                    State &state);


    bool is_finite(double x);

  }
}

#endif
