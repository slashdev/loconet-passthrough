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

#include "opcodes.hpp"

namespace loconet
{
	namespace messages
	{

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
			OPCode::eOPCode_t opcode();

		protected:
			/*
			 * Creates an empty message of a certain type.
			 * As this is an abstract class, nobody is allowed to
			 * create a generic message.
			 */
			Message(OPCode::eOPCode_t, size_t);
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
			OPCode::eOPCode_t opcode_;
		};
	}
}

#endif
