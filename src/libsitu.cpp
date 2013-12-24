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
#include <string.h>

#include <gpsdebug.h>
#include <libsitu.h>
#include <gpswatch.h>

namespace libsitu {

  void* poller(void *arg);

  Gps::Gps(const char *host, const char *port, int poll_us, int sleep_us)
    : m_host(strdup(host)),
      m_port(strdup(port)),
      m_poll_us(poll_us),
      m_sleep_us(sleep_us),
      m_poll_thread(),
      m_watch_mutex(),
      m_watches(),
      m_polling(false),
      m_last_fix()
  {
    if (0 != pthread_mutex_init(&m_watch_mutex, NULL)) {
      LIBSITU_WARN("Failed to initialise watch mutex\n");
    }

    start_polling();
  }

  Gps::~Gps()
  {
    stop_polling();

    if (0 != pthread_mutex_destroy(&m_watch_mutex)) {
      LIBSITU_WARN("Failed to destroy watch mutex\n");
    }

    free(m_host);
    m_host = NULL;
    free(m_port);
    m_port = NULL;
  }

  void Gps::add_watch(const char *name,
                      double lat, double lon, double rad, WatchAlarm alarm,
                      void *data)
  {
    const Watch watch(lat, lon, rad, alarm, data);
    lock_watches();
    m_watches[name] = watch;
    unlock_watches();
  }

  void Gps::remove_watch(const char *name)
  {
    lock_watches();
    m_watches.erase(name);
    unlock_watches();
  }

  void Gps::handle_poll_fix(const Fix &fix)
  {
    lock_watches();

    /* \todo FIXME: Possibly dodgy copy */
    m_last_fix = fix;

    handle_fix(fix);

    for (WatchMap::iterator iter = m_watches.begin();
         m_watches.end() != iter; ++iter) {
      iter->second.handle_fix(fix, iter->first.c_str());
    }

    unlock_watches();
  }

  void Gps::handle_poll_timeout()
  {
    handle_timeout();
  }

  const char* Gps::get_host() const
  {
    return m_host;
  }

  const char* Gps::get_port() const
  {
    return m_port;
  }

  int Gps::get_poll_us() const
  {
    return m_poll_us;
  }

  int Gps::get_sleep_us() const
  {
    return m_sleep_us;
  }

  void Gps::get_last_fix(Fix &fix) const
  {
    /* \todo FIXME: Possibly dodgy copy */
    fix = m_last_fix;
  }

  void Gps::handle_fix(const Fix &UNUSED(fix))
  {
  }

  void Gps::handle_timeout() {
  }

  void Gps::lock_watches()
  {
    if (0 != pthread_mutex_lock(&m_watch_mutex)) {
      LIBSITU_WARN("Failed to lock watch mutex\n");
    }
  }

  void Gps::unlock_watches()
  {
    if (0 != pthread_mutex_unlock(&m_watch_mutex)) {
      LIBSITU_WARN("Failed to unlock watch mutex\n");
    }
  }

  void Gps::start_polling()
  {
    if (m_polling) {
      LIBSITU_WARN("Already polling\n");
    } else {
      LIBSITU_DBGV("Starting poller thread\n");
      if (0 != pthread_create(&m_poll_thread, NULL, &poller, this)) {
        LIBSITU_WARN("Failed to start poller thread\n");
      } else {
        m_polling = true;
      }
    }
  }

  void Gps::stop_polling()
  {
    if (!m_polling) {
      LIBSITU_WARN("Not currently polling\n");
    } else {
      void *res = NULL;
      LIBSITU_DBG("Cancelling poller thread\n");
      if (0 != pthread_cancel(m_poll_thread)) {
        LIBSITU_WARN("Failed to cancel poller thread\n");
      }
      LIBSITU_DBGV("Joining poller thread\n");
      if (0 != pthread_join(m_poll_thread, &res)) {
        LIBSITU_WARN("Failed to join poller thread\n");
      }
      if (NULL == res) {
        LIBSITU_WARN("Poller thread returned NULL\n");
      } else {
        if (PTHREAD_CANCELED != res) {
          LIBSITU_WARN("Expected poller thread to have been cancelled\n");
        }
      }

      m_polling = false;
    }
  }

}
