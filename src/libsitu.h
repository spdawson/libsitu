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

  /** @brief Event type
   *
   * Enumerates the event types
   */
  typedef enum {
    EVENT_NONE = 0, /**< None */
    EVENT_ARRIVE = 1, /**< Arrival at a watch */
    EVENT_DEPART = 2 /**< Departure from a watch */
  } Event;

  /** @brief Fix data
   *
   * A simple structure to represent fix data
   */
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

  /** @brief Watch alarm
   *
   * A function pointer type for watch callbacks
   */
  typedef void (*WatchAlarm)(const char *name, double distance, Event event,
                             void *data);

  /** @brief Utility functions
   */
  namespace Util {
    /** @brief Get a string representation of an event
     *
     * @param[in] event The event to be stringified
     * @return String representation of event
     */
    const char* event_str(Event event);

    /** @brief Dump fix data
     *
     * Dump fix data to standard output, in JSON format
     *
     * @param[in] fix The fix to be dumped
     */
    void dump_fix_json(const Fix &fix);

    /** @brief Parse string to integer
     *
     * Attempt to parse a string to an integer value
     *
     * @param[in] str The string to be parsed
     * @param[out] val The integer value returned
     * @return Success flag
     */
    bool parse_string_to_integer(const char *str, int *val);
  }

  /** @brief Opaque type used internally to represent a watch */
  class Watch;

  /** @brief GPS interface
   *
   * Main API class, representing a GPS interface
   */
  class Gps {
  public:
    /** @brief Constructor
     *
     * @param[in] host Host name
     * @param[in] port Port designation
     * @param[in] poll_us GPS poll timeout, in microseconds
     * @param[in] sleep_us Inter-poll sleep time, in microseconds
     */
    Gps(const char *host, const char *port, int poll_us, int sleep_us);

    /** @brief Destructor */
    virtual ~Gps();

    /** @brief Add a watch
     *
     * Add a named GPS watch
     *
     * @param[in] name The name of the watch to be added
     * @param[in] lat Latitude of the watch
     * @param[in] lon Longitude of the watch
     * @param[in] rad The watch radius, in meters
     * @param[in] alarm Watch alarm callback function
     * @param[in] data Opaque data to be passed to the watch alarm callback
     */
    void add_watch(const char *name, double lat, double lon, double rad,
                   WatchAlarm alarm, void *data);

    /** @brief Remove a watch
     *
     * Remove a named watch
     *
     * @param[in] name The name of the watch to be removed
     */
    void remove_watch(const char *name);

    /** @brief Get the host name
     *
     * @return The host name
     */
    const char* get_host() const;

    /** @brief Get the port designation
     *
     * @return The port designation
     */
    const char* get_port() const;

    /** @brief Get the GPS poll timeout, in microseconds
     *
     * @return The GPS poll timeout, in microseconds
     */
    int get_poll_us() const;

    /** @brief Get the inter-poll sleep time, in microseconds
     *
     * @return The inter-poll sleep time, in microseconds
     */
    int get_sleep_us() const;

    /** @brief Get the last fix
     *
     * @param[out] fix The fix
     */
    void get_last_fix(Fix &fix) const;

  private:

    Gps(const Gps&);
    Gps& operator=(const Gps&);

    typedef std::map<std::string,Watch> WatchMap;

    /*
     * This needs to be a friend, so that it can call handle_poll_fix() and
     * handle_poll_timeout()
     */
    friend void* poller(void *arg);

    void handle_poll_fix(const Fix &fix);
    void handle_poll_timeout();

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
