#ifndef WIFIREQUEST_HANDLER_HPP_
#define WIFIREQUEST_HANDLER_HPP_


#include "httpd/requesthandler.hpp"

class WifiRequestHandler : public httpd::RequestHandler
{
public:
  bool accept(httpd::Request*);
  void handle(httpd::Request*, httpd::Response*);
};

#endif
