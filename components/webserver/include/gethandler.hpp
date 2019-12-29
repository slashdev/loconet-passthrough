#ifndef GETHANDLER_HPP_
#define GETHANDLER_HPP_

#include "urihandler.hpp"

class GetHandler : public UriHandler
{
public:
  http_method getHttpMethod();
};


#endif
