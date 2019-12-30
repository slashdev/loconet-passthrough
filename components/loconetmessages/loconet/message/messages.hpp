#ifndef LOCONET_MESSAGE_MESSAGES_HPP_
#define LOCONET_MESSAGE_MESSAGES_HPP_


#include "message.hpp"

// all 2 byte messages
#include "busy.hpp"

// all 4 byte messages
#include "inputreport.hpp"

// all n-byte messages


namespace loconet
{
	namespace message
	{

		/*
		 * Creates a message from a byte representation
		 * if parsing fails, NULL is returned.
		 */
		Message* parse(uint8_t *, size_t);
	}
}

#endif
