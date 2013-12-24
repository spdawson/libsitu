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

#ifndef _LIBSITU_H_
#define _LIBSITU_H_

#include <map>
#include <string>

#include <pthread.h>

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

namespace libsitu {

  typedef enum {
    EVENT_NONE = 0,
    EVENT_ARRIVE = 1,
    EVENT_DEPART = 2
  } Event;

  struct Fix {
    bool valid;

    double latitude;
    double longitude;
    double eph;

    bool has_speed;
    double speed;
    double eps;

    bool has_track;
    double track;

    unsigned satellites_used;
  };

  typedef void (*WatchAlarm)(const char *name, double distance, Event event,
                             void *data);

  namespace Util {
    const char* event_str(Event event);
    void dump_fix_json(const Fix &fix);
    bool parse_string_to_integer(const char *str, int *val);
  }

  class Watch;

  class Gps {
  public:
    Gps(const char *host, const char *port, int poll_us, int sleep_us);
    virtual ~Gps();
    void add_watch(const char *name, double lat, double lon, double rad,
                   WatchAlarm alarm, void *data);
    void remove_watch(const char *name);

    void handle_poll_fix(const Fix &fix);
    void handle_poll_timeout();

    const char* get_host() const;
    const char* get_port() const;
    int get_poll_us() const;
    int get_sleep_us() const;

    void get_last_fix(Fix &fix) const;
  private:

    Gps(const Gps&);
    const Gps& operator=(const Gps&);

    typedef std::map<std::string,Watch> WatchMap;

    virtual void handle_fix(const Fix &fix);
    virtual void handle_timeout();

    void lock_watches();
    void unlock_watches();

    void start_polling();
    void stop_polling();

    char *m_host;
    char *m_port;
    int m_poll_us;
    int m_sleep_us;

    pthread_t m_poll_thread;
    pthread_mutex_t m_watch_mutex;
    WatchMap m_watches;

    bool m_polling;

    Fix m_last_fix;
  };

}

#endif
