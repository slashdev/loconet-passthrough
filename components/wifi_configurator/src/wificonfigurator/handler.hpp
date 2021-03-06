#ifndef WIFIREQUEST_HANDLER_HPP_
#define WIFIREQUEST_HANDLER_HPP_

#include <string>
#include "httpd/request_handler.hpp"

namespace wificonfigurator
{
  class Handler : public httpd::RequestHandler
  {
  public:
    bool accept(httpd::Request*);
    void handle(httpd::Request*, httpd::Response*);

    void GET_url(std::string);
    std::string GET_url(void);

    void POST_url(std::string);
    std::string POST_url(void);

  private:
    std::string get_url_ = "/wifi";
    std::string post_url_ = "/wifi/set";
  };
}

#endif
