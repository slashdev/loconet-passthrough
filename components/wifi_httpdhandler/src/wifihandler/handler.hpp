#ifndef WIFIREQUEST_HANDLER_HPP_
#define WIFIREQUEST_HANDLER_HPP_

#include <string>
#include "httpd/request_handler.hpp"
#include "httpd/server.hpp"

namespace httpd
{
  namespace handlers
  {
    namespace wifi
    {
      class Handler : public httpd::RequestHandler
      {
      public:
        Handler(httpd::Server*);
        bool accept(httpd::Request*);
        void handle(httpd::Request*, httpd::Response*);

        void GET_url(std::string);
        std::string GET_url(void);

        void POST_url(std::string);
        std::string POST_url(void);
      private:
        httpd::Server *server_ = NULL;
        std::string get_url_ = "/wifi";
        std::string post_url_ = "/wifi/set";
      };
    }
  }
}

#endif
