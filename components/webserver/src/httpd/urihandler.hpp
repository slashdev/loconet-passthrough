#ifndef URIHANDLER_HPP_
#define URIHANDLER_HPP_

#include "request.hpp"
#include "response.hpp"

namespace httpd
{

  class URIHandler
  {
  public:
    virtual bool accept(Request*);
    virtual void handle(Request*, Response*);
  };
}

#endif
