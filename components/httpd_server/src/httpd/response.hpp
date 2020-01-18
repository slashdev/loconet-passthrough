#ifndef HTTPD_RESPONSE_HPP_
#define HTTPD_RESPONSE_HPP_

#include <unordered_map>
#include "status.hpp"

namespace httpd
{
  class Response
  {
  public:
    Response();

    // Set the status
    void status(status::eStatus_t);

    // Get the status
    status::eStatus_t status();

    // Set a header
    void header(std::string, std::string);

    // Get a header
    std::string header(std::string);

    // Set the body
    void body(const char*);
    void body(const char*, size_t);
    void body(std::string);

    // Get the body
    std::string body();
    std::string message();

    void constructDefaultMessage(std::string);

    static const char* responsePhrase(status::eStatus_t);

  private:
    status::eStatus_t status_ = status::NOT_FOUND;
    std::string body_;
    std::unordered_map<std::string, std::string> headers_;
  };

}

#endif
