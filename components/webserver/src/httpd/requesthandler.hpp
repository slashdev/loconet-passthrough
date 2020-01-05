#ifndef REQUESTHANDLER_HPP_
#define REQUESTHANDLER_HPP_

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
