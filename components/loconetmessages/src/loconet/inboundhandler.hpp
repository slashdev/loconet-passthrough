#ifndef LOCONET_INBOUNDHANDLER_HPP_
#define LOCONET_INBOUNDHANDLER_HPP_

#include "messages/message.hpp"

namespace loconet
{
	class InboundHandler
	{
	public:
		virtual void handle_message(messages::Message*);
	};
}

#endif
