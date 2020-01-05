#include "util.hpp"

namespace loconet
{
  namespace messages
  {
    namespace util
    {
      const char* to_string(OPCode::eOPCode_t opc)
      {
        switch(opc)
        {
          case OPCode::BUSY:
            return "BUSY";
          case OPCode::GPOFF:
            return "GPOFF";
          case OPCode::GPON:
            return "GPON";
          case OPCode::IDLE:
            return "IDLE";
          case OPCode::LOCO_SPD:
            return "LOCO_SPD";
          case OPCode::LOCO_DIRF:
            return "LOCO_DIRF";
          case OPCode::LOCO_SND:
            return "LOCO_SND";
          case OPCode::SW_REQ:
            return "SW_REQ";
          case OPCode::SW_REP:
            return "SW_REP";
          case OPCode::INPUT_REP:
            return "INPUT_REP";
          case OPCode::LONG_ACK:
            return "LONG_ACK";
          case OPCode::SLOT_STAT1:
            return "SLOT_STAT1";
          case OPCode::CONSIST_FUNC:
            return "CONSIST_FUNC";
          case OPCode::UNLINK_SLOTS:
            return "UNLINK_SLOTS";
          case OPCode::LINK_SLOTS:
            return "LINK_SLOTS";
          case OPCode::MOVE_SLOTS:
            return "MOVE_SLOTS";
          case OPCode::RQ_SL_DATA:
            return "RQ_SL_DATA";
          case OPCode::SW_STATE:
            return "SW_STATE";
          case OPCode::SW_ACK:
            return "SW_ACK";
          case OPCode::LOCO_ADR:
            return "LOCO_ADR";
          case OPCode::PEER_XFER:
            return "PEER_XFER";
          case OPCode::RD_SL_DATA:
            return "RD_SL_DATA";
          case OPCode::IMM_PACKET:
            return "IMM_PACKET";
          case OPCode::WR_SL_DATA:
            return "WR_SL_DATA";
        }
        return "";
      }

      uint8_t calculate_checksum(uint8_t *data, size_t length)
      {
        // calculate the checksum: XOR each byte with checksum
        uint8_t checksum = 0xFF;
        for(size_t i = 0 ; i < length ; i++)
        {
          checksum ^= data[i];
        }
        return checksum;
      }
    }
  }
}
