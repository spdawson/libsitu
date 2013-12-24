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

#include <math.h>

#include <gpsdebug.h>
#include <gpswatch.h>

namespace libsitu {

  Watch::Watch()
    : m_lat(0), m_lon(0), m_rad(0), m_alarm(NULL), m_data(NULL),
      m_state(Math::STATE_UNKNOWN)
  {
  }

  Watch::Watch(double lat, double lon, double rad,
               WatchAlarm alarm, void *data)
    : m_lat(lat), m_lon(lon), m_rad(rad), m_alarm(alarm), m_data(data),
      m_state(Math::STATE_UNKNOWN)
  {
  }

  Watch::~Watch()
  {
  }

  Watch::Watch(const Watch &original)
    : m_lat(original.m_lat),
      m_lon(original.m_lon),
      m_rad(original.m_rad),
      m_alarm(original.m_alarm),
      m_data(original.m_data),
      m_state(original.m_state)
  {
  }

  const Watch& Watch::operator=(const Watch &rhs)
  {
    if (this != &rhs) {
      m_lat = rhs.m_lat;
      m_lon = rhs.m_lon;
      m_rad = rhs.m_rad;
      m_alarm = rhs.m_alarm;
      m_data = rhs.m_data;
      m_state = rhs.m_state;
    }

    return *this;
  }

  void Watch::handle_fix(const Fix &fix, const char *name)
  {
    Math::State state = Math::STATE_UNKNOWN;
    const double distance =
      fabs(Math::distance(fix, m_lat, m_lon, m_rad, state));

    if (state != m_state) {
      if (NULL != m_alarm) {
        /* Figure out the event type for the alarm */
        Event event = EVENT_NONE;
        switch (state) {
        case Math::STATE_UNKNOWN:
          /* FAR -> UNKNOWN: No event (transition through error zone) */
          /* NEAR -> UNKNOWN: No event (transition through error zone) */
          break;
        case Math::STATE_FAR:
          /* UNKNOWN -> FAR: No event (initially found FAR) */
          /* NEAR -> FAR: DEPART */
          if (Math::STATE_NEAR == m_state) {
            event = EVENT_DEPART;
          }
          break;
        case Math::STATE_NEAR:
          /* UNKNOWN -> NEAR: ARRIVE (initially found NEAR) */
          /* FAR -> NEAR: ARRIVE */
          event = EVENT_ARRIVE;
          break;
        default:
          LIBSITU_WARN("Invalid state\n");
          break;
        }
        if (EVENT_NONE != event) {
          (*m_alarm)(name, distance, event, m_data);
        }
      }

      /* N.B. Don't record the unknown state */
      if (Math::STATE_UNKNOWN != state) {
        m_state = state;
      }
    }
  }

}
