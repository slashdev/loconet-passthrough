#ifndef WEBSERVER_HPP__
#define WEBSERVER_HPP__

#include "esp_err.h"

#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include <list>
#include <map>

#include "session.hpp"

#ifndef HTTPD_SESSION_QUEUE_SIZE
#define HTTPD_SESSION_QUEUE_SIZE 5
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

    // void add(UriHandler*);
    // void remove(UriHandler*);

    esp_err_t start();
    esp_err_t stop();

    esp_err_t process_session();
    esp_err_t process_accept();

  private:
    // std::list<UriHandler*> handlers_;
    sockaddr_in dest_addr_;
    int socket_;
    const char* TAG = "HTTPD";



    QueueHandle_t sessionQueue_;
  };
}

#endif
