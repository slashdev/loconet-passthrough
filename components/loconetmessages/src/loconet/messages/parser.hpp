#ifndef LOCONET_MESSAGE_PARSER_HPP_
#define LOCONET_MESSAGE_PARSER_HPP_

#include "../messages.hpp"

namespace loconet
{
  namespace messages
  {
    /*
     * Creates a message from a byte representation
     * if parsing fails, NULL is returned.
     */
    Message* parse(uint8_t *, size_t);
  }
}




#endif
