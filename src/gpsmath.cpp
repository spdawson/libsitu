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

#include <config.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gps.h>
#ifdef HAVE_LIBMPFR
#include <mpfr.h>
#endif /* HAVE_LIBMPFR */

#include <gpsdebug.h>
#include <libsitu.h>
#include <gpsmath.h>

/* Number of precision bits to be used in GNU MPFR floating-point
 * calculations.
 *
 * When near a waypoint we are dealing with small angles (10^-5 of a
 * degree or less) and distances (metres or less, in calculations
 * related to an earthradius), and we require to perform a trig
 * calculation which may vary only in the ninth significant digit and
 * then take its acos.
 *
 * AVR32 floating point emulation is limited in precision (or perhaps just
 * buggy), and doesn't support sinl/cosl for long doubles, so we can't use
 * native libm for distance/time calculations as floats aren't precise
 * enough and we get a lot of NaN answers because of the rounding errors.
 *
 * We will instead use the 3rd party GMPR library which gives slightly more
 * consistent cross-platform results (within the limits of precision),
 * and also increase the significand's precision from default of 53 bits
 * (long double) to 64 bits so AVR32's results should be comparable with
 * i386 FPU.
 */
#define LIBSITU_PRECISION_BITS 64

#define LIBSITU_EARTH_RADIUS_m 6378137.0

namespace libsitu {
  namespace Math {

    bool is_finite(double x)
    {
      return isfinite(x);
    }

    double deg2rad(double d)
    /* Convert from degrees to radians */
    {
      static const double pi_over_180 = acos(-1.0) / 180.0;
      return d * pi_over_180;
    }

