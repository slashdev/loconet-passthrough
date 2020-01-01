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

class WebServer
{
public:
  WebServer();
  WebServer(uint16_t port);
  ~WebServer();

  // void add(UriHandler*);
  // void remove(UriHandler*);

  esp_err_t start();
  esp_err_t stop();

  void thread();

private:
  // std::list<UriHandler*> handlers_;
  sockaddr_in dest_addr_;
  int socket_;
  const char* TAG = "HTTPD";

	esp_err_t accept_process();

  HttpSession* active_session_ = NULL;
	// std::map<int, HttpSession*> sessions_;
};



#endif
