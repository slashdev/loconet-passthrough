#ifndef URIHANDLER_HPP_
#define URIHANDLER_HPP_

#include "esp_http_server.h"
#include "http_parser.h"
#include "esp_err.h"

#include <string>
#include <unordered_map>

class UriHandler
{
public:
  virtual const char * getUri();
  virtual esp_err_t handle(httpd_req_t *);
  virtual http_method getHttpMethod();
protected:
  std::unordered_map<std::string, std::string> parse_query_string(std::string query);
};

#endif