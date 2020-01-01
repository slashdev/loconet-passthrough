#include "urihandler.hpp"

#define TOKEN_SPLIT "&"
#define VARIABLE_SPLIT "="

std::unordered_map<std::string, std::string> UriHandler::parse_query_string(std::string query)
{
  std::unordered_map<std::string, std::string> vars;

  printf("I parse string: '%s'\n", query.c_str());

  size_t found = query.find(TOKEN_SPLIT);
  size_t prev = 0;
  //printf("I have found: %d and prev: %d", found, prev);
    
  while( found <= std::string::npos)
  {
    
    size_t split = query.find(VARIABLE_SPLIT, prev);
    if (split != std::string::npos)
    {
      std::string key = query.substr(prev, split-prev);
      std::string val = query.substr(split+1, found-split-1);
      vars.insert({key, val});
    }

    if (found == std::string::npos) break;

    prev = found+1;
    found = query.find(TOKEN_SPLIT, prev);
  }
  
  return vars;
}