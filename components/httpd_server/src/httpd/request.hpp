#ifndef HTTPDMESSAGE_HPP_
#define HTTPDMESSAGE_HPP_

#include <string>
#include <unordered_map>

#include "method.hpp"

namespace httpd
{
  class Request
  {
  public:
    Request(char*);
    ~Request();

    method::eHTTPMethod_t method();
    std::string uri();

    std::string header(std::string);

    std::string body();

    void log();

    static std::string methodToString(method::eHTTPMethod_t);

  protected:
    method::eHTTPMethod_t method_ = method::OTHER;
    std::string uri_;
    std::string body_;

    std::unordered_map<std::string, std::string> headers_;
    char* parseMethod(char*);
    char* parseUri(char*);
    char* parseHeaders(char*);
  };
}

#endif
