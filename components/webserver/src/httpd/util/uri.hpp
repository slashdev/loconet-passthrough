#ifndef HTTPD_UTIL_URI_HPP_
#define HTTPD_UTIL_URI_HPP_ 1

#include <string>
#include <unordered_map>

namespace httpd
{
  namespace uri
  {
    std::unordered_map<std::string, std::string> parse_query_string(std::string);
    std::string decode(std::string);
  }
}


#endif