    bool calculate_rms(double x, double y, double &rms)
    {
      /* N.B. Calculate the RMS horizontal positional error. */
#ifdef HAVE_LIBMPFR
      MPFR_DECL_INIT(epx, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(epy, LIBSITU_PRECISION_BITS);
      mpfr_set_ld(epx, x, GMP_RNDN);
      mpfr_set_ld(epy, y, GMP_RNDN);
      MPFR_DECL_INIT(hypotenuse, LIBSITU_PRECISION_BITS);
      mpfr_hypot(hypotenuse, epx, epy, GMP_RNDN);
      if (mpfr_nan_p(hypotenuse)) {
        LIBSITU_WARN("Hypotenuse is NaN\n");
        return false;
      }

      rms = mpfr_get_d(hypotenuse, GMP_RNDN);
      return true;
#else /* HAVE_LIBMPFR */
      rms = hypot(x, y);
      return true;
#endif /* HAVE_LIBMPFR */
    }

    double distance(
      const Fix &fix,
      double there_lat,
      double there_lon,
      double there_rad,
      State &state
    )
    /* Calculate the distance from here to there
     *
     * See http://www.movable-type.co.uk/scripts/latlong.html for algorithm
     */
    {
      const double here_lat = fix.latitude;
      const double here_lon = fix.longitude;

#ifdef HAVE_LIBMPFR
      /* Get eastings and northings in radian measure */
      MPFR_DECL_INIT(here_e, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(here_n, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(there_e, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(there_n, LIBSITU_PRECISION_BITS);
      mpfr_set_ld(here_e, deg2rad(here_lon), GMP_RNDN);
      mpfr_set_ld(here_n, deg2rad(90.0 - here_lat), GMP_RNDN);
      mpfr_set_ld(there_e, deg2rad(there_lon), GMP_RNDN);
      mpfr_set_ld(there_n, deg2rad(90.0 - there_lat), GMP_RNDN);

      MPFR_DECL_INIT(diff_e, LIBSITU_PRECISION_BITS);
      mpfr_sub(diff_e, here_e, there_e, GMP_RNDN);

      MPFR_DECL_INIT(A, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(B, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(C, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(D, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(E, LIBSITU_PRECISION_BITS);

      mpfr_sin(A, here_n, GMP_RNDN);
      mpfr_sin(B, there_n, GMP_RNDN);
      mpfr_cos(C, here_n, GMP_RNDN);
      mpfr_cos(D, there_n, GMP_RNDN);
      mpfr_cos(E, diff_e, GMP_RNDN);

      MPFR_DECL_INIT(AA, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(BB, LIBSITU_PRECISION_BITS);
      MPFR_DECL_INIT(CC, LIBSITU_PRECISION_BITS);

      mpfr_mul(AA, A, B, GMP_RNDN);
      mpfr_mul(BB, C, D, GMP_RNDN);
      mpfr_mul(BB, BB, E, GMP_RNDN);
      mpfr_add(CC, AA, BB, GMP_RNDN);

      MPFR_DECL_INIT(angular_delta, LIBSITU_PRECISION_BITS);
      mpfr_acos(angular_delta, CC, GMP_RNDN);

      /* Convert to a great circle distance in meters */
      MPFR_DECL_INIT(earth_radius_m, LIBSITU_PRECISION_BITS);
      mpfr_set_ld(earth_radius_m, LIBSITU_EARTH_RADIUS_m, GMP_RNDN); /* Equatorial Earth radius. */

      /* return angular_delta * earth_radius_m; */
      MPFR_DECL_INIT(res, LIBSITU_PRECISION_BITS);
      mpfr_mul(res, angular_delta, earth_radius_m, GMP_RNDN);

      if (mpfr_nan_p(res)) {
        LIBSITU_DBGV("Distance is NAN:\n");
      }

#ifdef LIBSITU_DEBUG_MATH
      LIBSITU_DBG("here_e: %.12g, here_n: %.12g, there_e: %.12g, there_n: %.12g, diff_e: %.12g\n",
                  mpfr_get_d(here_e, GMP_RNDN),
                  mpfr_get_d(here_n, GMP_RNDN),
                  mpfr_get_d(there_e, GMP_RNDN),
                  mpfr_get_d(there_n, GMP_RNDN),
                  mpfr_get_d(diff_e, GMP_RNDN));
      LIBSITU_DBG("A: %.12g, B: %.12g, C: %.12g, D: %.12g, E: %.12g\n",
                  mpfr_get_d(A, GMP_RNDN),
                  mpfr_get_d(B, GMP_RNDN),
                  mpfr_get_d(C, GMP_RNDN),
                  mpfr_get_d(D, GMP_RNDN),
                  mpfr_get_d(E, GMP_RNDN));
      LIBSITU_DBG("AA: %.12g, BB: %.12g\n",
                  mpfr_get_d(AA, GMP_RNDN), mpfr_get_d(BB, GMP_RNDN));
      LIBSITU_DBG("CC: %.12g, angular_delta: %.12g, result: %.12g\n",
                  mpfr_get_d(CC, GMP_RNDN),
                  mpfr_get_d(angular_delta, GMP_RNDN),
                  mpfr_get_d(res, GMP_RNDN));

#endif /* LIBSITU_DEBUG_MATH */

      const double distance_val = mpfr_get_d(res, GMP_RNDN);
#else /* HAVE_LIBMPFR */
      const double here_e = deg2rad(here_lon);
      const double here_n = deg2rad(90.0 - here_lat);
      const double there_e = deg2rad(there_lon);
      const double there_n = deg2rad(90.0 - there_lat);
      const double diff_e = here_e - there_e;
      const double A = sin(here_n);
      const double B = sin(there_n);
      const double C = cos(here_n);
      const double D = cos(there_n);
      const double E = cos(diff_e);
      const double AA = A * B;
      double BB = C * D;
      double BB = BB * E;
      const double CC = AA * BB;
      const double angular_delta = acos(CC);
      const double earth_radius_m = LIBSITU_EARTH_RADIUS_m;
      const double res = angular_delta * earth_radius_m;

      const double distance_val = res;
#endif /* HAVE_LIBMPFR */
      const double distance_abs_value = fabs(distance_val);

      /* Allow for up to 3 standard deviations of error */
      const double here_eph = fix.eph;
      double error_radius = 3 * here_eph;
      if (error_radius >= there_rad) {
        /* It is possible that an unrealistically low watch radius has
         * been set for the waypoint.
         *
         * Equally, the position might be known to a low precision.
         *
         * Anyway, we need to avert disaster here. */
        LIBSITU_WARN("Error radius is %f, but watch radius is only %f\n",
                     error_radius, there_rad);
        /* \todo FIXME: Can we do this in a less arbitrary manner? */
        error_radius = 0.2 * there_rad;
      }

      state =
        isnan(distance_abs_value) || isnan(error_radius) ? STATE_UNKNOWN :
        distance_abs_value + error_radius <= there_rad ? STATE_NEAR :
        distance_abs_value - error_radius > there_rad ? STATE_FAR :
        /* Failing that, we're not certain */
        STATE_UNKNOWN;

      return distance_val;
    }

  }
}
