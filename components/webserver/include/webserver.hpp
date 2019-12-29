#ifndef WEBSERVER_HPP__
#define WEBSERVER_HPP__

#include "esp_http_server.h"
#include "esp_err.h"
#include "urihandler.hpp"

#include <vector>

class Webserver
{
public:
  esp_err_t register_handler(UriHandler*);
  
  esp_err_t start();
  esp_err_t stop();

  static Webserver* get_instance();
  static esp_err_t handle(httpd_req_t *);

private:
  Webserver();
  httpd_handle_t server_;
  httpd_config_t config_;
  
  std::vector<UriHandler*> handlers_;

  static Webserver* instance_;
};



#endif