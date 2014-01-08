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

#include <client.h>

namespace libsitu {

  Client::Client(const char *host, const char *port, int timeout_s,
                 bool oneshot)
    : Gps(host, port, timeout_s * 1000000, 500000),
      m_oneshot(oneshot),
      m_fix_found(false)
  {
  }

  Client::~Client() {
  }

  bool Client::fix_found() const
  {
    return m_fix_found;
  }

  void Client::handle_fix(const Fix &fix)
  {
    if (fix.valid) {
      m_fix_found = true;
      Util::dump_fix_json(fix);
      if (m_oneshot) {
        exit(EXIT_SUCCESS);
      }
    }
  }

  void Client::handle_timeout()
  {
    fprintf(stderr, "Timeout\n");
    exit(EXIT_FAILURE);
  }

}
