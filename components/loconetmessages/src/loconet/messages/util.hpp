#ifndef LOCONET_MESSAGES_UTIL_HPP_
#define LOCONET_MESSAGES_UTIL_HPP_
#endif

// Weird: if I include opcodes.hpp, I get error messages...
#include "message.hpp"

namespace loconet
{
  namespace messages
  {
    namespace util
    {
      const char* to_string(OPCode::eOPCode_t);

      uint8_t calculate_checksum(uint8_t *, size_t);
    }
  }
}
