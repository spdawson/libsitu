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

#ifndef _LIBSITU_CLIENT_H_
#define _LIBSITU_CLIENT_H_

#include <libsitu.h>

namespace libsitu {

class Client : public Gps {
public:
  Client(const char *host, const char *port, int timeout_s, bool oneshot);
  virtual ~Client();

  bool fix_found() const;

private:
  Client(const Client&);
  Client& operator=(const Client&);

  virtual void handle_fix(const Fix &fix);
  virtual void handle_timeout();

  bool m_oneshot;
  bool m_fix_found;
};

}

#endif
