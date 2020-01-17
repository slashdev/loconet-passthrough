#ifndef REQUEST_HANDLER_HPP_
#define REQUEST_HANDLER_HPP_

#include "request.hpp"
#include "response.hpp"

namespace httpd
{

  class RequestHandler
  {
  public:
    virtual bool accept(Request*);
    virtual void handle(Request*, Response*);
  };
}

#endif
