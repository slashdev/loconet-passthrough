#ifndef HTTPDMETHOD_HPP_
#define HTTPDMETHOD_HPP_

namespace httpd
{
  namespace method
  {
    typedef enum HTTP_METHOD {
      GET,
      HEAD,
      POST,
      PUT,
      DELETE,
      CONNECT,
      OPTIONS,
      TRACE,
      OTHER
    } eHTTPMethod_t;
  }
}

#endif
