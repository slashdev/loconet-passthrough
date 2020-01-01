#ifndef LOCONET_MESSAGES_OPCODES_HPP_
#define LOCONET_MESSAGES_OPCODES_HPP_

#include "message.hpp"

namespace loconet
{
  namespace messages
  {
    // namespace OPCode
    // {

        typedef enum OPCODE : uint8_t
    {
      // 2 byte messages
        BUSY         = 0x81,
        GPOFF        = 0x82,
        GPON         = 0x83,
        IDLE         = 0x85,

      // 4 byte messages
        LOCO_SPD     = 0xA0,
        LOCO_DIRF    = 0xA1,
        LOCO_SND     = 0xA2,
        SW_REQ       = 0xB0,
        SW_REP       = 0xB1,
        INPUT_REP    = 0xB2,
        LONG_ACK     = 0xB4,
        SLOT_STAT1   = 0xB5,
        CONSIST_FUNC = 0xB6,
        UNLINK_SLOTS = 0xB8,
        LINK_SLOTS   = 0xB9,
        MOVE_SLOTS   = 0xBA,
        RQ_SL_DATA   = 0xBB,
        SW_STATE     = 0xBC,
        SW_ACK       = 0xBD,
        LOCO_ADR     = 0xBF,

      // 6 byte messages

      // variable length
        PEER_XFER    = 0xE5,
        RD_SL_DATA   = 0xE7,
        IMM_PACKET   = 0xED,
        WR_SL_DATA   = 0xEF
    } eOPCode_t;




      // const char* to_str(eOPCode);

    // }
  }
}

#endif
