#ifndef LOCONET_INBOUNDHANDLER_HPP_
#define LOCONET_INBOUNDHANDLER_HPP_

#include "message/message.hpp"

namespace loconet
{
	class InboundHandler
	{
	public:
		virtual void handle_message(message::Message*);
	};
}

#endif
