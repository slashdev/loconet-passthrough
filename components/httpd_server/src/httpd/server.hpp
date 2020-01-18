#ifndef WEBSERVER_HPP__
#define WEBSERVER_HPP__

#include <list>
#include "esp_err.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "session.hpp"
#include "request_handler.hpp"
#include "tcpip_adapter.h"

#ifndef HTTPD_SESSION_QUEUE_SIZE
#define HTTPD_SESSION_QUEUE_SIZE 15
#endif

#ifndef HTTD_SERVER_MAX_REQUEST_LENGTH
#define HTTD_SERVER_MAX_REQUEST_LENGTH 512
#endif

namespace httpd
{
  class Server
  {
  public:
    Server();
    Server(uint16_t port);
    ~Server();

    void add(RequestHandler*);
    void remove(RequestHandler*);

    esp_err_t start();
    esp_err_t stop();

    esp_err_t thread();

    esp_err_t processSessions();
    esp_err_t processAccept();

  private:
    sockaddr_in dest_addr_;
    int socket_;
    const char* TAG = "HTTPD";

    std::list<RequestHandler*> handlers_;
    std::list<Session*> sessions_;
  };
}

#endif
