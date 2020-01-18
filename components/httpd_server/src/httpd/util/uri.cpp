#include "uri.hpp"

#include <cstdio>

#define HTTPD_UTIL_URI_TOKEN_SPLIT "&"
#define HTTPD_UTIL_URI_VARIABLE_SPLIT "="
#define HTTPD_UTIL_URI_AMP

namespace httpd
{
  namespace uri
  {
    std::string decode(std::string str)
    {
      std::string ret;
      for (int i = 0; i < str.length(); i++)
      {
          if(str[i] != '%')
          {
            if(str[i] == '+') ret += ' ';
            else ret += str[i];
          }
          else
          {
            int code = 0;
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &code);
            char ch = static_cast<char>(code);
            ret += ch;
            i = i + 2;
          }
      }

      return ret;
    }

    std::unordered_map<std::string, std::string> parse_query_string(std::string query)
    {
      std::unordered_map<std::string, std::string> vars;

      size_t found = query.find(HTTPD_UTIL_URI_TOKEN_SPLIT);
      size_t prev = 0;

      while( found <= std::string::npos)
      {
        size_t split = query.find(HTTPD_UTIL_URI_VARIABLE_SPLIT, prev);
        if (split != std::string::npos)
        {
          std::string key = query.substr(prev, split-prev);
          std::string val = query.substr(split+1, found-split-1);
          vars.insert({decode(key), decode(val)});
        }

        if (found == std::string::npos) break;

        prev = found+1;
        found = query.find(HTTPD_UTIL_URI_TOKEN_SPLIT, prev);
      }

      return vars;
    }
  }
}
