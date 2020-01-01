#ifndef STRINGHANDLER_HPP_
#define STRINGHANDLER_HPP_

#include "GetHandler.hpp"

class StringHandler : public GetHandler
{
public:
  StringHandler(const char*);
  StringHandler(const char*, const char*);
  const char * getUri();
  esp_err_t handle(httpd_req_t *);

  const char * getContent();

private:
  const char *TAG = "StringHandler";
  const char *uri_;
  const char *text_;
};


#endif