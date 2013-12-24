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
#include <time.h>

#include <unistd.h>

#include <gpsdebug.h>
#include <libsitu.h>

class Client : public libsitu::Gps {
public:
  Client(const char *host, const char *port, int timeout_s, bool oneshot)
    : libsitu::Gps(host, port, timeout_s * 1000000, 500000),
      m_oneshot(oneshot),
      m_fix_found(false)
  {}
  virtual ~Client() {}

  bool fix_found() const { return m_fix_found; }

private:
  Client(const Client&);
  Client& operator=(const Client&);

  virtual void handle_fix(const libsitu::Fix &fix) {
    if (fix.valid) {
      m_fix_found = true;
      libsitu::Util::dump_fix_json(fix);
      if (m_oneshot) {
        exit(EXIT_SUCCESS);
      }
    }
  }
  virtual void handle_timeout() {
    fprintf(stderr, "Timeout\n");
    exit(EXIT_FAILURE);
  }

  bool m_oneshot;
  bool m_fix_found;
};

void print_help(
  const char *program_name
)
{
  printf("Usage: %s [OPTION]...\n"
         "\n"
         "Output GPS fix data\n"
         "\n"
         "Options:\n"
         "  -h, --help                 Print usage information\n"
         "  -l, --loop                 Run in a loop, outputting fix information\n"
         "  -t, --timeout <timeout_s>  Timeout waiting for GPS data after the\n"
         "                             specified number of seconds\n"
         "  --host <host>              Connect to gpsd on specified host\n"
         "  -p, --port <port>          Connect to gpsd on specified port\n",
         program_name);
}

int main(int argc, char *argv[])
{
  int exit_code = EXIT_SUCCESS;

  /* Process command line */
  bool loop = false;
  int timeout_s = 5;
  const char *host = "localhost";
  const char *port = "gpsd";
  {
    const char *program_name = argv[0];
    int i = 0;
    for (i = 1; i < argc; ++i) {
      const char *arg_i = argv[i];
      if (NULL != arg_i && '\0' != *arg_i) {
        if (0 == strcmp("-h", arg_i) || 0 == strcmp("--help", arg_i)) {
          print_help(program_name);
          exit_code = EXIT_SUCCESS;
          return exit_code;
        } else if (0 == strcmp("-l", arg_i) || 0 == strcmp("--loop", arg_i)) {
          loop = true;
        } else if (0 == strcmp("-t", arg_i) ||
                   0 == strcmp("--timeout", arg_i)) {
          if (argc > ++i) {
            if (!libsitu::Util::parse_string_to_integer(argv[i], &timeout_s)) {
              fprintf(stderr, "Failed to parse --timeout option value\n");
              exit_code = EXIT_FAILURE;
              return exit_code;
            }
          } else {
            fprintf(stderr, "No argument given for --timeout option\n");
            exit_code = EXIT_FAILURE;
            return exit_code;
          }
        } else if (0 == strcmp("--host", arg_i)) {
          if (argc > ++i) {
            host = argv[i];
            if (NULL == host || '\0' == *host) {
              fprintf(stderr, "Empty argument given for --host option\n");
              exit_code = EXIT_FAILURE;
              return exit_code;
            }
          } else {
            fprintf(stderr, "No argument given for --host option\n");
            exit_code = EXIT_FAILURE;
            return exit_code;
          }
        } else if (0 == strcmp("-p", arg_i) || 0 == strcmp("--port", arg_i)) {
          if (argc > ++i) {
            port = argv[i];
            if (NULL == port || '\0' == *port) {
              fprintf(stderr, "Empty argument given for --port option\n");
              exit_code = EXIT_FAILURE;
              return exit_code;
            }
          } else {
            fprintf(stderr, "No argument given for --port option\n");
            exit_code = EXIT_FAILURE;
            return exit_code;
          }
        }
      }
    }
  }

  Client client(host, port, timeout_s, !loop);

  /* N.B. Sleep here for longer than the timeout, to keep the main thread
     alive */
  {
    struct timespec req;
    req.tv_sec = 2 * timeout_s;
    req.tv_nsec = 0;
    if (0 != nanosleep(&req, NULL)) {
      perror("nanosleep");
    }
  }

  if (!client.fix_found()) {
    exit_code = EXIT_FAILURE;
  }

  return exit_code;
}
