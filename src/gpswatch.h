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

#ifndef _LIBSITU_GPSWATCH_H_
#define _LIBSITU_GPSWATCH_H_

/* N.B. for definition of WatchAlarm */
#include <libsitu.h>

/* N.B. for definition of State */
#include <gpsmath.h>

namespace libsitu {

  class Fix;

  class Watch {
  public:
    Watch();
    Watch(double lat, double lon, double rad, WatchAlarm alarm, void *data);
    ~Watch();
    Watch(const Watch &original);
    Watch& operator=(const Watch &rhs);
    void handle_fix(const Fix &fix, const char *name);
  private:
    double m_lat;
    double m_lon;
    double m_rad;
    WatchAlarm m_alarm;
    void *m_data;
    Math::State m_state;
  };

}

#endif
