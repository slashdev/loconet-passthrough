#ifndef POSTHANDLER_HPP_
#define POSTHANDLER_HPP_

#include "urihandler.hpp"

class PostHandler : public UriHandler
{
public:
  http_method getHttpMethod();
};


#endif
