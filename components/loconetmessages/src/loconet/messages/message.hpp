#ifndef LOCONET_MESSAGE_MESSAGEBASE_HPP_
#define LOCONET_MESSAGE_MESSAGEBASE_HPP_

/**
 * @brief Base for Loconet messages
 *
 * \copyright Copyright 2019 /Dev. All rights reserved.
 * \license This project is released under MIT license.
 *
 * This file contains the processing of sending loconet messages.
 *
 * @author Jan Martijn van der Werf <janmartijn@slashdev.nl>
 *
 * For the types of messages, see:
 * See: http:=www.digitrax.com/static/apps/cms/media/documents/loconet/loconetpersonaledition.pdf
 */

// Required for all the types being used
#include "esp_system.h"

namespace loconet
{
	namespace messages
	{

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
  	} opcode_t;

		class Message
		{
		public:

			~Message();

			/*
			 * Returns the byte representation of the message
			 * can be directly used to send out
			 */
			uint8_t *bytes();

			/*
			 * returns the length of the byte representation
			 */
			size_t length();

			/*
			 * returns the current checksum of a message.
			 * If not yet set, it is calculated.
			 */
			uint8_t checksum();

			/*
			 * returns the calculated checksum
			 */
			uint8_t calculate_checksum();

			/*
			 * Returns the opcode of a message
			 */
			opcode_t opcode();

		protected:
			/*
			 * Creates an empty message of a certain type.
			 * As this is an abstract class, nobody is allowed to
			 * create a generic message.
			 */
			Message(opcode_t, size_t);
			/*
			 * The complete internal representation of the message.
			 * The first byte is the opcode, the last is the checksum
			 */
			uint8_t *data_;

			/*
			 * The length of the byte representation, i.e., the length of *data
			 */
			size_t length_ = 2;

		// private:
			opcode_t opcode_;
		};
	}
}

#endif
