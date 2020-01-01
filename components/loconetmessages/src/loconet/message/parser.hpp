#ifndef LOCONET_MESSAGE_PARSER_HPP_
#define LOCONET_MESSAGE_PARSER_HPP_


namespace loconet
{
  namespace message
  {
    /**
     * Returns a string representation of the known OPCodes
     */
    const char* to_str(opcode_t opc);

    namespace Parser
    {
      /*
       * Creates a message from a byte representation
       * if parsing fails, NULL is returned.
       */
      Message* parse(uint8_t *, size_t);
    }
  }
}




#endif
