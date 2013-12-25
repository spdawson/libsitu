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

#include <getopt.h>
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
         "  -x,--host <host>           Connect to gpsd on specified host\n"
         "  -p, --port <port>          Connect to gpsd on specified port\n",
         program_name);
}

int main(int argc, char *argv[])
{
  int exit_code = EXIT_SUCCESS;

  /* Process command line */
  const char *program_name = argv[0];
  bool loop = false;
  int timeout_s = 5;
  const char *host = "localhost";
  const char *port = "gpsd";
  while (true) {
    int option_index = 0;
    const static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"loop", no_argument, 0, 'l'},
      {"timeout", required_argument, 0, 't'},
      {"host", required_argument, 0, 'x'}, /* N.B. Cannot use 'h' */
      {"port", required_argument, 0, 'p'}
    };
    const int c = getopt_long(argc, argv, "hlt:x:p:",
                              long_options, &option_index);
    if (-1 == c) {
      /* All options parsed */
      break;
    }
    switch (c) {
    case 'h':
      print_help(program_name);
      exit_code = EXIT_SUCCESS;
      return exit_code;
    case 'l':
      loop = true;
      break;
    case 't':
      if (!libsitu::Util::parse_string_to_integer(optarg, &timeout_s)) {
        fprintf(stderr, "Failed to parse --timeout option value\n");
        exit_code = EXIT_FAILURE;
        return exit_code;
      }
      break;
    case 'x':
      /* Host option */
      host = optarg;
      break;
    case 'p':
      port = optarg;
      break;
    case '?':
      /* Unexpected option parsed */
      exit_code = EXIT_FAILURE;
      return exit_code;
    default:
      break;
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
