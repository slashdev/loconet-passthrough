#ifndef HTTPSESSION_HPP_
#define HTTPSESSION_HPP_

#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "request.hpp"
#include "response.hpp"

#ifndef HTTD_SERVER_MAX_REQUEST_LENGTH
#define HTTD_SERVER_MAX_REQUEST_LENGTH 1024
#endif

namespace httpd
{
  class Session
  {
  public:
    Session(int, sockaddr_in);
    ~Session();

    int socket();

    Request* request();
    Response* response();

    void reply();

    // Returns true if the data on the connection
    // could be read and processed.
    // Request and Response are only set if process() == true.
    bool process();

  private:
    int socket_ = -1;
    sockaddr_in from_ = {};

    Request* request_ = NULL;
    Response* response_ = NULL;

    const char* TAG = "HTTPD Session";
  };
}

#endif
