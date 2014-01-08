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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <errno.h>

#include <gps.h>
#include <libgpsmm.h>

#include <gpsdebug.h>
#include <libsitu.h>
#include <gpsmath.h>

namespace libsitu {

  bool parse_raw_gps_data(
    const gps_data_t *gps_data,
    Fix &data
  )
  /* Parse the specified raw GPS data, and populate the specified data
   * object
   *
   * Returns true on success
   */
  {
    /* Posit data not valid, until known otherwise. */
    memset(&data, 0, sizeof(data));

    if (NULL == gps_data) {
      LIBSITU_WARN("Null GPS data pointer\n");
    } else {
      if (gps_data->set & PACKET_SET) {

        LIBSITU_DBGV("parse_raw_gps_data(), mask=%04llx\n", gps_data->set);

        if (gps_data->set & STATUS_SET) {
          switch (gps_data->status) {
          case STATUS_NO_FIX:
            break;
          case STATUS_FIX:
            /* Run into next case. */
          case STATUS_DGPS_FIX:
            if (gps_data->set & MODE_SET) {
              switch (gps_data->fix.mode) {
              case MODE_NOT_SEEN:
                LIBSITU_DBG("gpsd reports: mode update not seen yet\n");
                break;
              case MODE_NO_FIX:
                LIBSITU_DBG("gpsd reports: no fix\n");
                break;
              case MODE_2D:
                /* Run into next case. */
              case MODE_3D:
                LIBSITU_DBGV("gpsd reports: data valid, mode=%d\n",
                             gps_data->fix.mode);
                if ((gps_data->set & LATLON_SET) &&
                    (gps_data->set & HERR_SET)) {
                  {
                    LIBSITU_DBGV("  mode=%d\n", gps_data->fix.mode);
                    LIBSITU_DBGV("  lat=%f, long=%f\n",
                                 gps_data->fix.latitude, gps_data->fix.longitude);

                    /* N.B. Calculate the RMS horizontal positional error. */
                    double eph = 0;
                    if (!Math::calculate_rms(gps_data->fix.epx,
                                             gps_data->fix.epy,
                                             eph)) {
                      LIBSITU_WARN("Failed to calculate RMS horizontal position error\n");
                    } else {
                      data.latitude = gps_data->fix.latitude;
                      data.longitude = gps_data->fix.longitude;
                      data.eph = eph;
                      data.satellites_used = gps_data->satellites_used;
                      if ((gps_data->set & SPEED_SET) &&
                          (gps_data->set & SPEEDERR_SET)) {
                        data.has_speed = true;
                        data.speed = gps_data->fix.speed;
                        data.eps = gps_data->fix.eps;
                      } else {
                        LIBSITU_DBGV("GPS data missing required speed field(s)\n");
                      }
                      if (gps_data->set & TRACK_SET) {
                        data.has_track = true;
                        data.track = gps_data->fix.track;
                      } else {
                        LIBSITU_DBGV("GPS data missing required track field(s)\n");
                      }

                      data.valid = Math::is_finite(data.latitude) &&
                        Math::is_finite(data.longitude);
                    }
                  }
                } else {
                  LIBSITU_DBGV("GPS data missing required position field\n");
                }
                break;
              default:
                LIBSITU_WARN("Unrecognised GPSD mode\n");
                break;
              }
            } else {
              LIBSITU_WARN("GPS data missing mode field\n");
            }
            break;
          default:
            LIBSITU_WARN("Unrecognised GPSd status\n");
            break;
          }
        } else {
          LIBSITU_DBGV("GPS data missing status field\n");
        }
      } else {
        LIBSITU_DBG("No data since last read\n");
      }
    }

    return data.valid;
  }

  void* poller(void *arg)
  {
    if (NULL == arg) {
      LIBSITU_WARN("Null poll data\n");
    } else {
      Gps *context = static_cast<Gps*>(arg);
      if (NULL == context->get_host() || NULL == context->get_port()) {
        LIBSITU_WARN("Host and/or port unknown\n");
      } else {
        LIBSITU_DBG("Opening interface %s:%s\n",
                    context->get_host(), context->get_port());
        gpsmm gps_interface(context->get_host(), context->get_port());
        if (NULL == gps_interface.stream(WATCH_ENABLE|WATCH_JSON)) {
          LIBSITU_WARN("Failed to start GPS stream: %d, %s\n",
                       errno, gps_errstr(errno));
        } else {

          LIBSITU_DBG("Entering GPS poll loop (%dus)\n",
                      context->get_poll_us());
          while (true) {
            if (gps_interface.waiting(context->get_poll_us())) {
              const struct gps_data_t *gps_data = gps_interface.read();
              if (NULL == gps_data) {
                LIBSITU_WARN("Null GPS data from interface: %d, %s\n",
                             errno, gps_errstr(errno));
              } else {
                Fix fix;
                if (parse_raw_gps_data(gps_data, fix)) {
                  /* Call back */
                  LIBSITU_DBGV("Calling back...\n");
                  context->handle_poll_fix(fix);
                } else {
                  LIBSITU_DBGV("Failed to parse raw GPS data\n");
                }
              }
            } else {
              LIBSITU_DBGV("Timeout\n");
              context->handle_poll_timeout();
            }

            {
              if (0 != pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)) {
                LIBSITU_WARN("Failed to set thread cancel state\n");
              }
              /* N.B. We can use sleep() here without the parenthetical
               * disabling of thread cancellation.
               *
               * However, the disable is required in order to use usleep or
               * nanosleep. If the thread is cancelled while we are in a
               * usleep or nanosleep call, we get a segfault.
               *
               * This seems odd, because all of sleep, usleep and nanosleep
               * are meant to be thread cancellation points.
               *
               * Anyway, it doesn't matter: just do the parenthetical thread
               * cancellation disable, and all is well.
               */
              {
                struct timespec req;
                req.tv_sec = context->get_sleep_us() / 1000000;
                req.tv_nsec = 1000 * (context->get_sleep_us() % 1000000);
                if (0 != nanosleep(&req, NULL)) {
                  perror("nanosleep");
                }
              }

              if (0 != pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
                LIBSITU_WARN("Failed to set thread cancel state\n");
              }
            }
          }

        }
      }
    }

    return NULL;
  }

}
