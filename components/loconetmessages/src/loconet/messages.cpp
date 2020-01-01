#include "messages.hpp"

namespace loconet
{
  namespace messages
  {
    const char* to_str(opcode_t opc)
    {
      switch(opc)
      {
        case BUSY:
          return "BUSY";
        case GPOFF:
          return "GPOFF";
        case GPON:
          return "GPON";
        case IDLE:
          return "IDLE";
        case LOCO_SPD:
          return "LOCO_SPD";
        case LOCO_DIRF:
          return "LOCO_DIRF";
        case LOCO_SND:
          return "LOCO_SND";
        case SW_REQ:
          return "SW_REQ";
        case SW_REP:
          return "SW_REP";
        case INPUT_REP:
          return "INPUT_REP";
        case LONG_ACK:
          return "LONG_ACK";
        case SLOT_STAT1:
          return "SLOT_STAT1";
        case CONSIST_FUNC:
          return "CONSIST_FUNC";
        case UNLINK_SLOTS:
          return "UNLINK_SLOTS";
        case LINK_SLOTS:
          return "LINK_SLOTS";
        case MOVE_SLOTS:
          return "MOVE_SLOTS";
        case RQ_SL_DATA:
          return "RQ_SL_DATA";
        case SW_STATE:
          return "SW_STATE";
        case SW_ACK:
          return "SW_ACK";
        case LOCO_ADR:
          return "LOCO_ADR";
        case PEER_XFER:
          return "PEER_XFER";
        case RD_SL_DATA:
          return "RD_SL_DATA";
        case IMM_PACKET:
          return "IMM_PACKET";
        case WR_SL_DATA:
          return "WR_SL_DATA";
      }
      return "";
    }
  }
}
